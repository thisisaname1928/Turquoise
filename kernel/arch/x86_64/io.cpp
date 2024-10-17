#include <cstdint>

uint8_t inb(uint16_t port) {
  uint8_t val;

  asm volatile("inb %w1, %b0" : "=a"(val) : "Nd"(port) : "memory");

  return val;
}

uint16_t inw(uint16_t port) {
  uint16_t val;

  asm volatile("inw %w1, %w0" : "=a"(val) : "Nd"(port) : "memory");

  return val;
}

uint32_t inl(uint16_t port) {
  uint32_t val;

  asm volatile("inl %w1, %d0" : "=a"(val) : "Nd"(port) : "memory");

  return val;
}

void outb(uint16_t port, uint8_t val) {
  __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

void outw(uint16_t port, uint16_t val) {
  __asm__ volatile("outw %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

void outw(uint16_t port, uint32_t val) {
  __asm__ volatile("outl %d0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

void ioWait() { outb(0x80, 0); }