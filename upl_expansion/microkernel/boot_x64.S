/* =========================================================
 * NEURO-OS GENESIS: PUENTE ENSAMBLADOR NATIVO 64-BITS (boot_x64.S)
 * Transición Bare-Metal 32-Bit -> PML4 4-Niveles -> 64-Bit x86_64
 * Autor: José Manuel Moreno Cano (Noxferion)
 * ========================================================= */

.section .mboot, "a"
.align 4
mboot1_header_start:
.long 0x1BADB002                       /* MULTIBOOT1_MAGIC */
.long 0x00000007                       /* ALIGN + MEMINFO + VIDEO_MODE (Bit 2) */
.long -(0x1BADB002 + 0x00000007)       /* CHECKSUM */
.long 0; .long 0; .long 0; .long 0; .long 0 /* Unused header addr fields */
.long 0                                /* Mode type: 0 = Linear Graphics */
.long 1024                             /* Width */
.long 768                              /* Height */
.long 32                               /* Depth (BPP) */

.align 8
mboot2_header_start:
.long 0xe85250d6                       /* MULTIBOOT2_MAGIC */
.long 0                                 /* Architecture 0 = i386 protected mode */
.long mboot2_header_end - mboot2_header_start
.long -(0xe85250d6 + 0 + (mboot2_header_end - mboot2_header_start))
.short 0
.short 0
.long 8
mboot2_header_end:

/* Stack de 32 KB para el Kernel x64 */
.section .bss
.align 16
.global stack_bottom_x64
.global stack_top_x64
stack_bottom_x64:
.space 32768
stack_top_x64:

/* Tablas PML4 alineadas a 4KB (4096 bytes) */
.align 4096
.global pml4_table
.global pdpt_table
.global page_directory_x64
.global page_directory_high_x64
pml4_table:
.space 4096
pdpt_table:
.space 4096
page_directory_x64:
.space 4096
page_directory_high_x64:
.space 4096

.section .text.entry
.code32
.global _start
.global start
.global _start_x64
_start:
start:
_start_x64:
    cli
    movl %ebx, %esi                       /* Preservar MBI ptr */
    movl $stack_top_x64, %esp

    /* 1. Construir mapeo directo identidad en tablas PML4 */
    movl $pdpt_table, %eax
    orl $0x07, %eax
    movl %eax, (pml4_table)
    movl $0, (pml4_table + 4)

    movl $page_directory_x64, %eax
    orl $0x07, %eax
    movl %eax, (pdpt_table)
    movl $0, (pdpt_table + 4)

    movl $page_directory_high_x64, %eax
    orl $0x07, %eax
    movl %eax, (pdpt_table + 24)
    movl $0, (pdpt_table + 28)

    /* Mapear 32 entradas de 2MB (0 a 64MB) en page_directory_x64 */
    movl $page_directory_x64, %edi
    movl $0x00000087, %eax
    movl $0x00000000, %edx
    movl $32, %ecx
.Lfill_low_pd:
    movl %eax, (%edi)
    movl %edx, 4(%edi)
    addl $0x00200000, %eax
    adcl $0, %edx
    addl $8, %edi
    loop .Lfill_low_pd

    /* Mapear 512 entradas de 2MB en page_directory_high_x64 (3GB a 4GB: 0xC0000000 - 0xFFFFFFFF) */
    movl $page_directory_high_x64, %edi
    movl $0xC0000087, %eax
    movl $0x00000000, %edx
    movl $512, %ecx
.Lfill_high_pd:
    movl %eax, (%edi)
    movl %edx, 4(%edi)
    addl $0x00200000, %eax
    adcl $0, %edx
    addl $8, %edi
    loop .Lfill_high_pd

    /* 2. Cargar CR3 con la dirección de pml4_table */
    movl $pml4_table, %eax
    movl %eax, %cr3

    /* 3. Activar PAE (0x20), OSFXSR (0x200) y OSXMMEXCPT (0x400) en CR4 -> 0x620 */
    movl %cr4, %eax
    orl $0x620, %eax
    movl %eax, %cr4

    /* 4. Activar Long Mode (LME) en MSR 0xC0000080 */
    movl $0xC0000080, %ecx
    rdmsr
    orl $0x00000100, %eax
    wrmsr

    /* 5. Activar Paginación (0x80000000) y Configurar SSE en CR0 */
    movl %cr0, %eax
    andl $~0x00000004, %eax
    orl $0x80000002, %eax
    movl %eax, %cr0

    /* 6. Cargar GDT de 64-bits */
    lgdt (gdt64_ptr)

    /* 7. Transición en caliente a 64-bits Long Mode */
    pushl $0x08
    pushl $long_mode_entry
    lret

.code64
long_mode_entry:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    movq $stack_top_x64, %rsp
    andq $-16, %rsp

    movl %esi, %edi
    call kernel_main_x64

.Lhalt_x64_loop:
    hlt
    jmp .Lhalt_x64_loop

.section .data
.align 16
.global gdt64
.global gdt64_ptr
gdt64:
    .quad 0x0000000000000000
    .quad 0x00AF9A000000FFFF
    .quad 0x00CF92000000FFFF
    .quad 0x0000000000000000
    .quad 0x0000000000000000
gdt64_end:

gdt64_ptr:
    .word gdt64_end - gdt64 - 1
    .quad gdt64

.section .note.GNU-stack,"",@progbits
