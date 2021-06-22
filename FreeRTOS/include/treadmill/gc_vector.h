/*
 * gc_vector.h
 *
 *  Created on: 2021年6月20日
 *      Author: HSNL
 */

#ifndef INCLUDE_TREADMILL_GC_VECTOR_H_
#define INCLUDE_TREADMILL_GC_VECTOR_H_

#include <treadmill/gc.h>
#include <treadmill/object.h>

int gc_vector_push(TmHeap *heap, Object *obj_vector, double number);
void gc_vector_pop(Object *obj_vector);
void gc_vector_clear(Object *obj_vector);


#endif /* INCLUDE_TREADMILL_GC_VECTOR_H_ */
