# RESEARCH-007 — Recommendations for XBridge

**PACKAGE_TYPE:** RESEARCH
**PRODUCT_BOUNDARY:** development-only
**Access date for every external source in this package:** 2026-11-22
**Status:** research proposals only. Nothing in this document is an approved implementation package, ADR, interface change or product requirement.
**Evidence labels:** `documented`, `observed`, `hypothesis`, `unknown` as defined in `docs/RESEARCH_REGISTER.md`.
**Classification scale:** same `1`–`5` scale as `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md`.
**Reproduction:** fetch the exact URL in a row and search the quoted phrase; resolve IDs via `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md`. Local files are read from checkout root.

The coordinator owns package creation and assignment. Only six agents are installed; the nine requested specialist roles are `planned`, not installed/freigegeben. Recommendations that mention a specialist owner are therefore conditional and not executable today (`observed`: local `docs/AGENT_WORKFLOW.md`).

---

## 1. Recommended development workflow changes (`1`)

These are documentation/process proposals. They do not change product code, interfaces or dependencies.

| ID | Recommendation | Exact source claim / URL | Source label | Class | Owner/decision | Risk and verification |
|---|---|---|---|---|---|---|
| REC-01 | Add an explicit evidence-status/capability register for Xbox claims: investigated, unsupported, implemented, validated and unknown, with source and reproduction path. Do not let a stub count as success. | “No fake success”: leave unsupported rows `missing` and explain why — https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `documented` | `1` | Coordinator can propose docs-only package; no owner decision needed unless scope changes. | Risk: status inflation. Verify by checking that each status has evidence and no unsupported behavior is marked validated. |
| REC-02 | Keep a current, append-only evidence log and a separate debt/assumptions file; explicitly mark stale design documents rather than silently treating them as current. | Development log is append-only — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DEVELOPMENT_LOG.md ; debt ledger is “Honest ledger. Updated continuously.” — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md ; README says architecture docs lag — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md | `documented` + `observed` | `1` | Coordinator; docs-only. | Risk: duplicated facts. Verify by pointing current log entries to exact commands/results and keeping assumptions labeled. |
| REC-03 | If a future status/progress artifact is desired, generate it from checked-in, public-only machine-readable data and include generator self-checks; do not hand-maintain percentages. | `tools/progress/gen.py` generates map/badges and has selftests — https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py | `observed` | `1` | Owner decision required before adding any new tool/dependency. | Risk: false metrics. Verify generator output, denominator rules and that no proprietary data is consumed. |
| REC-04 | Require a public-evidence expectation before implementing a new guest-visible behavior, and a test that fails when the behavior is missing. Keep boundary/failure tests. | Unit-test requirement — https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md ; XBridge already requires tests/boundaries — local `AGENTS.md` §6 | `documented` | `1` | Installed `tester` assignment; no product change unless approved. The planned `test-engineer` role is not installed. | Risk: test by implementation detail only. Verify test names, exact expectations and CTest results. |
| REC-05 | Preserve artifact gates: no binaries, game/console files, personal data or undisclosed third-party code in packages; record exact build/CTest/diff commands and results. | PR gates — https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md ; XBridge verification rules — local `AGENTS.md` §6 | `documented` | `1` | Coordinator/tester. | Risk: unrecorded failures. Verify by including exact outputs and exit states in handoff. |
| REC-06 | When package work is parallel, require disjoint write sets and an explicit integration check; do not assign installed-agent work to the nine planned specialists. | Integration incident — https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md ; local roles — local `docs/AGENT_WORKFLOW.md` | `observed` | `1` | Coordinator. | Risk: conflicting edits or unexecutable owner names. Verify envelope FILES/ALLOWED write sets and installed role list before assignment. |
| REC-07 | Keep a single source of truth for evidence tags; project-authored external claims must not be promoted to authoritative Xbox facts. | External research tags `[DOC]`, `[PUB]`, `[INF]`, `[HYP]`, `[NDA]` — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md ; XBridge labels — local `docs/RESEARCH_REGISTER.md` | `documented` | `1` | Coordinator/researcher. | Risk: false authority. Verify every table has Label and URL; treat project-authored secondary sources as such. |

---

## 2. Technical ideas that may be independently implemented in C++20 (`2`)

These are pattern-level proposals only. They do not authorize code copying or Xbox-specific behavior claims.

| ID | Recommendation | Exact source claim / URL | Source label | Class | Owner/decision | Risk and verification |
|---|---|---|---|---|---|---|
| REC-08 | Strengthen untrusted-input tests: random/truncated inputs, no-panic/no-crash assertions, and independent comparison against a separate reference tool where one is publicly available. | Upstream PE tooling has a no-panic random/truncated-input sweep and `pe-inspect` was cross-checked with `dumpbin`; `cargo fuzz` was not configured — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DEVELOPMENT_LOG.md ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `documented` + `observed` | `2` | Installed `tester`; product changes only in a separate approved package. The planned `test-engineer` role is not installed. | Risk: platform-dependent oracles. Verify exact tool/version and that tests do not depend on protected inputs. |
| REC-09 | Where a host capability or backend assumption is encoded, pair it with an explicit test and keep runtime verification distinct from static translation/coverage counts. | "Translating is not the same as rendering correctly." — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md ; barrier fix with synthetic test but unverified runtime — https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994 | `documented` + `observed` | `2` | Architect/tester; ADR if interface changes. | Risk: coverage mistaken for correctness. Verify that claims of correctness cite runtime/public evidence, not translation counts. |
| REC-10 | Keep CPU status contracts explicit (`Ok`, error variants, no partial mutation) for any future decoder/interpreter extension; do not adopt JIT or recompilation from external projects. | External native execution model — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DECISIONS.md ; XBridge baseline — local `include/xbridge/cpu/*.hpp`, `docs/DECISIONS.md` | `documented` + `observed` | `2` | Architect plus owner decision for any execution-model change. | Risk: implicit host/guest coupling. Verify public API and ADR constraints remain intact. |

Clarification for REC-10: documenting the existing CPU status contract is class `1` and requires no change; strengthening contracts for a future decoder/interpreter extension is class `2` and requires an approved implementation package, not an ADR merely because it touches CPU.

---

## 3. Items requiring architecture/ADR changes (`3`)

| ID | Recommendation | Exact source claim / URL | Source label | Class | Required decision | Risk and verification |
|---|---|---|---|---|---|---|
| REC-11 | Do not change XBridge to native guest execution, static relinking or import redirection unless a separate ADR, design, interface review and test plan are approved. Keep the interpreter baseline. | ADR-0002 rejects emulation and chooses native execution — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DECISIONS.md ; README says no CPU emulation/recompilation — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md ; XBridge rules — local `docs/DECISIONS.md` | `documented` | `3` | Owner ADR required before any implementation. | Risk: violating accepted architecture and host/guest separation. Verify no loader/relinker code enters product scope without approval. |
| REC-12 | If unified-memory or GPU-visible allocation is ever studied, create a separate public-source evidence model and graphics/backend ADR; do not add it to generic `GuestMemory`. | Unified-memory explanation — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md ; XBridge generic memory — local `include/xbridge/memory/memory.hpp`, `docs/ARCHITECTURE.md` | `documented` (project-authored secondary) + `observed` | `3` | Owner ADR plus architect interface review. | Risk: Xbox-specific semantics leaking into a generic interface. Verify `GuestMemory` remains generic until changed by an approved package. |

---

## 4. Items unsuitable or blocked for XBridge (`4`)

| ID | Recommendation | Exact source claim / URL | Source label | Class | Reason |
|---|---|---|---|---|---|
| REC-13 | Do not adopt console dumps, extracted DLLs, shaders, keys, firmware, game assets or dump-derived binaries into XBridge workflows. | Primary legal boundary — https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/README.md ; upstream legal boundary — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md ; XBridge rules — local `AGENTS.md` | `documented` | `4` | Prohibited by XBridge rules; this research used none. |
| REC-14 | Do not copy Rust source, CI files, generator code or design text from either repository until license compatibility is explicitly resolved. | Primary GPL statement — https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/README.md ; upstream license 404s — https://api.github.com/repos/drycheetah84/TomBox/license ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/LICENSE | `observed` | `4` | Upstream license `unknown`; primary current GPL status conflicts with earlier MIT/Apache releases for adoption purposes. |
| REC-15 | Do not implement D3D12.X wrapping, RDNA2 translation, texture mirroring, local user/store hooks or network redirects from these projects as XBridge features. | External architecture layers — https://github.com/drycheetah84/TomBox ; limitations — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `documented` (project-authored) + `observed` | `4` | Requires game/platform content, a graphics backend and scope changes XBridge does not have. |
| REC-16 | Do not contact publisher production services in research or tests. | Upstream states its own development/testing never connects to publisher production services; network guard was added after analytics traffic — https://github.com/drycheetah84/TomBox ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `documented` + `observed` | `4` | XBridge has no network product surface from this research, and such traffic is outside lawful/lab scope here. |

---

## 5. Items needing further investigation (`5`)

| ID | Recommendation | Exact source claim / URL | Source label | Class | Open question / verification |
|---|---|---|---|---|---|
| REC-17 | Keep D3D12.X equivalence, shader-stage formats, mesh/amplification support and rendered-pixel correctness as `unknown`; investigate only through public authoritative sources. | NDA gap — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md ; no rendered-pixel validation — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `documented` + `observed` | `5` | What public authoritative evidence exists? Until then, do not claim compatibility or equivalence. |
| REC-18 | Treat AVX2/Zen 2/Windows x64 ABI notes as project-authored secondary context, not XBridge requirements, until authoritative public sources are checked. | CPU/ABI notes — https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | `documented` (project-authored secondary) | `5` | Which claims are explicitly documented by Microsoft/AMD public material? Record separately if researched. |
| REC-19 | If any external design is ever considered for adoption, first resolve license status for the exact revision and third-party credits. | License findings — https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/README.md ; https://api.github.com/repos/omonkeyz/xbox-emulator/license ; https://api.github.com/repos/drycheetah84/TomBox/license ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/THIRD_PARTY/CREDITS | `observed` + `unknown` | `5` | Owner/legal review must record outcome; this research gives no legal advice. |
| REC-20 | Re-evaluate specialist ownership only after roles are installed; until then map potential work to installed coordinator/architect/developer/tester/reviewer/researcher roles. | Six installed and nine planned roles — local `docs/AGENT_WORKFLOW.md` | `observed` | `5` | Owner decision/permission changes required to install roles; agents must not install them. |

---

## 6. Priority and decision summary

| Priority | Items | Why |
|---|---|---|
| Now, docs/process only | REC-01, REC-02, REC-05, REC-06, REC-07 | Low risk, directly transferable workflow methods, no product change. |
| Before any new tool/interface | REC-03, REC-04, REC-08, REC-09 | Need coordinator assignment, tester evidence and possible owner approval. |
| Do not implement without approval | REC-10, REC-11, REC-12 | REC-10 needs an approved implementation package only; REC-11/REC-12 also require ADR. |
| Hard exclusions | REC-13, REC-14, REC-15, REC-16 | Prohibited data, unresolved license, missing architecture, or out-of-scope services. |
| Investigate only | REC-17, REC-18, REC-19, REC-20 | Evidence, legal or role availability is incomplete. |

---

## 7. Deliberately not implemented

This package did not implement, change or approve:

- product code, product tests, CMake, presets, agent configuration or other docs beyond RESEARCH-007 files and the register;
- CPU interpreter/JIT changes;
- GuestMemory/Xbox memory semantics;
- D3D12.X, shader translation, texture mirroring, loader/relinker, import redirection or network guard;
- use of dumps, keys, firmware, game content, protected assets or DRM circumvention;
- code copying from either repository;
- installation of the nine planned specialist roles;
- commits, pushes, merges, tags, releases or PRs.

No build, CTest or diff verification was applicable to this research-only package.
