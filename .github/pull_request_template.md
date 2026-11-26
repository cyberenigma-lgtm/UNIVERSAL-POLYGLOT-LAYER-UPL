## Description
Please include a summary of the change, target problem, and motivation.

Fixes #(issue number)

## Type of Change
- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Upstream audit case & verification benchmark
- [ ] Documentation update

## Verification & Testing
Describe the tests you ran to verify your changes:
- [ ] Executed `uplc.exe` test suite without regressions.
- [ ] Verified that zero `.exe`, `.obj`, `.o`, or `.bin` artifacts are staged in Git.
- [ ] All code comments and documentation follow formal computer science English standards.

```bash
# Command executed to verify:
bin/uplc.exe ...
```

## Checklist
- [ ] My code follows the style guidelines of this project.
- [ ] I have performed a self-review of my own code.
- [ ] I have commented my code, particularly in hard-to-understand areas.
- [ ] I have updated the documentation corresponding to my changes.
- [ ] My changes generate no new warnings or memory leaks.
