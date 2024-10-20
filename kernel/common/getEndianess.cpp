
#include <cstdint>

bool isLittleEndian() {
  union {
    uint32_t i;
    char c[4];
  } tmp = {0x01020304};

  return tmp.c[0] != 1;
}