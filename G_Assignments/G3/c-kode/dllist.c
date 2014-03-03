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
