#include "kernel/semaphore.h"
#include "kernel/config.h"
#include "kernel/interrupt.h"
#include "kernel/kmalloc.h"
#include "proc/usr_semaphore.h"
#include "lib/libc.h"
#define MAX_USR_SEMAPHORES CONFIG_MAX_SEMAPHORES / 8

/* Configure a separate userland semaphore table
 * - this will never be fully populated as long as there
 * are active kernel semaphores. */
static usr_sem_t usr_semaphore_table[MAX_USR_SEMAPHORES];

static spinlock_t usr_semaphore_table_slock;

void usr_semaphore_init(void)
{
  int i;
  interrupt_status_t intr_status;
  intr_status = _interrupt_disable();
  spinlock_reset(&usr_semaphore_table_slock);
  spinlock_acquire(&usr_semaphore_table_slock);
  for(i = 0; i < MAX_USR_SEMAPHORES; i++) {
    usr_semaphore_table[i].sem = NULL;
  }
  spinlock_release(&usr_semaphore_table_slock);
  _interrupt_set_state(intr_status);
}

usr_sem_t* syscall_sem_open(char const *name, int value)
{
  usr_sem_t *usr_sem = NULL;
  interrupt_status_t intr_status;
  int i;
  if (strlen(name) > MAX_NAME_LEN) {
    return NULL;
  }
  intr_status = _interrupt_disable();
  spinlock_acquire(&usr_semaphore_table_slock);
  // Get an existing userland semaphore
  if (value < 0) {
    for (i = 0; i < MAX_USR_SEMAPHORES; i++) {
      if(stringcmp(usr_semaphore_table[i].name, name) == 0) {
        usr_sem = &usr_semaphore_table[i];
        spinlock_release(&usr_semaphore_table_slock);
        _interrupt_set_state(intr_status);
        return usr_sem;
      }
    }
    spinlock_release(&usr_semaphore_table_slock);
    _interrupt_set_state(intr_status);
    return NULL; // No semaphore with given name exists
  }
  // Create new userland semaphore
  else {
    int sem_id = MAX_USR_SEMAPHORES;
    for (i = 0; i < MAX_USR_SEMAPHORES; i++) {
      if(stringcmp(usr_semaphore_table[i].name, name) == 0) {
        spinlock_release(&usr_semaphore_table_slock);
        _interrupt_set_state(intr_status);
        return NULL; // Semaphore already exists
      }
      if (i < sem_id && usr_semaphore_table[i].sem == NULL) {
        sem_id = i;
      }
    }
    usr_semaphore_table[sem_id].sem = semaphore_create(value);
    stringcopy(usr_semaphore_table[sem_id].name, name, MAX_NAME_LEN);
    usr_sem = &usr_semaphore_table[sem_id];
    spinlock_release(&usr_semaphore_table_slock);
    _interrupt_set_state(intr_status);
    return usr_sem;
  }
  // Something went wrong
  spinlock_release(&usr_semaphore_table_slock);
  _interrupt_set_state(intr_status);
  return NULL;
}

int syscall_sem_p(usr_sem_t* handle)
{
  semaphore_P(handle->sem);
  return 0;
}

int syscall_sem_v(usr_sem_t* handle)
{
  semaphore_V(handle->sem);
  return 0;
}

int syscall_sem_destroy(usr_sem_t* handle)
{
  int i;
  interrupt_status_t intr_status;

  intr_status = _interrupt_disable();
  spinlock_acquire(&usr_semaphore_table_slock);

  /* Look through the user semaphore table for
   * a usr_sem_t address matching the handle */
  for(i = 0; i < MAX_USR_SEMAPHORES; i++) {
    if(&usr_semaphore_table[i] == handle) {
      semaphore_destroy(handle->sem);
      usr_semaphore_table[i].sem = NULL;
      spinlock_release(&usr_semaphore_table_slock);
      _interrupt_set_state(intr_status);
      return 0;
    }
  }
  // If no match is found, return an error
  spinlock_release(&usr_semaphore_table_slock);
  _interrupt_set_state(intr_status);
  return 1;
}
