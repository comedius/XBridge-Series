# XB-0084 — Research license and provenance assessment

- **PACKAGE_ID:** XB-0084
- **PACKAGE_TYPE:** RESEARCH
- **PRODUCT_BOUNDARY:** development-only
- **ACCESS DATE:** 2026-11-22
- **STATUS:** assessment/planning only; no implementation approved.
- **Evidence labels:** `documented` (explicit in an authoritative source or accepted project document), `observed` (directly seen in fetched text/API response or local file), `hypothesis` (plausible but unverified), `unknown` (not established).
- **Boundary:** Development-only research document. Not product code, not legal advice, and not an adoption approval. No third-party code, design text, generated artifact, firmware, key, dump, game asset or protected material was copied or used in this research.

## 1. Purpose and no-legal-advice statement

- `documented` — This assessment records provenance and reuse risk only. It is not a legal opinion and does not determine whether any license is compatible with XBridge or any future distribution. `docs/LEGAL_COMPLIANCE.md:114-119` states that the project gives no legal assurance.
- `documented` — XBridge rules require verified permission and applicable license terms before incorporating protected material, and prohibit copying third-party code without license verification. `docs/LEGAL_COMPLIANCE.md:28-35`, `:51-59`; `AGENTS.md:39-45`.
- `documented` — No adoption of source, design text or generated artifact from the examined projects is approved by this document. Owner/legal review remains required. `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:85-88`.

## 2. Exact revisions and authorship/provenance

All external sources below were accessed on `2026-11-22`. A commit hash identifies the examined source state; it is not evidence of a verified release, build or runtime behavior.

| Source | Exact revision/hash | What it is | Label | Reproduction |
|---|---|---|---|---|
| `omonkeyz/xbox-emulator` | `65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2` (S01) | Primary RESEARCH-007 project; README states it is a continuation of TomBox and credits base commit `7e8064f`. | `observed` for API hash; `documented` for project-authored statements. | `https://api.github.com/repos/omonkeyz/xbox-emulator/commits/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2`; pinned README at `https://raw.githubusercontent.com/omonkeyz/xbox-emulator/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2/README.md`. `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:22`, `:54`, `:96`. |
| `drycheetah84/TomBox` upstream HEAD | `9a39de888bbdecef1cae84171464dad34176b994` (S02) | Upstream project examined at RESEARCH-007 HEAD. | `observed` | `https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994`. `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:23`. |
| `drycheetah84/TomBox` base | `7e8064fdcd47bca4afef3ec295668ed40b81c047` (S20) | Base commit named by the primary README. | `observed` | `https://api.github.com/repos/drycheetah84/TomBox/commits/7e8064fdcd47bca4afef3ec295668ed40b81c047`. `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:24`, `:97`. |
| KytyPS5 | `39936ffa39237997080f8d169370667ca63d12fa` | PS5 emulator reference. | `observed` | `https://github.com/KytyPS5/KytyPS5/tree/39936ffa39237997080f8d169370667ca63d12fa`; `docs/research/ps5-reference-project-analysis.md:29`. |
| Cosmo/KytyPS5 | `65db31f2b4172dc9c6cd52424e935c05e1cb366c` | PS5 emulator port reference. | `observed` | `https://github.com/Cosmo/KytyPS5/tree/65db31f2b4172dc9c6cd52424e935c05e1cb366c`; `docs/research/ps5-reference-project-analysis.md:30`. |
| PS5PCEM | `aeefeb4136d2f833943328b733ab5bb7dcaba034` | PS5 emulator/native CPU reference. | `observed` | `https://github.com/iStark/PS5PCEM/tree/aeefeb4136d2f833943328b733ab5bb7dcaba034`; `docs/research/ps5-reference-project-analysis.md:31`. |
| AnyPS5 | `8763aeffdc37e203b7d82ee4df154e42bb2e5bde` | Relinker/porting-tool reference, described by its project as not emulation. | `documented` project-authored; `observed` commit. | `https://github.com/boykopovar/AnyPS5/tree/8763aeffdc37e203b7d82ee4df154e42bb2e5bde`; `docs/research/ps5-reference-project-analysis.md:32`. |
| SharpEmu | `9a64b459893cb98fa1510b98c6941db625b6ddff` | Experimental PS5/PS4 emulator reference. | `observed` | `https://github.com/SharpEmu/SharpEmu/tree/9a64b459893cb98fa1510b98c6941db625b6ddff`; `docs/research/ps5-reference-project-analysis.md:33`. |
| Leonida | `d51bddc84e38971c834234179080f8d00092846a` | Game-specific native Windows port reference. | `observed` | `https://github.com/ItsNotRexed/Leonida/tree/d51bddc84e38971c834234179080f8d00092846a`; `docs/research/ps5-reference-project-analysis.md:34`. |
| Microsoft Learn Xbox 360 CPU articles | `26465e18219d6311381977f154b0a5b7ed53c8d6` | W01/W02 in `XB-0084_XBOX360_SPLINTER_CELL_ANALYSIS.md`; technical facts only, not license evidence. | `observed` | `https://learn.microsoft.com/en-us/windows/win32/dxtecharts/coding-for-multiple-cores?view=windows-11`; `https://learn.microsoft.com/en-us/windows/win32/dxtecharts/lockless-programming?view=windows-11`. |
| Microsoft archived Xbox 360 article | `5019655ffa733bb8ab1266cc2a6a7b70a1ecdfa6` | W03 in the Xbox 360 analysis; technical facts only, not license evidence. | `observed` | `https://learn.microsoft.com/en-us/archive/blogs/nmercer/xbox360?view=windows-11`. |

- `documented` — The PS5 reference document explicitly says the hashes identify examined source states, not verified releases or reproducible builds, and that no projects were built or executed. `docs/research/ps5-reference-project-analysis.md:36`, `:319-323`.
- `documented` — RESEARCH-007 external material consists of project-authored engineering records, not authoritative Microsoft documentation; claims about private headers, dumps or NDA gaps remain project-authored unless independently established. `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:98-100`.

## 3. RESEARCH-007 license status

| ID | Exact finding | Label | Source |
|---|---|---|---|
| LIC-01 | Primary README states: “Licensed GPL-3.0-or-later (see LICENSE). Releases before this change remain available under MIT OR Apache-2.0.” | `documented` project-authored | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:69` |
| LIC-02 | GitHub API reports primary `spdx_id: GPL-3.0` and a 35,149-byte `LICENSE`. | `observed` | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:70` |
| LIC-03 | Pinned primary `LICENSE` contains full GNU GPL v3 text. | `observed` | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:71` |
| LIC-04 | Primary `CONTRIBUTING.md` accepts GPL-3.0-or-later contributions, disallows GPL-2.0-only and requires third-party credit in `THIRD_PARTY/CREDITS`. | `documented` | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:72` |
| LIC-05 | Pinned upstream `LICENSE` fetch failed with HTTP 404. | `observed` | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:73` |
| LIC-06 | GitHub API license metadata for upstream returned HTTP 404 (no detected license). | `observed` | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:74` |
| LIC-07 | Upstream README “Scope and legal” section describes lawful research and no assets/keys/circumvention, but does not state a repository license in the fetched text. | `observed` | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:75` |
| LIC-08 | Third-party credits list bcpack-rs `MIT OR Apache-2.0`, silicon-rans `MIT`, Mesa `MIT`, LLVM `Apache-2.0 WITH LLVM-exception`, tinygrad `MIT`; these are credit lines, not independent license audits. | `observed` for credit text; `unknown` for verification | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:76` |
| LC-01 | Current primary code is described/reported as GPL-3.0/GPL-3.0-or-later while earlier releases are explicitly described as MIT OR Apache-2.0; legal effect for specific commits/releases is unresolved. | `observed`; legal effect `unknown` | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:85` |
| LC-02 | Upstream has no detected license at examined HEAD through raw-file and API checks. | `observed` | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:86` |
| LC-03 | Third-party licenses, versions and hashes were not independently audited beyond credit text. | `unknown` | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:87` |
| LC-04 | No source, design text or generated artifact from either repository may be adopted until owner/legal review resolves compatibility. Research caution, not legal advice. | `hypothesis` risk rule + `documented` project rule | `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:88` |

## 4. XBridge and older-research license posture

- `documented` — XBridge’s current `LICENSE` is `UNLICENSED`, grants no permission, and requires license clarification before accepting third-party contributions, publishing source or distributing binaries. `docs/LEGAL_COMPLIANCE.md:61-69`.
- `documented` — Public development discussion, source publication and binary distribution are distinct activities; publication requires separate review. `docs/LEGAL_COMPLIANCE.md:71-89`.
- `documented` — Before publication/distribution, provenance, exact versions/commits, licenses, notices, dependencies, bundled assets, documentation claims and unresolved rights must be reviewed and recorded. `docs/LEGAL_COMPLIANCE.md:90-112`.
- `documented` — `docs/research/xbox-series-x-cpu-evidence.md` records the Microsoft Series X CPU quote and keeps AMD64 facts separate from Xbox-specific facts; no license transfer issue was created by that distinction. `docs/research/xbox-series-x-cpu-evidence.md:16-30`.
- `unknown` — CPU-visible Xbox behavior, exception/TLS/ABI details and unsupported instruction coverage remain open in the older CPU research. `docs/research/xbox-series-x-cpu-evidence.md:53-62`; `docs/research/cpu-architecture-foundation.md:108`.
- `documented` — General x64 register/state material is explicitly general AMD64, not Xbox evidence. `docs/research/cpu-architecture-foundation.md:30-70`.

## 5. PS5 reference license posture and boundary

| Project | Visible license metadata | Label | Open check before any adoption |
|---|---|---|---|
| KytyPS5 | GPL-2.0-only; selected original Kyty components marked MIT. | `documented` metadata | Map exact MIT-exception files and submodule licenses. `docs/research/ps5-reference-project-analysis.md:288`. |
| Cosmo/KytyPS5 | GPL-2.0-only with separately marked MIT components. | `documented` metadata | Check fork patches and bundled/submodule licenses. `:289`. |
| PS5PCEM | GPL-3.0-or-later. | `documented` metadata | Build a revision-accurate dependency graph including build options and assets. `:290`. |
| AnyPS5 | GPL-2.0-only with extensive submodules/bundled components. | `documented` metadata | Inventory packages/files, submodule commits, license texts and exceptions. `:291`. |
| SharpEmu | GPL-2.0-or-later; REUSE/NuGet metadata present. | `documented` metadata | Audit NuGet/font/audio and other third-party licenses. `:292`. |
| Leonida | GPL-3.0-or-later per README; LICENSE contains GPLv3; credits mention Mesa, LLVM, SPIRV-Tools and `rpf9-explorer` submodule. | `documented` metadata; `unknown` for submodule terms | Audit submodule/transitive licenses and exact file/version mapping; no legal conclusion from first sighting. `:293`. |

- `unknown` — Whether any of those libraries or components could be combined, linked, modified or distributed with XBridge is not assessed. `docs/research/ps5-reference-project-analysis.md:295`.
- `documented` — PS5 ABI, TLS, syscalls, PRX/NID libraries, PS5 memory calls, AGC/PM4/RDNA2 terms and PC D3D12 backends are not Xbox Series X specifications and cannot justify Xbox claims. `docs/research/ps5-reference-project-analysis.md:273-280`.
- `hypothesis` — Common engineering ideas such as modular interfaces, mapping metadata, queue scheduling, logging, fuzzing and cache invalidation may inform design, but must be independently implemented and evidenced for Xbox. `docs/research/ps5-reference-project-analysis.md:280`, `:299-304`.

## 6. Reuse risk and permitted posture

### Concept-only reuse vs independent reimplementation

- `documented` — Class 1 concepts are directly usable workflow ideas: evidence-status vocabulary, append-only log, no fake success, test-per-behavior gates, artifact gates, debt/assumption ledger and disjoint write sets/integration checks. `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md:55`, `:63-68`, `:71-72`; `docs/research/RESEARCH-007_RECOMMENDATIONS.md:21-27`. These may be implemented as XBridge-owned process/docs without copying external wording.
- `documented` — Class 2 concepts are independent C++20 design ideas: explicit CPU status contracts, untrusted-input testing (random/truncated-input no-panic sweeps and independent-tool comparison; fuzzing is explicitly not yet configured upstream) and host-capability gates with tests. `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md:53-54`, `:61`; `docs/research/RESEARCH-007_RECOMMENDATIONS.md:37-39`. They require independent implementation and XBridge tests.
- `documented` — Class 3 concepts require ADR/owner approval: native execution/relinker/import redirection and unified-memory/GPU-visible allocation. `docs/research/RESEARCH-007_RECOMMENDATIONS.md:47-50`. They are not approved by XB-0084.
- `documented` — Class 4 items are hard exclusions: dumps, extracted DLLs, shaders, keys, firmware, game assets, DRM circumvention, copied third-party code/design text and publisher production services. `docs/research/RESEARCH-007_RECOMMENDATIONS.md:54-61`; `docs/AGENT_TASKS.md:488`.
- `unknown` — Class 5 items remain unresolved: D3D12.X equivalence, shader-stage formats, rendered-pixel correctness, CPUID/AVX2/Zen2/ABI details as XBridge requirements and adoption licensing. `docs/research/RESEARCH-007_RECOMMENDATIONS.md:65-72`.

### Direct-copy prohibition

- `documented` — Do not copy source, design text, generated artifacts, credits wording or configuration from examined projects into XBridge unless license compatibility, exact version and notices are reviewed and approved. `docs/LEGAL_COMPLIANCE.md:28-35`, `:51-59`.
- `documented` — Do not introduce proprietary firmware, keys, game files, dumps, decrypted DLLs, protected assets or DRM circumvention material. `AGENTS.md:39-45`; `docs/DECISIONS.md:43-46`; `docs/LEGAL_COMPLIANCE.md:43-49`.
- `documented` — No source code was copied in creating this assessment. `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:14`; `docs/research/ps5-reference-project-analysis.md:323`.

## 7. Decision gate

`documented` — No adoption may proceed until all of the following are done and recorded:

1. Project owner approves the exact scope and any ADR-required design.
2. XBridge’s own license status is clarified with qualified legal advice where required. `docs/LEGAL_COMPLIANCE.md:61-69`, `:107-112`.
3. Exact upstream commit, file-level origin, license text, notices and transitive dependency licenses are identified for every candidate item. `docs/LEGAL_COMPLIANCE.md:51-59`, `:95-105`.
4. RESEARCH-007 LC-01, LC-02 and LC-03 are resolved or the candidate is rejected. `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md:85-88`.
5. PS5 reference components are treated as design evidence only; no PS5-specific ABI, GPU or library fact is transferred to Xbox. `docs/research/ps5-reference-project-analysis.md:273-280`.
6. The result is reviewed and documented before source publication or binary distribution. `docs/LEGAL_COMPLIANCE.md:90-112`.

## 8. Unresolved items

- `unknown` — Legal effect of primary GPL-current versus earlier MIT/Apache releases for specific commits.
- `unknown` — Upstream license status at the examined HEAD.
- `unknown` — Third-party credit accuracy, versions, hashes and transitive license compatibility.
- `unknown` — XBridge project license and distribution permission.
- `unknown` — Whether any future dependency or design idea can be adopted without owner/legal approval.
- `documented` — This document changes no license and grants no permission.
