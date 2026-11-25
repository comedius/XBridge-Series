# XB-NEXT-DEVELOPMENT-03-R0 — performance measurement concept

Development infrastructure; not part of a product package. Access date for every URL and local file read in this document: **2026-11-25**. Target: **Xbox Series X only**. This is a research concept for future, lawful measurement; it contains **no measured result**, no game target, no playability claim and no approved implementation. No proprietary binary, key, firmware, game asset, DRM material, other-console evidence, build, CTest, guest execution or GPU run was used. All web content was treated as untrusted data.

Evidence labels follow `docs/RESEARCH_REGISTER.md` Sections 2–6. The owner-approved direction in `docs/AGENT_TASKS.md:3645` is hybrid native execution plus a compatibility/runtime layer, with the interpreter retained as reference, diagnostic and fallback. That direction is `documented (project-authored)`. It does not prove that arbitrary commercial Xbox binaries can execute or meet any performance target.

## 1. Source inventory

| ID | Exact URL / local path | Class | Precise section or excerpt used | Reproduction actually performed |
|---|---|---|---|---|
| S0 | `docs/AGENT_TASKS.md:3643-3673` | project-authored | Owner-approved hybrid direction; no arbitrary-game proof; no JIT/binary rewriting/code patching/real-game execution | Local file read, 2026-11-25 |
| S1 | exact local path `docs/ARCHITECTURE.md:141-183` | project-authored | Current interpreter is reference, not JIT; supports NOP and a narrow MOV subset; no SIMD/FPU, privileged instructions or Xbox-specific CPU properties | Local file read, 2026-11-25 |
| S2 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/command-queues-and-command-lists.md` | authoritative (public Windows D3D12 only) | Applications record and submit command lists from multiple threads to one or more queues; precomputing rendering work for later reuse; multi-core spreading | Direct `fetch`, 2026-11-25 |
| S3 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/executing-and-synchronizing-command-lists.md` | authoritative (public Windows D3D12 only) | Command queues, fences and synchronization for command-list execution and resources accessed by queues | Direct `fetch`, 2026-11-25 |
| S4 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/using-resource-barriers-to-synchronize-resource-states-in-direct3d-12.md` | authoritative (public Windows D3D12 only) | Applications specify before/after subresource states; `The application must ensure that the before and after states of consecutive calls to ResourceBarrier agree` | Direct `fetch`, 2026-11-25 |
| S5 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/direct3d12/memory-management-strategies.md` | authoritative (public Windows D3D12 only) | Committed, placed and reserved resource strategies; physical/virtual address separation; reserved resources have sparse GPU VA space | Direct `fetch`, 2026-11-25 |
| S6 | `https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170` | authoritative (general Windows x64 only) | Fast-call register arguments, shadow store and 16-byte stack alignment are candidate boundary-transition concerns, not Xbox guest facts | Direct `fetch`, 2026-11-25 |
| S7 | exact local paths `docs/research/XB-NEXT-03_EXECUTION_MODELS.md`, `docs/research/XB-NEXT-03_NATIVE_EXECUTION_FEASIBILITY.md`, `docs/research/XB-NEXT-03_GRAPHICS_ANALYSIS.md`, `docs/research/XB-NEXT-03_LIBRARY_FAMILIES.md` | project-authored | Execution-model axes, native execution unknowns, graphics mapping hypotheses and library-family evidence prepared in this package | Local file reads, 2026-11-25 |
| S8 | `docs/RESEARCH_REGISTER.md:107-119,331-355` | project-authored | Register scope, prior PE/COFF unknowns and no capability promotion without evidence | Local file read, 2026-11-25 |
| S9 | exact local path `docs/research/XB-NEXT-03_PERFORMANCE_CONCEPT.md` | project-authored | Full performance measurement concept; candidate metrics, reporting rules and reproduction paragraph; register source S9 for this package | Local file read, 2026-11-25 |

`observed` — S2–S6 returned the quoted content when fetched on 2026-11-25 and local files S0/S1/S7/S8/S9 were read. `unknown` — external truth after that date and authenticated GDK material were not re-checked.

## 2. Why no performance claim is made

1. `documented (project-authored)` S1: the current interpreter is a narrow scaffold with no SIMD/FPU and no JIT. It cannot serve as a real-game performance baseline.
2. `documented (project-authored)` S0/S7: native/hybrid execution is an owner-approved direction, but guest CPUID/AVX2/BMI/FMA requirements, ABI, TLS, exception semantics, syscall surface, memory model and privileged instructions are `unknown`.
3. `documented (project-authored)` S7: D3D12-to-D3D12 is a research approach only. Guest command encoding, shader format and D3D12.X equivalence are `unknown`.
4. `unknown`: no lawful synthetic workload has been executed in this package, and no real game was run. Therefore no speedup, frame time, compatibility percentage or stability claim is supportable.

## 3. Candidate measurement decomposition

All component names below are `hypothesis` until a specific lawful workload and instrumentation are approved. They describe what should be separated in a future experiment, not measured behavior.

| Layer | Candidate measurement | Relevant evidence or reason | Current label |
|---|---|---|---|
| Guest instruction execution | Guest instructions/decoded operations per second on a controlled synthetic stream; separate scalar, SIMD and unsupported-type cases. | `documented (project-authored)` S1 shows current scope; guest instruction mix is `unknown` (S7). | `hypothesis` |
| Interpreter path | CPU time per synthetic operation, branch/memory-fault counts, and failure behavior at narrow MOV/NOP boundaries. | `documented (project-authored)` S1 provides the baseline scope; no benchmark was run. | `hypothesis` |
| Native/hybrid path | CPU time for the same synthetic stream, boundary-transition count and fault/exception behavior. | `documented (project-authored)` S0 approves research direction only; no native worker exists. | `hypothesis` |
| ABI/TLS/exception transitions | Calls per second, time spent in transition, register/stack restoration correctness checks. | `documented (authoritative, general Windows x64 only)` S6 gives general ABI candidates; guest equivalent is `unknown` (S7). | `hypothesis` |
| Compatibility service calls | Number and latency of synthetic service requests; failure count for unsupported service IDs. | `documented (project-authored)` S0/S7: compatibility layer is intended to provide runtime behavior; service set is `unknown`. | `hypothesis` |
| Guest memory / permissions | Translation and protection fault counts; time spent validating bounded accesses. | `documented (project-authored)` S7 records generic guest memory and unknown Xbox memory semantics; no Xbox VM model is claimed. | `hypothesis` |
| Graphics command translation | Command lists recorded/submitted per synthetic frame, barrier-count validation, queue-fence waits and descriptor/PSO setup time. | `documented (authoritative, public Windows D3D12 only)` S2–S5; guest command format is `unknown` (S7). | `hypothesis` |
| Resource movement | Allocations by committed/placed/reserved strategy; host-side copies and mapped VA checks. | `documented (authoritative, public Windows D3D12 only)` S5 for public D3D12 strategies; guest memory model is `unknown`. | `hypothesis` |
| Frame pacing | Interval distribution of synthetic presents, not a real-game FPS target. | No target or measurement exists; this is a proposed metric only. | `hypothesis` |

No metric above should be reported as an XBridge result without an executed, reproducible harness and explicit evidence labels.

## 4. Candidate synthetic workload design

`hypothesis`: a future approved harness could use only lawful, self-authored inputs:

1. A synthetic PE32+/AMD64 image with bounded headers and only the two in-scope relocation types documented in `docs/research/XB-NEXT-03_PECOFF_RELOCATIONS.md`: `ABSOLUTE (0)` and `DIR64 (10)`. Unsupported relocation types would be a failure case, not silently corrected.
2. A synthetic instruction stream exercising scalar operations first; SIMD/BMI/FMA cases only after feature-detection evidence defines the required subset. Feature detection is a general x64 capability question; guest requirements remain `unknown`.
3. Synthetic compatibility-layer calls with fixed IDs and explicit unsupported-ID failure behavior. No Xbox service list is inferred.
4. A synthetic graphics command sequence using locally constructed host D3D12 objects only; guest command opcodes are not invented and no D3D12.X equivalence is assumed.
5. Boundary/failure cases: truncated relocation block, unsupported relocation type, ABI mismatch, TLS callback mismatch, injected exception, memory-protection violation, consecutive inconsistent resource barriers, fence timeout in a synthetic sequence.

This design is a research proposal. No harness, test, product interface or benchmark was created by this package.

## 5. Reporting rules for any future measurement

- Report exact command lines, tool versions, host CPU/GPU/OS configuration, workload hashes and access dates.
- Keep native, interpreter and hybrid results separate; never average incompatible execution models into one compatibility claim.
- Separate CPU guest-instruction time, compatibility-layer time, synchronization time and graphics submission/GPU time where instrumentation permits.
- Report distributions (minimum, median, p95/p99) for frame intervals when a present loop exists; a single average is insufficient for consistent-frame-time evaluation.
- Treat aborts, unsupported relocations, unsupported service IDs and validation failures as first-class results.
- Do not extrapolate from synthetic workloads to real games, and do not use published Xbox hardware specifications as a substitute for guest behavior evidence.
- `unknown` remains `unknown` when instrumentation cannot observe guest semantics; no capability token may be promoted by timing data alone.

## 6. Open questions

- Which synthetic instruction subset is sufficient to make the native/interpreter comparison meaningful without guessing guest requirements?
- What boundary-transition count would make a hybrid path worthwhile, and who approves that threshold?
- Can graphics translation be measured without assuming a guest command format or shader binary format? `unknown` with current sources.
- What isolation boundary and failure containment would be required before running native synthetic code? See `docs/research/XB-NEXT-03_NATIVE_EXECUTION_FEASIBILITY.md`; no worker process is implemented.

## 7. Conclusion and reproduction

`documented (project-authored)`: the hybrid direction and interpreter fallback are approved; the current interpreter is narrow. `documented (authoritative, general Windows/D3D12 only)`: public ABI and D3D12 sources describe candidate transition, queue, barrier, memory and synchronization cost centers. `hypothesis`: the layered measurement concept could compare synthetic paths once approved and instrumented. `unknown`: all Xbox guest-specific counterparts, real-game feasibility and actual performance. No measured speedup, frame-time target, compatibility claim or game run is recorded.

Reproduction: locally read S0, S1, S7, S8 and S9; directly `fetch` S2–S6 on 2026-11-25 and search the quoted anchors in the source table. No shell command was available in this researcher tool set; no diff check was attempted or claimed in this package.
