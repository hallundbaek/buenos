#include "tests/lib.h"

int main(void)
{
  int cid = fork();
  char button[1];
  
  syscall_halt();
  return 0;
}
