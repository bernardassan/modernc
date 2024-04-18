#include <stddef.h>
#include <stdio.h>

[[maybe_unused]] static void endian() {
  unsigned val = 0xAABBCCDD;
  unsigned char *valp = (unsigned char *)&val;
  for (size_t i = 0; i < sizeof val; ++i) {
    printf("byte[%zu] : %#.02hhX\n", i, valp[i]);
  }
}
