#pragma once

#include <cstdint>

class bitmap {
public:
  void set(uint64_t index, bool value);
  void set(uint64_t index, uint64_t n, uint64_t value);
  bool get(uint64_t index);
  uint64_t *arr;
  uint64_t size;

private:
  bool check(uint64_t index);
};