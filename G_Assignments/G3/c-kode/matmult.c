/* Matrix multiplication row-wise in separate threads
 *
 * Compile with:
 *   gcc -Wall -Wextra -std=c99 -pedantic -o matmult matmult.c stack.c dllist.c -lpthreads
 *
 * Usage:
 *   ./PThread_matmult [SIZE (default = 100)] [NUMBER OF THREADS (default = 4)]
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "stack.h"
#include <sys/time.h>

#define DEFAULT_SIZE 100
#define DEFAULT_THREADS_SIZE 4
/* Data structure to drive a thread */
typedef struct {
  double *row_a;
  double *matrix_b;
  int a_length, b_columns;
  double *row_result;
} ttask_t;

stack_t stack;

/* Zero the result row of doubles in memory, compute dot product of row A with
 * all columns of B, return.
 */
void* rowmult(void *arg) {
  int i, j;
  ttask_t *t;

  t = (ttask_t*) arg;

  for (j = 0; j < t->b_columns; j++) {
    t->row_result[j] = 0.0;
  }
  for (i = 0; i < t->a_length; i++) {
    for (j = 0; j < t->b_columns; j++) {
      t->row_result[j] += t->row_a[i] * t->matrix_b[i * (t->b_columns) + j];
    }
  }
  return NULL;
}

void* worker(void *param) {
  param = param;
  ttask_t *t;
  while (1) {
    t = stack_pop(&stack);
    if (!t) {
      break;
    }
    rowmult(t);
  }
  return NULL;
}

/* Output a matrix of dimensions size x size. */
void output_square_matrix(double *m, int size) {
  int i, j;
  
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      printf("%3.2f  ", m[i * size + j]);
    }
    putchar('\n');
  }
}

/* Main program: Fill two matrices (randomly or with particular patterns; for
 * instance diagonals to "mirror" a matrix or some blocks of it), then create
 * one thread per row to multiply. Join all threads, then output the matrix (or
 * just declare success).
 *
 * Parameters: matrix size
 */
int main(int argc, char* argv[]) {
  int i, j;
  int size,no_of_threads;

  double *matrices, *a, *b, *r;

  pthread_t *threads;
  ttask_t *tasks, *t;
  char* num_end;

  /* Initialize stack */
  stack_init(&stack);
  /* Find out which size to compute with. */
  if (argc > 1) {
    size = strtol(argv[1], &num_end, 10);
    if (num_end[0] != '\0') {
      fprintf(stderr, "argument not a number: %s\n", argv[1]);
      exit(EXIT_FAILURE);
    }
    if (size < 1) {
      fprintf(stderr, "size negative or zero: %d\n", size);
      exit(EXIT_FAILURE);
    }
  }
  else {
    size = DEFAULT_SIZE;  
  }
  if (argc > 2) {
    no_of_threads = strtol(argv[2], &num_end, 10);
    if (num_end[0] != '\0') {
      fprintf(stderr, "argument not a number: %s\n", argv[2]);
      exit(EXIT_FAILURE);
    }
    if (no_of_threads < 1) {
      fprintf(stderr, "threads negative or zero: %d\n", no_of_threads);
      exit(EXIT_FAILURE);
    }
  } else {
    no_of_threads = DEFAULT_THREADS_SIZE;
  }
  printf("Number of threads: %d\n",no_of_threads);
  printf("Multiplying random square matrices of size %d x %d\n",
         size, size);
 /* Allocate memory for both input matrices and the result matrix. */
  matrices = (double*) malloc(sizeof(double) * size * size * 3);
  if (matrices == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  a = matrices;
  b = matrices + size * size;
  r = matrices + size * size * 2;
  /* Allocate memory for threads and thread structures. */
  threads = (pthread_t*) malloc(no_of_threads * sizeof(pthread_t));
  if (threads == NULL) {
    perror("malloc");
    free(matrices);
    exit(EXIT_FAILURE);
  }
  tasks = (ttask_t*) malloc(size * sizeof(ttask_t));
  if (tasks == NULL) {
    perror("malloc");
    free(matrices);
    free(threads);
    exit(EXIT_FAILURE);
  }
   /* Fill matrices with values. a is random, b is diagonal. */
  srand(time(NULL)); /* A very bad RNG, but standard */

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      a[i * size + j] = (double) rand();
      b[i * size + j] = 0.0;
    }
    b[i * size + (size - i - 1)] = 1.0; /* second diagonal */
  }

  /* If size is small, output matrices. */
  if (size < 20) {
    printf("\nMatrix A:\n");
    output_square_matrix(a, size);
    printf("\nMatrix B (diagonal):\n");
    output_square_matrix(b, size);
  }
  
  struct timeval start, end;
  long mtime, secs, usecs;
  gettimeofday(&start, NULL);
  /* Add the threads to the stack. */
  for (i = 0; i < size; i++) {
    t = &tasks[i];
    t->row_a = a + i * size;
    t->matrix_b = b;
    t->a_length = size;
    t->b_columns = size;
    t->row_result = r + i * size;
    stack_push(&stack, t);
  }
  /* Start the threads. */
  for (i = 0; i < no_of_threads; i++) {
    if (pthread_create(&threads[i], NULL, worker, NULL) != 0) {
      perror("pthread_create");
      free(matrices);
      free(threads);
      free(tasks);
      exit(EXIT_FAILURE);
    }
  }
  /* Wait for the threads to finish. */
  for(i = 0; i < no_of_threads; i++) {
    pthread_join(threads[i], NULL);
    putchar('.');
  }
  putchar('\n');
  /* If size is small, output result matrix. */
  if (size < 20) {
    printf("\nResult:\n");
    output_square_matrix(r, size);
  } 
  gettimeofday(&end, NULL);
  secs  = end.tv_sec  - start.tv_sec;
  usecs = end.tv_usec - start.tv_usec;
  mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
  printf("\nFinished in %lu milliseconds!\n", mtime);

  free(matrices);
  free(threads);
  free(tasks);
  free(stack.data); 
  exit(EXIT_SUCCESS);
}
