#ifndef _lcthw_HashMap_h
#define _lcthw_HashMap_h

#include <stdint.h>
#include <lcthw/darray.h>

#define DEFAULT_NUMBER_OF_BUCKETS 100

typedef int (*Hashmap_compare)(void * a, void * b);
typedef uint32_t (*Hashmap_hash)(void * key);
typedef int (*Hashmap_sort)(DArray * array, DArray_compare cmp);

typedef struct Hashmap{
  DArray * buckets;
  Hashmap_compare compare;
  Hashmap_hash hash;
  Hashmap_sort sort;
} Hashmap;

typedef struct HashmapNode {
  void * key;
  void * data;
  uint32_t hash;
} HashmapNode;

typedef int (*Hashmap_traverse_cb)(HashmapNode * node);

Hashmap * Hashmap_create(Hashmap_compare compare, Hashmap_hash hash, Hashmap_sort sort, size_t size);
void Hashmap_destroy(Hashmap * map);

int Hashmap_set(Hashmap * map, void * key, void * value);
void * Hashmap_get(Hashmap * map, void * key);
int Hashmap_get_node(Hashmap * map, uint32_t hash, DArray * bucket, void * key);

int Hashmap_traverse(Hashmap * map, Hashmap_traverse_cb traverse_cb);
void * Hashmap_delete(Hashmap * map, void * key);

int HashmapNode_compare(HashmapNode * a, HashmapNode * b);
int HashmapNode_find_compare(void * key, HashmapNode * node);

#endif
