# XB-0084 — Risk and dependency overview

- **PACKAGE_ID:** XB-0084
- **PACKAGE_TYPE:** RESEARCH
- **PRODUCT_BOUNDARY:** development-only
- **ACCESS DATE:** 2026-11-22
- **STATUS:** assessment/planning only; no implementation approved.
- **Evidence labels:** `documented` (explicit in an authoritative source or accepted project document), `observed` (directly seen in code, test output, or a reproducible local check), `hypothesis` (plausible but unverified), `unknown` (not established).
- **Boundary:** Development-only research document. Not product code. No compatibility, performance, legal or implementation conclusion is approved by this document.

## 1. Authority and current target boundary

- `documented` — XB-0084 is assessment/planning only; implementation is not approved. `docs/AGENT_TASKS.md:469-497`, especially `:477`, `:488`, `:494`.
- `documented` — Xbox Series X is the sole current target; Xbox One, Xbox 360 and original Xbox compatibility are deferred, optional and isolated future modules. `docs/ARCHITECTURE.md:11-13`; `docs/DECISIONS.md:18-21`.
- `documented` — Correctness, stability and consistent frame times precede performance and graphics quality. `AGENTS.md:7-12`.
- `documented` — Unknown and hypothesis findings must never be treated as implemented behavior. `docs/ARCHITECTURE.md:219-227`; `docs/RESEARCH_REGISTER.md:16-22`.
- `observed` — Local scaffold facts were supplied by architect, developer and tester handoffs returned to the coordinator (`ses_dfb8b2c84ffed9nKKaN1zVSUCM`, `ses_dfb8af93bffeqPGOh2dL4DmGmO`, `ses_dfb8aabfbffezkAi7jlMKnothv`); this researcher did not re-run their commands.

## 2. Research-register dependencies

| ID | Topic | Status in register | Next action in register | Label |
|---|---|---|---|---|
| R-001 | Publicly documented Xbox Series CPU-visible behavior | `unknown` | Review authoritative public documentation. `docs/RESEARCH_REGISTER.md:7` | `documented` |
| R-002 | Xbox-specific graphics command interfaces | `unknown` | Map public GDK documentation and gaps. `:8` | `documented` |
| R-003 | Shader input representation and semantics | `unknown` | Build lawful shader research corpus. `:9` | `documented` |
| R-004 | Guest virtual-memory behavior | `unknown` | Specify observable requirements from public sources. `:10` | `documented` |
| R-005 | Storage/decompression APIs | `partial` | Map documented APIs and host equivalents. `:11` | `documented` |
| R-006 | System services required by test software | `unknown` | Start from public GDK APIs. `:12` | `documented` |
| RESEARCH-007 | Public Xbox Series X\|S emulator architecture/workflow concepts | Complete as evidence record; unresolved items remain `unknown` | Revisit unresolved license/legal, D3D12.X, rendered-pixel correctness and installed-role items before adoption. `:13` | `documented` + `unknown` |

- `documented` — These register items are dependencies for the fixed twelve GTA VI component areas, not evidence that XBridge supports those areas. `docs/research/XB-0084_GTA_VI_COMPATIBILITY_INVENTORY.md`.

## 3. Risk register

| Risk ID | Risk | Evidence label | Impact | Required mitigation/gate | Current status |
|---|---|---|---|---|---|
| RK-01 | GTA VI game-internal requirements are unknown; no authoritative public Series X performance, memory, API, shader, storage, audio or input profile was established. | `unknown` | Any title-specific compatibility or performance claim would be fabricated. | Keep game requirements `unknown`; use only official availability evidence W09–W12; require separate lawful evidence before claims. | Open |
| RK-02 | Official GTA VI searches returned marketing/availability pages, not technical specifications; Rockstar Support GTA VI pages timed out. | `observed` for search/fetch results; values `unknown` | No baseline for “performance-first” targets. | Pre-register methodology; do not invent FPS/resolution/memory values. `XB-0084_GTA_VI_PERFORMANCE_PROFILE.md`. | Open |
| RK-03 | RESEARCH-007 primary license is GPL-current with earlier MIT/Apache releases; upstream has no detected license at examined HEAD. | `observed`; legal effect `unknown` | Copying code/text/artifacts could violate project rules or third-party rights. | No adoption without owner/legal review; exact commit/file/dependency audit. `XB-0084_RESEARCH_LICENSE_ASSESSMENT.md`. | Open |
| RK-04 | Third-party credits in RESEARCH-007 and PS5 references were not independently audited. | `unknown` | Hidden incompatible or unattributed dependencies. | File-level provenance and transitive license review before adoption. | Open |
| RK-05 | XBridge is `UNLICENSED` and grants no permission. | `documented` (`docs/LEGAL_COMPLIANCE.md:61-69`) | Source publication/binary distribution rights cannot be assumed. | Clarify project license with review/legal advice before publication. | Open |
| RK-06 | Two distinct Splinter Cell: Double Agent SKUs exist: original Xbox `BWV6WF6XMFZX` and Xbox 360 `BPQ8RMCDSLRL`. | `documented` (W05/W08) | Evidence from the wrong SKU could justify false compatibility claims. | Keep original-Xbox evidence separate from the Xbox 360 target; use `XB-0084_XBOX360_SPLINTER_CELL_ANALYSIS.md`. | Open |
| RK-07 | Xbox 360 is not the current target and has no local module/profile. | `documented` (`docs/ARCHITECTURE.md:13`; `docs/DECISIONS.md:18-21`) | Scope creep into unsupported guest semantics. | Separate ADR/module/evidence/tests required; no compatibility transfer from Series X. | Open |
| RK-08 | Current x64 decoder/interpreter is limited to NOP and MOV forms; no flags/SIMD/FPU/control-flow corpus or integrated execute loop. | `observed` (architect/developer handoffs) | CPU correctness cannot support game compatibility. | Interpreter-first expansion with conformance corpus; no JIT/native execution. | Open |
| RK-09 | `GuestMemory` is generic and lacks Xbox virtual-memory semantics. | `documented` + `observed` (`docs/ARCHITECTURE.md:30-39`) | Silent semantic corruption if Xbox VM rules are assumed. | Separate guest VM contract; preserve generic API until approved. | Open |
| RK-10 | D3D12.X equivalence, shader formats and rendered-pixel correctness are unknown. | `unknown` | Fake graphics compatibility or wrong rendering. | Backend-neutral ADRs, synthetic lawful corpus and correctness tests before implementation. | Open |
| RK-11 | Host CPU/GPU facts could redefine guest semantics or trigger silent accuracy reductions. | `documented` (`AGENTS.md:19-25`; `docs/ADAPTIVE_EXECUTION.md:22-24`) | Wrong guest behavior disguised as optimization. | Keep host/guest separation; explicit safe-point policy reports; preserve pure selector contracts. | Open |
| RK-12 | Reserving or pinning all host RAM/VRAM for guest memory. | `documented` (`AGENTS.md:25`) | Host instability and poor fallback behavior. | Dynamic host resource management; controlled errors/degradation. | Open |
| RK-13 | Mid-operation execution-path switching. | `documented` (`docs/HOST_HARDWARE_POLICY.md:20-28`; `docs/ADAPTIVE_EXECUTION.md:17-25`) | Nondeterministic state and race conditions. | Safe-point-only changes; deterministic replay tests. | Open |
| RK-14 | Performance claims without pre-registered measurement. | `documented` (`XB-0084_GTA_VI_PERFORMANCE_PROFILE.md`) | False success and misleading project status. | Keep metric values `unknown`; record exact workload/revision/environment/sample counts. | Open |
| RK-15 | Translation counts, pass counts or generated artifact counts used as capability proxies. | `documented` (`docs/research/RESEARCH-007_WORKFLOW_ANALYSIS.md:67-69`) | Appearance of progress without correctness. | Report raw counts plus correctness tests; never equate count with playability. | Open |
| RK-16 | Prohibited material or protected content introduced during research. | `documented` (`AGENTS.md:39-45`; `docs/LEGAL_COMPLIANCE.md:28-49`) | Legal, security and project-boundary violation. | No dumps, firmware, keys, game files, DRM circumvention or publisher service endpoints. | Open |
| RK-17 | Installation of planned specialist roles is not executable today. | `documented` (`docs/AGENT_WORKFLOW.md`) | Plans that assume unavailable owners cannot execute. | Use installed roles and coordinator envelopes; no plan depends on uninstalled specialists. | Open |

## 4. Dependency graph

`hypothesis` — Dependency-safe path; arrows mean “must be resolved before”:

```text
XB-0084 six evidence documents
  -> owner acceptance of assessment
    -> ADR/decision register (license, pacing, timeline, lifetime, cache, streaming, audio, scheduling, optional 360 scope)
      -> interpreter-first CPU corpus
      -> generic memory hardening
      -> real host detection + deterministic telemetry
        -> backend-neutral graphics/shader/resource contracts
          -> synthetic graphics/resource prototypes
          -> evidenced system/storage/audio/input designs
            -> safe-point CPU/GPU/audio integration
              -> pre-registered performance measurement
                -> optional, separate Xbox 360 module planning
```

- `documented` — The graph does not authorize implementation; each arrow must become a separate approved package.
- `hypothesis` — Starting with game playability, FPS proof, D3D12.x implementation or Xbox 360 support skips required evidence and is therefore not a valid shortcut.
- `documented` — FINAL_CLEANUP is not in scope and requires feature-complete status plus explicit owner approval. `AGENTS.md:135-141`.

## 5. Evidence-source dependency summary

| Evidence group | Usable for | Not usable for | Label |
|---|---|---|---|
| W01–W03 Microsoft Xbox 360 CPU/memory docs | Xbox 360 architecture only | Xbox Series X CPU/memory claims | `documented` |
| W04–W08 official Xbox/Store backward-compatibility sources | Availability and SKU identity only | Emulator internals or title runtime behavior | `documented`; internals `unknown` |
| W09–W12 official GTA VI availability sources | Platform availability and native/cloud messaging | FPS, resolution, memory, API or workload requirements | `documented`; technical details `unknown` |
| RESEARCH-007 project-authored records | Workflow concepts and evidence-discipline ideas | Authoritative Xbox facts or adoptable code without license review | `documented` project-authored; platform facts `unknown` |
| PS5 reference project analysis | Separation principles, test/diagnostic ideas and license warnings | Xbox ABI, GPU, shader, memory or system facts | `documented`/`hypothesis`; Xbox facts `unknown` |
| Local scaffold and tests | Current implementation status only | Game compatibility or performance claims | `observed` |

### W09–W12 evidence register

`documented` — W09–W12 below define the identifiers used in this file. Access date for each: `2026-11-22`; hash not exposed for each. These sources support availability/native-platform messaging only; all technical details are `unknown`.

| ID | Source | Exact scope | Label | Reproduction |
|---|---|---|---|---|
| W09 | `https://news.xbox.com/en-us/2026/11/10/grand-theft-auto-6-comes-to-xbox-cloud-gaming-november-19/amp` | Xbox Wire GTA VI cloud/launch article: launch date, day-one cloud play on Xbox One and Xbox Series X\|S, native Series X messaging, and cloud-only Series S graphics settings. | `documented` for availability/native-platform messaging; technical details `unknown` | Fetch URL; find launch, cloud and native Series X lines. |
| W10 | `https://news.xbox.com/en-us/2026/11/19/grand-theft-auto-vi-is-out-now/amp` | Xbox Wire article title “Grand Theft Auto VI is Out Now”; no technical body text exposed in the earlier fetch. | `documented` for title only; technical details `unknown` | Fetch URL; read page title. |
| W11 | `https://www.rockstargames.com/newswire/article/39akk424a9oo53/grand-theft-auto-vi-now-available` | Rockstar Newswire official search snippet: GTA VI now available worldwide for PlayStation 5 and Xbox Series X\|S. | `documented` for official snippet; technical details `unknown` | Official-domain search for “Grand Theft Auto VI Now Available”; open result. |
| W12 | `https://www.xbox.com/en-US/games/store/grand-theft-auto-vi/9NNZSNHLR63L` | Xbox Store official-domain search snippet: "Pre-Order Grand Theft Auto VI Now. Coming to XBOX Series X\|S November 19, 2026." The direct page body fetch returned HTTP 200 but did not expose that exact date string. | `documented` for official snippet; body-level date `unknown`; technical details `unknown` | Official-domain search for product `9NNZSNHLR63L`; open result and note body-fetch limitation. |

## 6. Performance-claim risk boundary

- `documented` (owner statement for XB-0084) — “At least 30 FPS” is an owner-desired XB-0084 development target only. `30 FPS <=> ~33.3 ms average presented-frame interval` is an arithmetic definition, not a measurement. GTA VI’s Series X FPS requirement and XBridge’s achievable FPS are both `unknown` (achievability is `hypothesis`). `XB-0084_GTA_VI_PERFORMANCE_PROFILE.md`.
- `unknown` — XBridge currently has no frame loop, backend, lawful game workload or telemetry; no FPS value is reportable.
- `documented` — Cloud “Xbox Series S graphics settings” applies to cloud streaming only and must not be transferred to native Series X. W09.
- `documented` — Any future measurement must pre-register warm-up, `N`, seeds, percentile algorithm, frame boundary, refresh target, stutter threshold, exclusions and instrumentation overhead.
- `documented` — If a pre-registered condition is not met, the metric remains `unknown`.

## 7. Open owner decisions

The following are unresolved decision dependencies, not questions answered by this document:

1. Accept or reject the XB-0084 assessment as the evidence baseline.
2. Choose the XBridge project license and approve any third-party adoption policy.
3. Accept ADRs for frame pacing, GPU timeline, resource lifetime, shader cache keys/invalidation, streaming/residency, audio clock and deterministic scheduling.
4. Decide whether any native execution/JIT/relinker work may ever be investigated; current rules prohibit it without separate evidence, design, tests and approval.
5. Decide whether an optional Xbox 360 module is ever in scope and define its own ADR/evidence/test boundary.
6. Decide whether future performance results may be published and under what pre-registered protocol.
7. Decide whether any FINAL_CLEANUP activity is appropriate after feature-complete status; not part of XB-0084.

## 8. Unresolved unknowns carried forward

- `unknown` — All GTA VI Series X internal technical requirements.
- `unknown` — GTA VI Series X FPS, resolution, memory and workload values.
- `unknown` — Rockstar Support installation-requirement content; pages timed out on `2026-11-22`.
- `unknown` — RESEARCH-007 license compatibility and third-party license audits.
- `unknown` — D3D12.X equivalence, shader-stage formats and rendered-pixel correctness.
- `unknown` — Xbox Series X CPU-visible behavior, guest VM rules, graphics command interfaces and system-service inventory.
- `unknown` — Xbox 360 backward-compatibility emulator internals and target-SKU title behavior.
- `unknown` — Whether any planned specialist role will be installed; current installed-role status is recorded in `docs/AGENT_WORKFLOW.md`.

## 9. Deliberate omissions

- `documented` — No product code, test, build file, preset, workflow file, permission file or roadmap/task ledger was modified by this research write phase.
- `documented` — No build, CTest, commit, push, merge, tag, release or PR was performed.
- `documented` — No compatibility or performance result is marked pass.
