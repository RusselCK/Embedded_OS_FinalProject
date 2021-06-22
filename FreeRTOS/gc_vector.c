/*
 * gc_vector.c
 *
 *  Created on: 2021年6月20日
 *      Author: HSNL
 */

#include <treadmill/gc_vector.h>


void gc_vector_push(TmHeap *heap, Object *obj_vector, double num) {
	Object *obj_num = Number_new(heap, num);

	Tm_DArray_push(OBJ2ARY(obj_vector), obj_num);

	Object_relate(obj_vector, obj_num);
}


void gc_vector_pop(Object *obj_vector) {
	Object *obj_num = Tm_DArray_pop(OBJ2ARY(obj_vector));
	Object_unrelate(obj_vector, obj_num);
}

void gc_vector_clear(Object *obj_vector) {

	Tm_DArray *vector = OBJ2ARY(obj_vector);

	for(int i = Tm_DArray_end(vector); i > 0; --i) {
		gc_vector_pop(obj_vector);
	}
}
