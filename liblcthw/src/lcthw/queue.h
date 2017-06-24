#ifndef _lcthw_queue_h
#define _lcthw_queue_h

#include <lcthw/list.h>

typedef List Queue;

#define Queue_create List_create
#define Queue_destroy List_destroy
#define Queue_clear List_clear
#define Queue_clear_destroy List_clear_destroy

#define Queue_send List_unshift
#define Queue_recv List_pop

#define Queue_peek List_first
#define Queue_count List_count

#define QUEUE_FOREACH(Q, C) LIST_FOREACH(S,last,prev,C)

#endif
