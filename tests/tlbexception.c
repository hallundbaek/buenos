#include "tests/lib.h"



int main(void)
{
  const int c = 10;
  c = 40;
  kprintf("c is: %d",c);
  return 0;
}
