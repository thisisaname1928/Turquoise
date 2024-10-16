
#include "kernel/arch/x86_64/memory/paging.hpp"
#include "kernel/essential.hpp"
#include <cstdint>
#include <kernel/kprintf.hpp>

pageEntry *getPML4(uint64_t address) {
  // where is PML4 table
  pageEntry *PML4 = (pageEntry *)(getcr3() + virtualAddressOffset);

  // return PML4 table for address
  uint64_t pos = (address >> 39) & 511;
  return &PML4[pos];
}

pageEntry *getPDPT(uint64_t address) {
  // get PML4
  pageEntry *PML4 = getPML4(address);

  if (PML4->present != 1) {
    kprintf("PML4 of address=0x%x is not present!\n", address);
    return nullptr;
  }

  pageEntry *PDPT = (pageEntry *)((PML4->address << 12) + virtualAddressOffset);

  // calculate PDPT index
  uint64_t index = (address >> 30) & 511;

  return &PDPT[index];
}

pageEntry *getPD(uint64_t address) {
  // get PDPT
  pageEntry *PDPT = getPDPT(address);

  if (PDPT->present != 1) {
    kprintf("PML4 of address=0x%x is not present!\n", address);
    return nullptr;
  }

  // get address PD from PDPT
  pageEntry *PD = (pageEntry *)((PDPT->address << 12) + virtualAddressOffset);

  // calculate index
  uint64_t index = (address >> 21) & 511;

  return &PD[index];
}

pageEntry *getPT(uint64_t address) {
  // get PD
  pageEntry *PD = getPD(address);

  if (PD->present != 1) {
    kprintf("PD of address=0x%x is not present!\n", address);
    return nullptr;
  }

  // get PT address from PD
  pageEntry *PT = (pageEntry *)((PD->address << 12) + virtualAddressOffset);

  // calculate index
  uint64_t index = (address >> 12) & 511;

  return &PT[index];
}