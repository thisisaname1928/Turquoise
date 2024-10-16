#include <cstdint>
#pragma once

#define IDT_InterruptGate 0xe
#define IDT_TrapGate 0xf

typedef struct __attribute__((__packed__)) {
  uint16_t offset0;
  uint16_t cs;
  uint16_t attribute;
  uint16_t offset1;
  uint32_t offset2;
  uint32_t reserved;
} IDT;

typedef struct __attribute((__packed__)) {
  uint16_t size;
  uint64_t offset;
} IDTR;

void setIDT(IDT *tmp, uint64_t offset, uint8_t IST, uint8_t gateType,
            uint8_t DPL);
void loadIDTR(IDTR *idtr);