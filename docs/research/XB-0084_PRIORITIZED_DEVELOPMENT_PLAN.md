# XB-0084 — Prioritized development plan (assessment only)

- **PACKAGE_ID:** XB-0084
- **PACKAGE_TYPE:** RESEARCH
- **PRODUCT_BOUNDARY:** development-only
- **ACCESS DATE:** 2026-11-22
- **STATUS:** assessment/planning only; no implementation approved.
- **Evidence labels:** `documented` (explicit in an authoritative source or accepted project document), `observed` (directly seen in code, test output, or a reproducible local check), `hypothesis` (plausible but unverified), `unknown` (not established).
- **Boundary:** Development-only research document. Not product code, not an approved implementation package, and not a claim that any phase has started or completed.

## Planning basis and fixed constraints

- `documented` — XB-0084 is assessment/planning only; implementation is not approved. `docs/AGENT_TASKS.md:469-497`, especially `:477`, `:488`, `:494`.
- `documented` — Xbox Series X is the only current target; Xbox One, Xbox 360 and original Xbox compatibility are deferred, optional and isolated future modules. `docs/ARCHITECTURE.md:11-13`; `docs/DECISIONS.md:18-21`.
- `documented` — Interpreter-first remains the safe CPU baseline; JIT is not implemented and requires separate evidence, design and tests. `docs/ARCHITECTURE.md:78-95`, `:210-213`; `AGENTS.md:19-20`.
- `documented` — `GuestMemory` is generic and must not become an Xbox virtual-memory model without an approved scope change. `docs/ARCHITECTURE.md:30-39`; `docs/DECISIONS.md:38-41`.
- `documented` — Host capabilities affect implementation and performance, not guest identity or semantics. `AGENTS.md:19-25`; `docs/DECISIONS.md:23-31`.
- `documented` — Do not switch execution paths mid-operation; use safe points and explicit reporting. `docs/HOST_HARDWARE_POLICY.md:20-28`; `docs/ADAPTIVE_EXECUTION.md:17-25`.
- `documented` — No dumps, firmware, keys, decrypted DLLs, game assets, protected content, DRM circumvention or copied third-party text/code may be introduced. `AGENTS.md:39-45`; `docs/LEGAL_COMPLIANCE.md:28-49`; `docs/AGENT_TASKS.md:488`.
- `documented` — The twelve GTA VI component areas are fixed by owner order and are not replaced here. `docs/AGENT_TASKS.md:499`; `docs/research/XB-0084_GTA_VI_COMPATIBILITY_INVENTORY.md`.
- `observed` — Local scaffold facts and subagent findings are attributed to architect handoff `ses_dfb8b2c84ffed9nKKaN1zVSUCM`, developer handoff `ses_dfb8af93bffeqPGOh2dL4DmGmO` and tester handoff `ses_dfb8aabfbffezkAi7jlMKnothv`; this researcher did not re-run their commands.
- `hypothesis` — The ten-phase sequence below is a dependency-safe planning model, not a guaranteed schedule or completion claim.

## Why the sequence starts with evidence and contracts

- `documented` — Current XBridge runtime executes a synthetic `TestCpu` program and prints “scaffold demo completed (not Xbox emulation).” Architect handoff; `src/core/runtime.cpp:15-38`.
- `documented` — The x64 decoder covers NOP and MOV forms only; there is no SIMD/FPU/privileged instruction support, no flag/control-flow corpus and no integrated fetch-decode-execute loop. Architect/developer handoffs; `docs/ARCHITECTURE.md:97-180`.
- `documented` — No graphics, storage, audio, input or system module exists in the current build. Architect handoff; `CMakeLists.txt:10-37`.
- `documented` — Therefore, starting with GTA VI playability, an FPS proof, D3D12.x equivalence or Xbox 360 support would be a false completion path. `hypothesis` — The safe order is evidence and contracts first, then interpreter/memory correctness, then host telemetry, then backend-neutral graphics prototypes, then integration and measurement.

## Component-area coverage map

The fixed twelve areas are addressed across phases as follows:

| Fixed area | Earliest relevant phase | Current status basis |
|---|---|---|
| 1 CPU instructions/threading | Phase 3 | `partial`: synthetic CPU plus limited x64 scaffold. |
| 2 Memory/unified memory/bandwidth | Phase 4 | `partial`: generic `GuestMemory`; unified memory `unknown`. |
| 3 GPU/D3D12.x | Phase 6 | `unknown`: enums/selector only. |
| 4 RDNA2/shaders | Phase 6 | `unknown`: no shader module. |
| 5 Shader translation/caching | Phase 7 | `unknown`: no translator/cache. |
| 6 Resources/textures | Phase 7 | `unknown`: no resource model. |
| 7 System libraries/Xbox functions | Phase 8 | `unknown`: loader/system services non-goals/absent. |
| 8 Filesystem/storage | Phase 8 | `unknown`: no storage subsystem. |
| 9 Audio | Phase 8 | `unknown`: no audio subsystem. |
| 10 Input/controller | Phase 8 | `unknown`: no input subsystem. |
| 11 CPU/GPU/audio synchronization | Phase 9 | `unknown`: no timeline. |
| 12 Scheduling/resource management | Phase 5 | `partial`: pure deterministic selector only. |

## Phase 1 — Evidence register and six-document assessment

- **Goal:** `documented` — Establish the XB-0084 evidence baseline without product changes.
- **Dependencies/gates:** Owner envelope complete; sole write set limited to the six XB-0084 research documents; no product source/test/build changes.
- **Deliverables:** `hypothesis` — Six evidence-labeled documents covering GTA VI compatibility, performance methodology, Xbox 360/Splinter Cell separation, license assessment, prioritized plan and risks/dependencies.
- **Owner decisions:** None required to write the documents; owner decides whether to accept the assessment.
- **Tests to add later:** None in this phase; documentation-only.
- **Not implemented by XB-0084:** This phase does not add code, change interfaces, run builds/CTest, or create compatibility.

## Phase 2 — Owner/ADR decision register

- **Goal:** `hypothesis` — Convert open technical questions into explicit accepted/rejected decisions before product work.
- **Dependencies/gates:** Phase 1 accepted. Decisions must be recorded in accepted project documents, not inferred from this plan.
- **Deliverables:** ADRs or decision entries for frame pacing, GPU timeline, resource lifetime, shader cache keys/invalidation, streaming/residency, audio clock, deterministic scheduling, project license posture and optional Xbox 360 scope.
- **Owner decisions:** Required before any related implementation. No decision is made by XB-0084.
- **Tests to add later:** Tests for each approved contract; exact test matrix depends on the ADR.
- **Not implemented by XB-0084:** No ADR is accepted and no product interface is changed by this document.

## Phase 3 — Interpreter-first CPU correctness

- **Goal:** `hypothesis` — Build a documented x64 decoder/reference interpreter corpus with explicit status contracts before any JIT or native-execution discussion.
- **Dependencies/gates:** Phase 2 decisions; lawful public technical evidence; preserve decoder/interpreter signatures, status enums and no-partial-mutation behavior.
- **Deliverables:** Instruction-form matrix, truncated-byte cases, flag/control-flow semantics where evidenced, reset/state contract, deterministic replay corpus and explicit unsupported-opcode diagnostics.
- **Owner decisions:** JIT/native execution remains prohibited without separate evidence, design, tests and ADR.
- **Tests to add later:** Decoder conformance, interpreter conformance, truncated-byte sweeps, state/no-partial-mutation assertions and deterministic trace replay.
- **Not implemented by XB-0084:** No new instruction is added and no execution path is changed.

## Phase 4 — Generic memory hardening

- **Goal:** `hypothesis` — Strengthen generic byte-memory behavior and discuss a separate guest VM contract only under an approved scope.
- **Dependencies/gates:** Phase 3 corpus; ADR-0008 guest/host memory separation; no host mapping or Xbox VM behavior added to generic `GuestMemory`.
- **Deliverables:** Bounds/overflow/failure tests, documented generic contract, and a separate evidence-backed guest address-space design if approved.
- **Owner decisions:** Approval required before Xbox virtual-memory, paging, permissions, aliasing, reservation or GPU-visible allocation semantics.
- **Tests to add later:** Boundary, overflow, truncated input, allocation failure, alias/permission cases only if the contract exists, and generic concurrency checks where applicable.
- **Not implemented by XB-0084:** `GuestMemory` remains generic; no unified-memory model is introduced.

## Phase 5 — Host detection and deterministic telemetry

- **Goal:** `hypothesis` — Replace illustrative profile metadata with real host capability reporting while keeping host facts separate from guest semantics.
- **Dependencies/gates:** ADR-0005, ADR-0010/ADR-0011 and architecture host-detection requirements; immutable/mockable `HostCapabilities`; pure selector behavior preserved.
- **Deliverables:** Structured execution-plan report, opt-in monotonic instrumentation points, seeded replay support and safe-point policy reporting.
- **Owner decisions:** Approved profiling categories, telemetry boundary and rules against silent profile accuracy reductions.
- **Tests to add later:** Detector mocking, report stability, replay determinism, capability-gate tests and no-mid-operation-switch tests.
- **Not implemented by XB-0084:** No detector, telemetry field or policy change is added.

## Phase 6 — ADR-defined backend-neutral graphics contracts

- **Goal:** `hypothesis` — Define guest command, shader-input, IR and host-backend boundaries from evidence rather than assumed PC/Xbox equivalence.
- **Dependencies/gates:** Phase 2 ADRs; R-002/R-003 evidence; D3D12.X equivalence remains `unknown`; no dumps, DLLs, shaders or protected assets.
- **Deliverables:** Separate command decoder contract, shader IR contract, resource-state contract and backend abstraction; explicit unsupported-feature reporting.
- **Owner decisions:** Choose whether and how to proceed with backend-neutral contracts; do not declare D3D12.X equivalence.
- **Tests to add later:** Command decoding, invalid commands, shader-input boundary, resource-state transition and backend selection tests.
- **Not implemented by XB-0084:** No graphics device, command processor or shader module is created.

## Phase 7 — Synthetic graphics/resource prototypes

- **Goal:** `hypothesis` — Prototype translator/cache/resource behavior with original synthetic material and safe software fallback before title-oriented work.
- **Dependencies/gates:** Phase 6 contracts; lawful synthetic corpus; cache key/invalidation and resource lifetime decisions; hardware acceleration with safe fallback.
- **Deliverables:** Translator skeleton for documented cases, cache telemetry design, resource lifetime tests, residency/streaming model and controlled fallback reporting.
- **Owner decisions:** Cache keys/invalidation, eviction, streaming/residency and fallback limits.
- **Tests to add later:** Cache hit/miss/invalidation/eviction, cold/warm translation time, resource transition/lifetime and synthetic image correctness checks.
- **Not implemented by XB-0084:** No shader translator, cache, resource manager or backend is implemented.

## Phase 8 — Evidenced system, storage, audio and input design

- **Goal:** `hypothesis` — Define system-service, filesystem, audio and input contracts only from public evidence and synthetic test programs.
- **Dependencies/gates:** R-005/R-006 evidence; ADRs for audio and input design; no game files, firmware, keys or service endpoints.
- **Deliverables:** Documented call surface, unsupported-call reporting, guest-visible storage contract, audio clock proposal, input timestamping model and ADR package.
- **Owner decisions:** Audio clock, input protocol scope, storage/decompression contract and system-service boundary.
- **Tests to add later:** API coverage, invalid/truncated input, async I/O ordering, audio underrun, input latency with injected timestamps and unsupported-call behavior.
- **Not implemented by XB-0084:** No system library, storage, audio or input product code is added.

## Phase 9 — CPU/GPU/audio integration at safe points

- **Goal:** `hypothesis` — Integrate subsystem timelines without changing guest semantics or switching paths mid-operation.
- **Dependencies/gates:** Phases 3–8 contracts accepted; frame/presentation boundary, GPU timeline and audio clock decisions accepted.
- **Deliverables:** One stable frame start/present hook, timeline diagnostics, deterministic replay, safe-point profile changes and explicit degradation reporting.
- **Owner decisions:** Frame pacing, missed-deadline policy and whether any performance/stability trade-off is acceptable.
- **Tests to add later:** Frame pacing, stutter threshold, timeline ordering, audio/input latency, replay determinism and safe-point switching tests.
- **Not implemented by XB-0084:** No runtime frame loop or cross-subsystem integration is added.

## Phase 10 — Performance measurement and optional separate-generation planning

- **Goal:** `hypothesis` — Apply the pre-registered methodology from `XB-0084_GTA_VI_PERFORMANCE_PROFILE.md` to lawful workloads, and separately plan deferred backward compatibility.
- **Dependencies/gates:** All prior contracts implemented and tested; exact revision/workload/environment recorded; no game binaries or protected assets; Xbox 360 remains a separate optional module under ADR-0004.
- **Deliverables:** Raw measurement reports, metric distributions, unresolved-issue ledger and a separate Xbox 360 evidence/ADR plan if the owner approves it.
- **Owner decisions:** Whether to publish performance claims, whether to open an optional Xbox 360 module package and whether any FINAL_CLEANUP work is appropriate later.
- **Tests to add later:** Long-run stability, memory growth slope, counter drift, full metric capture and replay consistency tests.
- **Not implemented by XB-0084:** No FPS result is measured, no performance claim is made, no Xbox 360 module is started, and FINAL_CLEANUP is not in scope. `AGENTS.md:135-141`.

## Explicit non-goals for this plan

- `documented` — No phase in this document authorizes product code, build changes, test changes, interface changes, JIT, native execution, D3D12.X implementation, game compatibility, Xbox 360 support or publication.
- `documented` — No GTA VI internal requirement is assumed; all game-specific technical facts remain `unknown` unless separately evidenced.
- `documented` — No PS5, PC, cloud or other-console behavior is transferred to Xbox Series X.
- `documented` — No compatibility or performance result may be marked pass without recorded commands, outputs and evidence.
