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

## 9. Assessment and audit notes

- `observed` — the compact index has eight rows and Section 8 has eight detail blocks with the same IDs: R-001, R-002, R-003, R-004, R-005, R-006, RESEARCH-007, XB-0084. Conformance distribution: 8 `conformant`, 0 `non-conformant`, 0 `not_assessed`, 0 `not_applicable`. Capability-status distribution: 8 `unknown`, 0 `not investigated`, 0 `investigated-unsupported`, 0 `implemented-unvalidated`, 0 `validated`.
- `observed` — AC-2's original wording says "seven rows" because it predates the XB-0084 row; all **eight** current rows are assessed here. No row is `validated` or `implemented-unvalidated`; class-4/class-5 items from RESEARCH-007 remain unapproved and `unknown`, and genuinely unknown items stay unknown.
- `observed` — AC-5 review is deliberately not performed here: the independent reviewer verifies conformance with file:line evidence, and the coordinator records that verdict later. No reviewer PASS for XB-0080-A is claimed in this file.
- `unknown` — external web truth, license/legal effect and fresh web access were not re-checked by XB-0080-A; no new research, build, CTest, staging, commit or publication was performed by XB-0080-A.
- `observed` — XB-0083-AUTHORITATIVE-EVIDENCE re-fetched authoritative public sources on 2026-11-24 for R-001…R-006 and recorded exact URLs, access date, scoped evidence labels and reproduction in `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md`. Row count, all-eight conformance distribution and the all-`unknown` capability-status distribution are unchanged; no row was promoted to `validated` or `implemented-unvalidated`.
- `unknown` — web search integration returned HTTP 402 during XB-0083; direct webfetch was used instead. AMD press-release URLs attempted did not yield a usable primary CPU document, so no AMD-specific claim is recorded as authoritative. Fresh external truth after 2026-11-24 was not re-checked.
