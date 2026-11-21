# Adaptive execution and stability policy

## Objective

Select the most stable supported combination of host hardware and software implementations while preserving one fixed guest target: Xbox Series X. Host adaptation must never silently redefine guest-visible CPU, GPU, memory, timing, or system behavior.

## Selection model

- Discover host facts in a platform-specific `HostCapabilities` provider.
- Keep discovery separate from policy and from guest emulation.
- `select_execution_plan` is a deterministic, side-effect-free policy function.
- Automatic graphics selection prefers an available accelerated backend (Vulkan, then Direct3D 12), then the software/reference path when permitted.
- CPU selection uses the interpreter as the conservative reference path and selects JIT only when available and permitted.
- Manual preferences override automatic defaults when the requested backend is available. Unsupported requests use the configured software fallback or report `Unavailable`.
- The selected plan includes a human-readable diagnostic report.

## Stability-first runtime policy

1. Correct guest-visible behavior takes priority over throughput.
2. Stability and deterministic synchronization take priority over peak average FPS.
3. Track frame-time percentiles, missed presentation deadlines, shader compilation stalls, memory pressure, audio underruns, and backend/device errors.
4. Adapt host-only controls first: worker counts, cache budgets, asynchronous compilation, resource residency, and presentation resolution/quality when permitted.
5. Do not migrate in-flight GPU work or change a backend mid-command. Backend changes require a safe synchronization point and resource recreation.
6. Accuracy-affecting shortcuts must be explicit, opt-in, and reported; never silently trade compatibility for speed.
7. Software fallback is feature-specific and may be too slow for real-time play. It is a correctness/debugging path unless measured otherwise.

## Performance profiles

- `low`: conservative host resource budgets and reduced presentation workload where safe.
- `balanced`: default compromise.
- `high`: larger host-side caches and more parallel work where measurements justify it.
- `custom`: user-defined host-side policy.

Profiles are hints, not minimum/recommended hardware guarantees. They may not alter guest memory capacity or guest instruction/API semantics.

## Current implementation boundary (0.3.0)

The repository contains a small, deterministic policy selector and unit tests using synthetic capability sets. It does **not** yet detect real Windows hardware, initialize Vulkan/Direct3D 12, implement a software graphics renderer, execute x86-64, or adapt a running game. Those are later milestones.
