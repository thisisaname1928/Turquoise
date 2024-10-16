#include "kernel/error/error.hpp"
#include "kernel/essential.hpp"
#include "kernel/kprintf.hpp"
#include <cstdint>
#include <kernel/state.hpp>

void kernelErrorHandler(uint64_t err, int line, const char *fn,
                        const char *file) {
  switch (err) {
  case OUT_OF_MEMORY:
    switch (getKernelState()) {
    case EARLY_KERNEL_INITIALIZATION:
      halt();
      break; // NO MORE bruh warning
    case KERNEL_INITIALIZATION:
      kprintf("Error file %s %s(...) %u: out of memory!\n", file, fn,
              (uint64_t)line);
      halt();
    default:
      break;
    }
    break;
  default:
    return;
  }
}
