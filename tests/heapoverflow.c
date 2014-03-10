#include "tests/lib.h"



int main(void)
{
  /* Attempts to allocate more physical memory than
   * the TLB can hold.
   */
  int* overflow = malloc(4096*17*4);
  overflow [0] = 1;
  return 0;
}
