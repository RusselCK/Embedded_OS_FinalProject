/*
 * gc_test.h
 *
 *  Created on: Jun 8, 2021
 *      Author: HSNL
 */

#ifndef INC_GC_TEST_H_
#define INC_GC_TEST_H_

#include <treadmill/gc.h>
#include <treadmill/state.h>


Tm_DArray* test_rootset(TmStateHeader *state_h);
void test_scan_pointers(TmHeap *heap, TmObjectHeader *object, TmCallbackFn callback);
void test_release(void *value);
State* State_new();
TmHeap* new_heap(State *state, int size, int growth_rate);

#endif /* INC_GC_TEST_H_ */
