# XB-0083 — Authoritative public-source re-verification of the R-001…R-006 claims

This file is **development infrastructure** and is not part of the product package. It is the detailed evidence record for research package `XB-0083-AUTHORITATIVE-EVIDENCE` (decision B1, owner technical delegation). It builds on the already-recorded R-001…R-006 rows in `docs/RESEARCH_REGISTER.md`; it does not re-run the earlier RESEARCH-007 or XB-0084 research.

## 1. Envelope, authority and boundaries

- PACKAGE_ID: XB-0083-AUTHORITATIVE-EVIDENCE (decision B1)
- PACKAGE_TYPE: RESEARCH
- PRODUCT_BOUNDARY: development-only
- GOAL: Re-verify the six project-authored external claims behind R-001…R-006 against authoritative public Microsoft/AMD sources, keeping unverified Xbox-specific items `unknown`.
- FILES: this file (new); `docs/RESEARCH_REGISTER.md`.
- ALLOWED: lawful public web research with exact URLs and access date; evidence-labeled findings; scoped register updates.
- FORBIDDEN: other-console claims as Xbox evidence; NDA/GDK/private material; proprietary firmware, keys or game content; DRM circumvention; code copying; product, ADR, interface, dependency, test, build or config changes; compatibility or performance claims derived from translation counts.
- ACCESS DATE for every URL in this record: **2026-11-24**.
- WRITE SCOPE actually used: only `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md` and `docs/RESEARCH_REGISTER.md`. No shell, Git, CMake, CTest, ledger, roadmap, product code, test or configuration commands or edits were performed.
- Evidence-label and source-class tokens follow `docs/RESEARCH_REGISTER.md` Sections 2–6. Capability status is a separate axis: all six rows remain `unknown` because no executed package-local validation exists. `partial` is retained only as a workflow note for R-005 and is never a capability token.

## 2. Source inventory (exact URL, access date 2026-11-24)

| ID | Exact URL / local path | Access date | What it establishes | Scope limit |
|---|---|---|---|---|
| A1 | `https://www.xbox.com/en-US/consoles/xbox-series-x` | 2026-11-24 | Official Xbox Series X tech specs: "CPU. 8X Cores @ 3.8 GHz (3.6 GHz w/SMT) Custom Zen 2 CPU"; "GPU. 12 TFLOPS, 52 CUs @1.825 GHz Custom RDNA 2 GPU"; "Memory. 16GB GDDR6 w/320 bit-wide bus"; "Memory Bandwidth. 10GB @ 560 GB/s, 6GB @ 336 GB/s"; "I/O Throughput. 2.4 GB/s (Raw), 4.8 GB/s (Compressed, with custom hardware decompression block)"; custom NVMe SSD; Xbox Velocity Architecture; Quick Resume; 3D Spatial Sound; Storage Expansion Cards; backward compatibility | Published hardware/user-facing facts only; no guest-visible CPUID, memory map, API surface or service list |
| A2 | `https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention` | 2026-11-24 | Windows x64 calling convention: integer arguments in RCX/RDX/R8/R9, floating-point in XMM0–XMM3, caller-allocated shadow store, 16-byte stack alignment, volatile/nonvolatile registers, unwind data, MXCSR/FPCSR rules | General Windows x64/AMD64, **not** an Xbox guest ABI |
| A3 | `https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions` | 2026-11-24 | Windows x64 ABI conventions: `__fastcall`, scalar/aggregate alignment, register volatility, R10:R11 used in syscall/sysret, direction flag clear on Windows system calls, PE32+ image format | General Windows x64, **not** an Xbox syscall or service interface |
| A4 | `https://learn.microsoft.com/en-us/windows/win32/direct3d12/direct3d-12-graphics` | 2026-11-24 | Public Direct3D 12 programming guide; D3D12 headers and libraries are part of the Windows 10 SDK | Public PC/Windows D3D12, **not** Xbox D3D12.X |
| A5 | `https://raw.githubusercontent.com/microsoft/DirectX-Specs/master/d3d/MeshShader.md` | 2026-11-24 | Official Microsoft DirectX-Specs mesh shader spec v0.86: mesh/amplification shaders supported only for D3D12; IA disabled; mesh inputs are thread IDs plus optional group-uniform payload; `[numthreads]` X*Y*Z <= 128; PSO creation requires DXIL bytecode and forbids VS/GS/HS/DS; signature linkage by semantic name and index, not packed location; public API calls include CheckFeatureSupport, CreatePipelineState, DispatchMesh, ExecuteIndirect | Public D3D12 semantics, **not** an Xbox guest shader or command format |
| A6 | `https://raw.githubusercontent.com/microsoft/DirectXShaderCompiler/main/docs/DXIL.rst` | 2026-11-24 | Official Microsoft DXIL spec: DXIL is a subset of LLVM 3.7 IR; shader models include mesh (ms) and amplification (as) at 6.5; signature points include MSIn/MSOut/MSPOut/ASIn; text mentions an "offline XBOX shader compiler" as one possible IR consumer | No Xbox binary layout is specified by that mention; no Xbox format claim |
| A7 | `https://learn.microsoft.com/en-us/windows/win32/dstorage/dstorage-api-reference` | 2026-11-24 | Public Windows reference for APIs declared in `dstorage.h` | Public Windows API, **not** an Xbox console API |
| A8 | `https://devblogs.microsoft.com/directx/directstorage-api-downloads/` | 2026-11-24 | Official DirectX blog DirectStorage SDK landing page: SDK releases, `microsoft/DirectStorage` GitHub samples/guidance, GDeflate and Zstd decompression support in SDK releases | Public Windows/PC SDK, **not** proof of equivalence to the console decompression block |
| A9 | exact local path `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md` | 2026-11-24 | This file: full inventory, claim-by-claim 23-field records, conformance evidence and reproduction path | Package-local documentary record; not a product source |
| P1 | exact local path `docs/RESEARCH_REGISTER.md` (RESEARCH-007 detail block) | 2026-11-24 (read-back) | Project-authored external claim lineage; never authoritative Xbox evidence | External project statements only |
| P2 | exact local path `docs/AGENT_TASKS.md:254-265` and `docs/AGENT_TASKS.md:1493-1502` | 2026-11-24 (read-back) | Original XB-0083 envelope and B1 intent listing Zen 2, AVX2, Windows x64 ABI, D3D12.x, shader/mesh formats | Project-authored task text, not external evidence |

### Attempted but unusable sources (recorded as `unknown`, not as evidence)

- `https://www.amd.com/en/corporate/press-releases/amd-to-power-next-generation-xbox-consoles-with-custom-zen-2-and-rDNA-architecture` — request timed out on two attempts (2026-11-24); no AMD primary CPU press-release text was obtained.
- `https://www.amd.com/en/newsroom/press-releases/2019-10-8-amd-to-power-next-generation-xbox-consoles-with-custom-zen-2-and-rdna-architecture.html` — request timed out (2026-11-24).
- `https://ir.amd.com/news-events/press-releases/detail/952/amd-to-power-next-generation-xbox-consoles-with-custom-zen-2` — resolved to an unrelated 2020 AMD EPYC/AWS press release; not usable for any Xbox or Zen 2 claim.
- `https://news.xbox.com/en-us/2020/03/16/xbox-series-x-games-velocity-architecture/`, `https://news.xbox.com/en-us/2020/03/16/power-of-xbox-series-x/`, `https://news.xbox.com/en-us/2019/12/12/introducing-xbox-series-x/`, `https://news.xbox.com/en-us/2020/03/16/xbox-series-x-technical-specifications/` — HTTP 404 (2026-11-24); replaced by A1.
- `https://learn.microsoft.com/en-us/windows/win32/direct3d12/directstorage` — HTTP 404 (2026-11-24); replaced by A7/A8.
- `https://learn.microsoft.com/en-us/windows/win32/direct3d12/mesh-shaders` — HTTP 404 (2026-11-24); replaced by A5.
- `websearch` integration returned HTTP 402 for all three bundled queries (2026-11-24); direct `webfetch` on the exact URLs above was used instead. No search-result snippet is used as evidence.

No GDK, NDA, private, proprietary or other-console source was used. Nothing in this record establishes a Series S fact.

## 3. Result summary (evidence label kept separate from capability status)

| Row | Authoritative public evidence obtained | Still `unknown` | Capability status |
|---|---|---|---|
| R-001 | Custom Zen 2 8-core CPU hardware fact (A1); general Windows x64 ABI (A2, A3) | Guest-visible CPUID, AVX2 on the custom part, guest ABI/syscall interface | unknown |
| R-002 | Public PC/Windows D3D12 and its public API surface (A4, A5) | Any Xbox-specific D3D12.X command interface, format or PC equivalence | unknown |
| R-003 | Public D3D12 mesh/amplification semantics (A5); public DXIL format (A6) | Xbox guest shader binary format, lawful corpus result, translation correctness | unknown |
| R-004 | Published 16 GB GDDR6 configuration and split bandwidth (A1) | Page tables, guest address-space layout, GPU-visible allocation semantics | unknown |
| R-005 | Published Xbox I/decompression hardware figures (A1); public Windows DirectStorage (A7, A8) | Xbox-specific API surface and validated host equivalents | unknown |
| R-006 | Published user-facing features (A1); general x64 syscall register note (A3) | Xbox-specific system-service API surface or minimum test-service set | unknown |

Translate counts, shader-stage enumerations and API lists are recorded only as what the cited documents say. They are **not** used to claim emulation compatibility, implementation progress, frame-rate behavior or performance.

## 4. Re-verification records (23 required fields, exact order of `RESEARCH_REGISTER.md:46-72`)

### 4.1 R-001 — Zen 2 / CPU-visible behavior

- ID: R-001
- Title: Publicly documented Xbox Series CPU-visible behavior
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored Zen 2 / CPU-visible claim; no implementation, ADR, interface or product change authorised.
- Research question: Which Xbox Series X CPU-visible behaviors are explicitly documented in authoritative public sources, and which project-authored claims remain unverified?
- Scope: Xbox Series X only; public official Microsoft/AMD pages and public Microsoft specification repositories; access date 2026-11-24; general x86-64/AMD64 facts are labelled as general and are not presented as Xbox-only behavior.
- Capability status: unknown
- Workflow/lifecycle status: re-verified against authoritative public sources by XB-0083-AUTHORITATIVE-EVIDENCE; unverified CPU-visible items remain unknown
- Evidence label(s): documented (project-authored); documented (authoritative, published hardware specs and general Windows x64 ABI only); observed; unknown
- Source class: project-authored; authoritative (published hardware specs and general Windows x64 ABI only); N/A (non-documented sub-claims)
- Source: P1; P2; A1 `https://www.xbox.com/en-US/consoles/xbox-series-x`; A2 `https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention`; A3 `https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions`; A9. All URLs accessed 2026-11-24.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external project statements assert Zen 2 / AVX2 / Windows x64 ABI behavior; not authoritative Xbox facts. F2 (documented, authoritative for published hardware specs only): A1 publishes the custom Zen 2 8-core CPU. F3 (documented, authoritative for general Windows x64 only): A2/A3 document the Windows x64 register/stack/shadow-space/PE32+ conventions. F4 (unknown): no authoritative public source examined establishes guest CPUID, AVX2 on the custom part or the guest ABI. F5 (observed): quoted content was returned by the exact URLs on 2026-11-24.
- Finding-to-source mapping: F1 → P1, P2; F2 → A1; F3 → A2, A3; F4 → none recorded — see Uncertainties, conclusion `unknown`; F5 → A9.
- Reproduction: fetch A1, A2, A3 and search the quoted phrases; inventory in Section 2. No build, CTest or guest execution; no capability claim.
- Uncertainties: unknown — guest CPUID/AVX2; unknown — guest ABI; observed — AMD press-release fetches were unusable; observed — authoritative scope of F2/F3 is limited exactly as stated.
- Open questions: Can authoritative public evidence establish guest-visible CPUID/AVX2 or a guest calling convention without proprietary material?
- Result/conclusion: custom Zen 2 hardware and general Windows x64 ABI documented; project-authored claim not promoted; remaining CPU-visible capability claims `unknown`. No compatibility/performance claim.
- Architecture/implementation impact: none — no ADR, interface, CPU, decoder, memory or test change.
- Dependencies: A9 inventory; P1/P2 claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created 2026-11-24 (this package); register row R-001 changed 2026-11-24 by this package.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### 4.2 R-002 — Xbox-specific graphics command interfaces / D3D12.x

- ID: R-002
- Title: Xbox-specific graphics command interfaces
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored D3D12.X claim; no implementation, ADR, interface or product change authorised.
- Research question: Which Xbox-specific graphics command interfaces are documented in authoritative public sources, and which project-authored claims remain unverified?
- Scope: Xbox Series X only; public official Microsoft pages and specifications; access date 2026-11-24; no D3D12.X equivalence assumed; no GDK/NDA or other-console material.
- Capability status: unknown
- Workflow/lifecycle status: re-verified against authoritative public sources by XB-0083-AUTHORITATIVE-EVIDENCE; Xbox-specific interface items remain unknown
- Evidence label(s): documented (project-authored); documented (authoritative, public PC/Windows D3D12 only); observed; unknown
- Source class: project-authored; authoritative (public PC/Windows D3D12 only); N/A (non-documented sub-claims)
- Source: P1; P2; A4 `https://learn.microsoft.com/en-us/windows/win32/direct3d12/direct3d-12-graphics`; A5 `https://raw.githubusercontent.com/microsoft/DirectX-Specs/master/d3d/MeshShader.md`; A9. All URLs accessed 2026-11-24.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external D3D12.X-equivalent statements; not authoritative Xbox facts. F2 (documented, authoritative for public PC/Windows D3D12 only): A4 documents the public D3D12 guide and SDK headers; A5 documents public D3D12 API calls and PSO rules. F3 (unknown): no authoritative public source examined documents an Xbox-specific command interface, its command format or PC equivalence. F4 (observed): quoted content returned by exact URLs on 2026-11-24.
- Finding-to-source mapping: F1 → P1, P2; F2 → A4, A5; F3 → none recorded — see Uncertainties, conclusion `unknown`; F4 → A9.
- Reproduction: fetch A4, A5 and search quoted phrases; inventory in Section 2. No GDK access, build, CTest or guest execution; no capability claim.
- Uncertainties: unknown — any Xbox-specific graphics command interface; unknown — D3D12.X semantics or equivalence; observed — one direct3d12/directstorage URL returned 404 and was replaced by A7/A8; observed — public D3D12 is not Xbox console evidence.
- Open questions: Can authoritative public evidence establish Xbox-specific graphics command behavior without GDK/NDA material?
- Result/conclusion: public PC/Windows D3D12 documented; D3D12.X claim not promoted; Xbox-specific graphics capability claims `unknown`. No equivalence, backend or compatibility claim.
- Architecture/implementation impact: none — no ADR, backend, interface or test change.
- Dependencies: A9 inventory; P1/P2 claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created 2026-11-24 (this package); register row R-002 changed 2026-11-24 by this package.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### 4.3 R-003 — Shader/mesh input representation

- ID: R-003
- Title: Shader input representation and semantics
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored shader/mesh claim; no implementation, ADR, interface or product change authorised.
- Research question: Which shader input representations and semantics are documented in authoritative public sources, and which project-authored shader/mesh claims remain unverified?
- Scope: Xbox Series X only; lawful public Microsoft specification repositories; access date 2026-11-24; no game assets, dumps, extracted shaders, proprietary formats or other-console evidence.
- Capability status: unknown
- Workflow/lifecycle status: re-verified against authoritative public sources by XB-0083-AUTHORITATIVE-EVIDENCE; Xbox-specific shader format and translation items remain unknown
- Evidence label(s): documented (project-authored); documented (authoritative, public D3D12 mesh-shader and DXIL specifications only); observed; unknown
- Source class: project-authored; authoritative (public D3D12 mesh-shader and DXIL specifications only); N/A (non-documented sub-claims)
- Source: P1; P2; A5 `https://raw.githubusercontent.com/microsoft/DirectX-Specs/master/d3d/MeshShader.md`; A6 `https://raw.githubusercontent.com/microsoft/DirectXShaderCompiler/main/docs/DXIL.rst`; A9. All URLs accessed 2026-11-24.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external shader/mesh format statements; not authoritative Xbox facts. F2 (documented, authoritative for public D3D12 mesh spec only): A5 states D3D12-only mesh/amplification shaders, IA disabled with thread IDs and optional group-uniform payload, `[numthreads]` X*Y*Z <= 128, DXIL bytecode required in the PSO, and semantic-name linkage. F3 (documented, authoritative for public DXIL spec only): A6 defines DXIL as LLVM 3.7 IR subset with ms/as at 6.5 and MSIn/MSOut/MSPOut/ASIn; its "offline XBOX shader compiler" mention specifies no Xbox binary format. F4 (unknown): no authoritative public source examined specifies an Xbox guest shader format or translation correctness; no corpus built. F5 (observed): quoted content returned by exact URLs on 2026-11-24.
- Finding-to-source mapping: F1 → P1, P2; F2 → A5; F3 → A6; F4 → none recorded — see Uncertainties, conclusion `unknown`; F5 → A9.
- Reproduction: fetch A5, A6 and search quoted phrases; inventory in Section 2. No build, CTest, shader compilation, corpus, game run or guest execution; no capability claim.
- Uncertainties: unknown — Xbox guest shader binary format; unknown — lawful corpus result and translation correctness; observed — the A6 XBOX mention names a consumer but no layout; observed — public D3D12/DXIL is not Xbox console shader evidence.
- Open questions: Can authoritative public evidence establish any Xbox Series X guest shader format without protected assets or GDK material?
- Result/conclusion: public mesh/DXIL representations documented; project-authored claim not promoted; Xbox-specific format and translation claims `unknown`. No translator/backend/compatibility claim.
- Architecture/implementation impact: none — no ADR, translator, backend, interface or test change.
- Dependencies: A9 inventory; P1/P2 claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created 2026-11-24 (this package); register row R-003 changed 2026-11-24 by this package.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### 4.4 R-004 — Guest virtual-memory behavior

- ID: R-004
- Title: Guest virtual-memory behavior
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored guest-memory claim; no implementation, ADR, interface or product change authorised.
- Research question: Which observable guest virtual-memory requirements are documented in authoritative public sources, and which project-authored claims remain unverified?
- Scope: Xbox Series X only; public official Microsoft hardware specifications; access date 2026-11-24; `GuestMemory` stays generic and is not an Xbox virtual-memory model.
- Capability status: unknown
- Workflow/lifecycle status: re-verified against authoritative public sources by XB-0083-AUTHORITATIVE-EVIDENCE; guest virtual-memory items remain unknown
- Evidence label(s): documented (project-authored); documented (authoritative, published hardware memory configuration only); observed; unknown
- Source class: project-authored; authoritative (published hardware memory configuration only); N/A (non-documented sub-claims)
- Source: P1; P2; A1 `https://www.xbox.com/en-US/consoles/xbox-series-x`; A9. All URLs accessed 2026-11-24.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external guest/unified-memory statements; not authoritative Xbox facts. F2 (documented, authoritative for published hardware configuration only): A1 publishes 16 GB GDDR6 on a 320-bit bus with 10 GB @ 560 GB/s and 6 GB @ 336 GB/s. F3 (unknown): no authoritative public source examined documents page tables, guest layout or GPU-visible allocation semantics. F4 (observed): quoted content returned by A1 on 2026-11-24.
- Finding-to-source mapping: F1 → P1, P2; F2 → A1; F3 → none recorded — see Uncertainties, conclusion `unknown`; F4 → A9.
- Reproduction: fetch A1 and search quoted phrases; inventory in Section 2. No build, CTest, memory test, game run or guest execution; no capability claim.
- Uncertainties: unknown — page tables, address space, GPU-visible allocation; unknown — whether public sources expose observable requirements; observed — a published split is hardware configuration, not a guest VM model.
- Open questions: What observable requirements, if any, can be specified without adding Xbox-specific semantics to `GuestMemory`?
- Result/conclusion: hardware memory configuration documented; project-authored claim not promoted; guest virtual-memory capability claims `unknown`. No unified-memory or compatibility claim.
- Architecture/implementation impact: none — generic `GuestMemory` unchanged; no Xbox VM model introduced.
- Dependencies: A9 inventory; P1/P2 claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created 2026-11-24 (this package); register row R-004 changed 2026-11-24 by this package.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### 4.5 R-005 — Storage/decompression APIs

- ID: R-005
- Title: Storage/decompression APIs
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored storage/decompression claim; no implementation, ADR, interface or product change authorised.
- Research question: Which storage/decompression facts and APIs are documented in authoritative public sources, and which project-authored mappings remain unverified?
- Scope: Xbox Series X only; public official Microsoft hardware specifications and public Windows DirectStorage documentation; access date 2026-11-24; public Windows DirectStorage is reported as PC/Windows, not as an Xbox console API.
- Capability status: unknown
- Workflow/lifecycle status: partial — mapping started, not complete; authoritative re-verification added by XB-0083-AUTHORITATIVE-EVIDENCE (`partial` is a workflow note only, never a capability token)
- Evidence label(s): documented (project-authored); documented (authoritative, published Xbox hardware I/O specs and public Windows DirectStorage only); observed; unknown
- Source class: project-authored; authoritative (published Xbox hardware I/O specs and public Windows DirectStorage only); N/A (non-documented sub-claims)
- Source: P1; P2; A1 `https://www.xbox.com/en-US/consoles/xbox-series-x`; A7 `https://learn.microsoft.com/en-us/windows/win32/dstorage/dstorage-api-reference`; A8 `https://devblogs.microsoft.com/directx/directstorage-api-downloads/`; A9. All URLs accessed 2026-11-24.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external storage/API mapping statements; not authoritative Xbox facts. F2 (documented, authoritative for published hardware I/O only): A1 publishes custom NVMe SSD, 2.4 GB/s raw and 4.8 GB/s compressed with a custom hardware decompression block, Velocity Architecture and Quick Resume. F3 (documented, authoritative for public Windows DirectStorage only): A7 references APIs in `dstorage.h`; A8 lists Windows SDK releases and GDeflate/Zstd support. F4 (unknown): no authoritative public source examined documents an Xbox-specific API surface or validated host equivalent; mapping incomplete. F5 (observed): quoted content returned by exact URLs on 2026-11-24.
- Finding-to-source mapping: F1 → P1, P2; F2 → A1; F3 → A7, A8; F4 → none recorded — see Uncertainties, conclusion `unknown`; F5 → A9.
- Reproduction: fetch A1, A7, A8 and search quoted phrases; inventory and unusable-URL notes in Section 2. No build, CTest, benchmark, game run or guest execution; no capability claim.
- Uncertainties: unknown — Xbox-specific API surface and lawful host equivalents; unknown — whether public DirectStorage corresponds to the console decompression block; observed — one direct3d12/directstorage URL returned 404; observed — public Windows DirectStorage is not Xbox console API evidence.
- Open questions: Which documented Xbox-specific APIs, if any, have lawful host equivalents, and which parts of the mapping remain unmapped?
- Result/conclusion: hardware I/O specs and public DirectStorage documented; project-authored mapping not promoted; mapping stays incomplete and `unknown`. No API capability, throughput-equivalence or compatibility claim.
- Architecture/implementation impact: none — no ADR, storage/decompression implementation, interface or test change.
- Dependencies: A9 inventory; P1/P2 claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created 2026-11-24 (this package); register row R-005 changed 2026-11-24 by this package with legacy `partial` retained as workflow note and capability status kept `unknown`.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

### 4.6 R-006 — System services required by test software

- ID: R-006
- Title: System services required by test software
- Package/Goal: XB-0083-AUTHORITATIVE-EVIDENCE — development-only re-verification of the project-authored system-services claim; no implementation, ADR, interface or product change authorised.
- Research question: Which system services are documented in authoritative public sources, and which project-authored system-service claims remain unverified?
- Scope: Xbox Series X only; public official Microsoft pages; access date 2026-11-24; no GDK/NDA, proprietary, private or other-console material; no native-platform dependency assumed for XBridge.
- Capability status: unknown
- Workflow/lifecycle status: re-verified against authoritative public sources by XB-0083-AUTHORITATIVE-EVIDENCE; Xbox-specific system-service items remain unknown
- Evidence label(s): documented (project-authored); documented (authoritative, published Xbox user-facing features and general Windows x64 syscall convention only); observed; unknown
- Source class: project-authored; authoritative (published Xbox user-facing features and general Windows x64 syscall convention only); N/A (non-documented sub-claims)
- Source: P1; P2; A1 `https://www.xbox.com/en-US/consoles/xbox-series-x`; A3 `https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions`; A9. All URLs accessed 2026-11-24.
- Date checked: 2026-11-24
- Concrete finding(s): F1 (documented, project-authored): external Xbox-function/service statements; not authoritative Xbox facts. F2 (documented, authoritative for published user-facing features only): A1 describes Quick Resume, faster load times, 3D Spatial Sound, Storage Expansion Cards and backward compatibility. F3 (documented, authoritative for general Windows x64 only): A3 notes R10:R11 in syscall/sysret and direction-flag expectations on Windows system calls. F4 (unknown): no authoritative public source examined documents an Xbox-specific service API surface or minimum test-service set. F5 (observed): quoted content returned by exact URLs on 2026-11-24.
- Finding-to-source mapping: F1 → P1, P2; F2 → A1; F3 → A3; F4 → none recorded — see Uncertainties, conclusion `unknown`; F5 → A9.
- Reproduction: fetch A1, A3 and search quoted phrases; inventory in Section 2. No GDK access, build, CTest, game run or guest execution; no capability claim.
- Uncertainties: unknown — Xbox-specific services and minimum test-service set; observed — published user-facing features are not an API surface; observed — general Windows x64 evidence is not Xbox service evidence.
- Open questions: Can any authoritative public source define a minimum Xbox-specific system-service set without GDK/NDA material?
- Result/conclusion: user-facing features and general x64 syscall note documented; project-authored claim not promoted; Xbox-specific service capability claims `unknown`. No service/host-API/compatibility claim.
- Architecture/implementation impact: none — no ADR, host service, interface or test change; no native-platform dependency introduced.
- Dependencies: A9 inventory; P1/P2 claim lineage.
- Responsible agent: researcher (XB-0083-AUTHORITATIVE-EVIDENCE); independent reviewer verifies conformance.
- Created/Changed: Created 2026-11-24 (this package); register row R-006 changed 2026-11-24 by this package.
- Conformance status: conformant
- Missing fields if non-conformant: none — all 23 fields present and non-empty or explicit N/A with reason.

## 5. Conformance status per Section 6, with concrete file:line evidence

The researcher records `conformant` for all six rows below; this is **not** a reviewer verdict. The independent reviewer verifies these entries later. Line references are to the current `docs/RESEARCH_REGISTER.md` after this package's edits.

| Row | Recorded conformance status | File:line evidence for the eight Section 6 criteria |
|---|---|---|
| R-001 | `conformant` | `docs/RESEARCH_REGISTER.md:124` unique ID; `:124-146` all 23 fields; `:129` capability `unknown`, `:145` conformance `conformant`, `:131` labels, `:132` source class (allowed tokens only); `:131-132` every `documented` sub-claim has a qualifier; `:136` finding-to-source mapping with explicit no-source reason for F4; `:138` labeled uncertainties; `:128` scope declared; `:133` concrete URLs with access date at `:134` and reproduction at `:137` satisfy Section 4 for `unknown`. |
| R-002 | `conformant` | `docs/RESEARCH_REGISTER.md:150` unique ID; `:150-172` all 23 fields; `:155` capability `unknown`, `:171` conformance `conformant`, `:157` labels, `:158` source class; `:157-158` documented qualifiers present; `:162` mapping incl. explicit no-source reason for F3; `:164` labeled uncertainties; `:154` scope; `:159` URLs, `:160` date, `:163` reproduction. |
| R-003 | `conformant` | `docs/RESEARCH_REGISTER.md:176` unique ID; `:176-198` all 23 fields; `:181` capability `unknown`, `:197` conformance `conformant`, `:183` labels, `:184` source class; `:183-184` documented qualifiers present; `:188` mapping incl. explicit no-source reason for F4; `:190` labeled uncertainties; `:180` scope; `:185` URLs, `:186` date, `:189` reproduction. |
| R-004 | `conformant` | `docs/RESEARCH_REGISTER.md:202` unique ID; `:202-224` all 23 fields; `:207` capability `unknown`, `:223` conformance `conformant`, `:209` labels, `:210` source class; `:209-210` documented qualifiers present; `:214` mapping incl. explicit no-source reason for F3; `:216` labeled uncertainties; `:206` scope; `:211` URL, `:212` date, `:215` reproduction. |
| R-005 | `conformant` | `docs/RESEARCH_REGISTER.md:228` unique ID; `:228-250` all 23 fields; `:233` capability `unknown`, `:249` conformance `conformant`, `:235` labels, `:236` source class; `:235-236` documented qualifiers present; `:240` mapping incl. explicit no-source reason for F4; `:242` labeled uncertainties; `:232` scope; `:237` URLs, `:238` date, `:241` reproduction. `partial` at `:234` is a workflow note only. |
| R-006 | `conformant` | `docs/RESEARCH_REGISTER.md:254` unique ID; `:254-276` all 23 fields; `:259` capability `unknown`, `:275` conformance `conformant`, `:261` labels, `:262` source class; `:261-262` documented qualifiers present; `:266` mapping incl. explicit no-source reason for F4; `:268` labeled uncertainties; `:258` scope; `:263` URLs, `:264` date, `:267` reproduction. |

No row is `non-conformant`, `not_assessed` or `not_applicable`, so `Missing fields if non-conformant` is an explicit `none — …` line in every block (`docs/RESEARCH_REGISTER.md:146,172,198,224,250,276`). No capability status was promoted: the all-`unknown` distribution and eight-`conformant` distribution are recorded in the register audit notes.

## 6. Deliberate boundaries and unresolved uncertainty

- **No code adoption, ADR, public-interface, test, build or configuration change.** No product source was read for implementation purposes and no implementation task was approved.
- **Other-console evidence is not Xbox evidence.** No PlayStation/Nintendo material was used; general x86-64/AMD64 and public PC/Windows D3D12/DirectStorage facts are explicitly labeled as general or PC/Windows.
- **Project-authored claims stay project-authored.** P1/P2 claims are not promoted to authoritative Xbox facts; an authoritative statement about general hardware, a public API or a public format does not validate an emulator capability.
- **No compatibility or performance claim** is derived from shader-model numbers, translation counts, API lists or published console throughput figures. A1 figures are quoted as published hardware specifications only.
- `unknown` remains: guest CPUID/AVX2 and guest ABI; Xbox D3D12.X command semantics and equivalence; Xbox guest shader binary format and translation correctness; guest virtual-memory semantics; Xbox-specific storage API surface and host mapping; Xbox-specific system services.
- Fresh external truth after 2026-11-24, GDK/NDA material, license/legal effect of external projects and any private or proprietary source remain `unknown` and were not investigated.
- `websearch` was unavailable (HTTP 402); only direct fetches of the exact URLs in Section 2 were used. Unusable URLs are listed verbatim so a reviewer or the coordinator can repeat the attempt.

## 7. Reproduction path for the coordinator/reviewer

1. Read `docs/RESEARCH_REGISTER.md:107-118` (compact index) and `:120-276` (R-001…R-006 detail blocks) and check field order against `docs/RESEARCH_REGISTER.md:46-72`.
2. Read this file's Section 2 and fetch the exact A1–A8 URLs; confirm the quoted strings appear and note that each URL was accessed on 2026-11-24.
3. Cross-check Section 5's file:line references against the register after any later edit; line numbers are valid for the state produced by this package.
4. Verify no forbidden file was changed by this package: the only intended write set is this file and `docs/RESEARCH_REGISTER.md`.
5. The coordinator runs `git diff --check`; the independent reviewer records the conformance verdict. This researcher did not run shell, Git, CMake or CTest commands.
