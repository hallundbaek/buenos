#include "kernel/read.h"
#include "drivers/gcd.h"
#include "drivers/device.h"

int read_kernel(int fhandle, void *buffer, int length){
  int len;
  if (fhandle != 0) {
    return 0;
  }
  device_t *dev;
  gcd_t *gcd;
  dev = device_get(YAMS_TYPECODE_TTY, 0 );
  gcd = (gcd_t *)dev->generic_device;
  len = gcd->read(gcd, buffer, length);
  return len;
}
