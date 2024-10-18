#include "kernel/driver/acpi/amlOpcodes.hpp"
#include "kernel/kprintf.hpp"
#include <cstdint>

bool isEqual(const char *str, const char *nonstr) {
  int i = 0;
  while (str[i] != 0) {
    if (str[i] != nonstr[i]) {
      return false;
    }
    i++;
  }
  return true;
}

void AMLSearchName(char *pos, uint64_t size, const char *Name) {
  for (uint64_t i = 0; i < size; i++) {
    if (pos[i] == ACPIOpcodes::NAME) {
      if (isEqual(Name, &pos[i + 1])) {
        kprintf(&pos[i]);
      }
    }
  }
}