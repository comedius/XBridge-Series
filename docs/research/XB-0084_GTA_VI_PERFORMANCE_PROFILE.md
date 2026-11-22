# XB-0084 — GTA VI performance profile (methodology only)

- **PACKAGE_ID:** XB-0084
- **PACKAGE_TYPE:** RESEARCH
- **PRODUCT_BOUNDARY:** development-only
- **ACCESS DATE:** 2026-11-22
- **STATUS:** assessment/planning only; no implementation approved.
- **Evidence labels:** `documented` (explicit in an authoritative source or accepted project document), `observed` (directly seen in code, test output, or a reproducible local check), `hypothesis` (plausible but unverified), `unknown` (not established).
- **Boundary:** Development-only research document. Not product code. No performance measurement, FPS result, resolution target, memory-size claim, game compatibility claim, or implementation authorization is claimed.

## Scope and authority

- `documented` — “Performance-first” is an XB-0084 project directive, not a documented GTA VI requirement. `docs/AGENT_TASKS.md:471`, `:478`, `:487`.
- `documented` — Project priority order is correctness, stability, consistent frame times, performance, then graphics quality. `AGENTS.md:7-12`.
- `documented` — Public official evidence establishes GTA VI availability on Xbox Series X|S and native Series X play, but no authoritative public Series X performance requirement was established. Xbox Wire `https://news.xbox.com/en-us/2026/11/10/grand-theft-auto-6-comes-to-xbox-cloud-gaming-november-19/amp` (access `2026-11-22`, hash not exposed); Rockstar Newswire `https://www.rockstargames.com/newswire/article/39akk424a9oo53/grand-theft-auto-vi-now-available` (access `2026-11-22`, hash not exposed); Xbox Store `https://www.xbox.com/en-US/games/store/grand-theft-auto-vi/9NNZSNHLR63L` (access `2026-11-22`, hash not exposed).
- `unknown` — No authoritative public GTA VI Series X value for FPS, resolution, memory footprint, CPU/GPU workload, API surface, shader format, storage requirement or system-service inventory was found. Official-domain searches for `fps`, `frame rate`, `resolution`, `performance mode` and Series X technical requirements returned availability/pre-order/cloud pages only.
- `documented` — Cloud “Xbox Series S graphics settings” applies to cloud streaming only and must not be transferred to native Series X. Xbox Wire source above; companion document `docs/research/XB-0084_GTA_VI_COMPATIBILITY_INVENTORY.md`.
- `observed` — Metric definitions and measurability findings below are attributed to the tester handoff returned to the coordinator (`ses_dfb8aabfbffezkAi7jlMKnothv`). That agent read the cited local files directly; this researcher did not re-run its commands.

## Goal-only statement

- `documented` (owner statement for XB-0084) — “At least 30 FPS” is an owner-desired future development target within XB-0084 planning only. It is expressly not a documented GTA VI Series X title requirement (that remains `unknown`), not an XBridge capability claim (achievability is `hypothesis`) and not a measured performance result (`unknown`; XBridge has no frame loop, backend, workload or telemetry). No authoritative owner/project source establishes it as an accepted title requirement.
- `documented` — The relation `30 FPS <=> ~33.3 ms average presented-frame interval` is an arithmetic identity/definition, not a measurement, not a title requirement, and not evidence that XBridge can reach that rate.
- `unknown` — XBridge has no present frame loop, backend, game workload or telemetry; no FPS value is currently reportable.
- `documented` — Any future claim that XBridge achieves an FPS target would require actual pre-registered measurement on a lawful, reproducible workload. `docs/AGENT_TASKS.md:487`.

## Pre-registration rules

Before any future performance run, the exact protocol must be fixed in writing and preserved with the result:

1. `documented` — Identify the exact XBridge revision, build configuration, compiler/runtime versions, host OS, CPU, GPU and driver versions. Host facts describe the test platform; they never redefine guest semantics. `AGENTS.md:19-25`; `docs/ARCHITECTURE.md:15-24`.
2. `documented` — Identify the lawful workload and its provenance. No game binary, dump, firmware, key, decrypted DLL, protected asset, shader asset or publisher service endpoint may be used. `AGENTS.md:39-45`; `docs/LEGAL_COMPLIANCE.md:28-49`; `docs/AGENT_TASKS.md:488`.
3. `documented` — Define the frame boundary and the single stable frame start/present hook before sampling. Without a defined presented-frame interval, FPS and frame-time metrics are `unknown`.
4. `documented` — Pre-register warm-up count, sample count `N`, number of repetitions, seeds, replay inputs, sampling duration, exclusion criteria and how aborted/incomplete runs are reported.
5. `documented` — Pre-register the percentile algorithm, histogram bins and whether “1% low” means the 99th-percentile frame time or the mean of the slowest 1%. Mixing definitions after data collection is prohibited.
6. `documented` — Pre-register the host-refresh target interval, deadline definition and stutter threshold (declared multiple of a running median or missed present deadline).
7. `documented` — Pre-register instrumentation overhead limits, counter reset points, cache telemetry behavior and whether cache/event counters are side-effect-free. A metric not pre-registered cannot be reported as a pass criterion.
8. `documented` — Report raw distributions and counts together with summaries. Translation counts, pass counts and generated artifact counts are not capability or performance proxies. `docs/research/RESEARCH-007_WORKFLOW_ANALYSIS.md:67-69`.
9. `documented` — Treat all unmeasured values as `unknown`; never convert `hypothesis` into a measured result.

## Metric definitions and current measurability

`observed` — All metrics below are currently not reportable as XBridge performance results: no GTA VI workload, executable loader, graphics backend, frame loop, cross-subsystem timeline or structured telemetry exists. Supporting scaffold facts are in the architect, developer and tester handoffs and in `docs/research/XB-0084_GTA_VI_COMPATIBILITY_INVENTORY.md`.

| Metric | Pre-registered definition | Why it is unmeasurable today | Required future instrumentation/evidence | Label |
|---|---|---|---|---|
| Average FPS | Mean of presented-frame intervals at the defined frame/presentation boundary; report `N`, warm-up and interval definition. | No frame/presentation boundary or frame loop exists. | Stable present hook, lawful workload, recorded frame intervals. | `unknown` |
| 1% low | 99th-percentile frame time with the percentile algorithm stated; alternative “mean of slowest 1%” only if pre-registered. | No frame-time samples exist. | Frame-time distribution capture with pre-registered percentile method. | `unknown` |
| 0.1% low | 99.9th-percentile frame time with a larger pre-registered `N` and stated percentile algorithm. | No frame-time samples and no pre-registered large `N`. | Larger sample set and percentile capture. | `unknown` |
| Frame-time distribution | Histogram plus p50, p90, p99 and p99.9 frame times. | No frame time source. | Histogram bins and percentiles fixed before sampling. | `unknown` |
| Frame pacing | Jitter about the declared host-refresh target interval plus missed-deadline count. | No host-refresh target binding, present boundary or deadline model. | Frame hook, refresh target record, deadline counter. | `unknown` |
| CPU utilization | Per-thread and process CPU time divided by wall time over the measured interval. | No workload runs; host CPU time would not describe guest CPU work. | Per-thread/process timers, workload identity, guest/host separation. | `unknown` |
| GPU utilization | Backend/vendor busy counters over the measured interval. | No graphics backend or device exists. | Backend-neutral counters and vendor documentation. | `unknown` |
| Memory bandwidth | Bytes moved per unit time at guest-memory/host-copy sites. | Generic `GuestMemory` exists, but no guest/host copy topology or backend exists. | Copy-site counters with unchanged public contracts where possible. | `unknown` |
| Memory latency | Access-to-completion time distribution in nanoseconds. | No timed guest memory pipeline or backend residency path. | Monotonic timestamps at defined access/completion boundaries. | `unknown` |
| Shader compile time | Cold and warm wall time per translation. | No shader translator, lawful corpus or cache. | Translator instrumentation and pre-registered cold/warm protocol. | `unknown` |
| Cache hit rate | `hits / (hits + misses)` plus raw hit, miss, invalidation and eviction counts. | No shader/resource cache exists. | Side-effect-free cache telemetry. | `unknown` |
| Streaming/request-to-ready time | Cold and warm request-to-ready time for declared resources. | No resource or streaming subsystem exists. | Resource request/ready timestamps and cache state record. | `unknown` |
| Audio latency | Source-event to audio-callback delta. | No audio backend, clock or callback boundary exists. | ADR-defined audio clock and timestamp injection. | `unknown` |
| Input latency | Injection-to-consumer-visible delta. | No input backend or consumer boundary exists. | ADR-defined input path and seeded timestamp injection. | `unknown` |
| Stutter | Rate of intervals exceeding a declared multiple of a running median, or missed present deadlines, using the pre-registered threshold. | No frame intervals, present deadlines or threshold. | Frame boundary, deadline counter, threshold rule. | `unknown` |
| Long-run stability | Uptime to failure plus memory growth slope plus counter drift over a pre-registered interval. | No long-running guest workload, failure model or telemetry. | Stability run protocol, counters, failure log and memory samples. | `unknown` |

## Instrumentation requirements before any measurement

- `hypothesis` — Add opt-in monotonic timestamps only at subsystem boundaries; timestamps must never feed guest timing or change guest semantics.
- `hypothesis` — Provide exactly one stable frame start/present hook so frame intervals have one definition.
- `hypothesis` — Use seeded deterministic replay for input and scheduling decisions.
- `hypothesis` — Keep cache telemetry side-effect-free; record raw counts even when a rate is reported.
- `hypothesis` — Keep `HostCapabilities` mockable and immutable for repeatable policy tests. `docs/ARCHITECTURE.md:229-231`.
- `documented` — Preserve existing public interfaces during any future instrumentation: decoder/interpreter status contracts, `GuestMemory` generic API, host-only capability struct and pure execution selector. Developer handoff `ses_dfb8af93bffeqPGOh2dL4DmGmO`.
- `documented` — Host wall-clock around `cpu.run()`, an executed-instruction counter and access counters are possible host-side instrumentation points, but none are present as reportable telemetry. Tester and developer handoffs.

## Reporting format for a future run

A future report must include, at minimum: exact revision and configuration; workload provenance; pre-registration record; warm-up and sample counts; seeds; raw metric files; percentile algorithm; exclusions and failures; host platform facts; guest/host separation statement; confidence or uncertainty statement; and an explicit `documented`/`observed`/`hypothesis`/`unknown` label for every claim.

If any pre-registered condition is not met, the affected metric remains `unknown`; it must not be reported as a pass, improvement or title capability.

## Deliberate omissions

- `unknown` — No FPS, resolution, memory-size, CPU/GPU utilization, latency or stability value is asserted.
- `documented` — No PS5, PC, cloud or other-console number is transferred to native Xbox Series X.
- `documented` — No optimization gain is claimed.
- `documented` — No implementation, benchmark run, build or CTest verification is performed by XB-0084.
