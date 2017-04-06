#include <stdio.h>
#include <lcthw/dbg.h>
#include <lcthw/list_algos.h>


void List_swap(ListNode *node_1,ListNode *node_2){

  void *temp_value = node_1->value;
  node_1->value = node_2->value;
  node_2->value = temp_value;

}

int List_bubble_sort(List *words,List_compare cmp){

  check(words != NULL,"List is invalid.");

  if(List_count(words) <= 1){
    return 0;
  }

  int sorted = 1;

  do{
    sorted = 1;
    LIST_FOREACH(words,first,next,cur){
      if(cur->next){
        if(cmp(cur->value,cur->next->value) > 0){
          List_swap(cur,cur->next);         
          sorted = 0;
        }
      }
    }
  }while(!(sorted));

  return 0;

error:
  return 1;
}


List *List_merge(List *left, List *right){




}

List *List_merge_sort(List *words, List_compare cmp){

  check(words != NULL,"List is invalid.");

  int i = 0;
  int length = words->count;
  if(length <=1) return words;

  List *left = calloc(1,sizeof(List));
  List *right = calloc(1,sizeof(List));

  LIST_FOREACH(words,first,next,cur){
    if(i < (length/2)){
      List_push(left,cur->value);
    }else{
      List_push(right,cur->value);
    }
    i++;
  }

  List_merge_sort(left,cmp);
  List_merge_sort(right,cmp);


  return List_merge(left,right);

error:
  return NULL;

