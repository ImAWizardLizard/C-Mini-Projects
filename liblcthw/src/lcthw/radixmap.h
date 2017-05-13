#ifndef _radixmap_h
#define _radixmap_h

#include <stdint.h>

typedef union RMElement{
  uint64_t raw;
  struct{
    uint32_t key;
    uint32_t value;
  } data;
} RMElement;

typedef struct RadixMap{
  size_t max;
  size_t end;
  uint32_t counter;
  RMElement *contents;
  RMElement *temp;
} RadixMap;

#define getKey(x) (x).data.key

// RadixMap creation function
RadixMap *RadixMap_create(size_t max);

// RadixMap destroy function
void RadixMap_destroy(RadixMap * map);

// RadixMap sort function
void RadixMap_sort(RadixMap * map,int position);

// RadixMap quicksort function
RadixMap * RadixMap_quicksort(RadixMap * map,int low, int high);

// RadixMap heapsort function
RadixMap * RadixMap_heapsort(RadixMap * map);

// RadixMap mergesort function
RadixMap * RadixMap_mergesort(RadixMap *map);

// Function to return a RadixMap element based on a given key
RMElement *RadixMap_find(RadixMap *map,uint32_t key);

int RadixMap_bisect(RadixMap *map,uint32_t key);

// RadixMap function to add element to the map
int RadixMap_add(RadixMap * map,uint32_t key, uint32_t value);

// RadixMap function to remove element fromt he map
int RadixMap_delete(RadixMap *map,RMElement *el);

#endif
