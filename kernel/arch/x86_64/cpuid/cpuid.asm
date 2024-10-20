global callCPUID
extern getReg
section .text
bits 64

callCPUID:
    mov rax, rdi
    cpuid
    call getReg
    ret