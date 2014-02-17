#include "kernel/write.h"
#include "drivers/gcd.h"
#include "drivers/device.h"

int write_kernel(int fhandle, const void *buffer, int length){
  int len;
  device_t *dev;
  gcd_t *gcd;

  /* Checks that the file handle is the write code (1)
   * otherwise return 0 since we wont write any characters.
   * fhandle itself is not needed at this point of implementation.
   */
  if (fhandle != 1) {
    return 0;
  }

  // Get the default TTY and generic device driver
  dev = device_get(YAMS_TYPECODE_TTY, 0);
  gcd = (gcd_t *)dev->generic_device;
  // Write to the buffer
  len = gcd->write(gcd, buffer, length);
  return len;
}
