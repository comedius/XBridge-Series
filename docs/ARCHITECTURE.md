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
2. Select safe defaults and the best supported acceleration path automatically.
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

## Non-goals for 0.1.0

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

## CPU strategy

The MVP's `TestCpu` is a synthetic instruction set for exercising fetch/decode/execute and memory interfaces. It is not x86-64.

A future x86-64 engine must have:
- an instruction decoder backed by a documented test corpus;
- a reference interpreter before JIT optimization;
- explicit exception and flag semantics;
- tests for each supported instruction form;
- differential tests against a trusted reference where lawful and practical.

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

A future `HostCapabilities` service should return immutable, testable facts (CPU features/core counts, memory budgets, available graphics APIs/extensions/features, device limits, driver identity, and storage information). A separate `BackendSelector` applies policy and returns the selected backend plus a human-readable decision report. Detection must not be mixed into guest CPU/GPU emulation code. Provide deterministic mock capability sets for tests.

Suggested initial profiles: `low`, `balanced`, and `high`, plus `custom`. Profiles are policy presets, not hard hardware gates. The UI/CLI must allow users to inspect and override safe settings. Automatic mode should remain the default within the hybrid design.
