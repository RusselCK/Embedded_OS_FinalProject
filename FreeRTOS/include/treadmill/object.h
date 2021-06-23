/*
 * object.h
 *
 *  Created on: 2021年6月10日
 *      Author: HSNL
 */

#ifndef INCLUDE_TREADMILL_OBJECT_H_
#define INCLUDE_TREADMILL_OBJECT_H_

#include <treadmill/gc.h>
#include <treadmill/darray.h>
#include <treadmill/state.h>
#include <treadmill/hashmap.h>
#include <treadmill/value_type.h>

#include "FreeRTOS.h"
#include "semphr.h"

SemaphoreHandle_t xSemaphore;

typedef struct object_s Object;

struct object_s {
  TmObjectHeader gc;
  ValueType type;
  union {
    double as_num;
    char *as_str;
    void *as_data;
  } data;
  Object* parent;
  Tm_DArray *children;
};

Object* Object_new(TmHeap *heap, ValueType type);
void Object_print(Object *self);

Object* Number_new(TmHeap *heap, double num);
#define OBJ2NUM(obj) (obj->data.as_num)

Object* String_new(TmHeap *heap, char* value);
#define OBJ2STR(obj) ((obj)->data.as_str)

Object* Vector_new(TmHeap *heap, int vector_size);
#define OBJ2ARY(obj) ((Tm_DArray* )(obj->data.as_data))

Object* Map_new(TmHeap *heap);
#define OBJ2HASH(obj) ((Hashmap* )(obj->data.as_data))

void Object_relate(Object* parent, Object* child);
void Object_unrelate(Object* parent, Object* child);
void Object_make_root(Object *self, State *state);
void Object_delete_root(Object *self, State *state);
void Object_destroy(Object *self);


#endif /* INCLUDE_TREADMILL_OBJECT_H_ */
