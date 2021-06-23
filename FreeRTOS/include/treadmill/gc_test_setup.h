/*
 * gc_test.h
 *
 *  Created on: Jun 8, 2021
 *      Author: HSNL
 */

#ifndef INC_GC_TEST_H_
#define INC_GC_TEST_H_

#include "minunit.h"
#include <treadmill/gc.h>
#include <treadmill/state.h>

#define assert_heap_size(A) mu_assert(TmHeap_size(heap) == (A), "Wrong heap size. Expected " #A)
#define assert_white_size(A) mu_assert(TmHeap_white_size(heap) == (A), "Wrong white size. Expected " #A)
#define assert_ecru_size(A) mu_assert(TmHeap_ecru_size(heap) == (A), "Wrong ecru size. Expected " #A)
#define assert_grey_size(A) mu_assert(TmHeap_grey_size(heap) == (A), "Wrong grey size. Expected " #A)
#define assert_black_size(A) mu_assert(TmHeap_black_size(heap) == (A), "Wrong black size. Expected " #A)

Tm_DArray* test_rootset(TmStateHeader *state_h);
void test_scan_pointers(TmHeap *heap, TmObjectHeader *object, TmCallbackFn callback);
void test_release(void *value);
State* State_new();
TmHeap* new_heap(State *state, int size, int growth_rate);

#endif /* INC_GC_TEST_H_ */
