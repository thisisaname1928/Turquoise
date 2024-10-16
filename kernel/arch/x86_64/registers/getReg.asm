bits 64
global getReg
global registersData
section .text
getReg:
    mov qword [registersData.rax], rax
    mov qword [registersData.rbx], rbx
    mov qword [registersData.rcx], rcx
    mov qword [registersData.rdx], rdx
    mov qword [registersData.rsi], rsi
    mov qword [registersData.rdi], rdi
    mov qword [registersData.rsp], rsp
    mov qword [registersData.rbp], rbp
    mov qword [registersData.r8], r8
    mov qword [registersData.r9], r9
    mov qword [registersData.r10], r10
    mov qword [registersData.r11], r11
    mov qword [registersData.r12], r12
    mov qword [registersData.r13], r13
    mov qword [registersData.r14], r14
    mov qword [registersData.r15], r15

    ret

section .data
registersData:
    .rax:
        dq 0
    .rbx:
        dq 0
    .rcx:
        dq 0
    .rdx:
        dq 0
    .rsi:
        dq 0
    .rdi:
        dq 0
    .rsp:
        dq 0
    .rbp:
        dq 0
    .r8:
        dq 0
    .r9:
        dq 0
    .r10:
        dq 0
    .r11:
        dq 0
    .r12:
        dq 0
    .r13:
        dq 0
    .r14:
        dq 0
    .r15:
        dq 0