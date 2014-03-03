#ifndef DLLIST_H
#define DLLIST_H

typedef int bool;
typedef void item;

typedef struct node_ {
  item        *thing;
  struct node_ *ptr;
} node;

typedef struct dlist_ {
  node *head, *tail;
} dlist;

void insert(dlist*, item*, bool);


item* extract(dlist*, bool);


void reverse(dlist*);


item* peek(dlist*, bool);
#endif
