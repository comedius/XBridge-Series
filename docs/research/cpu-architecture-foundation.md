# CPU Architecture Foundation for XBridge Series

**Sprint:** 6  
**Research date:** 2026-11-21  
**Scope:** documentation-only research for a future x64/AMD64 interpreter. No Xbox CPU interpreter is implemented by this report.

This report distinguishes general x86-64/AMD64 architecture concepts from Microsoft's published Xbox Series X information and from Xbox-specific unknowns. It does not claim that XBridge currently executes Xbox code.

## Evidence labels

- **Documented fact:** directly stated in a cited public source or current project documentation.
- **Observation:** a bounded reading of the cited material or existing XBridge code.
- **Hypothesis:** a possible future design direction that requires further validation.
- **Unknown:** a point not established by the sources reviewed here.

## Xbox Series X public evidence

**Documented fact:** Microsoft's technology overview describes the CPU as:

> 8x Cores @ 3.8 GHz (3.6 GHz w/ SMT) Custom Zen 2 CPU

This is a high-level published description. It does not identify a desktop CPU model, CPUID family/model/stepping, optional instruction extensions, or an exact hardware-thread count. This report does not infer those details.

**Documented fact:** Microsoft GDK documentation lists `_GAMING_XBOX_SCARLETT` for the Xbox Series X|S target and `_M_X64` as a processor-architecture definition.

**Observation:** These GDK macros identify build targets; they are not a runtime CPU-feature inventory and do not establish which desktop x64 facilities are available to Xbox titles. The supplied Microsoft product-specifications page was not readable in this research access, so no additional CPU claim depends on it.

**Unknown:** Exact Xbox CPU identification, enabled extensions, and the title-visible user/system-mode contract remain unestablished by these sources.

## General x86-64 / AMD64 CPU-state concepts

The register descriptions below are general architecture concepts, not a claim that every optional feature is available in the Xbox Series X execution environment. Microsoft's x64 ABI documentation describes Windows calling conventions and register usage; it is not a complete processor specification. AMD's manuals are the intended AMD64 references, but the relevant online manual contents could not be inspected during this research. Their exact provisions must be checked directly before defining XBridge behavior.

### General-purpose registers

**Documented fact:** Microsoft's x64 conventions describe 16 general-purpose 64-bit registers. The familiar names are `RAX`, `RBX`, `RCX`, `RDX`, `RSI`, `RDI`, `RBP`, `RSP`, and `R8` through `R15`. The Microsoft x64 ABI documentation assigns some registers argument, return-value, stack, or caller/callee-preservation roles.

**Interpretation boundary:** ABI roles describe function-call conventions, not intrinsic guest CPU semantics. A future interpreter would need architectural register values and the documented access behavior for the supported instruction forms. Subregister aliases and writes to narrower register names must follow the selected architecture specification; this report does not define those rules.

### Instruction pointer

`RIP` is the 64-bit instruction-pointer name used in x86-64 programming references. A CPU state model needs a guest instruction position so the decoder can fetch the next instruction and control-flow instructions can change execution location.

**Unknown for Xbox:** the sources reviewed do not specify Xbox-specific instruction-fetch, exception-return, or context-switch details. Exact update and fault behavior must be taken from the applicable AMD64 provisions and any separately established Xbox execution contract.

### Flags register

`RFLAGS` is the x86-64 flags-register name. Instructions may consume or update selected status or control flags; the effect is instruction-specific. The Windows ABI's requirements about flags at function boundaries are ABI rules, not a complete definition of CPU flags behavior.

A future interpreter should model only the flags and effects needed by its explicitly supported instruction set, but those effects must be precise. Host processor flags must not be used as hidden guest state or assumed to have identical semantics.

### SIMD and floating-point state

**Documented fact:** Microsoft's x64 ABI material discusses 16 XMM/YMM register names in its Windows x64 conventions and uses XMM registers for floating-point arguments and operations under that ABI.

**Interpretation boundary:** calling-convention documentation does not establish a console's enabled SIMD extensions. XMM/YMM and other vector or floating-point state are relevant only when supported instructions use them. The required vector width, register-state layout, floating-point control/status state, and any extended-state save/restore rules depend on the exact architecture features and execution mode being targeted.

**Unknown for Xbox:** the reviewed Microsoft Xbox sources do not enumerate enabled optional instruction extensions, vector widths, or the complete floating-point and extended processor state exposed to title code. Do not infer AVX-family, wider-vector, or other optional feature availability from the Zen 2 description or from desktop documentation.

### Other potentially relevant state

Depending on the supported instruction subset and execution mode, an interpreter may eventually need state beyond GPRs, `RIP`, `RFLAGS`, and vector registers. Candidate categories to investigate in the manuals include floating-point control/status, segment or privilege state, memory-management state, debug state, and exception-related state. This is a research checklist, not a claim that all such state is accessible to Xbox title code or belongs in an initial interpreter.

## User mode and system mode

**General architecture:** x86-64 documentation distinguishes application-level execution from system-programming facilities. System-level topics include protection, memory management, interrupts, exceptions, and privileged state. The exact operations available depend on architecture rules, privilege level, operating environment, and implemented features.

**Xbox evidence boundary:** Microsoft's GDK preprocessor definitions distinguish an Xbox target (`_GAMING_XBOX_SCARLETT`) and an x64 architecture target (`_M_X64`). These are build-target definitions; they are not a runtime CPU-feature list or proof that a title can use every desktop x64 system facility.

**Unknown:** the sources reviewed do not establish the Xbox title execution privilege model, the availability of privileged instructions or control registers, or Xbox-specific exception and interrupt delivery. Do not implement desktop operating-system behavior as if it were the Xbox system environment.

## Candidate logical components for a future interpreter

These are architectural responsibilities to consider, not a finalized design or implementation commitment.

| Component | Possible responsibility | Evidence boundary |
| --- | --- | --- |
| CPU state | Hold guest GPR values, instruction pointer, flags, and only the vector, floating-point, mode, or exception state required by the supported subset. | Exact fields and widths depend on verified architecture features and execution scope. |
| Instruction decoder | Fetch bytes and decode supported encodings into an explicit instruction/operand representation. | Encoding rules must be verified against the relevant AMD64 manual; unsupported encodings must not be guessed. |
| Instruction execution | Apply the defined operation to guest state and request memory effects. | Each supported instruction form needs specified results, flags, and fault behavior. |
| Register access | Centralize register selection, aliases, and width-specific reads/writes. | Alias and partial-write rules require manual verification. |
| Flags handling | Compute, preserve, and expose flags as required by each instruction. | Do not assume host arithmetic flags match guest rules. |
| Memory access | Route guest reads/writes through a guest memory interface with explicit address and fault behavior. | The current `GuestMemory` is a generic scaffold, not an Xbox virtual-memory or address-translation specification. |
| Exception handling | Distinguish invalid/unsupported instructions, guest-visible faults, and implementation errors. | Guest exception classes, delivery, and restart semantics require authoritative evidence before system-level emulation. |

Separating these responsibilities would allow instruction decoding and pure instruction semantics to be tested independently from host scheduling and graphics. It does not require adding system-mode emulation to the first interpreter.

## Relationship to the existing XBridge architecture

**Documented project decisions:** [ARCHITECTURE.md](../ARCHITECTURE.md) states that `TestCpu` is a synthetic ISA, that a future x86-64 engine needs a documented decoder corpus, explicit exception and flag semantics, and tests for each supported form, and that a reference interpreter precedes any JIT. [DECISIONS.md](../DECISIONS.md) records ADR-0003: the MVP CPU is a tiny project-defined instruction set and a real x86-64 interpreter is a separate milestone.

**Observed in the current code:** [`TestCpu`](../../include/xbridge/cpu/test_cpu.hpp) has four 32-bit registers, a 32-bit instruction pointer, a halt flag, and four synthetic opcodes (`LoadImm`, `AddImm`, `Store`, `Halt`). Its implementation uses a small opcode switch and throws C++ exceptions for its test-ISA errors. It has no x86-64 flags, SIMD state, privilege model, or x86 instruction decoder.

**Observed memory boundary:** [`GuestMemory`](../../include/xbridge/memory/memory.hpp) exposes byte and 32-bit operations addressed by `std::uint32_t`; the architecture overview documents it as a bounded test scaffold. It does not provide x86-64 virtual-address translation, page permissions, or Xbox memory semantics. A future interpreter must not assume this interface already models those facilities.

**Design implication:** keep `TestCpu` and its public state as the synthetic test harness. A later x64 interpreter could have its own state and components rather than stretching `TestCpu`'s 32-bit registers, instruction pointer, or opcode model. This is a possible fit with existing boundaries, not a request to change architecture decisions now.

## Work that can be developed independently of Xbox-specific details

The following work could be developed and tested against a carefully selected, directly verified x86-64/AMD64 subset without asserting Xbox compatibility:

- Separate decoder, register-access, execution, flags, memory-interface, and exception responsibilities.
- Unit tests for byte decoding and instruction behavior from a documented corpus, including malformed and unsupported encodings.
- Pure tests for register and flag effects once each rule has been checked against the relevant architecture manual.
- A mock guest-memory interface to test reads, writes, and faults without relying on host memory layout.
- A reference interpreter step loop and deterministic test programs, with unsupported behavior reported explicitly.

The following remain dependent on further evidence and should not be inferred from desktop x64 behavior: the exact Xbox CPU identity and CPUID signature, enabled optional extensions, title-visible hardware-thread behavior, privilege/system-mode scope, platform exception behavior, and Xbox-specific memory or execution details.

## Constraints retained

- The interpreter remains the first reference implementation; JIT remains a later optional optimization requiring separate justification and tests.
- No exact Xbox CPU model, CPUID value, optional instruction extension, or system function is asserted.
- No Xbox CPU interpreter or implementation is added by this research.
- No PS5 material is used as Xbox evidence, and no protection mechanism is bypassed.
- Existing architecture decisions and the synthetic `TestCpu` contract remain unchanged.

## Sources

Public sources accessed or checked on **2026-11-21**. AMD manual pages were reachable only as online viewer shells in this research environment; their exact provisions must be inspected directly before they define implementation behavior.

1. **Microsoft, Xbox Series X technology overview.** Published CPU description and overview. https://news.xbox.com/en-us/2020/03/16/xbox-series-x-tech/
2. **Microsoft, Xbox Series X specifications.** Supplied official specifications page; its CPU details were not retrieved during this research access. https://www.microsoft.com/en-us/d/xbox-series-x/8wj714n3rbtl
3. **Microsoft, GDK preprocessor definitions.** Documents `_GAMING_XBOX_SCARLETT` and `_M_X64` build-target macros. https://learn.microsoft.com/en-us/gaming/gdk/docs/tools/tools-console/visualstudio/preprocessor-definitions
4. **Microsoft Learn, x64 ABI conventions.** Windows x64 register and ABI conventions; not a complete CPU or Xbox specification. https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions?view=msvc-170
5. **Microsoft Learn, x64 calling convention.** Windows x64 function-call ABI. https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170
6. **AMD, AMD64 Architecture Programmer's Manual, Volume 1: Application Programming.** Exact manual text was not inspectable through the online viewer during this research. https://docs.amd.com/v/u/en-US/24592_3.24
7. **AMD, AMD64 Architecture Programmer's Manual, Volume 3: General-Purpose and System Instructions.** Exact manual text was not inspectable through the online viewer during this research. https://docs.amd.com/v/u/en-US/24594_3.37
8. **Intel, Intel 64 and IA-32 Architectures Software Developer's Manuals.** General x86-64 architectural reference only; not evidence of AMD-specific features or Xbox behavior. https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html

This report is research documentation, not a change to the accepted XBridge architecture or a claim of Xbox CPU compatibility.
