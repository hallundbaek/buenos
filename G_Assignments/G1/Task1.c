#include <stdio.h>
#include <stdlib.h>

typedef int bool;
typedef void item; /* we store pointers */

typedef struct node_ {
  item *thing;
  struct node_ *ptr;
} node;

typedef struct dlist_ {
  node *head, *tail;
} dlist;

void insert(dlist *this, item *thing, bool atTail) { 
  node* n = (node*) malloc(sizeof(node));
  (*n).thing = thing;
  (*n).ptr = n;
  if((*this).head == NULL) {     
    (*this).tail = n;
    (*this).head = n;
    return;
  }
  node* dlistNode = (atTail) ? (*this).tail : (*this).head;  
  dlistNode -> ptr = (node*) ((long) dlistNode -> ptr ^ (long) n); 
  (*n).ptr = dlistNode;
  (atTail) ? ((*this).tail = n) : ((*this).head = n);
}

item* extract(dlist *this, bool atTail) {
  node* returnNode = (*this).head;
  item* value;
  if(returnNode == ((*returnNode).ptr)) {
    (*this).tail = NULL;
    (*this).head = NULL;
    value = (*returnNode).thing;
    free(returnNode);
    return value;
  }
  if (atTail) {
    returnNode = (*this).tail;
    (*this).tail = (*returnNode).ptr;
    (*this).tail -> ptr = (node*) ((long)(*this).tail -> ptr ^ (long) returnNode);
    value = (*returnNode).thing;
    free(returnNode);
    return value;
  }  
  (*this).head = (*returnNode).ptr;
  (*this).head -> ptr = (node*) ((long)(*this).head -> ptr ^ (long) returnNode);
  value = (*returnNode).thing;
  free(returnNode);
  return value;
}

void reverse (dlist *this) {
  void* temp = ((*this).head);
  (*this).head = (*this).tail;
  (*this).tail = temp;
}

item* search(dlist *this, bool (*matches)(item*)){
  node* itNode = this -> head;
  node* oldPtr = (node*) 0;
  node* tempPtr;
  if (matches(itNode -> thing)) {
    return itNode -> thing;
  }
  while (itNode != this -> tail) {
    tempPtr = itNode;
    itNode = (node*) ((long) itNode -> ptr ^ (long) oldPtr);
    oldPtr = tempPtr;
    if (matches(itNode -> thing)) {
      return itNode -> thing;
    }
  }
  return NULL;
}

bool find(item* thing) { 
  return (*((int*)thing) == 5);
 }

void main() {
  dlist list = {0};
  int i = 5;
  int i1 = 6;
  int i2 = 7;
  bool b = 0;
  bool b1 = 1;
  insert(&list,&i,b);
  insert(&list,&i1,b);
  insert(&list,&i2,b);
  bool (*match)(item*) = find;
  int* s = search(&list,match);
  printf("The search found:%2d\n",*s);
  int* p = extract(&list,b);
  printf("%d\n",*p);
  int* p1 = extract(&list,b);
  printf("%d\n",*p1);
  int* p2 = extract(&list,b);
  printf("%d\n",*p2);
}
