#include "minunit.h"
#include "timer.h"
#include <lcthw/darray.h>

static clock_t start,end;

int testcmp(char **a, char **b){

  return strcmp(*a,*b);

}

DArray *create_words(){

  DArray *array = DArray_create(0,5);
  char *words[] = { "asdfasfd",
    "werwar", "13234", "asdfasfd", "oioj" };

  int i = 0;
  for(i = 0; i < 5;i++){
    DArray_push(array,words[i]);
  }
  return array;
}


int is_sorted(DArray *array){
  check(array != NULL,"Invalid array.");

  int i = 0;

  for(i = 0;i<DArray_count(array)-1;i++){
    // testing if anything is not sorted in the array, return 0,(aka. false)
    if(strcmp(DArray_get(array,i),DArray_get(array,i+1)) > 0){
      return 0;
    }
  }
  
  return 1;
error:
  return -1;

}

char *run_sort_test(int (*sort_func)(DArray *,DArray_compare),const char *name){

  DArray *words = create_words();
  mu_assert(!is_sorted(words),"Words should not be sorted");

  debug("--- Testing %s sorting algorithm", name);
  timer_start(&start);
  int rc = sort_func(words,(DArray_compare) testcmp);
  timer_end(&start,&end,"sorting array");
  mu_assert(rc == 0,"Could not sort words");
  mu_assert(is_sorted(words),"Words should be sorted");

  DArray_destroy(words);
  return NULL;
}

char *test_qsort(){
  
  return run_sort_test(DArray_qsort,"qsort");

}

char *test_heapsort(){
  
  return run_sort_test(DArray_heapsort,"heapsort");

}

char *test_mergesort(){
  
  return run_sort_test(DArray_mergesort,"mergesort");

}

char *test_add_sort(){

  DArray *array = DArray_create(0,5);
  char *words[] = { "asdfasfd",
    "werwar", "13234", "asdfasfd", "oioj" };

  int i = 0;
  timer_start(&start);
  for(i = 0; i < 5;i++){
    DArray_sort_add(array,words[i],DArray_mergesort,(DArray_compare) strcmp);
  }
  timer_end(&start,&end,"sorting and adding elements to array");

  mu_assert(is_sorted(array) != -1,"Array is not sorted");

  return NULL;
}

char *test_find(){

  DArray *array = DArray_create(0,5);
  char *words[] = { "asdfasfd",
    "werwar", "13234", "asdfasfd", "oioj" };

  int i = 0;
  for(i = 0; i < 5;i++){
    DArray_sort_add(array,words[i],DArray_mergesort,(DArray_compare) strcmp);
  }

  int position = DArray_find(array,"werwar",(DArray_compare) strcmp);
  printf("Position is %d\n",position);
  printf("%s\n",array->contents[2]);



  return NULL;
}


char *all_tests(){
  mu_suite_start();

  mu_run_test(test_qsort);
  mu_run_test(test_heapsort);
  mu_run_test(test_mergesort);
  mu_run_test(test_add_sort);
  mu_run_test(test_find);

  return NULL;
}

RUN_TESTS(all_tests);
