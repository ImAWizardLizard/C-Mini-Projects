#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <lcthw/radixmap.h>
#include <lcthw/dbg.h>


RadixMap *RadixMap_create(size_t max){

  RadixMap *map = calloc(1,sizeof(RadixMap));
  check_mem(map);

  map->contents = calloc(max + 1, sizeof(RMElement));
  check_mem(map->contents);

  map->temp = calloc(max + 1, sizeof(RMElement));
  check_mem(map->temp);

  map->max = max;
  map->end = 0;

  return map;
error:
  return NULL;
}

void RadixMap_destroy(RadixMap * map){
  if(map){
    free(map->contents);
    free(map->temp); 
    free(map);
  }
}

// Two ways to get each individual byte from a 32 bit(4 Bytes) integer

//#define ByteOf(x,y) (((uint8_t *)x)[(y)])
#define ByteOf(x,y) (*(x) >> (8*(y)) & 0xFF)
//#define ByteOf(x,y) (*((uint8_t *)x + (y)))

// TODO: Make radix_sort algorithm more efficent

// 1. Find minimum position for new element with find function, then only sort
// from there to the end

// 2.Keep track of the biggest key currently being used, and then only sort
// enough digits to handle that key (vice versa for smallest key as well)
static inline void radix_sort(short offset, uint64_t max, uint64_t * source, uint64_t * dest){
  
  uint64_t count[256] = { 0 };
  uint64_t *cp = NULL;
  uint64_t *sp = NULL;
  uint64_t *end = NULL;
  uint64_t s = 0;
  uint64_t c = 0;

  // count occurances of every byte value;
  for(sp = source, end = source + max;sp < end; sp++){
    count[ByteOf(sp,offset)]++; // increases the number of occurances of the byte in the array
  }

  // transform count into index by summing 
  // elements and storing into same array
  for(s = 0,cp = count,end = count + 256; cp < end; cp++){
    c = *cp;
    *cp = s;
    s+= c; 
  }

  // fill dest with right values in the right place
  for(sp = source, end = source + max;sp < end; sp++){
    cp = count + ByteOf(sp,offset);
    dest[*cp] = *sp;
    ++(*cp); 
  }

}

// TODO: add parameter  to sort from mimimum position
void RadixMap_sort(RadixMap * map){
  uint64_t *source = &map->contents[0].raw;
  uint64_t *temp = &map->temp[0].raw;

  radix_sort(0,map->end,source,temp);
  radix_sort(1,map->end,temp,source);
  radix_sort(2,map->end,source,temp);
  radix_sort(3,map->end,temp,source);
}

void RadixMap_sort_position(RadixMap * map, size_t position){
  uint64_t *source = &map->contents[position].raw;
  uint64_t *temp = &map->temp[position].raw;

  if(ByteOf(source,0)){
    radix_sort(0,map->end,source,source);
    if(ByteOf(source,1)){
      radix_sort(1,map->end,source,source);
      if(ByteOf(source,2)){
        radix_sort(2,map->end,source,source);
        if(ByteOf(source,3)){
          radix_sort(3,map->end,source,source);
        }
      }
    }
  }

  /*
  if(ByteOf(source,0)){
    radix_sort(0,map->end,source,temp);
    if(ByteOf(temp,1)){
      radix_sort(1,map->end,temp,source);
      if(ByteOf(source,2)){
        radix_sort(2,map->end,source,temp);
        if(ByteOf(source,3)){
          radix_sort(3,map->end,temp,source);
        }
      }
    }
  }
  */

}

RMElement *RadixMap_find(RadixMap * map, uint32_t to_find){
  
  check(map != NULL, "Invalid map.");
  check(map->contents != NULL, "Invalid map.");

  int low = 0;
  int high = map->end - 1;
  RMElement *data = map->contents;

  while(low <= high){

    int middle = low + (high - low) / 2;
    uint32_t key = data[middle].data.key;
    
    if(to_find < key){
      high = middle - 1;
    }else if(to_find > key){
      low = middle + 1;
    }else{
      return &data[middle];
    }

  }

error: //fallthrough
  return NULL;
}

int RadixMap_find_minimum(RadixMap * map, uint32_t to_find){
  check(map != NULL, "Invalid map.");
  check(map->contents != NULL, "Invalid map.");

  int low = 0;
  int high = map->end - 1;
  RMElement *data = map->contents;

  int middle = 0;
  while(low <= high){
    
    int middle = low + (high - low) / 2;
    uint32_t key = data[middle].data.key;

    if(to_find < key){
      if(to_find > data[middle - 1].data.key){
        return middle;
      }
      high = middle - 1;
    }else if(to_find > key){
      if(to_find > data[middle + 1].data.key){
        return middle;
      }
      low = middle + 1;
    }else{
      return middle;
    }
  }
  return middle;

error:
    return -1;

}

int RadixMap_add(RadixMap * map,uint32_t key, uint32_t value){
  check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX.");

  RMElement element = {.data = {.key = key,.value = value } };
  check(map->end + 1 < map->max, "RadixMap is full");

  size_t minimum = RadixMap_find_minimum(map,key); 
  check(minimum != -1,"Could not get minimum index");
   
  map->contents[map->end++] = element;
  RadixMap_sort_position(map,minimum);

  return 0;
error:
  return -1;
}

int RadixMap_delete(RadixMap * map, RMElement * el){
  check(map->end > 0,"There is nothing to delete.");
  check(el != NULL,"Can't delete a NULL element.");

  el->data.key = UINT32_MAX;
  
  if(map->end > 1){
    RadixMap_sort(map);
  }

  map->end--;

  return 0;
error:
  return -1;
}
