#include "tests/lib.h"

int main(void)
{
  char buffer[1];
  char newline[1] = {'\n'};
  char larger[12] = {'k',' ','i','s',' ','l','a','r','g','e','r','\n'};
  char smaller[13] = {'k',' ','i','s',' ','s','m','a','l','l','e','r','\n'};
  char correct[8] = {'c','o','r','r','e','c','t','\n'};
  int l, len;
  int k = 32;
  while (1) {
    syscall_read(0,buffer,1);
    syscall_write(1,buffer,1);
    l = (buffer[0] - '0')*10;
    syscall_read(0,buffer,1);
    syscall_write(1,buffer,1);
    syscall_write(1,newline,1);
    l += (buffer[0] - '0');
    if (l < k) {
      len = 12;
      syscall_write(1, larger, len);
    } else if (l > k) { 
      len = 13;
      syscall_write(1, smaller, len);
    } else {
      len = 8;
      syscall_write(1, correct, len);
      break;
    }
  }
  syscall_halt();
  return 0;
}
