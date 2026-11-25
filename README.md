# XBridge Series

XBridge is an experimental C++20 project investigating the architecture and engineering foundations required for an Xbox Series X system emulator on Windows x64.

**Current status: early foundation / research scaffold. It is not a working Xbox emulator and does not currently run Xbox games or Xbox executables.**

## What exists today

- CMake-based C++20 project with a CLI application and CTest test target.
- Generic guest-memory scaffold and a synthetic `TestCpu` used to exercise basic execution and memory interfaces.
- Separate x64 CPU-state model, a limited instruction decoder, and a reference interpreter for a small documented subset.
- Host capability and execution-policy scaffolding.

These components are foundations and test models. They do not establish Xbox hardware compatibility.

## Scope and constraints

- Current target: Xbox Series X only. Series S is not a separate target.
- Interpreter-first CPU development; JIT is excluded from the current baseline.
- Xbox 360, Xbox One and original Xbox compatibility are deferred, separate future considerations.
- No proprietary firmware, encryption keys, commercial game binaries, protected assets, or DRM-circumvention functionality are included.
- No compatibility, game-playability, or performance claims are made.

## Build

Requirements: Windows x64, a C++20 toolchain, CMake, and either Visual Studio 2026 (the `windows-debug` preset) or Ninja (the `ninja-debug` preset).

Visual Studio:

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug --config Debug
ctest --test-dir build/windows-debug -C Debug --output-on-failure
```

Ninja:

```powershell
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

The repository currently defines one CTest test target, `xbridge_tests`. A successful scaffold test run is not evidence of Xbox compatibility.

## Project documentation

- [Architecture](docs/ARCHITECTURE.md)
- [Roadmap](docs/ROADMAP.md)
- [Architecture decisions](docs/DECISIONS.md)
- [Research register](docs/RESEARCH_REGISTER.md)
- [Legal and compliance constraints](docs/LEGAL_COMPLIANCE.md)
- [Development and agent workflow](docs/AGENT_WORKFLOW.md)

## Development infrastructure

This public repository may contain project-related development documentation, AI-agent prompts, OpenCode configuration, and automation. These files support development; they are not automatically part of a future end-user product distribution. Product packaging will use an explicit, separately reviewed allowlist.

## License

The repository currently declares `UNLICENSED` in [LICENSE](LICENSE). No open-source reuse or redistribution permission is granted by that file. A different license requires an explicit project-owner decision and legal review.
