#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>

static List *list = NULL;
static List *list_2 = NULL;
static List *list_3 = NULL;
static List *list_4 = NULL;
static List *list_5 = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

static inline int string_compare(char *string1, char *string2){
  return strcmp(string1,string2) == 0;
}

char *test_create(){

  list = List_create();
  list_2 = List_create();
  list_3 = List_create();
  list_4 = List_create();
  list_5 = List_create();
  mu_assert(list != NULL, "Failed to create list.")
  mu_assert(list_2 != NULL, "Failed to create list.")
  mu_assert(list_3 != NULL, "Failed to create list.")
  return NULL;
}

char *test_destroy(){

  List_destroy(list);
  List_destroy(list_2);
  List_destroy(list_3);
  return NULL;
}

char *test_push_pop(){

  List_push(list,test1);
  mu_assert(string_compare(List_last(list), test1), "Wrong last value");

  List_push(list,test2);
  mu_assert(string_compare(List_last(list), test2), "Wrong last value");

  List_push(list,test3);
  mu_assert(string_compare(List_last(list), test3), "Wrong last value");
  mu_assert(List_count(list) == 3, "Wrong count push");

  char *val = List_pop(list);
  mu_assert(string_compare(val, test3), "Wrong value on pop");

  val = List_pop(list);
  mu_assert(string_compare(val, test2), "Wrong value on pop");

  val = List_pop(list);
  mu_assert(string_compare(val, test1), "Wrong value on pop");
  mu_assert(List_count(list) == 0, "Wrong count on pop");

  return NULL;
}


char *test_unshift(){

  // list_1

  List_unshift(list,test1);
  mu_assert(string_compare(List_first(list), test1), "Wrong first value");

  List_unshift(list,test2);
  mu_assert(string_compare(List_first(list), test2), "Wrong first value");

  List_unshift(list,test3);
  mu_assert(string_compare(List_first(list), test3), "Wrong first value");

  List_unshift(list,"hello");
  mu_assert(string_compare(List_first(list), "hello"), "Wrong first value");
  mu_assert(List_count(list) == 4, "Wrong count on unshift");

  // list_2

  List_unshift(list_2,"harry");
  mu_assert(string_compare(List_first(list_2), "harry"), "Wrong first value");

  List_unshift(list_2,"newton");
  mu_assert(string_compare(List_first(list_2), "newton"), "Wrong first value");

  List_unshift(list_2,"james");
  mu_assert(string_compare(List_first(list_2), "james"), "Wrong first value");
  mu_assert(List_count(list_2) == 3,"Wrong count on unshift");

  List_unshift(list_2,"issac");
  mu_assert(string_compare(List_first(list_2), "issac"), "Wrong first value");
  mu_assert(List_count(list_2) == 4,"Wrong count on unshift");
  return NULL;
}


char *test_remove(){

  // We only need to test the remove function in the middle since push/shift
  // exists in the other cases
  
  char *val = List_remove(list,list->first->next);
  mu_assert(string_compare(val, test2), "Wrong element removed");
  mu_assert(List_count(list) == 2, "Wrong count after remove");
  mu_assert(string_compare(List_first(list), test3), "Wrong first after remove");
  mu_assert(string_compare(List_last(list), test1),"Wrong last after remove");
  
  return NULL;

}


char *test_shift(){

  mu_assert(List_count(list) != 0,"Wrong count before shift");

  char *val = List_shift(list);
  mu_assert(string_compare(val, test3), "Wrong value on shift");

  val = List_shift(list);
  mu_assert(string_compare(val, test1), "Wrong value on shift");

  mu_assert(List_count(list) == 0,"Wrong count before shift");
  return NULL;
}

char *test_join(){
  
  mu_assert(string_compare(List_first(list), "hello"), "Wrong first value");
  mu_assert(string_compare(List_last(list), "test1 data"), "Wrong last value");

  mu_assert(string_compare(List_first(list_2), "issac"), "Wrong first value");
  mu_assert(string_compare(List_last(list_2), "harry"), "Wrong last value");

  List_join(list,list_2);

  mu_assert(string_compare(List_first(list), "hello"), "Wrong first value");
  mu_assert(string_compare(List_last(list), "harry"), "Wrong last value");

  mu_assert(list->count == 8,"Wrong count after join");

  return NULL;
}

char *test_split(){

  List_split(list,list_5,list_3);

  mu_assert(string_compare(List_first(list_5), "hello"), "wrong first value");
  mu_assert(string_compare(List_last(list_5), test1),"wrong last value");
  
  mu_assert(string_compare(List_first(list_3), "issac"), "wrong first value");
  mu_assert(string_compare(List_last(list_3), "harry"), "wrong last value");
  

  return NULL;
}

char *test_insert(){

  List_insert_index(list,2,"hello");

  return NULL;
}

char *test_values(){


  List_values(list_5);
  List_values(list_3);

  return NULL;
}

char *test_copy(){

  List_copy(list_4,list_3);


  return NULL;
}

char *all_tests(){

  mu_suite_start();

  mu_run_test(test_create);

  /*
  mu_run_test(test_push_pop);
  mu_run_test(test_remove);
  mu_run_test(test_shift);
  mu_run_test(test_destroy);
  */

  mu_run_test(test_unshift);
  mu_run_test(test_join);
  mu_run_test(test_split);
  mu_run_test(test_copy);
  mu_run_test(test_insert);
  //mu_run_test(test_values);
  mu_run_test(test_destroy);


  return NULL;

}

RUN_TESTS(all_tests)
