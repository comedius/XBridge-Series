# AI development workflow

Use small, reviewable tasks. Each agent works on a dedicated branch or isolated worktree.

## Required workflow

1. Read `docs/ARCHITECTURE.md`, `docs/DECISIONS.md`, and relevant headers.
2. Restate the task and list files to be changed.
3. Implement only the assigned scope.
4. Add or update tests.
5. Run CMake build and CTest.
6. Report commands, results, limitations, and unresolved questions.
7. Open a pull request; never merge directly to the protected main branch.

## Global guardrails

- Do not claim Xbox compatibility unless demonstrated by tests.
- Do not invent undocumented APIs, registers, opcodes, or hardware behavior.
- Do not add firmware, decryption keys, game dumps, or proprietary assets.
- Do not implement bypasses of access controls or security protections.
- Do not modify another agent's module without an approved interface change.
- Keep `TestCpu` separate from future x86-64 code.
