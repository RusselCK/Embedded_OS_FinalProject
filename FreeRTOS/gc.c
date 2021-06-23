#include <stdlib.h>
#include <treadmill/gc.h>
#include <treadmill/_dbg.h>

static inline void
unsnap(TmHeap *heap, TmCell* self) {
  TmCell *my_prev = self->prev;
  TmCell *my_next = self->next;
  self->next = NULL;
  self->prev = NULL;

  if(BOTTOM == self) BOTTOM = my_next;
  if(TOP    == self) TOP    = my_next;
  if(SCAN   == self) SCAN   = my_next;
  if(FREE   == self) FREE   = my_next;

  my_prev->next = my_next;
  my_next->prev = my_prev;
}

static inline void
insert_in(TmHeap *heap, TmCell* self, TmCell* him) {
  if(self == him) return; // we do nothing

  unsnap(heap, self);

  TmCell *his_prev = him->prev;

  his_prev->next = self;
  him->prev      = self;

  self->prev  = his_prev;
  self->next  = him;

  if(him == TOP)    TOP = self;
  if(him == BOTTOM) BOTTOM = self;
  if(him == SCAN)   SCAN = self;
  if(him == FREE)   FREE = self;
}

//static inline void
void
make_ecru(TmHeap *heap, TmCell *self)
{
  if(self == BOTTOM) {
    if (self == TOP)  TOP  = self->next;
    if (self == SCAN) SCAN = self->next;
    if (self == FREE) FREE = self->next;
  } else {
    insert_in(heap, self, BOTTOM);
  }
  self->ecru = 1;
}

//static inline void
void
make_grey(TmHeap *heap, TmCell *self)
{
  insert_in(heap, self, TOP);
  self->ecru = 0;

  if (self == SCAN) {
    SCAN = self->next;
  }
  if (self == FREE) {
    FREE = self->next;
  }
}


TmHeap*
TmHeap_new(
  TmStateHeader* state,
  int size,
  int growth_rate,
  int scan_every,
  size_t object_size,
  TmReleaseFn release_fn,
  TmScanPointersFn scan_pointers_fn)
{
  TmHeap *heap = calloc(1, sizeof(TmHeap));

  heap->state  = state;
  heap->chunks = Tm_DArray_create(sizeof(TmCell*), 100);

  TmChunk chunk = TmChunk_new(size + 1);
  TmCell *head = chunk.head;
  TmCell *tail = chunk.tail;

  // Save a reference to the chunk to deallocate it later
  Tm_DArray_push(heap->chunks, head);

  heap->growth_rate   = growth_rate;
  heap->release       = release_fn;
  heap->scan_pointers = scan_pointers_fn;
  heap->object_size   = object_size;
  heap->allocs        = 0;
  heap->scan_every    = scan_every;

  FREE   = head;
  BOTTOM = head;
  TOP    = head;
  SCAN   = head;

  // Close the circle.
  tail->next = head;
  head->prev = tail;

  return heap;
}

void
TmHeap_print(TmHeap *heap)
{
  portDISABLE_INTERRUPTS();
  fprintf(stderr,
    "[HEAP] (%i) (WHITE %i | ECRU %i | GREY %i | BLACK %i)\n\r",
    (int)TmHeap_size(heap),
    (int)TmHeap_white_size(heap),
    (int)TmHeap_ecru_size(heap),
    (int)TmHeap_grey_size(heap),
    (int)TmHeap_black_size(heap)
    );
  portENABLE_INTERRUPTS();
}

void
TmHeap_print_all(TmHeap *heap)
{
  portDISABLE_INTERRUPTS();

  TmHeap_print(heap);

  TmCell *ptr = TOP;
  do {
    fprintf(stderr,"* %p", ptr);
    if(ptr == TOP) fprintf(stderr, " (TOP)");
    if(ptr == BOTTOM) fprintf(stderr," (BOTTOM)");
    if(ptr == FREE) fprintf(stderr, " (FREE)");
    if(ptr == SCAN) fprintf(stderr, " (SCAN)");
    fprintf(stderr, "\n\r");
  } while((ptr = ptr->next) && ptr != TOP);
  fprintf(stderr, "[END HEAP]\n\r");
  portENABLE_INTERRUPTS();
}

void
TmHeap_grow(TmHeap *heap, int size)
{
  if(size < 1) return;

  TmChunk chunk = TmChunk_new(size);
  TmCell *head = chunk.head;
  TmCell *tail = chunk.tail;

  // Save a reference to the chunk to deallocate it later
  Tm_DArray_push(heap->chunks, head);

  // Put the new chunk before the current free.
  TmCell *oldfree  = FREE;
  TmCell *previous = oldfree->prev;

  // Attach tail
  oldfree->prev = tail;
  tail->next    = oldfree;

  // Attach head
  previous->next = head;
  head->prev     = previous;

  if(BOTTOM == FREE) BOTTOM = head;
  if(TOP    == FREE) TOP = head;
  if(SCAN   == FREE) SCAN = head;
  FREE = head;
}

static inline int
TmHeap_distance_between(TmCell *a, TmCell *b)
{
  int count = 1;
  TmCell *ptr = a;
  while((ptr = ptr->next) && ptr != b) count++;

  return count;
}


double
TmHeap_size(TmHeap *heap)
{
  return TmHeap_distance_between(TOP, TOP);
}

double
TmHeap_white_size(TmHeap *heap)
{
  if(FREE == BOTTOM &&
     FREE == TOP &&
     FREE == SCAN) {
    return TmHeap_size(heap);
  }

  return TmHeap_distance_between(FREE, BOTTOM);
}

double
TmHeap_ecru_size(TmHeap *heap)
{
  if(BOTTOM == TOP) return 0;
  return TmHeap_distance_between(BOTTOM, TOP);
}

double
TmHeap_grey_size(TmHeap *heap)
{
  if(TOP == SCAN) return 0;
  return TmHeap_distance_between(TOP, SCAN);
}

double
TmHeap_black_size(TmHeap *heap)
{
  if(SCAN == FREE) return 0;
  return TmHeap_distance_between(SCAN, FREE);
}

static inline Tm_DArray*
null_rootset(TmStateHeader *state)
{
  return Tm_DArray_create(sizeof(TmObjectHeader*), 1);
}

void
TmHeap_destroy(TmHeap* heap)
{
  debug("[GC] Destroying the heap");

  // Ignore the rootset and flip twice to clean up
  heap->state->rootset_fn = null_rootset;
  Tm_flip(heap);
  Tm_flip(heap);

  TmCell *ptr = NULL;

  // free Objects
  ITERATE(BOTTOM, FREE, ptr) {
    // RELEASE() will free a Object
	RELEASE(ptr->value);
    ptr = ptr->next;
  }

  for(int i=0; i < Tm_DArray_count(heap->chunks); i++) {
    TmCell *chunk = (TmCell*)Tm_DArray_at(heap->chunks, i);
    free(chunk);
  }

  Tm_DArray_destroy(heap->chunks);

  free(heap);
}

TmChunk
TmChunk_new(int size)
{
  TmCell *memory = calloc(size, sizeof(TmCell));
  check_mem(memory);
  TmCell *ptr    = memory;

  for(int i=0; i < size; i++) {
    if(i>0) {
      TmCell *prev = ptr; prev--;
      ptr->prev = prev;
    }
    if(i<size) {
      TmCell *next = ptr; next++;
      ptr->next = next;
    }
    ptr++;
  }

  TmCell *tail = --ptr;

  memory->prev = NULL;
  tail->next   = NULL;

  TmChunk chunk = { .head = memory, .tail = tail };
  return chunk;

error:
  exit(EXIT_FAILURE);

}

static inline void
make_grey_if_ecru(TmHeap *heap, TmObjectHeader *o)
{
  TmCell *cell = o->cell;
  if(cell->ecru) {
    // Unsnap the cell from the ecru area,
    // and put it in the gray area.
    make_grey(heap, cell);
  }
}

void
Tm_scan(TmHeap *heap)
{
  debug("[GC] Scan");
  // If scan == top, the collection has finished
  if(SCAN == TOP) return;

  // Move the scan pointer backwards, converting the scanned grey cell into a
  // black cell.
  SCAN = SCAN->prev;
  heap->scan_pointers(heap, SCAN->value, make_grey_if_ecru);
  TmHeap_print(heap);
}

void
Tm_flip(TmHeap *heap)
{
  debug("[GC] Flip");
  TmHeap_print_all(heap);

  // Scan all the grey cells before flipping.
  debug("[GC] Flip - 1. Scan all grey");
  while(SCAN != TOP) {
	  Tm_scan(heap);
  }
  TmHeap_print_all(heap);

  // Make all the ecru into white and release them
  debug("[GC] Flip - 2. Make all ecru into white");
  TmCell *ptr = NULL;
  ITERATE(BOTTOM, TOP, ptr) {
    ptr->ecru = 0;
    RELEASE(ptr->value);
    ptr = ptr->next;
  }
  BOTTOM = TOP;
  TmHeap_print_all(heap);

  /*
   * If there are still no slots in the white list,
   * grow the heap.
   */
  if(FREE->next == BOTTOM) {
	  debug("[GC] Flip - 3. Grow the heap");
	  TmHeap_grow(heap, heap->growth_rate);
	  TmHeap_print_all(heap);
  }

  // Make all black into ecru.
  debug("[GC] Flip - 4. Make all black into ecru");
  ITERATE(SCAN, FREE, ptr) {
    TmCell *next = ptr->next;
    make_ecru(heap, ptr);
    ptr = next;
  }
  TmHeap_print_all(heap);

  // Add all the rootset into the grey set.
  debug("[GC] Flip - 5. Make all roots into grey");
  Tm_DArray *rootset = heap->state->rootset_fn(heap->state);

  int count = Tm_DArray_count(rootset);
  debug("[GC] Adding rootset (%i)", count);
  for(int i=0; i < count; i++) {
    TmObjectHeader *o = (TmObjectHeader*)(rootset->contents[i]);
    TmCell *cell = o->cell;
    make_grey(heap, cell);
  }

  Tm_DArray_destroy(rootset);
  TmHeap_print_all(heap);
}

TmObjectHeader*
Tm_allocate(TmHeap *heap)
{
  if(heap->allocs >= heap->scan_every) {
    heap->allocs = 0;
    Tm_scan(heap);
  }

  /*
   * If there are no slots in the white list,
   * force a collection.
   */
  if(FREE->next == BOTTOM) {
    Tm_flip(heap);
    check(FREE != BOTTOM, "Heap full.");
  }

  TmObjectHeader *header = calloc(1, heap->object_size);
  check(header, "Out of memory.");

  TmCell *free = FREE;
  header->cell = free;
  header->cell->value = header;

  FREE = FREE->next;

  heap->allocs++;

  return header;
error:
  exit(EXIT_FAILURE);
  return NULL;
}
