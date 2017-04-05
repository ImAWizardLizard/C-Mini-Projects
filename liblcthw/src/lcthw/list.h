#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>


struct ListNode;

// A ListNode which is what a List is comprised of
typedef struct ListNode{
  struct ListNode *next;
  struct ListNode *prev;
  void *value;
} ListNode;


// A list which contains the nodes, having specific pointers to the first and
// last nodes and the # which are in it
typedef struct List{
  int count;
  ListNode *first;
  ListNode *last;
} List;

// List_create function which returns memory the size of a List
List *List_create();

// function which free's the memory allocated for the list and  clears its
// values
void List_clear_destroy(List *list);


// macro functions which return basic values from a list
#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value: NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value: NULL)

// A function which pushes a node to the front of a list, with its specified
// value
void List_push(List *list, void *value);

// A function which removes the last node off a list
void *List_pop(List *list);

// A function which unshifts(inserts) a node to the front of a list
void List_unshift(List *list, void *value);

// A function which removes the first node from a list
void *List_shift(List *list);

// A function which inserts a node at a certain index
void List_insert(List *list,size_t index, void *value);

// join list_2 onto list_1 or essentially combine two lists
void List_join(List *list_1, List *list_2);

// A function which given a list, it will put the first half in list and the
// second half in list_2, list_2 should be empty
void List_split(List *list, List *list_2);

// copys a list
void List_copy(List *to,List *from);

// Loops through and prints out all of the values of a list
void List_values(List *list);

// Removes a specific node from a list
void *List_remove(List *list, ListNode *node);

// Iterable code which is just a boilerplate for-loop
#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
                                                   ListNode *V = NULL;\
for(V = _node = L->S; _node != NULL; V = _node = _node->M)\

#endif
