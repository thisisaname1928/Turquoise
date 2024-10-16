#include "limine/limine.h"
#include <cstddef>
#include <cstdint>
#pragma once

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void halt();
extern uint64_t virtualAddressOffset;
extern limine_memmap_response *memoryMap;