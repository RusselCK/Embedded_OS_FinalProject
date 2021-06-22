/*
 * object.c
 *
 *  Created on: 2021年6月10日
 *      Author: HSNL
 */

#include <treadmill/object.h>

#include "string.h"

Object*
Object_new(TmHeap *heap, ValueType type)
{
  Object *obj = (Object*)Tm_allocate(heap);
  obj->type = type;
  obj->parent = NULL;
  obj->children = Tm_DArray_create(sizeof(Object*), 10);
  return obj;
}

void
Object_print(Object *self)
{
  printf("#<Object %p @cell=%p, @children=%i>\n", self, self->gc.cell, Tm_DArray_count(self->children));
}

Object*
Number_new(TmHeap *heap, double num)
{
  Object* obj = Object_new(heap, NumberType);
  obj->data.as_num = num;

  return obj;
}

Object*
String_new(TmHeap *heap, char* value)
{
  Object* obj = Object_new(heap, StringType);
  obj->data.as_str = strdup(value);
  return obj;
}

Object*
Vector_new(TmHeap *heap, int vector_size)
{
  Object* obj = Object_new(heap, VectorType);
  Tm_DArray *array = Tm_DArray_create(sizeof(Object*), vector_size);
  obj->data.as_data = array;

  return obj;
}

Object*
Map_new(TmHeap *heap)
{
  Object* obj = Object_new(heap, MapType);
  obj->data.as_data = Hashmap_create(NULL, NULL);

  return obj;
}

void
Object_relate(Object* parent, Object* child)
{
  Tm_DArray_push(parent->children, child);
  child->parent = parent;
}

void
Object_make_root(Object *self, State *state)
{
  Tm_DArray_push(state->registers, self);
}

void Object_unrelate(Object* parent, Object* child) {

	int children_i;
	for (int i= 0; i < Tm_DArray_end(parent->children); ++i) {
		if (Tm_DArray_at(parent->children, i) == child) {
			children_i = i;
			--Tm_DArray_end(parent->children);
			break;
		}
	}

	for (int i = children_i; i < Tm_DArray_end(parent->children); ++i)
		Tm_DArray_at(parent->children, i) = Tm_DArray_at(parent->children, i+1);

	child->parent = NULL;
}

void
Object_destroy(Object *self)
{
  if (self->parent)
	Object_unrelate(self->parent, self);

  Tm_DArray_destroy(self->children);

  if (self && self->type == StringType) {
	  if (self->data.as_str)
		  free(self->data.as_str);
  }
  free(self);
}
