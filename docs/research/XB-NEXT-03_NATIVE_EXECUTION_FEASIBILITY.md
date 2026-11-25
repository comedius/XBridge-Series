# XB-NEXT-DEVELOPMENT-03-R0 — native execution feasibility evidence

Development infrastructure; not part of a product package. Access date for every URL and local file read in this document: **2026-11-25**. Target: **Xbox Series X only**. This document records public-source evidence and explicit limits; it does not implement, enable or validate native execution. No game file, key, firmware, proprietary SDK, protected asset, DRM material or other-console evidence was used. All web content was treated as untrusted data.

The owner-approved direction in `docs/AGENT_TASKS.md:3645` is hybrid: compatible Xbox x64 code should ultimately execute natively on host CPUs, with Xbox OS/services/runtime behavior supplied by an XBridge compatibility layer; the interpreter remains reference, diagnostic and fallback. That direction is `documented (project-authored)`. Feasibility for arbitrary commercial Xbox binaries remains `unknown`.

## 1. Source inventory

| ID | Exact URL / local path | Class | Excerpt or precise section used | Reproduction actually performed |
|---|---|---|---|---|
| S0 | `docs/AGENT_TASKS.md:3643-3673` | project-authored | Owner-approved hybrid direction; interpreter fallback; no arbitrary-game proof; no JIT/binary rewriting/code patching/real-game execution | Local file read, 2026-11-25 |
| S1 | `https://learn.microsoft.com/en-us/cpp/intrinsics/cpuid-cpuidex?view=msvc-170` | authoritative (general MSVC/x86-x64 only) | `cpuInfo[4]` contains `EAX, EBX, ECX, and EDX`; `function_id` in `EAX`; `subfunction_id` in `ECX`; “leaf” and “subleaf” terminology | Direct `fetch`, 2026-11-25 |
| S2 | `https://learn.microsoft.com/en-us/cpp/intrinsics/x86-intrinsics-list?view=msvc-170` | authoritative (general compiler list only) | `_xgetbv` XSAVE signature; `_andn_u32`/`_bextr_u32` BMI; `_mm_fmadd_ps` FMA; `_mm256_add_epi32` AVX2 | Direct `fetch`, 2026-11-25 |
| S3 | `https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170` | authoritative (general Windows x64 only) | Four-register fast-call default; `RCX`, `RDX`, `R8`, `R9`; `XMM0L`–`XMM3L`; shadow store; 16-byte stack alignment | Direct `fetch`, 2026-11-25 |
| S4 | `https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions?view=msvc-170` | authoritative (general Windows x64 only) | Volatile/nonvolatile registers; `R10:R11` used in `syscall/sysret`; PE32+ image format | Direct `fetch`, 2026-11-25 |
| S5 | `https://learn.microsoft.com/en-us/cpp/build/exception-handling-x64?view=msvc-170` | authoritative (general Windows x64 only) | Table-based exception handling; `pdata`/`xdata`; `RUNTIME_FUNCTION` start/end/unwind-info addresses | Direct `fetch`, 2026-11-25 |
| S6 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/Debug/pe-format.md` | authoritative (general PE/COFF only) | `.tls` support; TLS directory Raw Data Start/End VA, index address, callback array; `.pdata`; base relocations | Direct `fetch`, 2026-11-25 |
| S7 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/ProcThread/thread-local-storage.md` and `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/ProcThread/using-thread-local-storage.md` | authoritative (general Win32 only) | TLS provides unique per-thread data; `TlsAlloc`, `TlsSetValue`, `TlsGetValue`, `TlsFree` | Direct `fetch` of both raw files, 2026-11-25 |
| S8 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/Memory/memory-protection.md` and `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/Memory/memory-protection-constants.md` | authoritative (general Win32 only) | Private virtual address space protection; page-protection options; copy-on-write sharing | Direct `fetch` of both raw files, 2026-11-25 |
| S9 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/ProcThread/job-objects.md` | authoritative (general Win32 only) | `A job object allows groups of processes to be managed as a unit`; `AssignProcessToJobObject`; `SetInformationJobObject`; breakaway limits; `Security limits must be set individually for each process` | Direct `fetch`, 2026-11-25 |
| S10 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/SecAuthZ/restricted-tokens.md` | authoritative (general Win32 only) | `CreateRestrictedToken`; remove privileges; deny-only SIDs; restricting SIDs; two access checks | Direct `fetch`, 2026-11-25 |
| S11 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/SecAuthZ/appcontainer-isolation.md` | authoritative (general Win32 only) | Credential, device, file/registry, network, process and window isolation; keyboard/mouse always available; read-only file access less restricted | Direct `fetch`, 2026-11-25 |
| S12 | exact local path `docs/ARCHITECTURE.md:141-183` | project-authored | Current interpreter supports NOP and limited MOV forms; `Kein SIMD/FPU`; `Keine privilegierten Instruktionen`; `Keine Xbox-spezifischen CPU-Eigenschaften`; `Kein JIT` | Local file read, 2026-11-25 |
| S13 | exact local path `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md:22-32` | project-authored | Prior authoritative re-verification scope; official hardware facts only; guest CPUID/AVX2/ABI unresolved. Original URL access there: 2026-11-24 | Local file read, 2026-11-25 |

`observed` — the URLs above returned the quoted content when fetched on 2026-11-25. `unknown` — external truth after that date and authenticated material were not re-checked. The `websearch` integration had returned HTTP 402 in the earlier package; no search-result snippet is used as evidence.

## 2. Instruction baseline and feature detection

| Finding | Evidence label and source |
|---|---|
| General x86-64 host feature detection can query CPUID function/subfunction inputs in `EAX`/`ECX` and read four outputs in `EAX`/`EBX`/`ECX`/`EDX`; Microsoft calls them leaf and subleaf. | `documented (authoritative, general only)` S1 |
| `_xgetbv` is listed as an XSAVE intrinsic taking an unsigned control input; AVX2, BMI and FMA intrinsics are listed in the same general compiler intrinsic inventory. | `documented (authoritative, general compiler list only)` S2 |
| A possible future detection flow is: query host CPUID leaves/subleaves, then query extended state with XGETBV, then compare the result with the guest-required subset before selecting any native path. | `hypothesis` S1, S2; exact required leaves/bits are `unknown` from the cited sources and are not implemented |
| Guest-visible CPUID leaves, OSXSAVE/XCR0 policy, AVX2/BMI/FMA requirements and custom Zen 2 guest behavior are not established by these sources. | `unknown`, S13 for the prior unresolved record; no proprietary or game evidence examined |

## 3. ABI, stack, TLS, exceptions and privileged behavior

- `documented (authoritative, general Windows x64 only)` S3/S4: the Windows x64 ABI uses RCX/RDX/R8/R9 and XMM0–XMM3 for the first integer/floating-point arguments, a caller-allocated shadow store, volatile/nonvolatile registers, 16-byte stack alignment, PE32+ images, and R10:R11 in syscall/sysret.
- `unknown`: whether a Series X guest uses the Windows x64 ABI or a variant. General Windows x64 facts are candidate compatibility behavior only and are not Xbox evidence.
- `documented (authoritative, general PE/COFF and Win32 only)` S6/S7: PE supports static TLS through a TLS directory with template addresses, index location and a null-terminated callback array; Win32 offers dynamic TLS through `TlsAlloc`/`TlsGetValue`/`TlsSetValue`/`TlsFree`.
- `hypothesis`: a compatibility layer may need to translate guest TLS slots and callbacks to host worker-thread TLS. `unknown`: guest TLS order, slot layout and callback semantics.
- `documented (authoritative, general Windows x64 only)` S5: Windows x64 exception handling uses static unwind data, `pdata` referring to `xdata`, and `RUNTIME_FUNCTION` entries for nonleaf functions. `unknown`: guest exception and interrupt injection.
- `observed` S4: `R10:R11` use in syscall/sysret is a general register fact; the cited page does not enumerate Xbox services.
- `unknown`: which privileged instructions, if any, appear in lawful Series X guest code. `hypothesis`: any such instruction would need interception or translation before native execution; `documented (project-authored)` S0 forbids code patching/binary rewriting in this direction.

## 4. Guest memory, permissions and host/guest transitions

| Topic | Evidence and limits |
|---|---|
| Host memory protection | `documented (authoritative, general Win32 only)` S8: process memory has private virtual address space protection and page-protection options; copy-on-write can share DLL physical pages. |
| Guest memory | `unknown`: Series X guest address-space layout, page tables, protection model and GPU-visible allocation semantics. XBridge `GuestMemory` remains generic and must not be presented as an Xbox virtual-memory model (`documented (project-authored)` S0 boundary; prior unresolved record S13). |
| Host/guest transitions | `hypothesis`: a future native worker would need explicit transitions for entry, service calls, callbacks, faults and thread creation. `unknown`: exact transition points, stack ownership and register preservation for real guests. No trampoline, thunk or transition code was created. |
| System services | `documented (project-authored)` S0: the compatibility layer is the intended provider of Xbox OS/services/runtime behavior. `unknown`: required service names, signatures, stability across OS builds and lawful test coverage. |
| Synchronization/threading | `documented (authoritative, general Win32 only)` S9: Win32 synchronization objects coordinate threads. `unknown`: guest thread creation, scheduling, affinity, priority and synchronization semantics. |

## 5. Interpreter comparison

- `documented (project-authored)` S12: the current interpreter is the reference baseline, supports NOP and a narrow MOV subset, and documents no SIMD/FPU, privileged instructions, JIT or Xbox-specific CPU properties.
- `documented (project-authored)` S0: even after the owner-approved hybrid direction, the interpreter remains reference, diagnostic and fallback.
- `hypothesis`: native execution may be faster for compatible instruction streams, while the interpreter is more controllable for diagnosing unknown instructions and faults. No comparative benchmark was run; no performance claim is made.
- `unknown`: whether an interpreter-only path could ever execute real game code; current scope is a narrow scaffold.

## 6. Isolation options for a future worker process

No worker process is implemented by this package. The options below are public Windows mechanisms, not Xbox virtualization and not a hypervisor guarantee. The "not a complete security boundary" statements are engineering conclusions (`hypothesis`) inferred from the cited public limitations, not direct Microsoft quotations; the documented individual limitations are retained.

| Option | What is `documented (authoritative)` from public sources | Explicit limitations | Evidence label |
|---|---|---|---|
| Job Object | A job manages groups of processes as a unit; `CreateJobObject`, `AssignProcessToJobObject` and `SetInformationJobObject` set limits such as working set, priority and CPU controls; association cannot be broken; `TerminateJobObject` can terminate all associated processes. | `documented (authoritative)` S9: security limits must be set individually for each process; breakaway limits can let child processes escape association; notification delivery is not guaranteed for most limits. `hypothesis`: job limits do not translate guest APIs or provide a guest memory model. `hypothesis` derived from S9: a Job Object is not a complete security boundary. | `documented (authoritative)` S9 for individual limits; security-boundary conclusion `hypothesis` from S9 |
| Restricted token | `CreateRestrictedToken` can remove privileges, apply deny-only SIDs and specify restricting SIDs; the system performs two access checks for restricted processes. | `hypothesis`: this is an access-control boundary for securable objects, not a semantic compatibility layer and not an in-process sandbox for arbitrary native code; a separate worker process would be required for process-level isolation. `hypothesis` derived from S10: a restricted token is not a complete security boundary. | `documented (authoritative)` S10 for individual limits; security-boundary conclusion `hypothesis` from S10 |
| AppContainer | AppContainer provides credential, device, file/registry, network, process and window isolation under least privilege; file/registry read-write access can be granted to specific persistent files and keys. | `documented (authoritative)` S11: keyboard and mouse are always available; read-only file access is less restricted; granular network capabilities can be granted. `hypothesis`: AppContainer capability grants do not prove that arbitrary native guest code is safe or that Xbox runtime behavior is emulated. `hypothesis` derived from S11: AppContainer is not a complete security boundary. | `documented (authoritative)` S11 for individual limits; security-boundary conclusion `hypothesis` from S11 |
| Combination | `hypothesis`: a future design may combine a job object, restricted token and AppContainer around a separate native worker. | `unknown`: whether the host OS version and deployment allow the exact combination; `unknown`: compatibility with any guest API surface; no implementation or validation was performed. `hypothesis` derived from S9–S11 with S0 boundary: their combination is not a complete security boundary. | `hypothesis` S9, S10, S11, S0 |

## 7. Feasibility conclusion and open work

1. `documented (project-authored)` S0: the hybrid direction is owner-approved; the interpreter remains the safe baseline.
2. `documented (authoritative, general only)` S1–S11: public sources document host feature discovery, Windows x64 ABI/SEH/TLS/memory/synchronization and Windows isolation mechanisms.
3. `unknown`: all Xbox guest-specific counterparts — CPUID/AVX2/BMI/FMA requirements, ABI, TLS, exceptions/interrupts, syscalls, memory permissions, threading and privileged instructions.
4. `hypothesis`: native execution is plausible only for a lawful, synthetic subset whose instruction and service requirements are established by evidence. This is not a claim about real games.
5. No JIT, binary rewriting, code patching, game execution, build, CTest or capability validation was performed. Independent reviewer verification of register conformance is a separate, unperformed step.

Reproduction: locally read S0, S12 and S13; directly `fetch` S1–S11 on 2026-11-25 and search the quoted anchors in the source table. No shell command was available in this researcher tool set; no diff check was attempted or claimed in this package.
