#ifndef lcthw_list_algos_h
#define lcthw_list_algos_h

#include <stdlib.h>
#include <lcthw/list.h>

typedef int (*List_compare)(const void *a,const void *b);

void List_swap(ListNode *node_1,ListNode *node_2);

// Sorting algorithms
int List_bubble_sort(List *words,List_compare cmp);
List *List_merge_sort(List *words,List_compare cmp);

// Insert node into list which automatically places it in the list
int List_insert_sorted(List *words,ListNode *node,List_compare cmp);

#endif
