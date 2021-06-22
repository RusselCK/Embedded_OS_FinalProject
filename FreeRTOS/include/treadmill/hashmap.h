/*
 * hashmap.h
 *
 *  Created on: 2021年6月20日
 *      Author: HSNL
 */

#ifndef INCLUDE_TREADMILL_HASHMAP_H_
#define INCLUDE_TREADMILL_HASHMAP_H_

#include <stdint.h>
#include <treadmill/gc.h>
#include <treadmill/darray.h>

#define DEFAULT_NUMBER_OF_BUCKETS 100

typedef int (*Hashmap_compare)(void *a, void *b);
typedef uint32_t (*Hashmap_hash)(void *key);

typedef struct Hashmap {
  Tm_DArray *buckets;
  Hashmap_compare compare;
  Hashmap_hash hash;
} Hashmap;

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash);
void Hashmap_destroy(Hashmap *map);

int reverse_compare(void *a, void *b);

#endif /* INCLUDE_TREADMILL_HASHMAP_H_ */
