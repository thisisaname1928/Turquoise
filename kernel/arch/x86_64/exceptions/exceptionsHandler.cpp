#include "kernel/essential.hpp"
#include <cstdint>
#include <kernel/kprintf.hpp>

const char *errName[] = {"Division Error",
                         "Debug",
                         "Non-maskable Interrupt",
                         "Breakpoint",
                         "Overflow",
                         "Bound Range Exceeded",
                         "Invalid Opcode",
                         "Device Not Available",
                         "Double Fault",
                         "Coprocessor Segment Overrun",
                         "Invalid TSS",
                         "Segment Not Present",
                         "Stack-Segment Fault",
                         "General Protection Fault",
                         "Page Fault",
                         "Reserved",
                         "x87 Floating-Point Exception",
                         "Alignment Check	17",
                         "Machine Check",
                         "SIMD Floating-Point Exception",
                         "Virtualization Exception",
                         "Control Protection Exception",
                         "Reserved",
                         "Reserved",
                         "Reserved",
                         "Reserved",
                         "Reserved",
                         "Reserved",
                         "Hypervisor Injection Exception",
                         "VMM Communication Exception",
                         "Security Exception",
                         "Reserved",
                         "Triple Fault",
                         "FPU Error Interrupt",
                         "UNKOWN_Interrupt"};

void exceptionHandler(uint64_t errCode) {
  sprint("\nError: ");
  sprint(errName[errCode]);
  halt();
}