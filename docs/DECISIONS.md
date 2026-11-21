# Architecture Decision Records

## ADR-0001: C++20 as primary language
Status: Accepted

C++20 is selected for broad systems-programming support, compiler availability, and interoperability with LLVM and Windows graphics APIs. Rust may be considered for isolated tools later, but is not part of the MVP.

## ADR-0002: CMake build system
Status: Accepted

CMake provides a portable build description and supports Visual Studio and Ninja workflows.

## ADR-0003: Synthetic CPU for MVP
Status: Accepted

A tiny project-defined instruction set validates memory and execution interfaces without implying Xbox or x86-64 compatibility. A real x86-64 interpreter is a separate milestone.

## ADR-0004: Xbox Series X is the sole current target
Status: Accepted

XBridge focuses on Xbox Series X only. Series S is not a separate target. Xbox One, Xbox 360, and original Xbox compatibility may be considered later as isolated, optional modules after the Series X foundation.

## ADR-0005: Hybrid host hardware management
Status: Accepted

XBridge automatically detects host capabilities and selects safe defaults, while exposing user-overridable profiles and advanced settings. Detection/policy is separate from guest emulation. The host's components affect implementation and performance, not the identity or semantics of the emulated Series X.

## ADR-0006: Hardware acceleration with software fallback
Status: Accepted

Use hardware acceleration where supported. Provide software/reference fallbacks where feasible, with explicit feature and performance limitations. Fallback is capability-specific and does not imply that every game or feature will run acceptably.

## ADR-0007: Backend-neutral graphics architecture
Status: Accepted

Keep guest graphics interpretation, resource/synchronization tracking, shader translation, and host graphics APIs behind separate interfaces. Investigate Vulkan and Direct3D 12 as host backends, with an optional CPU software/reference path. Vulkan is a host API, not a hardware emulator.

## ADR-0008: Guest/host memory separation
Status: Accepted

Model guest Series X memory logically and independently from host RAM/VRAM. Allocate and map host resources according to available budgets. Host constraints must not silently change guest-visible memory semantics.

## ADR-0009: No proprietary firmware or game assets
Status: Accepted

The repository contains no proprietary console firmware, keys, game binaries, or copyrighted test assets. Testing starts with independently created programs and public documentation.

## ADR-0010: Stability-first adaptive execution
Status: Accepted

Choose the best supported combination of hardware acceleration and software reference/fallback paths per subsystem. Correctness and stability outrank peak FPS. Runtime adaptation may tune host-only scheduling, cache/resource budgets, and presentation quality at safe synchronization points. It must not silently alter guest-visible semantics. The selector must be deterministic and produce diagnostics.

## ADR-0011: Backend policy is capability-based and user-overridable
Status: Accepted

Automatic mode is the default; users may override graphics and CPU backends and choose low, balanced, high, or custom host-performance profiles. Backend selection is based on discovered capabilities and successful initialization, not GPU brand/model assumptions. If a requested backend is unavailable, use an explicitly permitted fallback or fail clearly.
