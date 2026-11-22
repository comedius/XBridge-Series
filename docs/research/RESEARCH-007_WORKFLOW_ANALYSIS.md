# RESEARCH-007 — Development workflow analysis

**PACKAGE_TYPE:** RESEARCH
**PRODUCT_BOUNDARY:** development-only
**Access date for every external source in this package:** 2026-11-22
**Compared revisions:** `omonkeyz/xbox-emulator` @ `65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2`; `drycheetah84/TomBox` @ `9a39de888bbdecef1cae84171464dad34176b994`.
**Evidence labels:** `documented` (explicit in a source), `observed` (directly seen in fetched text/API/local checkout), `hypothesis` (analyst interpretation, unverified), `unknown` (not established).
**Reproduction:** fetch the exact URL in a finding, then search the quoted phrase. Local sources are read from checkout root. Source IDs are resolved in `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md`.

This document analyses workflow only. It does not propose copying code, using dumps, changing XBridge architecture, or treating another project's test title as an XBridge compatibility target.

---

## 1. Executive summary

`observed`: both projects maintain dense chronological logs, explicit status/data files, generated progress artifacts and visible debt ledgers. The primary project adds a contribution gate requiring per-function tests, evidence notes and no binaries/game data. These are the transferable workflow patterns.

`hypothesis`: the strongest XBridge-compatible workflow is “evidence first, explicit gaps, generated status, test-per-behavior, current debt ledger, CI/PR gates, no unverified capability claims”. This is a development-method recommendation, not an implementation change.

`unknown`: independent legal review, exact upstream license status and whether any external implementation detail is correct for Xbox Series X beyond the project-authored claims recorded here.

---

## 2. Workflow stage findings

### 2.1 Evidence intake and legal boundary

| Finding | Exact claim | Exact URL | Label | Reproduction |
|---|---|---|---|---|
| WF-01 | Primary README says it uses legally obtained, already-decrypted dumps; the runtime does no key extraction or package decryption; the source workspace contains no game files, binaries or build output. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/README.md | `documented` (project-authored) | Fetch URL; read intro and “Source code”. |
| WF-02 | Upstream README says users need their own legally obtained already-decrypted dump; no game files are provided; no console-security circumvention, key extraction or package decryption; no game executables/assets distributed. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md | `documented` (project-authored) | Fetch URL; read “Requirements” and “Scope and legal”. |
| WF-03 | Contribution rules reject binaries, game/console files, personal data and added comment lines in `.rs`/`.py`/`.ps1`; no runtime network, no decryption/key extraction/DRM circumvention. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `documented` | Fetch URL; find PR gate and rules. |
| WF-04 | XBridge accepted rules prohibit proprietary firmware, keys, game files, protected assets and protection/DRM circumvention. | local `AGENTS.md`, `docs/ARCHITECTURE.md`, `docs/DECISIONS.md` | `documented` | Open files; find constraints sections. |

Interpretation (`hypothesis`): XBridge can adopt the *rule shape* (no protected data, explicit legal boundary) without adopting the other project's dump-based workflow. Any future evidence source must be public and lawful.

### 2.2 Per-function status and explicit gaps

| Finding | Exact claim | Exact URL | Label | Reproduction |
|---|---|---|---|---|
| WF-05 | One compat crate per console DLL; `exports.txt` line format is `ordinal Name status | notes`; documented statuses include `implemented`, `forwarded`, `pc`, `title`, `missing`. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `documented` | Fetch URL; find status vocabulary. |
| WF-06 | Example `d3d12_xs/exports.txt` rows use `implemented`, `forwarded` and `data` with console addresses/tests in notes; `gen.py` recognises ten statuses including `pending`, `stub`, `unsupported`, `unverified` and `missing`. This is a vocabulary mismatch with CONTRIBUTING.md. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/crates/compat/d3d12_xs/exports.txt ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py | `observed` | Fetch URLs; compare status strings. |
| WF-07 | “No fake success”: do not return `S_OK` or accept-and-ignore when the console would do something; leave the row `missing` and explain why. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `documented` | Fetch URL; find exact phrase. |
| WF-08 | A module counts as loaded/resolved only when `loaded: true` and `unresolved: []`; a system-function import flips to done only when its name is in `resolved`; host-PC rows are reclassified from `pc` to `hooked` only with `oshost.json` evidence. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` | Fetch URLs; find `loaded`, `unresolved`, `resolved`, `oshost`. |
| WF-09 | Some functions are deliberately `pending`/`stub` even after implementation: `gameinput_ext` is dead at runtime; MFPlat stubs have argument checks but no PC work queue; `xg_x.dll` texture computers return `E_NOTIMPL`. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` | Fetch URL; find library entries. |

Interpretation (`hypothesis`): XBridge docs should distinguish “not investigated”, “investigated but unsupported”, “implemented without validation” and “validated”. This is a documentation method only; no XBridge status taxonomy has been approved by this package.

### 2.3 Implement → test → evidence note loop

| Finding | Exact claim | Exact URL | Label | Reproduction |
|---|---|---|---|---|
| WF-10 | For unknown functions, the documented method is: use your own console DLL from a lawful dump, public GDK docs and compatible open-source references; derive return codes/edge cases/output layout; implement; add a unit test; update `exports.txt` with a source note. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `documented` | Fetch URL; read contribution steps. |
| WF-11 | Every new implementation requires a unit test calling the function and checking console behavior; a maintainer reviews against console behavior. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `documented` | Fetch URL; find unit test requirement. |
| WF-12 | Upstream reports that `pe-inspect` was cross-checked against Microsoft `dumpbin` on the same binary; parser tests include no-panic sweeps over random/truncated inputs. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md ; https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DEVELOPMENT_LOG.md | `documented` (project-authored) + `observed` (report text) | Fetch URLs; find “dumpbin” and “random / PE-shaped / truncated”. |
| WF-13 | XBridge already requires tests for new behavior, boundary/failure tests, VS2026 Debug build, CTest and `git diff --check` for implementation tasks. | local `AGENTS.md` | `documented` | Open file; read §6. |

Interpretation (`hypothesis`): for XBridge, the analogue of “console behavior oracle” must be a public authoritative source or a locally reproducible test expectation; dumps and proprietary binaries are not acceptable oracles under current project rules.

### 2.4 Generated status and metrics

| Finding | Exact claim | Exact URL | Label | Reproduction |
|---|---|---|---|---|
| WF-14 | `tools/progress/gen.py` generates `docs/LIBRARY_MAP.md`, `docs/img/progress.svg` and per-metric badges from JSON plus `exports.txt`; it contains a `selftest()` with denominator/evidence-flip checks. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py | `observed` | Fetch URL; find output paths and `selftest`. |
| WF-15 | `summary.json` contains headline metrics, extra metrics, per-title import status and console OS builds; `docs/MILESTONES.md` is generated and cites evidence paths such as `%TEMP%\tombox\run.txt:6496`. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/summary.json ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/MILESTONES.md | `observed` | Fetch URLs; inspect JSON keys and milestone table. |
| WF-16 | “Not targeted” sections keep uncovered exports in the denominator; status rows explain why a function is stub/unsupported/unverified. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/gen.py ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` | Fetch URLs; find denominator and status-note logic. |
| WF-17 | Headline metrics at examined revision include 671/671 system functions, 194/200 D3D12.x methods, 768/783 RDNA2 instructions, 105,506/105,506 shaders translated and 0/8 shader stages. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/tools/progress/summary.json | `observed` | Fetch URL; inspect `headline`. |

Interpretation (`hypothesis`): an XBridge status generator, if ever approved, must use public/local evidence only, must not hide gaps and must not claim capability from translation/coverage counts. Any new tool/dependency requires owner approval.

### 2.5 Current logs, debt ledgers and documentation lag

| Finding | Exact claim | Exact URL | Label | Reproduction |
|---|---|---|---|---|
| WF-18 | Upstream `DEVELOPMENT_LOG.md` says it is append-only and records what was actually done, run and observed. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/DEVELOPMENT_LOG.md | `documented` | Fetch URL; read opening line. |
| WF-19 | `TECHNICAL_DEBT.md` is titled “Honest ledger. Updated continuously.” and lists unverified assumptions, missing tests and known gaps. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `documented` | Fetch URL; read title/opening. |
| WF-20 | Upstream README states `ARCHITECTURE.md`, `XBOX_RESEARCH.md` and `COMPATIBILITY.md` lag behind code while the development log and technical-debt file are current. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md | `documented` (project-authored) | Fetch URL; find “Documentation”. |
| WF-21 | At the same upstream commit, `ARCHITECTURE.md` says several subsystems are not started while README reports substantial progress; this is an observed documentation lag/contradiction. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/ARCHITECTURE.md ; https://github.com/drycheetah84/TomBox | `observed` | Fetch both; compare subsystem table/status. |
| WF-22 | `XBOX_RESEARCH.md` uses explicit tags `[DOC]`, `[PUB]`, `[INF]`, `[HYP]`, `[NDA]` and states nothing is implemented on the strength of `[HYP]` alone. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/XBOX_RESEARCH.md | `documented` | Fetch URL; read opening evidence discipline. |

Interpretation (`hypothesis`): XBridge's required evidence labels already serve a similar purpose. Keep “current verified state” separate from “old design intent”; do not silently rewrite stale documents.

### 2.6 CI, PR gates and parallel work

| Finding | Exact claim | Exact URL | Label | Reproduction |
|---|---|---|---|---|
| WF-23 | CI workflow runs on PR/push/dispatch with `contents: read`, pinned action SHAs, `cargo check --locked` for selected crates and test steps; PR gates also require no binaries/game data/personal data. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/.github/workflows/rust.yml ; https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/CONTRIBUTING.md | `observed` + `documented` | Fetch URLs; inspect workflow and PR gate. |
| WF-24 | Parallel agent integration is recorded with dated credits; one entry notes an agent’s in-progress edit broke another crate’s build, and unverified/unwired work is not credited. | https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/docs/LIBRARY_DISCOVERY.md | `observed` | Fetch URL; find dated integration entries. |
| WF-25 | XBridge has six installed agents (`coordinator`, `architect`, `developer`, `tester`, `reviewer`, `researcher`); the nine requested specialists are listed as `planned`, not installed/freigegeben. | local `docs/AGENT_WORKFLOW.md` | `observed` | Open file; read “Role catalogue and ownership”. |

Interpretation (`hypothesis`): use disjoint write sets and integration checks, but assign only installed owners. A recommendation that names a specialist owner is not executable until that role is installed and permitted.

### 2.7 Network safety and production-service isolation

| Finding | Exact claim | Exact URL | Label | Reproduction |
|---|---|---|---|---|
| WF-26 | A network guard was added after runs reached Epic production analytics; known gaps include `ConnectEx` to literal IP and non-WinHTTP TLS. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/docs/TECHNICAL_DEBT.md | `observed` | Fetch URL; find network-guard section. |
| WF-27 | Upstream states network access is the user's choice and the project's own development/testing never connects to publisher production services. | https://raw.githubusercontent.com/drycheetah84/TomBox/9a39de888bbdecef1cae84171464dad34176b994/README.md | `documented` (project-authored) | Fetch URL; read “Scope and legal”. |
| WF-28 | XBridge has no title execution or network product surface in the verified scaffold. | local `src/app/main.cpp`, `docs/ARCHITECTURE.md` | `observed` | Open files; inspect CLI demo and architecture scope. |

Interpretation (`hypothesis`): XBridge should avoid any workflow that contacts publisher services. No network subsystem is justified by this research.

---

## 3. Workflow elements deliberately not transferred

| Element | Reason | Label |
|---|---|---|
| Legally obtained dumps and extracted console DLLs as test inputs | XBridge rules prohibit proprietary system files and protected assets; this research used no such material. | `documented` (XBridge rules) + `observed` (research boundary) |
| Native execution/import redirection of game binaries | Requires architecture change and conflicts with current interpreter baseline until an ADR exists. | `documented` (external ADR) + `hypothesis` (XBridge implication) |
| RDNA2/HLSL translator and DXC runtime compilation | Requires console shader corpus and a graphics backend XBridge does not have. | `documented` (external README) + `observed` (XBridge baseline) |
| Title-specific hooks, local users/stores and network redirects | Title-specific, not general, and outside current XBridge scope. | `documented` (external README) |
| Rust code, action workflows or generator implementation | License status unresolved and language/build model differs; no code was copied. | `observed` + `unknown` |
| Agent team represented by nine specialists | They are `planned`, not installed. | `observed` |

---

## 4. XBridge-compatible workflow synthesis (`hypothesis`)

The following sequence is a research interpretation only; it is not approved and does not authorize edits:

1. State the question and boundary; label every technical claim.
2. Collect only public, lawful sources and pin exact URLs/commits.
3. Record exact claims, reproduction paths and contradictions in a register/research document.
4. Keep current evidence in a current log/debt file; mark stale design documents as stale.
5. For any future behavior, write the public-evidence expectation before implementation.
6. Implement the smallest coherent change; add boundary and failure tests.
7. Run the approved build/CTest/diff checks and record exact commands/results.
8. Keep gaps explicit; never report a stub as implemented success.
9. Route work through installed roles with disjoint write sets; do not assume planned specialists exist.
10. Escalate license, architecture, interface and dependency decisions to the owner.

Each numbered item is `hypothesis` as an XBridge workflow proposal. Items 1, 2, 3, 4, 5, 8, 9 and 10 are directly supported in method form by the workflow findings above; items 6 and 7 are `documented` in local `AGENTS.md`.

---

## 5. Open workflow questions (`unknown`)

- Will the owner approve any new status generator/tool, and if so which language/dependency?
- Which future XBridge requirement, if any, justifies an executable loader, graphics backend or network isolation?
- How should license/legal review be recorded if any external design is ever considered for adoption?
- How should planned specialist roles be installed or replaced before packages that name them can be executed?

No build, CTest or diff verification was applicable to this research-only package.
