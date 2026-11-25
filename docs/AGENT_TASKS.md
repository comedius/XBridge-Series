# Agent Task Ledger

The historical task ledger has been split into six smaller archive files. The archive content is preserved verbatim; original line numbers in historical records refer to the former monolithic file.

## Archive index

| Archive | Coverage | Original lines |
|---|---|---:|
| [01 — Foundation and early agent history](agent-tasks/01-foundation-and-early-agent-history.md) | Setup, early validation, autonomy activation, research and initial planning | 1–753 |
| [02 — Planning, research and owner decisions](agent-tasks/02-planning-research-and-owner-decisions.md) | Planning records, research evidence, XB-0084 to XB-0086, owner decision consolidation | 754–1520 |
| [03 — Technical delegation and architecture waves](agent-tasks/03-technical-delegation-and-architecture-waves.md) | Technical delegation, CPU execution foundations, architecture implementation waves | 1521–2224 |
| [04 — Documentation and autonomy corrections](agent-tasks/04-documentation-and-autonomy-corrections.md) | Change completion, owner documentation work, autonomy-policy corrections | 2225–2791 |
| [05 — GitHub and next development 01](agent-tasks/05-github-and-next-development-01.md) | GitHub enablement and XB-NEXT-DEVELOPMENT-01 | 2792–3178 |
| [06 — Next development 02](agent-tasks/06-next-development-02.md) | XB-NEXT-DEVELOPMENT-02: analyzer, CPU integration, reviews and completion | 3179–3642 |

## Ledger conventions

- Status values used by the historical records: `planned`, `in_progress`, `needs_owner`, `pass`, `blocked`, `superseded`.
- The archive files preserve the original evidence and historical wording. A historical status is not automatically the current project status.
- Historical `file:line` references inside the archives point to the former monolithic `docs/AGENT_TASKS.md`; use the archive table above to locate the corresponding section.
- New work packages should be recorded in a dedicated file under `docs/agent-tasks/` and added to this index. Do not append new package histories to an archive solely to preserve the old monolithic format.
- Do not mark a package `pass` unless its actual verification commands and results are recorded.
