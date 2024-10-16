#pragma once
#include <cstdint>

void resetcr3();
uint64_t getPhysicalAddress(uint64_t address);
uint64_t getPageOffset(uint64_t address);