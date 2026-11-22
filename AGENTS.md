# XBridge Series – Agent Instructions

## 1. Project overview

XBridge Series is a Windows-based Xbox Series X emulator project written in C++20.

Primary priorities, in order:
1. Correctness
2. Stability
3. Consistent frame times
4. Performance
5. Graphics quality

The project does not currently claim Xbox Series X game compatibility.

## 2. Architecture rules

- Target Xbox Series X only. Xbox Series S is out of scope.
- Keep the interpreter as the safe CPU execution baseline.
- Do not implement JIT without separate evidence, design and tests.
- Keep guest CPU semantics separate from host CPU capabilities.
- Keep GuestMemory generic. Do not assume it represents the Xbox virtual-memory system.
- Prefer hardware acceleration, with a safe software fallback.
- Do not switch execution paths in the middle of an operation unless explicitly designed and tested.
- Manage host RAM and VRAM dynamically. Never reserve all host RAM for guest memory.
- Preserve existing public interfaces unless the task explicitly requires a change.
- Do not make Xbox-specific claims based only on research about other consoles.

## 3. Evidence and legal constraints

Classify technical claims as:
- documented
- observed
- hypothesis
- unknown

Never present a hypothesis as a verified fact.

Do not:
- add proprietary firmware, keys, game files or protected assets;
- implement protection or DRM circumvention;
- copy third-party code without verifying its license;
- add downloads of proprietary system files or game content.

Keep the project independent of proprietary Xbox software and assets.

## 4. Mandatory task workflow

Before editing:
1. Read the relevant headers, implementations, tests and documentation.
2. Inspect the current Git status and existing changes.
3. Identify dependencies and public interfaces.
4. Produce a concise implementation plan.
5. List the files that need changes and explain why.
6. Identify risks and required tests.

Do not edit files until the task plan has been reviewed and approved.

During implementation:
- Follow the approved plan.
- Make the smallest coherent change that solves the task.
- Do not modify unrelated modules.
- Do not silently change public interfaces.
- Do not remove, weaken or skip existing tests.
- Do not resolve conflicts by blindly accepting both versions.
- Check declarations, definitions, includes and call sites for consistency.
- Keep comments and documentation consistent with actual behavior.
- Do not introduce placeholder implementations that appear functional.
- Do not claim an implementation is complete if important behavior is unsupported.

## 5. C++ requirements

- Use C++20.
- Prefer explicit types and clear ownership.
- Check bounds before memory access.
- Avoid integer overflow and unsafe narrowing conversions.
- Handle invalid and truncated input explicitly.
- Ensure failed operations do not partially modify CPU state or guest memory unless explicitly documented.
- Keep decoder, CPU state, interpreter and memory responsibilities separate.
- Maintain compatibility with the project's Windows and Visual Studio CMake build.

## 6. Testing and verification

For every implementation task:
1. Add tests for new behavior.
2. Add boundary and failure tests where relevant.
3. Preserve all existing tests.
4. Build the VS2026 Debug configuration.
5. Run CTest.
6. Run `git diff --check`.
7. Inspect the complete diff.
8. Report every command and its actual result.

Never claim that a build or test passed unless it was actually executed successfully.

If a command fails:
- report the exact failure;
- identify the likely cause;
- fix only the relevant issue;
- rerun the affected checks.

## 7. Scope control

- Only modify files required by the approved task.
- Do not perform opportunistic refactoring.
- Do not change TestCpu, GuestMemory or ExecutionPolicy unless explicitly required.
- Do not introduce new dependencies without approval.
- Do not add new architecture layers without a concrete requirement.

## 8. Git and user control

- Never commit, push, merge or create a release.
- Never rewrite Git history.
- Do not discard or overwrite existing user changes.
- Keep implementation changes available for user review.
- Report the current branch and changed files.
- The user performs all GitHub Desktop and GitHub operations.

## 9. Final report

At the end of every task, report:

1. What changed
2. Which files changed
3. What was deliberately not implemented
4. Tests added or changed
5. Build result
6. CTest result
7. `git diff --check` result
8. Known limitations and unresolved issues
9. Any manual action required from the user

Separate verified results from assumptions and unresolved questions.