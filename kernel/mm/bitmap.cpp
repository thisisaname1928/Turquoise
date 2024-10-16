#include "bitmap.hpp"
#include <cstdint>

bool bitmap::check(uint64_t index) {
  if (index >= size * 8) {
    return false;
  }

  return true;
}

bool bitmap::get(uint64_t index) {
  if (!check(index)) {
    return 0;
  }

  return (arr[(uint64_t)(index / 64)] >> (uint64_t)(index % 64)) & 1;
}

void bitmap::set(uint64_t index, bool value) {
  if (!check(index)) {
    return;
  }

  if (value == 1) {
    uint64_t tmp = ((uint64_t)1 << (uint64_t)(index % 64));
    arr[(uint64_t)(index / 64)] |= tmp;
  } else {
    uint64_t tmp = ((uint64_t)1 << (uint64_t)(index % 64));
    arr[(uint64_t)(index / 64)] &= ~tmp;
  }
}

void bitmap::set(uint64_t index, uint64_t n, uint64_t value) {
  uint64_t u64Value = 0;
  if (value == 1) {
    u64Value = UINT64_MAX;
  } else {
    u64Value = 0;
  }
  // stage 1
  while (n >= 64 && (index % 64 != 0)) { // fill until index begin of uint64_t
    set(index, value);
    n--;
    index++;
  }

  // stage 2
  while (n >= 64) {
    arr[index / 64] = u64Value;
    n -= 64;
    index += 64;
  }

  // stage 3
  while (n > 0) {
    set(index, value);
    n--;
    index++;
  }
}