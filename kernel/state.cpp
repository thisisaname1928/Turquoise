#include <cstdint>
uint64_t KERNEL_STATE;

void setKernelState(uint64_t state) { KERNEL_STATE = state; }
uint64_t getKernelState() { return KERNEL_STATE; }