#ifndef _treadmill_gc_h
#define _treadmill_gc_h

#include <treadmill/darray.h>
#include "FreeRTOS.h"

#define BOTTOM  heap->bottom
#define TOP     heap->top
#define SCAN    heap->scan
#define FREE    heap->free
#define RELEASE heap->release

#define ITERATE(A, B, N) \
  (N) = (A);             \
  while((N) != (B))

/*
 * -(bottom)- ECRU -(top)- GREY -(scan)- BLACK -(free)- WHITE ...
 */

struct tm_cell_s;

typedef struct tm_cell_s {
  struct tm_cell_s *next;
  struct tm_cell_s *prev;
  void *value;
  char ecru;
} TmCell;

typedef struct tm_object_header_s {
  TmCell *cell;
} TmObjectHeader;

struct tm_state_header_s;
typedef Tm_DArray* (*TmRootsetFn)(struct tm_state_header_s *state);

typedef struct tm_state_header_s {
  TmRootsetFn rootset_fn;
} TmStateHeader;

struct tm_heap_s;
typedef void (*TmReleaseFn)(void *value);
typedef void (*TmCallbackFn)(struct tm_heap_s *state, TmObjectHeader *object);
typedef void (*TmScanPointersFn)(struct tm_heap_s *state, TmObjectHeader *object, TmCallbackFn callback);

typedef struct tm_heap_s {
  TmCell *bottom;
  TmCell *top;
  TmCell *free;
  TmCell *scan;
  int growth_rate;
  int allocs;
  int scan_every;
  size_t object_size;
  TmReleaseFn release;
  TmScanPointersFn scan_pointers;
  TmStateHeader *state;
  Tm_DArray *chunks;
} TmHeap;

typedef struct tm_chunk_s {
  TmCell *head;
  TmCell *tail;
} TmChunk;

TmHeap* TmHeap_new(TmStateHeader *state, int size, int growth_rate, int scan_every, size_t object_size, TmReleaseFn release_fn, TmScanPointersFn scan_pointers_fn);
void TmHeap_grow(TmHeap *heap, int size);

TmObjectHeader* Tm_allocate(TmHeap *heap);
void Tm_scan(TmHeap *heap);
void Tm_flip(TmHeap *heap);

void make_ecru(TmHeap *heap, TmCell *self);
void make_grey(TmHeap *heap, TmCell *self);

void TmHeap_print(TmHeap *heap);
void TmHeap_print_all(TmHeap *heap);
double TmHeap_size(TmHeap *heap);
double TmHeap_white_size(TmHeap *heap);
double TmHeap_ecru_size(TmHeap *heap);
double TmHeap_grey_size(TmHeap *heap);
double TmHeap_black_size(TmHeap *heap);

void TmHeap_destroy(TmHeap* heap);

TmChunk TmChunk_new(int size);

#endif
