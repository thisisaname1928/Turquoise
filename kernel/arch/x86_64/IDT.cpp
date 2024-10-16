#include "IDT.hpp"
#include <cstdint>

void setIDT(IDT *tmp, uint64_t offset, uint8_t IST, uint8_t gateType,
            uint8_t DPL) {

  tmp->offset0 = offset & 0xffff;
  tmp->offset1 = (offset >> 16) & 0xffff;
  tmp->offset2 = (offset >> 32);
  tmp->cs = 0x28;

  tmp->attribute = (1 << 15) | (DPL << 13) | (gateType << 8) | IST;
}

void loadIDTR(IDTR *idtr) {
  __asm__ volatile("lidt %0" : : "m"(*idtr));
  __asm__ volatile("sti");
}