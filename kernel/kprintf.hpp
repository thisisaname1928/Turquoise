#pragma once
#include <cstdint>
void test_put(int x, int y, uint16_t ch);
void sprint(const char *st);
void kprintf(const char *fmt, ...);
void printstr(const char *st, uint64_t length);
extern char *fontPointer;