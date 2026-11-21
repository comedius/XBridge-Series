# Xbox Series X CPU: Public Evidence and Interpreter Requirements

**Research type:** documentation-only. **Access date:** 2026-11-21. **Scope:** publicly available Microsoft and AMD material listed below. No PS5 material is used as evidence for Xbox behavior.

This report separates statements made by sources from observations about those sources, hypotheses for future work, and facts that remain unknown. It is not a hardware specification or a claim of Xbox CPU compatibility.

## Evidence labels

- **Documented fact:** explicitly stated by a cited public source.
- **Observation:** a limited reading of what a source does or does not identify; it is not a claim about undocumented hardware behavior.
- **Hypothesis:** a plausible engineering direction that still needs evidence.
- **Unknown:** information not established by the sources reviewed here.

## Documented CPU description

Microsoft's Xbox Series X technology overview publishes the following CPU line:

> 8x Cores @ 3.8 GHz (3.6 GHz w/ SMT) Custom Zen 2 CPU

This is the wording and level of detail reported by Microsoft. It identifies a custom Zen 2 CPU, eight cores, and the listed frequencies, including the 3.6 GHz SMT configuration. It does **not** state an exact desktop processor model, CPUID family/model/stepping, or an exact number of hardware threads. This report does not infer those details.

Microsoft's Xbox Series X product specifications page is also included in the references. Its CPU-specific contents could not be retrieved by the research browser on the access date; the quotation above is therefore attributed to the accessible Microsoft technology overview, not inferred from the product page.

## AMD64 ISA versus the Xbox processor implementation

**Documented fact:** Microsoft's overview calls the Xbox processor a *custom Zen 2 CPU*. The Microsoft GDK documentation lists `_GAMING_XBOX_SCARLETT` for Xbox Series X|S and `_M_X64` as a processor-architecture definition.

**Interpretation boundary:** AMD64 (also commonly called x86-64) describes an instruction-set architecture and associated programmer-visible behavior. AMD's Architecture Programmer's Manuals are intended as references for AMD64 architectural behavior, including instruction semantics and system-programming topics. However, the relevant online manual contents could not be inspected during this research. Their exact provisions must therefore be checked directly before they are used to define XBridge interpreter behavior. An ISA manual is not a specification of every processor implementing that ISA.

The Xbox CPU is a platform-specific custom implementation. Once inspected, the AMD64 manuals may inform candidate instruction semantics; they cannot establish that a given desktop Zen 2 model is the Xbox CPU, identify its CPUID signature, or prove that every desktop Zen 2 feature is enabled on Xbox. Likewise, the GDK's architecture and platform preprocessor definitions describe build targets; they are not a complete hardware feature enumeration.

## Evidence assessment

### Documented facts

1. Microsoft describes the Xbox Series X CPU as a custom Zen 2 CPU and publishes the core count and clock-frequency wording quoted above.
2. The Microsoft GDK documentation identifies `_GAMING_XBOX_SCARLETT` as the Xbox Series X|S target definition and documents `_M_X64` among processor-architecture definitions.
3. AMD publishes AMD64 Architecture Programmer's Manuals, including the supplied general manual reference and Volumes 1 and 3. These are intended as architectural references, not Xbox Series X platform specifications; their relevant contents could not be inspected during this research.

### Observations

- The public CPU description is high-level. It does not expose an exact AMD retail product identity or a complete feature/CPUID table.
- The GDK definitions distinguish an Xbox target from an architecture target. They do not establish which individual optional ISA extensions or system facilities are available at runtime.
- The AMD64 manuals are intended as references for architectural rules, but their relevant contents were not inspectable here; they do not, by themselves, identify the configuration chosen for a custom console processor.

### Hypotheses, not implementation facts

- AMD64 is a reasonable architecture family to investigate for a future Xbox CPU interpreter, given Microsoft's Zen 2 description and the GDK's x64 target definition.
- Any particular instruction subset, extension, exception behavior, or system-mode feature must still be confirmed for the relevant Xbox execution environment before being treated as required or supported.

These hypotheses must not be used to claim current Xbox execution or compatibility.

### Unknowns in the reviewed public material

- Exact Xbox Series X CPU identification, including CPUID family, model, and stepping.
- Which optional AMD64 instruction extensions and feature bits are enabled and exposed to title code.
- The exact number and guest-visible behavior of hardware threads.
- System-mode behavior and the availability or restrictions of privileged instructions, control registers, MSRs, and related facilities.
- Platform-specific execution details such as exception delivery, interrupt behavior, context switching, scheduling, and the interfaces through which software observes or controls them.
- Any differences between the documented AMD64 architectural rules and the Xbox-specific processor/platform configuration that matter to a particular program.

The sources reviewed here do not resolve these unknowns. No desktop CPU identity or feature set should be substituted for missing Xbox evidence.

## Cautious requirements for a future interpreter

The following are proposed engineering requirements, not claims about existing functionality:

1. **Keep the reference interpreter first.** Implement and validate any future CPU interpreter before considering JIT optimization. The current interpreter-first, JIT-later policy remains unchanged.
2. **Define an evidence-backed supported subset.** Consult the relevant AMD64 manual directly and verify its exact provision before using it to define each supported instruction and form; separately verify that the behavior is relevant to the Xbox execution environment. Do not treat the "Zen 2" label as an exact processor identity or as proof that all desktop Zen 2 features are enabled on Xbox.
3. **Specify instruction effects and test them.** For every implemented form, define its results, flags, exceptions, and relevant memory effects. Add focused, deterministic tests before expanding the supported subset.
4. **Keep unknowns explicit.** Do not invent CPUID values, optional instruction extensions, system-mode behavior, or a hardware-thread count. Report unsupported or unverified behavior rather than presenting it as Xbox-compatible.
5. **Separate guest semantics from host capabilities.** The host CPU and its available instructions must not silently determine guest-visible behavior. Any host-specific optimization must preserve the interpreter's defined results.
6. **Require evidence for system-level behavior.** Treat privileged instructions and platform-specific exception, interrupt, and execution behavior as separate research questions. Do not infer Xbox behavior from GDK preprocessor definitions or from an AMD64 manual alone.
7. **Validate without overclaiming.** Use independently created, deterministic test programs and documented behavior. A desktop reference may help validate an architectural rule, but desktop results alone do not establish Xbox-specific behavior.

These are research-derived requirements, not implemented capabilities or proof of Xbox compatibility. Keep the existing interpreter-first, JIT-later policy unchanged: a reference interpreter remains the first implementation step, and any later JIT requires separate technical justification and tests. The current `TestCpu` remains a synthetic test ISA.

## Sources

The following sources were checked on **2026-11-21**. Microsoft and AMD material is used as technical documentation, not as proof of undocumented Xbox behavior.

1. **Microsoft Xbox Series X Technology Overview, "Xbox Series X: A Closer Look at the Technology Powering the Next Generation" (2020-03-16).** The CPU description quoted above is available in this article. [Direct URL](https://news.xbox.com/en-us/2020/03/16/xbox-series-x-tech/)
2. **Microsoft Xbox Series X Specifications.** The supplied official product page could not be retrieved by the research browser on the access date; no CPU claim in this report depends on content unavailable from the Technology Overview. [Direct URL](https://www.microsoft.com/en-us/d/xbox-series-x/8wj714n3rbtl)
3. **Microsoft GDK Preprocessor Definitions.** Documents `_GAMING_XBOX_SCARLETT` and `_M_X64` build-target definitions; these are not a runtime CPU feature inventory. [Direct URL](https://learn.microsoft.com/en-us/gaming/gdk/docs/tools/tools-console/visualstudio/preprocessor-definitions)
4. **AMD64 Architecture Programmer's Manual, document identifier 40332, revision 4.09** (identifier and revision as shown in the supplied URL). The online viewer did not expose readable manual text during this review. [Direct URL](https://docs.amd.com/v/u/en-US/40332_4.09_APM_PUB)
5. **AMD Architecture Programmer's Manual, Volume 1: Application Programming.** The online viewer did not expose readable manual text during this review. [Direct URL](https://docs.amd.com/v/u/en-US/24592_3.24)
6. **AMD Architecture Programmer's Manual, Volume 3: General-Purpose and System Instructions.** The online viewer did not expose readable manual text during this review. [Direct URL](https://docs.amd.com/v/u/en-US/24594_3.37)

For project policy context, see [the CPU strategy in the architecture overview](../ARCHITECTURE.md#cpu-strategy). This report does not change that policy or claim that XBridge executes Xbox CPU code.
