# XBridge Agent Workflow

This document is **development infrastructure**. It is not part of the product package.

## Authority order

When sources conflict, stop and ask the project owner. Do not silently choose.

1. `AGENTS.md`
2. `docs/DECISIONS.md` (accepted ADRs)
3. `docs/ARCHITECTURE.md`, `docs/RESEARCH_REGISTER.md`, and other project documentation
4. This workflow document
5. Agent prompt files under `.opencode/agents/`

## Package envelope

Every development package must contain all of these fields:

```text
PACKAGE_ID:        XB-0001
PACKAGE_TYPE:      FEATURE | BUGFIX | RESEARCH | FINAL_CLEANUP
PRODUCT_BOUNDARY:  product | development-only | excluded-from-product
GOAL:              one-sentence goal and reason
FILES:             affected files
ALLOWED:           allowed changes
FORBIDDEN:         forbidden changes
ACCEPTANCE:        measurable acceptance criteria
TESTS:             required tests and verification commands
REVIEW:            architect/tester/reviewer requirements
WORKTREE:          assigned local worktree
OWNER_APPROVALS:   required decisions before implementation
```

A package without a complete envelope is not executable.

## Roles

| Agent | Mode | Main duty |
|---|---|---|
| `coordinator` | primary | Plans packages, delegates, tracks state, asks owner decisions |
| `architect` | subagent | Read-only interface, ADR, dependency and risk analysis |
| `developer` | subagent | General product writer for approved packages; may add tests when the package grants `tests/**` |
| `tester` | subagent | Independent tests, build/CTest/diff verification |
| `reviewer` | subagent | Strictly read-only diff and evidence review |
| `researcher` | subagent | Evidence-labeled public-source research |

Subagents do not start further subagents. The coordinator is the only central router.

**Installed-role status (authoritative for what can execute today):** only the six agents in the table above are installed. Specialist roles below are `planned`, not installed; `test-engineer` in particular remains future-planned and must never be reported as installed. Under the installed permission files, both `developer` and `tester` may write `tests/**`; there is no installed exclusive test owner. Statements about specialist ownership apply only after the owner installs and permits those roles.

## Standard flow

1. Coordinator creates the complete package.
2. Architect checks architecture, interfaces, ADRs, risks, and boundary classification.
3. Project owner approves any required decision.
4. Coordinator creates/assigns the worktree through the OpenCode worktree function.
5. Developer implements only the approved scope and adds tests when that package explicitly grants `tests/**` writes. With the installed roles, `developer` and `tester` both have that permission; there is no installed exclusive test owner.
6. Tester independently verifies without weakening tests.
7. Reviewer performs the read-only review.
8. Coordinator records results and open points in `docs/AGENT_TASKS.md`.
9. Project owner performs all Git/GitHub publication.

Maximum two normal review correction rounds. After that the coordinator escalates with findings and a concrete decision request.

## Product / development boundary

The private development repository keeps all agent systems permanently, including:

- `.opencode/**`
- `opencode.jsonc`
- `AGENTS.md`
- `ai/**`
- internal agent prompts and commands
- `docs/AGENT_WORKFLOW.md`, `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`, `docs/RELEASE_CHECKLIST.md`
- internal automation and temporary prototypes

These paths must never be copied automatically into a product package. The product package will later be created from an explicit allowlist. No final product manifest or CMake install rules are part of this setup task.

`FINAL_CLEANUP` may start only after feature-complete status and an explicit owner approval. Cleanup means producing a filtered product tree; it does not mean deleting the development repository or its agent configuration.

## Permission model

- Effective configuration is V2 syntax in `opencode.jsonc` plus per-agent Markdown frontmatter.
- Global rules are loaded before agent rules; the last matching rule wins.
- Configured `deny` rules override saved approvals. Top-level `policies` are hard denials and cannot be granted by a permission rule.
- Shell starts with `ask`; only the listed Git, CMake and CTest commands are allowed. They are not all read-only: CMake configure/build and CTest write outputs under `build/**`.
- Unknown shell commands remain `ask`. Never use blanket `--auto` in agent runs.
- External directories require confirmation by default.
- Git/GitHub publication commands and global Git configuration changes are hard-denied by policy.
- The reviewer has a per-agent `edit: * = deny`. OpenCode V2 policies are not agent-scoped, so this is an agent configuration deny plus review verification, not a global per-agent policy.
- Shell pattern matching is not an operating-system sandbox. A sufficiently authorized human can still bypass tool permissions outside OpenCode; the rules protect agent behavior, not the host account.

## Verification and review

Required evidence for a completed package:

- exact commands run;
- actual results/exit states;
- changed files;
- `git diff --check`;
- test additions or an explanation why none were needed;
- reviewer verdict with file:line references;
- unresolved questions.

Build/debug verification follows `AGENTS.md`. The VS2026-versus-VS2022 preset discrepancy is resolved: `windows-debug` uses `Visual Studio 18 2026`, and configure/build/CTest were executed successfully with exit `0` using the absolute VS2026 executables recorded in `docs/AGENT_TASKS.md`. The original VS2022 line is preserved only in a local, unversioned `CMakePresets.json.backup`; that file is not part of the repository and is not published through any branch or pull request. The backup file remains excluded from publication. Plain `cmake`/`ctest` resolution in other shells remains `unknown`.

## Evidence labels

Use these labels for every technical claim:

- `documented` — explicit in an authoritative source or accepted project document;
- `observed` — directly seen in code, test output, or a reproducible local check;
- `hypothesis` — plausible but unverified;
- `unknown` — not established.

Never present `hypothesis` or `unknown` as implemented behavior.

## Worktrees

- Use the OpenCode worktree functions only. Do not use Git shell commands to create, switch, merge, or remove worktrees.
- A setup worktree must never be automatically merged or removed by an agent.
- If a worktree is created across different filesystem drives, Git may reject it with `dubious ownership`. Prefer the configured `../worktrees` location on the repository drive; never fix this by silently changing global Git configuration.

## Portable development setup

Agents and the project owner must work from the currently opened project directory, never from a previous machine path. See `docs/PORTABLE_SETUP.md` for second-PC setup, tool discovery, fresh build directories, and known portability limitations. Product code and the product package must not depend on this document or on any OpenCode configuration.

### Session portability

- All 13 existing OpenCode sessions must be transferred with the supported `session export` / `session import` commands only. The three XBridge project sessions belong to the main checkout; the ten setup sessions belong to a separate, permanent portable-setup context on the target drive.
- Never modify or delete the originals on `C:`. Never copy an active SQLite database directly; export through the supported OpenCode commands instead.
- Agent roles deny shell `opencode *`, and `session export/import` is not in the global shell allowlist. The project owner therefore performs these commands manually outside agent runs. Agents only record the reported outcome in `docs/AGENT_TASKS.md`.

### Visual Studio generator

- Historical decision: `windows-debug` was changed from `Visual Studio 17 2022` to `Visual Studio 18 2026`; the verification record is described above. `ninja-debug` must remain unchanged.
- After the change, configure, build, and test with CTest. If any step fails, restore the original preset state and document the exact failure.
- No agent role has edit permission for `CMakePresets.json`; the project owner performs this approved edit. The tester role runs the verification commands and reports exact commands and results.

### Worktree verification

- Worktrees must be created, inspected, and removed through the supported OpenCode functions (`worktree_create`, `worktree_list`, `worktree_remove`), never through Git shell commands.
- On the current installation, `worktree_create` returned `Created worktree could not be opened`. The checkout directory was created but was not registered by `worktree_list`. If opening fails again, stop this part immediately, document the error, and make no speculative configuration changes.
- A setup worktree is never automatically merged or removed by an agent. Cleanup of a failed test worktree requires an owner decision.

## Autonomous development cycle

This section defines the coordinator-controlled development loop. It is development infrastructure only.

Current execution note: this loop describes the planned specialist model. Only the six installed agents exist today; every reference below to a specialist is future-planned and must be performed by an installed-role substitute under an approved envelope until the owner installs that specialist. In particular `test-engineer` remains `planned`, not installed.

### Mandatory sequence per package

1. Coordinator selects the next prioritized package from the queue below and emits the full envelope.
2. Architect reviews interfaces, ADRs, boundaries and risks when the package touches an interface or more than one module. For documentation-only, evidence-only or single-file test additions the coordinator may skip the architect and record why.
3. Coordinator assigns the package to exactly one owning agent. Parallel assignment is allowed only when the write sets are disjoint.
4. The assigned implementing agent (installed `developer`, or `coordinator` for documentation-only packages) makes the smallest coherent change and adds tests only when its package grants `tests/**` writes.
5. Independent verification is performed by installed `tester` (or `reviewer` for documentation-only packages) without weakening tests; `test-engineer` is `planned` and not installed.
6. Reviewer performs the read-only review with file:line findings and a `PASS` / `NEEDS_CHANGE` / `BLOCKED` verdict.
7. Planned `integration-engineer` (not installed) is the intended integration checker; until then the coordinator performs read-only composition checks with installed roles.
8. Coordinator evaluates the result, assigns concrete correction packages for failed checks, and never marks a failed check as passed.
9. Coordinator updates `docs/AGENT_TASKS.md` and `docs/ROADMAP.md` with the recorded commands, results and open points.
10. Coordinator starts the next unlocked package automatically. Owner decisions are the only reason to pause.

A failed check produces a correction package with the exact failing command and output. After two normal correction rounds the coordinator escalates with findings and a concrete decision request.

### Role catalogue and ownership

Installed and validated (existing): `coordinator` (primary, sole orchestrator), `architect`, `developer`, `tester`, `reviewer`, `researcher`.

Planned specialists (status `planned` — not installed until the owner grants scoped write access to `.opencode/agents/**`; do not report them as active):

| Agent | Owns (write) | May read | Must not |
|---|---|---|---|
| `cpu-developer` | `src/cpu/**`, `include/xbridge/cpu/**` | `include/xbridge/memory/**`, docs | Change `TestCpu` or `GuestMemory`, implement JIT, edit `tests/**`, touch other modules |
| `memory-developer` | `src/memory/**`, `include/xbridge/memory/**` | CPU interfaces, docs | Add Xbox memory semantics or platform-specific physical memory, edit `tests/**` |
| `gpu-developer` | `docs/design/gpu/**` only, until an approved ADR defines a backend interface | `src/**`, `include/**`, architecture docs | Claim D3D12.X equivalence, invent guest command formats, write product code without ADR |
| `audio-developer` | `docs/design/audio/**` only, until an approved ADR exists | architecture docs, host interfaces | Invent Xbox audio APIs or claim real-time fallback, write product code without ADR |
| `input-developer` | `docs/design/input/**` only, until an approved ADR exists | architecture docs, host interfaces | Invent Xbox input protocols or claim game compatibility, write product code without ADR |
| `platform-developer` | `src/host/**`, `src/profiles/**`, `include/xbridge/host/**` | core runtime, docs | Mix host facts into guest semantics, change `ExecutionPolicy` contract without approval, edit `tests/**` |
| `test-engineer` | `tests/**` (exclusive owner) | all product sources | Weaken, skip or delete tests; change product code |
| `performance-engineer` | `docs/performance/**` (measurement reports only) | all sources, build config | Change guest-visible behavior, edit `tests/**` without a separate package, apply unmeasured optimizations |
| `integration-engineer` | no edit rights; read-only verification, findings handed to coordinator | all sources, tests, config | Edit product code or docs, merge, tag or publish |

Planned test ownership (applies only after the owner installs `test-engineer`): specialist developers do not write tests; they hand a behavior/boundary/failure test matrix to the coordinator, who assigns it to `test-engineer` exclusively. Installed reality today: `test-engineer` is `planned`, not installed, and both `developer` and `tester` may write `tests/**` under their agent files; the coordinator assigns one owner per package to keep write sets disjoint. Design-only roles use `docs/design/**` so they do not collide with `researcher`'s `docs/RESEARCH_REGISTER.md` and `docs/research/**`.

Every specialist: `mode: subagent`, `subagent:*=deny`, `question:*=deny` (except coordinator), `shell opencode *=deny`, and `webfetch`/`websearch` deny unless the role is explicitly a research role. Inherit the global hard policies; never add blanket shell, file or system access.

### Delegation matrix

Rows naming a specialist are future-planned designs, not executable assignments today. Until the owner installs that role, the coordinator assigns an installed substitute in the approved package envelope. In particular `test-engineer` is `planned`, not installed.

| Package type | Owner | Required reviewers | Parallel-safe? |
|---|---|---|---|
| CPU decoder/interpreter | `cpu-developer` | `architect` (interface), `test-engineer`, `reviewer`, `integration-engineer` | Only against disjoint memory/host files |
| GuestMemory scaffold | `memory-developer` | `architect`, `test-engineer`, `reviewer` | Only against disjoint CPU files |
| Host capabilities / profiles | `platform-developer` | `architect`, `test-engineer`, `reviewer` | Only against disjoint CPU/memory files |
| GPU/audio/input research | `researcher` + the matching design-only specialist | `architect`, `reviewer` | Yes, docs paths must be disjoint |
| Build/test infrastructure | `test-engineer` or `tester` | `reviewer` | Yes, no product source overlap |
| Performance measurement | `performance-engineer` | `test-engineer`, `reviewer` | Yes, tests-only artifacts |
| Cross-module integration | `integration-engineer` (verification only) | `reviewer` | After the owning developers finish |
| Agent system / workflow docs | `coordinator` | `reviewer` | Only when file sets are disjoint |

### Task queue and milestones

The current queue lives in `docs/AGENT_TASKS.md` (packages) and `docs/ROADMAP.md` (phases). A package may start when: its envelope is complete, its owner is installed and permitted, its write set does not overlap an active package, and no owner decision is pending. The coordinator re-plans after each integration result; this document does not freeze the order.

### Quality and release criteria

- Every new behavior has a test; boundary and failure cases are included.
- Build, CTest and `git diff --check` are executed with exact commands and actual results recorded.
- No claim is marked `pass` without recorded evidence; `hypothesis` and `unknown` stay visible.
- Product code contains no agent prompts, OpenCode references, API keys or AI service calls.
- Fundamental architecture changes, public interface changes, destructive actions and strategic changes require an owner decision. Everything else within an approved envelope is automatic.

### Parallel change rules

- One package owns each file path at a time; the coordinator records the write set in the envelope.
- Shared files (`CMakeLists.txt`, `CMakePresets.json`, `opencode.jsonc`, `.opencode/**`, `AGENTS.md`) are coordinator-serialized and are never edited by two agents concurrently.
- A developer never edits another specialist's module. Interface changes go through the architect and an owner decision.
- Conflicting changes are stopped, not merged blindly.

### Handoff and error recovery

- Handoff payload: package ID, exact write set, changed files, exact commands and outputs, test results, deliberate omissions, evidence labels, unresolved questions.
- The coordinator passes this payload to the next agent; the owner is not a messenger.
- On a failed check the coordinator creates a correction package referencing the exact failure. On a permission rejection the coordinator requests the minimal scoped permission instead of broadening access or bypassing the rule.
- Progress survives interruption because the ledger records package status, evidence and open decisions.

### Background-capable work versus manual steps

- Subagent runs (architect, developer, tester, reviewer, researcher) execute in the background of an OpenCode session; the coordinator collects their results and continues other work meanwhile. Code Mode worktree functions are available directly.
- Shell access for the coordinator is limited by role rules (`cmake`, `ctest`, `opencode` are denied); verification is delegated to `tester`, which may run the allowlisted commands.
- Not automatable by agents: `opencode session export/import` (no allowlisted command, no Code Mode tool, and all roles deny shell `opencode *`), editing `.opencode/**` or `opencode.jsonc`, and installing or changing permissions. These require one owner action each; the coordinator bundles such requests.
- Interactive TUI permission prompts and a second-PC test remain `unknown`.
