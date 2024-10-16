#include <cstdint>
#pragma once

class allocator {
public:
  void init(uint64_t addr, uint64_t _size);
  void *alloc(uint64_t memSize);
  void free(void *ptr);
  uint64_t address;
  uint64_t size;
  uint32_t baseBlock;
  uint64_t getFreeMem(void *block);
};