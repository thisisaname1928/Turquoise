#include <cstdint>
#pragma once

extern uint64_t memorySize;
extern uint64_t bitmapSize;
extern uint64_t baseBitmapAddress;
extern uint64_t baseBitmapFreeSize;
extern uint64_t freeMemory;

void initPageFrameAllocator();
bool checkPage(uint64_t address);
uint64_t allocPage(uint64_t nPage);
void deallocPage(uint64_t address, uint64_t nPage);