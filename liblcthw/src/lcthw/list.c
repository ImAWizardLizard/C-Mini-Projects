#include <lcthw/list.h>
#include <lcthw/dbg.h>
#include <math.h>

List *List_create(){

  return calloc(1,sizeof(List));

}

void List_destroy(List *list){

  check(list != NULL,"Invalid list.");

  LIST_FOREACH(list,first,next,cur){
    if(cur->prev){
      free(cur->prev);
    }

    free(cur->value);
  }

  free(list->last);
  free(list);

error:
  return;

}


void List_push(List *list, void *value){
  
  check(list != NULL,"List is invalid.");
  ListNode *node = calloc(1,sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if(list->last == NULL){
    list->first = node;
    list->last = node;
  }else{
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }

  list->count++;


error:
  return;

}

void *List_pop(List *list){

  check(list != NULL,"List is invalid.");
  check(list->count > 0 ,"List count is zero.");

  ListNode *node = list->last;
  return node != NULL ? List_remove(list,node): NULL;

error:
  return NULL;
}

void List_unshift(List *list,void *value){

  check(list != NULL,"List is invalid.");

  ListNode *node = calloc(1,sizeof(ListNode));

  check_mem(node);
  node->value = value;

  if(list->first == NULL){
    list->first = node;
    list->last = node;
  }else{
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  }

  list->count++;

error:
  return;
}

void *List_shift(List *list){

  check(list != NULL,"List is invalid.");
  check(list->count > 0 && list->first != NULL ,"List count is zero.");

  ListNode *node = list->first;
  return node != NULL ? List_remove(list,node): NULL;
error:
  return NULL;
}

void List_join(List *list_1,List *list_2){

  check(list_1 != NULL && list_2 != NULL,"Lists is invalid.");
  check(list_1->count > 0 && list_2->count > 0,"List counts are zero");

  LIST_FOREACH(list_2,first,next,cur){
    List_push(list_1,cur->value);
  }

error:
  return;
}

void List_insert(List *list,size_t index,void *value){

  check(list != NULL,"Invalid list");
  ListNode *node = calloc(1,sizeof(ListNode));
  check_mem(node);

  node->value = value;

  size_t count = 1;

  LIST_FOREACH(list,first,next,cur){

    if(count == index){
      node->next = cur;
      node->prev = cur->prev;

      cur->prev->next = node;
      cur->prev = node;
    }
    count++;
  }

  list->count++;


error:
  return;
}

void List_split(List *list,List *list_1,List *list_2){

  check(list != NULL,"Invalid list");
  check(list_1 != NULL && list_2 != NULL,"Invalid lists");
  check(list->count > 1,"List is not big enough to split.");

  int middle = List_count(list)/2;
  
  LIST_FOREACH(list,first,next,cur){
    
   if(middle > 0){
    List_push(list_1,cur->value);
   }else{
    List_push(list_2,cur->value);
   }
    middle--;
  }

error:
  return;

}

void List_values(List *list){

  check(list != NULL,"List is invalid.");
  check(list->count > 0,"Count is less than 0, cannot loop through zero length list");

  printf("\n List count is equal to %d\n",list->count);
  int count = 0;
  LIST_FOREACH(list,first,next,cur){
    printf("Index is %d, value is %s\n",count,(char *)cur->value);
    count++;
  }

error:
  return;

}

void List_copy(List *to, List *from){

  LIST_FOREACH(from,first,next,cur){

    List_push(to,cur->value);

  }

}

void *List_remove(List *list,ListNode *node){

  void *result = NULL;
  check(list != NULL,"Invalid list.");
  check(list->first && list->last,"List is empty");
  check(node,"node cant be NULL");

  if(node == list->first && node == list->last){
    list->first = NULL;
    list->last = NULL;
  }else if(node == list->first){
    list->first = node->next;
    check(list->first != NULL,"Invalid list, somehow got a NULL first");
    list->first->prev = NULL;
  }else if(node == list->last){
    list->last = node->prev;
    check(list->last != NULL,"Invalid list, somehow got a next which is NULL.");
    list->last->next = NULL;
  }else{
    ListNode *after = node->next;
    ListNode *before = node->prev;

    after->prev = before;
    before->next = after;
  }

  list->count--;
  result = node->value;
  free(node);


error:
  return result;
}
