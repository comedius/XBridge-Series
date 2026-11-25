# Architecture overview

## Goals

- Modular host application for Windows x64.
- Separate target hardware metadata from implementation.
- Explicit interfaces between CPU, memory, graphics, storage, and system layers.
- Deterministic test harnesses before undocumented hardware behavior is attempted.
- Long-term research toward Xbox Series X compatibility and system emulation. Older Xbox generations are optional, isolated future modules.

## Target scope

Xbox Series X is the only current emulation target. Do not maintain a parallel Series S target or assume Series S-specific behavior. Backward compatibility with Xbox One, Xbox 360, and original Xbox is explicitly optional and deferred; each generation would require a separate compatibility module and evidence-backed scope.

## Host hardware policy (accepted)

XBridge uses a **hybrid** host-hardware policy:

1. Detect host CPU features, logical cores, available RAM, GPU/device capabilities, VRAM when exposed, graphics API/driver support, and storage characteristics.
2. Select safe defaults and the best supported graphics acceleration path automatically.
3. Expose user-overridable performance profiles and advanced settings.
4. Re-evaluate capabilities when devices or drivers change; report why a backend was selected or rejected.

Hardware acceleration is preferred. A **software fallback** is required where a feasible implementation exists, especially for reference rendering, debugging, and unsupported acceleration paths. Fallback is capability-specific: it may be substantially slower, may support fewer features, and must report limitations rather than claim universal compatibility. Do not promise playable performance on low-end PCs.

### Host/guest separation

The guest remains a logically fixed Xbox Series X regardless of host hardware. The host's 16 GB RAM or 8 GB VRAM is not treated as equivalent to the console's unified GDDR6 pool. Guest memory is modeled separately and mapped/committed to host resources on demand, subject to host budgets. Host resource exhaustion must produce controlled errors or documented degradation, not silently change guest memory semantics.

### Current GuestMemory scaffold contract

The current `GuestMemory` is a generic test scaffold, not an Xbox memory specification:

- Guest addresses are 32-bit unsigned byte offsets into one zero-initialized contiguous region. The maximum representable region is 2^32 bytes; a region larger than that is rejected.
- A non-empty access is valid only when its complete half-open range `[address, address + length)` lies inside `[0, size)`. Boundary arithmetic must not wrap.
- `read8`/`write8` access one byte. `read32`/`write32` operate bytewise in little-endian order and support unaligned addresses. `load` copies the provided bytes without interpreting them.
- An out-of-range access throws `std::out_of_range`; a requested region larger than the address space throws `std::length_error`. A zero-length `load` is a no-op at any address from zero through `size`, inclusive, and is invalid beyond that.
- Reservation/release, per-region permissions, unmapped/reserved/committed states, aliasing, and host mappings are not implemented. Their Xbox-specific behavior remains `unknown`; this scaffold must not be used to infer it.

Performance profiles may tune host thread scheduling, cache sizes, resource residency, asynchronous work, and presentation resolution/quality where safe. They must not silently alter guest-visible CPU/GPU behavior, timing assumptions, memory layout, or API results. Any accuracy/performance trade-off that can affect compatibility must be explicit and opt-in.

### Graphics backend strategy

Define a backend-neutral host graphics interface. Research and implement backends independently, initially comparing **Vulkan** and **Direct3D 12**. Vulkan is an API, not an emulator: it provides a common way to use supported host GPUs. Keep these layers separate:

- guest command/API interpretation;
- guest resource and synchronization model;
- shader translation and intermediate representation;
- host graphics backend (Vulkan, Direct3D 12);
- optional CPU software/reference backend.

Never assume PC Direct3D 12 is equivalent to Xbox-specific graphics interfaces. Backend selection must be capability-based, not based only on GPU brand or model.

## Original 0.1.0 milestone exclusions (historical baseline)

The exclusions below describe the project's original 0.1.0 milestone scope. They are historical baseline statements, not a declaration that the repository's current CMake project version is 0.1.0. The current CMake project version is 0.3.0. Any change to these scope exclusions requires an explicit architecture decision and must not be inferred from the version number.

- Executing Xbox executables.
- Loading Xbox firmware or keys.
- Implementing Xbox security or boot chains.
- Claiming Zen 2 or RDNA 2 emulation.
- Shipping or decrypting commercial game content.

## Layering

1. App: CLI and later UI.
2. Core runtime: lifecycle, configuration, scheduling, diagnostics.
3. Target system: documented API compatibility and system services.
4. Hardware engines: CPU, GPU, memory, storage, audio, input.
5. Host capability layer: Windows APIs, device discovery, and graphics/audio backends.

Dependencies should point inward through interfaces. CPU and GPU modules may depend on memory interfaces, but must not reach into each other's private implementation.

### Product vs development infrastructure boundary

OpenCode configuration, specialized agents, prompts, internal automation, workflow documents and temporary development artifacts are development infrastructure. They stay in the public development repository and are excluded from a future product package, which is defined later by an explicit allowlist. Product code must not depend on AI services or agent control unless a separate ADR and owner approval define that behavior as a product function. The final cleanup is a late-stage, owner-approved package; it must not remove the development infrastructure needed for post-release maintenance.

## CPU strategy

The MVP's `TestCpu` is a synthetic instruction set for exercising fetch/decode/execute and memory interfaces. It is not x86-64.

`X64CpuState` is a separate, zero-initialized storage container for 16 64-bit
general-purpose register slots, `RIP`, and `RFLAGS`. Its typed register index
selects storage, and its `read_gpr()`/`write_gpr()` helpers expose 64-, 32-,
16-, 8-, and legacy high-byte views. A 32-bit write zero-extends; narrower
writes preserve all non-selected bits. High-byte access applies only to
`RAX`, `RBX`, `RCX`, and `RDX`; instruction-specific legality, including REX
prefix handling, belongs to a future decoder. This model defines no
instruction execution, CPU reset behavior, SIMD/FPU state, or Xbox-specific
properties. `TestCpu` remains the synthetic test ISA and is not changed.

A future x86-64 engine must have:
- an instruction decoder backed by a documented test corpus;
- a reference interpreter before JIT optimization;
- explicit exception and flag semantics;
- tests for each supported instruction form;
- differential tests against a trusted reference where lawful and practical.

### x64 Instruction Decoder (Sprint 8)

**Aufgabe**
Dekodiert x86-64/AMD64-Instruction-Bytes in eine explizite, strukturierte Darstellung.
Der Decoder ist state-los, arbeitet unabhängig von `X64CpuState` und verändert keinen
CPU-Zustand. Dekodierung und Ausführung sind sauber getrennt.

**Unterstützte Opcodes und Encodings**
- `NOP` (`0x90`) → `status == Ok`, `length == 1`, `opcode == 0x90`. REX-Präfixe vor NOP
  werden akzeptiert und ignoriert.
- `MOV r64/r32, imm64/imm32` (`0xB8 + reg`, optional mit REX.W für 64-bit)
- `MOV r64/r32, r64/r32` (`0x89 /r` bzw. `0x8B /r` mit ModR/M `mod == 11`)
- `MOV r64/r32, m64/m32` und `MOV m64/m32, r64/r32` (`0x89 /r` bzw. `0x8B /r`
  mit ModR/M `mod == 0b00`, einfache Register-Indirektadressierung ohne SIB
  und ohne Displacement; `rm == 100` (SIB) und `rm == 101` (RIP-relative)
  werden als `InvalidEncoding` zurückgewiesen)
- 8-bit MOV (`0x88`, `0x8a`) → `UnknownOpcode` (nicht unterstützt)

**REX-Präfixe (0x40–0x4F)**
- W: 64-bit Operandengröße
- R: Erweitertes ModR/M reg-Feld (Register R8–R15; ob Quelle oder Ziel hängt vom Opcode ab)
- B: Erweitertes ModR/M r/m- oder Opcode-Register-Feld (Register R8–R15 als Quelle/Ziel)
- X: Wird erkannt, aber nicht verwendet (keine SIB-Unterstützung in Sprint 8)

**Fehlerbehandlung**
- Leere Eingabe: `EmptyInput`, `length == 0`, keine Ausnahme.
- Unbekannter Opcode: `UnknownOpcode`, `length` umfasst die bis zum Opcode gelesenen
  Bytes (inkl. REX-Präfixe), keine Ausnahme.
- Ungültige Präfix-/Encoding-Kombination: `InvalidEncoding`, keine Ausnahme.
  Dazu zählen `mod == 0b00, rm == 0b100` (SIB erforderlich) sowie
  `mod == 0b00, rm == 0b101` (RIP-relative Adressierung erforderlich).
- Unvollständige Eingabe: `TruncatedInput`, keine Ausnahme.
- Keine Exceptions für Dekodierfehler.

**Schnittstelle**
- Eingabe: `std::span<const std::uint8_t>`
- Rückgabe: `DecodedInstruction` mit Feldern `status`, `opcode`, `length`,
  `operand_size`, `rex`, `dest_reg`, `src_reg`, `memory_operand`, `displacement`,
  `immediate`, `immediate_size`
- Status-Enum: `X64DecodeStatus { Ok, EmptyInput, UnknownOpcode, InvalidEncoding,
  TruncatedInput }`
- Header: `include/xbridge/cpu/x64_instruction_decoder.hpp`
- Implementation: `src/cpu/x64_instruction_decoder.cpp`

### x64 Instruction Interpreter (Sprint 8)

**Aufgabe**
Führt dekodierte x64-Instruktionen auf `X64CpuState` und `GuestMemory` aus.
Der Interpreter ist die Referenzimplementierung (kein JIT).

**Unterstützte Operationen**
- NOP: RIP um Instruktionslänge vorantreiben.
- MOV Register, Immediate: Register setzen (32-bit schreibt zero-extends auf 64-bit).
- MOV Register zu Register: Quellregister in Zielregister kopieren.
- MOV zwischen Register und Speicher: Little-Endian Lade-/Speicherzugriff.

**Fehlerverhalten**
- Speicheradresse über UINT32_MAX: `MemoryAccessError` (kein stilles Truncating).
- Speicherzugriff außerhalb der Grenzen: vollständiger Bereich wird vor dem Zugriff
  validiert; bei Verletzung `MemoryAccessError` ohne jegliche Zustandsänderung.
- 64-Bit-Schreibzugriffe: alle 8 Bytes werden vor dem Schreiben validiert;
  bei Fehler bleiben weder CPU-Zustand noch Speicher verändert.
- `std::out_of_range` von `GuestMemory` wird abgefangen,
  `X64ExecuteStatus::MemoryAccessError` zurückgegeben.
- Bei Fehlern wird keinerlei CPU-Zustand oder Speicher verändert.
- Nicht ausführbare Dekodierergebnisse (TruncatedInput, UnknownOpcode, etc.)
  werden als No-op behandelt.

**Schnittstelle**
- Header: `include/xbridge/cpu/x64_instruction_interpreter.hpp`
- Implementation: `src/cpu/x64_instruction_interpreter.cpp`
- Funktionen:
  - `execute_instruction(const DecodedInstruction&, X64CpuState&, GuestMemory&) noexcept` führt eine bereits dekodierte Instruktion aus.
  - `step_x64_instruction(std::span<const std::uint8_t>, X64CpuState&, GuestMemory&) noexcept` dekodiert höchstens eine Instruktion aus einem expliziten Byte-Puffer und führt sie aus; Non-Ok-Dekodierungen bleiben `Ok`-No-ops mit unverändertem RIP.
  - `step_x64_at_rip(X64CpuState&, GuestMemory&) noexcept` liest höchstens 15 Bytes ab `state.rip` aus `GuestMemory` und reicht sie an `step_x64_instruction(...)` weiter. Die 15 ist ein lokales Lese-Limit, keine RIP-relative Guest-Adressierung; kein Status-Übersetzen und kein Run-Loop.

**Aktuelle Einschränkungen (Sprint 8)**
- Keine REX-Präfixe für 8-bit Operanden (keine AH/BH/CH/DH High-Byte Register).
- Keine 8-bit MOV (`0x88`, `0x8a`).
- Keine SIB-Byte Unterstützung.
- Kein Displacement bei Speicheroperanden (außer implicit 0).
- Keine Instruktionsausführung für FLAGS-Bezeichnungen.
- Kein SIMD/FPU.
- Keine privilegierten Instruktionen.
- Keine Xbox-spezifischen CPU-Eigenschaften.
- Kein JIT.
- Es werden keine Exceptions für unbekannte Opcodes geworfen.

### Address-space seam (additive, reviewed)

`documented` — The interpreter additionally exposes non-owning `_via` entry points (`execute_instruction_via`, `step_x64_instruction_via`, `step_x64_at_rip_via`) through `IGuestVirtualMemory` / `GuestAddressSpaceView` / `GuestMemoryView`. These preserve every existing signature and `X64ExecuteStatus`; memory failures still map to `MemoryAccessError`, with the originating `MemoryResult` retained in `X64ExecutionResult`. Fetch is bounded to 15 bytes, guest addresses are not narrowed, and the interface is generic — it does not model Xbox virtual memory. `TestCpu`, `GuestMemory`, `ExecutionPolicy`, CMake and presets are unchanged; No ADR-0012 is currently recorded in `docs/DECISIONS.md`; references to ADR-0012 identify an unresolved decision placeholder, not an accepted decision. A new ADR and any change to the CPU execution strategy require explicit owner approval. Design, acceptance matrix and reviewer evidence are in `docs/architecture/CPU_ADDRESS_SPACE_INTEGRATION.md` and `docs/AGENT_TASKS.md` (`XB-NEXT-DEVELOPMENT-02-C/D`). No JIT or device dispatch was added.

### TestCpu state and execution contract

`TestCpu` is a synthetic test ISA, not an Xbox or x86-64 CPU. `load_program()`
resets its registers, instruction pointer, halt state, and loaded-program
extent before copying bytes to guest address zero. Bytes in `GuestMemory`
beyond the copied program are not cleared, but instruction fetch is bounded
to the currently loaded program, so stale trailing bytes are not executed.
An empty program can be loaded; attempting to run it throws
`std::out_of_range`. If loading fails, the CPU remains reset with no loaded
program.

Instruction fetch advances the instruction pointer only after each byte is
read successfully. 32-bit operands are fetched little-endian, one byte at a
time; a truncated instruction throws `std::out_of_range`, and the pointer
reflects bytes already fetched. Unknown opcodes, invalid register indices,
and an exhausted instruction limit throw `std::runtime_error`. A `Store` that
exceeds GuestMemory bounds propagates `std::out_of_range`; GuestMemory checks
the full write before modifying bytes.

The instruction limit is checked before each instruction fetch. A zero limit
fails without executing an instruction; a program that executes `Halt` within
the limit completes. Errors do not set the halt flag or roll back already
consumed bytes or completed instructions. A later `run()` starts from the
current state with a fresh per-call limit; there is no automatic error
recovery. `Halt` advances past its opcode and makes subsequent `run()` calls
no-ops.

For host CPU selection, `Automatic` and `Interpreter` select the interpreter.
`Jit` is selected only when explicitly requested and host capabilities report it
available; otherwise the policy falls back to the interpreter and explains why
in its report. This is a selection rule, not a JIT implementation. Any future
JIT requires its own technical justification and tests.

## GPU strategy

Build a graphics research harness before implementing a guest command processor. Keep guest command decoding, shader IR, resource tracking, and host backend separate. Do not assume PC D3D12 is equivalent to Xbox D3D12.X.

## Unknowns register

Every undocumented behavior must be tagged:
- `documented`: supported by an authoritative public specification;
- `observed`: reproducibly observed in a lawful test environment;
- `hypothesis`: plausible but unverified;
- `unknown`: not yet understood.

No `hypothesis` or `unknown` item may be treated as implemented behavior.

## Host hardware detection interface requirements

The repository now contains a deterministic, host-only seam: `HostCapabilities` is treated as immutable by this seam; `ICapabilityProvider` exposes it read-only; `FixedCapabilityProvider` stores a supplied set for deterministic tests; `select_execution_plan(provider, settings)` forwards to the pure `HostCapabilities` overload without platform probing or initialization. A future platform-specific detector may implement the same seam, but real OS/API probing is not implemented and remains a separate owner decision. `BackendSelector` remains a design concept; current selection is `select_execution_plan(...)`. Detection must not be mixed into guest CPU/GPU emulation code.

Suggested initial profiles: `low`, `balanced`, and `high`, plus `custom`. Profiles are policy presets, not hard hardware gates. The UI/CLI must allow users to inspect and override safe settings. Automatic mode should remain the default within the hybrid design.

## Public repository and product packaging clarification

The XBridge source repository is public. Project-related AI/agent configuration, prompts, workflow documentation, research and development automation may be maintained and published in this repository. Their exclusion from a future end-user product package is a packaging rule, not a repository-privacy rule. Only private personal data and actual secrets/credentials are excluded from publication. This owner policy supersedes any wording in this document that describes the repository as private or says these project materials must not be published.
