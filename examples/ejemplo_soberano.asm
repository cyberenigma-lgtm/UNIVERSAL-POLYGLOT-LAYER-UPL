section .rodata
align 16
; Blindaje Vectorial DVTRGAS-30 (Landing Pad Maestro)
dvtrgas30_landing_pad: times 6 db 0x00
str_const_5: db "checkpoint_iteracion_3.gxp", 0

default rel
section .text
global _start_upl_microkernel

; === FUNCIÓN BARE-METAL / RING 0: inicializar_hardware ===
global inicializar_hardware
inicializar_hardware:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov rax, 462
    mov [rbp -8], rax
    mov rax, 65
    mov [rbp -16], rax
    ; --- BLOQUE ASM SOBERANO CRUDA ---
    cli
    cld
    mov rax, 1
    mov rsp, rbp
    pop rbp
    ret
.epilogue_inicializar_hardware:
    mov rsp, rbp
    pop rbp
    ret

global main
main:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov rax, 0
    mov [rbp -8], rax
    mov rax, 1
    mov [rbp -16], rax
    call inicializar_hardware
L_while_start_1:
    mov rax, 5
    push rax
    mov rax, [rbp -8]
    pop rbx
    cmp rax, rbx
    setl al
    movzx rax, al
    test rax, rax
    jz L_while_end_2
    mov rax, 1
    push rax
    mov rax, [rbp -8]
    pop rbx
    add rax, rbx
    mov [rbp -8], rax
    mov rax, 3
    push rax
    mov rax, [rbp -8]
    pop rbx
    cmp rax, rbx
    sete al
    movzx rax, al
    test rax, rax
    jz L_else_3
    lea rax, [str_const_5]
    mov rdi, rax
    call salto_logico_guardar
    jmp L_endif_4
L_else_3:
L_endif_4:
    jmp L_while_start_1
L_while_end_2:
    mov rax, [rbp -8]
    mov rsp, rbp
    pop rbp
    ret
.epilogue_main:
    mov rsp, rbp
    pop rbp
    ret
