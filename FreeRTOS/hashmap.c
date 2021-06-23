/*
 * hashmap.c
 *
 *  Created on: 2021年6月20日
 *      Author: HSNL
 */
#include <stdint.h>
#include <string.h>
#include <treadmill/hashmap.h>
#include <treadmill/_dbg.h>

int reverse_compare(void *a, void *b)
{
  return strcmp((const char *)b, (const char *)a);
}

static int default_compare(void *a, void *b)
{
  return strcmp((const char *)a, (const char *)b);
}

static uint32_t default_hash(void *a)
{
  size_t len = strlen((const char *)a);
  char *key = (char *)a;
  uint32_t hash = 0;
  uint32_t i = 0;

  // Jenkins hash function
  for(hash = i = 0; i < len; ++i)
  {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
  Hashmap *map = calloc(1, sizeof(Hashmap));
  check_mem(map);

  map->compare = compare == NULL ? default_compare : compare;
  map->hash = hash == NULL ? default_hash : hash;
  map->buckets = Tm_DArray_create(sizeof(Tm_DArray *), DEFAULT_NUMBER_OF_BUCKETS);
  map->buckets->end = map->buckets->max;
  check_mem(map->buckets);

  return map;

error:
  if(map) {
    Hashmap_destroy(map);
  }

  return NULL;
}

void Hashmap_destroy(Hashmap *map)
{
  if(map) {
    if(map->buckets) {
      for(int i = 0; i < Tm_DArray_count(map->buckets); i++) {
        Tm_DArray *bucket = Tm_DArray_get(map->buckets, i);
        if(bucket) {
          for(int j = 0; j < Tm_DArray_count(bucket); j++) {
            free(Tm_DArray_get(bucket, j));
          }
          Tm_DArray_destroy(bucket);
        }
      }
      Tm_DArray_destroy(map->buckets);
    }

    free(map);
  }
}
