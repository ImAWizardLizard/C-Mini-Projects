#include <lcthw/darray.h>
#include <assert.h>

// Returns a DArrat struct
DArray *DArray_create(size_t element_size,size_t inital_max){

  DArray *array = malloc(sizeof(DArray));
  check_mem(array);

  array->max = inital_max;
  check(array->max > 0,"Array inital max value must be > 0");

  array->contents = calloc(inital_max,sizeof(void *));
  check_mem(array->contents);

  array->end = 0;
  array->element_size = element_size;
  array->expand_rate = DEFAULT_EXPAND_RATE;

  return array;
error:
  if(array) DArray_destroy(array);
  return NULL;

}


// Sets all the values in the array to NULL
void DArray_clear(DArray *array){

  check(array != NULL,"Array is invalid.");

  int i  = 0;
  if(array->element_size > 0){
    for(i = 0;i < array->max;i++){
      if(array->contents[i] != NULL){
        free(array->contents[i]);
      }
    }
  }
  
error:
  return;
}


// Increases the size/length of the array, whilst keeping the original values,
// plus more values to be able to be filled in
static inline int DArray_resize(DArray * array, size_t new_size){
  check(array != NULL,"Array is invalid.");

  array->max = new_size;
  check(array->max > 0,"The newsize for the array must be > 0");

  void *contents = realloc(
      array->contents,array->max * sizeof(void *));

  check_mem(contents);

  array->contents = contents;
  
  return 0;
error:
  return -1;

}

// Expands the array by the expand_rate value. It does this using the
// DArray_resize function to increase the size of the array + expand_rate.
// It then sets the array_contents pointer, plus the max value of the array, to
// point to the start of the newly created slots of the array, and to set them
// to 0/NULL
int DArray_expand(DArray * array){
  
  check(array != NULL,"Array is invalid.");

  size_t old_max = array->max;
  check(DArray_resize(array,array->max + (int) array->expand_rate) == 0,
      "Failed to expand array to new size %d", array->max + (int) array->expand_rate);

  memset(array->contents + old_max, 0, array->expand_rate + 1);
  return 0;
error:
  return -1;
}

// essentially just re-adjust the max value and adjust length of array,etc
int DArray_contract(DArray * array){
  check(array != NULL,"Array is invalid.");

  int new_size = array->end < (int) array->expand_rate ? 
    (int)  array->expand_rate: array->end;

  return DArray_resize(array,new_size+1);

error:
  return -1;
}


// Destroys the DArray by freeing the memory of the contents and the struct
// itself
void DArray_destroy(DArray * array){

  if(array){
    if(array->contents)
      free(array->contents);
    free(array);
  }
}


// Clears the values of the array first, the destroys the array
void DArray_clear_destroy(DArray * array){
  DArray_clear(array);
  DArray_destroy(array);
}

// Push a value onto the last index of the array, increasing the end value, and
// then chacking is the end value is greater than or equal to the max value of
// the array, if so, expand the array by the default expand rate
int DArray_push(DArray * array, void *el){

  check(array != NULL,"Array is invalid.");

  array->contents[array->end] = el;
  array->end++;

  if(DArray_end(array) >= DArray_max(array)){
    return DArray_expand(array);
  }else{
    return 0;
  }

error:
  return -1;
}

// Function which removes element off end of array, and brings max value down
// to the previous end value, so when the end value exceeds the max value, the
// new max value is assigned properly, but if the end value is not greater than
// the expand rate and the expand rate is in-line with itself (expanding
// properly every time max is exceeded) then dont contract the array
void *DArray_pop(DArray * array){

  check(array->end-1 >=0 ,"Attempt to pop from empty array.");

  void *el = DArray_remove(array,array->end -1);
  array->end--;

  if(DArray_end(array) > (int)array->expand_rate 
      && DArray_end(array) % array->expand_rate){
    DArray_contract(array);
  }

  return el;
error:
  return NULL;
}

int DArray_sort_add(DArray * array,void *el,int (*sort_func)(DArray *,DArray_compare), DArray_compare cmp){

  int rc = 0;

  rc = DArray_push(array,el);
  check(rc != -1, "Could not push element on to end of array");

  if(array->end > 1) sort_func(array,cmp);

error: //fallthrough
  return rc;
}

int DArray_find(DArray * array, void * el, DArray_compare cmp){
  check(array != NULL,"Array is invalid");

  int low = 0, high = (array->end - 1);
  int middle = 0;

  while (low <= high){
    middle = low + (high - low) / 2;

    if(cmp(el,array->contents[middle]) < 0){
      high = middle - 1;
    }else if(cmp(el,array->contents[middle]) > 0){
      low = middle + 1;
    }else{
      return middle;
    }
  }

error: // fall through
  return -1;
}
