/*
 * gc_hashmap.c
 *
 *  Created on: 2021年6月20日
 *      Author: HSNL
 */

/*
 * hashmap.c
 *
 *  Created on: 2021年6月15日
 *      Author: HSNL
 */

#include <stdint.h>
#include <string.h>
#include <treadmill/gc_hashmap.h>
#include <treadmill/_dbg.h>


static inline HashmapNode *Hashmap_node_create(int hash, Object *obj_key, Object *obj_data)
{
  HashmapNode *node = calloc(1, sizeof(HashmapNode));
  check_mem(node);

  node->key = obj_key;
  node->data = obj_data;
  node->hash = hash;

  return node;

error:
  return NULL;
}

static inline Tm_DArray *Hashmap_find_bucket(Hashmap *map, void *key,
  int create, uint32_t *hash_out)
{
  uint32_t hash = map->hash(key);
  int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS;
  check(bucket_n >= 0, "Invalid bucket found: %d", bucket_n);
  *hash_out = hash;

  Tm_DArray *bucket = Tm_DArray_get(map->buckets, bucket_n);

  if (!bucket && create) {
    bucket = Tm_DArray_create(sizeof(HashmapNode *), DEFAULT_NUMBER_OF_BUCKETS);
    check_mem(bucket);
    Tm_DArray_set(map->buckets, bucket_n, bucket);
  }

  return bucket;

error:
  return NULL;
}

int gc_Hashmap_set(TmHeap *heap, Object *obj_map, char *key, char *data)
{
  Hashmap *map = OBJ2HASH(obj_map);

  uint32_t hash = 0;
  Tm_DArray *bucket = Hashmap_find_bucket(map, key, 1, &hash);
  check(bucket, "Error can't create bucket.");

  Object *obj_key = String_new(heap, key);
  check(obj_key, "Error can't create obj_key.");
  Object *obj_data = String_new(heap, data);
  check(obj_data, "Error can't create obj_data.");
  HashmapNode *node = Hashmap_node_create(hash, obj_key, obj_data);
  check_mem(node);

  Tm_DArray_push(bucket, node);

  Object_relate(obj_key, obj_data);
  Object_relate(obj_map, obj_key);

  return 0;

error:
  return -1;
}

static inline int Hashmap_get_node(Hashmap *map, uint32_t hash, Tm_DArray *bucket, char *key)
{
  for(int i = 0; i < Tm_DArray_end(bucket); i++) {
    HashmapNode *node = Tm_DArray_get(bucket, i);
    if(node->hash == hash && map->compare(OBJ2STR(node->key), key) == 0) {
      return i;
    }
  }

  return -1;
}

void *gc_Hashmap_get(Object *obj_map, char *key)
{
  Hashmap *map = OBJ2HASH(obj_map);

  uint32_t hash = 0;
  Tm_DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
  if(!bucket) return NULL;

  int i = Hashmap_get_node(map, hash, bucket, key);
  if(i == -1) return NULL;

  HashmapNode *node = Tm_DArray_get(bucket, i);
  check(node != NULL, "Failed to get node from bucket when it should exist.");

  return OBJ2STR(node->data);

error:
  return NULL;
}


void gc_Hashmap_delete(Object *obj_map, char *key)
{
  Hashmap *map = OBJ2HASH(obj_map);

  uint32_t hash = 0;
  Tm_DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
  if(!bucket) return;

  int i = Hashmap_get_node(map, hash, bucket, key);
  if(i == -1) return;

  HashmapNode *node = Tm_DArray_get(bucket, i);
  Object *obj_key = node->key;

  Object_unrelate(obj_map, obj_key);
  free(node);

  HashmapNode *ending = Tm_DArray_pop(bucket);
  if(ending != node) {
    Tm_DArray_set(bucket, i, ending);
  }

}

