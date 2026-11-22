# RESEARCH-007 — Licenses, source inventory and reproduction paths

**PACKAGE_TYPE:** RESEARCH
**PRODUCT_BOUNDARY:** development-only
**Access date for every source below:** 2026-11-22
**Exact compared revisions:**
- `omonkeyz/xbox-emulator` @ `65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2` (primary)
- `drycheetah84/TomBox` @ `9a39de888bbdecef1cae84171464dad34176b994` (upstream HEAD)
- `drycheetah84/TomBox` @ `7e8064fdcd47bca4afef3ec295668ed40b81c047` (upstream base named by primary README)

**Evidence labels:** `documented` (explicit in an authoritative or project-authored public source), `observed` (directly seen in fetched text/API response or local checkout), `hypothesis` (unverified interpretation), `unknown` (not established).
**Reproduction:** fetch each exact URL and read the named field/file/phrase. Pinned raw URLs are preferred so the examined revision is reproducible. Local sources are read from checkout root.

No repository was cloned. No binary, dump, firmware, key, game file, protected asset or proprietary source was downloaded, opened or used. No code was copied.

---

## 1. Exact commit determination

| ID | Exact URL | Result | Label | Reproduction |
|---|---|---|---|---|
| S01 | https://api.github.com/repos/omonkeyz/xbox-emulator/commits/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2 (stable replay; discovered via https://api.github.com/repos/omonkeyz/xbox-emulator/commits/HEAD on 2026-11-22) | JSON top-level `sha`: `65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2`; commit date `2026-11-22T15:43:20Z`; message begins “Merge #143, #145 from lex021: keep acp_state.rs as text (NUL escape), pinned Opus sources for the workspace check”. | `observed` | Fetch the pinned URL; read `sha`, `commit.committer.date`, `commit.message`. The `commits/HEAD` URL is a discovery record, not a stable replay step. |
| S02 | https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994 (stable replay; discovered via https://api.github.com/repos/drycheetah84/TomBox/commits/HEAD on 2026-11-22) | JSON top-level `sha`: `9a39de888bbdecef1cae84171464dad34176b994`; commit date `2026-11-22T02:40:52Z`; message begins “Translator: group barriers for one-wave compute shaders that rely on lockstep LDS”. | `observed` | Fetch the pinned URL; read `sha`, `commit.committer.date`, `commit.message`. The `commits/HEAD` URL is a discovery record, not a stable replay step. |
| S20 | https://api.github.com/repos/drycheetah84/TomBox/commits/7e8064fdcd47bca4afef3ec295668ed40b81c047 (stable replay of short hash `7e8064f` named by the primary README) | JSON top-level `sha`: `7e8064fdcd47bca4afef3ec295668ed40b81c047`; commit date `2026-11-20T23:29:46Z`. | `observed` | Fetch the pinned URL; read `sha`. |

GitHub HTML repository pages reported 167 commits on primary `main` and 136 commits on upstream `master`; a commit count is not a commit identity and was not used as a hash.

---

## 2. Full source catalogue

All URLs below were fetched/read on 2026-11-22. “Truncated” means the fetch tool returned only part of a large file; unread portions remain `unknown`.

| ID | Exact URL | Fetched content / result | Label | Reproduction |
|---|---|---|---|---|
| S03 | https://github.com/omonkeyz/xbox-emulator | Repository page and README; continuation of TomBox; GPL-3.0-or-later; earlier releases MIT OR Apache-2.0; status/metric definitions and title results. | `documented` (project-authored) + `observed` (page content) | Open URL; read README sections. |
| S04 | https://github.com/drycheetah84/TomBox | Repository page and README; native x64 runtime; no CPU emulation/recompilation; scope/legal section; documentation-lag statement; no license badge in examined root listing. | `documented` (project-authored) + `observed` (page content) | Open URL; read README and root listing. |
| S05 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/LICENSE | Full GNU General Public License, Version 3, 29 June 2007 text. | `observed` | Fetch URL; find “GNU GENERAL PUBLIC LICENSE” and “Version 3”. |
| S06 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | Contribution rules, `exports.txt` format/status vocabulary, unit-test requirement, legal gates, accepted/rejected license statements. | `documented` | Fetch URL; find headings “Adding a function”, “Rules”, “Pull requests”. |
| S07 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/LICENSE | **FAILED:** HTTP 404 (`404 GET .../LICENSE`). No `LICENSE` file at pinned upstream HEAD. | `observed` | Fetch URL; observe 404. |
| S08 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/ARCHITECTURE.md | Provisional “proposal under construction”; native execution model, subsystem decomposition and design rules; subsystem table lags README. | `documented` (project-authored proposal) + `observed` | Fetch URL; read opening and subsystem table. |
| S09 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py | Progress generator; output paths, status vocabulary, evidence-driven counting and `selftest()`; fetch truncated at about 797/916 lines. | `observed` | Fetch URL; find `selftest`, `LIBRARY_MAP.md`, `progress.svg`, status strings. |
| S10 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/THIRD_PARTY/CREDITS | Third-party component credits and stated licenses; fetch truncated after several entries. | `observed` | Fetch URL; find named projects below. |
| S11 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | Library discovery log; per-library measurements, evidence gating, pending/stub cases and dated parallel-agent integration notes; fetch truncated. | `observed` | Fetch URL; find dated entries and status terms. |
| S12 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | Honest debt ledger; unverified assumptions, missing tests, texture/network/graphics limitations; fetch truncated. | `documented` (project-authored) + `observed` | Fetch URL; read opening and sections. |
| S13 | https://api.github.com/repos/drycheetah84/TomBox/license | **FAILED:** HTTP 404. GitHub reports no detected repository license at examined state. | `observed` | Fetch URL; observe 404. |
| S14 | https://api.github.com/repos/omonkeyz/xbox-emulator/license | JSON; `name: LICENSE`, `spdx_id: GPL-3.0`, size 35149; content begins GPL v3. | `observed` | Fetch URL; read `spdx_id`, `size`, `content`. |
| S15 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | Project-authored public-source research notes with `[DOC]`, `[PUB]`, `[INF]`, `[HYP]`, `[NDA]` tags; CPU/ABI, memory and graphics sections. | `documented` (project-authored secondary source) | Fetch URL; read evidence discipline and sections 1–3. |
| S16 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/.github/workflows/rust.yml | Rust CI workflow at pinned commit with `contents: read`, pinned action SHAs, `cargo check --locked` and tests; it has no actionlint step — actionlint is a separate workflow. | `observed` | Fetch URL; find `cargo check`, `jobs`, action SHAs. |
| S17 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/crates/compat/d3d12_xs/exports.txt | Machine-readable per-export rows with statuses and console evidence notes; fetch truncated. | `observed` | Fetch URL; find `D3D12ConfigureVirtualMemoryX`, `D3D12ZeroMemoryX`. |
| S18 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/summary.json | Machine-readable metrics; `headline`, `extra`, per-title imports and OS builds; fetch truncated after first portion. | `observed` | Fetch URL; read `headline`. |
| S19 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/MILESTONES.md | Generated milestone table with evidence paths; fetch truncated. | `observed` | Fetch URL; find `9/18`, `run.txt:6496`. |
| S21 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md | Full upstream README at pinned HEAD; status, tested titles, architecture layers, known problems, docs list, scope/legal. | `documented` (project-authored) + `observed` | Fetch URL; find “How it works” and “Scope and legal”. |
| S22 | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/README.md | Full primary README at pinned HEAD; continuation statement, license statement, metrics definitions, milestones, tested titles, credit to base commit `7e8064f`. | `documented` (project-authored) + `observed` | Fetch URL; find “Contributing” and “Credit”. |
| S23 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DEVELOPMENT_LOG.md | Append-only chronological log with actual commands/results/evidence; first ~807/1425 lines returned; remainder `unknown`. | `documented` (project-authored) + `observed` | Fetch URL; read opening line and dated sections. |
| S24 | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DECISIONS.md | ADR-0001 (Rust toolkit) and ADR-0002 (native relink/import redirection, not emulation) with evidence/options/risks/tests. | `documented` (project-authored ADRs) | Fetch URL; find ADR headings. |
| L01 | local `CMakeLists.txt` | XBridge C++20/CMake/MSVC warning flags and targets; version 0.3.0. | `observed` | Open file; find `CXX_STANDARD 20`, `/W4`, `/permissive-`, target names. |
| L02 | local `include/xbridge/cpu/x64_instruction_decoder.hpp`, `x64_instruction_interpreter.hpp`, `x64_cpu_state.hpp` | XBridge CPU scaffold facts: status enums, no partial mutation, GPR/RIP/RFLAGS only. | `observed` | Open files; inspect declarations. |
| L03 | local `include/xbridge/memory/memory.hpp`, `docs/ARCHITECTURE.md` | Generic GuestMemory and accepted architecture rules. | `observed` + `documented` | Open files; inspect `check_range` and rules. |
| L04 | local `include/xbridge/host/capabilities.hpp`, `include/xbridge/host/execution_policy.hpp`, `docs/DECISIONS.md` | Host-only capability facts, pure execution selector, no JIT implemented and accepted ADR constraints. | `observed` + `documented` | Open files; inspect enums/selectors/ADRs. |
| L05 | local `docs/AGENT_WORKFLOW.md` | Six installed agents; nine requested specialists are `planned`, not installed/freigegeben. | `observed` | Open file; read installed roles and planned specialist table. |

---

## 3. License findings

| ID | Exact claim | Exact URL | Label | Reproduction |
|---|---|---|---|---|
| LIC-01 | Primary README states: “Licensed GPL-3.0-or-later (see LICENSE). Releases before this change remain available under MIT OR Apache-2.0.” | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/README.md | `documented` (project-authored) | Fetch URL; find “Licensed GPL-3.0-or-later”. |
| LIC-02 | GitHub API license metadata for primary repo returns `spdx_id: GPL-3.0` and a 35,149-byte `LICENSE`. | https://api.github.com/repos/omonkeyz/xbox-emulator/license | `observed` | Fetch URL; read `spdx_id` and `size`. |
| LIC-03 | Pinned primary `LICENSE` contains the full GNU GPL v3 text. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/LICENSE | `observed` | Fetch URL; read title/version and license text. |
| LIC-04 | Primary CONTRIBUTING states GPL-3.0-or-later contributions are accepted, GPL-2.0-only is not allowed, and third-party code must be credited in `THIRD_PARTY/CREDITS`. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `documented` | Fetch URL; find license lines and CREDITS requirement. |
| LIC-05 | Upstream pinned `LICENSE` fetch failed with HTTP 404. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/LICENSE | `observed` | Fetch URL; observe 404. |
| LIC-06 | GitHub API license metadata for upstream returned HTTP 404 (no detected license). | https://api.github.com/repos/drycheetah84/TomBox/license | `observed` | Fetch URL; observe 404. |
| LIC-07 | Upstream README’s “Scope and legal” section describes lawful interoperability research, no circumvention/key extraction/package decryption, no distributed game assets and no affiliation with Microsoft/publishers; it does not state a repository license in the fetched text. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md | `observed` | Fetch URL; read “Scope and legal” and search for “license”. |
| LIC-08 | Primary third-party credits name bcpack-rs as `MIT OR Apache-2.0`; silicon-rans as `MIT`; Mesa as `MIT`; LLVM as `Apache-2.0 WITH LLVM-exception`; tinygrad as `MIT`. These are project-authored credit lines, not independently re-verified license audits. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/THIRD_PARTY/CREDITS | `observed` (credit text) / `unknown` (independent license verification) | Fetch URL; find each named project and license string. |
| LIC-09 | XBridge rules prohibit copying third-party code without verifying its license and prohibit proprietary assets/keys/DRM circumvention. | local `AGENTS.md`, `docs/ARCHITECTURE.md` | `documented` | Open files; read evidence/legal constraints. |

---

## 4. License contradictions and unresolved points

| ID | Exact observation | URLs | Label | Reproduction |
|---|---|---|---|---|
| LC-01 | Primary current code is described/reported as GPL-3.0/GPL-3.0-or-later while earlier releases are explicitly described as MIT OR Apache-2.0. The legal effect of that statement for specific commits/releases was not established here. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/README.md ; https://api.github.com/repos/omonkeyz/xbox-emulator/license ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/LICENSE | `observed` | Fetch URLs; compare README, API `spdx_id`, LICENSE. |
| LC-02 | Upstream has no detected license at examined HEAD through two independent public checks (raw pinned LICENSE 404 and API license 404), while upstream README contains detailed legal/scope statements but no license grant in fetched text. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/LICENSE ; https://api.github.com/repos/drycheetah84/TomBox/license ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md | `observed` | Fetch URLs; observe both 404s and README text. |
| LC-03 | Primary credits third-party components with specific license strings, but this research did not open those third-party license texts or verify versions/hashes beyond the credit lines. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/THIRD_PARTY/CREDITS | `unknown` | Fetch URL; note that only credit text was read. |
| LC-04 | Conclusion: no source, design text or generated artifact from either repository may be adopted into XBridge until the owner/legal review resolves compatibility. This is a research caution, not legal advice. | local `AGENTS.md` ; URLs in LC-01/LC-02 | `hypothesis` (risk assessment) + `documented` (XBridge rule) | Read XBridge rule and compare unresolved license findings. |

---

## 5. Source-origin and legal-source notes

| ID | Exact claim | Exact URL | Label | Reproduction |
|---|---|---|---|---|
| SO-01 | Primary README credits upstream `drycheetah84/TomBox` and base commit `7e8064f`. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/README.md | `documented` (project-authored) | Fetch URL; find “Credit”. |
| SO-02 | API resolved base `7e8064f` to `7e8064fdcd47bca4afef3ec295668ed40b81c047`. | https://api.github.com/repos/drycheetah84/TomBox/commits/7e8064f | `observed` | Fetch URL; read `sha`. |
| SO-03 | Upstream development log states it studied AnyPS5 and that ADR-0002 chose native x64 execution with import redirection, not emulation. This is a project-authored origin claim about a different console project; it is not Xbox Series X evidence by itself. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DEVELOPMENT_LOG.md ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DECISIONS.md | `documented` (project-authored) + `hypothesis` (XBridge relevance) | Fetch URLs; find “AnyPS5” and ADR-0002. |
| SO-04 | Upstream `XBOX_RESEARCH.md` says its own tags distinguish public documentation, credible public/community reverse-engineering, inference, hypothesis and NDA-gated knowledge, and that nothing is implemented on `[HYP]` alone. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | `documented` | Fetch URL; read opening. |
| SO-05 | The examined external files are project-authored engineering records, not authoritative Microsoft documentation. Statements about D3D12.x NDA gaps, private headers or dump-derived measurements remain project-authored unless independently established. | URLs in §2 | `observed` + `unknown` | Compare source types in §2; no authoritative Microsoft source was fetched in this task. |

---

## 6. Reproduction checklist for the complete package

1. Fetch S01/S02/S20 and verify the three commit hashes.
2. Fetch S03/S04/S21/S22 and verify the README claims about architecture, status and legal scope.
3. Fetch S05/S06/S14/S13/S07 and verify the license findings and both API/raw failures.
4. Fetch S08/S12/S15/S23/S24 and verify architecture, debt, research tags, development log and ADRs.
5. Fetch S09/S10/S11/S16/S17/S18/S19 and verify workflow/generator/metrics/credit claims.
6. Open L01–L05 and verify the XBridge baseline and agent-availability limitation.
7. For every finding in the other RESEARCH-007 documents, resolve the ID or exact URL here and search the quoted phrase.

All external URLs were accessed on 2026-11-22. Large files whose fetch output was truncated are explicitly partial; unread tails are `unknown`.
