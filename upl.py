#!/usr/bin/env python3
"""
Universal Polyglot Layer (UPL+) - Native CLI Forwarder
Delegates all compiler and runtime tasks directly to the standalone C99 binary: bin/uplc.exe
"""
import sys
import subprocess
from pathlib import Path

def main():
    repo_root = Path(__file__).resolve().parent
    binary = repo_root / "bin" / ("uplc.exe" if sys.platform == "win32" else "uplc")

    if not binary.exists():
        print(f"Error: Native compiler not found at {binary}", file=sys.stderr)
        print("Please run upl.bat or upl.sh to build bin/uplc.", file=sys.stderr)
        sys.exit(1)

    result = subprocess.call([str(binary)] + sys.argv[1:])
    sys.exit(result)

if __name__ == "__main__":
    main()
