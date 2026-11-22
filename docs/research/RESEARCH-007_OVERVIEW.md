# RESEARCH-007 — Xbox Series X|S Emulator Architecture & Workflow Analysis (overview)

**PACKAGE_TYPE:** RESEARCH
**PRODUCT_BOUNDARY:** development-only
**Access date for every source in this package:** 2026-11-22
**Method:** public GitHub web pages, GitHub REST API metadata and pinned raw file reads only. No repository was cloned, no binary, game file, dump, firmware, key or protected asset was downloaded, inspected or used. No code from either project was copied.

Evidence labels used throughout this package: `documented` (explicit in an authoritative or project-authored public source), `observed` (directly seen in a fetched file/API response or in the local XBridge checkout), `hypothesis` (interpretation, unverified), `unknown` (not established). A project-authored statement is labelled `documented` with the qualifier "project-authored"; it is not treated as independently verified platform fact.

---

## 1. Scope and examined revisions

The primary source is `omonkeyz/xbox-emulator`; the upstream source is `drycheetah84/TomBox`. All findings are tied to the exact commits below.

| Repository | Branch / role | Exact commit examined | How determined | Label |
|---|---|---|---|---|
| `omonkeyz/xbox-emulator` | `main`, primary; pinned examined commit (not current HEAD) | `65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2` | GitHub API `GET /repos/omonkeyz/xbox-emulator/commits/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2` returned this `sha`; commit message "Merge #143, #145 from lex021: keep acp_state.rs as text (NUL escape), pinned Opus sources for the workspace check", dated 2026-11-22T15:43:20Z | `observed` |
| `drycheetah84/TomBox` | `master`, upstream; pinned examined commit (not current HEAD) | `9a39de888bbdecef1cae84171464dad34176b994` | GitHub API `GET /repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994` returned this `sha`; commit message "Translator: group barriers for one-wave compute shaders that rely on lockstep LDS", dated 2026-11-22T02:40:52Z | `observed` |
| `drycheetah84/TomBox` | upstream base named by the primary project | `7e8064fdcd47bca4afef3ec295668ed40b81c047` | Primary README states base commit `7e8064f`; API `GET /repos/drycheetah84/TomBox/commits/7e8064f` resolved the full hash (2026-11-20T23:29:46Z) | `observed` |

Reproduction path for the hashes: the `commits/HEAD` calls in the source table record how the hashes were determined on 2026-11-22; for a stable replay, fetch `https://api.github.com/repos/omonkeyz/xbox-emulator/commits/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2`, `https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994` and `https://api.github.com/repos/drycheetah84/TomBox/commits/7e8064fdcd47bca4afef3ec295668ed40b81c047` and read the top-level `sha` field. GitHub HTML repository pages report 167 commits on `main` and 136 commits on `master`; those counts are not commit identities.

---

## 2. Source catalogue (exact URLs, all accessed 2026-11-22)

| ID | Exact URL | What was fetched / result |
|---|---|---|
| S01 | https://api.github.com/repos/omonkeyz/xbox-emulator/commits/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2 | JSON; HEAD sha `65ca6dd…` |
| S02 | https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994 | JSON; HEAD sha `9a39de88…` |
| S03 | https://github.com/omonkeyz/xbox-emulator | Rendered repository page and README |
| S04 | https://github.com/drycheetah84/TomBox | Rendered repository page and README |
| S05 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/LICENSE | Full GPL-3.0 text |
| S06 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | Contribution rules, status vocabulary, PR gates |
| S07 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/LICENSE | **FAILED: HTTP 404** — no LICENSE at examined commit |
| S08 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/ARCHITECTURE.md | Provisional architecture proposal |
| S09 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py | Progress generator (first ~797/916 lines read; output truncated by tool) |
| S10 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/THIRD_PARTY/CREDITS | Third-party origin/credit lines |
| S11 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | Function discovery and evidence-gating log |
| S12 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | Unverified assumptions and risks |
| S13 | https://api.github.com/repos/drycheetah84/TomBox/license | **FAILED: HTTP 404** — GitHub reports no detected license |
| S14 | https://api.github.com/repos/omonkeyz/xbox-emulator/license | JSON; `spdx_id: GPL-3.0` detected |
| S15 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | Project-authored public-source research notes |
| S16 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/.github/workflows/rust.yml | CI workflow |
| S17 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/crates/compat/d3d12_xs/exports.txt | Example machine-readable per-function list |
| S18 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/summary.json | Machine-readable headline metrics |
| S19 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/MILESTONES.md | Generated milestone table |
| S20 | https://api.github.com/repos/drycheetah84/TomBox/commits/7e8064f | JSON; resolved base commit full hash |
| L01 | local `CMakeLists.txt` | Verified build targets and flags |
| L02 | local `include/xbridge/cpu/x64_instruction_decoder.hpp`, `x64_instruction_interpreter.hpp`, `x64_cpu_state.hpp` | Verified CPU interfaces |
| L03 | local `include/xbridge/memory/memory.hpp`, `docs/ARCHITECTURE.md` | Verified memory contract |
| L04 | local `include/xbridge/host/capabilities.hpp`, `execution_policy.hpp`, `docs/DECISIONS.md` | Verified host/policy boundary |

For every finding below, the exact URL is given (or the source ID resolves to an exact URL in this table), the access date is 2026-11-22, and the label is stated.

---

## 3. Agent-availability limitation (required disclosure)

Only six OpenCode agents are installed in this checkout; the nine requested specialist roles (`cpu-developer`, `memory-developer`, `gpu-developer`, `audio-developer`, `input-developer`, `platform-developer`, `test-engineer`, `performance-engineer`, `integration-engineer`) are **listed as `planned`, not installed/freigegeben** in `docs/AGENT_WORKFLOW.md`. Consequence for this research: recommendations may name a possible specialist owner, but no recommendation assumes that specialist exists today. Any implementation package still needs the coordinator to assign an installed owner (the installed `tester` for test work) and, where a new tool/file/dependency or interface change is involved, an explicit owner decision. This is `observed` (local `docs/AGENT_WORKFLOW.md`, access date 2026-11-22).

This research subagent did not start subagents, ask the owner, edit product code/tests/CMake/presets/AGENTS.md/agent files, or perform any Git publication action.

---

## 4. Verified XBridge baseline used for all comparisons

These facts were read directly from the local checkout on 2026-11-22 and are labelled `observed`:

- `CMakeLists.txt`: C++20, `xbridge_core` library, `xbridge` CLI, `xbridge_tests`, CTest with one test `xbridge_tests`; MSVC `/W4 /permissive-`; project version 0.3.0.
- `src/cpu`: `test_cpu.cpp` (synthetic ISA), `x64_instruction_decoder.cpp`, `x64_instruction_interpreter.cpp`.
- Decoder: stateless, `noexcept`, status enum `Ok/EmptyInput/UnknownOpcode/InvalidEncoding/TruncatedInput`; supports NOP (`0x90`) and MOV forms (reg/imm, reg/reg, reg<->mem) with REX prefixes; no exceptions.
- Interpreter: executes one decoded instruction on `X64CpuState` + `GuestMemory`, advances RIP by instruction length, returns `Ok`/`MemoryAccessError`, and performs no partial CPU-state or guest-memory mutation on a memory error.
- `X64CpuState`: 16 x 64-bit GPRs, RIP, RFLAGS, typed GPR slice access; no instruction semantics, no reset semantics, no SIMD/FPU, no Xbox-specific properties.
- `GuestMemory`: generic contiguous byte vector, 32-bit address, `write8/read8/write32/read32/load`, bounds check via `check_range`; not Xbox virtual memory; no aliasing, no protection, no paging.
- `HostCapabilities` describes the host PC only (threads, RAM, VRAM, `cpu_jit_available`, Vulkan/D3D12/software flags, GPU name, driver version). `execution_policy.hpp` exposes `select_execution_plan`, a deterministic selector returning `ExecutionPlan`; `CpuBackend` has `Jit` and `Interpreter`, but the interpreter is the baseline and **no JIT is implemented**; backend selection is policy, not initialization.
- `src/profiles/hardware_profile.cpp` is a minimal placeholder; `src/app/main.cpp` is a CLI demo.
- `docs/ARCHITECTURE.md` and `docs/DECISIONS.md` contain accepted rules: Series X only, interpreter baseline, no JIT, generic GuestMemory, no proprietary assets, host/guest separation, backend-neutral graphics.

Nothing in this package converts any of those facts into an unimplemented capability.

---

## 5. Summary of findings by research area

### A. Architecture

| ID | Exact claim from the source | Exact URL | Label |
|---|---|---|---|
| A1 | Primary project describes itself as "Progress notes for running Xbox Series X\|S builds on PC (TomBox + rage9)" and a continuation of `drycheetah84/TomBox`. | https://github.com/omonkeyz/xbox-emulator | `documented` |
| A2 | Runtime "reimplements the console's platform layer (Xbox-flavoured Direct3D 12, precompiled RDNA2 shaders, unified-memory textures, system libraries) so dumped, already-decrypted Xbox Series X\|S builds run as normal Windows processes. It is not tied to any single title." | https://github.com/omonkeyz/xbox-emulator | `documented` (project-authored) |
| A3 | The project states it uses legally obtained already-decrypted dumps and does no key extraction or package decryption; source workspace is in `crates/`, `tools/`, `Cargo.toml` and contains no game files, binaries or build output. | https://github.com/omonkeyz/xbox-emulator | `documented` (project-authored; not independently verified here) |
| A4 | Upstream describes itself as an Xbox Series X\|S compatibility runtime for Windows PCs that maps/shims system APIs; one title reached its main menu and is not playable; a second title's shaders were examined but not run. | https://github.com/drycheetah84/TomBox | `documented` (project-authored) |
| A5 | Upstream architecture proposal says native x86-64 game code runs in a host Windows process: loader/relinker maps and resolves imports to compat libraries; host services and a D3D12.x→host-D3D12/Vulkan graphics backend sit behind interfaces. It is explicitly a "proposal under construction". | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/ARCHITECTURE.md | `documented` (project-authored proposal, not a completed design) |
| A6 | Subsystem table marks only `tombox-pe` and `pe-inspect` implemented/tested at the time of that document; loader/relinker, compat libraries, host services, graphics backend and diagnostics were "not started". The README at examined HEAD shows these later became substantial, so the architecture document lags the code. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/ARCHITECTURE.md ; https://github.com/drycheetah84/TomBox | `observed` (contradiction between documents at same commit) |
| A7 | Repository layout: `crates/` (PE, CLI, archive, RPF, disassembler, RDNA2 shader translator, one compat crate per DLL plus `compat-common`), `tools/dev/`, `tools/progress/`, `analysis/`, `docs/`, git-ignored `input/`. | https://github.com/drycheetah84/TomBox | `documented` |
| A8 | Design rules: fail loud (no silent success stubs), evidence-gated subsystem work, host code behind interfaces, game-specific workarounds labelled, each subsystem independently testable. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/ARCHITECTURE.md | `documented` (project-authored) |
| A9 | Relation of CPU to platform compatibility: because game code is x86-64, there is no CPU emulation and no recompilation; "all the work is at the platform boundary." | https://github.com/drycheetah84/TomBox | `documented` (project-authored) |
| A10 | `hypothesis` (analyst interpretation): the primary project has the same implicit CPU assumption as upstream, but the examined primary README does not state "no CPU emulation" verbatim; that statement is from the upstream README. Any XBridge decision to prefer native execution would be an architecture change, not an adopted XBridge feature. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md (link target of S04) | `hypothesis` |

### B. CPU and memory

| ID | Exact claim from the source | Exact URL | Label |
|---|---|---|---|
| B1 | Project research notes state Series X\|S use semi-custom AMD Zen 2, x86-64, with AVX2; game code is inferred to be native x86-64 using the Windows x64 ABI and needs no instruction translation. Their own tags are `[DOC]`/`[INF]`; I did not independently re-verify Microsoft documentation in this task. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | `documented` (project-authored secondary source) |
| B2 | Project notes classify host ISA superset (Zen 3 vs Zen 2) as a hypothesis and require an instruction scan to confirm. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | `documented` (their hypothesis is recorded; the underlying claim remains `hypothesis`) |
| B3 | Upstream technical-debt ledger still listed "game code is native x64 needing no instruction translation" as unverified assumption A2 in that document, despite the README asserting it; the ledger is explicitly "updated continuously" and other entries are dated 2026-11-17..19. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `observed` (internal inconsistency/lag) |
| B4 | Project notes: Series X has 16 GB unified GDDR6 (10 GB "fast" + 6 GB); CPU and GPU share an address space and GPU-visible allocations are first-class. A PC has separate RAM/VRAM, so unified memory cannot be reproduced by allocating bytes; GPU-visible resources must be mapped through host D3D12/Vulkan. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | `documented` (project-authored secondary source) |
| B5 | Upstream executes the game as an ordinary Windows process; Xbox-only DLL imports are satisfied by compatibility DLLs placed next to the executable. PE tooling has a no-panic sweep over random/PE-shaped/truncated inputs; `cargo fuzz`/sanitizer builds were not yet configured, and the pinned sources do not explicitly call the PE parser bounds-checked. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DEVELOPMENT_LOG.md ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `documented` (project-authored) |
| B6 | `d3d12_xs.dll` implements `D3D12ConfigureVirtualMemoryX` and `D3D12ZeroMemoryX`; the latter validates 16-byte-aligned ranges, zero-scratch sizes (≥64 KiB command / 256 B fence), CPU VAs ≥1 TiB and GPU-VA fill through a copy queue. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/crates/compat/d3d12_xs/exports.txt | `observed` (table rows/notes at examined commit) |
| B7 | `xcrdapi.dll` console vtables were measured: `IDStorageFactoryX` 11 slots, `IDStorageFileX` 8, `IDStorageQueueX` 14, `IDStorageStatusArrayX` 6; console has no GDeflate; format 1 is zlib and BCPack is the only other format. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` (project-reported measurements; underlying binaries not inspected here) |
| B8 | Placed-texture/texel mirroring uses per-frame hashing of game memory under a byte budget; changes can lag, reads can race, streamed mips are skipped, and Texture3D/multi-plane/render-target/depth placed textures are not mirrored. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `observed` (project-reported limitation) |
| B9 | `XALLOC_ATTRIBUTES` bit layout is recalled from a public XDK header and matches two observed attribute values; page-size/memory-type fields are ignored. Memory aliasing/access-protection semantics beyond that are not documented in the fetched sources. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `unknown` for general Xbox aliasing/protection |
| B10 | Synchronisation/execution model: console build Fortnite had all static imports resolve at load and presented frames; Xbox `PresentX` is queue slot 30; a 2026-11-22 fix inserts group barriers in translated compute shaders of ≤64 threads that use LDS without `s_barrier`, because a PC group may run as several waves. | https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994 ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DEVELOPMENT_LOG.md | `observed` (commit message/patch and development-log patch seen in API response) |
| B11 | I did not verify any guest paging, per-region permissions, address-space aliasing or Xbox memory protection behavior from these sources; XBridge `GuestMemory` remains a generic scaffold. | local `include/xbridge/memory/memory.hpp` and https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | `unknown` |

### C. GPU / graphics

| ID | Exact claim from the source | Exact URL | Label |
|---|---|---|---|
| C1 | Xbox graphics API is D3D12.x; the specific differences page is NDA-gated publicly. Public module names observed: `d3d12_x` (Xbox One) and `d3d12_xs` (Series). A shader translator alone is insufficient. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | `documented` (project-authored; NDA gap acknowledged) |
| C2 | `d3d12_xs` wraps a real PC D3D12 device and presents the Xbox D3D12.x interface: different method-table layouts and Xbox-only device/command-list/queue methods (`CreatePlacedResourceX`, `PresentX`, frame-pacing events, derived pipeline states), reconstructed from call sites. | https://github.com/drycheetah84/TomBox | `documented` (project-authored) |
| C3 | Machine-readable metric at examined commit: `d3d12x_methods` 194/200 (97.00%); `rdna2_instructions` 768/783 (98.08%); `shaders_translated` 105,506/105,506; `shader_stages` 0/8; `shader_compile_full_sweep` has no final count. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/summary.json | `observed` |
| C4 | Upstream metric legend at the primary pinned commit: "green = done, amber = partial (a stub, a forwarder, or not verified to work), grey = not done" and "mesh and amplification still grey". The upstream pinned README states exactly: "Translating is not the same as rendering correctly." and "Hull, domain, mesh and amplification have no translator support." | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/README.md ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md | `documented` (project-authored) |
| C5 | Console builds ship no DXIL; shaders are precompiled AMD RDNA2 machine code. The translator disassembles it and emits HLSL, compiled at run time with Microsoft DXC and cached on disk. | https://github.com/drycheetah84/TomBox | `documented` (project-authored) |
| C6 | Shader-form hypothesis in upstream research notes (DXIL vs microcode) was later resolved in practice for the tested build: "plain compressed Xbox shader containers", 26,503 shaders, 4,000-sample all disassemble, about 1/3 of vertex shaders translate; blockers mesh shaders and shaders without root signatures. | https://github.com/drycheetah84/TomBox ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | `observed` (project-reported result superseding the earlier `hypothesis`) |
| C7 | Texture/memory formats: typed-buffer and texture integer-vs-float types are inferred from use; 7 of 4,553 paired shaders have integer textures declared as float; structured-buffer element size is a heuristic fitted to one title; MUBUF store opcodes 0x1e/0x1f swapped by analogy, unverified; indexed constant-buffer arrays assume 16-byte stride. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `observed` (project-reported unverified assumptions) |
| C8 | Graphics pipeline/sync: for one-wave compute shaders with LDS and no `s_barrier`, barriers are emitted at the first top-level point after each LDS access; a synthetic test asserts barriers between LDS write/add/read and that groups >64 threads are untouched. The fix was not yet verified in the running title. | https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994 ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DEVELOPMENT_LOG.md | `observed` (patch + log); runtime verification `unknown` |
| C9 | PresentX plane/parameter structures (0x90 / 0x20 bytes) were logged to find colour-space/HDR fields; the title was found to present SDR. Layout is not documented and is filled at run time. | https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994 | `observed` (log statement + patch); general layout `unknown` |
| C10 | Rendered-pixel validation is absent: "Translated output has not yet been validated against rendered pixels"; 3D rendering is not verified correct. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md ; https://github.com/drycheetah84/TomBox | `observed` |

### D. Xbox system libraries

| ID | Exact claim from the source | Exact URL | Label |
|---|---|---|---|
| D1 | One compat crate per console DLL; `exports.txt` has one line per export: `ordinal Name status | notes`; statuses documented as `implemented`, `forwarded`, `pc`, `title`, `missing`. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `documented` |
| D2 | Example `d3d12_xs/exports.txt` rows use statuses `implemented`, `forwarded` and `data`, with detailed notes citing console addresses/tests (e.g. `D3D12XboxSetProcessDebugFlags` masks `0x83f00001` on OS 22000+ and `0x81f00001` on 19041; `DXGIXPresentArray` body returns `0x887a0004`). The generator recognises ten statuses: implemented, forwarded, hooked, pc, title, pending, stub, unsupported, unverified, missing. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/crates/compat/d3d12_xs/exports.txt ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py | `observed` (vocabulary mismatch with CONTRIBUTING.md) |
| D3 | Library-map scope: only exports the console provides and a PC does not; console DLLs that are modified PC DLLs count only for console-only exports; plain Windows DLLs are excluded. Statuses include `pc`, `hooked`, `title`, `stub`, `unsupported`, `unverified`. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py | `observed` |
| D4 | Machine-readable headline at examined commit: console exports 2,142/2,155 (99.4%); targeted 2,142/2,154; imported system functions 671/671; per-title console-library import coverage 50/50 Fortnite, 50/52 XDefiant, 61/61 Terraria, 48/48 Trackmania, 34/35 Minecraft. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/summary.json | `observed` |
| D5 | Methodology for unknown functions: use your own console DLL from a legally obtained dump, public GDK documentation and compatible open-source references; work out return codes/edge cases/output layout; implement; add a unit test; update `exports.txt` with a note citing the source. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `documented` |
| D6 | "No fake success": do not return `S_OK` or accept-and-ignore when the console would do something; leave the row `missing` and explain why. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `documented` |
| D7 | Evidence-gated classification: a module counts as loaded/resolved only when `loaded: true` and `unresolved: []`; a system-function import flips to done only when its name is in `resolved`; `oshost.json` turns PC-twin rows from `pc` to `hooked`. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` |
| D8 | Xbox-specific API examples handled: GDK wrappers statically linked in the Fortnite EXE; XCurl, Bink2Scarlett, chad (`XDsp*`), xapu (`XApu*`), xcrdapi, xg_xs, AcpHal, xmem, d3d12_xs, dxcompiler_xs, mfplat; API-set imports attributed to console host DLLs per OS build. | https://github.com/omonkeyz/xbox-emulator ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` |
| D9 | Some functions are deliberately `pending` or `stub` even after implementation: e.g. `gameinput_ext` builds and tests pass but is dead code at run time; MFPlat stubs have console argument checks but no PC work-queue manager; `xg_x.dll` texture computers return `E_NOTIMPL`. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` |
| D10 | Xbox OS export surfaces change between builds: Fortnite 10.0.19041.3948, XDefiant/Terraria 10.0.22000.4429, Trackmania 10.0.22621.1852; new DLLs and exports are tracked per build and remain `missing` until implemented. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` |

### E. Development workflow

| ID | Exact claim from the source | Exact URL | Label |
|---|---|---|---|
| E1 | `tools/progress/gen.py` generates `docs/LIBRARY_MAP.md`, `docs/img/progress.svg` and per-metric badges from JSON data plus `exports.txt` files; it also has a `selftest()` with checks such as "twin mapping keeps the console denominator" and evidence-flip checks. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py | `observed` |
| E2 | `summary.json` is a machine-readable headline file with `headline`, `extra`, `all_including_pc`, per-title import status and console OS builds; `docs/MILESTONES.md` is generated and cites evidence paths such as `%TEMP%\tombox\run.txt:6496`. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/summary.json ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/MILESTONES.md | `observed` |
| E3 | Per-function proof: every new implementation must add a unit test calling the function and checking console behaviour; PR gate requires no binaries/game data, no file >1 MiB, no personal data, no added comment lines in `.rs`/`.py`/`.ps1`; `CONTRIBUTING.md` requires "**actionlint** for workflow changes", implemented in a separate `.github/workflows/actionlint.yml` job using `rhysd/actionlint`; `rust.yml` runs `cargo check` and unit/WARP tests but contains no actionlint step; a maintainer reviews against console behaviour. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/.github/workflows/rust.yml ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/.github/workflows/actionlint.yml | `documented` + `observed` |
| E4 | Technical sources are documented in `docs/XBOX_RESEARCH.md`, `docs/DEVELOPMENT_LOG.md`, `docs/TECHNICAL_DEBT.md`, `docs/DECISIONS.md`; the upstream README states `ARCHITECTURE.md`, `XBOX_RESEARCH.md` and `COMPATIBILITY.md` lag behind code while the log and debt file are current. | https://github.com/drycheetah84/TomBox | `documented` + `observed` |
| E5 | Missing/partial functions are handled by explicit status rows, "not targeted" sections that keep uncovered exports in the denominator to avoid inflated percentages, and written notes on why a function is stub/unsupported/unverified. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` |
| E6 | Parallel subsystem development is evident from dated integration entries crediting separate "console library", "memory", "audio", "shader", "Xbox One" agents and from explicit notes that one agent's in-progress edit broke another crate's build; unverified or unwired work is not credited. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` |
| E7 | Network isolation incident: runs reached Epic production analytics; mitigation is an in-process `net_guard` with known gaps (`ConnectEx` to literal IP, non-WinHTTP TLS, other processes). The project states its own development/testing never connects to publisher production services. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md ; https://github.com/drycheetah84/TomBox | `observed` |
| E8 | `hypothesis` (analyst interpretation): the machine-readable map + evidence JSON + unit-test gate is the most transferable part of the workflow to XBridge; the dump-derived parts are not transferable under ADR-0009. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `hypothesis` |

---

## 6. Explicitly not verified / unknown

- No full commit diff, source implementation file, binary, dump, shader archive or console DLL was fetched; claims about implementation internals are project-authored table notes or docs, not independently reproduced here. `unknown` where not stated above.
- The primary project's `gen.py` output was truncated at ~797/916 lines; the unread tail may contain additional generator behavior. `unknown`.
- Exact Xbox memory aliasing/protection, GameOS syscall behavior, D3D12.x NDA details, mesh/amplification shader formats, and rendered-pixel equivalence: `unknown`.
- Independent legal advice: `unknown`. This package records sources only.

No claim in this overview is derived solely from other consoles; Microsoft GDK/D3D12.x/PC analogies are recorded as project-authored statements, not as verified Xbox Series X behavior.
