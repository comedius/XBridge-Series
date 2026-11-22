# Research register

Use one entry per question. Record source, date checked, confidence, and reproduction steps.

| ID | Topic | Status | Next action |
|---|---|---|---|
| R-001 | Publicly documented Xbox Series CPU-visible behavior | unknown | Review authoritative public documentation |
| R-002 | Xbox-specific graphics command interfaces | unknown | Map public GDK documentation and gaps |
| R-003 | Shader input representation and semantics | unknown | Build lawful shader research corpus |
| R-004 | Guest virtual-memory behavior | unknown | Specify observable requirements from public sources |
| R-005 | Storage/decompression APIs | partial | Map documented APIs and host equivalents |
| R-006 | System services required by test software | unknown | Start from public GDK APIs |
| RESEARCH-007 | Public Xbox Series X\|S emulator architecture/workflow concepts from `omonkeyz/xbox-emulator` and `drycheetah84/TomBox` | complete as evidence record; unresolved items remain `unknown` (upstream license/legal status, D3D12.X specifics, rendered-pixel correctness, installed specialist roles) | Date checked: 2026-11-22. Sources: GitHub API pinned-commit responses, repository/README pages and pinned raw files; full inventory in `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md`. Exact commits examined on 2026-11-22: primary `65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2`; upstream HEAD at that time `9a39de888bbdecef1cae84171464dad34176b994`; upstream base `7e8064fdcd47bca4afef3ec295668ed40b81c047`. Reproduction: fetch the pinned full-SHA URLs `https://api.github.com/repos/omonkeyz/xbox-emulator/commits/65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2`, `https://api.github.com/repos/drycheetah84/TomBox/commits/9a39de888bbdecef1cae84171464dad34176b994` and `https://api.github.com/repos/drycheetah84/TomBox/commits/7e8064fdcd47bca4afef3ec295668ed40b81c047`, then resolve the exact URLs/source IDs in `docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md` and search the quoted phrases in the five `docs/research/RESEARCH-007_*.md` files. Current branch HEADs differ from these pinned commits, so `commits/HEAD` is not a stable reproduction path. Next: coordinator records handoff; owner/legal review is required before any adoption because upstream license is `unknown` and the legal effect of the primary project's current GPL-3.0-or-later status versus earlier MIT/Apache-2.0 releases remains `unknown`. |
| XB-0084 | GTA VI Series X performance-first assessment and deferred Xbox 360 / Splinter Cell: Double Agent backward-compatibility planning | complete assessment/planning evidence record only; no implementation approved; GTA VI technical requirements and performance values `unknown` | Date checked: 2026-11-22. Six documents: `docs/research/XB-0084_GTA_VI_COMPATIBILITY_INVENTORY.md`, `XB-0084_GTA_VI_PERFORMANCE_PROFILE.md`, `XB-0084_PRIORITIZED_DEVELOPMENT_PLAN.md`, `XB-0084_RESEARCH_LICENSE_ASSESSMENT.md`, `XB-0084_RISK_DEPENDENCY_OVERVIEW.md`, `XB-0084_XBOX360_SPLINTER_CELL_ANALYSIS.md`. Twelve owner-fixed component areas (`docs/AGENT_TASKS.md:499`); no FPS/resolution/memory values asserted as measured; no Series S target and no current Xbox 360 target. Official sources support availability/native-platform messaging only, as recorded in `docs/research/XB-0084_RISK_DEPENDENCY_OVERVIEW.md:89-98`. Next: owner approval required before implementation, ADR, or any compatibility/performance claim. |

Evidence labels:
- documented
- observed
- hypothesis
- unknown

Never convert a hypothesis into a specification without evidence.
