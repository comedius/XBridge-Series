# XBridge Series

**Version:** 0.3.0  
**Status:** Early development scaffold  
**Target host:** Windows x64 (cross-platform development builds are useful)  
**Language:** C++20

XBridge Series is an experimental, modular research project exploring the architecture required for Xbox Series X software compatibility and, as a long-term research objective, system emulation. Xbox One, Xbox 360, and original Xbox compatibility are optional future research modules, not current targets.

## Important scope statement

This repository is **not an Xbox emulator** and does not currently execute Xbox binaries, load Xbox firmware, emulate AMD Zen 2 or RDNA 2, or implement Xbox system APIs.

The 0.1.0 MVP provides a compilable host application, an Xbox Series X target profile, a bounded guest-memory abstraction, a tiny purpose-built test instruction set (`TestCpu`), and dependency-free automated tests. The test instruction set is scaffolding for testing CPU-engine interfaces; it is not x86-64 and must never be described as Xbox CPU support.

The project will use lawful, documented interfaces and independently created test programs. Do not add proprietary firmware, keys, game files, or circumvention functionality to this repository.

## Current MVP features

- CMake-based C++20 build.
- `xbridge` command-line executable.
- Xbox Series X target metadata (illustrative, not a verified hardware model).
- Bounded guest-memory abstraction with checked reads/writes.
- Tiny test CPU with `LOAD_IMM`, `ADD_IMM`, `STORE`, and `HALT`.
- Automated unit/integration smoke tests using only the C++ standard library.

## Host hardware strategy

XBridge uses a **hybrid, stability-first adaptive execution policy**. It detects host capabilities, chooses supported hardware acceleration where appropriate, and retains software/reference paths where feasible. Users can override automatic choices and select `low`, `balanced`, `high`, or `custom` host-performance profiles.

The goal is the most stable and consistent execution, not maximum average FPS at any cost. Correctness and deterministic synchronization take priority, followed by stability and consistent frame times, then throughput and optional visual quality. Runtime adaptation may tune host-only settings such as worker counts, cache budgets, asynchronous compilation, resource residency, and presentation quality. It must not silently change guest-visible CPU/GPU behavior, memory layout, timing assumptions, or API results. Backend changes require safe synchronization points.

The host CPU, RAM, GPU, VRAM, drivers, and storage affect implementation and resource budgets, but do not redefine the emulated Xbox Series X. Guest memory remains logically separate from host RAM/VRAM. Graphics research compares Vulkan and Direct3D 12, with a software/reference path for explicitly supported operations. Vulkan is a host graphics API, not a hardware emulator.

**Implementation status:** version 0.3.0 includes a deterministic policy selector and mock-capability tests. Real Windows hardware detection, Vulkan/D3D12 initialization, a software renderer, JIT execution, telemetry, and live adaptation are not yet implemented. Software fallback may be too slow for real-time gameplay and does not guarantee universal compatibility.

## Requirements

- CMake 3.20+
- C++20 compiler:
  - Visual Studio 2022 (MSVC) with Desktop development with C++
  - or recent Clang/GCC
- Ninja is optional.

## Build on Windows (Visual Studio)

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug
ctest --test-dir build/windows-debug --output-on-failure
.\build\windows-debug\Debug\xbridge.exe --profile series_x
```

For a single-configuration generator such as Ninja:

```powershell
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug
.\build\ninja-debug\xbridge.exe --profile series_x
```

## Example output

The application prints the selected *emulation target profile* and runs a small synthetic CPU test program. Profile values are explicitly illustrative configuration metadata, not a complete or verified hardware model.

## Repository map

```text
include/xbridge/     Public interfaces (including host policy)
src/app/             CLI entry point
src/core/            Configuration and runtime
src/host/            Host execution policy
src/cpu/             Test CPU (not x86-64)
src/memory/          Guest-memory abstraction
src/profiles/        Xbox Series X profile metadata
tests/               Dependency-free tests
docs/                Architecture decisions and research notes
ai/                  Agent roles, task templates and guardrails
```

## Development rules

1. Keep modules behind explicit interfaces.
2. Do not claim support for undocumented hardware behavior.
3. Record unknowns as research questions, not assumptions.
4. Every behavior change must include tests.
5. Do not add proprietary firmware, keys, or copyrighted game assets.
6. Do not implement security bypasses or circumvention features.
7. Keep the test CPU clearly separate from any future x86-64 execution engine.

## Roadmap

- [x] 0.1.0: buildable host scaffold, profiles, memory, synthetic test CPU
- [x] 0.2.0: Series-X-only scope and hybrid host hardware policy documented
- [x] 0.3.0: deterministic adaptive execution-policy scaffold and mock-capability tests
- [ ] 0.4.0: real Windows host capability discovery and backend initialization
- [ ] 0.5.0: reference-tested x86-64 interpreter for a documented subset
- [ ] 0.6.0: guest/host memory mapping and synchronization design
- [ ] 0.7.0: Vulkan/D3D12 graphics research harness and shader test corpus
- [ ] Later: telemetry-driven adaptation, JIT, documented API compatibility experiments
- [ ] Later: documented API compatibility experiments and progressively broader system research

A commercial-game compatibility milestone is not scheduled until executable formats, system interfaces, graphics behavior, and lawful test inputs are sufficiently understood.

## License

The project is initially marked `UNLICENSED` in package metadata until maintainers choose and review a license. Do not assume third-party code may be copied into this repository.
