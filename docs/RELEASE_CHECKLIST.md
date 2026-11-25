# Project Completion Checklist

Development-only document. Not part of the product package.

`FINAL_CLEANUP` must not start before feature-complete status and explicit owner approval. This checklist has **not** been executed by the agent-system setup task.

## Gates

- [ ] Feature-complete status approved by project owner.
- [ ] `FINAL_CLEANUP` package approved with explicit owner sign-off.
- [ ] Product allowlist approved; no product manifest or CMake install rule was added during agent-system setup.

## Development infrastructure preservation

- [ ] `.opencode/**` remains in the private development repository.
- [ ] `opencode.jsonc` remains in the private development repository.
- [ ] `AGENTS.md`, `ai/**`, and agent/workflow docs remain in the development repository.
- [ ] Agents remain usable for post-release bug fixes and updates. Only six agents are installed (`coordinator`, `architect`, `developer`, `tester`, `reviewer`, `researcher`); the nine specialists, including `test-engineer`, remain `planned` and must not be reported as installed.

## Product filtering

- [ ] Product tree contains no `.opencode/**`, `opencode.jsonc`, `AGENTS.md`, `ai/**`, internal prompts, workflow docs, or internal automation.
- [ ] Product tree contains no temporary prototypes, debug helpers, logs, `build/**`, `.kilo/**`, or local development artifacts.
- [ ] Product source contains no AI service calls, API-key handling, OpenCode references, or agent communication.
- [ ] Any AI feature explicitly approved as a product function is documented separately; otherwise it must be absent.

## Code and structure review

- [ ] Source and directory structure reviewed for clarity and maintainability.
- [ ] Unnecessary complexity and leftover prototypes removed only with owner-approved scope.
- [ ] Public interfaces and documented contracts still consistent.

## Verification after cleanup

- [ ] Build executed after cleanup; exact command and result recorded.
- [ ] CTest executed after cleanup; exact command and result recorded.
- [ ] `git diff --check` executed; result recorded.
- [ ] Product smoke test executed from the filtered product tree.
- [ ] Application demonstrated without AI services, agents, or development configuration.
- [ ] No unverified claim marked as passing.

## Portability and infrastructure items (SETUP-004 / 005 / 006)

These items are development infrastructure only. None of them is a `FINAL_CLEANUP` gate and none may be marked complete without recorded commands and results in `docs/AGENT_TASKS.md`.

- [ ] **Session portierung (owner task):** All 13 OpenCode sessions exported with `session export` and imported on the target drive — three project sessions into the main checkout, ten setup sessions into a separate permanent portable-setup context. Originals on `C:` untouched; no active SQLite database copied. **Still open:** the inspected portable state directory is empty; no imported session database was found. Agent roles deny shell `opencode *` and no export/import tool exists, so this step is not automatable by agents.
- [x] **VS2026 generator (verified):** `CMakePresets.json:7` uses `Visual Studio 18 2026`; `CMakePresets.json.backup:7` still holds `Visual Studio 17 2022`; `ninja-debug` is unchanged. The tester executed configure, Debug build and CTest with the absolute VS2026 executables from `CMakeCache.txt`; all three returned exit `0`, CTest reported `100% tests passed, 0 tests failed out of 1`. `git diff --check` returned exit `0` with only the informational CRLF warning. Exact commands and outputs are recorded in `docs/AGENT_TASKS.md`. `unknown`: plain `cmake`/`ctest` resolution in other shells was not verified.
- [ ] **Worktree verification (blocked):** `worktree_create` again returned `Created worktree could not be opened`. The checkout at `../worktrees/portability-worktree-test` exists but was not registered by `worktree_list`. No speculative configuration change was made. Owner decision required for cleanup and for any further worktree attempt.
- [ ] **Build-Ordner:** `build/` contains `windows-debug`, `windows-debug-vs2022` and `windows-2026-debug`. `windows-debug-vs2022` has VS solution/project files but **no `CMakeCache.txt`**, so it is not a complete independent backup; the original generator line is preserved in the untracked `CMakePresets.json.backup`. No build folder was deleted or overwritten.
- [ ] **Second-PC confirmation:** No portability test on a second, independent machine has been executed. `docs/PORTABLE_SETUP.md` must not claim one.

`documented`: owner decisions and the approved package scope. `observed`: the verified VS2026 preset line with recorded exit-0 configure/build/CTest, the worktree error and the unregistered checkout. The earlier 20-entry Git status was a historical snapshot of that verification pass and is **superseded**; the current working-tree inventory is recorded in `docs/AGENT_TASKS.md`. `observed` — XB-0086 read-only audit re-checked the cited evidence and confirmed it is still findable; no build, CTest or publication was rerun. `unknown`: session-transfer success, plain `cmake`/`ctest` resolution in other shells, worktree opening on the target drive and any second-PC test.

## Owner sign-off

- [ ] Project owner accepts the product artifact and schedules publication.

## Repository publication policy

The XBridge development repository is public. Project-related AI/agent configuration, prompts, workflow documentation, research and automation may be committed and published here. These materials are excluded only from a future end-user product package unless explicitly selected by its reviewed allowlist; they are not excluded from this public repository. Only private personal data and actual secrets/credentials are excluded from publication. Older checklist wording that calls this a private development repository is superseded by this policy.
