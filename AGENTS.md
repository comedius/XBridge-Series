# XBridge Series – Agent Instructions

## 1. Project overview

XBridge is an experimental Windows x64, C++20 project investigating an Xbox Series X emulator architecture. It is an early foundation and research scaffold, **not a working Xbox emulator**; it does not currently run Xbox games or Xbox executables.

Primary priorities:
1. Correctness
2. Stability
3. Consistent frame times
4. Performance
5. Graphics quality

## 2. Architecture rules

- Target Xbox Series X only. Xbox Series S is out of scope as a separate target.
- Keep the interpreter as the safe CPU execution baseline; JIT is excluded from the current baseline.
- Keep guest CPU semantics separate from host CPU capabilities.
- Keep GuestMemory generic. Do not assume it represents the Xbox virtual-memory system.
- Prefer hardware acceleration, with a safe software/reference fallback where feasible.
- Do not switch execution paths in the middle of an operation unless explicitly designed and tested.
- Manage host RAM and VRAM dynamically. Never reserve all host RAM for guest memory.
- Preserve existing public interfaces unless the task explicitly requires a reviewed change.
- Do not make Xbox-specific claims based only on research about other consoles.
- Xbox 360, Xbox One and original Xbox compatibility are deferred, separate future considerations.

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

Keep the project independent of proprietary Xbox software and assets. Do not make compatibility, playability or performance claims without the required evidence and approved protocol.

## 4. Mandatory task workflow

Before editing:
1. Read relevant headers, implementations, tests and documentation.
2. Inspect current Git status and existing changes when operating in a local checkout.
3. Identify dependencies and public interfaces.
4. Prepare one complete implementation plan for the whole approved package.
5. List affected files, acceptance criteria, risks and tests.

A package-level owner approval authorizes all changes inside its explicit envelope; do not request repetitive per-file or per-change approvals. Stop only for a genuine owner decision, scope conflict, security/legal issue, destructive action, or blocker.

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
4. Build the VS2026 Debug configuration when available.
5. Run CTest.
6. Run `git diff --check`.
7. Inspect the complete diff.
8. Report every command and its actual result.

Never claim that a build or test passed unless it was actually executed successfully. If local execution is unavailable, clearly mark it as not run.

## 7. Scope control

- Only modify files required by the approved task.
- Do not perform opportunistic refactoring.
- Do not change TestCpu, GuestMemory or ExecutionPolicy unless required by the approved package.
- Do not introduce new dependencies without owner approval.
- Do not add new architecture layers without a concrete requirement.

## 8. Git and user control

- Agents never commit, push, merge, tag, release, rewrite history, discard changes or create pull requests.
- Do not overwrite or discard existing user changes.
- The project owner controls Git/GitHub publication.
- Report the current branch and changed files.

## 9. Final report

Report:
1. What changed and which files changed
2. What was deliberately not implemented
3. Tests added or changed
4. Build, CTest and diff-check results (actual results only)
5. Known limitations and unresolved issues
6. Manual action required from the owner

Separate verified results from assumptions and unresolved questions.

## 10. Development infrastructure vs product

This is a **public project repository**. Project-related AI/agent prompts, OpenCode configuration, workflow documents, research, automation and development tools may be stored here. Do not describe them as private or prohibited from publication merely because they are AI-related.

Development infrastructure is not automatically part of a future end-user product distribution. The product package must later be produced from an explicit, reviewed allowlist. Do not add AI runtime behavior, API-key handling or service dependencies to product code unless separately approved as a product function with its own ADR and tests.

A `FINAL_CLEANUP` package may start only after feature-complete status and explicit owner approval. It must not delete development materials still needed for maintenance.

## 11. Agent system control

- The coordinator is the primary agent; architect, developer, tester, reviewer and researcher are subagents.
- Subagents do not start further subagents. The coordinator routes all hand-offs.
- Every package follows the envelope in `docs/AGENT_WORKFLOW.md`.
- Agents do not publish Git/GitHub changes. The project owner performs publication.
