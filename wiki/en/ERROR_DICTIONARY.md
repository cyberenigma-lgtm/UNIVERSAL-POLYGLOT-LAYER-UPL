# 🪟 MULTI-OS DIAGNOSTICS & ERROR DICTIONARY (57 LANGUAGES)
## Real-Time Intelligent Troubleshooting for Windows, Linux, and Bare-Metal CPU

UPL integrates an intelligent diagnostic engine that explains and translates error codes across 57 languages.

---

### 1. CLI Diagnostic Command

Run `upl diag` with any hexadecimal or decimal code:

```bash
# Diagnose in English
upl diag 0xC0000005 -l en

# Diagnose in Spanish
upl diag 0xC0000096 -l es

# Diagnose in Japanese
upl diag 0xC00000FD -l ja
```

---

### 2. Common Windows NTSTATUS Exceptions

| Hex Code | NTSTATUS Name | Root Cause | Solution in UPL |
|---|---|---|---|
| `0xC0000005` | `STATUS_ACCESS_VIOLATION` | Null pointer or write to read-only page. | Validate `*ptr != NULL` or use `@snapshot` landing-pads. |
| `0xC0000096` | `STATUS_PRIVILEGED_INSTRUCTION` | Ring 0 instruction (`cli`, `inb`) executed in Ring 3. | Build with `upl build file.upl -t asm --baremetal` for Ring 0. |
| `0xC000001D` | `STATUS_ILLEGAL_INSTRUCTION` | Invalid opcode or unsupported vector extension. | Verify inline `asm { ... }` instructions. |
| `0xC00000FD` | `STATUS_STACK_OVERFLOW` | Thread stack exhausted by unbounded recursion. | Check recursion base case or allocate large buffers on heap. |
| `0xC0000094` | `STATUS_INTEGER_DIVIDE_BY_ZERO` | Integer division by zero (`/ 0`). | Add guard `if divisor != 0` or use NeuroCOBOL `comp3` types. |

---

### 3. Bare-Metal x86_64 CPU Exceptions

| CPU Vector | Mnemonic | Silicon Cause | Fix in UPL |
|---|---|---|---|
| `0x00` | `#DE` | Division error or quotient overflow in `rax`. | Pre-check divisor before `idiv`. |
| `0x06` | `#UD` | Invalid opcode decoded by CPU. | Check code alignment and 64-bit GDT descriptor. |
| `0x08` | `#DF` | Double fault during interrupt dispatch. | Configure dedicated IST stack in 64-bit TSS. |
| `0x0D` | `#GP` | General Protection Fault (non-canonical address). | Ensure all pointers reside within canonical 48-bit address space. |
| `0x0E` | `#PF` | Page Fault in virtual memory translation. | Check PML4 / PDPT page table entries. |
