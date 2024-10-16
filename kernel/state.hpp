#pragma once

#include <cstdint>

enum kernelState {
  EARLY_KERNEL_INITIALIZATION,
  KERNEL_INITIALIZATION,
};

extern uint64_t KERNEL_STATE;
void setKernelState(uint64_t state);
uint64_t getKernelState();