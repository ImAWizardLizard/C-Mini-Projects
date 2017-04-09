#include <stdio.h>
#include <lcthw/dbg.h>
#include <lcthw/list_algos.h>


void List_swap(ListNode *node_1,ListNode *node_2){

  check(node_1 != NULL && node_2 != NULL,"Nodes to swap are invalid.");

  void *temp_value = node_1->value;
  node_1->value = node_2->value;
  node_2->value = temp_value;

error: //fall through
  return;
}

int List_bubble_sort(List *words,List_compare cmp){

  check(words != NULL,"List is invalid.");

  if(List_count(words) <= 1){
    return 0;
  }

  int length = List_count(words);
  int index = 0;
  int sorted = 1;

  do{
    index = 0;
    sorted = 1;
    LIST_FOREACH(words,first,next,cur){
      if(index < length){
        index++;
        if(cur->next){
          if(cmp(cur->value,cur->next->value) > 0){
            List_swap(cur,cur->next);         
            sorted = 0;
          }
        }
      }
    }
    length--;
  }while(!(sorted));

  return 0;

error:
  return 1;
}


List *List_merge(List *left, List *right,List_compare cmp){

  List *result = List_create();
  check_mem(result);

  while(List_count(left) > 0 && List_count(right) > 0){

    if(cmp(List_first(left),List_first(right)) <= 0){
      List_push(result,left->first);
      left->count--;
      List_shift(left);
    }else{
      List_push(result,right->first);
      right->count--;
      List_shift(right);
    }
  }

  while(left->count > 0){
    List_push(result,left->first);
    left->count--;
    List_shift(left);
  }

  while(right->count > 0){
    List_push(result,right->first);
    right->count--;
    List_shift(right);
  }

  return result;
error:
  return NULL;
}

List *List_merge_sort(List *words, List_compare cmp){

  check(words != NULL,"List is invalid.");

  if(List_count(words) <= 1) return words;

  List *result = NULL;

  List *left = List_create();
  List *right = List_create();

  check_mem(left);
  check_mem(right);

  int middle = List_count(words)/2;

  LIST_FOREACH(words,first,next,cur){
    if(middle > 0){
      List_push(left,cur->value);
    }else{
      List_push(right,cur->value);
    }

    middle--;
  }

  left = List_merge_sort(left,cmp);
  right = List_merge_sort(right,cmp);

  result = List_merge(left,right,cmp);

  List_destroy(left);
  List_destroy(right);

  return result;

error:
  return NULL;
}
