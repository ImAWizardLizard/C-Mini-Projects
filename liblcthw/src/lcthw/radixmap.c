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

//define ByteOf(x,y) (((uint8_t *)x)[(y)])
#define ByteOf(x,y) (*(x) >> (8*(y)) & 0xFF)
//#define ByteOf(x,y) (*((uint8_t *)x + (y)))

// TODO: Make radix_sort algorithm more efficent

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


void RadixMap_sort(RadixMap * map, int position){
  check(map != NULL,"RadixMap is invalid.");
  uint64_t *source = &map->contents[position].raw;
  uint64_t *temp = &map->temp[position].raw;

  /*
  uint64_t *added_element = &map->contents[map->end - 1].raw;
  int length = map->end - position;
  int i = 0;
  */
  
  radix_sort(0,map->end - position,source,temp);
  radix_sort(1,map->end - position,temp,source);
  radix_sort(2,map->end - position,source,temp);
  radix_sort(3,map->end - position,temp,source);
  
error:
  return;

}

static inline int partition(RadixMap * map,int low, int high){
  int pivot = getKey(map->contents[high]);
  RMElement temp;

  int i = (low - 1);
  int j = 0;

  for(j = low; j <= (high - 1);j++){
    if(getKey(map->contents[j]) <= pivot){
      i++;

      temp = map->contents[j];
      map->contents[j] = map->contents[i];
      map->contents[i] = temp;
    }
  }

  temp = map->contents[i + 1];
  map->contents[i + 1] = map->contents[high];
  map->contents[high] = temp;

  return (i + 1);
}

RadixMap * RadixMap_quicksort(RadixMap * map,int low, int high){
  int p = 0;
  if(low < high){
    p = partition(map,low, high);
    RadixMap_quicksort(map,low,p - 1);
    RadixMap_quicksort(map,p + 1,high);
  }
  
  return map;
}



RadixMap * RadixMap_heapsort(RadixMap * map){



}

static inline RadixMap *RadixMap_merge(RadixMap * left,RadixMap * right){
  RadixMap * result = RadixMap_create(left->max);

  while(left->end > 0 && right->end > 0){
    if(left->contents[0].data.key < right->contents[0].data.key){
      result->contents[result->end++] = left->contents[0];
      left->contents++;
      left->end--;
    }else{
      result->contents[result->end++] = right->contents[0];
      right->contents++;
      right->end--;
    }
  }

  while(left->end > 0){
      result->contents[result->end++] = left->contents[0];
      left->contents++;
      left->end--;
  }

  while(right->end > 0){
      result->contents[result->end++] = right->contents[0];
      right->contents++;
      right->end--;
  }
  
  return result;
}

RadixMap * RadixMap_mergesort(RadixMap * map){
  if(map->end <= 1){
    return map;
  }

  RadixMap * result = NULL;
  RadixMap * left = RadixMap_create(map->max);
  RadixMap * right = RadixMap_create(map->max);

  check_mem(left != NULL && right != NULL);

  int i = 0;
  int middle = map->end / 2;

  for(i = 0;i< map->end - 1;i++){
    if(i < middle){
      left->contents[left->end++] = map->contents[i];
    }else{
      right->contents[right->end++] = map->contents[i];
    }
  }

  left = RadixMap_mergesort(left);
  right = RadixMap_mergesort(right);

  result =  RadixMap_merge(left,right);

  RadixMap_destroy(left);
  RadixMap_destroy(right);

  return result;

error:
  if(left) RadixMap_destroy(left);
  if(right) RadixMap_destroy(right);
  return NULL;

}

RMElement *RadixMap_find(RadixMap * map, uint32_t to_find){
  
  check(map != NULL, "Invalid map.");
  check(map->contents != NULL, "Invalid map.");

  int low = 0;
  int high = map->end - 1;
  RMElement *data = map->contents;

  while(low <= high){

    int middle = low + (high - low) / 2;
    uint32_t key = getKey(data[middle]);
    
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

int RadixMap_bisect(RadixMap * map, uint32_t key){

  check(map != NULL, "Invalid map.");
  check(map->contents != NULL, "Invalid map.");

  int low = 0;
  int high = map->end - 1;
  RMElement *data = map->contents;

  int result = 0;
  int middle = 0;


  if( key < getKey(data[0])){
    result =  0;
  }else if(key > getKey(data[map->end - 1])){
    result =  map->end;
  }else{
    while(low <= high){
      middle = low + ( high - low) / 2;
      if(getKey(data[middle]) < key){
        low = middle + 1;
      }else{
        result = middle;
        high = middle - 1;
      }
    }

  }

  return result;

error:
  return -1;

}

int RadixMap_add(RadixMap * map,uint32_t key, uint32_t value){
  check(key < UINT32_MAX, "Key can't be equal to UINT32_MAX.");

  RMElement element = {.data = {.key = key,.value = value } };
  check(map->end + 1 < map->max, "RadixMap is full");

  int minimum = RadixMap_bisect(map,key); 
  check(minimum != -1,"Could not get minimum index");
   
  map->contents[map->end++] = element;
  RadixMap_sort(map,minimum);

  return 0;
error:
  return -1;
}

int RadixMap_delete(RadixMap * map, RMElement * el){
  check(map->end > 0,"There is nothing to delete.");
  check(el != NULL,"Can't delete a NULL element.");
  
  int minimum = RadixMap_bisect(map,el->data.key);
  check(minimum != -1,"Could not get minimum position");
  el->data.key = UINT32_MAX;
  
  if(map->end > 1){
    RadixMap_sort(map,minimum);
  }

  map->end--;

  return 0;
error:
  return -1;
}
