# Research register

This file is **development infrastructure** and is not part of the product package. It is the single authoritative, machine-checkable register of research questions, evidence and capability status for XBridge. Use one detail block per question (Section 8) and keep the index (Section 7) derived from those blocks.

The former instruction "Record source, date checked, confidence, and reproduction steps" is superseded: there is **no separate free-text `confidence` field**. A claim is described by **Evidence label + Source class + Uncertainties** instead. `documented` claims additionally require the source-class qualifier `authoritative` or `project-authored`.

## 1. Authority and boundaries

1. `AGENTS.md` takes precedence over this register; accepted ADRs and `docs/ARCHITECTURE.md` take precedence over research conjectures.
2. This register records evidence; it does not authorise implementation, an ADR, code adoption, a compatibility claim, a performance claim or publication.
3. Only public, lawful sources are eligible. No proprietary firmware, keys, game content, DRM material or protected assets are recorded here.
4. Xbox Series X is the only target. Xbox Series S is out of scope; Xbox 360 remains deferred and is not a current target.
5. Evidence about general x86-64/AMD64 behaviour must not be presented as Xbox Series X behaviour.
6. Project-authored external statements are always `documented (project-authored)` and are never authoritative Xbox facts.

## 2. Two independent axes

### 2.1 Evidence label — what a statement is

| Token | Exactly one use rule |
|---|---|
| documented | Use when a statement is explicitly present in a cited source and the entry supplies the required source-class qualifier. |
| observed | Use when something was directly seen in code, test output or a reproducible local check. |
| hypothesis | Use for a plausible proposition that has not been verified. |
| unknown | Use when available evidence is insufficient to establish the statement. |

### 2.2 Capability status — what is known about a capability

| Token | Exactly one use rule |
|---|---|
| not investigated | Use when no source was examined and no capability claim is made. |
| investigated-unsupported | Use when examined sources do not support the capability claim. |
| implemented-unvalidated | Use when package-local implementation exists but executed validation evidence does not. |
| validated | Use only when executed, reproducible package-local evidence confirms the capability claim. |
| unknown | Use when evidence is insufficient to determine support or the claim is deliberately left open. |

Capability status and Evidence label are independent: a row may carry `documented (project-authored)` sources and still have capability status `unknown`. `partial` and compound statuses are **workflow/lifecycle notes only** and are never capability-status tokens.

### 2.3 Source class (required qualifier for `documented`)

- `authoritative` — use only for a `documented` claim from a primary/official public source.
- `project-authored` — use for a `documented` claim authored by a project, including external project statements about another platform; never promote it to an authoritative Xbox fact.
- `N/A (no documented claim)` — use when the entry carries no `documented` label.
- An entry with both `documented` and non-`documented` labels uses `project-authored` and/or `authoritative` for the documented sub-claims plus `; N/A (non-documented sub-claims)`.

## 3. Required fields for every entry

Every detail block in Section 8 contains all of these fields, in this order, as a single non-empty line. A field with no applicable content must contain an explicit `N/A (reason)`; blank placeholders are not permitted.

1. ID
2. Title
3. Package/Goal
4. Research question
5. Scope
6. Capability status
7. Workflow/lifecycle status
8. Evidence label(s)
9. Source class
10. Source
11. Date checked
12. Concrete finding(s)
13. Finding-to-source mapping
14. Reproduction
15. Uncertainties
16. Open questions
17. Result/conclusion
18. Architecture/implementation impact
19. Dependencies
20. Responsible agent
21. Created/Changed
22. Conformance status
23. Missing fields if non-conformant

## 4. Status-dependent source and reproduction requirements

- `not investigated`: Source = `none — not investigated`; Reproduction = `N/A — no claim`; Date checked may be `not recorded (legacy row; no source examined)` with that exact reason.
- `unknown`: record the search scope or uncertainty in `Uncertainties`; Source may be `none recorded — see Uncertainties`; Reproduction = `N/A — no capability claim` when no capability claim is made.
- `investigated-unsupported`, `implemented-unvalidated`, `validated`: require a concrete Source (a URL with an access date, or an exact local path) and a concrete Reproduction citing actually executed package-local evidence. No current row is assigned any of these three statuses because no such evidence exists in this register.
- Every field must be non-empty or an explicit `N/A (reason)`.

## 5. Explicit mapping of legacy statuses

| Legacy row | Legacy value | Capability status (new) | Workflow/lifecycle status (new) | Mapping note |
|---|---|---|---|---|
| R-005 | `partial` | `unknown` | `partial — mapping started, not complete` | `partial` is retained as a workflow note; it never becomes a capability token. |
| RESEARCH-007 | `complete as evidence record; unresolved items remain unknown` (compound) | `unknown` | `complete as evidence record; unresolved items remain unknown` | The compound value is retained verbatim as a workflow note; the unresolved external items stay `unknown`. |

No legacy row is marked `validated` or `implemented-unvalidated`.

## 6. Conformance model

Allowed conformance statuses: `conformant`, `non-conformant`, `not_assessed`, `not_applicable`.

Objective criteria for `conformant`:

1. the ID is unique in this register;
2. all 23 required fields are present and non-empty or an explicit `N/A (reason)`;
3. only allowed tokens appear in Capability status, Conformance status, Evidence label(s) and Source class;
4. every `documented` label carries `authoritative` or `project-authored`;
5. Finding-to-source mapping is present, with an explicit reason where no source was recorded;
6. Uncertainties are labeled (each uncertainty carries an evidence-label token or is explicitly `unknown`);
7. Scope is declared; and
8. the status-dependent Source/Reproduction requirements of Section 4 are satisfied.

`non-conformant` requires the failing criteria to be listed in `Missing fields if non-conformant`. `not_assessed` means conformance was not yet checked. `not_applicable` is allowed **only** for template/header rows or owner-excluded entries, always with a reason, and never for an entry with findings. The **researcher records** the conformance status; the **independent reviewer verifies** it with file:line evidence. A recorded `conformant` value is not a reviewer verdict.

## 7. Compact index

| ID | Topic | Capability status | Evidence label(s) | Conformance status | Next action |
|---|---|---|---|---|---|
| R-001 | Publicly documented Xbox Series CPU-visible behavior | unknown | documented (project-authored); documented (authoritative, published hardware specs and general Windows x64 ABI only); observed; unknown | conformant | XB-0083 re-verified custom Zen 2 + general x64 ABI; guest CPUID/AVX2 remain unknown |
| R-002 | Xbox-specific graphics command interfaces | unknown | documented (project-authored); documented (authoritative, public PC/Windows D3D12 only); observed; unknown | conformant | XB-0083 re-verified public D3D12; D3D12.X remains unknown |
| R-003 | Shader input representation and semantics | unknown | documented (project-authored); documented (authoritative, public D3D12 mesh-shader and DXIL specifications only); observed; unknown | conformant | XB-0083 re-verified public mesh/DXIL specs; Xbox guest format remains unknown |
| R-004 | Guest virtual-memory behavior | unknown | documented (project-authored); documented (authoritative, published hardware memory configuration only); observed; unknown | conformant | XB-0083 re-verified published memory config; guest requirements remain unknown |
| R-005 | Storage/decompression APIs (`partial` retained as workflow note) | unknown | documented (project-authored); documented (authoritative, published Xbox hardware I/O specs and public Windows DirectStorage only); observed; unknown | conformant | XB-0083 re-verified hardware I/O + public DirectStorage; host mapping remains unknown |
| R-006 | System services required by test software | unknown | documented (project-authored); documented (authoritative, published Xbox user-facing features and general Windows x64 syscall convention only); observed; unknown | conformant | XB-0083 re-verified user-facing features + general x64 syscall note; Xbox services remain unknown |
| RESEARCH-007 | Public Xbox Series X\|S emulator architecture/workflow concepts from `omonkeyz/xbox-emulator` and `drycheetah84/TomBox` | unknown | documented (project-authored); observed; unknown | conformant | Coordinator records handoff; owner/legal review before any adoption |
| XB-0084 | GTA VI Series X performance-first assessment and deferred Xbox 360 / Splinter Cell: Double Agent backward-compatibility planning | unknown | documented (project-authored); documented (authoritative, availability/native-platform messaging only); observed; unknown | conformant | Owner approval required before implementation, ADR or any compatibility/performance claim |
| R-XB-NEXT-02-GI | PE/COFF metadata and Xbox Series X game-file indicators for a read-only local analyzer | unknown | documented (authoritative, Microsoft PE/COFF spec, public GDK/Xbox/DirectX sources only); documented (project-authored); observed; hypothesis; unknown | conformant | Coordinator routes independent reviewer verification; no implementation, ADR or compatibility claim |
| XB-NEXT-03-EXEC | Execution models for native, hybrid and current-interpreter paths | unknown | documented (authoritative, general Windows x64/PE/COFF/Win32 only); documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no architecture choice or implementation |
| XB-NEXT-03-NATIVE | Native execution feasibility and worker-process isolation limits | unknown | documented (authoritative, general Windows x64/Win32/PE/COFF only); documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no worker, JIT, patching or execution claim |
| XB-NEXT-03-RELOC | PE32+/AMD64 headers, alignment and in-scope base relocations | unknown | documented (authoritative, general PE/COFF only); documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; unsupported relocation types remain explicit failures |
| XB-NEXT-03-LIBS | Runtime library family evidence for xmem, xg_xs, d3d12_xs, xgameruntime and dxcompiler_xs | unknown | documented (authoritative, public Windows D3D12 only); documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; owner/legal review before any adoption |
| XB-NEXT-03-GFX | Graphics compatibility analysis and D3D12-to-D3D12 research boundary | unknown | documented (authoritative, public Windows D3D12/mesh/DXIL specifications only); documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no D3D12.X equivalence or backend approval |
| XB-NEXT-03-PERF | Performance measurement concept without game or native-execution claims | unknown | documented (authoritative, general Windows x64/public Windows D3D12 only); documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no benchmark, harness or performance claim |
| XB-NEXT-03-SYNTH | XB-NEXT-DEVELOPMENT-03-R0 synthesis and final-feasibility boundary | unknown | documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; arbitrary commercial binary feasibility remains unknown |
| XB-NEXT-04-A | PE loader/module requirements from the supplied game-installation report | unknown | documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no loader implementation or compatibility claim |
| XB-NEXT-04-B | XMem and guest-memory requirements from the supplied report | unknown | documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no Xbox memory semantics adopted |
| XB-NEXT-04-C | x64 ABI, TLS, SEH, CRT and runtime requirements from the supplied report | unknown | documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no guest ABI or execution claim |
| XB-NEXT-04-D | D3D12.X, XG, FFX and BVH graphics requirements from the supplied report | unknown | documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no D3D12.X equivalence or backend approval |
| XB-NEXT-04-E | Bink, chad, xapu and Media Foundation audio/video requirements | unknown | documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no codec or media behavior claim |
| XB-NEXT-04-F | Config identity, HTTP, Winsock, input, window, crypto and registry requirements | unknown | documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no system-service implementation claim |
| XB-NEXT-04-G | RPF/TTPF and DStorage container/streaming requirements | unknown | documented (project-authored); observed; hypothesis; unknown | conformant | Independent reviewer verification; no container internals or decryption claim |

## 8. Entry details

### R-001

- ID: R-001
- Title: Publicly documented Xbox Series CPU-visible behavior
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored Zen 2 / CPU-visible claim; no implementation, ADR, interface or product change authorised.
- Research question: Which Xbox Series X CPU-visible behaviors are explicitly documented in authoritative public sources, and which project-authored claims remain unverified?
- Scope: Xbox Series X only; public official Microsoft/AMD pages and public Microsoft specification repositories; access date 2026-11-24; no proprietary firmware, keys, game content, DRM material or other-console evidence; general x86-64/AMD64 facts are labelled as general and are not presented as Xbox-only behavior.
- Capability status: unknown
- Workflow/lifecycle status: re-verified against authoritative public sources by XB-0083-AUTHORITATIVE-EVIDENCE; unverified CPU-visible items remain unknown
- Evidence label(s): documented (project-authored); documented (authoritative, published hardware specs and general Windows x64 ABI only); observed; unknown
- Source class: project-authored; authoritative (published hardware specs and general Windows x64 ABI only); N/A (non-documented sub-claims)
- Source: P1 RESEARCH-007 detail block in this register (project-authored external claim lineage); P2 `docs/AGENT_TASKS.md:254-265` and `docs/AGENT_TASKS.md:1493-1502` (project-authored claim list); A1 `https://www.xbox.com/en-US/consoles/xbox-series-x` (access 2026-11-24); A2 `https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention` (access 2026-11-24); A3 `https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions` (access 2026-11-24); A9 `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md` (full inventory and reproduction).
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external project statements assert Zen 2 / AVX2 / Windows x64 ABI behavior; they remain project-authored and are not authoritative Xbox facts. F2 (documented, authoritative for published hardware specs only): A1 publishes "8X Cores @ 3.8 GHz (3.6 GHz w/SMT) Custom Zen 2 CPU" for Xbox Series X. F3 (documented, authoritative for general Windows x64 ABI only): A2/A3 document the Windows x64 ABI — first four integer arguments in RCX/RDX/R8/R9, floating-point arguments in XMM0-XMM3, shadow space, 16-byte stack alignment, volatile/nonvolatile registers and PE32+ image format; this is general x86-64/Windows behavior, not Xbox-specific. F4 (unknown): A1 does not document guest-visible CPUID leaves, AVX2 availability on the custom part, a syscall interface or a guest ABI choice; no authoritative public source examined establishes these as Xbox Series X facts. F5 (observed): the exact URLs above returned the quoted content when fetched on 2026-11-24; the reproduction path is recorded in A9.
- Finding-to-source mapping: F1 → P1, P2; F2 → A1; F3 → A2, A3; F4 → none recorded — see Uncertainties, conclusion `unknown`; F5 → A9.
- Reproduction: fetch A1, A2 and A3 and search the quoted phrases; the full URL/date/excerpt inventory and unusable-URL notes are in A9. No build, CTest, game run or guest execution was performed; no capability claim is made.
- Uncertainties: unknown — whether the Xbox Series X guest exposes AVX2 or any specific CPUID leaf layout; unknown — whether the Windows x64 ABI is the guest ABI; observed — `websearch` returned HTTP 402 and two AMD press-release URLs were unusable (timeout; unrelated slug), so no AMD primary CPU document was confirmed; observed — the authoritative scope of F2/F3 is limited exactly as stated.
- Open questions: Can authoritative public evidence establish guest-visible CPUID/AVX2 or the guest calling convention without proprietary material?
- Result/conclusion: the custom Zen 2 hardware fact and the general Windows x64 ABI are documented from authoritative public sources; project-authored CPU-visible claims are not promoted; all remaining CPU-visible capability claims stay `unknown`. No compatibility or performance claim.
- Architecture/implementation impact: none — no ADR, interface, decoder, CPU, memory or test change; general x64 facts are not mapped to guest semantics.
- Dependencies: A9 for the full URL inventory and unusable-URL notes; P1/P2 for the original project-authored claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created not recorded (legacy row); Changed 2026-11-24 by researcher (XB-0083-AUTHORITATIVE-EVIDENCE): authoritative re-verification added, legacy provenance retained.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### R-002

- ID: R-002
- Title: Xbox-specific graphics command interfaces
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored D3D12.X / graphics command interface claim; no implementation, ADR, interface or product change authorised.
- Research question: Which Xbox-specific graphics command interfaces are documented in authoritative public sources, and which project-authored claims remain unverified?
- Scope: Xbox Series X only; public official Microsoft pages and specifications; access date 2026-11-24; no D3D12.X equivalence assumed; no GDK/NDA, proprietary or other-console material; public D3D12 is reported as general DirectX and not as an Xbox console interface.
- Capability status: unknown
- Workflow/lifecycle status: re-verified against authoritative public sources by XB-0083-AUTHORITATIVE-EVIDENCE; Xbox-specific interface items remain unknown
- Evidence label(s): documented (project-authored); documented (authoritative, public PC/Windows D3D12 only); observed; unknown
- Source class: project-authored; authoritative (public PC/Windows D3D12 only); N/A (non-documented sub-claims)
- Source: P1 RESEARCH-007 detail block in this register (project-authored external D3D12.X claim lineage); P2 `docs/AGENT_TASKS.md:254-265` and `docs/AGENT_TASKS.md:1493-1502`; A4 `https://learn.microsoft.com/en-us/windows/win32/direct3d12/direct3d-12-graphics` (access 2026-11-24); A5 `https://raw.githubusercontent.com/microsoft/DirectX-Specs/master/d3d/MeshShader.md` (access 2026-11-24, public D3D12 command/PSO surface only); A9 `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md`.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external project statements assert D3D12.X-equivalent graphics interfaces; these remain project-authored and are not authoritative Xbox facts. F2 (documented, authoritative for public PC/Windows D3D12 only): A4 states the Direct3D 12 programming guide covers the D3D12 programmable pipeline and that D3D12 headers/libraries ship in the Windows 10 SDK; A5 documents public D3D12 API calls such as CheckFeatureSupport, CreatePipelineState, DispatchMesh and ExecuteIndirect. F3 (unknown): no authoritative public source examined documents an Xbox-specific D3D12.X command interface, its command format, or equivalence to PC D3D12; GDK material was not used. F4 (observed): the exact URLs above returned the quoted content when fetched on 2026-11-24; reproduction is recorded in A9.
- Finding-to-source mapping: F1 → P1, P2; F2 → A4, A5; F3 → none recorded — see Uncertainties, conclusion `unknown`; F4 → A9.
- Reproduction: fetch A4 and A5 and search the quoted phrases; the full URL/date/excerpt inventory is in A9. No build, CTest, GDK access, game run or guest execution was performed; no capability claim is made.
- Uncertainties: unknown — whether any Xbox-specific graphics command interface is publicly documented at all; unknown — D3D12.X command semantics or PC-D3D12 equivalence; observed — a `direct3d12/directstorage` URL returned 404 and was replaced by the valid DirectStorage reference (see R-005); observed — public D3D12 evidence is not Xbox console evidence.
- Open questions: Can any authoritative public source establish Xbox-specific graphics command behavior without GDK/NDA material?
- Result/conclusion: public PC/Windows D3D12 is documented from authoritative sources; the project-authored D3D12.X claim is not promoted; all Xbox-specific graphics capability claims stay `unknown`. No D3D12.X equivalence, backend or compatibility claim.
- Architecture/implementation impact: none — no ADR, graphics backend, interface or test change; no equivalence inferred from public D3D12.
- Dependencies: A9 for the full URL inventory; P1/P2 for the project-authored claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created not recorded (legacy row); Changed 2026-11-24 by researcher (XB-0083-AUTHORITATIVE-EVIDENCE): authoritative re-verification added, legacy provenance retained.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### R-003

- ID: R-003
- Title: Shader input representation and semantics
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored shader/mesh input representation claim; no implementation, ADR, interface or product change authorised.
- Research question: Which shader input representations and semantics are documented in authoritative public sources, and which project-authored shader/mesh claims remain unverified?
- Scope: Xbox Series X only; lawful public Microsoft specification repositories; access date 2026-11-24; no game assets, dumps, extracted shaders, proprietary formats or other-console evidence; public D3D12/DXIL format facts are reported as general DirectX, not as Xbox console shader binaries.
- Capability status: unknown
- Workflow/lifecycle status: re-verified against authoritative public sources by XB-0083-AUTHORITATIVE-EVIDENCE; Xbox-specific shader format and translation items remain unknown
- Evidence label(s): documented (project-authored); documented (authoritative, public D3D12 mesh-shader and DXIL specifications only); observed; unknown
- Source class: project-authored; authoritative (public D3D12 mesh-shader and DXIL specifications only); N/A (non-documented sub-claims)
- Source: P1 RESEARCH-007 detail block in this register (project-authored external shader claim lineage); P2 `docs/AGENT_TASKS.md:254-265` and `docs/AGENT_TASKS.md:1493-1502`; A5 `https://raw.githubusercontent.com/microsoft/DirectX-Specs/master/d3d/MeshShader.md` (access 2026-11-24); A6 `https://raw.githubusercontent.com/microsoft/DirectXShaderCompiler/main/docs/DXIL.rst` (access 2026-11-24); A9 `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md`.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external project statements assert shader/mesh format and translation behavior; they remain project-authored and are not authoritative Xbox facts. F2 (documented, authoritative for public D3D12 mesh-shader specification only): A5 states that mesh/amplification shaders are D3D12-only, IA is disabled, each mesh-shader thread receives thread IDs plus an optional group-uniform mesh payload, `[numthreads]` must satisfy X*Y*Z <= 128, PSO creation requires DXIL bytecode and forbids VS/GS/HS/DS, and signature linkage is by semantic name and index rather than packed location. F3 (documented, authoritative for public DXIL specification only): A6 defines DXIL as a subset of LLVM 3.7 IR, lists shader models including mesh (ms) and amplification (as) at 6.5, and enumerates MSIn/MSOut/MSPOut/ASIn signature points; A6 mentions an "offline XBOX shader compiler" as one possible IR consumer but specifies no Xbox shader binary format. F4 (unknown): no authoritative public source examined specifies an Xbox Series X guest shader binary representation, a lawful corpus result or translation correctness; no corpus was built by this package. F5 (observed): the exact URLs above returned the quoted content when fetched on 2026-11-24; reproduction is recorded in A9.
- Finding-to-source mapping: F1 → P1, P2; F2 → A5; F3 → A6; F4 → none recorded — see Uncertainties, conclusion `unknown`; F5 → A9.
- Reproduction: fetch A5 and A6 and search the quoted phrases; the full URL/date/excerpt inventory is in A9. No build, CTest, shader compilation, corpus creation, game run or guest execution was performed; no capability claim is made.
- Uncertainties: unknown — Xbox Series X guest shader binary format; unknown — whether any lawful corpus can be assembled and what translation correctness would require; observed — A6 names an offline XBOX shader compiler but documents no Xbox-specific binary layout, so that mention is not an Xbox format discovery; observed — public D3D12/DXIL evidence is not Xbox console shader evidence.
- Open questions: Can authoritative public evidence establish any Xbox Series X guest shader format without protected assets or GDK material?
- Result/conclusion: public D3D12 mesh-shader and DXIL representations are documented from authoritative sources; the project-authored shader/mesh claim is not promoted; all Xbox-specific shader format and translation capability claims stay `unknown`. No translator, backend or compatibility claim.
- Architecture/implementation impact: none — no ADR, shader translator, backend, interface or test change; public D3D12 semantics are not mapped to guest behavior.
- Dependencies: A9 for the full URL inventory; P1/P2 for the project-authored claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created not recorded (legacy row); Changed 2026-11-24 by researcher (XB-0083-AUTHORITATIVE-EVIDENCE): authoritative re-verification added, legacy provenance retained.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### R-004

- ID: R-004
- Title: Guest virtual-memory behavior
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored guest virtual-memory claim; no implementation, ADR, interface or product change authorised.
- Research question: Which observable guest virtual-memory requirements are documented in authoritative public sources, and which project-authored claims remain unverified?
- Scope: Xbox Series X only; public official Microsoft hardware specifications; access date 2026-11-24; `GuestMemory` stays generic and is not an Xbox virtual-memory model; no proprietary firmware, keys, game content or other-console evidence.
- Capability status: unknown
- Workflow/lifecycle status: re-verified against authoritative public sources by XB-0083-AUTHORITATIVE-EVIDENCE; guest virtual-memory items remain unknown
- Evidence label(s): documented (project-authored); documented (authoritative, published hardware memory configuration only); observed; unknown
- Source class: project-authored; authoritative (published hardware memory configuration only); N/A (non-documented sub-claims)
- Source: P1 RESEARCH-007 detail block in this register (project-authored external memory claim lineage); P2 `docs/AGENT_TASKS.md:254-265` and `docs/AGENT_TASKS.md:1493-1502`; A1 `https://www.xbox.com/en-US/consoles/xbox-series-x` (access 2026-11-24); A9 `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md`.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external project statements assert guest virtual-memory / unified-memory behavior; they remain project-authored and are not authoritative Xbox facts. F2 (documented, authoritative for published hardware configuration only): A1 publishes "16GB GDDR6 w/320 bit-wide bus" and "Memory Bandwidth. 10GB @ 560 GB/s, 6GB @ 336 GB/s" for Xbox Series X. F3 (unknown): A1 does not document page tables, guest address-space layout, GPU-visible allocation semantics or unified-memory behavior; no authoritative public source examined derives an observable guest virtual-memory requirement. F4 (observed): the exact URL above returned the quoted content when fetched on 2026-11-24; reproduction is recorded in A9.
- Finding-to-source mapping: F1 → P1, P2; F2 → A1; F3 → none recorded — see Uncertainties, conclusion `unknown`; F4 → A9.
- Reproduction: fetch A1 and search the quoted phrases; the full URL/date/excerpt inventory is in A9. No build, CTest, memory test, game run or guest execution was performed; no capability claim is made.
- Uncertainties: unknown — page-table, address-space and GPU-visible allocation semantics; unknown — whether any authoritative public source exposes observable guest requirements; observed — the published split is a hardware specification, not a guest virtual-memory model; observed — public hardware facts are not mapped into `GuestMemory`.
- Open questions: What observable requirements, if any, can be specified without adding Xbox-specific semantics to `GuestMemory`?
- Result/conclusion: the published hardware memory configuration is documented from an authoritative source; the project-authored guest-memory claim is not promoted; all guest virtual-memory capability claims stay `unknown`. No unified-memory or compatibility claim.
- Architecture/implementation impact: none — no ADR, interface or product change; generic `GuestMemory` remains unchanged and no Xbox virtual-memory model is introduced.
- Dependencies: A9 for the full URL inventory; P1/P2 for the project-authored claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created not recorded (legacy row); Changed 2026-11-24 by researcher (XB-0083-AUTHORITATIVE-EVIDENCE): authoritative re-verification added, legacy provenance retained.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### R-005

- ID: R-005
- Title: Storage/decompression APIs
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored storage/decompression claim; no implementation, ADR, interface or product change authorised.
- Research question: Which storage/decompression facts and APIs are documented in authoritative public sources, and which project-authored mappings remain unverified?
- Scope: Xbox Series X only; public official Microsoft hardware specifications and public Windows DirectStorage documentation; access date 2026-11-24; no proprietary storage APIs, keys, game data or other-console evidence; public Windows DirectStorage is reported as PC/Windows, not as an Xbox console API.
- Capability status: unknown
- Workflow/lifecycle status: partial — mapping started, not complete; authoritative re-verification added by XB-0083-AUTHORITATIVE-EVIDENCE (`partial` remains a workflow note only, never a capability token)
- Evidence label(s): documented (project-authored); documented (authoritative, published Xbox hardware I/O specs and public Windows DirectStorage only); observed; unknown
- Source class: project-authored; authoritative (published Xbox hardware I/O specs and public Windows DirectStorage only); N/A (non-documented sub-claims)
- Source: P1 RESEARCH-007 detail block in this register (project-authored external storage claim lineage); P2 `docs/AGENT_TASKS.md:254-265` and `docs/AGENT_TASKS.md:1493-1502`; A1 `https://www.xbox.com/en-US/consoles/xbox-series-x` (access 2026-11-24); A7 `https://learn.microsoft.com/en-us/windows/win32/dstorage/dstorage-api-reference` (access 2026-11-24); A8 `https://devblogs.microsoft.com/directx/directstorage-api-downloads/` (access 2026-11-24); A9 `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md`.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external project statements assert storage/decompression API behavior and host equivalents; they remain project-authored and are not authoritative Xbox facts. F2 (documented, authoritative for published Xbox hardware I/O specs only): A1 publishes a custom NVMe SSD, "I/O Throughput. 2.4 GB/s (Raw), 4.8 GB/s (Compressed, with custom hardware decompression block)", Xbox Velocity Architecture and Quick Resume. F3 (documented, authoritative for public Windows DirectStorage only): A7 states the reference covers APIs declared in `dstorage.h`; A8 lists Windows SDK releases and GDeflate/Zstd decompression support in the DirectStorage SDK; neither documents an Xbox console API. F4 (unknown): no authoritative public source examined documents an Xbox-specific storage/decompression API surface or a validated host equivalent; the mapping remains incomplete. F5 (observed): the exact URLs above returned the quoted content when fetched on 2026-11-24; reproduction is recorded in A9.
- Finding-to-source mapping: F1 → P1, P2; F2 → A1; F3 → A7, A8; F4 → none recorded — see Uncertainties, conclusion `unknown`; F5 → A9.
- Reproduction: fetch A1, A7 and A8 and search the quoted phrases; the full URL/date/excerpt inventory and unusable-URL notes are in A9. No build, CTest, decompression benchmark, game run or guest execution was performed; no capability claim is made.
- Uncertainties: unknown — Xbox-specific storage/decompression API surface and lawful host-equivalent mapping; unknown — whether public Windows DirectStorage corresponds to the console decompression block; observed — `https://learn.microsoft.com/en-us/windows/win32/direct3d12/directstorage` returned 404 and the valid A7/A8 URLs were used instead; observed — public Windows DirectStorage is not Xbox console API evidence.
- Open questions: Which documented Xbox-specific APIs, if any, have lawful host equivalents, and which parts of the mapping remain unmapped?
- Result/conclusion: published Xbox hardware I/O specs and public Windows DirectStorage are documented from authoritative sources; the project-authored mapping is not promoted; the host-equivalent mapping stays incomplete and `unknown`. No API capability, throughput-equivalence or compatibility claim.
- Architecture/implementation impact: none — no ADR, storage/decompression implementation, interface or test change; no host API adopted as an Xbox equivalent.
- Dependencies: A9 for the full URL inventory; P1/P2 for the project-authored claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created not recorded (legacy row); Changed 2026-11-24 by researcher (XB-0083-AUTHORITATIVE-EVIDENCE): authoritative re-verification added; legacy `partial` retained as a workflow note and capability status kept `unknown`.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### R-006

- ID: R-006
- Title: System services required by test software
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored system-services claim; no implementation, ADR, interface or product change authorised.
- Research question: Which system services are documented in authoritative public sources, and which project-authored system-service claims remain unverified?
- Scope: Xbox Series X only; public official Microsoft pages; access date 2026-11-24; no GDK/NDA, proprietary, private or other-console material; no native-platform dependency assumed for XBridge.
- Capability status: unknown
- Workflow/lifecycle status: re-verified against authoritative public sources by XB-0083-AUTHORITATIVE-EVIDENCE; Xbox-specific system-service items remain unknown
- Evidence label(s): documented (project-authored); documented (authoritative, published Xbox user-facing features and general Windows x64 syscall convention only); observed; unknown
- Source class: project-authored; authoritative (published Xbox user-facing features and general Windows x64 syscall convention only); N/A (non-documented sub-claims)
- Source: P1 RESEARCH-007 detail block in this register (project-authored external system claim lineage); P2 `docs/AGENT_TASKS.md:254-265` and `docs/AGENT_TASKS.md:1493-1502`; A1 `https://www.xbox.com/en-US/consoles/xbox-series-x` (access 2026-11-24); A3 `https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions` (access 2026-11-24, general x64 syscall register note only); A9 `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md`.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external project statements assert required system services / Xbox functions; they remain project-authored and are not authoritative Xbox facts. F2 (documented, authoritative for published user-facing features only): A1 publicly describes Quick Resume, faster load times, 3D Spatial Sound, storage expansion cards and backward compatibility as Xbox Series X features. F3 (documented, authoritative for general Windows x64 only): A3 states that R10:R11 are used in syscall/sysret instructions and that the direction flag is expected clear on Windows system calls; this is general x64/Windows behavior, not an Xbox service list. F4 (unknown): no authoritative public source examined documents an Xbox-specific system-service API surface, a minimum service set for test software or an XBridge mapping; GDK/NDA material was not used. F5 (observed): the exact URLs above returned the quoted content when fetched on 2026-11-24; reproduction is recorded in A9.
- Finding-to-source mapping: F1 → P1, P2; F2 → A1; F3 → A3; F4 → none recorded — see Uncertainties, conclusion `unknown`; F5 → A9.
- Reproduction: fetch A1 and A3 and search the quoted phrases; the full URL/date/excerpt inventory is in A9. No build, CTest, GDK access, game run or guest execution was performed; no capability claim is made.
- Uncertainties: unknown — which Xbox-specific services, if any, are required and publicly documented; unknown — any minimum service set for lawful test software; observed — published user-facing features are not an API surface; observed — general Windows x64 evidence is not Xbox service evidence.
- Open questions: Can any authoritative public source define a minimum Xbox-specific system-service set without GDK/NDA material?
- Result/conclusion: published user-facing features and a general Windows x64 syscall convention are documented from authoritative sources; the project-authored system-service claim is not promoted; all Xbox-specific system-service capability claims stay `unknown`. No service, host-API or compatibility claim.
- Architecture/implementation impact: none — no ADR, host service, interface or test change; no native-platform dependency introduced.
- Dependencies: A9 for the full URL inventory; P1/P2 for the project-authored claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created not recorded (legacy row); Changed 2026-11-24 by researcher (XB-0083-AUTHORITATIVE-EVIDENCE): authoritative re-verification added, legacy provenance retained.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### RESEARCH-007

- ID: RESEARCH-007
- Title: Public Xbox Series X|S emulator architecture/workflow concepts from `omonkeyz/xbox-emulator` and `drycheetah84/TomBox`
- Package/Goal: RESEARCH-007 — development-only research into public architecture/workflow concepts of two external projects; no code adoption, ADR or product change authorised.
- Research question: Which publicly visible architecture and workflow ideas from these projects are usable as evidence discipline for XBridge, and what legal/version uncertainty blocks adoption?
- Scope: public GitHub API commit responses, repository/README pages and pinned raw files only; all three URLs below are pinned commit SHAs examined on 2026-11-22; no proprietary firmware, keys, game content or protected assets; Xbox Series X target only.
- Capability status: unknown
- Workflow/lifecycle status: complete as evidence record; unresolved items remain unknown
- Evidence label(s): documented (project-authored); observed; unknown
- Source class: project-authored; N/A (non-documented sub-claims)
- Source: S1 `https://api.github.com/repos/omonkeyz/xbox-emulator/commits/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2` (access date 2026-11-22); S2 `https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994` (access date 2026-11-22); S3 `https://api.github.com/repos/drycheetah84/TomBox/commits/7e8064fdcd47bca4afef3ec295668ed40b81c047` (access date 2026-11-22); S4 exact local path `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md` (full inventory and reproduction pointer); S5 the five `docs/research/RESEARCH-007_*.md` files.
- Date checked: 2026-11-22
- Concrete finding(s): F1 (documented, project-authored): external project sources describe workflow and architecture ideas; these are project-authored external statements, not authoritative Xbox facts. F2 (observed): the exact commit SHAs examined on 2026-11-22 were primary `65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2`, upstream HEAD at that time `9a39de888bbdecef1cae84171464dad34176b994`, upstream base `7e8064fdcd47bca4afef3ec295668ed40b81c047`. F3 (observed, locally recorded): installed-role status is recorded in `docs/AGENT_WORKFLOW.md`; the nine requested specialist roles were not installed. F4 (unknown): upstream license/legal status, D3D12.X specifics, rendered-pixel correctness and the legal effect of the primary project's current GPL-3.0-or-later status versus earlier MIT/Apache-2.0 releases.
- Finding-to-source mapping: F1 → S1, S2, S3, S4, S5; F2 → S1, S2, S3; F3 → local `docs/AGENT_WORKFLOW.md` (installed-role status) and `docs/AGENT_TASKS.md`; F4 → S4, S5 (gaps and license notes), conclusion `unknown`.
- Reproduction: fetch the pinned full-SHA URLs `https://api.github.com/repos/omonkeyz/xbox-emulator/commits/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2`, `https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994` and `https://api.github.com/repos/drycheetah84/TomBox/commits/7e8064fdcd47bca4afef3ec295668ed40b81c047`, then resolve the exact URLs/source IDs in `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md` and search the quoted phrases in the five `docs/research/RESEARCH-007_*.md` files. Reproduction caution: current branch HEADs differ from these pinned commits, so `commits/HEAD` is not a stable reproduction path.
- Uncertainties: unknown — upstream license/legal status; D3D12.X specifics; rendered-pixel correctness; installed specialist roles; the legal effect of GPL-3.0-or-later versus earlier MIT/Apache-2.0 releases for adoption. External web truth was not re-fetched by XB-0080-A.
- Open questions: What does owner/legal review conclude about the exact revisions and their licenses? Which D3D12.X or pixel-correctness claims, if any, are supported by authoritative public evidence?
- Result/conclusion: complete as an evidence record only; owner/legal review is required before any adoption because the upstream license is `unknown` and the legal effect of GPL-3.0-or-later versus earlier MIT/Apache-2.0 releases remains `unknown`. No code adoption, ADR or compatibility claim.
- Architecture/implementation impact: none — no product code, tests, CMake, presets, ADR or interface change; no external status vocabulary copied into capability status.
- Dependencies: owner/legal review before adoption; `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md` for the full inventory; `docs/AGENT_TASKS.md` for coordinator handoff.
- Responsible agent: researcher (RESEARCH-007); coordinator records the handoff; independent reviewer verified RESEARCH-007 in its own package, not in XB-0080-A.
- Created/Changed: Created 2026-11-22 (RESEARCH-007); Changed 2026-11-24 by XB-0080-A (schema migration only; date, pinned commit SHAs, URLs, reproduction pointer and full inventory pointer preserved verbatim). REC-01…REC-20 remain unchanged in `docs/research/RESEARCH-007_RECOMMENDATIONS.md` with their existing ID/URL/label/class and are not edited by this package.
- Conformance status: conformant
- Missing fields if non-conformant: none — all required fields present and non-empty or explicit N/A with reason.

### XB-0084

- ID: XB-0084
- Title: GTA VI Series X performance-first assessment and deferred Xbox 360 / Splinter Cell: Double Agent backward-compatibility planning
- Package/Goal: XB-0084 — development-only assessment/planning record; implementation, ADR, interface, dependency, code adoption and compatibility/performance claims were not approved.
- Research question: What can be established about GTA VI on Xbox Series X and deferred Xbox 360 backward compatibility from public evidence, without fabricating technical requirements or measured values?
- Scope: six owner-scoped documents listed below; twelve owner-fixed component areas; public official messaging limited to availability/native-platform statements; no FPS/resolution/memory value asserted as measured; no Series S target and no current Xbox 360 target.
- Capability status: unknown
- Workflow/lifecycle status: complete assessment/planning evidence record; no implementation approved
- Evidence label(s): documented (project-authored); documented (authoritative, availability/native-platform messaging only); observed; unknown
- Source class: project-authored; authoritative (availability/native-platform messaging only); N/A (non-documented sub-claims)
- Source: S1 `docs/research/XB-0084_GTA_VI_COMPATIBILITY_INVENTORY.md`; S2 `docs/research/XB-0084_GTA_VI_PERFORMANCE_PROFILE.md`; S3 `docs/research/XB-0084_PRIORITIZED_DEVELOPMENT_PLAN.md`; S4 `docs/research/XB-0084_RESEARCH_LICENSE_ASSESSMENT.md`; S5 `docs/research/XB-0084_RISK_DEPENDENCY_OVERVIEW.md`; S6 `docs/research/XB-0084_XBOX360_SPLINTER_CELL_ANALYSIS.md`; S7 `docs/AGENT_TASKS.md:499` (twelve owner-fixed component areas); S8 `docs/research/XB-0084_RISK_DEPENDENCY_OVERVIEW.md:89-98` (official-source evidence register).
- Date checked: 2026-11-22
- Concrete finding(s): F1 (documented, project-authored): the six documents provide inventory, performance-methodology, phased plan, license assessment, risk/dependency and Xbox 360 planning records. F2 (documented, authoritative for availability/native-platform messaging only; unknown for technical details): official sources support availability/native-platform messaging only, as recorded in `docs/research/XB-0084_RISK_DEPENDENCY_OVERVIEW.md:89-98`. F3 (observed): twelve owner-fixed component areas are recorded at `docs/AGENT_TASKS.md:499` — CPU instructions/threading; memory/unified memory/bandwidth; GPU / D3D12.x; RDNA2/shaders; shader translation/caching; resources/textures; system libraries/Xbox functions; filesystem/storage; audio; input/controller; CPU/GPU/audio synchronization; scheduling/resource management. F4 (unknown): GTA VI Series X technical requirements and performance values; Xbox 360 emulator internals and title behavior.
- Finding-to-source mapping: F1 → S1, S2, S3, S4, S5, S6; F2 → S8; F3 → S7; F4 → S5 (risk notes), conclusion `unknown`; no FPS/resolution/memory measurement is mapped to any source.
- Reproduction: read the six exact local paths S1–S6, then verify the twelve component areas at `docs/AGENT_TASKS.md:499` and the official-source scope at `docs/research/XB-0084_RISK_DEPENDENCY_OVERVIEW.md:89-98`. These are package-local documentary records; no build, CTest, game run, FPS/resolution/memory measurement or emulation execution was performed by XB-0080-A.
- Uncertainties: unknown — GTA VI Series X internal requirements, technical details and performance values; Xbox 360 internals; external truth of web-sourced claims; RESEARCH-007 license compatibility; D3D12.X equivalence, shader-stage formats and rendered-pixel correctness. No FPS/resolution/memory value is asserted as measured; "30 FPS" is an owner-desired target, not a measurement.
- Open questions: Which technical requirements, if any, can be established from authoritative public evidence? What does owner approval require before any implementation or claim?
- Result/conclusion: complete assessment/planning evidence record only; owner approval is required before implementation, an ADR, code adoption, a Series S target, a current Xbox 360 target, or any compatibility/performance claim. No compatibility or playability is claimed for either title.
- Architecture/implementation impact: none — ADR-0004 and the Xbox Series X-only target are unchanged; no ADR, interface, dependency, product code or test change.
- Dependencies: owner order and twelve component areas at `docs/AGENT_TASKS.md:499`; `docs/research/XB-0084_RISK_DEPENDENCY_OVERVIEW.md:89-98` for source scope; no implementation dependency started.
- Responsible agent: researcher + coordinator + reviewer (XB-0084 assessment only); their historical verdict is recorded in `docs/AGENT_TASKS.md` and is not a review of XB-0080-A.
- Created/Changed: Created 2026-11-22 (XB-0084); Changed 2026-11-24 by XB-0080-A (schema migration only; date, six paths, twelve areas, source-scope and approval limits preserved).
- Conformance status: conformant
- Missing fields if non-conformant: none — all required fields present and non-empty or explicit N/A with reason.

### R-XB-NEXT-02-GI

- ID: R-XB-NEXT-02-GI
- Title: PE/COFF metadata and Xbox Series X game-file indicators for a read-only local analyzer
- Package/Goal: XB-NEXT-DEVELOPMENT-02-B1 — development-only public-source research into reliably readable PE/COFF metadata and lawful Xbox Series X game-file indicators; no analyzer implementation, ADR, public interface, dependency or product change authorised.
- Research question: Which PE/COFF structure fields and package/resource indicators can be read with strict bounds from a local file, which fields are only architecture/platform indicators, and which Xbox Series X specifics remain unknown without proprietary material?
- Scope: Xbox Series X target only; public Microsoft PE/COFF specification, Microsoft Learn GDK packaging/configuration pages, Xbox user-facing product page, and public DirectX/DDS/DXIL specifications; access date 2026-11-25; no game file, key, firmware, protected asset, DRM circumvention, game-content upload, other-console evidence or local corpus; general x86-64/Windows PE facts are labelled general and never presented as Xbox behaviour; no build, CTest, game run or guest execution.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (authoritative, Microsoft PE/COFF spec, public GDK/Xbox/DirectX sources only); documented (project-authored, package envelope and prior register entries); observed; hypothesis; unknown
- Source class: authoritative; project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:3231-3246` (package envelope, local read 2026-11-25); S1 `https://learn.microsoft.com/en-us/windows/win32/debug/pe-format` (access 2026-11-25, sections `MS-DOS Stub`, `Signature`, `COFF File Header`, `Optional Header`, `Optional Header Data Directories`, `Section Table`, `.edata`, `.idata`, `.reloc`, `.debug`, `.rsrc`); S2 `https://learn.microsoft.com/en-us/archive/msdn-magazine/2002/february/inside-windows-win32-portable-executable-file-format-in-detail` (access 2026-11-25, `The IMAGE_NT_HEADERS Header`); S3 `https://learn.microsoft.com/en-us/gaming/gdk/docs/features/common/packaging/overviews/packaging` (access 2026-11-25, `Packaging for XBOX and PC`, `Encryption`); S4 `https://learn.microsoft.com/en-us/gaming/game-publishing/tutorial-xbox-managed/how-to-create-a-package` (access 2026-11-25, `Create a game package`); S5 `https://learn.microsoft.com/en-us/gaming/gdk/docs/features/common/game-config/microsoftgameconfig-overview` (access 2026-11-25); S6 `https://learn.microsoft.com/en-us/gaming/gdk/docs/reference/system/microsoftgameconfig/elements/microsoftgameconfig-element-executable` (access 2026-11-25); S7 `https://learn.microsoft.com/en-us/gaming/gdk/docs/features/common/packaging/overviews/packaging-getting-started-for-console` (access 2026-11-25); S8 `https://learn.microsoft.com/en-us/gaming/gdk/docs/features/common/packaging/deployment/makepkg` (access 2026-11-25); S9 `https://www.xbox.com/en-US/consoles/xbox-series-x` (access 2026-11-25, user-facing hardware/features only); S10 `https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dx-graphics-dds` (access 2026-11-25, PC/DirectX only); S11 `https://raw.githubusercontent.com/microsoft/DirectXShaderCompiler/main/docs/DXIL.rst` (access 2026-11-25, unversioned `main`); S12 `docs/RESEARCH_REGISTER.md:122-276` and `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md` (project-authored lineage); S13 exact local path `docs/research/XB-NEXT-02_GAME_INTELLIGENCE_FORMATS.md` (full finding/source inventory and reproduction); U1 `https://learn.microsoft.com/en-us/gdk/overview` (404), `https://learn.microsoft.com/en-us/gdk/` (404), `https://developer.xbox.com/` (connection refused), `websearch` (HTTP 402) on 2026-11-25; U2 `https://support.xbox.com/en-US/help/games-apps/my-games-apps/install-games-disc-or-digital` (`webfetch` returned no tool output and raw HTML had no usable article text) on 2026-11-25.
- Date checked: 2026-11-25
- Concrete finding(s): F1 (`documented (authoritative)` S1, S2): DOS `e_magic` `0x5A4D`/`MZ`, `e_lfanew`/offset `0x3c`, `PE\0\0` signature and the full COFF/optional-header/section-table field layout are readable metadata; the specification is explicitly not guaranteed complete and requires `SizeOfOptionalHeader` plus `NumberOfRvaAndSizes` bounds checks. F2 (`documented (authoritative)` S1): `Machine 0x8664` = x64, optional Magic `0x20b` = PE32+ and `Subsystem 14` = XBOX are field facts; the subsystem value is an indicator only and PE is `not architecture specific`. F3 (`documented (authoritative)` S1): import descriptors (20 bytes, null-terminated), 32/64-bit thunks with ordinal flag, hint/name entries, export directory (40 bytes), EAT/forwarders, ordinal table, base relocation blocks (Page RVA/Block Size, 4-bit type and 12-bit offset, `DIR64 = 10`), 28-byte debug directory entries, and the Type/Name/Language resource tree have documented layouts. F4 (`hypothesis` S1): bounded loops, overflow-safe section extents, string/terminator caps, block-size and directory-size checks are engineering requirements inferred from those documented structures; no analyzer was implemented or executed. F5 (`documented (authoritative)` S3, S4, S7, S8): console packages are XVC `.xvc` built with MakePkg; PC packages are MSIXVC `.msixvc`; XVC content is documented as encrypted on disc, CDN, console install and external games drive, with `MicrosoftGame.config`, referenced icons/images/strings and Intelligent Delivery tags always unencrypted. F6 (`documented (authoritative)` S5, S6): GDK `TargetDeviceFamily` allows `XboxOne`, `Scarlett`, `PC` and `Architecture` allows `x64`, `ARM64`; Series X dev-kit launch behaviour is documented, but retail file-format use is not established. F7 (`documented (authoritative, PC/DirectX only)` S10, S11): `.dds` is a documented DirectDraw Surface texture format and DXIL a documented DirectX shader IR mentioning an `offline XBOX shader compiler` consumer, neither being a confirmed Series X game format. F8 (`documented (project-authored)` S0, S12): PE32+ alone does not imply Windows compatibility or direct executability, and no prior register row establishes Xbox guest file formats. F9 (`observed` S13, U1, U2): exact URLs returned the quoted content; unsuccessful endpoints and `websearch` HTTP 402 are recorded without inferred content. F10 (`unknown`, none): XVC internal layout, keys, chunk format, retail PE visibility, Series X runtime DLL/API set and Xbox-specific shader/resource signatures; no game file was obtained or inspected.
- Finding-to-source mapping: F1 → S1, S2, S13; F2 → S1, S13; F3 → S1, S13; F4 → S1 (engineering inference), S0; F5 → S3, S4, S7, S8; F6 → S5, S6; F7 → S10, S11; F8 → S0, S12; F9 → S13, U1, U2; F10 → none recorded — see Uncertainties, conclusion `unknown`.
- Reproduction: read S0 and S12 locally; use `webfetch`/`fetch` on S1–S11 on 2026-11-25 and search the verbatim anchors listed in S13 Section 1 (`0x5A4D`, `PE\0\0`, `IMAGE_FILE_MACHINE_AMD64 0x8664`, `IMAGE_SUBSYSTEM_XBOX 14`, `NumberOfRvaAndSizes`, each named PE section, `XBOX consoles XVC .xvc`, `content is encrypted`, `TargetDeviceFamily allowed values`, `.dds`, `DXIL is intended`); record U1/U2 failures as failures only. Full excerpts, per-finding labels and section names are in S13. No build, CTest, game run, file corpus, upload or capability validation was executed.
- Uncertainties: `unknown` — internal XVC layout, chunk format, encryption construction and retail PE-header visibility (S3 explains why content can be unavailable); `unknown` — Series X runtime DLL/API sets, guest ABI and subsystem usage; `unknown` — Xbox-specific shader/resource extensions or magic bytes; `observed` — S11 is unversioned `main`, so its exact bytes may change; `observed` — U1/U2 yielded no usable source and S3/S5/S6/S7/S8 display an authorization notice although the cited public text was returned unauthenticated; `unknown` — external truth after 2026-11-25.
- Open questions: Can authoritative public evidence establish any retail Series X container or shader format without protected assets? Which strict bounds and failure semantics should a future read-only analyzer adopt, and who approves that component?
- Result/conclusion: PE/COFF field layouts, x64/PE32+ and subsystem indicators, directory enumeration structures, XVC/MSIXVC packaging workflow and PC/DirectX resource/shader formats are documented with explicit scope; all Xbox Series X file-format internals and runtime dependencies remain `unknown`. No compatibility, playability, performance or implementation claim; capability status stays `unknown`.
- Architecture/implementation impact: none — no ADR, interface, decoder, CPU, memory, product code, test, CMake, config, dependency or licence change; possible analyzer components in S13 Section 7 are `hypothesis` candidates and are not approved work.
- Dependencies: S0 package envelope; S12 prior project-authored claim lineage; S13 for the full URL/excerpt inventory and reproduction path. No implementation dependency started.
- Responsible agent: researcher (XB-NEXT-DEVELOPMENT-02-B1); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-25 by researcher (XB-NEXT-DEVELOPMENT-02-B1); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason; reviewer verification is a separate, unperformed step.

### XB-NEXT-03-EXEC

- ID: XB-NEXT-03-EXEC
- Title: Execution models for native, hybrid and current-interpreter paths
- Package/Goal: XB-NEXT-DEVELOPMENT-03-R0 — development-only public-source research comparing native execution, the owner-approved hybrid direction and the current interpreter without choosing or implementing an architecture.
- Research question: Which published general x64/Windows facts can be compared across the three execution models, and which Xbox Series X guest semantics remain unknown?
- Scope: Xbox Series X target only; access date 2026-11-25; public Microsoft general x64/Windows/PE/COFF/Win32 sources plus project-authored local sources; no proprietary material, other-console evidence, game execution, guest execution, JIT, binary rewriting, code patching, build or CTest; general x86-64 facts are never promoted to Xbox facts.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (authoritative, general Windows x64/PE/COFF/Win32 only); documented (project-authored); observed; hypothesis; unknown
- Source class: authoritative; project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:3643-3673` (local read 2026-11-25); S1 `https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170`; S2 `https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions?view=msvc-170`; S3 `https://learn.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=msvc-170`; S4 `https://learn.microsoft.com/en-us/cpp/intrinsics/x86-intrinsics-list?view=msvc-170`; S5 `https://learn.microsoft.com/en-us/cpp/build/exception-handling-x64?view=msvc-170`; S6 `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/Debug/pe-format.md`; S7 raw `ProcThread/thread-local-storage.md` and `ProcThread/using-thread-local-storage.md`; S8 raw `Memory/memory-protection.md` and `Memory/memory-protection-constants.md`; S9 raw `Sync/synchronization-objects.md`; S10 raw `Debug/structured-exception-handling.md`; S11 `docs/ARCHITECTURE.md:141-183`; S12 `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md:22-32`; S13 exact local path `docs/research/XB-NEXT-03_EXECUTION_MODELS.md` (full source inventory and reproduction). URLs accessed 2026-11-25.
- Date checked: 2026-11-25
- Concrete finding(s): F1 (`documented (project-authored)` S0): the owner-approved direction is hybrid native execution plus an XBridge compatibility/runtime layer, with the interpreter retained as reference, diagnostic and fallback; no arbitrary-game proof, JIT, binary rewriting, code patching or real-game execution is authorized. F2 (`documented (authoritative, general only)` S1–S6): Windows x64 ABI, CPUID/XGETBV/AVX2/BMI/FMA intrinsics, table-based exceptions and PE/TLS structures are public general facts. F3 (`documented (project-authored)` S11): the current interpreter supports NOP and a narrow MOV subset, with no SIMD/FPU, privileged instructions, JIT or Xbox-specific CPU properties. F4 (`hypothesis` S7–S11): TLS translation, SEH bridging, ABI matching, thread mapping and memory mapping are candidate compatibility concerns, not implemented behavior. F5 (`observed` S1–S10, S13): the exact URLs returned the quoted content when fetched on 2026-11-25. F6 (`unknown`): guest CPUID leaves, AVX2/BMI/FMA requirements, ABI, TLS, exceptions/interrupts, syscall surface, memory map, threading and privileged instructions are not established.
- Finding-to-source mapping: F1 → S0, S13; F2 → S1–S6; F3 → S11; F4 → S7–S11; F5 → S1–S10 and S13 source-inventory observations; F6 → none recorded — see Uncertainties, conclusion `unknown`.
- Reproduction: locally read S0, S11, S12 and S13; directly `fetch` S1–S10 on 2026-11-25 and search the quoted anchors in S13 Section 1. No build, CTest, game run, guest execution or capability validation was performed.
- Uncertainties: `unknown` — all Xbox guest-specific counterparts of the cited general facts; `unknown` — whether any commercial Series X binary can execute under the direction; `observed` — the `websearch` integration returned HTTP 402 earlier, so no search-result snippet is used; `unknown` — external truth after 2026-11-25.
- Open questions: Which instruction subsets, if any, should a future lawful synthetic proof exercise? Which ABI/TLS/exception/memory assumptions should a compatibility layer expose? How could privileged instructions or interrupts be handled without JIT, binary rewriting or code patching?
- Result/conclusion: The hybrid direction and interpreter fallback are `documented (project-authored)`. General x64/Windows mechanisms are `documented (authoritative)` only in their general scope. Native/hybrid feasibility for arbitrary commercial binaries remains `unknown`; no performance or compatibility claim is made.
- Architecture/implementation impact: none — no ADR, interface, decoder, CPU, memory, product code, test, CMake, config, dependency or licence change; candidate mappings in S13 are `hypothesis` and are not approved work.
- Dependencies: S0 package envelope; S11 current interpreter documentation; S12 prior authoritative-evidence lineage; S13 full execution-model comparison.
- Responsible agent: researcher (XB-NEXT-DEVELOPMENT-03-R0); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-25 by researcher (XB-NEXT-DEVELOPMENT-03-R0); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason; reviewer verification is a separate, unperformed step.

### XB-NEXT-03-NATIVE

- ID: XB-NEXT-03-NATIVE
- Title: Native execution feasibility and worker-process isolation limits
- Package/Goal: XB-NEXT-DEVELOPMENT-03-R0 — development-only public-source feasibility record for the owner-approved hybrid direction; no execution path, JIT, patching, worker process or game run authorized.
- Research question: Which host feature-discovery, ABI, TLS, exception, memory and isolation mechanisms are publicly documented, and which Xbox guest counterparts prevent a feasibility conclusion?
- Scope: Xbox Series X target only; access date 2026-11-25; public Microsoft general x64/Windows/PE/COFF/Win32 sources and project-authored local sources; no proprietary firmware, SDK, key, game asset, DRM material, other-console evidence, build, CTest, guest execution or implementation.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (authoritative, general Windows x64/Win32/PE/COFF only); documented (project-authored); observed; hypothesis; unknown
- Source class: authoritative; project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:3643-3673` (local read 2026-11-25); S1 `https://learn.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=msvc-170`; S2 `https://learn.microsoft.com/en-us/cpp/intrinsics/x86-intrinsics-list?view=msvc-170`; S3 `https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170`; S4 `https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions?view=msvc-170`; S5 `https://learn.microsoft.com/en-us/cpp/build/exception-handling-x64?view=msvc-170`; S6 raw `Debug/pe-format.md`; S7 raw `ProcThread/thread-local-storage.md` and `ProcThread/using-thread-local-storage.md`; S8 raw `Memory/memory-protection.md` and `Memory/memory-protection-constants.md`; S9 raw `ProcThread/job-objects.md`; S10 raw `SecAuthZ/restricted-tokens.md`; S11 raw `SecAuthZ/appcontainer-isolation.md`; S12 `docs/ARCHITECTURE.md:141-183`; S13 `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md:22-32`; S14 exact local path `docs/research/XB-NEXT-03_NATIVE_EXECUTION_FEASIBILITY.md`. URLs accessed 2026-11-25.
- Date checked: 2026-11-25
- Concrete finding(s): F1 (`documented (authoritative, general only)` S1, S2): CPUID leaf/subleaf inputs, XGETBV and AVX2/BMI/FMA intrinsics are documented host-side capabilities; guest requirements are not established. F2 (`documented (authoritative, general Windows x64 only)` S3–S6): fast-call ABI, volatile/nonvolatile registers, syscall/sysret register note, table-based exceptions, TLS and PE structures are public facts. F3 (`documented (authoritative, general Win32 only)` S8–S11): page protection, Job Objects, restricted tokens and AppContainer provide general host mechanisms and explicit limits. F4 (`documented (project-authored)` S0, S12, S13): the hybrid direction is owner-approved, the interpreter is fallback, and prior guest CPUID/ABI questions remain unresolved. F5 (`hypothesis` S1–S11): feature matching, TLS translation, SEH bridging, memory mapping and a separate worker with combined isolation are candidate mechanisms; none is implemented or proven safe. F6 (`unknown`): guest CPUID/AVX2/BMI/FMA requirements, ABI, TLS order, exception/interrupt behavior, syscalls, memory permissions, threading and privileged instructions remain open. F7 (`observed` S1–S11, S14): the exact URLs returned quoted content on 2026-11-25. F8 (`hypothesis` derived from `documented (authoritative)` S9, S10, S11 and `documented (project-authored)` S0): a Job Object is not a complete security boundary (S9); a restricted token is not a complete security boundary (S10); AppContainer is not a complete security boundary (S11); their combination is not a complete security boundary (S9–S11 with S0 boundary). The exact phrase is an engineering conclusion from the documented limitations, not a direct Microsoft quotation.
- Finding-to-source mapping: F1 → S1, S2; F2 → S3–S6; F3 → S8–S11; F4 → S0, S12, S13; F5 → S1–S11 with S0 boundary; F6 → none recorded — see Uncertainties; F7 → S1–S11 and S14 source inventory; F8 → S9 (Job Object), S10 (restricted token), S11 (AppContainer), S9–S11 with S0 boundary (combination).
- Reproduction: locally read S0, S12, S13 and S14; directly `fetch` S1–S11 on 2026-11-25 and search the quoted anchors in S14 Section 1. No build, CTest, worker process, native code execution, game run or capability validation was performed.
- Uncertainties: `unknown` — all Xbox guest-specific counterparts; `hypothesis` — isolation mechanisms may contain but do not translate guest behavior; `hypothesis` — the F8 "not a complete security boundary" statements are engineering conclusions derived from documented limitations and are not direct source quotations; `observed` — Job Object breakaway/security-limit caveats and AppContainer capability caveats are documented general limits; `unknown` — external truth after 2026-11-25.
- Open questions: Which guest instruction features must be matched before native entry? How should host/guest transitions, TLS, faults and privileged instructions be handled without JIT, rewriting or patching? What isolation combination, if any, is lawful and stable for a future synthetic worker?
- Result/conclusion: General host mechanisms are `documented (authoritative)` in general scope only. The hybrid direction is `documented (project-authored)`. Xbox guest feasibility remains `unknown`; no native execution, safety, compatibility or performance result is claimed.
- Architecture/implementation impact: none — no worker, host interface, guest semantics, product code, test, build, ADR or dependency change; design options in S14 are `hypothesis`.
- Dependencies: S0 package envelope; S12 current interpreter documentation; S13 prior unresolved evidence; S14 full native-feasibility record.
- Responsible agent: researcher (XB-NEXT-DEVELOPMENT-03-R0); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-25 by researcher (XB-NEXT-DEVELOPMENT-03-R0); Changed 2026-11-25 by researcher (XB-NEXT-DEVELOPMENT-03-R0-C1): F8 security-boundary corrections only; no capability-status change.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason; reviewer verification is a separate, unperformed step.

### XB-NEXT-03-RELOC

- ID: XB-NEXT-03-RELOC
- Title: PE32+/AMD64 headers, alignment and in-scope base relocations
- Package/Goal: XB-NEXT-DEVELOPMENT-03-R0 — development-only public-source evidence for a future strict bounds checker/loader; no loader, analyzer, game file read, decryption or binary patching authorized.
- Research question: Which Microsoft-documented PE32+/AMD64 header and relocation fields bound a future strict reader, and how must unsupported relocation types be reported?
- Scope: Xbox Series X target only; access date 2026-11-25; Microsoft PE/COFF specification and project-authored prior evidence; general PE facts only; no proprietary file, key, firmware, protected asset, DRM circumvention, build, CTest or implementation.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (authoritative, general PE/COFF only); documented (project-authored); observed; hypothesis; unknown
- Source class: authoritative; project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:3649-3660` (local read 2026-11-25); S1 `https://learn.microsoft.com/en-us/windows/win32/debug/pe-format` (canonical Microsoft PE Format specification, accessed 2026-11-25); S2 `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/Debug/pe-format.md` (accessed 2026-11-25, sections `COFF File Header`, `Optional Header`, `Optional Header Data Directories`, `Section Table`, `The .reloc Section (Image Only)`, `Base Relocation Block`, `Base Relocation Types`); S3 `docs/research/XB-NEXT-02_GAME_INTELLIGENCE_FORMATS.md:29-87`; S4 `docs/RESEARCH_REGISTER.md:331-355`; S5 exact local path `docs/research/XB-NEXT-03_PECOFF_RELOCATIONS.md`.
- Date checked: 2026-11-25
- Concrete finding(s): F1 (`documented (authoritative, general PE/COFF only)` S1, S2): PE field layout includes `Machine 0x8664`, PE32+ `Magic 0x20b`, `SizeOfOptionalHeader`, `NumberOfRvaAndSizes`, section alignment/file-alignment rules and data-directory index 5 for base relocations. F2 (`documented (authoritative)` S2): relocation blocks are per 4 KiB page, begin with Page RVA and Block Size, and contain 16-bit entries with a 4-bit type and 12-bit offset. F3 (`documented (project-authored)` S0): only `ABSOLUTE (0)` and `DIR64 (10)` are in the defined handling scope for future work in this research direction; `HIGH`, `LOW`, `HIGHLOW`, `HIGHADJ`, machine-specific and reserved types must be explicitly unsupported and never silently corrected. F4 (`hypothesis` S2, S5): overflow-safe extent checks, `Block Size >= 8`, even entry-byte count, mapped 8-byte `DIR64` target validation and typed errors are engineering candidates. F5 (`observed` S1, S2, S5): sources returned the quoted content on 2026-11-25. F6 (`unknown`): retail Series X PE-header visibility, encryption consequences and real-binary relocation requirements are not established by this package.
- Finding-to-source mapping: F1 → S1, S2, S3; F2 → S1, S2; F3 → S0, S2; F4 → S2, S3, S5; F5 → S1, S2, S5 source-inventory observations; F6 → none recorded — see Uncertainties.
- Reproduction: locally read S0, S3, S4 and S5; use `webfetch`/direct `fetch` S1/S2 on 2026-11-25 and search anchors `COFF File Header`, `SizeOfOptionalHeader`, `NumberOfRvaAndSizes`, `SectionAlignment`, `FileAlignment`, `The .reloc Section (Image Only)`, `Base Relocation Block`, `Base Relocation Types`, `IMAGE_REL_BASED_ABSOLUTE`, `IMAGE_REL_BASED_DIR64`. No build, CTest, file corpus, game run or capability validation was performed.
- Uncertainties: `unknown` — Xbox-specific file visibility and real-binary relocation requirements; `unknown` — any authenticated GDK material; `observed` — S1 page metadata and S2 raw text were accessed on 2026-11-25; `unknown` — external truth after that date.
- Open questions: Can any lawful corpus establish retail PE-header visibility? Which typed errors and failure semantics should a future bounded reader use, and who approves them?
- Result/conclusion: PE32+/AMD64 fields and relocation block layout are `documented (authoritative, general PE/COFF only)`. The in-scope boundary of `ABSOLUTE (0)` and `DIR64 (10)` is `documented (project-authored)`; all other relocation types remain explicitly unsupported. No reader was implemented and capability status stays `unknown`.
- Architecture/implementation impact: none — no ADR, public interface, loader, analyzer, test, CMake, config, dependency or licence change; strict-check rules in S5 are `hypothesis`.
- Dependencies: S0 package scope; S3 prior PE evidence; S4 prior register row; S5 full relocation evidence record.
- Responsible agent: researcher (XB-NEXT-DEVELOPMENT-03-R0); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-25 by researcher (XB-NEXT-DEVELOPMENT-03-R0); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason; reviewer verification is a separate, unperformed step.

### XB-NEXT-03-LIBS

- ID: XB-NEXT-03-LIBS
- Title: Runtime library family evidence for xmem, xg_xs, d3d12_xs, xgameruntime and dxcompiler_xs
- Package/Goal: XB-NEXT-DEVELOPMENT-03-R0 — development-only cataloguing of public project-authored library leads; no external code, licence, compatibility layer, ADR or product change adopted.
- Research question: Which function/export names and compatibility roles are visible in pinned public sources, and which authoritative Xbox interface claims remain unknown?
- Scope: Xbox Series X target only; access date 2026-11-25; pinned `omonkeyz/xbox-emulator` commit `65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2`, public Windows D3D12 docs and project-authored local evidence; no proprietary SDK, game file, key, firmware, DRM material, other-console evidence, source copying, build, CTest or implementation.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation or adoption approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (authoritative, public Windows D3D12 only); documented (project-authored); observed; hypothesis; unknown
- Source class: authoritative; project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:3649-3660` (local read 2026-11-25); S1 `https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md`; S2 same pinned root plus `crates/compat/xmem/src/lib.rs`; S3 same pinned root plus `crates/compat/xmem/Cargo.toml`; S4 same pinned root plus `crates/compat/xmem/src/{alloc.rs,heap.rs,phys.rs,stats.rs,virt.rs}`; S5 same pinned root plus `crates/compat/xg_xs/exports.txt`; S6 same pinned root plus `crates/compat/xg_xs/Cargo.toml`; S7 same pinned root plus `crates/compat/d3d12_xs/exports.txt`; S8 raw `MicrosoftDocs/win32/docs/desktop-src/direct3d12/{resource-binding.md,using-resource-barriers-to-synchronize-resource-states-in-direct3d-12.md,managing-graphics-pipeline-state-in-direct3d-12.md,command-queues-and-command-lists.md}`; S9 `docs/research/RESEARCH-007_OVERVIEW.md:46-144` and `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:50`; S10 exact local paths `docs/research/XB-NEXT-02_GAME_INTELLIGENCE_FORMATS.md` and `docs/research/XB-NEXT-03_LIBRARY_FAMILIES.md`. URLs accessed 2026-11-25.
- Date checked: 2026-11-25
- Concrete finding(s): F1 (`documented (project-authored)` S1–S7): `xmem` source records heap/virtual/physical-memory interfaces, `xg_xs` records texture/layout helpers with stub entries, `d3d12_xs` records selected device/root-signature/debug/DXGIX/IID exports, and `xgameruntime` plus `dxcompiler_xs` are recorded as public discovery leads. F2 (`observed` S3, S6, S10): pinned external crates declare `GPL-3.0-or-later`; no code was copied and owner/legal review would be required before adoption. F3 (`documented (authoritative, public Windows D3D12 only)` S8): Windows D3D12 binding, barriers, PSOs and command queues are documented for PC. F4 (`hypothesis` S1–S8, S10): Windows heap/memory and D3D12 concepts are possible counterpart areas only, not Xbox equivalence. F5 (`unknown`): authoritative Xbox signatures, build stability, runtime correctness, real-title dependencies and real-game applicability are not established. F6 (`observed` S1–S8, S10): exact URLs returned quoted content on 2026-11-25; source words such as `implemented` and `stub` are quoted project vocabulary, not XBridge capability tokens.
- Finding-to-source mapping: F1 → S1–S7, S10; F2 → S3, S6, S10; F3 → S8; F4 → S1–S8, S10; F5 → none recorded — see Uncertainties; F6 → S1–S8, S10 source-inventory observations.
- Reproduction: locally read S0, S9 and S10; directly `fetch` S1–S8 on 2026-11-25 and search the exact family names, export names and quoted descriptions in S10 Section 1. No build, CTest, game run, code adoption, external build or capability validation was performed.
- Uncertainties: `unknown` — authoritative Xbox interfaces and complete signatures; `unknown` — real-title dependencies and correctness; `observed` — external project status words are project-authored and unversioned upstream state may change; `unknown` — external truth after 2026-11-25.
- Open questions: Which families are actually required by a lawful synthetic test? Would owner/legal review ever permit adoption of GPL-3.0-or-later material? Which D3D12.X claims can be tested without proprietary material?
- Result/conclusion: Named exports and compatibility roles are `documented (project-authored)` in pinned sources; public Windows D3D12 concepts are `documented (authoritative)` in PC scope only. No Xbox equivalence, compatibility, playability or performance claim is made; capability status stays `unknown`.
- Architecture/implementation impact: none — no external code, licence, dependency, compatibility layer, ADR, product code, test, CMake or config change; counterpart mappings are `hypothesis`.
- Dependencies: S0 package scope; S9 prior library lineage; S10 full family table and prior PE evidence.
- Responsible agent: researcher (XB-NEXT-DEVELOPMENT-03-R0); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-25 by researcher (XB-NEXT-DEVELOPMENT-03-R0); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason; reviewer verification is a separate, unperformed step.

### XB-NEXT-03-GFX

- ID: XB-NEXT-03-GFX
- Title: Graphics compatibility analysis and D3D12-to-D3D12 research boundary
- Package/Goal: XB-NEXT-DEVELOPMENT-03-R0 — development-only evaluation of a possible D3D12-to-D3D12 research direction; no graphics backend, ADR, shader translation or D3D12.X equivalence claim authorized.
- Research question: Which public Windows D3D12, mesh-shader and DXIL concepts could be compared with a hypothetical Xbox compatibility layer, and which guest graphics facts remain unknown?
- Scope: Xbox Series X target only; access date 2026-11-25; public Windows D3D12 and DirectX specification sources plus pinned project-authored export evidence; no proprietary SDK, key, game file, shader asset, DRM material, other-console evidence, build, CTest, GPU run or implementation.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (authoritative, public Windows D3D12/mesh/DXIL specifications only); documented (project-authored); observed; hypothesis; unknown
- Source class: authoritative; project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:3643-3673` (local read 2026-11-25); S1 raw `MicrosoftDocs/win32/docs/desktop-src/direct3d12/resource-binding.md`; S2 raw `.../using-resource-barriers-to-synchronize-resource-states-in-direct3d-12.md`; S3 raw `.../descriptor-heaps-overview.md`; S4 raw `.../managing-graphics-pipeline-state-in-direct3d-12.md`; S5 raw `.../command-queues-and-command-lists.md`; S6 raw `.../executing-and-synchronizing-command-lists.md`; S7 raw `.../memory-management-strategies.md`; S8 `https://raw.githubusercontent.com/microsoft/DirectX-Specs/master/d3d/MeshShader.md`; S9 `https://raw.githubusercontent.com/microsoft/DirectXShaderCompiler/main/docs/DXIL.rst`; S10 `https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/crates/compat/d3d12_xs/exports.txt`; S11 exact local paths `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md`, `docs/research/XB-NEXT-03_LIBRARY_FAMILIES.md`, `docs/RESEARCH_REGISTER.md:158-200` and `docs/research/XB-NEXT-03_GRAPHICS_ANALYSIS.md`. URLs accessed 2026-11-25.
- Date checked: 2026-11-25
- Concrete finding(s): F1 (`documented (authoritative, public Windows D3D12 only)` S1–S7): public D3D12 binding, descriptor heaps, PSOs, command queues, fences, resource barriers and placement/reservation strategies are specified. F2 (`documented (authoritative, public mesh/DXIL specifications only)` S8, S9): mesh/amplification PSO rules require DXIL bytecode and forbid traditional vertex/geometry/hull/domain stages in that pipeline; DXIL is a documented IR consumer contract mentioning an `offline XBOX shader compiler` without defining a guest shader binary format. F3 (`documented (project-authored)` S10, S11): a pinned external project records selected `d3d12_xs` exports and prior project research leaves D3D12.X unknowns open. F4 (`hypothesis` S1–S10, S11): comparing device creation, command lists, resources, binding, PSOs, barriers and debug surfaces is a research direction, not an accepted architecture. F5 (`unknown`): guest command encoding, shader format, D3D12.X equivalence, queue ordering, memory semantics, debug behavior and real-game rendering correctness are not established. F6 (`observed` S1–S10, S11): exact URLs returned quoted content on 2026-11-25; S8 `master` and S9 `main` are unversioned.
- Finding-to-source mapping: F1 → S1–S7; F2 → S8, S9; F3 → S10, S11; F4 → S1–S10 with S0 boundary; F5 → none recorded — see Uncertainties; F6 → S1–S10 and S11 source-inventory observations.
- Reproduction: locally read S0 and S11; directly `fetch` S1–S10 on 2026-11-25 and search the quoted anchors in S11 (`docs/research/XB-NEXT-03_GRAPHICS_ANALYSIS.md` Section 1). No build, CTest, shader compilation, game run, guest execution or GPU validation was performed.
- Uncertainties: `unknown` — Xbox guest graphics command and shader formats; `unknown` — D3D12.X semantics and real-game applicability; `observed` — unversioned upstream specs may change; `unknown` — external truth after 2026-11-25.
- Open questions: Can a lawful synthetic guest command corpus be produced without proprietary content? Which shader representation must be identified before translation research? How could guest-specific behavior be distinguished at a native boundary?
- Result/conclusion: Public D3D12 and shader-spec facts are `documented (authoritative)` in general scope only. A D3D12-to-D3D12 mapping is `hypothesis`. D3D12.X equivalence, shader translation correctness and real-game rendering remain `unknown`; no compatibility or performance claim is made.
- Architecture/implementation impact: none — no ADR, backend, shader translator, interface, product code, test, CMake, config, dependency or licence change; mapping rows in S11 are `hypothesis`.
- Dependencies: S0 package envelope; S10 pinned external export evidence; S11 full graphics analysis and prior register lineage.
- Responsible agent: researcher (XB-NEXT-DEVELOPMENT-03-R0); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-25 by researcher (XB-NEXT-DEVELOPMENT-03-R0); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason; reviewer verification is a separate, unperformed step.

### XB-NEXT-03-PERF

- ID: XB-NEXT-03-PERF
- Title: Performance measurement concept without game or native-execution claims
- Package/Goal: XB-NEXT-DEVELOPMENT-03-R0 — development-only design of a future lawful measurement concept; no benchmark, harness, product code, game run or performance target authorized.
- Research question: Which cost centers and reporting rules would be needed to compare interpreter, native and hybrid paths without extrapolating general Windows facts or synthetic results to Xbox games?
- Scope: Xbox Series X target only; access date 2026-11-25; public Microsoft general Windows x64 and D3D12 sources plus project-authored execution/native/graphics/library research; no proprietary binary, key, firmware, game asset, DRM material, other-console evidence, build, CTest, guest execution, GPU run or measurement.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation or measurement approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (authoritative, general Windows x64/public Windows D3D12 only); documented (project-authored); observed; hypothesis; unknown
- Source class: authoritative; project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:3643-3673` (local read 2026-11-25); S1 `docs/ARCHITECTURE.md:141-183`; S2 raw `MicrosoftDocs/win32/docs/desktop-src/direct3d12/command-queues-and-command-lists.md`; S3 raw `.../executing-and-synchronizing-command-lists.md`; S4 raw `.../using-resource-barriers-to-synchronize-resource-states-in-direct3d-12.md`; S5 raw `.../memory-management-strategies.md`; S6 `https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170`; S7 exact local paths `docs/research/XB-NEXT-03_EXECUTION_MODELS.md`, `docs/research/XB-NEXT-03_NATIVE_EXECUTION_FEASIBILITY.md`, `docs/research/XB-NEXT-03_GRAPHICS_ANALYSIS.md`, `docs/research/XB-NEXT-03_LIBRARY_FAMILIES.md`; S8 `docs/RESEARCH_REGISTER.md:107-119,331-355`; S9 exact local path `docs/research/XB-NEXT-03_PERFORMANCE_CONCEPT.md`. URLs accessed 2026-11-25.
- Date checked: 2026-11-25
- Concrete finding(s): F1 (`documented (project-authored)` S0, S1, S7): the hybrid direction and interpreter fallback are approved and the current interpreter is a narrow scaffold; no benchmark result exists. F2 (`documented (authoritative, public Windows D3D12 only)` S2–S5): command lists/queues, fences, resource barriers and memory strategies describe candidate graphics cost centers for Windows D3D12. F3 (`documented (authoritative, general Windows x64 only)` S6): general ABI items are candidate transition costs, not Xbox guest facts. F4 (`hypothesis` S1–S9): decomposition into guest instructions, interpreter/native paths, ABI/TLS/exception transitions, compatibility services, memory, graphics translation and frame pacing is a future measurement plan. F5 (`unknown`): guest instruction mix, service set, graphics command format, shader format and real-game feasibility are not established. F6 (`observed` S2–S6, S9): exact URLs returned quoted content on 2026-11-25 and S9 contains the full concept.
- Finding-to-source mapping: F1 → S0, S1, S7, S9; F2 → S2–S5; F3 → S6; F4 → S1–S9; F5 → none recorded — see Uncertainties; F6 → S2–S6 and S9 source-inventory observations.
- Reproduction: locally read S0, S1, S7, S8 and S9; directly `fetch` S2–S6 on 2026-11-25 and search the quoted anchors in S9 Section 1. No shell command was available in the researcher tool set; no diff check was attempted or claimed. No build, CTest, benchmark, guest execution or GPU run was performed.
- Uncertainties: `unknown` — all Xbox guest-specific counterparts and real-game behavior; `hypothesis` — candidate metrics and synthetic workloads are unexecuted; `observed` — no shell tool was available in this subagent tool set; `unknown` — external truth after 2026-11-25.
- Open questions: Which synthetic instruction subset is meaningful without guessing guest requirements? What transition threshold would justify a hybrid path, and who approves it? Can graphics translation be measured without assuming a guest format?
- Result/conclusion: General Windows/D3D12 sources are `documented (authoritative)` in general scope only; the layered measurement concept is `hypothesis`. No speedup, frame-time target, compatibility claim or game result is recorded; capability status stays `unknown`.
- Architecture/implementation impact: none — no ADR, harness, test, product code, interface, build, CMake, config, dependency or licence change; proposed metrics in S9 are `hypothesis`.
- Dependencies: S0 package envelope; S1 current interpreter scope; S7 sibling research records; S8 register boundaries; S9 full performance concept.
- Responsible agent: researcher (XB-NEXT-DEVELOPMENT-03-R0); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-25 by researcher (XB-NEXT-DEVELOPMENT-03-R0); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason; reviewer verification is a separate, unperformed step.

### XB-NEXT-03-SYNTH

- ID: XB-NEXT-03-SYNTH
- Title: XB-NEXT-DEVELOPMENT-03-R0 synthesis and final-feasibility boundary
- Package/Goal: XB-NEXT-DEVELOPMENT-03-R0 — development-only synthesis of six research documents; preserve the owner-approved hybrid direction while keeping arbitrary commercial-binary feasibility explicitly `unknown`.
- Research question: What combined conclusion is supported by the execution, native-feasibility, relocation, library, graphics and performance evidence, and which conclusions are not supported?
- Scope: Xbox Series X target only; access date 2026-11-25; synthesis of package-local public-source research and the package envelope only; no new external source, proprietary material, other-console evidence, game execution, JIT, binary rewriting, code patching, implementation, build or CTest.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (project-authored); observed; hypothesis; unknown
- Source class: project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:3643-3673` (local read 2026-11-25); S1 exact local path `docs/research/XB-NEXT-03_EXECUTION_MODELS.md`; S2 exact local path `docs/research/XB-NEXT-03_NATIVE_EXECUTION_FEASIBILITY.md`; S3 exact local path `docs/research/XB-NEXT-03_PECOFF_RELOCATIONS.md`; S4 exact local path `docs/research/XB-NEXT-03_LIBRARY_FAMILIES.md`; S5 exact local path `docs/research/XB-NEXT-03_GRAPHICS_ANALYSIS.md`; S6 exact local path `docs/research/XB-NEXT-03_PERFORMANCE_CONCEPT.md`; S7 `docs/RESEARCH_REGISTER.md:7-105,107-119` (schema and prior index). All local files read 2026-11-25.
- Date checked: 2026-11-25
- Concrete finding(s): F1 (`documented (project-authored)` S0): the owner-approved direction is hybrid native execution plus a compatibility/runtime layer, with the interpreter retained as reference, diagnostic and fallback. F2 (`documented (project-authored)` S1–S6): the package's sibling research documents record general Windows x64, PE/COFF, Win32 and public D3D12 facts against primary URLs and scoped evidence labels; this synthesis entry reports that package-authored record only and does not independently re-assert authoritative primary-source facts. F3 (`documented (project-authored)` S1–S6): only `DIR64 (10)` and `ABSOLUTE (0)` relocations are in the defined scope for future handling; external library exports are project-authored observations; D3D12-to-D3D12 is only a research approach. F4 (`hypothesis` S1–S6): native/hybrid execution, mapping and measurement may be viable for a lawful synthetic subset. F5 (`unknown`): arbitrary commercial Xbox binaries, guest CPUID/instruction requirements, ABI, TLS, exceptions/interrupts, syscalls, memory model, privileged instructions, library signatures, shader format, D3D12.X equivalence and real-game performance remain open. F6 (`observed` S1–S7): the six research documents were created on 2026-11-25 with source inventories and reproduction paths; no execution evidence was produced.
- Finding-to-source mapping: F1 → S0, S1, S2; F2 → S1–S6 as project-authored package research only; F3 → S3, S4, S5; F4 → S1–S6; F5 → none recorded — see Uncertainties; F6 → S1–S7 local reads.
- Reproduction: locally read S0–S7; follow the exact URL fetches and anchors recorded in Sections 1 of S1–S6. No shell command was available in this subagent tool set; no diff check was attempted or claimed. No build, CTest, game run, guest execution, capability validation, commit, push, merge, tag or PR was performed.
- Uncertainties: `unknown` — all guest-specific and real-game questions listed in F5; `hypothesis` — synthetic-subset viability; `observed` — all six documents are evidence-only; `unknown` — external truth after 2026-11-25.
- Open questions: What lawful synthetic corpus could test the direction without proprietary material? Which owner decisions would be required before any ADR, implementation or adoption? How should independent reviewer verification be recorded?
- Result/conclusion: The owner direction is `documented (project-authored)`; F2 reports the sibling documents' general host/API facts as `documented (project-authored)` package research only and makes no independent authoritative claim; mapping and measurement are `hypothesis`; arbitrary commercial binary feasibility is `unknown`. No compatibility, playability, performance, JIT, patching or implementation claim is supported.
- Architecture/implementation impact: none — no ADR, interface, product code, test, CMake, preset, automation, config, dependency or licence change; all candidate work remains unapproved `hypothesis`.
- Dependencies: S0 package envelope; S1–S6 sibling research documents; S7 register schema and index.
- Responsible agent: researcher (XB-NEXT-DEVELOPMENT-03-R0); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-25 by researcher (XB-NEXT-DEVELOPMENT-03-R0); Changed 2026-11-25 by researcher (XB-NEXT-DEVELOPMENT-03-R0-C1): F2 source-class relabel and Result/conclusion alignment only; no capability-status change.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason; reviewer verification is a separate, unperformed step.

### XB-NEXT-04-A

- ID: XB-NEXT-04-A
- Title: PE loader/module requirements from the supplied game-installation report
- Package/Goal: XB-NEXT-04 — development-only gap analysis comparing supplied report facts with current repository evidence; no implementation, ADR, interface or product change authorised.
- Research question: Which PE loader and module requirements are observable in the supplied reports, and what is the current XBridge evidence boundary for each?
- Scope: `documented (project-authored)` — Xbox Series X target only; local report/repository reads on 2026-11-26; no proprietary asset use, execution, decryption, JIT, patching or compatibility/performance claim; general PE facts are not promoted to Xbox guest facts.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (project-authored); observed; hypothesis; unknown
- Source class: project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:4089-4103`; S1 exact local path `D:\xbox\Spiel Daten\report\report.json` (lines 2-34,109-110,229-243,404-407,458-480,745-750,814-820,874-879,930-962,965-1060,1037-1058,1787-1797); S2 exact local path `D:\xbox\Spiel Daten\report\report.md` (lines 3-15,23-28); S3 `docs/research/XB-NEXT-04_GAME_REQUIREMENTS_GAP_ANALYSIS.md:35-49`; S4 `docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,18-24`; S5 `include/xbridge/loader/pe_image.hpp:15-40,42-59,74-116`; S6 `include/xbridge/loader/executable_loader.hpp:15-17,46-52`; S7 `include/xbridge/analysis/game_intelligence.hpp:16-30,43-60`; S8 `docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:53-55`; S9 `docs/ARCHITECTURE.md:58`.
- Date checked: 2026-11-26
- Concrete finding(s): F1 (`observed` S1, S2): the analyzer reports 26 files, six PE entries, main PE fields/sections and signing/entropy indicators. F2 (`observed` S1, S2): `XboxBvhBuilder.dll` is reported invalid with quoted DOS-header failure. F3 (`documented (project-authored)` S3-S9): XBridge has a bounded synthetic loader, synthetic PE metadata, a read-only analyzer and a restricted relocation scope. F4 (`hypothesis` S3, S4): the report may guide future bounded-reader research; it is not executability evidence. F5 (`unknown` S1, S3, S4): `.xbld`/`.retplne`/`.voltbl` semantics, the invalid local DLL's cause and optional-header Magic remain unresolved.
- Finding-to-source mapping: F1 → S1, S2; F2 → S1, S2; F3 → S3-S9; F4 → S3, S4; F5 → none recorded — see Uncertainties.
- Reproduction: directly read S0-S9 on 2026-11-26 and compare the exact line ranges in Source with aliases `RPT-JSON`/`RPT-MD` in S3; no shell command, build, CTest, game run, guest execution, capability validation, commit, push, merge or publication was performed.
- Uncertainties: `unknown` — toolchain section semantics; `unknown` — why the local `XboxBvhBuilder.dll` lacks `MZ`; `unknown` — relationship to imported `xboxbvhbuilder.dll`; `observed` — no optional-header Magic is quoted in the supplied report.
- Open questions: What lawful evidence could establish section semantics and invalid-DLL cause without protected content? Which loader rules would require an owner-approved interface or implementation package?
- Result/conclusion: `documented (project-authored)` — current XBridge loader evidence is synthetic-only; `observed` — the report supplies PE metadata and one invalid-PE observation; `unknown` — real loader requirements and Xbox behavior remain unresolved. No compatibility or execution claim.
- Architecture/implementation impact: `documented (project-authored)` — none; no ADR, interface, code, test, CMake, preset, dependency or licence change; future loader work requires a separate approved envelope.
- Dependencies: S0 package envelope; S3/S4 analysis and matrix; S5-S9 cited repository evidence. No implementation dependency started.
- Responsible agent: researcher (XB-NEXT-04); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-26 by researcher (XB-NEXT-04); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — criteria 1-8 of `docs/RESEARCH_REGISTER.md:94-105` satisfied: unique ID; 23 non-empty fields; allowed tokens; documented source class; finding mapping; labeled uncertainties; declared scope; Source/Reproduction recorded for `unknown`; reviewer verification is a separate, unperformed step.

### XB-NEXT-04-B

- ID: XB-NEXT-04-B
- Title: XMem and guest-memory requirements from the supplied report
- Package/Goal: XB-NEXT-04 — development-only gap analysis comparing supplied report facts with current repository evidence; no implementation, ADR, interface or product change authorised.
- Research question: Which XMem and memory requirements are observable in the supplied reports, and what is the current XBridge evidence boundary for each?
- Scope: `documented (project-authored)` — Xbox Series X target only; local report/repository reads on 2026-11-26; no proprietary asset use, execution, decryption, JIT, patching or compatibility/performance claim; generic host/guest memory facts are not promoted to Xbox guest facts.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (project-authored); observed; hypothesis; unknown
- Source class: project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:4089-4103`; S1 exact local path `D:\xbox\Spiel Daten\report\report.json` (lines 266-269,1247-1265,1525-1534,1788-1791); S2 exact local path `D:\xbox\Spiel Daten\report\report.md` (line 54); S3 `docs/research/XB-NEXT-04_GAME_REQUIREMENTS_GAP_ANALYSIS.md:51-62`; S4 `docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,30-34`; S5 `docs/ARCHITECTURE.md:30-38,58,194-196`; S6 `include/xbridge/runtime/system_services.hpp:7-28,75-78`; S7 `docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:49`; S8 `include/xbridge/loader/executable_loader.hpp:19-22,61-62`.
- Date checked: 2026-11-26
- Concrete finding(s): F1 (`observed` S1, S2): the main exe imports twelve `xmem.dll` symbols and Bink imports `XMemAlloc`/`XMemFree`. F2 (`observed` S1): the main exe exports `__XMemAllocationHooks__` and imports generic virtual-memory and APU-address helpers. F3 (`documented (project-authored)` S3-S8): XBridge has generic bounded GuestMemory, a neutral VirtualMemory service ID, loader physical backing and a recorded research lead only. F4 (`hypothesis` S3, S4): generic memory seams could support future lawful study; no XMem behavior is implemented. F5 (`unknown` S1, S3, S4): signatures, page semantics, hook shape and guest-address ownership remain unresolved.
- Finding-to-source mapping: F1 → S1, S2; F2 → S1; F3 → S3-S8; F4 → S3, S4; F5 → none recorded — see Uncertainties.
- Reproduction: directly read S0-S8 on 2026-11-26 and compare the exact line ranges in Source with alias `RPT-JSON`/`RPT-MD` in S3; no shell command, build, CTest, game run, guest execution, capability validation, commit, push, merge or publication was performed.
- Uncertainties: `unknown` — XMem signatures and allocation policies; `unknown` — physical-page and working-set semantics; `unknown` — guest virtual-memory layout; `observed` — XBridge GuestMemory is generic and explicitly not an Xbox VM model.
- Open questions: Which lawful evidence could establish Xbox memory-service semantics? Which owner decision would be needed before changing the guest-memory contract?
- Result/conclusion: `documented (project-authored)` — current XBridge memory evidence is generic/synthetic; `observed` — the report supplies XMem-related strings; `unknown` — Xbox memory capability remains unresolved. No implementation or compatibility claim.
- Architecture/implementation impact: `documented (project-authored)` — none; no ADR, memory interface, code, test, CMake, preset, dependency or licence change; future memory work requires a separate approved envelope.
- Dependencies: S0 package envelope; S3/S4 analysis and matrix; S5-S8 cited repository evidence. No implementation dependency started.
- Responsible agent: researcher (XB-NEXT-04); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-26 by researcher (XB-NEXT-04); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — criteria 1-8 of `docs/RESEARCH_REGISTER.md:94-105` satisfied: unique ID; 23 non-empty fields; allowed tokens; documented source class; finding mapping; labeled uncertainties; declared scope; Source/Reproduction recorded for `unknown`; reviewer verification is a separate, unperformed step.

### XB-NEXT-04-C

- ID: XB-NEXT-04-C
- Title: x64 ABI, TLS, SEH, CRT and runtime requirements from the supplied report
- Package/Goal: XB-NEXT-04 — development-only gap analysis comparing supplied report facts with current repository evidence; no implementation, ADR, interface or product change authorised.
- Research question: Which CPU/ABI/TLS/SEH/CRT/runtime requirements are observable in the supplied reports, and what is the current XBridge evidence boundary for each?
- Scope: `documented (project-authored)` — Xbox Series X target only; local report/repository reads on 2026-11-26; no proprietary asset use, execution, decryption, JIT, patching or compatibility/performance claim; general x64/Windows facts are not promoted to Xbox guest facts.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (project-authored); observed; hypothesis; unknown
- Source class: project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:4089-4103`; S1 exact local path `D:\xbox\Spiel Daten\report\report.json` (lines 959,1005-1027,1063-1786,1427-1431,1576-1661,1792); S2 exact local path `D:\xbox\Spiel Daten\report\report.md` (lines 23-28); S3 `docs/research/XB-NEXT-04_GAME_REQUIREMENTS_GAP_ANALYSIS.md:64-76`; S4 `docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,40-45`; S5 `docs/ARCHITECTURE.md:89-104,106-122,150-192`; S6 `include/xbridge/execution/native_execution_backend.hpp:10-33`; S7 `docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:20-28,37-39,49`.
- Date checked: 2026-11-26
- Concrete finding(s): F1 (`observed` S1, S2): valid PEs report AMD64 and the main exe reports TLS, `.pdata`, CRT and unwind imports. F2 (`documented (project-authored)` S3-S7): XBridge has x64 state storage, a narrow decoder/interpreter and fixed synthetic native routines. F3 (`hypothesis` S3, S4): a lawful synthetic subset may be useful for future study; no guest execution is enabled. F4 (`unknown` S1, S3, S4, S7): guest CPUID/AVX2/BMI/FMA needs, ABI, TLS order, exceptions, syscalls and privileged-instruction behavior remain unresolved.
- Finding-to-source mapping: F1 → S1, S2; F2 → S3-S7; F3 → S3, S4; F4 → none recorded — see Uncertainties.
- Reproduction: directly read S0-S7 on 2026-11-26 and compare the exact line ranges in Source with alias `RPT-JSON`/`RPT-MD` in S3; no shell command, build, CTest, game run, guest execution, capability validation, commit, push, merge or publication was performed.
- Uncertainties: `unknown` — guest CPUID/ISA requirements; `unknown` — guest calling convention and TLS callback semantics; `unknown` — exception, interrupt, syscall and privileged-instruction surface; `observed` — current native IDs are compiled-in synthetic routines only.
- Open questions: What lawful synthetic evidence could establish ABI/TLS/SEH requirements? Which owner decision would be required before any native-path or CPU-state change?
- Result/conclusion: `documented (project-authored)` — current XBridge CPU evidence is narrow and synthetic; `observed` — the report supplies static ABI/TLS/SEH/CRT indicators; `unknown` — guest execution capability remains unresolved. No compatibility or performance claim.
- Architecture/implementation impact: `documented (project-authored)` — none; no ADR, CPU interface, code, test, CMake, preset, dependency or licence change; future execution work requires a separate approved envelope.
- Dependencies: S0 package envelope; S3/S4 analysis and matrix; S5-S7 cited repository evidence. No implementation dependency started.
- Responsible agent: researcher (XB-NEXT-04); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-26 by researcher (XB-NEXT-04); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — criteria 1-8 of `docs/RESEARCH_REGISTER.md:94-105` satisfied: unique ID; 23 non-empty fields; allowed tokens; documented source class; finding mapping; labeled uncertainties; declared scope; Source/Reproduction recorded for `unknown`; reviewer verification is a separate, unperformed step.

### XB-NEXT-04-D

- ID: XB-NEXT-04-D
- Title: D3D12.X, XG, FFX and BVH graphics requirements from the supplied report
- Package/Goal: XB-NEXT-04 — development-only gap analysis comparing supplied report facts with current repository evidence; no implementation, ADR, interface or product change authorised.
- Research question: Which graphics requirements are observable in the supplied reports, and what is the current XBridge evidence boundary for D3D12.X, XG, FFX and BVH families?
- Scope: `documented (project-authored)` — Xbox Series X target only; local report/repository reads on 2026-11-26; no proprietary asset use, shader extraction, execution, decryption, JIT, patching or compatibility/performance claim; public PC/Windows D3D12 facts are not promoted to Xbox D3D12.X facts.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (project-authored); observed; hypothesis; unknown
- Source class: project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:4089-4103`; S1 exact local path `D:\xbox\Spiel Daten\report\report.json` (lines 753-756,866-877,930-962,1324-1328,1452-1472); S2 exact local path `D:\xbox\Spiel Daten\report\report.md` (lines 33,42,48,51); S3 `docs/research/XB-NEXT-04_GAME_REQUIREMENTS_GAP_ANALYSIS.md:78-88`; S4 `docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,51-56`; S5 `docs/ARCHITECTURE.md:42-52,231-233`; S6 `include/xbridge/runtime/system_services.hpp:7-28,100-103`; S7 `docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:61`; S8 `include/xbridge/analysis/game_intelligence.hpp:53-60`.
- Date checked: 2026-11-26
- Concrete finding(s): F1 (`observed` S1, S2): the main exe imports D3D12.X, XG, FFX and mangled BVH names; the local `XboxBvhBuilder.dll` is invalid. F2 (`documented (project-authored)` S3-S8): XBridge requires separate graphics layers and keeps D3D12-to-D3D12 as a research approach only. F3 (`hypothesis` S3, S4): the strings may inform future lawful graphics research. F4 (`unknown` S1, S3, S4, S7, S8): command encoding, shader format, D3D12.X layout, BVH semantics, swizzles and rendering correctness remain unresolved.
- Finding-to-source mapping: F1 → S1, S2; F2 → S3-S8; F3 → S3, S4; F4 → none recorded — see Uncertainties.
- Reproduction: directly read S0-S8 on 2026-11-26 and compare the exact line ranges in Source with alias `RPT-JSON`/`RPT-MD` in S3; no shell command, build, CTest, GPU run, game run, guest execution, capability validation, commit, push, merge or publication was performed.
- Uncertainties: `unknown` — D3D12.X equivalence and command format; `unknown` — guest shader binary format and BVH/XDXR semantics; `unknown` — local invalid-DLL relationship to imported `xboxbvhbuilder.dll`; `hypothesis` — any future mapping remains unapproved.
- Open questions: What lawful evidence could establish guest graphics formats? Which ADR/owner decision would be required before any backend, interface or translator work?
- Result/conclusion: `documented (project-authored)` — current XBridge graphics evidence is strategy-only; `observed` — the report supplies graphics import strings; `unknown` — graphics capability and correctness remain unresolved. No rendering, compatibility or performance claim.
- Architecture/implementation impact: `documented (project-authored)` — none; no ADR, graphics backend, interface, code, test, CMake, preset, dependency or licence change; future graphics work requires a separate approved envelope.
- Dependencies: S0 package envelope; S3/S4 analysis and matrix; S5-S8 cited repository evidence. No implementation dependency started.
- Responsible agent: researcher (XB-NEXT-04); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-26 by researcher (XB-NEXT-04); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — criteria 1-8 of `docs/RESEARCH_REGISTER.md:94-105` satisfied: unique ID; 23 non-empty fields; allowed tokens; documented source class; finding mapping; labeled uncertainties; declared scope; Source/Reproduction recorded for `unknown`; reviewer verification is a separate, unperformed step.

### XB-NEXT-04-E

- ID: XB-NEXT-04-E
- Title: Bink, chad, xapu and Media Foundation audio/video requirements
- Package/Goal: XB-NEXT-04 — development-only gap analysis comparing supplied report facts with current repository evidence; no implementation, ADR, interface or product change authorised.
- Research question: Which audio/video requirements are observable in the supplied reports, and what is the current XBridge evidence boundary for Bink, chad, xapu and Media Foundation families?
- Scope: `documented (project-authored)` — Xbox Series X target only; local report/repository reads on 2026-11-26; no proprietary asset use, codec extraction, execution, decryption, JIT, patching or compatibility/performance claim; import names are static metadata only.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (project-authored); observed; hypothesis; unknown
- Source class: project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:4089-4103`; S1 exact local path `D:\xbox\Spiel Daten\report\report.json` (lines 297-299,351-403,1399-1408,1445-1450,1518-1559); S2 exact local path `D:\xbox\Spiel Daten\report\report.md` (lines 36,39); S3 `docs/research/XB-NEXT-04_GAME_REQUIREMENTS_GAP_ANALYSIS.md:90-103`; S4 `docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,62-66`; S5 `include/xbridge/runtime/system_services.hpp:7-28,65-68,95-98`; S6 `docs/ARCHITECTURE.md:63-69,76-77`.
- Date checked: 2026-11-26
- Concrete finding(s): F1 (`observed` S1, S2): the main exe imports Bink, chad, xapu and Media Foundation symbols; Bink DLL exports Bink functions and imports an XAudio2 ordinal. F2 (`documented (project-authored)` S3-S6): XBridge has neutral Audio/Thread markers and repository non-goals exclude commercial content. F3 (`hypothesis` S3, S4): these families may define future lawful research questions. F4 (`unknown` S1, S3, S4): codecs, frame buffers, DSP graphs, APU commands and media-source semantics remain unresolved.
- Finding-to-source mapping: F1 → S1, S2; F2 → S3-S6; F3 → S3, S4; F4 → none recorded — see Uncertainties.
- Reproduction: directly read S0-S6 on 2026-11-26 and compare the exact line ranges in Source with alias `RPT-JSON`/`RPT-MD` in S3; no shell command, build, CTest, audio/video playback, game run, guest execution, capability validation, commit, push, merge or publication was performed.
- Uncertainties: `unknown` — codec and frame-buffer formats; `unknown` — DSP/APU command and result formats; `unknown` — XAudio2 ordinal signature and Media Foundation behavior; `observed` — no media file was executed or decoded.
- Open questions: What lawful synthetic media evidence could establish service requirements? Which owner/legal decision would be required before any codec or content-adjacent work?
- Result/conclusion: `documented (project-authored)` — current XBridge audio/video evidence is neutral-interface only; `observed` — the report supplies audio/video import strings; `unknown` — audio/video capability remains unresolved. No compatibility or performance claim.
- Architecture/implementation impact: `documented (project-authored)` — none; no ADR, audio/video interface, code, test, CMake, preset, dependency or licence change; future media work requires a separate approved envelope.
- Dependencies: S0 package envelope; S3/S4 analysis and matrix; S5/S6 cited repository evidence. No implementation dependency started.
- Responsible agent: researcher (XB-NEXT-04); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-26 by researcher (XB-NEXT-04); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — criteria 1-8 of `docs/RESEARCH_REGISTER.md:94-105` satisfied: unique ID; 23 non-empty fields; allowed tokens; documented source class; finding mapping; labeled uncertainties; declared scope; Source/Reproduction recorded for `unknown`; reviewer verification is a separate, unperformed step.

### XB-NEXT-04-F

- ID: XB-NEXT-04-F
- Title: Config identity, HTTP, Winsock, input, window, crypto and registry requirements
- Package/Goal: XB-NEXT-04 — development-only gap analysis comparing supplied report facts with current repository evidence; no implementation, ADR, interface or product change authorised.
- Research question: Which system/network/input requirements are observable in the supplied reports, and what is the current XBridge evidence boundary for config identity and service families?
- Scope: `documented (project-authored)` — Xbox Series X target only; local report/repository reads on 2026-11-26; no proprietary asset use, credential handling, execution, decryption, JIT, patching or compatibility/performance claim; import names are static metadata only.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (project-authored); observed; hypothesis; unknown
- Source class: project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:4089-4103`; S1 exact local path `D:\xbox\Spiel Daten\report\report.json` (lines 137-177,591-606,608-614,1063-1079,1271-1307,1329-1374,1399-1440,1474-1512,1565-1568); S2 exact local path `D:\xbox\Spiel Daten\report\report.md` (lines 45,56-70); S3 `docs/research/XB-NEXT-04_GAME_REQUIREMENTS_GAP_ANALYSIS.md:105-118`; S4 `docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,72-80`; S5 `include/xbridge/analysis/game_intelligence.hpp:38-40`; S6 `include/xbridge/runtime/system_services.hpp:7-28,60-73,90-113`; S7 `docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:43-49`; S8 `docs/ARCHITECTURE.md:63-69`.
- Date checked: 2026-11-26
- Concrete finding(s): F1 (`observed` S1): `MicrosoftGame.Config` reports identity/title fields. F2 (`observed` S1, S2): the PEs import HTTP, Winsock, input, window, crypto and registry symbols. F3 (`documented (project-authored)` S3-S8): XBridge recognizes `gdk_manifest` as a string type and defines neutral service IDs without implementations. F4 (`hypothesis` S3, S4): the families may define future lawful system-service research. F5 (`unknown` S1, S3, S4, S7): service signatures, title enforcement, input protocol and network/window/crypto/registry behavior remain unresolved.
- Finding-to-source mapping: F1 → S1; F2 → S1, S2; F3 → S3-S8; F4 → S3, S4; F5 → none recorded — see Uncertainties.
- Reproduction: directly read S0-S8 on 2026-11-26 and compare the exact line ranges in Source with alias `RPT-JSON`/`RPT-MD` in S3; no shell command, build, CTest, network call, game run, guest execution, capability validation, commit, push, merge or publication was performed.
- Uncertainties: `unknown` — title identity enforcement; `unknown` — HTTP/Winsock and input service semantics; `unknown` — window, crypto and registry behavior; `observed` — the analyzer's non-core DLL list is its own heuristic.
- Open questions: What lawful evidence could establish Xbox system-service semantics? Which owner decisions would be required before network, input or security-adjacent work?
- Result/conclusion: `documented (project-authored)` — current XBridge system evidence is neutral-interface only; `observed` — the report supplies config/static service strings; `unknown` — system-service capability remains unresolved. No compatibility or performance claim.
- Architecture/implementation impact: `documented (project-authored)` — none; no ADR, service interface, code, test, CMake, preset, dependency or licence change; future system work requires a separate approved envelope.
- Dependencies: S0 package envelope; S3/S4 analysis and matrix; S5-S8 cited repository evidence. No implementation dependency started.
- Responsible agent: researcher (XB-NEXT-04); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-26 by researcher (XB-NEXT-04); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — criteria 1-8 of `docs/RESEARCH_REGISTER.md:94-105` satisfied: unique ID; 23 non-empty fields; allowed tokens; documented source class; finding mapping; labeled uncertainties; declared scope; Source/Reproduction recorded for `unknown`; reviewer verification is a separate, unperformed step.

### XB-NEXT-04-G

- ID: XB-NEXT-04-G
- Title: RPF/TTPF and DStorage container/streaming requirements
- Package/Goal: XB-NEXT-04 — development-only gap analysis comparing supplied report facts with current repository evidence; no implementation, ADR, interface or product change authorised.
- Research question: Which container/streaming requirements are observable in the supplied reports, and what is the current XBridge evidence boundary for RPF/TTPF and DStorage families?
- Scope: `documented (project-authored)` — Xbox Series X target only; local report/repository reads on 2026-11-26; no proprietary asset use, extraction, execution, decryption, JIT, patching or compatibility/performance claim; extension hints are not format proof.
- Capability status: unknown
- Workflow/lifecycle status: complete evidence record; no implementation approved; independent reviewer verification not yet recorded
- Evidence label(s): documented (project-authored); observed; hypothesis; unknown
- Source class: project-authored; N/A (non-documented sub-claims)
- Source: S0 `docs/AGENT_TASKS.md:4089-4103`; S1 exact local path `D:\xbox\Spiel Daten\report\report.json` (lines 5-55,113-115,140,1465-1467); S2 exact local path `D:\xbox\Spiel Daten\report\report.md` (lines 5,9-10); S3 `docs/research/XB-NEXT-04_GAME_REQUIREMENTS_GAP_ANALYSIS.md:120-131`; S4 `docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,86-91`; S5 `include/xbridge/analysis/game_intelligence.hpp:11-13,16-30,38-40,53-60`; S6 `include/xbridge/runtime/system_services.hpp:7-28,80-88`; S7 `docs/RESEARCH_REGISTER.md:361`.
- Date checked: 2026-11-26
- Concrete finding(s): F1 (`observed` S1, S2): the analyzer lists `.rpf`/`.ttpf` sizes, a `containers: []` field and `DStorageGetFactory` import. F2 (`documented (project-authored)` S3-S7): XBridge's analyzer is read-only, has uncertainty fields and only a neutral Storage ID; prior register evidence leaves container internals unknown. F3 (`hypothesis` S3, S4): these facts may guide future lawful asset research. F4 (`unknown` S1, S3, S4, S7): encrypted/packed classification meaning, layouts and chunk semantics remain unresolved.
- Finding-to-source mapping: F1 → S1, S2; F2 → S3-S7; F3 → S3, S4; F4 → none recorded — see Uncertainties.
- Reproduction: directly read S0-S7 on 2026-11-26 and compare the exact line ranges in Source with alias `RPT-JSON`/`RPT-MD` in S3; no shell command, build, CTest, extraction, decryption, game run, guest execution, capability validation, commit, push, merge or publication was performed.
- Uncertainties: `unknown` — `.rpf`/`.ttpf` internal layout; `unknown` — catalog and alignment semantics; `unknown` — meaning of zero encrypted/packed classification; `observed` — no container was extracted or decoded.
- Open questions: What lawful evidence could establish container formats? Which owner/legal decision would be required before any encryption-adjacent or asset-format work?
- Result/conclusion: `documented (project-authored)` — current XBridge container evidence is read-only/neutral only; `observed` — the report supplies extension and size metadata; `unknown` — container and streaming capability remains unresolved. No compatibility or performance claim.
- Architecture/implementation impact: `documented (project-authored)` — none; no ADR, storage interface, code, test, CMake, preset, dependency or licence change; future asset work requires a separate approved envelope.
- Dependencies: S0 package envelope; S3/S4 analysis and matrix; S5-S7 cited repository evidence. No implementation dependency started.
- Responsible agent: researcher (XB-NEXT-04); independent reviewer verification is required and not yet recorded in this entry.
- Created/Changed: Created 2026-11-26 by researcher (XB-NEXT-04); Changed N/A (first creation).
- Conformance status: conformant
- Missing fields if non-conformant: none — criteria 1-8 of `docs/RESEARCH_REGISTER.md:94-105` satisfied: unique ID; 23 non-empty fields; allowed tokens; documented source class; finding mapping; labeled uncertainties; declared scope; Source/Reproduction recorded for `unknown`; reviewer verification is a separate, unperformed step.

## 9. Assessment and audit notes

- `observed` — after XB-NEXT-04 the compact index has 23 rows and Section 8 has 23 detail blocks with the same IDs: R-001, R-002, R-003, R-004, R-005, R-006, RESEARCH-007, XB-0084, R-XB-NEXT-02-GI, XB-NEXT-03-EXEC, XB-NEXT-03-NATIVE, XB-NEXT-03-RELOC, XB-NEXT-03-LIBS, XB-NEXT-03-GFX, XB-NEXT-03-PERF, XB-NEXT-03-SYNTH, XB-NEXT-04-A, XB-NEXT-04-B, XB-NEXT-04-C, XB-NEXT-04-D, XB-NEXT-04-E, XB-NEXT-04-F, XB-NEXT-04-G. Conformance distribution: 23 `conformant`, 0 `non-conformant`, 0 `not_assessed`, 0 `not_applicable`. Capability-status distribution: 23 `unknown`, 0 `not investigated`, 0 `investigated-unsupported`, 0 `implemented-unvalidated`, 0 `validated`.
- `observed` — AC-2's original wording says "seven rows" because it predates the XB-0084, XB-NEXT-03 and XB-NEXT-04 rows; all **twenty-three** current rows are assessed here. No row is `validated` or `implemented-unvalidated`; class-4/class-5 items from RESEARCH-007 remain unapproved and `unknown`, and genuinely unknown items stay unknown.
- `observed` — AC-5 review is deliberately not performed here: the independent reviewer verifies conformance with file:line evidence, and the coordinator records that verdict later. No reviewer PASS for XB-0080-A is claimed in this file.
- `unknown` — external web truth, license/legal effect and fresh web access were not re-checked by XB-0080-A; no new research, build, CTest, staging, commit or publication was performed by XB-0080-A.
- `observed` — XB-0083-AUTHORITATIVE-EVIDENCE re-fetched authoritative public sources on 2026-11-24 for R-001…R-006 and recorded exact URLs, access date, scoped evidence labels and reproduction in `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md`. The prior eight rows' conformance and all-`unknown` capability-status distribution are unchanged; the ninth row `R-XB-NEXT-02-GI` was added on 2026-11-25 with capability status `unknown`, and no row was promoted to `validated` or `implemented-unvalidated`.
- `observed` — XB-NEXT-DEVELOPMENT-03-R0 added seven entries on 2026-11-25 (`XB-NEXT-03-EXEC`, `XB-NEXT-03-NATIVE`, `XB-NEXT-03-RELOC`, `XB-NEXT-03-LIBS`, `XB-NEXT-03-GFX`, `XB-NEXT-03-PERF`, `XB-NEXT-03-SYNTH`), each with capability status `unknown` and researcher-recorded `conformant`. Their six supporting documents are evidence-only; no implementation, ADR, game run, guest execution, build, CTest or capability promotion was performed, and independent reviewer verification is not yet recorded. No shell tool was available in this researcher tool set, so `git diff --check` was not run in this package.
- `observed` — XB-NEXT-DEVELOPMENT-03-R0-C1 corrected RV-R0-01 (security-boundary wording in `XB-NEXT-03_NATIVE_EXECUTION_FEASIBILITY.md` and register `XB-NEXT-03-NATIVE`), RV-R0-02 (`XB-NEXT-03-SYNTH` source-class relabel and Result/conclusion alignment) and RV-R0-03 (performance document `S9` inventory/reproduction alignment). Section 7 had sixteen rows and Section 8 had sixteen detail blocks immediately after that correction; after XB-NEXT-04 both sections have 23 rows, and all seven XB-NEXT-03 capability statuses remain `unknown`. Mandatory independent reviewer re-check has not yet happened.
- `unknown` — web search integration returned HTTP 402 during XB-0083; direct webfetch was used instead. AMD press-release URLs attempted did not yield a usable primary CPU document, so no AMD-specific claim is recorded as authoritative. Fresh external truth after 2026-11-24 was not re-checked.
- `observed` — XB-NEXT-04 added seven entries (`XB-NEXT-04-A` through `XB-NEXT-04-G`) on 2026-11-26 from the supplied analyzer reports and local repository evidence; each has capability status `unknown` and researcher-recorded `conformant`. No reviewer PASS is claimed. No product code, test, build configuration, execution, decryption, commit, push, merge, tag, PR or publication was performed in this researcher package.
