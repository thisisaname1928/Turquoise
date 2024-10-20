#include "cpuid.hpp"
#include "kernel/arch/x86_64/registers/getReg.hpp"
#include <cstdint>

bool CPUID::checkCPUFeature(uint16_t feature) {
  callCPUID(1);

  uint32_t ecx = registersData.rcx & 0xffffffff;
  uint32_t edx = registersData.rdx & 0xffffffff;

  if (feature <= 31)
    return (ecx >> feature) & 1;
  else
    return (edx >> (feature - 32)) & 1;
}