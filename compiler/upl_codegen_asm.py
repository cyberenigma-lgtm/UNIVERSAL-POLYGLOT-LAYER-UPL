"""
UPL Native NASM Assembly Code Generator (Legacy Interface)
This module has been replaced by the standalone C99 native toolchain: compiler/uplc.c
"""
import sys

def generate_asm(*args, **kwargs):
    print("Notice: Assembly generation is now handled natively by bin/uplc.exe", file=sys.stderr)
    return ""
