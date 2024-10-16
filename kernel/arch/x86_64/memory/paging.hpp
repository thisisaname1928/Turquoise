#pragma once

#include <cstdint>
extern "C" uint64_t getcr3();

typedef struct __attribute__((__packed__)) {
  uint64_t present : 1;
  uint64_t RW : 1;
  uint64_t US : 1;
  uint64_t writeThrough : 1;
  uint64_t cacheDisable : 1;
  uint64_t accessed : 1;
  uint64_t available : 1;
  uint64_t pageSize : 1;
  uint64_t available1 : 4;
  uint64_t address : 40;
  uint64_t available2 : 11;
  uint64_t executeDisable : 1;
} pageEntry;

pageEntry *getPML4(uint64_t address);
pageEntry *getPDPT(uint64_t address);
pageEntry *getPD(uint64_t address);
pageEntry *getPT(uint64_t address);