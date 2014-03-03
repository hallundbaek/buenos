#include "kernel/semaphore.h"
#define MAX_NAME_LEN 20

typedef struct {
  semaphore_t* sem;
  char name[MAX_NAME_LEN];
} usr_sem_t;

void usr_semaphore_init(void);
usr_sem_t* syscall_sem_open(char const* name, int value);
int syscall_sem_p(usr_sem_t* handle);
int syscall_sem_v(usr_sem_t* handle);
int syscall_sem_destroy(usr_sem_t* handle);
