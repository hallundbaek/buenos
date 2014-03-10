#include "tests/lib.h"

int main(void)
{
  int i;

  int* a = (int*) malloc(2*sizeof(int));
  a[0] = 10;
  a[1] = 20;

  printf("a[0] = %d, a[1] = %d\n",a[0],a[1]);
  free(a);

  int* b = (int*) malloc(1100*sizeof(int));
  for(i=0; i <650; i++) {
    b[i] = i;
  }

  printf("b[0] = %d, b[200] = %d\n",
          b[0],b[200]);

  free(b);
  return 0;
}
