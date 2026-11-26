---
name: Bug Report
about: Create a report to help us improve UPL+ compiler, runtime, or tools
title: '[BUG] '
labels: 'bug, untriaged'
assignees: ''
---

## Description
A clear and concise description of what the bug is.

## Environment
- **OS & Architecture:** [e.g., Windows 11 x64, Ubuntu 24.04 x86_64]
- **UPL Compiler Version (`uplc --version`):** [e.g., v2.2.3]
- **Host C Compiler:** [e.g., GCC 13.2.0, Clang 18.1]

## Reproduction Steps
Steps to reproduce the behavior:
1. Create file `repro.upl` containing:
   ```upl
   // Minimal repro code here
   ```
2. Execute command:
   ```bash
   bin/uplc.exe repro.upl -o repro.exe
   ```
3. Observe failure or crash.

## Expected Behavior
A clear and concise description of what you expected to happen.

## Actual Behavior / Compiler Diagnostics
```text
[Paste terminal diagnostics or error output here]
```

## Additional Context
Add any other context about the problem here (AST dumps, generated `.upl.c` analysis, etc.).
