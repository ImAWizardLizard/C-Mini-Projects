#undef NDEBUG
#include <stdint.h>
#include <lcthw/hashmap.h>
#include <lcthw/dbg.h>
#include <lcthw/bstrlib.h>

static int default_compare(void * a, void * b){
  return bstrcmp((bstring) a, (bstring) b);
}

int HashmapNode_compare(HashmapNode * a, HashmapNode * b){
  return bstrcmp((bstring) a->key, (bstring) b->key);
}

int HashmapNode_find_compare(void * key, HashmapNode * node){
  return bstrcmp((bstring) key, (bstring) node->key);
}

/* Bob Jenkins hash algorithm */

static uint32_t default_hash(void *a){
  
  size_t len = blength((bstring) a);
  char *key = bdata((bstring) a);
  
  uint32_t hash = 0;
  uint32_t i = 0;

  for(hash = i; i < len; i++){
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

Hashmap * Hashmap_create(Hashmap_compare compare, Hashmap_hash hash,Hashmap_sort sort, size_t size){

  Hashmap * map = calloc(1,sizeof(Hashmap));
  check_mem(map);

  map->compare = compare == NULL ? default_compare : compare;
  map->hash = hash == NULL ? default_hash : hash;
  map->sort = sort == NULL ? DArray_qsort : sort;
  map->buckets = DArray_create(sizeof(DArray *), size);
  map->buckets->end = map->buckets->max;
  check_mem(map->buckets);

  return map;

error:
  if(map) Hashmap_destroy(map);
  return NULL;
}

void Hashmap_destroy(Hashmap * map){

  int i = 0;
  int j = 0;

  if(map){
    if(map->buckets){
      for(i = 0; i < DArray_count(map->buckets); i++){
        DArray * bucket = DArray_get(map->buckets,i);
        if(bucket){
          for(j = 0; j < DArray_count(bucket); j++){
            free(DArray_get(bucket,j));
          }
          DArray_destroy(bucket);
        }
      }
      DArray_destroy(map->buckets);
    }
    free(map);
  }
}

HashmapNode * HashmapNode_create(int hash, void * key, void * data){
  
  HashmapNode * node = calloc(1,sizeof(HashmapNode));
  check_mem(node);

  node->key = key;
  node->data = data;
  node->hash = (uint32_t) hash;

  return node;
error:
  if(node) free(node);
  return NULL;
}

static inline DArray *Hashmap_find_bucket(Hashmap * map,void * key, int create, uint32_t * hash_out){
  
  uint32_t hash = map->hash(key);
  int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS;
  check(bucket_n >= 0,"Invalid bucket found: %d", bucket_n);

  *hash_out = hash;

  DArray * bucket = DArray_get(map->buckets,bucket_n);

  if(!bucket && create){
    bucket = DArray_create(sizeof(void *),DEFAULT_NUMBER_OF_BUCKETS);
    check_mem(bucket);
    DArray_set(map->buckets,bucket_n,bucket);
  }

  return bucket;
error:
  return NULL;
}

int Hashmap_set(Hashmap * map, void * key, void * value){
  uint32_t hash = 0;
  DArray * bucket = Hashmap_find_bucket(map, key, 1, &hash);
  check(bucket != NULL, "Error can't create bucket");

  if(Hashmap_get_node(map,hash,bucket,key) != -1)
    return 0;

  HashmapNode * node = HashmapNode_create(hash, key, value);
  check_mem(node);

  DArray_sort_add(bucket, node, map->sort, (DArray_compare) HashmapNode_compare);
  return 0;

error:
  return -1;
}

int Hashmap_get_node(Hashmap * map, uint32_t hash, DArray * bucket, void * key){
  
  check(map != NULL,"Map is invalid");
  int index = DArray_find(bucket, key, (DArray_compare) HashmapNode_find_compare);
  return index;

error: 
  return -1;
}

void * Hashmap_get(Hashmap * map,void * key){
  check(map != NULL, "Map is invlaid");

  uint32_t hash = 0;
  DArray * bucket = Hashmap_find_bucket(map, key, 0, &hash);
  check(bucket != NULL,"Could not create bucket");

  int i = Hashmap_get_node(map, hash, bucket, key);
  check(i != -1,"Could not get node index");

  HashmapNode * node = DArray_get(bucket,i);
  check(node != NULL,"Node is invalid");

  return node->data;
error:
  return NULL;
}

int Hashmap_traverse(Hashmap * map, Hashmap_traverse_cb traverse_cb){
  check(map != NULL, "Map is invalid");

  int i = 0;
  int j = 0;
  int rc = 0;

  for(i = 0; i < DArray_count(map->buckets); i++){
    DArray * bucket = DArray_get(map->buckets,i);
    if(bucket){
      for(j = 0; j < DArray_count(bucket); j++){
        HashmapNode * node = DArray_get(bucket,j);
        rc = traverse_cb(node);
        if(rc != 0)
          return rc;
      }
    }
  }


error:
  return 0;
}

void * Hashmap_delete(Hashmap * map, void *key){
  check(map != NULL,"Map is invalid");
  uint32_t hash = 0;

  DArray * bucket = Hashmap_find_bucket(map, key, 0, &hash);
  check(bucket != NULL, "Bucket is invalid");

  int i = Hashmap_get_node(map, hash, bucket, key);
  check(i != -1, "Could not get index");

  HashmapNode * node = DArray_get(bucket,i);
  check(node != NULL, "Node is invalid");
  
  void * data = node->data;
  free(node);

  HashmapNode * ending = DArray_pop(bucket);

  if(ending != node){
    DArray_set(bucket, i, ending);
  }

  map->sort(bucket,(DArray_compare) HashmapNode_compare);

  return data;
error:
  return NULL;
}
