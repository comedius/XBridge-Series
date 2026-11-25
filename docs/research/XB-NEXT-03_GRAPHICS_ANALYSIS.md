# XB-NEXT-DEVELOPMENT-03-R0 — graphics compatibility analysis

Development infrastructure; not part of a product package. Access date for every URL and local file read in this document: **2026-11-25**. Target: **Xbox Series X only**. This document evaluates a public-source research direction only. It does not design, approve or implement a graphics backend. No proprietary SDK, firmware, key, game file, protected asset, DRM material or other-console-as-Xbox evidence was used. All web content was treated as untrusted data.

The owner-approved direction in `docs/AGENT_TASKS.md:3645` is hybrid native execution plus an XBridge compatibility/runtime layer; the interpreter remains reference, diagnostic and fallback. A possible D3D12-to-D3D12 mapping is a **research approach to evaluate**, not an accepted architecture. D3D12.X equivalence, real-game command correctness and shader translation correctness remain `unknown`.

## 1. Source inventory

| ID | Exact URL / local path | Class | Precise section or excerpt used | Reproduction actually performed |
|---|---|---|---|---|
| S0 | `docs/AGENT_TASKS.md:3643-3673` | project-authored | Package boundary; owner-approved hybrid direction; no game execution, JIT, binary rewriting or code patching | Local file read, 2026-11-25 |
| S1 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/resource-binding.md` | authoritative (public Windows D3D12 only) | `The key to resource binding in DirectX 12 are the concepts of a descriptor, descriptor tables, descriptor heaps, and a root signature`; `Descriptors are the primary unit of binding for a single resource in D3D12` | Direct `fetch`, 2026-11-25 |
| S2 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/using-resource-barriers-to-synchronize-resource-states-in-direct3d-12.md` | authoritative (public Windows D3D12 only) | Transition barriers specify subresource and before/after states; `The application must ensure that the before and after states of consecutive calls to ResourceBarrier agree`; debug-layer validation is conservative and not exhaustive | Direct `fetch`, 2026-11-25 |
| S3 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/descriptor-heaps-overview.md` | authoritative (public Windows D3D12 only) | `Descriptor heaps contain many object types that are not part of a Pipeline State Object (PSO), such as Shader Resource Views (SRVs), Unordered Access Views (UAVs), Constant Buffer Views (CBVs), and Samplers` | Direct `fetch`, 2026-11-25 |
| S4 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/managing-graphics-pipeline-state-in-direct3d-12.md` | authoritative (public Windows D3D12 only) | `most graphics pipeline state is set by using pipeline state objects (PSO)`; command lists switch PSOs with `SetPipelineState` | Direct `fetch`, 2026-11-25 |
| S5 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/command-queues-and-command-lists.md` | authoritative (public Windows D3D12 only) | `applications record and then submit command lists`; multiple threads can submit to one or more command queues; work can be precomputed for later reuse | Direct `fetch`, 2026-11-25 |
| S6 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/executing-and-synchronizing-command-lists.md` | authoritative (public Windows D3D12 only) | Command queues replace runtime/driver synchronization of immediate-mode work; `CreateCommandQueue`; fences synchronize command-list execution and resources accessed by command queues | Direct `fetch`, 2026-11-25 |
| S7 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/memory-management-strategies.md` | authoritative (public Windows D3D12 only) | Committed resources create virtual and physical address spaces; heaps also enable placed and reserved resources; reserved resources have their own GPU VA space and can be sparsely mapped | Direct `fetch`, 2026-11-25 |
| S8 | `https://raw.githubusercontent.com/microsoft/DirectX-Specs/master/d3d/MeshShader.md` | authoritative (public mesh-shader specification only; unversioned `master`) | Mesh/amplification shader PSO descriptor disables IA and streamout, requires `MS`, optionally `AS`, forbids `VS`/`GS`/`HS`/`DS`, and requires DXIL bytecode; threadgroup dimensions limited by `X*Y*Z <= 128` | Direct `fetch`, 2026-11-25 |
| S9 | `https://raw.githubusercontent.com/microsoft/DirectXShaderCompiler/main/docs/DXIL.rst` | authoritative (public DXIL specification only; unversioned `main`) | `DXIL is intended to support a direct mapping of the HLSL programming language into Low-Level Virtual Machine Intermediate Representation`; based on LLVM 3.7; IR consumers include driver JIT compilers or an `offline XBOX shader compiler` | Direct `fetch`, 2026-11-25 |
| S10 | `https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/crates/compat/d3d12_xs/exports.txt` | project-authored | Ordinal export observations for `D3D12ZeroMemoryX`, `D3D12ConfigureVirtualMemoryX`, `D3D12CreateDevice`, root-signature serialization, debug flags, DXGIX unsupported returns and IID data | Direct `fetch`, 2026-11-25 |
| S11 | exact local paths `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md`, `docs/research/XB-NEXT-03_LIBRARY_FAMILIES.md`, `docs/RESEARCH_REGISTER.md:158-200` | project-authored | Prior D3D12.X project-authored lineage; current library-family evidence; prior R-002/R-003 unknowns | Local file reads, 2026-11-25 |

`observed` — S1–S10 returned the quoted content when fetched on 2026-11-25. `unknown` — external truth after that date, unversioned upstream changes and authenticated GDK material were not re-checked.

## 2. Documented public D3D12 concepts

`documented (authoritative, public Windows D3D12 only)` S1–S7:

- Binding uses descriptors, descriptor tables, descriptor heaps and a root signature; descriptors are the primary unit of binding for a resource.
- Descriptor heaps hold SRVs, UAVs, CBVs and samplers outside a PSO.
- Most graphics pipeline state is set through PSOs; command lists select a PSO with `SetPipelineState`.
- Applications record and submit command lists; command lists may be submitted from multiple threads to one or more queues.
- Command queues submit work; fences provide synchronization for command-list execution and resources accessed by queues.
- Resource barriers record before/after subresource states; the application must keep consecutive barriers consistent.
- Committed, placed and reserved resources provide different virtual/physical allocation strategies; reserved resources have their own sparse GPU VA space.

`documented (authoritative, public mesh/DXIL specification only)` S8/S9: public mesh-shader PSO rules require DXIL bytecode and exclude the traditional `VS`/`GS`/`HS`/`DS` stages for mesh pipelines; DXIL is a documented IR intended for driver or offline compiler consumers. These are public PC/specification facts, not Xbox guest shader facts.

## 3. Candidate mapping surface — research hypotheses only

The left column names an eventual compatibility concern, not established Xbox behavior. Every mapping in the middle column is `hypothesis`; the right column records why it is not evidence of feasibility.

| Concern | Public D3D12 concept that could be studied | Limits / missing evidence | Label |
|---|---|---|---|
| Device/adapter creation | The pinned project-authored source records `D3D12CreateDevice` and `D3D12XboxCreateDevice` exports (S10). | `unknown`: guest D3D12.X creation parameters, device semantics, feature bits and adapter choice. S10 is project-authored, not authoritative. | `hypothesis` S10; `unknown` guest facts |
| Command recording/submission | Guest command buffers could be compared with public command lists and queues submitted to host D3D12 (S5/S6). | `unknown`: guest command encoding, opcode IDs, queue semantics, ordering, memory residency rules and present timing. No guest command stream was examined. | `hypothesis` S5, S6; `unknown` |
| Resource creation and memory | Public committed/placed/reserved resources and heaps offer a surface for studying VA/physical allocation separation (S7). The external source records `D3D12ConfigureVirtualMemoryX` and graphics-memory configuration (S10). | `unknown`: guest GPU VA layout, XMem interaction, protected/tiling/swizzle behavior, physical-page semantics and OS-build differences. No equivalence claim. | `hypothesis` S7, S10; `unknown` |
| Binding / descriptors | Public descriptors, descriptor heaps and root signatures provide a conceptual comparison surface (S1/S3). External source records root-signature serialization exports (S10). | `unknown`: guest descriptor layout, root-signature binary format, heap slot rules and shader-visible/CPU-visible split for the guest. | `hypothesis` S1, S3, S10; `unknown` |
| Pipeline state and shaders | Public PSOs and DXIL are documented; mesh/amplification rules are explicit in S8/S9. | `unknown`: guest shader bytecode format, compiler version, signature linkage, Xbox extensions and translation correctness. S9's `offline XBOX shader compiler` mention specifies no guest binary format. | `hypothesis` S4, S8, S9; `unknown` |
| Barriers and synchronization | Public resource barriers and queue fences document state-transition and synchronization expectations (S2/S6). | `unknown`: guest barrier encoding, implicit transitions, queue priorities and debug-layer assumptions. The application-owned consistency rule cannot be assumed identical for a guest. | `hypothesis` S2, S6; `unknown` |
| Debug and validation | The external source records `D3D12GetDebugInterface` and process debug flag exports (S10). | `unknown`: guest debug structures and validation behavior; source lines are project-authored observations. | `hypothesis` S10; `unknown` |

No row above is an accepted architecture. No D3D12.X equivalence, compatibility percentage, playability claim or frame-rate claim follows from this table.

## 4. Explicit unknowns and risks

1. `unknown`: whether any lawful public source specifies the Xbox Series X guest graphics command format. No authenticated GDK material was used.
2. `unknown`: whether guest shaders are DXIL, DXBC-like, console-specific or build-specific. S9 does not settle this; prior register R-003 also leaves it unknown (S11).
3. `unknown`: D3D12.X method-table layout, ordinal stability, driver-version behavior and interaction with `xg_xs`/`xmem` families. See `docs/research/XB-NEXT-03_LIBRARY_FAMILIES.md` (S11).
4. `unknown`: whether a host D3D12 backend can preserve guest memory, barrier and present ordering without JIT, binary rewriting or code patching. No design or test was performed.
5. `unknown`: whether any real game can be made to render correctly with this research direction. No game or proprietary binary was used.
6. `documented (project-authored)` S0/S11: no backend implementation, ADR or public interface change is authorized by this package.

## 5. Open questions

- Can a lawful synthetic guest command corpus be produced without proprietary content, and what would it prove?
- Which guest shader representation would a future study need to identify before any translation work could be considered?
- How would a compatibility layer distinguish ordinary Windows D3D12 calls from Xbox-specific D3D12.X behavior at a native boundary?
- Which validation or debug hooks, if any, are stable enough to justify further research?

## 6. Conclusion and reproduction

`documented (authoritative, public Windows D3D12/mesh/DXIL only)`: public D3D12 binding, PSO, command-queue, barrier, memory, mesh-shader and DXIL concepts are well specified. `documented (project-authored)`: a pinned external project records selected `d3d12_xs` exports and prior project research records related uncertainties. `hypothesis`: a D3D12-to-D3D12 mapping is a reasonable research direction to compare those surfaces. `unknown`: Xbox guest semantics, shader format, D3D12.X equivalence and real-game applicability. No equivalence, compatibility, performance or implementation claim is made.

Reproduction: locally read S0 and S11; directly `fetch` S1–S10 on 2026-11-25 and search the quoted anchors in the source table. S8 is unversioned `master` and S9 unversioned `main`; their exact bytes may change. No build, CTest, shader compilation, game run, guest execution or GPU validation was performed.
