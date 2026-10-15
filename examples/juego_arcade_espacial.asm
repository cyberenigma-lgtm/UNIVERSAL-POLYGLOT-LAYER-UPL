section .rodata
align 16
; Blindaje Vectorial DVTRGAS-30 (Landing Pad Maestro)
dvtrgas30_landing_pad: times 6 db 0x00
str_const_13: db "checkpoint_victoria_nivel1.gxp", 0

default rel
section .text
global _start_upl_microkernel

extern salto_logico_guardar

global crear_nave
crear_nave:
    sub rsp, 16
    push rbp
    mov rbp, rsp

    mov rax, 40
    mov rax, 20
    mov rax, 3
    mov rax, 100
    mov rax, 0
    mov rax, [rbp -8]
    mov rsp, rbp
    pop rbp
    ret
epilogue_crear_nave:
    mov rsp, rbp
    pop rbp
    ret

global disparar_laser
disparar_laser:
    sub rsp, 16
    push rbp
    mov rbp, rsp
    mov [rbp - 8], rdi
    mov [rbp - 16], rsi
    mov rax, [rbp -16]
    test rax, rax
    jz L_else_1
    mov rax, [rbp -8]
    mov rax, 1
    push rax
    mov rax, [rbp -8]
    pop rbx
    sub rax, rbx
    mov rax, 1
    jmp L_endif_2
L_else_1:
L_endif_2:
epilogue_disparar_laser:
    mov rsp, rbp
    pop rbp
    ret

global actualizar_fisica
actualizar_fisica:
    sub rsp, 32
    push rbp
    mov rbp, rsp
    mov [rbp - 8], rdi
    mov [rbp - 16], rsi
    mov [rbp - 24], rdx
    mov [rbp - 32], rcx
    mov rax, [rbp -16]
    test rax, rax
    jz L_else_3
    mov rax, 1
    push rax
    mov rax, [rbp -16]
    pop rbx
    sub rax, rbx
    mov rax, 0
    push rax
    mov rax, [rbp -16]
    pop rbx
    cmp rax, rbx
    setle al
    movzx rax, al
    test rax, rax
    jz L_else_5
    mov rax, 0
    jmp L_endif_6
L_else_5:
L_endif_6:
    mov rax, [rbp -32]
    push rax
    mov rax, [rbp -16]
    pop rbx
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    mov rax, [rbp -24]
    push rax
    mov rax, [rbp -16]
    pop rbx
    cmp rax, rbx
    sete al
    movzx rax, al
    pop rbx
    test rax, rax
    setnz al
    test rbx, rbx
    setnz bl
    and al, bl
    movzx rax, al
    test rax, rax
    jz L_else_7
    mov rax, 0
    mov rax, 100
    push rax
    mov rax, [rbp -8]
    pop rbx
    add rax, rbx
    mov rax, 1
    mov rsp, rbp
    pop rbp
    ret
    jmp L_endif_8
L_else_7:
L_endif_8:
    jmp L_endif_4
L_else_3:
L_endif_4:
    mov rax, 0
    mov rsp, rbp
    pop rbp
    ret
epilogue_actualizar_fisica:
    mov rsp, rbp
    pop rbp
    ret

global main
main:
    sub rsp, 48
    push rbp
    mov rbp, rsp
    call crear_nave
    mov [rbp -8], rax

    mov rax, 0
    mov rax, 0
    mov rax, 0
    mov rax, 40
    mov [rbp -24], rax
    mov rax, 5
    mov [rbp -32], rax
    lea rax, [rbp -8]
    mov rdi, rax
    lea rax, [rbp -16]
    mov rsi, rax
    call disparar_laser
    mov rax, 0
    mov [rbp -40], rax
L_while_start_9:
    mov rax, 16
    push rax
    mov rax, [rbp -40]
    pop rbx
    cmp rax, rbx
    setl al
    movzx rax, al
    test rax, rax
    jz L_while_end_10
    lea rax, [rbp -8]
    mov rdi, rax
    lea rax, [rbp -16]
    mov rsi, rax
    mov rax, [rbp -24]
    mov rdx, rax
    mov rax, [rbp -32]
    mov rcx, rax
    call actualizar_fisica
    mov [rbp -48], rax
    mov rax, [rbp -48]
    test rax, rax
    jz L_else_11
    lea rax, [str_const_13]
    mov rdi, rax
    call salto_logico_guardar
    jmp L_endif_12
L_else_11:
L_endif_12:
    mov rax, 1
    push rax
    mov rax, [rbp -40]
    pop rbx
    add rax, rbx
    mov [rbp -40], rax
    jmp L_while_start_9
L_while_end_10:
    mov rax, [rbp -8]
    mov rsp, rbp
    pop rbp
    ret
epilogue_main:
    mov rsp, rbp
    pop rbp
    ret
