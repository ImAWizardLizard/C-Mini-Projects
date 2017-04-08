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

  // Either use copy (List_push) method or just have pointers at either ends of
  // the lists be reassigned
  //
  // downsides of copying is that it takes longer, depending on the size of the
  // list, whilst just altering the pointers would change the reference points,
  // not having to loop the whole list, the downside to altering the pointers
  // is that list_2 still has access to the joined list, which is what is not
  // intended for as altering someting in list_2 could alter something in
  // list_2 and vice versa

  // copy method

  LIST_FOREACH(list_2,first,next,cur){

    List_push(list_1,cur->value);

  }

  // pointer method 

  /*
  list_1->last->next = list_2->first;
  list_2->first->prev = list_1->last;

  list_1->last = list_2->last;

  list_1->count += list_2->count;
  */

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

void List_split(List *list,List *list_2){

  check(list != NULL,"Invalid list");
  check(list->count > 1,"List is not big enough to split.");

  size_t length = (int)((list->count/2));
  size_t index = 1;
  
  LIST_FOREACH(list,first,next,cur){
    
   if(index == length){

    list_2->first = cur->next;
    list_2->last = list->last;
    list_2->count = list->count - length;

    list->last = cur;
    list->last->next = NULL;
    list->count = length;

    break;
   } 

   index++; 
  }


  // if size of either nodes is one, then makes it itself its own first node
  // and last node
  // must re-assign counts, meant to be inefficent i guess
  

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
