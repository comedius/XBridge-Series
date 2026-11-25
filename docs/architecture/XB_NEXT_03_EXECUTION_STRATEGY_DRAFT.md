# XB-NEXT-DEVELOPMENT-03 — execution strategy draft

This file is **development infrastructure**. It is an implementation-ready design draft, **not an accepted ADR** and not part of a product package. `docs/DECISIONS.md` is unchanged and still ends at ADR-0011; ADR-0012 remains open (`docs/architecture/CPU_ADDRESS_SPACE_INTEGRATION.md:7-8`). No owner decision is requested or made by this draft beyond the direction the owner already approved.

Evidence labels are `documented`, `observed`, `hypothesis` and `unknown` as defined in `docs/RESEARCH_REGISTER.md`. General Windows x64, PE/COFF and Win32 facts are never promoted to Xbox Series X guest facts.

## 1. Owner-approved direction and limits

- `documented` — The owner approved a hybrid direction: compatible Xbox x64 guest instructions should ultimately execute natively on host CPUs; Xbox-specific operating-system functionality, services and runtime behavior must be provided by an XBridge compatibility/runtime layer; the existing x64 interpreter remains available as reference, diagnostic tool and fallback. This is recorded in `docs/AGENT_TASKS.md:3645`.
- `documented` — The same approval forbids JIT, binary rewriting, code patching, arbitrary guest-byte execution, real game execution, proprietary material and unsupported compatibility/performance claims.
- `unknown` — Whether any commercial Xbox Series X binary is executable under this direction. No real game, key, firmware or protected asset was obtained, opened or executed.
- `documented` — The interpreter-first baseline remains intact (`docs/ARCHITECTURE.md:141-183`); no existing interpreter signature, status or test is changed by this package.

## 2. Decision matrix

The three models are compared below. "Hybrid" is the owner-approved long-term direction, not an implemented capability.

| Axis | Native-only model | Interpreter-only model | Owner-approved hybrid direction |
|---|---|---|---|
| Instruction coverage (AVX2/BMI/FMA) | `documented` (general only): host CPUID/XGETBV discovery and compiler intrinsics exist (`XB-NEXT-03_NATIVE_EXECUTION_FEASIBILITY.md:32-35`). `unknown`: guest requirements. | `documented`: current interpreter supports NOP and limited MOV forms, no SIMD/FPU (`docs/ARCHITECTURE.md:141-183`). | `documented`: native path is intended for compatible instructions; interpreter remains fallback. `unknown`: which real guest instruction subsets are required. |
| ABI / calling convention | `documented` (general Windows x64 only): four-register fast-call, shadow store, 16-byte stack alignment. `unknown`: guest ABI. | `documented`: no guest ABI layer in the current interpreter. | `hypothesis`: ABI matching at explicit transitions may be viable; guest ABI remains `unknown`. |
| Guest memory and permissions | `documented` (general Win32 only): page protection exists. `unknown`: guest layout and protection model. | `documented`: generic bounded guest memory with no Xbox VM claim. | `hypothesis`: guest addresses must be translated through a controlled seam; `GuestAddressSpace` stays generic. |
| Host/guest transitions | `hypothesis`: entry, service call, callback and fault transitions are required. | `documented`: no transition layer; instructions execute through a decoded state. | `documented`: transitions must be explicit and testable; no undocumented Xbox behavior is assumed. |
| Exceptions / faults / privileged instructions | `documented` (general Windows x64 only): table-based unwind data. `unknown`: guest exception and privileged surface. | `documented`: unknown opcodes are no-ops in the current scaffold; this is a limitation, not guest correctness. | `hypothesis`: explicit fault reporting is required before any native routine is selected; privileged instructions need interception in a later decision. |
| TLS / threading | `documented` (general only): PE TLS directory and Win32 TLS/synchronization APIs. `unknown`: guest semantics. | `documented`: no thread scheduler in the current interpreter scope. | `hypothesis`: neutral service interfaces can host future TLS/thread translation; no implementation is claimed. |
| Xbox services | `unknown`: native-only has no service provider. | `documented`: no service layer. | `documented`: compatibility layer is the intended provider; service behavior remains `unknown`. |
| Controllability and debugging | `hypothesis`: harder to diagnose unsupported guest semantics once native code runs. | `documented`: safest diagnostic baseline. | `documented`: interpreter retained for parity/diagnosis; native path is feature-gated and synthetic-only in N1. |
| Performance potential | `hypothesis`: potentially lower overhead for compatible code; no measured data. | `hypothesis`: higher interpretive overhead; no game data. | `hypothesis`: potential benefit unproven; no synthetic benchmark was executed in this phase. |
| Implementation effort / risk | High: isolation, ABI, faults and service interception unresolved. | `documented`: current scaffold is small and limited. | `documented`: incremental foundation is selected; arbitrary-game feasibility remains `unknown`. |

`documented` conclusion: the hybrid direction is owner-approved, but the matrix contains no evidence that real Xbox binaries can execute. `unknown` items are not converted into capabilities by translation counts, stubs, research or neutral interfaces.

## 3. N1 — native proof boundary

`documented` — The only native execution permitted in XB-03 is a fixed set of trusted, compiled-in synthetic routines selected by `NativeRoutineId`. There is no JIT, binary rewriting, code patching, caller-supplied function pointer, guest byte buffer, RVA execution or real game execution. The public API accepts only a routine ID and a prepared `GuestExecutionContext`.

`documented` — Host CPU features are detected independently (baseline x64, AVX2, BMI1, BMI2, FMA, OSXSAVE and XGETBV); AVX2 is not reported available without OSXSAVE/XGETBV state support. Tests use only an injectable `FixedCpuFeatureDetector`, never the developer machine's real CPUID result. Unsupported required features yield explicit errors before any routine runs, and backend selection/fallback is decided before execution; no mid-routine path switch occurs.

`documented` — Guest register, stack and memory state are accessed through `GuestRegisterState`, `GuestStack` and `IGuestVirtualMemory`; compiled routines never receive raw host pointers into guest backing storage. The interpreter remains independently usable. In this proof AVX2/BMI/FMA are gating metadata only: the routines do not emit those instructions, so the tests establish feature gating and transition discipline, not ISA-specific native execution.

`documented` — No restricted worker process is created. Public Windows Job Objects, restricted tokens and AppContainer have documented individual limitations: Job Object security limits must be set per process and breakaway limits can escape association; restricted tokens use privilege removal, deny-only SIDs and restricting SIDs with two access checks; AppContainer capability grants are subject to file, registry and network scope limits (`XB-NEXT-03_NATIVE_EXECUTION_FEASIBILITY.md:66-73`). `hypothesis` — The engineering conclusion derived from those limitations is that none of the three mechanisms, nor their combination, is a complete security boundary; this is an engineering conclusion, not a direct authoritative quotation or documented Microsoft statement, and matches register `XB-NEXT-03-NATIVE` F8/Uncertainties. A worker process for arbitrary guest-native code is a later owner decision.

## 4. Runtime boundary

`documented` — The runtime layer defines exactly thirteen neutral `ServiceId` tokens: System, RuntimeInitialization, GuestProcess, Thread, Synchronization, VirtualMemory, File, Storage, Input, Audio, Graphics, Timing and ErrorLogging. Each family has a pure abstract interface; no Xbox function signature, behavior or undocumented API is invented. A non-owning `SystemServiceRegistry` distinguishes `NotRegistered`, `Unsupported` and `Available`, rejects null and duplicate registrations, and never converts an unknown service into a no-op success.

`documented` — `xbridge::runtime` owns only a neutral `GuestHostTransitionDescriptor`; the concrete execution-side `xbridge::execution::GuestHostTransition` remains in N1. Neither package depends on the other. This avoids duplicate ownership while keeping both testable.

`unknown` — Required real Xbox service names, signatures, build stability, TLS/thread semantics, syscall surface and graphics service behavior. Library families `xmem`, `xg_xs`, `d3d12_xs`, `xgameruntime` and the additional public lead `dxcompiler_xs` are recorded as evidence leads only (`XB-NEXT-03_LIBRARY_FAMILIES.md:25-43`); no external GPL-licensed source is adopted.

## 5. Loader and Game Intelligence boundary

`documented` — B1 is a standalone, bounded PE32+/AMD64 loader for synthetic images. It validates headers, sections, alignment, ranges, imports/exports and relocation metadata; maps sections into the generic `GuestAddressSpace` with permissions derived from section characteristics; and records the entry point without calling it. It uses a loader-private parser, because the existing analyzer facts expose neither section characteristics nor relocation page/offset data. Analyzer sources and public interfaces are unchanged by B1.

`documented` — Only `ABSOLUTE` (0) and `DIR64` (10) are handled. All other relocation types fail explicitly before any write. Two-phase validation prevents a late unsupported type from leaving earlier writes applied. Failed virtual mappings are unmapped in reverse order; the first error is preserved. Physical backing cannot be removed through the current memory API, so failed loads are marked inaccessible and never reach `Ready`.

`documented` — F1 extends read-only Game Intelligence with stable string fields for runtime service candidates, loader requirements and unknown/unsupported structures. Candidate mappings use only the owner-listed family names and remain indicators, never capability claims. Every candidate carries an evidence label; missing section characteristics and relocation offsets are reported as `unknown`; no readiness percentage is produced. Existing exact-output assertions are extended with the new deterministic keys, not removed or weakened.

## 6. Graphics and performance boundary

`documented` — Direct D3D12-to-D3D12 mapping is a research approach to evaluate, not an accepted equivalence. Conceptual similarities in binding, barriers, command queues and pipeline state are documented for public Windows D3D12 only; D3D12.X semantics, shader formats, memory model and synchronization differences remain `unknown`. No GDKX header, proprietary SDK, shader translator or GPU emulation is added (`XB-NEXT-03_GRAPHICS_ANALYSIS.md`). Vulkan is comparative only.

`documented` — The reviewed R0 measurement concept records this synthetic candidate decomposition: guest instructions/decoded operations per second; interpreter CPU time per operation and fault counts; native/hybrid CPU time and boundary-transition counts; ABI/TLS/exception transition timing; compatibility service-call count and latency; guest-memory translation/protection fault counts; graphics command/barrier/queue/descriptor/PSO setup timing; resource movement by public D3D12 allocation strategy; and synthetic present-interval distribution (`XB-NEXT-03_PERFORMANCE_CONCEPT.md:35-45`). `hypothesis` — These are future measurement candidates, not measured results; the broader metric set named in the owner request is not attributed to R0 and remains a proposal pending a separate reviewed source. No real-game target or performance claim is set, and no benchmark was executed in this phase.

## 7. Open decisions not made here

1. `unknown` — Acceptance of a new ADR, including ADR-0012; `docs/DECISIONS.md` is outside this package's write set.
2. `unknown` — Isolation model for any future arbitrary guest-native or real game code; Job Objects, restricted tokens and AppContainer are not complete boundaries by themselves.
3. `unknown` — User-facing backend policy or CLI exposure of native execution; `src/app/main.cpp` and `ExecutionPolicy` are unchanged.
4. `unknown` — Whether physical-region removal or permission mutation is ever required for full resource rollback; not part of this package.
5. `unknown` — Which additional service mappings or guest instruction subsets are justified by lawful synthetic evidence beyond the four owner-listed library families.

## 8. Status

`documented` — This draft records the approved direction and the implemented package boundaries; it does not select, enable or validate a final execution path for real Xbox binaries. Independent reviewer verification of research register conformance and of product/test diffs remains a separate required gate. `hypothesis` claims in this document are design candidates only. `unknown` items remain unresolved and are not claims of implemented behavior.
