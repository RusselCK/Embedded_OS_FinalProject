/*
 * gc_hashmap.h
 *
 *  Created on: 2021年6月20日
 *      Author: HSNL
 */

#ifndef INCLUDE_TREADMILL_GC_HASHMAP_H_
#define INCLUDE_TREADMILL_GC_HASHMAP_H_

#include <stdint.h>
#include <treadmill/gc.h>
#include <treadmill/darray.h>
#include <treadmill/object.h>
#include <treadmill/hashmap.h>

typedef struct gc_HashmapNode {
  Object *key;
  Object *data;
  uint32_t hash;
} HashmapNode;


int gc_Hashmap_set(TmHeap *heap, Object *obj_map, char *key, char *data);
void *gc_Hashmap_get(Object *obj_map, char *key);

void gc_Hashmap_delete(Object *obj_map, char *key);

#endif /* INCLUDE_TREADMILL_GC_HASHMAP_H_ */
