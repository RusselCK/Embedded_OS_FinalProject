#include <treadmill/gc.h>
#include <treadmill/gc_test_setup.h>
#include <treadmill/state.h>
#include <treadmill/object.h>
#include "string.h"

Tm_DArray*
test_rootset(TmStateHeader *state_h)
{
  Tm_DArray *rootset = Tm_DArray_create(sizeof(TmObjectHeader*), 10);
  State *state = (State*)state_h;
  for(int i=0; i<Tm_DArray_count(state->rootset);i++) {
    Tm_DArray_push(rootset, state->rootset->contents[i]);
  }

  return rootset;
}

void
test_scan_pointers(TmHeap *heap, TmObjectHeader *object, TmCallbackFn callback)
{
  Object *self = (Object*)object;
  for(int i=0; i < Tm_DArray_count(self->children); i++) {
//    TmObjectHeader *o = (TmObjectHeader*)Tm_DArray_at(self->children, i);
	TmObjectHeader *o = (TmObjectHeader*)(self->children->contents[i]);
    callback(heap, o);
  }
}

void
test_release(void *value)
{
  Object_destroy((Object*)value);
}

State*
State_new()
{
  State *state = calloc(1, sizeof(State));
  state->gc.rootset_fn = test_rootset;
  state->rootset = Tm_DArray_create(sizeof(Object*), 10);
  return state;
}

TmHeap*
new_heap(State *state, int size, int growth_rate)
{
  return TmHeap_new(
    (TmStateHeader*)state,
    size,
    growth_rate,
    5,
    sizeof(Object),
    test_release,
    test_scan_pointers
    );
}
