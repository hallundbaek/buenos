#include "tests/lib.h"

int main(void)
{
  int* a = (int*) malloc(2*sizeof(int));
  int i;
  a[0] = 10;
  a[1] = 20;

  printf("a[0] = %d, a[1] = %d\n",a[0],a[1]);
  free(a);

  int* b = (int*) malloc(1100 * sizeof(int));
  for(i=0; i <1100;i++) {
  b[i] = i;
  }

  printf("b[0] = %d, b[200] = %d\nb[598] = %d, b[1099]\n",
          b[0],b[200],b[598],b[1099]);

  return 0;
}
