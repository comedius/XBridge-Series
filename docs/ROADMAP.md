# XBridge Series Roadmap

This roadmap describes development phases. It is development documentation and is not part of the product package. Detailed task history remains in `docs/AGENT_TASKS.md`.

## Phase 1 — Foundation (current state)

- C++20 CMake project with `xbridge_core`, CLI demo, and CTest.
- Generic `GuestMemory`, synthetic `TestCpu`, `X64CpuState`.
- Deterministic host execution policy with mock capabilities.
- x64 NOP/MOV decoder and reference interpreter subset.

## Phase 2 — Agent-assisted development

- Private development repository with the coordinator and five subagents (installed and validated).
- Package-based workflow defined in `docs/AGENT_WORKFLOW.md`.
- Strict separation of product code and development infrastructure.
- All Git/GitHub publication remains with the project owner.
- **Planned expansion (not installed):** nine additional specialists — `cpu-developer`, `memory-developer`, `gpu-developer`, `audio-developer`, `input-developer`, `platform-developer`, `test-engineer`, `performance-engineer`, `integration-engineer`. Their ownership and the autonomous cycle are specified in `docs/AGENT_WORKFLOW.md`. Installation requires a scoped owner permission for `.opencode/agents/**`; until then the coordinator uses the five existing subagents and never reports the new roles as active.

### Current milestone queue (development infrastructure)

1. `SETUP-004` — 13-session portability transfer (owner-only, still open).
2. `SETUP-005` — VS2026 generator: **verified pass** with absolute CMake/CTest paths.
3. `SETUP-006` — worktree opening on E:: blocked; unregistered test checkout awaits an owner decision.
4. `SETUP-007` — specialist-team rollout: **blocked**; agent files await persisted owner permission. Independent reviewer verdict recorded as `NEEDS_CHANGE` (test-ownership conflict, stale `PORTABLE_SETUP.md`, inconsistent ledger status).
5. `RESEARCH-007` — analysis of `omonkeyz/xbox-emulator` and upstream `drycheetah84/TomBox`: **complete, review PASS**. Researcher and architect both returned; five dated research files plus recommendations and four derived packages (`planned`) exist. Only 6 agents installed — the nine requested specialists are not available, so their analysis areas were covered by `researcher` and `architect`. No product code, architecture or ADR changes. Independent reviewer: round 1 `NEEDS_CHANGE` (four light documentation-only fixes), correction package `RESEARCH-007-C1` applied, final review `PASS`. Research yields documentation and derived packages only, not automatic code adoption.
6. Deferred product milestone after Phase 2 installation: RIP-based x64 single-step bridge with tests, using `cpu-developer` + `test-engineer` + `reviewer` (currently blocked by SETUP-007 permissions; not the current next package).
7. Implementation-phase preparation (`PLAN-001`): agent-system audit complete (all nine specialists physically absent and non-callable; no rule changed), XB-0080–0083 analysed, prioritization = XB-0081 → XB-0080 → XB-0083 → XB-0082. XB-0081 **complete with reviewer PASS** (owner-approved four-file scope, coordinator sole writer, `test-engineer` remains future-planned). Reviewer round 1 `NEEDS_CHANGE` → correction round 1 → final `PASS`; 26 working-tree entries preserved. No other package started; `XB-0080`, `XB-0083`, `XB-0082` remain `planned` and require separate owner approval.
8. `XB-0084` — GTA VI performance-first compatibility assessment and separate Xbox 360 / Splinter Cell: Double Agent backward-compatibility planning: owner-ordered `RESEARCH` package, `development-only`, **assessment/planning only, no implementation approved**. Six `docs/research/XB-0084_*.md` documents plus ledger/roadmap records; four read-only analyses (architect, developer, tester, researcher) fed the single `researcher` write set. Independent review round 1 `NEEDS_CHANGE` (five citation defects) → correction package `XB-0084-C1` → final reviewer `PASS`; 26 pre-existing working-tree entries preserved plus exactly six new documents; `git diff --check` EXIT=0. Envelope, commands and evidence in `docs/AGENT_TASKS.md`. This package does not make Xbox 360 a current target, does not change ADR-0004/`ARCHITECTURE.md`, and does not start `XB-0080`, `XB-0083` or `XB-0082`.

9. `XB-0085` — **complete read-only publication audit; independent reviewer PASS after one correction round.** 32 entries audited: 13 `READY_FOR_COMMIT`, 18 `NEEDS_REVIEW`, 1 `DO_NOT_PUBLISH`. No publication, no index/reference changes; PR #1 remains open and untouched. PR state was not independently re-verified by XB-0086 agents.

10. `XB-0086` — **complete documentation cleanup.** Coordinator-only correction of status/path/sequence documentation in the four permission-approved coordinator files, followed by owner-approved `XB-0086-C1` completion of `docs/PORTABLE_SETUP.md` and the final `XB-0086-C2` status alignment; four read-only audits completed. Independent reviewer `PASS` for the four files (`ses_dfb4f6c93ffehZ6NiedpQBdZ0I`), final `PASS` for the fifth file after one correction (`ses_dfb38042fffeU3MOxqXVwnB7vM`) and final `PASS` for `XB-0086-C2` (`ses_dfb25ec0bffeJ2AVA6w21ArW6R`). `XB-0086-C2` updated the XB-0086 status in this roadmap and added the definition of publication group 0 in `docs/AGENT_TASKS.md`; group 0 consists of exactly these five documentation files: `docs/AGENT_WORKFLOW.md`, `docs/RELEASE_CHECKLIST.md`, `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`, `docs/PORTABLE_SETUP.md`. All documentation work is complete and independently reviewed; completion does not imply approval for publication or merge. No product, test, CMake, preset, build, CTest, ADR, interface, dependency, PR or publication action; no agent-initiated permission broadening (the owner's scoped edit allow is recorded at `docs/AGENT_TASKS.md:570`).

11. `XB-0080-A/B` — **planned / blocked by open owner decisions.** Register schema/conformance (`XB-0080-A`) and workflow/ledger integration (`XB-0080-B`) remain `planned`; they must not start until the owner resolves the split write set and start approval recorded in `docs/AGENT_TASKS.md`.

12. GTA VI implementation — **not approved.** XB-0084 ended at assessment/planning only. Any first GTA VI implementation package requires separate preparation and explicit owner approval distinct from XB-0084; no compatibility or performance claim is authorized.

Current sequence: (1) **complete** — owner persisted the scoped `docs/PORTABLE_SETUP.md` edit rule and reloaded OpenCode; (2) **complete** — coordinator applied the prepared corrections and the reviewer checked that file; (3) resolve publication groups and owner decisions; (4) start `XB-0080-A/B` only after approval; (5) separately prepare and approve the first GTA VI implementation packages arising from XB-0084.

No package may be marked complete without recorded commands and results in `docs/AGENT_TASKS.md`.

## Phase 3 — CPU and host milestones

- RIP-based x64 single-step bridge, then controlled run loop.
- Additional instruction forms only with tests and explicit contracts.
- Real host capability detection separate from guest semantics.
- Backend initialization and stability telemetry only with approved designs.
- No JIT without separate evidence, design, and tests.

## Phase 4 — Feature complete

All planned product functionality is implemented and verified. Only then may a `FINAL_CLEANUP` package be proposed.

## Phase 5 — FINAL_CLEANUP

- Build a product tree from an explicit allowlist.
- Exclude all AI/development/agent infrastructure and temporary artifacts.
- Check source and directory structure for unnecessary complexity.
- Re-run build, CTest, and diff checks after cleanup.
- Prove the application works without AI services, agents, or development configuration.

See `docs/RELEASE_CHECKLIST.md`. The development repository and all agent configuration remain intact for post-release fixes and updates.

## Phase 6 — Owner release and post-release maintenance

The project owner performs all publication steps. The private development repository continues to host agents, prompts, roadmaps, and test infrastructure.
