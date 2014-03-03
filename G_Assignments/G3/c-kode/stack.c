#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "dllist.h"

pthread_mutex_t         mutex = PTHREAD_MUTEX_INITIALIZER;

void stack_init(stack_t* stack) {
  pthread_mutex_init(&mutex, NULL);
  dlist *list = malloc(sizeof(dlist));
  stack->data = list;
  stack->top = -1;
}

int stack_empty(stack_t* stack) {
  pthread_mutex_lock(&mutex);
  int retval = (stack->top == -1);
  pthread_mutex_unlock(&mutex);
  return retval;
}

void* stack_top(stack_t* stack) {
  pthread_mutex_lock(&mutex);
  if (stack->top == -1) {
    pthread_mutex_unlock(&mutex);
    return (void*) 0;
  }
  void* retval = peek(stack->data,0);
  pthread_mutex_unlock(&mutex);
  return retval;
}

void* stack_pop(stack_t* stack) {
  pthread_mutex_lock(&mutex);
  if (stack->top == -1) {
    pthread_mutex_unlock(&mutex);
    return (void*) 0;
  }
  stack->top--;
  void* retval = extract(stack->data,0);
  pthread_mutex_unlock(&mutex);
  return retval;
}

int stack_push(stack_t* stack, void* data) {
  pthread_mutex_lock(&mutex);
  stack->top++;
  insert(stack->data,data,0);
  pthread_mutex_unlock(&mutex);
  return 1;
}
