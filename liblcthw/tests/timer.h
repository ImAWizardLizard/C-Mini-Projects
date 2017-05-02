#undef NDEBUG
#ifndef _timer_h
#define _timer_h

#include <lcthw/dbg.h>
#include <time.h>

static inline void timer_start(clock_t *start){
  *start = clock();
}

static inline void timer_end(clock_t * start, clock_t * end, char * message){
  *end = clock();
  double time_spend = ((double)(*end - *start)) / CLOCKS_PER_SEC;
  printf("Time spent %s was %f seconds\n",message,time_spend);
}

#endif
