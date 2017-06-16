#include "minunit.h"
#include <assert.h>
#include <lcthw/hashmap.h> 
#include <lcthw/bstrlib.h> 

Hashmap * map = NULL;
static int traverse_called = 0;
struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring expect1 = bsStatic("THE VALUE 1");
struct tagbstring expect2 = bsStatic("THE VALUE 2");
struct tagbstring expect3 = bsStatic("THE VALUE 3");

static int traverse_good_cb(HashmapNode * node){
    
  debug("KEY : %s",bdata((bstring) node->key));
  traverse_called++;
  return 0;

}

static int traverse_fail_cb(HashmapNode * node){
    
  debug("KEY : %s",bdata((bstring) node->key));
  traverse_called++;

  if(traverse_called == 2){
    return 1;
  }
  return 0;

}

char *test_create(){

  map = Hashmap_create(NULL,NULL,NULL,100);
  mu_assert(map != NULL,"Could not create map");

  return NULL;
}

char *test_destroy(){

  Hashmap_destroy(map);

  return NULL;
}

char *test_set_get(){

  int rc = Hashmap_set(map,&test1,&expect1);
  mu_assert(rc == 0,"Failed to set &test1");
  bstring result = Hashmap_get(map,&test1);
  mu_assert(bstrcmp(result,(bstring) &expect1) == 0,"Wrong value for test1");

  rc = Hashmap_set(map,&test2,&expect2);
  mu_assert(rc == 0,"Failed to set &test2");
  result = Hashmap_get(map,&test2);
  mu_assert(bstrcmp(result,(bstring) &expect2) == 0,"Wrong value for test2");

  rc = Hashmap_set(map,&test3,&expect3);
  mu_assert(rc == 0,"Failed to set &test3");
  result = Hashmap_get(map,&test3);
  mu_assert(bstrcmp(result,(bstring) &expect3) == 0,"Wrong value for test3");

  return NULL;
}

char *test_traverse(){

  int rc = Hashmap_traverse(map,traverse_good_cb);
  mu_assert(rc == 0,"Failed to traverse");
  mu_assert(traverse_called == 3,"Wrong count traverse");
  
  traverse_called = 0;

  rc = Hashmap_traverse(map,traverse_fail_cb);
  mu_assert(rc == 1,"Failed to traverse");
  mu_assert(traverse_called == 2,"Wrong count for traverse fail");

  return NULL;
}


char *test_delete(){

  bstring deleted = Hashmap_delete(map,&test1);
  mu_assert(deleted != NULL,"Test1 is invalid");
  mu_assert(bstrcmp(deleted,(bstring) &expect1) == 0,"Wrong value for test1");
  bstring result = Hashmap_get(map,&test1);
  mu_assert(result == NULL,"Should delete");

  deleted = Hashmap_delete(map,&test2);
  mu_assert(deleted != NULL,"test2 is invalid");
  mu_assert(bstrcmp(deleted,(bstring) &expect2) == 0,"Wrong value for test2");
  result = Hashmap_get(map,&test2);
  mu_assert(result == NULL,"Should delete");

  deleted = Hashmap_delete(map,&test3);
  mu_assert(deleted != NULL,"test3 is invalid");
  mu_assert(bstrcmp(deleted,(bstring) &expect3) == 0,"Wrong value for test3");
  result = Hashmap_get(map,&test3);
  mu_assert(result == NULL,"Should delete");

  return NULL;
}


char *all_tests(){

  mu_suite_start();
  mu_run_test(test_create);
  mu_run_test(test_set_get);
  mu_run_test(test_traverse);
  mu_run_test(test_delete);
  mu_run_test(test_destroy);


  return NULL;
}

RUN_TESTS(all_tests);
