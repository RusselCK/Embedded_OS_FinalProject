/*
 * object.c
 *
 *  Created on: 2021年6月10日
 *      Author: HSNL
 */

#include <treadmill/object.h>

#include <string.h>

Object*
Object_new(TmHeap *heap, ValueType type)
{
  Object *obj = NULL;
  if (xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE ) {
	  obj = (Object*)Tm_allocate(heap);
	  xSemaphoreGive( xSemaphore );

	  obj->type = type;
	  obj->parent = NULL;
	  obj->children = NULL;
	  return obj;
  }
  check(obj, "Error can't create obj.");
error:
  return NULL;
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
  obj->children = array;

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
  if (!parent->children)
	  parent->children = Tm_DArray_create(sizeof(Object*), 10);

  Tm_DArray_push(parent->children, child);
  child->parent = parent;
}

void
Object_make_root(Object *self, State *state)
{
  Tm_DArray_push(state->rootset, self);
}

void Object_unrelate(Object* parent, Object* child) {

	if (child->parent != parent) return;

	int child_i = -1;
	for (int i= 0; i < Tm_DArray_end(parent->children); ++i) {
		if ((parent->children->contents[i]) == child) {
			child_i = i;
			--(parent->children->end);
			break;
		}
	}

	if (child_i == -1) return;

	for (int i = child_i; i < Tm_DArray_end(parent->children); ++i)
		parent->children->contents[i] = parent->children->contents[i+1];

	child->parent = NULL;
}

void
Object_destroy(Object *self)
{
  if (self->parent)
	Object_unrelate(self->parent, self);

  if (self->children) {
	  for (int i = 0; i < Tm_DArray_end(self->children); ++i) {
		  Object *child = self->children->contents[i];
		  child->parent = NULL;
	  }
  }
  Tm_DArray_destroy(self->children);

  if (self) {
	  if( self->type == StringType) {
		  if (self->data.as_str)
			  free(self->data.as_str);
	  } else if ( self->type == MapType ) {
		  Hashmap_destroy(OBJ2HASH(self));
	  }
  }


  free(self);
}

void
Object_delete_root(Object *self, State *state) {

	int root_i = -1;
	for(int i = 0; i < Tm_DArray_end(state->rootset); ++i) {
		if ((state->rootset->contents[i]) == self) {
			root_i = i;
			--(state->rootset->end);
			break;
		}
	}

	if (root_i == -1) return;

	for (int i = root_i; i < Tm_DArray_end(state->rootset); ++i)
		state->rootset->contents[i] = state->rootset->contents[i+1];
}
