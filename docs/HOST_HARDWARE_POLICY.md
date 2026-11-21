# Host hardware policy

## Fixed guest target

The sole current guest target is Xbox Series X. Host hardware is an implementation resource, not a replacement guest specification. Guest memory and device semantics remain logically fixed and separate from host RAM and VRAM.

## Hybrid management

XBridge uses automatic host capability detection with user-overridable settings. Detection reports CPU features/thread count, available system memory, GPU/API/device features, VRAM where exposed, driver identity, and storage facts where available. Detection is isolated behind a provider interface; policy consumes immutable capability data.

## Hardware/software combination

- Prefer a supported accelerated backend after capability checks and successful initialization.
- Keep software/reference implementations for supported operations, testing, debugging, and fallback.
- CPU interpreter is the correctness baseline; JIT is an optional optimized backend.
- Graphics backends are abstracted from guest command interpretation and shader translation. Vulkan and Direct3D 12 are candidate host APIs; software rendering is a separate backend.
- Audio, storage, and other subsystems should independently choose accelerated/native or software paths where meaningful.
- Fallback is per subsystem. A software graphics path does not imply software emulation of every console subsystem.

## Stability-first priorities

1. Guest-visible correctness.
2. Runtime stability and deterministic synchronization.
3. Consistent frame times and absence of audio underruns.
4. Throughput and average FPS.
5. Optional visual quality enhancements.

The runtime should measure frame-time distribution, missed deadlines, shader compilation stalls, memory pressure, audio underruns, and device errors. It should adapt host-side knobs before considering any accuracy/performance trade-off. Backend switches require a safe synchronization point; never switch while commands or resources are in flight.

## Profiles

`low`, `balanced`, `high`, and `custom` tune host-side budgets and presentation options only. They are not hardware certification tiers and do not change guest memory size or guest-visible behavior. Users can inspect the active plan and override automatic choices.

## Limits and fallback behavior

Software fallbacks may be slower or support fewer features. They are not a promise of playable performance on weak PCs. If no correct backend exists for a required operation, report a clear unsupported-feature error rather than silently approximate guest behavior.

## Current status

The 0.3.0 scaffold includes a deterministic execution-policy selector and tests using mock host capabilities. Real host detection, API initialization, performance telemetry, dynamic runtime adaptation, and concrete rendering backends are not implemented yet.
