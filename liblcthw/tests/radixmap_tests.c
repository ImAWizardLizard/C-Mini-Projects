#include "minunit.h"
#include <lcthw/radixmap.h>
#include "timer.h"

static clock_t start, end;

// TODO: implement timing unit test
static int make_random(RadixMap * map){
  size_t i = 0;

  for(i = 0; i < map->max - 1; i++){
    uint32_t key = (uint32_t) (rand() | (rand() << 16)); // must bit shift to turn into 32 bit integer from 16 bit
    check(RadixMap_add(map,key,i) == 0,"Failed to add key %u.",key);
  }
  return i;
error:
  return 0;
}

static int check_order(RadixMap * map){

  RMElement d1, d2;
  unsigned int i = 0;

  // only signal errors if any(should not be)
  for(i = 0; map->end > 0 && i < map->end -1;i++){
    d1 = map->contents[i];
    d2 = map->contents[i + 1];

    if(d1.data.key > d2.data.key){
      printf("FAIL:i=%u, key=%u, value=%u, equals max? %s\n",i,d1.data.key,d1.data.value,d2.data.key == UINT32_MAX ? "yes" : "no");
      printf("FAIL:%u > %u\n",d1.data.key,d2.data.key);
      return 0;
    }
  }

  return 1;

}

static int test_find(RadixMap * map){

  unsigned int i = 0;
  RMElement *d = NULL;
  RMElement *found = NULL;

  for(i = map->end / 2;i< map->end; i++){
      d = &map->contents[i];

      found = RadixMap_find(map,d->data.key);
      check(found != NULL,"Didn't find %u at %u",d->data.key,i);
      check(found->data.key == d->data.key,"Got the wrong result: %p:%u looking for %u at %u",
          found, found->data.key,d->data.key,i);
  }
  return 1;
error:
  return 0;
}

static void print_map(RadixMap * map){

  int i = 0;
  for(i = 0;i <= map->end - 1;i++){
    printf("%u ",map->contents[i].data.key);
  }

  printf("\n");

}



static char *test_operations(){

  //////// SETUP ////////
  
  size_t N = 5000;

  RadixMap * sorted_map;
  RadixMap * radixsort_map;
  RadixMap * quicksort_map = calloc(1,sizeof(RadixMap));
  RadixMap * heapsort_map = calloc(1,sizeof(RadixMap));
  RadixMap * mergesort_map = calloc(1,sizeof(RadixMap));

  
  // Time the creation of the radix map  
  timer_start(&start);
  radixsort_map = RadixMap_create(N);
  timer_end(&start,&end,"creating the radix map");

  mu_assert(radixsort_map != NULL,"Failed to make map.");

  // Time adding data to the radix map
  timer_start(&start);
  mu_assert(make_random(radixsort_map), "Didn't make random fake radix map.");
  timer_end(&start,&end,"randomizing the map");

  // Check the order of the radix map after adding data
  timer_start(&start);
  mu_assert(check_order(radixsort_map),"Failed to properly sort radix map after adding data");
  timer_end(&start,&end,"checking the order");

  // Make copies of original radixsort_map
  *quicksort_map = *radixsort_map;
  *heapsort_map = *radixsort_map; 
  *mergesort_map = *radixsort_map;

  mu_assert(quicksort_map != NULL && heapsort_map != NULL && mergesort_map != NULL,"Failed to make map.");

  //////// SETUP ////////

  printf("\n");

  /////////// Time sorting the list using quicksort ///////////
  
  // Sorting the map
  timer_start(&start);
  sorted_map = RadixMap_quicksort(quicksort_map,0,quicksort_map->end - 1);
  mu_assert(sorted_map != NULL, "Failed to properly sort the quicksort map");
  timer_end(&start,&end,"sorting the map using quicksort");

  // Checking the order
  timer_start(&start);
  mu_assert(check_order(sorted_map),"Failed to properly sort quicksort map");
  timer_end(&start,&end,"checking the order");

  /////////// Time sorting the list using quicksort ///////////

  printf("\n");

  /*
  /////////// Time sorting the list using heapsort ///////////
  
  // Sorting the map
  timer_start(&start);
  RadixMap_heapsort(heapsort_map, heapsort_map->end);
  mu_assert(heapsort_map != NULL, "Failed to properly sort the heaposrt map");
  timer_end(&start,&end,"sorting the map using heapsort");

  // Checking the order
  timer_start(&start);
  mu_assert(check_order(heapsort_map),"Failed to properly sort heapsort map");
  timer_end(&start,&end,"checking the order");

  /////////// Time sorting the list using heapsort ///////////
  //
  printf("\n");

  ////////// Time sorting the list using mergesort ///////////
  */
  
  // Sorting the map
  timer_start(&start);
  sorted_map = RadixMap_mergesort(mergesort_map);
  mu_assert(sorted_map != NULL, "Failed to properly sort the mergesort map");
  timer_end(&start,&end,"sorting the map using mergesort");

  // Checking the order
  timer_start(&start);
  mu_assert(check_order(sorted_map),"Failed to properly sort mergesort map");
  timer_end(&start,&end,"checking the order");

  ////////// Time sorting the list using mergesort ///////////

  printf("\n"); 

  ///////// Time sorting the list using radixsort ///////////
  
  // Sorting the map
  timer_start(&start);
  RadixMap_sort(radixsort_map,0);
  timer_end(&start,&end,"sorting the map using radixsort");
  
  // Check the order
  timer_start(&start);
  mu_assert(check_order(radixsort_map),"Failed to properly sort radix map");
  timer_end(&start,&end,"checking the order after sorting");

  ///////// Time sorting the list using radixsort ///////////

  printf("\n");
  
  ///////// Time searching the radixsort_map ///////////

  // Searching the map
  timer_start(&start);
  mu_assert(test_find(radixsort_map),"Failed the search test");
  timer_end(&start,&end,"finding elements");
  
  // check the order of the radix map after searching
  timer_start(&start);
  mu_assert(check_order(radixsort_map),"Map didn't stay sorted after search");
  timer_end(&start,&end,"checking the order after finding elements");

  ///////// Time searching the radixsort_map ///////////
  

  /////// Time seaching and deleting elements ////////
  /*
  timer_start(&start); 
  while(radixsort_map->end > 0){
    
    RMElement *el = RadixMap_find(radixsort_map,radixsort_map->contents[radixsort_map->end / 2].data.key);
    mu_assert(el != NULL,"Shouldn't get a result");

    size_t old_end = radixsort_map->end;

    mu_assert(RadixMap_delete(radixsort_map,el) == 0,"Could not delete element");
    mu_assert(old_end - 1 == radixsort_map->end,"Wrong size after deletion");

    mu_assert(check_order(radixsort_map),"RadixMap is not sorted after delete");

  }
  timer_end(&start,&end,"finding and deleting");
  */
  /////// Time seaching and deleting elements ////////
  
  printf("\n");

  ////// Time destroying the maps //////
  timer_start(&start); 
  RadixMap_destroy(radixsort_map);
  RadixMap_destroy(quicksort_map);
  RadixMap_destroy(heapsort_map);
  RadixMap_destroy(mergesort_map);
  timer_end(&start,&end,"destroying the maps");
  ////// Time destroying the maps //////

  return NULL;
}

char *all_tests(){

  mu_suite_start();
  srand(time(NULL));

  mu_run_test(test_operations);

  return NULL;
}

RUN_TESTS(all_tests);
