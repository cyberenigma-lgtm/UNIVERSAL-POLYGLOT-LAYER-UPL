import re

class UPLCodeGen:
    """
    UPL Code-Gen Engine v1.1
    Optimized for MultiLang-ASM / Neura Engine compatibility.
    """
    def __init__(self):
        self.reset()
        
    def reset(self):
        self.data_section = ["section .data"]
        self.text_section = []
        self.var_map = {}
        self.reg_pool = ["rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8", "r9"]
        self.reg_idx = 0

    def translate(self, ir_code):
        self.reset()
        # Header estándar para binarios soberanos
        self.text_section = [
            "section .text",
            "global _start",
            "",
            "_start:",
            "    call main",
            "    mov rax, 60          ; sys_exit",
            "    xor rdi, rdi",
            "    syscall",
            "",
            "_upl_print_service:",
            "    ; Placeholder para el servicio de salida de Neuro-OS",
            "    ret"
        ]
        
        lines = ir_code.split('\n')
        for line in lines:
            line = line.strip()
            if not line or line.startswith('#'): continue
            
            # Mapeo de mnemónicos a lógica ASM
            if line.startswith('PROC'):
                name = line.split(' ')[1].split('(')[0]
                self.text_section.append(f"\n{name}:")
                self.text_section.append("    push rbp")
                self.text_section.append("    mov rbp, rsp")
            
            elif line.startswith('RET'):
                val = line.replace('RET', '').strip()
                if val:
                    if val.isdigit():
                        self.text_section.append(f"    mov rax, {val}")
                    else:
                        reg = self._get_reg(val)
                        self.text_section.append(f"    mov rax, {reg}")
                self.text_section.append("    pop rbp")
                self.text_section.append("    ret")
            
            elif line.startswith('SET'):
                parts = line.replace('SET', '').strip().split(',')
                dest = parts[0].strip()
                src = parts[1].strip()
                reg = self._get_reg(dest)
                self.text_section.append(f"    mov {reg}, {src}")
            
            elif line.startswith('ADD'):
                parts = line.replace('ADD', '').strip().split(',')
                dest = parts[0].strip()
                src = parts[1].strip()
                reg = self._get_reg(dest)
                val = src
                if not val.isdigit(): val = self._get_reg(src)
                self.text_section.append(f"    add {reg}, {val}")

            elif line.startswith('IO_WRITE'):
                msg = line.split(',', 1)[1].strip()
                label = f"L_{len(self.data_section)}"
                self.data_section.append(f"    {label} db {msg}, 0xA, 0")
                self.text_section.append(f"    mov rsi, {label}")
                self.text_section.append("    call _upl_print_service")
            
            elif line.startswith(';'): # Raw ASM pass-through
                self.text_section.append(f"    {line[1:].strip()}")

        return "\n".join(self.data_section + [""] + self.text_section)

    def _get_reg(self, var_name):
        if var_name not in self.var_map:
            self.var_map[var_name] = self.reg_pool[self.reg_idx]
            self.reg_idx = (self.reg_idx + 1) % len(self.reg_pool)
        return self.var_map[var_name]

if __name__ == "__main__":
    codegen = UPLCodeGen()
    test_ir = """
    PROC main()
    SET count, 1
    ADD count, 4
    IO_WRITE CONSOLE, "UPL System Active"
    RET count
    """
    print(codegen.translate(test_ir))
