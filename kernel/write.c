#include "kernel/write.h"
#include "drivers/gcd.h"
#include "drivers/device.h"

int write_kernel(int fhandle,const void *buffer, int length){
  int len;
  if (fhandle != 1) {
    return 0;
  }
  device_t *dev;
  gcd_t *gcd;
  dev = device_get(YAMS_TYPECODE_TTY, 0 );
  gcd = (gcd_t *)dev->generic_device;
  len = gcd->write(gcd, buffer, length);
  return len;
}
