/* ============================================================================ */
/* CÓDIGO GENERADO AUTOMÁTICAMENTE POR EL COMPILADOR UPLC (ANSI C99)            */
/* ============================================================================ */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- PROTECCIÓN DE MEMORIA: CANARIOS DE SEGURIDAD Y TIPO DECIMAL FIJO --- */
typedef struct {
    uint8_t guard_head[6];
    int64_t valor_entero;
    uint32_t escala;
    uint8_t guard_tail[6];
} UPL_DecimalComp3;

static inline void checkpoint_guardar(const char* nombre) {
    printf("[CHECKPOINT] Estado guardado: %s\n", nombre);
}
#define salto_logico_guardar checkpoint_guardar

typedef struct CpuRegisterState CpuRegisterState;

static const uint32_t CPU_OP_NOP = 0x00;
static const uint32_t CPU_OP_MOV = 0x01;
static const uint32_t CPU_OP_ADD = 0x02;
static const uint32_t CPU_OP_SUB = 0x03;
static const uint32_t CPU_OP_CMP = 0x04;
static const uint32_t CPU_OP_HALT = 0xFF;
struct CpuRegisterState {
    uint8_t _guard_canary_head[6];
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rip;
    uint64_t rsp;
    bool flags_zero;
    bool halted;
    uint64_t cycles_executed;
    uint8_t _guard_canary_tail[6];
};

void cpu_init(CpuRegisterState* cpu, uint64_t stack_top);
void cpu_init(CpuRegisterState* cpu, uint64_t stack_top) {
    cpu->rax = 0 ;
    cpu->rbx = 0 ;
    cpu->rcx = 0 ;
    cpu->rdx = 0 ;
    cpu->rip = 0 ;
    cpu->rsp = stack_top ;
    cpu->flags_zero = false ;
    cpu->halted = false ;
    cpu->cycles_executed = 0 ;
}

void cpu_step(CpuRegisterState* cpu, uint32_t opcode, uint64_t oper_dst, uint64_t oper_src);
void cpu_step(CpuRegisterState* cpu, uint32_t opcode, uint64_t oper_dst, uint64_t oper_src) {
    if (cpu->halted ) {
    return ;
}
    if (opcode == CPU_OP_MOV ) {
    cpu->rax = oper_src ;
}
 else    if (opcode == CPU_OP_ADD ) {
    cpu->rax = cpu->rax + oper_src ;
}
 else    if (opcode == CPU_OP_SUB ) {
    cpu->rax = cpu->rax - oper_src ;
}
 else    if (opcode == CPU_OP_CMP ) {
    if (cpu->rax == oper_src ) {
    cpu->flags_zero = true ;
}
 else {
    cpu->flags_zero = false ;
}
}
 else    if (opcode == CPU_OP_HALT ) {
    cpu->halted = true ;
}
    cpu->rip = cpu->rip + 1 ;
    cpu->cycles_executed = cpu->cycles_executed + 1 ;
}

