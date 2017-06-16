#include <lcthw/string_algos.h>
#include <lcthw/bstrlib.h>
#include "minunit.h"
#include <time.h>

struct tagbstring IN_STR = bsStatic(
    "I have ALPHA beta ALPHA and oranges ALPHA");

struct tagbstring ALPHA = bsStatic("ALPHA");

const int TEST_TIME = 1;

char * test_find_scan(){

  StringScanner * scan = StringScanner_create(&IN_STR);
  mu_assert(scan != NULL,"Could not create a StringScanner");

  int find_i = String_find(&IN_STR,&ALPHA);
  mu_assert(find_i > 0, "Could not find 'ALPHA' in test string");

  int scan_i = StringScanner_scan(scan,&ALPHA);
  mu_assert(scan_i > 0,"Could not find 'ALPHA' in test \
      string using the StringScanner_scan");

  mu_assert(scan_i == find_i,"scan_i and find_i are not the same");

  // Find serveral other occurances of the pattern by callign
  // StringScanner_scan again
  scan_i = StringScanner_scan(scan,&ALPHA);
  mu_assert(scan_i > 0,"Should find the second occurance of ALPHA");

  scan_i = StringScanner_scan(scan,&ALPHA);
  mu_assert(scan_i > 0,"Should find the third occurance of ALPHA");

  scan_i = StringScanner_scan(scan,&ALPHA);
  mu_assert(scan_i == -1,"Should not find it");

  StringScanner_destroy(scan);
  return NULL;
}


char * test_binstr_performance(){

  int i = 0;
  int found_at = 0;
  unsigned long found_count = 0;
  time_t elapsed = 0;
  time_t start = time(NULL);

  do{

    for(i = 0; i < 1000; i++){
      found_at = binstr(&IN_STR, 0, &ALPHA);
      mu_assert(found_at != BSTR_ERR, "Failed to find");
      found_count++;
    }
    elapsed = time(NULL) - start;
  }while(elapsed <= TEST_TIME);

  printf("BINSTR COUNT: %lu, END TIME: %d, OPS: %f\n",
      found_count, (int) elapsed, (double)found_count / elapsed);

  return NULL;
}

char * test_find_performance(){

  int i = 0;
  int found_at = 0;
  unsigned long found_count = 0;
  time_t elapsed = 0;
  time_t start = time(NULL);

  do{
    for(i = 0; i < 1000; i++){
      found_at = String_find(&IN_STR, &ALPHA);
      found_count++;
    }
    elapsed = time(NULL) - start;
  }while(elapsed <= TEST_TIME);

  printf("FIND COUNT: %lu, END TIME: %d, OPS: %f\n",
      found_count, (int) elapsed, (double)found_count / elapsed);

  return NULL;
}

char * test_scan_performance(){

  int i = 0;
  int found_at = 0;
  unsigned long found_count = 0;
  time_t elapsed = 0;
  time_t start = time(NULL);
  StringScanner * scan = StringScanner_create(&IN_STR);

  do{
    for(i = 0; i < 1000; i++){
      found_at = 0;
      do{
        found_at = StringScanner_scan(scan,&ALPHA);
        found_count++;
      }while(found_at != -1);
    }
    elapsed = time(NULL) - start;
  }while(elapsed <= TEST_TIME);

  printf("SCAN COUNT: %lu, END TIME: %d, OPS: %f\n",
      found_count, (int) elapsed, (double)found_count / elapsed);

  StringScanner_destroy(scan);
  return NULL;
}

char * all_tests(){

  mu_suite_start();

  mu_run_test(test_find_scan);
  mu_run_test(test_binstr_performance);
  mu_run_test(test_find_performance);
  mu_run_test(test_scan_performance);

  return NULL;
}

RUN_TESTS(all_tests);
