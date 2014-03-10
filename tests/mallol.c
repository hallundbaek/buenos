#include "tests/lib.h"

int main(void)
{
  malloc(20);
  syscall_exit(2);
  return 0;
}
