#include <stdio.h>
#include <lcthw/dbg.h>
#include <lcthw/list_algos.h>


void List_swap(ListNode *node_1,ListNode *node_2){

  ListNode *temp_node = node_1;


  node_1 = node_2;
  node_2 = temp_node;

}

int List_bubble_sort(List *words,List_compare cmp){

  check(words != NULL,"List is invalid.");
  size_t n = words->count;
  size_t i = 0;

  // least optimized algorithm
  swapped = 0;

  while(!(swapped)){
    LIST_FOREACH(words,first,next,cur){
      cur = cur->next;
      if(cmp(cur->prev,cur) > 0){
        List_swap(cur->prev,cur);         
        swapped = 1;
      }
    }
  }


  return 0;

error:
  return 1;
}

