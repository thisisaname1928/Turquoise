bits 64
global getcr3
section .text
getcr3:
    mov rax, cr3
    ret