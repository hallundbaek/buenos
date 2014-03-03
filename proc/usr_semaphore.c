#include "kernel/semaphore.h"
#include "kernel/config.h"
#include "kernel/interrupt.h"
#include "kernel/kmalloc.h"
#include "proc/usr_semaphore.h"
#include "lib/libc.h"
#define MAX_USR_SEMAPHORES CONFIG_MAX_SEMAPHORES / 8

<<<<<<< HEAD
/* Configure a separate userland semaphore table of the
 * same size as the kernel semaphore table.
 * Hence, this will never be fully populated as long as
 * there are active kernel semaphores. */
=======
/* Configure a separate userland semaphore table
 * - this will never be fully populated as long as there
 * are active kernel semaphores. */
>>>>>>> 00d379deccf9e83be39ddeb5aebe451b5f281e36
static usr_sem_t usr_semaphore_table[MAX_USR_SEMAPHORES];

static spinlock_t usr_semaphore_table_slock;

void usr_semaphore_init(void)
{
  int i;
  interrupt_status_t intr_status;
  intr_status = _interrupt_disable();
  spinlock_reset(&usr_semaphore_table_slock);
  spinlock_acquire(&usr_semaphore_table_slock);
<<<<<<< HEAD

  // Initiale all userland semaphores to NULL
=======
>>>>>>> 00d379deccf9e83be39ddeb5aebe451b5f281e36
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

  // Get an existing userland semaphore (value < 0)
  if (value < 0) {
<<<<<<< HEAD
    for (i = 0; i < MAX_USR_SEMAPHORESS; i++) {
=======
    for (i = 0; i < MAX_USR_SEMAPHORES; i++) {
>>>>>>> 00d379deccf9e83be39ddeb5aebe451b5f281e36
      if(stringcmp(usr_semaphore_table[i].name, name) == 0) {
        usr_sem = &usr_semaphore_table[i];
        spinlock_release(&usr_semaphore_table_slock);
        _interrupt_set_state(intr_status);
        return usr_sem;
      }
    }
    // No semaphore with given name exists
    spinlock_release(&usr_semaphore_table_slock);
    _interrupt_set_state(intr_status);
    return NULL;
  }
  // Create new userland semaphore (value >= 0)
  else {
    int sem_id = MAX_USR_SEMAPHORES;
    for (i = 0; i < MAX_USR_SEMAPHORES; i++) {
<<<<<<< HEAD
      // Semaphore already exists
=======
>>>>>>> 00d379deccf9e83be39ddeb5aebe451b5f281e36
      if(stringcmp(usr_semaphore_table[i].name, name) == 0) {
        spinlock_release(&usr_semaphore_table_slock);
        _interrupt_set_state(intr_status);
        return NULL;
      }
      // Find an available spot in the userland semaphore table
      if (i < sem_id && usr_semaphore_table[i].sem == NULL) {
        sem_id = i;
      }
    }
    /* If there is no more space in the userland semaphore table,
     * return an error. This should never happen, since the actual
     * kernel semaphore table would be full before this could occur */
    if (sem_id == MAX_USR_SEMAPHORES) {
      return NULL;
    }
    // Create the actual userland semaphore
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
      /* Invalidate the kernel semaphore and set the entry
       * in the userland semaphore table to empty */
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
