/*
 * Userland read/write to file.
 */

#include "tests/lib.h"

static const size_t BUFFER_SIZE = 20;

int main(void)
{
  char name[BUFFER_SIZE];
  char input[BUFFER_SIZE];
  char output[BUFFER_SIZE];
  int count;
  int file;
  puts("Hello, Filesystem!\n\n");
  while (1) {
    printf("Please write name of file (max %d chars): ", BUFFER_SIZE);
    count = readline(name, BUFFER_SIZE);
    if (count == 0) {
      printf("No filename specified");
      break;
    }
    if ((file = syscall_open(name)) < 2) {
      printf("No such file found! creating file.\n");
      if (syscall_create(name,BUFFER_SIZE) < 0) {
        printf("Filename invalid. Aborting.");
        syscall_exit(2);
        return 0;
      }
      printf("Please write input to file (max %d chars): ", BUFFER_SIZE);
      count = readline(input, BUFFER_SIZE);
      file = syscall_open(name);
      syscall_write(file,input, count);
    }
    printf("Reading from file:\n");
    syscall_read(file, output, BUFFER_SIZE);
    syscall_close(file);
    syscall_write(1, output, BUFFER_SIZE);
  }
  puts("Now I shall exit!\n");
  syscall_exit(2);
  return 0;
}
