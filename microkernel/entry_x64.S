/* =========================================================
 * NEURO-OS GENESIS: 64-BIT LONG MODE TRAMPOLINE (entry_x64.S)
 * Multiboot 1 / 2 Entry -> PML4 Paging -> 64-Bit x86_64
 * Autor: José Manuel Moreno Cano (Noxferion)
 * ========================================================= */

.section .mboot, "a"
.align 8
mboot1_header_start:
.long 0x1BADB002                       /* MULTIBOOT1_MAGIC */
.long 0x00000003                       /* ALIGN + MEMINFO */
.long -(0x1BADB002 + 0x00000003)       /* CHECKSUM */

.align 8
mboot2_header_start:
.long 0xe85250d6                       /* MULTIBOOT2_MAGIC */
.long 0                                 /* Architecture 0 = i386 protected mode */
.long mboot2_header_end - mboot2_header_start /* Header length */
.long -(0xe85250d6 + 0 + (mboot2_header_end - mboot2_header_start)) /* Checksum */
.short 0
.short 0
.long 8
mboot2_header_end:

.section .bss
.align 16
.global stack_bottom_x64
.global stack_top_x64
stack_bottom_x64:
.space 32768
stack_top_x64:

.section .text.entry
.code32
.global _start
.global start
.global _start_x64
.global start_x64
_start:
start:
_start_x64:
start_x64:
    cli
    movl $stack_top_x64, %esp

    /* 1. Inicializar Tablas PML4 en C */
    call init_paging_x64

    /* 2. Cargar CR3 con pml4_table */
    movl $pml4_table, %eax
    movl %eax, %cr3

    /* 3. Habilitar PAE (Physical Address Extension) en CR4 (Bit 5 = 0x20) */
    movl %cr4, %eax
    orl $0x20, %eax
    movl %eax, %cr4

    /* 4. Habilitar Long Mode en MSR 0xC0000080 (EFER.LME Bit 8 = 0x100) */
    movl $0xC0000080, %ecx
    rdmsr
    orl $0x00000100, %eax
    wrmsr

    /* 5. Habilitar Paging en CR0 (Bit 31 = 0x80000000) */
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

    /* 6. Cargar GDT de 64-bits */
    lgdt (gdt64_ptr)

    /* 7. Salto lejano a Segmento de Código de 64-bits (Selector 0x08) */
    ljmp $0x08, $long_mode_start

.code64
long_mode_start:
    /* Cargar selectores de datos de 64-bits */
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    /* Llamar a kernel_main_x64 en Long Mode */
    call kernel_main_x64

.Lhalt_x64:
    hlt
    jmp .Lhalt_x64

/* ---- GDT de 64-bits ---- */
.section .rodata
.align 8
gdt64:
    .quad 0x0000000000000000 /* 0x00: Null Descriptor */
    .quad 0x00AF9A000000FFFF /* 0x08: 64-Bit Code Descriptor (Executable/Readable, Long Mode = 1) */
    .quad 0x00CF92000000FFFF /* 0x10: 64-Bit Data Descriptor (Writable) */
gdt64_end:

gdt64_ptr:
    .word gdt64_end - gdt64 - 1
    .long gdt64

.section .note.GNU-stack,"",@progbits
