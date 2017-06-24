#include "minunit.h"
#include <lcthw/bstrlib.h>
#include <lcthw/bstree.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

BSTree * map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");

struct tagbstring expected1 = bsStatic("test data 1");
struct tagbstring expected2 = bsStatic("test data 2");
struct tagbstring expected3 = bsStatic("test data 3");

static int traverse_good_cb(BSTreeNode * node){ 
  printf("KEY: %s\n",bdata((bstring) node->key));
  traverse_called++;
  return 0;
}

static int traverse_fail_cb(BSTreeNode * node){ 
  printf("KEY: %s\n",bdata((bstring) node->key));
  traverse_called++;

  if(traverse_called == 2){
    return 1;
  }

  return 0;
}


char * test_create(){
  map = BSTree_create(NULL);
  mu_assert(map != NULL,"Map is invalid");

  return NULL;
}

char * test_destroy(){
  BSTree_destroy(map);

  return NULL;
}


char * test_get_set(){

  int rc = BSTree_set(map,&test1, &expected1);
  mu_assert(rc == 0,"Failed to set &test1");
  bstring result = BSTree_get(map,&test1);
  mu_assert(result == &expected1,"Wrong value for test1");

  rc = BSTree_set(map,&test2, &expected2);
  mu_assert(rc == 0,"Failed to  set &test2");
  result = BSTree_get(map,&test2);
  mu_assert(result == &expected2,"Wrong value for test2");

  rc = BSTree_set(map,&test3, &expected3);
  mu_assert(rc == 0,"Failed to  set &test3");
  result = BSTree_get(map,&test3);
  mu_assert(result == &expected3,"Wrong value for test3");

  return NULL;
}

char * test_traverse(){

  int rc = BSTree_traverse(map,traverse_good_cb);
  mu_assert(rc == 0 ,"Failed to traverse");
  mu_assert(traverse_called == 3,"Wrong count traverse");

  traverse_called = 0;
  rc = BSTree_traverse(map,traverse_fail_cb);
  mu_assert(rc == 1,"Failed to traverse");
  mu_assert(traverse_called == 2,"Wrong count traverse");

  return NULL;
}

char * test_delete(){

  bstring deleted = (bstring) BSTree_delete(map, &test1);
  mu_assert(deleted != NULL, "Got NULL on delete");
  mu_assert(deleted == &expected1,"Should get expected1");
  bstring result = BSTree_get(map,&test1);
  mu_assert(result == NULL,"Should be deleted");


  deleted = (bstring) BSTree_delete(map,&test1);
  mu_assert(deleted == NULL,"Should get NULL on delete");

  deleted = (bstring) BSTree_delete(map, &test2);
  mu_assert(deleted != NULL, "Got NULL on delete");
  mu_assert(deleted == &expected2,"Should get expected2");
  result = BSTree_get(map,&test2);
  mu_assert(result == NULL,"Should be deleted");

  deleted = (bstring) BSTree_delete(map, &test3);
  mu_assert(deleted != NULL, "Got NULL on delete");
  mu_assert(deleted == &expected3,"Should get expected3");
  result = BSTree_get(map,&test3);
  mu_assert(result == NULL,"Should be deleted");
  
  // Deleting non-existent elements
  deleted = (bstring) BSTree_delete(map,&test3);
  mu_assert(deleted == NULL,"Should get NULL on delete");
  return NULL;
}

char * test_fuzzing(){
    
    BSTree * store = BSTree_create(NULL);
    mu_assert(store != NULL,"Store is invalid");

    int i = 0;
    int j = 0;

    bstring numbers[100] = { NULL };
    bstring data[100] = { NULL };

    srand((unsigned int) time(NULL));

    for(i = 0; i < 100; i++){
       int num = rand(); 
       numbers[i] = bformat("%d",num);
       data[i] = bformat("data %d",num);
       mu_assert(BSTree_set(store,numbers[i],data[i]) == 0, "Could not set data");
    }

    for(i = 0; i < 100; i++){
        bstring value = BSTree_delete(store,numbers[i]);
        mu_assert(value == data[i],"Failed to delete the right number");

        mu_assert(BSTree_delete(store,numbers[i]) == NULL,"Should get nothing");
        
        for(j = i + 1; j < 99 - i; j++){
            bstring value = BSTree_get(store,numbers[j]);
            mu_assert(value == data[j],
                    "Failed to get the right number");
        }

        bdestroy(value);
        bdestroy(numbers[i]);
    }

    BSTree_destroy(store);

    return NULL;
}
char * all_tests(){

  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_get_set);
  mu_run_test(test_traverse);
  mu_run_test(test_delete);
  mu_run_test(test_destroy);
  mu_run_test(test_fuzzing);


  return NULL;
}

RUN_TESTS(all_tests);
