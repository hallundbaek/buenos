#include <stdio.h>
#include <stdlib.h>
#include "dllist.h"

void insert(dlist *this, item* thing, bool atTail){
  node *newNode = malloc(sizeof(node));
  newNode->thing = thing;
  if (this -> head == NULL) {
    this -> head = newNode;
    this -> tail = newNode;
    return;
  }
  if (atTail) {
    this -> tail -> ptr = (node*) ((long) this -> tail -> ptr ^ (long) newNode);
    newNode->ptr = this -> tail;
    this -> tail = newNode;
  } else {
    this -> head -> ptr = (node*) ((long) this -> head -> ptr ^ (long) newNode);
    newNode ->ptr = this -> head;
    this -> head = newNode;
  } 
}

item* extract(dlist *this, bool atTail){
  item* returnedItem;
  if (atTail) {
    returnedItem = this -> tail -> thing;    
    node* nextNode = this -> tail -> ptr;
    if (nextNode != NULL) {
      nextNode -> ptr = (node*) ((long) nextNode -> ptr ^ (long) this -> tail);
      free(this -> tail);
      this -> tail = nextNode;
    } else {
      free(this -> tail);
      this -> tail = NULL;  
    }
  } else {
    returnedItem = this -> head -> thing;    
    node* nextNode = this -> head -> ptr;
    if (nextNode != NULL) {
      nextNode -> ptr = (node*) ((long) nextNode -> ptr ^ (long) this -> head);
      free(this -> head);
      this -> head = nextNode;
    } else {
      free(this -> head);
      this -> head = NULL;  
    }
  }
  return returnedItem;
}

void reverse(dlist *this){
  node* tailNode = this -> tail;
  this -> tail = this -> head;
  this -> head = tailNode;
}

item* peek(dlist *this, bool atTail){
  if (atTail) {
    return this -> tail -> thing;
  } else {
    return this -> head -> thing;
  }
}
/*
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

int main() {
  dlist list = {0};
  int i = 5;
  bool j = 0;
  insert(&list, &i, j);
  int l = 6;
  insert(&list, &l, j);
  bool (*match)(item*) = find;
  int* k = search(&list, match);
  int f = 9;
  insert(&list, &f, j);
  reverse(&list);
  printf("%d\n", *k);
  j = 0;
  k = extract(&list, j);
  printf("%d,", *k);
  j = 0;
  k = extract(&list, j);
  printf("%d,", *k);
  k = extract(&list, j);
  printf("%d", *k);
}*/
