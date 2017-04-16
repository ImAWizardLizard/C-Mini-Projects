#include <lcthw/darray_algos.h>
#include <stdlib.h>
#include <bsd/stdlib.h>

int DArray_qsort(DArray * array, DArray_compare cmp){
  check(array != NULL,"Array is invalid.");
  qsort(array->contents,DArray_count(array),sizeof(void *),cmp);
  return 0;
error:
  return -1;
}

int DArray_heapsort(DArray * array, DArray_compare cmp){
  int rc = -1;
  check(array != NULL,"Array is invalid.");
  rc = heapsort(array->contents,DArray_count(array),sizeof(void *),cmp);

error: // fallthrough
  return rc;
}

int DArray_mergesort(DArray * array, DArray_compare cmp){
  int rc = -1;
  check(array != NULL,"Array is invalid.");
  rc = mergesort(array->contents,DArray_count(array),sizeof(void *),cmp);
error: //fallthrough
  return rc;
}
