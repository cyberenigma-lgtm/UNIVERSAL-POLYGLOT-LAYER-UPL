; ============================================================================
; CÓDIGO GENERADO AUTOMÁTICAMENTE POR EL COMPILADOR SOBERANO UPLC (NASM x86_64)
; ============================================================================

default rel
global main
extern printf
extern ExitProcess

section .data
    msg_checkpoint db "[SALTO_LOGICO] Punto de control guardado: %s", 10, 0

section .text

salto_logico_guardar:
    sub rsp, 40
    mov rdx, rcx
    lea rcx, [msg_checkpoint]
    call printf
    add rsp, 40
    ret

main:
    push rbp
    mov rbp, rsp
    sub rsp, 64

    ; Ejecución nativa optimizada x86_64
    xor eax, eax
    mov rsp, rbp
    pop rbp
    ret
