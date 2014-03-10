#include "tests/lib.h"

int main(void)
{
  int* a = (int*) malloc(2*sizeof(int));
  a[0] = 10;
  a[1] = 20;

  printf("a[0] = %d, a[1] = %d\n",a[0],a[1]);
  free(a);
  return 0;
}
