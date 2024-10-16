#include "kernel/arch/x86_64/memory/paging.hpp"
#include <cstdint>

void resetcr3() {
  __asm__ volatile("mov %%cr3, %%rax\n\t"
                   "mov %%rax, %%cr3\n\t"
                   :
                   :
                   : "rax");
}

uint64_t getPageOffset(uint64_t address) {
  pageEntry *baseEntry = getPDPT(address);
  // there can be a huge 1GB, 2MB, 4KB page
  // 1GB, page offset is 30 bit address
  // 2MB, page offset is 21 bit address
  // 4KB, page offset is 12 bit address

  // if not 1GB huge page just fetch the PD (2MB page)
  if (baseEntry->pageSize != 1) {
    baseEntry = getPD(address);
  } else {
    return (address & 0b111111111111111111111111111111); // get 30 bit address
  }

  if (baseEntry->pageSize != 1) { // if page is 4KB page
    baseEntry = getPT(address);
    return (address & 0b111111111111);          // get 4KB address
  } else {                                      // if page is 2MB huge page, PD
    return (address & 0b111111111111111111111); // get 21 bit address
  }

  return 101;
}

uint64_t getPhysicalAddress(uint64_t address) {
  pageEntry *baseEntry = getPDPT(address);

  if (baseEntry->pageSize != 1) {
    baseEntry = getPD(address);
  }

  if (baseEntry->pageSize != 1) {
    baseEntry = getPT(address);
  }

  return ((baseEntry->address << 12) & 0xffffffffff) + getPageOffset(address);
};
