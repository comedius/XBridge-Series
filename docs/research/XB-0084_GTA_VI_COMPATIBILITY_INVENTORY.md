# XB-0084 — GTA VI compatibility inventory

- **PACKAGE_ID:** XB-0084
- **PACKAGE_TYPE:** RESEARCH
- **PRODUCT_BOUNDARY:** development-only
- **ACCESS DATE:** 2026-11-22
- **STATUS:** assessment/planning only; no implementation approved.
- **Evidence labels:** `documented` (explicit in an authoritative source or accepted project document), `observed` (directly seen in code, test output, or a reproducible local check), `hypothesis` (plausible but unverified), `unknown` (not established).
- **Component status labels:** `proven` (scaffold behavior directly demonstrated), `partial` (scaffold exists but is incomplete), `planned` (future design only), `unknown` (no evidence). `proven` never means game compatibility.
- **Boundary:** Development-only research document. Not product code. No Xbox Series X game compatibility, GTA VI playability, performance result, or implementation authorization is claimed.

## Authority and evidence basis

- `documented` — The owner order fixes exactly the twelve component areas used below; no alternative taxonomy is substituted. The XB-0084 envelope requires twelve component areas with implementation, tests, proven function, gaps, dependencies, compatibility/performance impact, optimizations, measurement and risks. `docs/AGENT_TASKS.md:490`, `:499`.
- `documented` — Xbox Series X is the only current target; older generations are deferred/optional isolated modules. `docs/ARCHITECTURE.md:11-13`; `docs/DECISIONS.md:18-21`.
- `documented` — Public official evidence establishes GTA VI availability on Xbox Series X|S and native Series X play, but no authoritative public technical requirements for the native Series X build were established. Xbox Wire: `https://news.xbox.com/en-us/2026/11/10/grand-theft-auto-6-comes-to-xbox-cloud-gaming-november-19/amp` (access `2026-11-22`, hash not exposed); Rockstar Newswire: `https://www.rockstargames.com/newswire/article/39akk424a9oo53/grand-theft-auto-vi-now-available` (access `2026-11-22`, hash not exposed); Xbox Store: `https://www.xbox.com/en-US/games/store/grand-theft-auto-vi/9NNZSNHLR63L` (access `2026-11-22`, hash not exposed).
- `unknown` — All GTA VI game-internal requirements are unknown: instruction mix, memory footprint, GPU command use, shader format, resource behavior, library calls, storage layout, audio/input protocol, synchronization and scheduling expectations. Official-domain searches for `fps`, `frame rate`, `resolution`, `performance mode` and Series X technical requirements returned availability/pre-order/cloud pages only.
- `observed` — The local baseline below uses read-only subagent findings returned to the coordinator. Those agents read the cited `file:line` locations directly; this researcher did not re-run their commands. Architect handoff `ses_dfb8b2c84ffed9nKKaN1zVSUCM`; developer handoff `ses_dfb8af93bffeqPGOh2dL4DmGmO`; tester handoff `ses_dfb8aabfbffezkAi7jlMKnothv`.
- `documented` — External emulator and PS5-reference material is project-authored evidence, not authoritative Xbox Series X evidence. `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md:49-72`; `docs/research/ps5-reference-project-analysis.md:273-280`.

## 1. CPU instructions/threading

- **Existing implementation:** `partial` — Synthetic `TestCpu` (4 u32 registers; `LoadImm`/`AddImm`/`Store`/`Halt`; 10,000-step run limit) is the runnable CPU; `X64CpuState` stores 16 GPRs, RIP and RFLAGS only; the x64 decoder handles NOP and MOV forms only; the x64 interpreter executes one decoded instruction with explicit status codes. Subagent findings: `include/xbridge/cpu/test_cpu.hpp:9-37`; `src/cpu/test_cpu.cpp:6-72`; `include/xbridge/cpu/x64_cpu_state.hpp:11-105`; `src/cpu/x64_instruction_decoder.cpp:85-195`; `src/cpu/x64_instruction_interpreter.cpp:26-147`.
- **Existing tests:** `partial` — Tests cover x64 state/views, decoder forms/statuses, interpreter status/no-partial-mutation behavior, TestCpu hardening and lifecycle; no arithmetic/control-flow/flags/SIMD corpus, no integrated fetch-decode-execute loop, no differential reference corpus. Subagent finding: `tests/test_main.cpp:55-238`, `:240-496`, `:498-999`, `:1001-1160`, `:1162-1214`.
- **Proven functionality/status:** `proven` for scaffold NOP/MOV decode, `X64CpuState` views, interpreter error contract and synthetic demo behavior; `unknown` for GTA VI instruction mix, threading model, CPUID visibility, AVX2/Zen 2 assumptions or console ABI/thread semantics.
- **Missing functionality:** `unknown` for game requirements; locally missing full x64 decoder/interpreter corpus, exception and flag semantics, SIMD/FPU, privileged instructions, SIB/displacement handling, reset semantics and Xbox-visible thread topology. `docs/research/xbox-series-x-cpu-evidence.md:53-62`.
- **Dependencies:** `documented` — R-001 remains `unknown` (`docs/RESEARCH_REGISTER.md:7`); any JIT/native execution change requires separate evidence, design, tests and owner approval (`docs/ARCHITECTURE.md:78-95`, `:210-213`); host CPU must not define guest semantics (`AGENTS.md:21`).
- **Compatibility impact:** `hypothesis` — Critical. Without a documented decoder corpus and reference interpreter semantics, no Series X executable can be interpreted correctly. Any native execution or relinker path is class 3 and not approved. `docs/research/RESEARCH-007_RECOMMENDATIONS.md:47-50`.
- **Performance impact:** `hypothesis` — Decoder and interpreter dispatch are likely hot paths, but no XBridge game workload exists and no measured values are available. Host wall-clock around `cpu.run()` and an executed-instruction counter are future instrumentation opportunities; they must not feed guest timing. Subagent findings: `src/core/runtime.cpp:29`; `src/cpu/test_cpu.cpp:35-37`.
- **Possible optimizations:** `planned` only — after a correct interpreter corpus, consider documented fast paths/block caching; JIT remains a selection enum only and is forbidden without separate evidence/design/tests/ADR. `include/xbridge/host/execution_policy.hpp:7-18`.
- **Measurement method:** `planned` — instruction-form coverage matrix, truncated-byte sweeps, deterministic replay of instruction traces, host wall-clock around CPU execution and non-guest-visible instruction counters. No guest-visible timing is derived from host measurements.
- **Risks/open questions:** `unknown` — GTA VI ISA/thread requirements; `documented` — do not infer guest CPU semantics from host AMD64 facts, do not implement JIT/native execution, do not switch execution paths mid-operation; `AGENTS.md:19-25`; `docs/ADAPTIVE_EXECUTION.md:17-25`.

## 2. Memory/unified memory/bandwidth

- **Existing implementation:** `partial` — `GuestMemory` is a generic contiguous byte vector with a 32-bit address cap, `check_range`, little-endian unaligned `read32`/`write32`, `load` and `std::out_of_range` on invalid ranges; no paging, protection, aliasing, reservation or host mapping. Subagent findings: `include/xbridge/memory/memory.hpp:8-21`; `src/memory/memory.cpp:9-59`; `docs/ARCHITECTURE.md:30-39`.
- **Existing tests:** `partial` — memory boundary and range tests exist; allocation-failure, concurrency and aliasing/protection tests do not apply to the current generic contract. Subagent finding: `tests/test_main.cpp:1374-1424`.
- **Proven functionality/status:** `proven` for generic byte reads/writes and bounds behavior; `unknown` for GTA VI memory footprint, Xbox guest virtual-memory behavior and unified-memory/GPU-visible allocation rules.
- **Missing functionality:** `unknown` for game requirements; locally missing guest address-space model, permissions, reservation/commit, aliasing, backing stores, host mapping, GPU-visible allocation, residency and bandwidth/latency model. R-004 is `unknown`. `docs/RESEARCH_REGISTER.md:10`.
- **Dependencies:** `documented` — ADR-0008 guest/host memory separation (`docs/DECISIONS.md:38-41`); graphics/backend ADR required before unified-memory/GPU-visible work (`docs/research/RESEARCH-007_RECOMMENDATIONS.md:49-50`); generic `GuestMemory` must remain generic until an approved change.
- **Compatibility impact:** `hypothesis` — Critical. A wrong memory model silently breaks guest semantics; host RAM/VRAM must not be equated with the console unified pool. `docs/ARCHITECTURE.md:26-38`.
- **Performance impact:** `hypothesis` — Memory bandwidth and latency may dominate, but no guest/host copy sites or backend exist to measure. Host budgets must produce controlled errors or documented degradation, not semantic changes.
- **Possible optimizations:** `planned` only — future demand-based host mapping, residency and copy-site reduction after ADR-defined contracts; never reserve all host RAM for guest memory. `AGENTS.md:25`.
- **Measurement method:** `planned` — bytes moved per unit time at guest-memory/host-copy sites and access-to-completion latency distributions, with side-effect-free counters and unchanged public signatures where possible. Subagent finding: `src/memory/memory.cpp:31-58`.
- **Risks/open questions:** `unknown` — GTA VI memory behavior, Xbox guest VM rules, GPU visibility and bandwidth needs; `documented` — Xbox VM must not be added to generic `GuestMemory` without approved scope change.

## 3. GPU/D3D12.x

- **Existing implementation:** `planned`/`unknown` — Only backend enums, the `ExecutionPlan` result struct and the pure `select_execution_plan` selector exist; no graphics device, command processor, queue, resource model, backend initialization or D3D12.x surface. Subagent findings: `include/xbridge/host/execution_policy.hpp:7-18`; `src/host/execution_policy.cpp:17-93`; `CMakeLists.txt:10-37`.
- **Existing tests:** `partial` — deterministic policy-selection tests exist; no graphics tests. Subagent finding: `tests/test_main.cpp:1326-1372`.
- **Proven functionality/status:** `proven` for deterministic plan selection only; `unknown` for D3D12.x functions, command streams, queues and GTA VI GPU behavior.
- **Missing functionality:** `unknown` for game requirements; locally missing guest command decoder, state snapshots, queue/fence model, backend device, presentation path and D3D12.x API compatibility.
- **Dependencies:** `documented` — ADR-0007 backend-neutral graphics architecture (`docs/DECISIONS.md:33-36`); R-002 `unknown` (`docs/RESEARCH_REGISTER.md:8`); public authoritative evidence required before any D3D12.x claim.
- **Compatibility impact:** `hypothesis` — Critical. PC Direct3D 12 is not equivalent to Xbox-specific graphics interfaces; D3D12.X equivalence is class 5/unknown. `docs/ARCHITECTURE.md:42-52`, `:215-217`; `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md:84-87`.
- **Performance impact:** `unknown` — no backend runs, so GPU busy time, queue stalls and presentation timing are unmeasurable today.
- **Possible optimizations:** `planned` only — capability-based backend selection, explicit fallback reporting and timeline instrumentation after ADR-defined backend contracts. No external DLL/dump-derived implementation.
- **Measurement method:** `planned` — backend/vendor busy counters, GPU timeline fences and presentation-boundary timestamps; all must be opt-in and never redefine guest timing.
- **Risks/open questions:** `unknown` — D3D12.x surface, queue semantics and GTA VI command use; `documented` — no dumps, extracted DLLs, shaders, firmware or protected assets. `docs/AGENT_TASKS.md:488`.

## 4. RDNA2/shaders

- **Existing implementation:** `unknown`/`planned` — No shader compiler, disassembler, IR, pipeline or shader-state module exists. Shader-stage formats and GTA VI shader inputs are unknown. Subagent finding: `CMakeLists.txt:10-19`.
- **Existing tests:** `unknown` — no shader tests exist.
- **Proven functionality/status:** `unknown` — no proven guest shader functionality; `proven` only that the current checkout has no shader product module.
- **Missing functionality:** `unknown` for game requirements; locally missing shader input decoder, IR, translator, pipeline state, stage semantics and validated correctness criteria.
- **Dependencies:** `documented` — R-003 is `unknown` (`docs/RESEARCH_REGISTER.md:9`); ADR-0007 separates shader translation from guest command interpretation and host backend (`docs/DECISIONS.md:33-36`).
- **Compatibility impact:** `hypothesis` — Critical. Shader semantics cannot be invented, and translation counts are not rendered-pixel correctness. `docs/research/RESEARCH-007_WORKFLOW_ANALYSIS.md:67-69`.
- **Performance impact:** `unknown` — shader compilation stalls and warm/cold behavior cannot be measured without a translator and lawful corpus.
- **Possible optimizations:** `planned` only — separate decoder/IR/backend and later caching after key/invalidation design; no use of game shader assets.
- **Measurement method:** `planned` — cold and warm wall time per translation, with stage/input format recorded; correctness tests must be independent of translation coverage counts.
- **Risks/open questions:** `unknown` — GTA VI shader format, stages, ISA and resource binding; `documented` — no game shader content, dumps or protected assets.

## 5. Shader translation/caching

- **Existing implementation:** `planned`/`unknown` — No translator, cache, keying, invalidation, persistence, deduplication or eviction exists.
- **Existing tests:** `unknown` — no shader-cache tests exist.
- **Proven functionality/status:** `unknown` — no proven translation or cache behavior; `documented` — external translation counts are other projects’ metrics, not XBridge capability. `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md:59`.
- **Missing functionality:** `unknown` for game requirements; locally missing translation pipeline, stable cache keys, invalidation rules, persistence format, eviction policy and side-effect-free telemetry.
- **Dependencies:** `documented` — R-003 and ADR-0007 (`docs/RESEARCH_REGISTER.md:9`; `docs/DECISIONS.md:33-36`); owner decisions for cache key/invalidation policy per architect handoff.
- **Compatibility impact:** `hypothesis` — High. Incorrect invalidation can produce wrong rendering that appears “fast”; unsupported behavior must remain visible. `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md:63-64`.
- **Performance impact:** `unknown` — cold compile stalls may matter, but no measurements exist.
- **Possible optimizations:** `planned` only — cache after correctness contract; record hits/misses/evictions without changing guest-visible results.
- **Measurement method:** `planned` — `hits / (hits + misses)` plus raw hit, miss, invalidation and eviction counts; cold/warm compile time per translation.
- **Risks/open questions:** `unknown` — cache key inputs, invalidation triggers and GTA VI shader volume; `documented` — translation count is not correctness.

## 6. Resources/textures

- **Existing implementation:** `unknown`/`planned` — No resource model, texture manager, state tracker, barrier logic or residency system exists.
- **Existing tests:** `unknown` — no resource or texture tests exist.
- **Proven functionality/status:** `unknown` — no proven guest resource behavior.
- **Missing functionality:** `unknown` for game requirements; locally missing resource creation/destruction semantics, state transitions, barriers, lifetime tracking, texture formats, sampling rules and residency.
- **Dependencies:** `documented` — graphics and memory ADRs, R-002/R-004 (`docs/RESEARCH_REGISTER.md:8`, `:10`); guest/host memory separation (`docs/DECISIONS.md:38-41`).
- **Compatibility impact:** `hypothesis` — High. Wrong lifetime, barrier or residency behavior corrupts rendering and may be timing-dependent.
- **Performance impact:** `unknown` — streaming and residency costs are unmeasurable today.
- **Possible optimizations:** `planned` only — tiled/streamed resources and headroom-aware residency after evidence and ADR; no external texture-mirroring implementation. `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md:62`.
- **Measurement method:** `planned` — cold/warm request-to-ready time, memory growth slope, resource-state transition counts and missed residency deadlines.
- **Risks/open questions:** `unknown` — GTA VI texture formats, resource lifetimes and streaming behavior; `documented` — game textures and packed assets are out of scope.

## 7. System libraries/Xbox functions

- **Existing implementation:** `unknown`/`planned` — No loader, compatibility library or Xbox system-service layer exists; executable loading, firmware and security/boot chains are explicit non-goals. Subagent findings: `docs/ARCHITECTURE.md:54-60`; `CMakeLists.txt:10-37`.
- **Existing tests:** `unknown` — no system-library tests exist.
- **Proven functionality/status:** `unknown` — no proven Xbox function behavior.
- **Missing functionality:** `unknown` for GTA VI call surface; locally missing API inventory, import/export handling, system-service semantics and diagnostics for unsupported calls.
- **Dependencies:** `documented` — R-005 `partial`, R-006 `unknown` (`docs/RESEARCH_REGISTER.md:11-12`); public GDK documentation and ADRs required; native execution/relink/import redirection is class 3. `docs/research/RESEARCH-007_RECOMMENDATIONS.md:47-50`.
- **Compatibility impact:** `hypothesis` — Critical. Missing or silently faked calls break correctness; unsupported entries must remain visible.
- **Performance impact:** `unknown` — call frequency and startup/import cost are unknown.
- **Possible optimizations:** `planned` only — independently implemented HLE prototypes for documented calls after evidence; no native guest execution.
- **Measurement method:** `planned` — API coverage matrix, unsupported-call counts and per-call host wall time, with no guest timing derived from host calls.
- **Risks/open questions:** `unknown` — GTA VI system-service requirements; `documented` — no firmware, keys, decrypted DLLs or protected system files.

## 8. Filesystem/storage

- **Existing implementation:** `unknown`/`planned` — No storage subsystem or decompression API exists; `HostCapabilities` has no storage facts. Subagent findings: `include/xbridge/host/capabilities.hpp:9-19`; `docs/ARCHITECTURE.md:229-231`.
- **Existing tests:** `unknown` — no storage tests exist.
- **Proven functionality/status:** `unknown` — no proven storage or decompression behavior.
- **Missing functionality:** `unknown` for GTA VI storage layout/decompression; locally missing host storage detection, guest-visible file semantics, decompression contracts, async I/O and queueing.
- **Dependencies:** `documented` — R-005 `partial` (`docs/RESEARCH_REGISTER.md:11`); host hardware policy and ADR needed before guest storage semantics. `docs/ARCHITECTURE.md:15-24`.
- **Compatibility impact:** `hypothesis` — High. Streaming/decompression timing can affect observed behavior; host file semantics must not silently redefine guest semantics.
- **Performance impact:** `unknown` — I/O and decompression costs cannot be measured today.
- **Possible optimizations:** `planned` only — asynchronous I/O and decompression after public evidence and host/guest contracts.
- **Measurement method:** `planned` — bytes moved per unit time, request-to-ready time (cold/warm), queue depth and decompression wall time.
- **Risks/open questions:** `unknown` — GTA VI package layout, compression and streaming behavior; `documented` — no game files or proprietary data.

## 9. Audio

- **Existing implementation:** `unknown`/`planned` — No audio device, clock, mixer or output backend exists. Audio design is design-only until an ADR. Subagent finding: `docs/AGENT_WORKFLOW.md:178`.
- **Existing tests:** `unknown` — no audio tests exist.
- **Proven functionality/status:** `unknown` — no proven audio output or clock behavior.
- **Missing functionality:** `unknown` for GTA VI audio requirements; locally missing output backend, audio clock, buffer model, underrun handling and timestamp injection.
- **Dependencies:** `documented` — ADR required before product audio; underrun behavior is a host-policy concern in `docs/HOST_HARDWARE_POLICY.md:20-28`.
- **Compatibility impact:** `hypothesis` — High. Audio timing couples to synchronization; guest-visible audio behavior must not be invented from host capability.
- **Performance impact:** `unknown` — latency and underrun rate are unmeasurable today.
- **Possible optimizations:** `planned` only — buffer sizing after a documented clock and underrun contract; no claim of real-time software fallback.
- **Measurement method:** `planned` — source-event to audio-callback delta and underrun/missed-deadline counts with timestamp injection.
- **Risks/open questions:** `unknown` — GTA VI audio API, formats and timing; `documented` — no game audio assets or service endpoints.

## 10. Input/controller

- **Existing implementation:** `unknown`/`planned` — No input device, controller protocol or mapping layer exists. Input design is design-only until an ADR. Subagent finding: `docs/AGENT_WORKFLOW.md:179`.
- **Existing tests:** `unknown` — no input tests exist.
- **Proven functionality/status:** `unknown` — no proven input behavior.
- **Missing functionality:** `unknown` for GTA VI controller requirements; locally missing device discovery, mapping, timestamping and latency compensation.
- **Dependencies:** `documented` — ADR required before product input; no Xbox input protocol may be invented from PS5/PC references. `docs/research/ps5-reference-project-analysis.md:273-280`.
- **Compatibility impact:** `hypothesis` — Medium/high; wrong input semantics break games even if rendering works.
- **Performance impact:** `unknown` — input latency is unmeasurable today.
- **Possible optimizations:** `planned` only — timestamp injection and deterministic replay after ADR-defined contracts.
- **Measurement method:** `planned` — injection-to-consumer-visible delta with seeded deterministic input sequences.
- **Risks/open questions:** `unknown` — GTA VI input API, device behavior and tolerance; `documented` — no controller captures or proprietary protocols.

## 11. CPU/GPU/audio synchronization

- **Existing implementation:** `unknown`/`planned` — No timeline, fences, presentation clock, audio clock or cross-subsystem synchronization exists; the policy forbids switching execution paths in the middle of an operation. Subagent findings: `docs/HOST_HARDWARE_POLICY.md:20-28`; `docs/ADAPTIVE_EXECUTION.md:17-25`.
- **Existing tests:** `unknown` — no synchronization tests exist.
- **Proven functionality/status:** `proven` only for deterministic execution-plan selection; `unknown` for guest synchronization semantics.
- **Missing functionality:** `unknown` for GTA VI synchronization; locally missing frame timeline, resource barriers, presentation semantics, audio clock coupling and safe-point switching.
- **Dependencies:** `documented` — graphics/audio ADRs and ADR-0010/ADR-0011 stability-first constraints (`docs/DECISIONS.md:48-56`); owner decisions for frame pacing, GPU timeline and audio clock (architect handoff).
- **Compatibility impact:** `hypothesis` — Critical. Race conditions and hidden timing changes produce nondeterministic failures.
- **Performance impact:** `unknown` — stutter and pacing are unmeasurable without a frame boundary and timeline.
- **Possible optimizations:** `planned` only — safe-point profile changes with explicit reporting; no silent accuracy/performance trade-offs.
- **Measurement method:** `planned` — frame-pacing jitter about a declared host-refresh target, missed-deadline count, audio/input deltas and stutter rate against a pre-registered running-median threshold.
- **Risks/open questions:** `unknown` — GTA VI synchronization contract; `documented` — no mid-operation switching, no host timing fed into guest semantics.

## 12. Scheduling/resource management

- **Existing implementation:** `partial` — `ExecutionPlan` selection is deterministic and unit-tested; `ProfileKind::SeriesX` only; host metadata such as “8 cores, 16 threads, 16 GiB, 12.15 TFLOPS” is explicitly illustrative, not detected or measured. Real host detection, telemetry and adaptation are absent. Subagent findings: `src/host/execution_policy.cpp:17-93`; `tests/test_main.cpp:1326-1372`; `src/core/config.cpp:5-21`; `tests/test_main.cpp:1284-1289`; `src/profiles/hardware_profile.cpp:1-2`.
- **Existing tests:** `partial` — policy selector and profile parsing tests exist; no real-detector, telemetry replay or adaptive-scheduling tests.
- **Proven functionality/status:** `proven` for pure selection and rejection of `series_s`; `unknown` for GTA VI scheduling needs and real host capability measurement.
- **Missing functionality:** `unknown` for game requirements; locally missing host detection, structured telemetry, resource budgets, runtime adaptation and deterministic replay of scheduling decisions.
- **Dependencies:** `documented` — ADR-0005/ADR-0010/ADR-0011 (`docs/DECISIONS.md:23-31`, `:48-56`); host detection requirements (`docs/ARCHITECTURE.md:15-24`, `:229-231`).
- **Compatibility impact:** `hypothesis` — High. Host resource exhaustion or profile tuning must not silently change guest-visible results.
- **Performance impact:** `unknown` — no measured adaptation gains exist; profile metadata is illustrative.
- **Possible optimizations:** `planned` only — capability-based safe defaults and opt-in profiles after telemetry; no reserving all host RAM.
- **Measurement method:** `planned` — per-thread and process CPU time/wall time, selected-plan reports, seeded deterministic replay and resource counters.
- **Risks/open questions:** `unknown` — GTA VI scheduling behavior; `documented` — no silent profile accuracy reductions and no host facts redefining guest semantics. `docs/ADAPTIVE_EXECUTION.md:22-24`.

## Deliberate omissions

- `documented` — No FPS, resolution, memory-size or timing values are asserted for GTA VI or XBridge.
- `documented` — No implementation phase is approved by this document. `docs/AGENT_TASKS.md:477`, `:494`.
- `documented` — No PS5, PC-game, other-console or other-title fact is transferred to Xbox Series X behavior.
