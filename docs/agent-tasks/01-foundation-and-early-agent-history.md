# Agent Task Ledger — Archive 01

> Extracted verbatim from the former `docs/AGENT_TASKS.md`, original lines 1–753. Historical line references in older records refer to the former monolithic file. Content is preserved; see the index at `docs/AGENT_TASKS.md`.

# Agent Task Ledger

This is development infrastructure and is not part of the product package.

Status values: `planned`, `in_progress`, `needs_owner`, `pass`, `blocked`, `superseded`.

Machine-specific paths in this file are a mix of neutral placeholders (`<SETUP_WORKTREE>`, `<OPENCODE_CLI>`, `<PROJECT_ROOT>`, `<EXTERNAL_TEST_FILE>`, `<EXTERNAL_TEST_DIR>`, `<EXTERNAL_DATA>`) and literal historical paths or drive labels (for example `C:`, `E:`, `E:\XBridge 1.0\XBridge-Series-Github`, `E:\XBridge 1.0\worktrees\portability-worktree-test`, and the VS2026 executable paths at `:94-96`). They are historical evidence only and are not project dependencies. Status entries for XB-0084, XB-0085 and XB-0086 are recorded in the terminal status addendum so that existing line citations remain stable.

| ID | Type | Boundary | Status | Responsible | Result / Evidence | Open owner decision |
|---|---|---|---|---|---|---|
| SETUP-001 | FEATURE | development-only | superseded | coordinator | Agent system files created in `<SETUP_WORKTREE>`. Technical validation observed: config parsed, coordinator primary, five subagents recognized, reviewer edit deny present, hard policies blocked, external directory auto-rejected, coordinator delegated successfully. Superseded in location by SETUP-002; setup worktree remains intact. | None after SETUP-002 takeover, except worktree disposition |
| SETUP-PILOT-001 | RESEARCH | development-only | pass | coordinator | Harmless pilot executed: coordinator emitted complete envelope in reply only and launched reviewer once; reviewer returned exactly `PILOT_READ_OK`; no product file was written. | None |
| SETUP-002 | FEATURE | development-only | needs_owner | coordinator | Agent system transferred into `<PROJECT_ROOT>`, user-specific configuration paths removed, portable setup guide added, OpenCode 2.3.2 config validated from the main checkout. All five subagents, roles, command files and permission boundaries confirmed; harmless reviewer delegation returned `REVIEWER_REACHABLE_E`; project rules read from `<PROJECT_ROOT>`. | Review uncommitted changes; decide VS2026 vs VS2022 preset; decide disposition of the original setup worktree |
| SETUP-003 | RESEARCH | development-only | needs_owner | coordinator | Complete storage-location inventory performed. Project-required agent/config/doc files are present under `<PROJECT_ROOT>`; remaining C-only items are local program installations, personal data, regenerable state, and the untouched original setup worktree. No additional files were copied. | Decide whether to transfer any optional C-only artefacts; no second-PC portability test has been executed |
| SETUP-004 | FEATURE | development-only | needs_owner | owner | Portierung aller 13 vorhandenen OpenCode-Sitzungen: die drei XBridge-Projektsitzungen in das Hauptprojekt auf E:, die zehn Setup-Sitzungen in einen separaten, dauerhaften Portable-Setup-Kontext auf E:. **Weiterhin nicht abgeschlossen.** Kein Agent darf `opencode *` in der Shell ausführen (Rollendefinitionen), `session export/import` steht nicht in der globalen Allowlist, und Code Mode bietet dafür kein Tool. Das geprüfte `<EXTERNAL_DATA>\opencode-portable-state\opencode` ist leer; dort liegt keine importierte Sitzungsdatenbank. Originale auf C: bleiben unangetastet; keine aktive SQLite-Datei wird direkt kopiert. | Owner führt Export/Import manuell aus und meldet die tatsächlichen Ergebnisse; für Agents ist dieser Schritt nicht automatisierbar |
| SETUP-005 | BUGFIX | development-only | pass | owner + tester | Genehmigte Änderung ausgeführt und verifiziert: `CMakePresets.json:7` = `Visual Studio 18 2026`; `CMakePresets.json.backup:7` = `Visual Studio 17 2022`; `ninja-debug` unverändert. Der erste Testlauf scheiterte nur an der Pfadauflösung (`CommandNotFoundException`). Mit den aus dem CMakeCache belegten absoluten VS2026-Pfaden führte der Tester Configure (`exit 0`), Debug-Build (`exit 0`, `xbridge_core.lib`, `xbridge.exe`, `xbridge_tests.exe`) und CTest (`exit 0`, `1/1 Test`, `100% tests passed`) erfolgreich aus. `git diff --check`: nur CRLF-Hinweis. | Keine offene Entscheidung; `cmake`/`ctest` bleiben im normalen Agenten-Pfad nicht auffindbar (siehe SETUP-007) |
| SETUP-006 | RESEARCH | development-only | blocked | coordinator | Worktree-Test über die unterstützten OpenCode-Funktionen: `worktree_create({name:"portability-worktree-test"})` → `Created worktree could not be opened`. `worktree_list` zeigt danach weiterhin nur den C:-Setup-Worktree und das Hauptprojekt; `<PROJECT_ROOT>/../worktrees/portability-worktree-test` enthält jedoch einen vollständigen Checkout (`.git`, `src`, `tests`, `docs`, …). Keine spekulative Konfigurationsänderung, kein automatisches Entfernen. | Owner entscheidet über Bereinigung des zurückgebliebenen Verzeichnisses und über das weitere Vorgehen beim Worktree-Öffnen |
| SETUP-007 | FEATURE | development-only | blocked | coordinator + architect + reviewer | Aufbau des autonomen KI-Entwicklungsteams: Analyse abgeschlossen, neun Spezialisten und Delegationsmatrix entworfen, autonomer Zyklus dokumentiert. **Blockiert:** nur sechs Agentendateien installiert; Schreibversuche für neue Agentendateien → `Permission denied: edit`; persönliche Freigabe wurde nicht in `coordinator.md` persistiert. Reviewer-Verdikt (session `ses_dfc1366f2ffeL7y2bkyRNPraAs`): `NEEDS_CHANGE` (u. a. TEST-OWNERSHIP-Konflikt, veraltete `PORTABLE_SETUP.md`, SETUP-007-Status widersprüchlich). | Owner muss minimale Regeln in `coordinator.md` persönlich persistieren; Review-Findings für SETUP-007 ausstehend |
| RESEARCH-007 | RESEARCH | development-only | pass | researcher + architect + reviewer | Analyse von `omonkeyz/xbox-emulator` und Upstream `drycheetah84/TomBox` für XBridge. Nur 6 Agenten installiert; die neun angefragten Spezialagenten sind nicht eingerichtet/freigegeben und werden nicht aufgerufen – ihre Themenbereiche wurden von `architect` und `researcher` abgedeckt (Einschränkung dokumentiert). Abgeschlossen: 5 Doku-Dateien + Register-Eintrag + 4 abgeleitete Pakete (`planned`). Reviewer Runde 1 `NEEDS_CHANGE` → Korrekturpaket `RESEARCH-007-C1` → Finalreview `PASS`. Keine Codeübernahme, kein ADR, keine Produkt-/Test-/CMake-/Preset-/Agenten-Änderung. | Keine für die Forschung selbst; spätere Codeübernahme/ADR braucht eigene Owner-Entscheidung |
| XB-0081 | BUGFIX | development-only | pass | coordinator + reviewer | Vom Owner freigegebene Korrektur der Entwicklungsdokumentation in genau vier Dateien (`AGENT_WORKFLOW.md`, `ROADMAP.md`, `AGENT_TASKS.md`, `RELEASE_CHECKLIST.md`); `coordinator` einziger Schreiber. Installed-Role-Status präzisiert, `test-engineer` bleibend future-planned, „read-only"-Formulierung korrigiert, historische Snapshots als `superseded` markiert, explizite Unresolved-Liste ergänzt. Reviewer Runde 1 `NEEDS_CHANGE` (2 HIGH/3 MEDIUM/2 LOW) → Korrekturrunde 1 → Finalreview `PASS`. Keine Produkt-/Test-/CMake-/Preset-/Agenten-/Rechte-Änderung, kein Commit. 26 Working-Tree-Einträge erhalten; `git diff --check` `EXIT=0` (nur CRLF-Hinweis). | Keine; weiterhin blockiert: SETUP-004/006/007, veraltete `PORTABLE_SETUP.md` (Owner-Aktion). Keine weiteren Pakete ohne separate Freigabe |
| XB-AUTONOMY-ACTIVATION-01 | FEATURE | development-only | pass | coordinator + reviewer | Konfigurationsprüfung aller Ebenen abgeschlossen; alle fünf Subagenten erfolgreich aufgerufen; eine shadowed Git-Read-Regel im Coordinator korrigiert und verifiziert; veraltete Berechtigungsdoku in zwei gebündelten Runden korrigiert; Reviewer Runde 1 `NEEDS_CHANGE` (7 Findings) → C1 → Runde 2 `NEEDS_CHANGE` (1 offen) → C2 → Finalreview `PASS` (7/7 geschlossen, 0 neue Findings); kein Build/CTest/Produktcode, kein Commit/PR. Final: `git diff --check` `EXIT=0`, 3 Dateien; `93 insertions(+), 14 deletions(-)` zum Review-Zeitpunkt, `95 insertions(+), 14 deletions(-)` nach Ledger-Abschluss. | Extern: GitHub-CLI nicht im PATH (`gh` CommandNotFoundException); Agenten können `opencode debug *` nicht ausführen; Root-Laufwerkprüfung vom User abgelehnt |
| XB-AUTONOMY-ACTIVATION-02 | FEATURE | development-only | pass | coordinator + reviewer | Finaler Abgleich von Agentenregeln, Berechtigungen und Prozessdokumenten; veraltete Owner-Only-/Private-Repo-/Pfadangaben korrigiert; `opencode.jsonc` und sechs Agenten-Frontmatter unverändert und konsistent; kein Build/CTest/Produktcode. Reviewer Runde 1 `NEEDS_CHANGE` (2 ACTIONABLE) → C1 → Re-Check `NEEDS_CHANGE` (1 neue ACTIONABLE) → C2 → Finalreview `PASS` (3/3 Findings geschlossen, 0 neue, 2 Korrekturrunden). Final: `git diff --check` `EXIT=0`, 13 Dateien; `229 insertions(+), 64 deletions(-)` im Commit. `observed` — Commit `71faef915996ed4e459cf318111a80daa4719013` auf `main`; Push `8339582..71faef9  main -> main` nach `https://github.com/comedius/XBridge-Series.git`, `EXIT=0`. | `gh auth status` weiterhin `CommandNotFoundException`, `EXIT=1`; PR-Status `unknown` (kein CLI). Remote-Autorisierung über Git-Credentials `observed` (Push erfolgreich). |

Do not mark a package `pass` unless commands and results have actually been recorded.

## SETUP-001 validation evidence (historical)

All commands were executed from `<SETUP_WORKTREE>` with `<OPENCODE_CLI>` (OpenCode v2.3.2).

- `--version` → `opencode v2.3.2`.
- `debug config` → config sources `directory,document,directory`; the document source was the setup `opencode.jsonc`; `default_agent=coordinator`; policy `shell:git config --global *=deny` was loaded.
- `debug agents` → `coordinator` was `primary`; `architect`, `developer`, `tester`, `reviewer`, and `researcher` were `subagent`. Coordinator subagent rules allowed exactly those five after `*=deny`. Reviewer edit rules ended with `*=deny`.
- `run --agent general ... "git commit --help"` → shell tool error `Blocked by configuration policy`; command was not executed.
- `run --agent general ... "git push --help; git worktree --help; gh --help; git config --global --help; Remove-Item -WhatIf"` → all five attempts returned `Blocked by configuration policy`; none was executed.
- `run --agent general ... read <EXTERNAL_TEST_FILE>` → `permission requested: external_directory (<EXTERNAL_TEST_DIR>/*); auto-rejecting`; no file content was read.
- `run --agent coordinator ... launch reviewer` → `subagent(agent="reviewer")` succeeded; result `REVIEWER_REACHABLE`.
- `run --agent reviewer ... attempt one harmless edit` → blocked before any tool call; no file created.
- `run --agent coordinator ... SETUP-PILOT-001` → complete envelope in reply only; one reviewer launch with `PILOT_READ_OK`; no file writes, no chained subagents.

`unknown`: an interactive TUI permission prompt was not exercised; non-interactive runs auto-reject `ask` permissions. Shell pattern matching is not an OS sandbox.

## SETUP-002 validation evidence

All commands were executed from `<PROJECT_ROOT>` with `<OPENCODE_CLI>`.

- `--version` → `opencode v2.3.2`.
- `debug config` → document source `<PROJECT_ROOT>\opencode.jsonc`; `default_agent=coordinator`; `worktree.directory=../worktrees`; no hard-coded user-profile path string in the loaded project config.
- Schema check of the installed CLI binary → `Config.Worktree` with description `Directory defaults for local worktree creation` and `Parent directory for new worktrees, relative to the project's primary checkout when not absolute`. The configured option is therefore supported in v2.3.2. Actual creation of a new worktree was **not** verified (the earlier setup attempt reported `Created worktree could not be opened`); the option was not changed on speculation.
- `debug agents` → `coordinator` is `primary`; `architect`, `developer`, `tester`, `reviewer`, `researcher` are `subagent`. Effective rules:
  - coordinator: `subagent:*=deny` then exactly five `allow` rules; edits limited to `docs/AGENT_TASKS.md`, `docs/AGENT_WORKFLOW.md`, `docs/ROADMAP.md`, `docs/RELEASE_CHECKLIST.md`.
  - architect and reviewer: final `edit:*=deny`.
  - developer: `edit:src/**`, `edit:include/**`, `edit:tests/**`, `edit:CMakeLists.txt` allowed; protected docs/config denied.
  - tester: only `edit:tests/**` allowed.
  - researcher: only `edit:docs/RESEARCH_REGISTER.md` and `edit:docs/research/**` allowed.
  - every subagent: `subagent:*=deny`.
- Hard policies read back from `debug config`: all Git publication/history commands, `gh *`, `git config --global *`, destructive shell commands, install commands, and `.env`/`.git` read/edit denies are present. No artificial Git write attempt was performed in this package.
- `run --standalone --agent coordinator "… launch the reviewer subagent once …"` → one subagent session completed with exactly `REVIEWER_REACHABLE_E`; no file edits, no chained subagents.
- `run --standalone --agent coordinator "Quote verbatim the first line of AGENTS.md and docs/AGENT_WORKFLOW.md …"` → `read` calls resolved to `<PROJECT_ROOT>\AGENTS.md` and `<PROJECT_ROOT>\docs\AGENT_WORKFLOW.md`; first lines were `# XBridge Series – Agent Instructions` and `# XBridge Agent Workflow`. Two attempted `Get-Content` shell calls were auto-rejected by the non-interactive `ask` permission, as expected; the `read` tool succeeded.
- Static scan of all non-`.git`, non-`build` files under `<PROJECT_ROOT>` for absolute user-profile paths, personal account names, fixed program/install paths, fixed drive-letter paths and personal directory variables: no matches.
- `git status --short --branch` → `## main...origin/main` with only the expected development-infrastructure modifications; `git diff --check` → no output.

`observed` label applies to every result above. `unknown`: interactive TUI permission flow, real worktree creation, and any second-PC portability test.

## SETUP-003 evidence

See the "Vollständige Speicherort-Inventur" section of the final report. No additional files were copied, moved, or deleted for this package.

## SETUP-004 / 005 / 006 evidence (current session)

All commands were executed from `<PROJECT_ROOT>` = `E:\XBridge 1.0\XBridge-Series-Github` on branch `main`.

- `Get-Location` → `E:\XBridge 1.0\XBridge-Series-Github`.
- `git branch --show-current` → `main`.
- `git status --porcelain=v1` → 9 top-level entries; `git status --porcelain=v1 --untracked-files=all` → exactly **18 entries**, matching the existing infrastructure changes. Nothing was discarded or overwritten.
- `git diff --check` → no output (before any documentation edit in this session).
- `opencode --version`, `opencode -v`, `opencode session --help`, `opencode debug paths`, `opencode api get /api/info`, `cmake --version` → each returned `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}`.
- Web fetch of the CLI documentation → `Permission denied: webfetch` (global `webfetch:*=deny` in `opencode.jsonc:51`).
- Code Mode `search({namespace:"opencode"})` → 8 tools (`session_move`, `session_rename`, `worktree_create`, `worktree_list`, `worktree_remove`, `models`, `list_mcp_resources`, `read_mcp_resource`); no session export/import tool exists.
- Read-only rule inspection: `.opencode/agents/coordinator.md:50-58` denies shell `cmake *`, `ctest *`, `opencode *`; `developer.md:53-55`, `tester.md:17-19`, `architect.md:14-22`, `reviewer.md:14-28` deny shell `opencode *`. `opencode.jsonc:41-47` allows only `opencode --version*`, `-v*`, `models*`, `api get *`, `debug paths*`, `mcp list*`, `run *`. Therefore **no agent** can run `opencode session export/import`; the owner performs those commands manually.
- At the time of the first pass, `CMakePresets.json:7` read `"generator": "Visual Studio 17 2022"`; `ninja-debug` (lines 12-19) was unchanged. No agent has edit permission for this file. The owner subsequently performed the approved edit; see the second-pass evidence below.
- `tools.opencode.worktree_list()` before creation → `<SETUP_WORKTREE>` (original setup worktree on `C:`) and `<PROJECT_ROOT>` (`E:`). The C: setup worktree was not touched.
- `tools.opencode.worktree_create({name:"portability-worktree-test"})` → `Created worktree could not be opened`. Per the approved package, this part was stopped immediately; no speculative config change and no automatic removal.
- `tools.opencode.worktree_list()` after creation → unchanged (new worktree not registered).
- Directory read of `E:\XBridge 1.0\worktrees\portability-worktree-test` → complete checkout present (`.git`, `.github`, `ai`, `docs`, `include`, `src`, `tests`, `AGENTS.md`, `CMakeLists.txt`, `CMakePresets.json`, `LICENSE`, `README.md`). This is an unregistered leftover, awaiting an owner decision.

`observed` applies to all results above. `documented`: the approved owner decisions that the owner performs session export/import and edits `CMakePresets.json` / `docs/PORTABLE_SETUP.md` manually. `unknown`: OpenCode's exact `session export/import` syntax in the installed version, VS2026 compatibility on this machine, successful worktree opening on E:, and any second-PC test.

## SETUP-004 / 005 / 006 evidence (second pass, coordinator + tester)

- `git status --porcelain=v1 --untracked-files=all` → **20 entries** (previously 18): `M CMakePresets.json` and `?? CMakePresets.json.backup` were added; all 18 original entries remain intact.
- `CMakePresets.json:7` → `"generator": "Visual Studio 18 2026"`; `CMakePresets.json.backup:7` → `"generator": "Visual Studio 17 2022"`; `ninja-debug` line 14 → `"generator": "Ninja"` (unchanged). The VS2022 line is preserved as an untracked backup file, not deleted.
- `build/**` contains three directories: `windows-debug`, `windows-debug-vs2022`, `windows-2026-debug`. `build/windows-debug-vs2022` contains VS solution/project files but **no `CMakeCache.txt`**, so it is not a complete independently usable cache backup. `build/windows-2026-debug/CMakeCache.txt` and `build/windows-debug/CMakeCache.txt` both report `CMAKE_GENERATOR:INTERNAL=Visual Studio 18 2026` and `CMAKE_HOME_DIRECTORY:INTERNAL=E:/XBridge 1.0/XBridge-Series-Github`.
- `build/windows-debug/Testing/Temporary/LastTest.log` (timestamp Nov 22 16:28) records `1/1 Test: xbridge_tests`, `Test Passed.` and `All tests passed.` This is a prior run, not a fresh verification by this session.
- Tester session `ses_dfc1cd051ffeCYMz0vZ22OJSvQ`: `cmake --preset windows-debug`, `cmake --build --preset windows-debug --config Debug`, `ctest --test-dir build/windows-debug -C Debug --output-on-failure` each returned exit `1` with `CommandNotFoundException` for `cmake` / `ctest` in the localized PowerShell shell; the executables were never started. `git diff --check` returned exit `0` with only the informational CRLF warning for `CMakePresets.json`.
- The absolute executables are recorded in `build/windows-debug/CMakeCache.txt` lines 3 and 219–223 under `E:/PRO/Programm Daten/Visual Studio/18/Community/.../CMake/CMake/bin/`. Follow-up tester run with exactly those paths, all exit `0`:
  - `& 'E:\PRO\Programm Daten\Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --preset windows-debug` → `Configuring done (0.0s)`, `Generating done (0.5s)`, build files written to `build/windows-debug`.
  - `& 'E:\PRO\Programm Daten\Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --build --preset windows-debug --config Debug` → MSBuild 18.12.3; `xbridge_core.lib`, `xbridge.exe`, `xbridge_tests.exe` produced in `build/windows-debug/Debug`.
  - `& 'E:\PRO\Programm Daten\Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure` → `1/1 Test #1: xbridge_tests ... Passed`, `100% tests passed, 0 tests failed out of 1`.
  - Post-check `git diff --check` → exit `0`, only the informational CRLF warning for `CMakePresets.json`. `git status --porcelain=v1 --untracked-files=all` → unchanged at 20 entries.
  - `unknown`: plain `cmake`/`ctest` in other shells or for other users was not verified.
- Directory reads: `<EXTERNAL_DATA>\opencode-portable-state\opencode` is empty; `<EXTERNAL_DATA>\opencode-portable-data\opencode` contains only `log\` and `repos\`. No imported session database was found in the inspected portable context. The 13-session transfer remains incomplete.
- Attempted edit of `docs/PORTABLE_SETUP.md` → `{"type":"permission.rejected","message":"Permission denied: edit"}`. Although the owner authorized the content change, `.opencode/agents/coordinator.md:5-19` has `edit:*=deny` with only four allowed documentation paths; the file is not among them. A minimal, scoped allow rule is required before this agent can write it.
- `tools.opencode.worktree_list()` (second pass) → unchanged: original setup worktree on C: and the main checkout on E:. The leftover test checkout is still unregistered.
- Permission syntax was verified against the installed, working agent files (OpenCode 2.3.2): YAML frontmatter with `description`, `mode: subagent`, and a `permissions:` list of `{action, resource, effect}` entries using glob resources; last matching rule wins. No web documentation was needed; the six installed agent files are the authoritative local examples.
- Owner granted the expansion in conversation and added constraints: write only the nine new agent files; do not modify existing agent files (especially `coordinator.md`, `architect.md`, `reviewer.md`, `tester.md`); `docs/PORTABLE_SETUP.md` may be updated; subagent calls limited to the nine new roles; all existing shell/Git/publication denials stay.
- Read-back of `.opencode/agents/coordinator.md:1-60` after the grant → unchanged; still `edit:*=deny`, `edit:.opencode/**=deny`, only four allowed doc paths, and only five `subagent ... allow` rules. A conversation answer does not change the configuration file.
- Six `write` attempts for `.opencode/agents/{cpu-developer,memory-developer,gpu-developer,audio-developer,input-developer,platform-developer}.md` → each `{"type":"permission.rejected","message":"Permission denied: edit"}`. No partial file was created; Git status stayed at 20 entries.
- Code Mode `search({query:"permission request approve grant"})` → no dynamic permission-request tool exists. The grant must be persisted in `coordinator.md` by the owner, or the owner must authorize this coordinator to edit the minimal lines there. This is a genuine approval boundary, not a routine action.
- Architect review (session `ses_dfc19bba3ffehxEPASRXRmFbPh`) identified and the coordinator resolved: test-path overlap (`tests/**` was planned as exclusive to the future-planned, not-installed `test-engineer` — see the supersession note below for installed reality), GPU/audio/input path ambiguity (`docs/design/<role>/**`), integration doc conflict (`integration-engineer` is read-only), and stale claims about VS2026/role counts in the docs.

`documented`: owner authorization for the coordinator to correct `PORTABLE_SETUP.md` and to add nine specialists; owner constraint forbidding edits to existing agent files; approved VS2026 package scope. `observed`: all commands and file contents above; fresh VS2026 configure/build/CTest passed via absolute paths; agent-file writes rejected because the grant is not persisted in `coordinator.md`. `hypothesis`: `cmake`/`ctest` are absent from the agent shell `PATH` although installed under the VS2026 directory. `unknown`: exact session-export syntax, worktree opening, plain `cmake` availability in other shells, and second-PC test.

## XB-0081 — owner approval and execution evidence (pass; reviewer verdict recorded)

`documented` — owner approved XB-0081 with these conditions: only `docs/AGENT_WORKFLOW.md`, `docs/ROADMAP.md`, `docs/AGENT_TASKS.md`, `docs/RELEASE_CHECKLIST.md` may be edited; `coordinator` is the sole writer; `reviewer` performs an independent check afterwards; `test-engineer` remains future-planned and is never presented as installed; Roadmap item 27 stays blocked; no changes to agent files, `PORTABLE_SETUP.md`, product code, tests, CMake or permissions; no commits/pushes/merges/releases; preserve all existing working-tree changes; no further implementation package without separate approval.

`observed` — supersession note, not a deletion of evidence: the first-pass snapshots at `:71-83` (18/20 status entries, VS2022 at `CMakePresets.json:7`, `unknown` VS2026 compatibility) are historical and superseded by the verified second-pass evidence at `:87-97`. The statement at `:106` that `tests/**` is "now exclusive to `test-engineer`" is superseded by the PLAN-001 audit: that role is not installed and both `developer.md:15` and `tester.md:9` allow `tests/**` writes; exclusive ownership applies only after installation. The `LastTest.log` timestamp at `:90` is a prior run, not fresh verification. Current status evidence is this section.

`observed` — coordinator-only edits: `docs/AGENT_WORKFLOW.md` (developer role at `:42`, installed-role note `:49`, standard flow `:57`, permission wording `:86`, autonomous-cycle note `:150`, steps `:157-160`, planned ownership `:185`, delegation note `:191`); `docs/RELEASE_CHECKLIST.md` (`:18`, `:52`); `docs/ROADMAP.md` (queue item 7: approved/in_progress, item 27 untouched); `docs/AGENT_TASKS.md` (this section). No other file changed; no rule, agent file, product source, test, CMake or preset touched.

`observed` — verification after the edits, executed by the coordinator: `git branch --show-current` → `main`; `git status --porcelain=v1 --untracked-files=all` → 26 entries (5 modified, 21 untracked), unchanged; `git diff --check; "EXIT=$LASTEXITCODE"` → `warning: in the working copy of 'CMakePresets.json', CRLF will be replaced by LF the next time Git touches it`, `EXIT=0`. Read-back of all changed sections confirmed the intended wording. Build and CTest were not run and are not claimed (documentation-only package).

`observed` — explicit unresolved-items list required by ACCEPTANCE (4), recorded here rather than silently closed:
1. **Agent-file overlap / test ownership:** installed `developer.md:15` and `tester.md:9` both allow `tests/**`; only the uninstalled, future-planned `test-engineer` claims exclusivity (`docs/AGENT_WORKFLOW.md:181,185`). No deny/permission rule was changed. Resolved only if the owner installs and permits that role.
2. **`opencode.jsonc` global rules:** `:51-52` deny `webfetch`/`websearch` globally while the researcher agent file allows them; the "last matching rule wins" layering (`docs/AGENT_WORKFLOW.md:95`) makes the researcher allows effective, so this is a documented non-defect boundary — but the global rule set remains unchanged and outside this package's write set.
3. **Stale `docs/PORTABLE_SETUP.md`:** `:59` and `:97` still claim VS2022 / an open preset decision although VS2026 is verified. Editing that file is denied to `coordinator` (`coordinator.md:5-19`); a separate owner action is required.
4. **SETUP-007 / SETUP-006 status:** both remain `blocked` (`docs/AGENT_TASKS.md:17-18`, `docs/ROADMAP.md:24-25`); neither was closed or marked pass. Agent installation, persisted permissions, session transfer and worktree cleanup await owner decisions.

`observed` — independent review round 1 (session ses_dfbb81b13ffelb5uZjR2lRGJJ7) returned **NEEDS_CHANGE** with 2 HIGH, 3 MEDIUM and 2 LOW findings, all documentation-only: (HIGH) no explicit unresolved-items list and `opencode.jsonc` global rules not marked unresolved; (HIGH) pre-approval "not started / awaiting approval" text not marked superseded; (MEDIUM) stale `file:line` references after the 12-line insertion; (MEDIUM) `tests/**` "exclusive" claims not qualified future-planned in situ; (MEDIUM) PLAN-001 conflict list presented as current. No product/test/CMake/preset/permission/agent-file change and no publication defect was found.

`observed` — correction round 1 (coordinator, same four files) applied: added the four-item unresolved-items list (agent-file overlap, `opencode.jsonc` global rules, stale `PORTABLE_SETUP.md`, SETUP-007/006 status); marked the derived-packages snapshot, the pre-approval decision template and the PLAN-001 closing note as superseded; converted shifted internal `file:line` references to stable field/section anchors and updated stable `AGENT_WORKFLOW.md` references to current values (`:84`, `:86`, `:181,185`); qualified all remaining "exclusive `tests/**`" claims as future-planned; marked the PLAN-001 conflict list as a historical snapshot with inline resolutions.

`observed` — verification after correction round 1: `git branch --show-current` → `main`; status → 26 entries (unchanged); `git diff --check; "EXIT=$LASTEXITCODE"` → known CMakePresets.json CRLF warning, `EXIT=0`. File mtimes confirm only the four approved docs were touched in this package (AGENT_TASKS.md correction round at 18:29, other three at 18:21); `PORTABLE_SETUP.md`, `opencode.jsonc`, `CMakeLists.txt` and `CMakePresets.json` retain earlier timestamps. No build/CTest run (documentation-only).

`observed` — independent review round 2 (same session) returned **PASS** with all seven round-1 findings resolved and no blocking defect. Reviewer re-ran `git status --porcelain=v1 --untracked-files=all` → 26 entries (`STATUS_EXIT=0`), `git diff --check; Write-Output "EXIT=$LASTEXITCODE"` → known CRLF warning, `EXIT=0`, and `git log -3` → head `10e5e75` unchanged (no commit/publication). Confirmed: only the four approved docs touched; Roadmap item 27 still blocked; `test-engineer` still future-planned; SETUP-006/007 still `blocked`; no invented build/CTest claim. Four non-blocking LOW housekeeping notes remained (literal escaped-newline sequences in the round-1 record, a supersession quote at `:106`, an immaterial 18:28-vs-18:29 mtime note, and item 4 only indirectly cross-referenced); the escaped-newline formatting was corrected by the coordinator after the verdict. XB-0081 is complete with reviewer `PASS`.

## SETUP-007 independent reviewer verdict (session ses_dfc1366f2ffeL7y2bkyRNPraAs)

`observed` — verdict `NEEDS_CHANGE`. Severity-ordered findings: (HIGH) planned exclusive `tests/**` ownership by `test-engineer` conflicts with installed `developer` and `tester` write rights and the old `Roles`/`Standard flow` text; (HIGH) `docs/PORTABLE_SETUP.md:59,97` still claims VS2022 and an open preset decision although the preset is verified VS2026 — coordinator cannot edit that file (permission); (MEDIUM) ledger lines 13/18/89 contain stale snapshots (VS decision open, architect review "läuft", `LastTest.log` timestamp); (MEDIUM) `docs/AGENT_WORKFLOW.md:84` calls CMake/CTest commands "read-only" although they write build outputs; (MEDIUM) old and new process models coexist in `docs/AGENT_WORKFLOW.md`; (MEDIUM) AI-boundary paragraph sits in `docs/ARCHITECTURE.md` whose product-package classification is not explicit; (LOW) `worktree_remove` is referenced but absent from the current Code Mode opencode tool set. Reviewer confirmed: complete SETUP-007 envelope, honest installed-vs-planted claims, no nine agent files created, disjoint planned write paths, no product/test source changes. Coordinator disposition: HIGH test-ownership conflict is a planned-role documentation issue only (no specialist installed); PORTABLE_SETUP stale claim requires the persisted edit permission from SETUP-007 (owner blocker); remaining documentation cleanups are queued after RESEARCH-007, not silently closed.

## XB-AUTONOMY-ACTIVATION-01 — configuration audit and autonomy activation

```text
PACKAGE_ID:        XB-AUTONOMY-ACTIVATION-01
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  development-only
GOAL:              Establish and verify the actually effective autonomy of the six installed project agents after PR #18.
FILES:             docs/AGENT_TASKS.md; and only if a verified defect requires it: opencode.jsonc, .opencode/agents/coordinator.md, .opencode/agents/{architect,researcher,developer,tester,reviewer}.md
ALLOWED:           Hierarchical configuration inspection; read-only agent-access probes; scoped configuration corrections inside the project; documentation of exact commands and results; independent reviewer check.
FORBIDDEN:         Product code, tests, CMake/presets, license, personal files, secrets, destructive Git commands, build/CTest/function tests, publication without a separate reviewed package.
ACCEPTANCE:        All five subagents invokable from the coordinator; effective tool rights recorded with observed evidence; Git/GitHub rights separated into configured vs effective; every correctable defect fixed and reverified; no build/CTest run.
TESTS:             Allowed read-only shell probes; five subagent access probes; reviewer config review; git status --porcelain=v1 --untracked-files=all; git diff --check. No development, build or functional tests.
REVIEW:            Architect used for permission-layer/boundary analysis; independent reviewer verdict after corrections.
WORKTREE:          Main checkout D:\XBridge 1.0\XBridge-Series-Autonomy on branch main.
OWNER_APPROVALS:   Owner explicitly authorized all project-scoped configuration corrections in this package; no product, license or destructive decision included.
```

`observed` — configuration audit start: branch `main`; `git status --porcelain=v1 --untracked-files=all` produced no output (clean tree); `git rev-parse HEAD` → `833958233cd3864cc04c716a4236207090e00987`; `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0`; `git branch --show-current` → `main`; `gh auth status` → `CommandNotFoundException` for `gh` (exit 1). No build, CTest or product test was started.

`observed` — `Start-Process -FilePath "cmd.exe" -PassThru | Select-Object Id, ProcessName` → `permission.rejected: shell`; the command is not in the allowlist and therefore the agent-level `shell:*=deny` remains effective. This is expected and not a defect of autonomy.

`observed` — hierarchy: `C:\ProgramData\opencode` → `File not found` (no system-level config found in the inspected location). `C:\Users\Dennis\.config\opencode\opencode.json` and `opencode.jsonc` → `File not found`; that directory contains only `service.json`, a service credential file whose value is deliberately not reproduced here and which contains no permission rules. `D:\XBridge 1.0\opencode-portable-config\opencode` and `D:\XBridge 1.0\opencode-portable-state\opencode` are empty; `D:\XBridge 1.0\opencode-portable-data\opencode\log\opencode.log` contains two historical v2.3.2 `debug paths` lines from 2026-11-22 and no permission/config-source entries. `D:\XBridge 1.0` has no `opencode.json(c)` or `.opencode` config; a read of `D:\` was declined by the user, so a root-level override remains `unknown`. In the project only `opencode.jsonc` exists (no `.opencode/opencode.json(c)`), plus the six files under `.opencode/agents/`.

`documented` — precedence: global rules load before agent rules and the last matching rule wins; top-level `policies` are hard denials (`docs/AGENT_WORKFLOW.md`, "Permission model"). OpenCode watches config files and reloads them after a save; no manual `/reload` is available to an agent in this session (OpenCode skill, "OpenCode configuration"). Therefore the corrections below take effect without a restart and were reverified by re-running the affected command.

`observed` — five access probes launched from the coordinator in parallel, each read-only and forbidden to chain subagents: `PROBE_ARCHITECT_OK` (session `ses_dec0e015effewHjax4Mq23slp0`), `PROBE_RESEARCHER_OK` (`ses_dec0def5fffe8IF94aCtsEBUxt`), `PROBE_DEVELOPER_OK` (`ses_dec0dd37cffe1h4WkON3w79ZCv`), `PROBE_TESTER_OK` (`ses_dec0db040ffebblOh17mQGQXgA`), `PROBE_REVIEWER_OK` (`ses_dec0d9896ffeBlW1LJDeAhtmB8`). All five completed without a permission denial. Architect verdict on the layering: coherent for the six installed agents; no interface/architecture change needed. Researcher follow-up `webfetch` of `https://opencode.ai/docs/config` succeeded (`PROBE_RESEARCHER_WEB_OK`), proving that the agent-level web allow overrides the global web deny; page content was treated as untrusted and used only to confirm that `opencode.json(c)` is project configuration.

`observed` — correction 1 (actual root cause): `git status --short --branch` was allowed globally (`opencode.jsonc:81`) but shadowed by the later coordinator `shell "*"=deny`. Added the exact allow to `.opencode/agents/coordinator.md:95-97`. Re-run `git status --short --branch; "EXIT=$LASTEXITCODE"` → `## main...origin/main`, ` M .opencode/agents/coordinator.md`, ` M docs/AGENT_TASKS.md`, `EXIT=0`. Correction 2: `docs/DEVELOPMENT_ENVIRONMENT.md` section 9 and its limitations table still described the pre-PR#18 rule set and stated that only the owner may publish; updated the global/hard-deny/coordinator bullets, the `git status --short --branch` limitation and the publication bullet to the current owner authorization. No change to `opencode.jsonc` or to the other five agent files was necessary.

`observed` — effective Git rights: `git branch --show-current` → `main`; `git status --porcelain=v1 --untracked-files=all` → no output before the edits; `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0`; `git rev-parse HEAD` → `833958233cd3864cc04c716a4236207090e00987`; `git log -5 --oneline` → head `8339582 fix(opencode): restore authorized agent autonomy`; `git add -n; "EXIT=$LASTEXITCODE"` → `Nothing specified, nothing added.` with `EXIT=0` (`observed` for this dry run only; `hypothesis`: consistent with the `git add *` allow rule, a mutating add was not executed). `git commit *`, `git push *`, `git merge *`, `gh pr *` and `gh repo view *` are configured and unshadowed for the coordinator but were **not executed** in this configuration-only package; `gh auth status` was executed and failed as recorded below.

`observed` — GitHub obstacle: `gh auth status` → `CommandNotFoundException` for `gh`, exit 1; no `gh.exe`/`gh.cmd` was found in `C:\Program Files`, `C:\Program Files (x86)`, `C:\Users\Dennis\AppData\Local` or `D:\XBridge 1.0`. The permission rules are present but the tool is unavailable in this shell. `unknown`: GitHub authentication state, remote write access, and whether `gh` exists outside the inspected locations. Owner action is required only if PR management is wanted now: install/expose GitHub CLI or provide an approved authenticated alternative; no installation was attempted.

`observed` — autonomy boundaries: `opencode --version` etc. remain denied for all project agents by `shell "opencode *" deny`; the hard policies in the top-level `policies` array (`opencode.jsonc:142-172`) still deny history rewriting, worktree Git commands, global Git configuration, destructive shell commands and `.git`/`.env` reads and edits. The four Code Mode identities (`automation_create`, `automation_delete`, `opencode_session_move`, `opencode_worktree_create`) are **configured global permission denies** at `opencode.jsonc:134-139`, not hard policies; their runtime effect is `unknown`. No owner decision was needed for this package because the owner pre-authorized all project-scoped configuration corrections. Automatic start of the next package is a ledger/envelope process (`docs/AGENT_TASKS.md`, one coordinator-owned writer, one reviewer), not a scheduler; no automation was created.

`observed` — final self-check of the uncorrected base state: `git diff HEAD --no-ext-diff --stat` → touched files `.opencode/agents/coordinator.md`, `docs/AGENT_TASKS.md`, `docs/DEVELOPMENT_ENVIRONMENT.md`, summary `3 files changed, 45 insertions(+), 6 deletions(-)`; `git status --porcelain=v1 --untracked-files=all` → exactly those three modified files; `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0`. This stat describes the state before correction round C1 and is not claimed as the final C1 diff.

### XB-AUTONOMY-ACTIVATION-01-C1 — bundled review corrections

```text
PACKAGE_ID:        XB-AUTONOMY-ACTIVATION-01-C1
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Resolve all six ACTIONABLE reviewer findings from review round 1 in one coordinator-owned correction round.
FILES:             docs/AGENT_TASKS.md; docs/DEVELOPMENT_ENVIRONMENT.md
ALLOWED:           Correct evidence consistency, hard-policy wording, the final diff-stat record, stale publication/path statements, stale line citations and the diff-check exit-code state.
FORBIDDEN:         Product/test/build/license files, new permissions beyond the one documented allow, publication, build/CTest, downgrading a finding.
ACCEPTANCE:        No ACTIONABLE finding remains open; mandatory independent re-review with file:line evidence; git diff --check EXIT=0.
TESTS:             Read-only Git re-check (diff --check, status, diff stat) and reviewer re-review. No build/CTest.
REVIEW:            Independent re-check by the same reviewer session.
WORKTREE:          Main checkout D:\XBridge 1.0\XBridge-Series-Autonomy on branch main.
OWNER_APPROVALS:   Covered by the owner authorization for XB-AUTONOMY-ACTIVATION-01; no new owner decision.
```

`documented` — disposition of review round 1: `REV-001`, `REV-004`, `REV-006` (AGENT_TASKS evidence/wording), `REV-002`, `REV-003`, `REV-005` (DEVELOPMENT_ENVIRONMENT stale statements/citations) are `ACTIONABLE` and bundled here. `REV-007` is classified `UNKNOWN` by the reviewer; the exact diff stat is recorded voluntarily because the coordinator executed that command. No `OWNER_DECISION` finding, no `NON_BLOCKING` finding, and no severity downgrade.

### XB-AUTONOMY-ACTIVATION-01-C2 — final bundled correction round

```text
PACKAGE_ID:        XB-AUTONOMY-ACTIVATION-01-C2
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Close the single ACTIONABLE finding REV-003 that remained open after round C1.
FILES:             docs/DEVELOPMENT_ENVIRONMENT.md; docs/AGENT_TASKS.md (ledger record only)
ALLOWED:           Correct the remaining current-path table row to the actual workspace and mark the obsolete checkout paths as historical; record the round in the ledger.
FORBIDDEN:         Any new permission, product/test/build/license change, publication, or reopening of closed findings.
ACCEPTANCE:        No statement in DEVELOPMENT_ENVIRONMENT.md calls XBridge-Series-Github the current working directory; reviewer re-check after this round.
TESTS:             Read-only git diff --check and status; reviewer re-review. No build/CTest.
REVIEW:            Mandatory final independent re-check; this is correction round 2 of at most 2.
WORKTREE:          Main checkout D:\XBridge 1.0\XBridge-Series-Autonomy on branch main.
OWNER_APPROVALS:   Covered by the owner authorization for XB-AUTONOMY-ACTIVATION-01.
```

`documented` — review round 2 disposition: `REV-003` remained `STILL_OPEN` (`docs/DEVELOPMENT_ENVIRONMENT.md:86`); all six other findings were `CLOSED`, no new finding was raised. `REV-003` is `ACTIONABLE`/`MEDIUM`, root cause = one missed table row in the same stale-path correction; it is fixed in this single final bundled round. After this round there is no further correction round available.

`observed` — final verification after C2: `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0`; `git status --porcelain=v1 --untracked-files=all` → exactly `.opencode/agents/coordinator.md`, `docs/AGENT_TASKS.md`, `docs/DEVELOPMENT_ENVIRONMENT.md`; `git diff HEAD --no-ext-diff --stat` → `3 files changed, 93 insertions(+), 14 deletions(-)` zum Review-Zeitpunkt; nach diesem Ledger-Abschluss erneut ausgeführt → `3 files changed, 95 insertions(+), 14 deletions(-)`, `git diff --check` weiterhin `EXIT=0`. Independent final re-check (session `ses_dec0d9896ffeBlW1LJDeAhtmB8`) → **PASS**: 7/7 prior findings `CLOSED`, 0 new findings, 2 of 2 correction rounds used. `documented`: the reviewer had no shell and did not independently re-execute the coordinator's Git commands. No commit, push, merge, PR, build or CTest was performed; XB-AUTONOMY-ACTIVATION-01 is complete.

## XB-AUTONOMY-ACTIVATION-02 — final autonomy alignment and commit

```text
PACKAGE_ID:        XB-AUTONOMY-ACTIVATION-02
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  development-only
GOAL:              Align all project-side agent, permission and workflow documentation with the actually effective autonomous operation of the six installed agents, then commit the reviewed result.
FILES:             docs/AGENT_TASKS.md; docs/AGENT_WORKFLOW.md; docs/AGENT_AUTONOMY.md; docs/AGENT_CORE_ARCHITECTURE.md; docs/GITHUB_AGENT_WORKFLOW.md; docs/DESKTOP_MIGRATION_CHECKLIST.md; docs/OPENCODE_AUTOMATION_PLAN.md; docs/DEVELOPMENT_ENVIRONMENT.md; docs/RELEASE_CHECKLIST.md; docs/ROADMAP.md; docs/ARCHITECTURE.md; ai/README.md; carry-over .opencode/agents/coordinator.md from XB-AUTONOMY-ACTIVATION-01.
ALLOWED:           Read-only hierarchy/config/document audit; coordinator-only corrections to these project-side documents; ledger/roadmap records; independent reviewer check; authorized commit/push after PASS.
FORBIDDEN:         Product code, tests, CMake/presets, LICENSE, personal files, secrets, destructive Git commands, history rewriting, new permissions, build/CTest/function tests, PR creation without a separate reviewed need.
ACCEPTANCE:        No unqualified stale claim of private repository, owner-only routine publication, obsolete current path/branch or inconsistent autonomy grade remains in the checked current-state documentation; six agent roles and effective rights remain coherent; reviewer PASS; git diff --check EXIT=0; reviewed changes committed on main.
TESTS:             Exact read-only Git commands and read-back of affected files; independent reviewer review. No build, CTest or product function tests by owner instruction.
REVIEW:            Architect skipped: pure documentation/process alignment with no product interface, module boundary, ADR or architecture-semantic change; prior architect verdict on six-agent layering remains valid. Independent reviewer mandatory.
WORKTREE:          Main checkout D:\XBridge 1.0\XBridge-Series-Autonomy on branch main.
OWNER_APPROVALS:   Owner explicitly authorized all project-scoped corrections and the final commit; push only if remote/authentication works; no product, license, permission or destructive decision included.
```

`observed` — audit start from the main checkout: `git branch --show-current` → `main`; `git rev-parse HEAD` → `833958233cd3864cc04c716a4236207090e00987`; `git status --porcelain=v1 --untracked-files=all` → exactly `.opencode/agents/coordinator.md`, `docs/AGENT_TASKS.md`, `docs/DEVELOPMENT_ENVIRONMENT.md`; `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0`; `git diff HEAD --no-ext-diff --stat` → `3 files changed, 95 insertions(+), 14 deletions(-)`. These three carry-over changes from XB-AUTONOMY-ACTIVATION-01 were preserved, not reset or discarded.

`observed` — inspected configuration layers and files: `opencode.jsonc` (global permissions, subagent allows, hard policies, built-in agents); all six `.opencode/agents/*.md`; `AGENTS.md`; `docs/AGENT_WORKFLOW.md`; `docs/AGENT_TASKS.md`; `docs/ROADMAP.md`; `docs/DEVELOPMENT_ENVIRONMENT.md`; `docs/RELEASE_CHECKLIST.md`; `docs/ARCHITECTURE.md`; `docs/AGENT_AUTONOMY.md`; `docs/AGENT_CORE_ARCHITECTURE.md`; `docs/GITHUB_AGENT_WORKFLOW.md`; `docs/DESKTOP_MIGRATION_CHECKLIST.md`; `docs/OPENCODE_AUTOMATION_PLAN.md`; `docs/AUTOMATED_QUALITY_GATES.md`; `docs/PORTABLE_SETUP.md`; `ai/README.md`; `.github/workflows/windows.yml`. No agent file or `opencode.jsonc` defect was found: global rules load before agent rules, later matching agent allows override earlier denies, top-level `policies` remain hard denials, all five subagent allows exist, all subagents deny chained delegation and independent publication, developer/tester may write `tests/**` only with an explicit sole-writer package assignment, researcher web access works, and reviewer/architect remain read-only.

`documented` — root causes corrected in this package: (1) several current-state sections still described the repository as private although the owner policy in `docs/AGENT_WORKFLOW.md` and the repository policy sections of `ARCHITECTURE.md`, `ROADMAP.md`, `RELEASE_CHECKLIST.md`, `LICENSE_STATUS.md` and `PORTABLE_SETUP.md` make it public with a packaging-only boundary; (2) `AGENT_AUTONOMY.md`, `AGENT_CORE_ARCHITECTURE.md`, `GITHUB_AGENT_WORKFLOW.md`, `OPENCODE_AUTOMATION_PLAN.md` and `ai/README.md` still described routine staging/commit/push/PR/merge as owner-only although `AGENTS.md` §8 and `docs/AGENT_WORKFLOW.md` authorize the coordinator after checks and review; (3) `GITHUB_AGENT_WORKFLOW.md` and `DESKTOP_MIGRATION_CHECKLIST.md` still named obsolete branch/HEAD/path values as current; (4) `OPENCODE_AUTOMATION_PLAN.md` still required owner approval for the next normal package. All were corrected in place; historical snapshots were explicitly marked historical instead of being rewritten as new facts.

`observed` — no configuration permission was broadened and no new agent/probe was launched. `gh` availability was not re-probed; the earlier `gh auth status` → `CommandNotFoundException` remains the authoritative observed result for this configuration-only package. GitHub Desktop is installed but is not an agent-callable authenticated integration; PR management and remote write authentication remain `unknown` until an actual Git push or an available `gh` executable is exercised.

`documented` — deliberate omissions: no build, CTest or product function test; no product/test/CMake/preset/LICENSE change; no secret, credential or personal-file inspection beyond the already recorded hierarchy check; no history rewrite, destructive Git command, stash cleanup, branch switch, PR creation or merge. The automatic next-package mechanism remains a ledger/envelope process under the coordinator; no scheduler/automation was created.

### XB-AUTONOMY-ACTIVATION-02-C1 — bundled review correction (round 1 of 2)

```text
PACKAGE_ID:        XB-AUTONOMY-ACTIVATION-02-C1
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Resolve both ACTIONABLE reviewer findings from the XB-AUTONOMY-ACTIVATION-02 review as one bundled documentation-accuracy correction.
FILES:             docs/ROADMAP.md; docs/GITHUB_AGENT_WORKFLOW.md; docs/DEVELOPMENT_ENVIRONMENT.md; docs/AGENT_TASKS.md (this record).
ALLOWED:           Coordinator-only reword of one premature current-state completion claim and replacement of three non-resolving line-number anchors with stable section-name references; ledger record.
FORBIDDEN:         Product code, tests, CMake/presets, LICENSE, secrets, new permissions, destructive Git commands, build/CTest, severity downgrade.
ACCEPTANCE:        ROADMAP activation-02 bullet makes no verdict/commit claim and defers status to the ledger; no live `:588` cross-reference remains (explanatory mentions that explicitly mark the old anchor as invalid are intentional); independent reviewer re-check PASS; git diff --check EXIT=0.
TESTS:             Read-only Git re-check and reviewer re-review. No build, CTest or product function tests.
REVIEW:            Mandatory independent reviewer re-check of this bundled correction.
WORKTREE:          Main checkout D:\XBridge 1.0\XBridge-Series-Autonomy on branch main.
OWNER_APPROVALS:   Covered by the XB-AUTONOMY-ACTIVATION-02 owner authorization; no new owner decision.
```

`documented` — finding classifications from reviewer result (`ses_debf2c308ffenMm3IKzQDHcSRb`), verdict `NEEDS_CHANGE`: `REV-ACT02-01` (MEDIUM) and `REV-ACT02-02` (LOW) are `ACTIONABLE`; `OWNER_DECISION` 0, `NON_BLOCKING` 0, `UNKNOWN` 0. No severity was downgraded.

`documented` — root-cause grouping: both findings are documentation-accuracy defects introduced or re-asserted in this package. Root cause A (premature completion claim): `docs/ROADMAP.md:19` asserted a recorded reviewer verdict and commit while ledger `:22` still read `in_progress`. Corrected by rewording the bullet to defer authoritative status to `docs/AGENT_TASKS.md` without duplicating a verdict/commit claim. Root cause B (stale line anchors): the five-file publication-group-0 definition was cited as `docs/AGENT_TASKS.md:588` although later ledger entries moved it to the section `XB-0086-C2 — final status alignment and publication group 0 definition` (header `:688`, definition `:694`). Corrected by replacing the numeric anchor with that stable section name in `docs/GITHUB_AGENT_WORKFLOW.md:19`, `docs/DEVELOPMENT_ENVIRONMENT.md:185` and `:189`, and by marking the old `:588` value explicitly invalid.

`observed` — before this correction the reviewer confirmed as coherent: public-repository claims, corrected coordinator-publication wording, current path/branch statements with old values marked historical, the A4/A5 grade model and role rights, absence of forbidden changes, complete envelope and evidence labeling. Those areas were left unchanged. A coordinator read-back additionally found the same stale `:588` anchor in the historical XB-0085 audit row for `AGENTS.md` (`:1075`); it was replaced with the same stable section reference under root cause B, without altering the historical verdict.

### XB-AUTONOMY-ACTIVATION-02-C2 — bundled re-check correction (round 2 of 2)

```text
PACKAGE_ID:        XB-AUTONOMY-ACTIVATION-02-C2
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Close the single new ACTIONABLE re-check finding by sweeping the remaining live stale anchors and making the C1 acceptance claim factually consistent.
FILES:             docs/AGENT_TASKS.md (this record, five historical rows and the C1 ACCEPTANCE field).
ALLOWED:           Coordinator-only replacement of five residual bare `:588` cross-references with the stable section name; precise narrowing of the C1 acceptance wording; ledger record.
FORBIDDEN:         Product code, tests, CMake/presets, LICENSE, secrets, new permissions, destructive Git commands, build/CTest, severity downgrade, rewriting of historical verdicts.
ACCEPTANCE:        A broad grep for `:588` shows only intentional explanatory mentions that mark the old anchor invalid; no live cross-reference remains; independent reviewer re-check PASS; git diff --check EXIT=0.
TESTS:             Broad read-only grep re-scan plus independent reviewer re-review. No build, CTest or product function tests.
REVIEW:            Mandatory independent reviewer re-check; this is correction round 2 of 2, the last permitted round.
WORKTREE:          Main checkout D:\XBridge 1.0\XBridge-Series-Autonomy on branch main.
OWNER_APPROVALS:   Covered by the XB-AUTONOMY-ACTIVATION-02 owner authorization; no new owner decision.
```

`documented` — finding classification from the C1 re-check (`ses_debf2c308ffenMm3IKzQDHcSRb`): `REV-ACT02-C1-01` (LOW) is `ACTIONABLE`; prior `REV-ACT02-01` and `REV-ACT02-02` are verified CLOSED with file:line evidence and were not reclassified. `OWNER_DECISION` 0, `NON_BLOCKING` 0, `UNKNOWN` 0; no severity downgrade.

`documented` — root cause: the root-cause-B sweep in C1 was incomplete. The coordinator verification grep matched only the full string `AGENT_TASKS.md:588` and could not detect bare internal `:588` pointers. Corrected by (1) replacing the five live internal anchors at `:1084`, `:1525`, `:1715`, `:1717` and `:1752` with the same stable section name used in C1, preserving every surrounding anchor (`:950`, `:959(c)`, `:594`, `:935`, `:957`) and all historical verdicts; (2) narrowing the C1 ACCEPTANCE field at `:255` so the claim matches what was actually verified. Explanatory mentions that explicitly declare `:588` invalid are retained deliberately and are not cross-references.

`observed` — verification method corrected: a broad grep for the literal `:588` across all Markdown files is used instead of the previous full-string-only grep, so both bare internal anchors and cross-document anchors are detected. This is a documentation-only correction; no product code, test, CMake, preset, LICENSE, permission or configuration file was changed.

### XB-AUTONOMY-ACTIVATION-02 — final reviewer verdict and completion record

`observed` — final independent reviewer verdict (`ses_debf2c308ffenMm3IKzQDHcSRb`): `PASS`, Case A. All three findings are closed against their success criteria: `REV-ACT02-01` (MEDIUM, `ACTIONABLE`) and `REV-ACT02-02` (LOW, `ACTIONABLE`) closed in C1; `REV-ACT02-C1-01` (LOW, `ACTIONABLE`) closed in C2. `OWNER_DECISION` 0, `NON_BLOCKING` 0, `UNKNOWN` 0; no severity downgrade. Two bundled correction rounds used, which is the permitted maximum. The reviewer verified the broad bare-`:588` scan (10 intentional explanatory mentions, 0 live pointers), all five previously live anchors resolved with adjacent anchors and historical verdicts intact, stable section target at `:734`/`:740`, C1 acceptance wording now factually consistent, complete C2 envelope, and no forbidden change or regression. Reviewer command results were attributed to the coordinator and not independently re-executed by the reviewer.

`observed` — completion: package status `pass` in the ledger row above. The authorized scoped commit of exactly these 13 files on branch `main` was executed as `71faef915996ed4e459cf318111a80daa4719013` (`13 files changed, 229 insertions(+), 64 deletions(-)`). The subsequent authorized push succeeded: `git push` → `To https://github.com/comedius/XBridge-Series.git` / `8339582..71faef9  main -> main`, `EXIT=0`. No merge, tag, PR or history operation was performed. Build, CTest and product function tests were explicitly excluded by owner instruction. GitHub CLI availability (`gh auth status` → `CommandNotFoundException`, `EXIT=1`) and PR state remain `unknown`; remote write authentication is `observed` via the successful Git push, not via `gh`. This bookkeeping record was added after the reviewer `PASS` and intentionally contains no policy or code change; it records only the executed commands and their results.

## Package envelopes

```text
PACKAGE_ID:        SETUP-004
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  development-only
GOAL:              Transfer all 13 OpenCode sessions to E: so development history is usable independently of the old machine.
FILES:             OpenCode session stores on E:; docs/AGENT_TASKS.md; docs/AGENT_WORKFLOW.md; docs/RELEASE_CHECKLIST.md
ALLOWED:           Supported session export/import commands; 3 project sessions to the main checkout; 10 setup sessions to a separate permanent portable-setup context; documentation of the result.
FORBIDDEN:         Modifying or deleting originals on C:; copying an active SQLite database directly; product-file changes; agent config changes.
ACCEPTANCE:        13 sessions importable on E: with correct 3/10 split; C: originals unchanged.
TESTS:             Owner-run export/import commands with recorded output; git status --porcelain=v1 --untracked-files=all; git diff --check.
REVIEW:            Coordinator records owner-reported evidence; no architect/tester/reviewer needed for development-only session transfer.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   Already granted. Owner executes the commands because agent roles deny shell opencode *.

PACKAGE_ID:        SETUP-005
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Align windows-debug with the AGENTS.md VS2026 Debug requirement after confirmed compatibility.
FILES:             CMakePresets.json (line 7 only); docs/AGENT_TASKS.md; docs/AGENT_WORKFLOW.md; docs/PORTABLE_SETUP.md (owner); docs/RELEASE_CHECKLIST.md
ALLOWED:           Change generator "Visual Studio 17 2022" to "Visual Studio 18 2026" only in windows-debug; configure, build and CTest; restore the original line if any step fails.
FORBIDDEN:         Any change to ninja-debug; product source changes; unapproved dependency or config changes; committing the change.
ACCEPTANCE:        windows-debug uses VS2026 and configure/build/CTest succeed, or the original preset is restored and the failure documented with exact output.
TESTS:             cmake --preset windows-debug; cmake --build --preset windows-debug --config Debug; ctest --test-dir build/windows-debug -C Debug --output-on-failure; git diff --check.
REVIEW:            Tester executes the three verification commands; coordinator records exact results.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   VS2026 compatibility must be confirmed; owner performs the edit because no agent has write access to CMakePresets.json.

PACKAGE_ID:        SETUP-006
PACKAGE_TYPE:      RESEARCH
PRODUCT_BOUNDARY:  development-only
GOAL:              Verify that worktree creation/opening/removal works from the portable checkout on E:.
FILES:             docs/AGENT_TASKS.md; docs/AGENT_WORKFLOW.md; docs/RELEASE_CHECKLIST.md
ALLOWED:           tools.opencode.worktree_create/list/remove for a disposable test worktree under ../worktrees; documentation of the exact result.
FORBIDDEN:         Git shell worktree commands; speculative config changes; touching the original C: setup worktree; automatic removal after a failed open.
ACCEPTANCE:        Test worktree created, opened and removed with all three results recorded. Not achieved: opening failed.
TESTS:             worktree_create -> "Created worktree could not be opened"; worktree_list before/after; directory read of ../worktrees/portability-worktree-test; git status --porcelain=v1 --untracked-files=all; git diff --check.
REVIEW:            Coordinator documents the blocked result; owner decides about cleanup and further attempts.
WORKTREE:          <PROJECT_ROOT>/../worktrees on E:.
OWNER_APPROVALS:   Decision required for the leftover unregistered checkout and for any further worktree attempt.

PACKAGE_ID:        SETUP-007
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  development-only
GOAL:              Extend the coordinator-controlled workflow with nine scoped specialist agents so routine development is delegated instead of routed through the owner.
FILES:             .opencode/agents/{cpu-developer,memory-developer,gpu-developer,audio-developer,input-developer,platform-developer,test-engineer,performance-engineer,integration-engineer}.md; docs/AGENT_WORKFLOW.md; docs/AGENT_TASKS.md; docs/ROADMAP.md; docs/RELEASE_CHECKLIST.md; docs/PORTABLE_SETUP.md
ALLOWED:           Create only the nine new agent files with disjoint write scopes; update development-only coordination docs; delegate to the nine roles; run independent review/verification with already-permitted agents.
FORBIDDEN:         Edit existing agent files (especially coordinator.md, architect.md, reviewer.md, tester.md); edit opencode.jsonc; change product code/tests/build files; broaden shell, Git or web permissions; commit, push, merge, tag or release.
ACCEPTANCE:        Nine agent files exist with valid OpenCode 2.3.2 frontmatter; coordinator can delegate to all nine; docs are factually current; reviewer verdict recorded. Currently **blocked**: write attempts to .opencode/agents/*.md returned `Permission denied: edit` because the owner-approved grant was not persisted in coordinator.md.
TESTS:             Read back each new agent file; test one subagent delegation; reviewer review with file:line findings; git status --porcelain=v1 --untracked-files=all; git diff --check.
REVIEW:            Architect analysis completed (read-only); independent reviewer check required after installation.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   Owner approved the content and the nine-role scope, but forbade editing existing agent files. A persisted minimal rule change in coordinator.md is required for the coordinator to create the files, edit PORTABLE_SETUP.md and call the new agents.

PACKAGE_ID:        RESEARCH-007
PACKAGE_TYPE:      RESEARCH
PRODUCT_BOUNDARY:  development-only
GOAL:              Analyse lawful, public concepts from omonkeyz/xbox-emulator and drycheetah84/TomBox to improve XBridge technically and organizationally without copying code or changing architecture.
FILES:             docs/research/RESEARCH-007_OVERVIEW.md; docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md; docs/research/RESEARCH-007_WORKFLOW_ANALYSIS.md; docs/research/RESEARCH-007_LICENSE_AND_SOURCES.md; docs/research/RESEARCH-007_RECOMMENDATIONS.md; docs/RESEARCH_REGISTER.md; docs/AGENT_TASKS.md; docs/ROADMAP.md
ALLOWED:           Public-source web research by researcher; read-only XBridge inspection by architect; writing only docs/research/RESEARCH-007_*.md and the RESEARCH_REGISTER entry; documentation of derived packages; read-only reviewer check.
FORBIDDEN:         Product code, product tests, CMake or preset changes; architecture or ADR changes; direct code copying; firmware, keys, game files, proprietary binaries or extracted assets; DRM circumvention; commits, pushes, merges, destructive actions; overwriting other uncommitted changes.
ACCEPTANCE:        Five dated research files with sources, exact URLs, examined commit hashes and evidence labels; per-finding 1-5 classification; honest XBridge comparison from real code; license contradictions documented; independent reviewer verdict recorded; derived independent packages created in the ledger; no product file modified.
TESTS:             git status --porcelain=v1 --untracked-files=all; git diff --check; read-back of all five research files; reviewer review with file:line findings. No build/CTest required (documentation-only package).
REVIEW:            researcher (web research + sources), architect (read-only XBridge comparison), reviewer (independent read-only verdict). The nine specialist agents are not installed and were not used; their analysis areas are covered by architect/researcher and this limitation is documented in the overview.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   None required for research or documentation. Any architecture change, public interface change or code adoption from the findings requires a separate owner decision and ADR.
```

## RESEARCH-007 derived packages (historical planning snapshot — see supersession note)

These packages are created only after research completion, per the RESEARCH-007 brief. Specialist names below are *conditional*: only the six installed agents exist, so the coordinator must assign installed owners until SETUP-007 is resolved.

`documented` — supersession: this text was written when all four packages were `planned`. XB-0081 has since been approved by the owner and executed (see "XB-0081 — owner approval and execution evidence"); the derived XB-0081 envelope below is the historical origin, superseded by the approved decision template and execution section. XB-0080, XB-0082 and XB-0083 remain `planned`; none executes a class 3/4 recommendation, and none may start without a separate owner approval.

```
PACKAGE_ID:        XB-0080
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  development-only
GOAL:              Adopt the "no fake success" evidence-status method as a development-only Xbox capability register, without product changes.
FILES:             docs/RESEARCH_REGISTER.md; docs/AGENT_WORKFLOW.md; docs/AGENT_TASKS.md
ALLOWED:           Extend the register with status states (not investigated / investigated-unsupported / implemented-unvalidated / validated / unknown) and require source + reproduction path per entry.
FORBIDDEN:         Product code/tests/CMake; promoting project-authored external claims to authoritative Xbox facts; marking a stub as implemented.
ACCEPTANCE:        Register schema documented; existing RESEARCH-007 entries conform; reviewer confirms no unsupported behavior is marked validated.
TESTS:             Read-back of updated docs; git diff --check; reviewer review. No build/CTest (docs-only).
REVIEW:            reviewer; coordinator records verdict.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   None (development-only documentation). Derived from REC-01/REC-05/REC-07 (class 1).

PACKAGE_ID:        XB-0081
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Resolve the reviewer-identified test-ownership and stale-documentation contradictions from SETUP-007 without installing specialist agents.
FILES:             docs/AGENT_WORKFLOW.md; docs/ROADMAP.md; docs/AGENT_TASKS.md; docs/RELEASE_CHECKLIST.md
ALLOWED:           Align old Roles/Standard-flow text with the installed-role reality; correct the "read-only CMake/CTest" wording; mark historical ledger snapshots explicitly as superseded.
FORBIDDEN:         Editing installed agent files, opencode.jsonc, docs/PORTABLE_SETUP.md (permission still blocked), product code or tests; silently closing BLOCKED items.
ACCEPTANCE:        No contradictory test-ownership statement remains; reviewer verdict recorded; no claim marked pass without evidence.
TESTS:             Read-back; git diff --check; reviewer review. No build/CTest (docs-only).
REVIEW:            reviewer; coordinator records verdict.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   None for documentation; installing the nine agents remains a separate owner decision (SETUP-007). Derived from SETUP-007 reviewer findings + REC-06.

PACKAGE_ID:        XB-0082
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  development-only
GOAL:              Evaluate whether a public-evidence-only machine-readable function-coverage/progress system is justified for XBridge.
FILES:             docs/research/RESEARCH-007_WORKFLOW_ANALYSIS.md (evaluation appendix); docs/AGENT_TASKS.md
ALLOWED:           Document a design proposal with generator self-checks, public-only data and no proprietary inputs; coordinator decides feasibility.
FORBIDDEN:         Writing any generator/tool code or CI change; consuming dumps/console DLLs/game data; new dependency; falsely claiming capability from coverage counts.
ACCEPTANCE:        Written decision (approve/defer/reject) with rationale; if approved, a separate follow-up package with owner sign-off is created.
TESTS:             Read-back; git diff --check; reviewer review. No build/CTest (docs-only evaluation).
REVIEW:            reviewer; owner decision if a tool/dependency is later proposed.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   Required before any tool/dependency/CI change. Derived from REC-03/REC-04 (class 1/2).

PACKAGE_ID:        XB-0083
PACKAGE_TYPE:      RESEARCH
PRODUCT_BOUNDARY:  development-only
GOAL:              Re-verify project-authored external CPU/graphics claims (Zen 2, AVX2, Windows x64 ABI, D3D12.x, shader/mesh formats) against authoritative public Microsoft/AMD sources.
FILES:             docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md (new); docs/RESEARCH_REGISTER.md
ALLOWED:           researcher web research with exact URLs, access date, labels; keep unverified items unknown.
FORBIDDEN:         Other-console claims as Xbox evidence; NDA/private material; proprietary assets; code copying; product changes.
ACCEPTANCE:        Each claim labeled with authoritative URL or explicitly unknown; no Xbox compatibility claim from translation counts.
TESTS:             Read-back; git diff --check; reviewer review.
REVIEW:            reviewer; architect for boundary check if a claim would change architecture.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   None for research. Derived from REC-17/REC-18 (class 5).
```

Not created intentionally: any package implementing native execution/JIT, GuestMemory Xbox semantics, D3D12.x/RDNA2/texture mirroring, dump-derived DLLs, network guard, or code adoption. Those are class 3/4/5 and require a separate owner ADR/legal decision; they are not development tasks to be auto-selected by agents.

## PLAN-001 — agent-system audit (coordinator, read-only; no rule changed)

`observed` — physical inventory via `glob(".opencode/agents/*.md")` → exactly six files: `architect.md`, `coordinator.md`, `developer.md`, `researcher.md`, `reviewer.md`, `tester.md`. None of the nine planned specialist files exists.

`observed` — actual invocability probes (one `subagent` call per role, harmless prompt; no session created, no edit, no question):

| Role | Exact result |
|---|---|
| `cpu-developer` | `{"error":{"type":"tool.execution","message":"Unknown agent: cpu-developer"}}` |
| `memory-developer` | `{"error":{"type":"tool.execution","message":"Unknown agent: memory-developer"}}` |
| `gpu-developer` | `{"error":{"type":"tool.execution","message":"Unknown agent: gpu-developer"}}` |
| `audio-developer` | `{"error":{"type":"tool.execution","message":"Unknown agent: audio-developer"}}` |
| `input-developer` | `{"error":{"type":"tool.execution","message":"Unknown agent: input-developer"}}` |
| `platform-developer` | `{"error":{"type":"tool.execution","message":"Unknown agent: platform-developer"}}` |
| `test-engineer` | `{"error":{"type":"tool.execution","message":"Unknown agent: test-engineer"}}` |
| `performance-engineer` | `{"error":{"type":"tool.execution","message":"Unknown agent: performance-engineer"}}` |
| `integration-engineer` | `{"error":{"type":"tool.execution","message":"Unknown agent: integration-engineer"}}` |

`observed` — control case: installed roles remain callable; this planning round successfully started `architect` (session ses_dfbd45f7dffeLW5CgkRO3kJ6eG) and `tester` (session ses_dfbd45f68ffe0tnHkkLCABn9JP). Indirect API verification `opencode api get /api/agent` was attempted and rejected: `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}`; no rule was changed to work around it.

`observed` — installed roles and effective rights: `coordinator.md:1-58` = primary; edits allowed only for `docs/AGENT_TASKS.md`, `docs/AGENT_WORKFLOW.md`, `docs/ROADMAP.md`, `docs/RELEASE_CHECKLIST.md`; subagent allow exactly architect/developer/tester/reviewer/researcher; `question:*=allow`; shell `cmake *`, `ctest *`, `opencode *=deny`. `architect.md:5-22` and `reviewer.md:5-28` = read-only (`edit:*=deny`). `developer.md:5-61` may edit `src/**`, `include/**`, `tests/**`, `CMakeLists.txt`; protected docs/config denied. `tester.md:5-25` may edit only `tests/**`. `researcher.md:5-34` may edit only `docs/RESEARCH_REGISTER.md`, `docs/research/**`; web tools allowed at `researcher.md:14-19`. Every installed subagent denies subagents and questions. Global `opencode.jsonc:49` denies `question`, `:50` denies `subagent`, and `:51-52` deny `webfetch`/`websearch`; `opencode.jsonc:63-95` holds hard publication/destructive policies. Per `docs/AGENT_WORKFLOW.md:84` "last matching rule wins", the coordinator's five subagent allows are effective, and by the same rule the researcher's web allows would be effective; neither layering is a defect.

`observed` — configuration conflicts identified during PLAN-001, preserved as a historical snapshot. `documented` — supersession: items 1, 2 and 5 were addressed by XB-0081 (installed-role framing, corrected CMake/CTest wording and superseded ledger snapshots); items 3 and 4 remain unresolved and are listed in the XB-0081 execution section. All `AGENT_WORKFLOW.md` line numbers below are their PLAN-001-era values; current locations are cited in the XB-0081 execution section.
1. `docs/AGENT_WORKFLOW.md:177,181` (PLAN-001-era; current `:181,185`) declared `tests/**` exclusively owned by the uninstalled, future-planned `test-engineer`, while installed `developer.md:15` and `tester.md:9` both allow `tests/**` edits; no installed role implements the claimed exclusive ownership. XB-0081 reworded this as future-planned only.
2. `docs/AGENT_WORKFLOW.md:84` (PLAN-001-era; current `:86`) called CMake/CTest commands "read-only" although they write build outputs; XB-0081 corrected this wording.
3. `docs/PORTABLE_SETUP.md:59,97` still claims VS2022/open preset decision although the preset is verified VS2026; coordinator edit is denied by `coordinator.md:31` — unresolved.
4. `docs/AGENT_WORKFLOW.md:189-194` and `docs/ROADMAP.md:27` assign work to uninstalled specialists; `docs/AGENT_WORKFLOW.md:200` requires an installed and permitted owner, so those assignments cannot start — unresolved.
5. `docs/AGENT_TASKS.md:49` records the developer/tester `tests/**` overlap as fact, while `:106` claimed `tests/**` was exclusive to the future-planned `test-engineer` — a historical internal ledger contradiction, superseded by the XB-0081 execution section.

No existing protection rule, deny rule, agent file, `opencode.jsonc`, product file, test, CMake or preset was changed. `git status --porcelain=v1 --untracked-files=all` → 26 entries (unchanged); `git diff --check` → `EXIT=0` with the known CMakePresets.json CRLF warning.

## PLAN-001 evidence — architect (session ses_dfbd45f7dffeLW5CgkRO3kJ6eG, read-only, returned)

`documented` — historical snapshot: bare `AGENT_TASKS.md:NNN` references in the three PLAN-001 evidence sections are their PLAN-001-era values and may have shifted after XB-0081 insertions. Authoritative current locations are the field/section anchors in the XB-0081 execution section; `AGENT_WORKFLOW.md` references there were updated to current values by XB-0081.

`observed` — no edits, no build/CTest, no web. All four packages are documentation-only and touch no product file. Interpreter-first/C++20/guest-host separation/public interfaces/error contracts are unaffected because none of the four write sets includes `include/**`, `src/**`, `tests/**`, `CMakeLists.txt`, presets or CI.

`observed` — executability under the installed permission model:
- XB-0080: **not executable as written**; `docs/RESEARCH_REGISTER.md` is researcher-only (`researcher.md:5-13`) while `docs/AGENT_WORKFLOW.md` and `docs/AGENT_TASKS.md` are coordinator-only (`coordinator.md:5-19`). Minimal gate: approve a write-set split (researcher = register, coordinator = workflow/ledger) or grant one installed agent the missing scoped path.
- XB-0081: **executable today by coordinator alone**; all four FILES are coordinator-editable (`coordinator.md:8-19`). No owner decision needed for the documentation itself.
- XB-0082: **not executable as written**; research appendix is researcher-only, ledger is coordinator-only. Same split gate; later tool/dependency/CI change always needs owner sign-off.
- XB-0083: executable by `researcher` (`researcher.md:5-13`); the global web denies at `opencode.jsonc:51-52` are followed by the researcher's web allows (`researcher.md:14-19`) under the "last matching rule wins" rule (`docs/AGENT_WORKFLOW.md:84`), and RESEARCH-007 did complete web research earlier. Fresh web access in this planning round remains `unknown` because no new web call was attempted and the reviewer works offline; that is an evidence limitation, not a configuration verdict.

`observed` — write-set conflicts: XB-0080, XB-0081, XB-0082 all touch `docs/AGENT_TASKS.md` (and 0080/0081 also `docs/AGENT_WORKFLOW.md`), so they are not parallel-safe under the one-path-one-package rule. XB-0083's new research file is disjoint; only coordinator ledger recording overlaps.

`observed` — architect ranking: 1. XB-0081 (only package executable by a single installed owner, resolves concrete reviewer findings on test ownership and CMake/CTest wording); 2. XB-0080 (lowest content risk but needs split); 3. XB-0083 (useful evidence cleanup, web permission `unknown`); 4. XB-0082 (evaluation only, needs split, largest future tool/dependency/false-metric risk). Architect notes XB-0080 has the clearest zero product-code risk, while XB-0081 is the best *first executable* package. `docs/ROADMAP.md:27` next product milestone is not executable today because it names uninstalled `cpu-developer` + `test-engineer`.

## PLAN-001 evidence — tester (session ses_dfbd45f68ffe0tnHkkLCABn9JP, read-only, returned)

`observed` — no edits, no build, no CTest, no `git diff --check` by the tester. It ran only `git branch --show-current` → `main` and `git status --porcelain=v1 --untracked-files=all` → 26 entries, matching the coordinator's inventory. All quoted build/CTest results are ledger records, not executions in this task.

`observed` — per-package verification (references by derived-envelope field, not shifted line numbers): XB-0080 `TESTS` field = read-back + `git diff --check` + reviewer, explicitly **no build/CTest**; XB-0081 `TESTS` field identical; XB-0082 `TESTS` field identical; XB-0083 `TESTS` field omits the explicit "no build/CTest" sentence although it is research-only — an envelope ambiguity to clarify before execution, not a license to invent a build result.

`observed` — VS2026 evidence quoted from `AGENT_TASKS.md:89-96`: plain `cmake`/`ctest` failed with `CommandNotFoundException` (exit 1); absolute VS2026 executables under `E:\PRO\Programm Daten\Visual Studio\18\Community\...\CMake\bin\` then produced configure exit 0, Debug build exit 0 (`xbridge_core.lib`, `xbridge.exe`, `xbridge_tests.exe`), CTest exit 0 (`1/1 Test`, `100% tests passed, 0 tests failed out of 1`). Locally **not rerun** in this planning task.

`observed` — risks recorded: plain `cmake`/`ctest` PATH resolution `unknown`; allowlist patterns at `opencode.jsonc:32-39` do not textually match the absolute-path form beginning `& 'E:\PRO\...\cmake.exe'`, so agent reproducibility of that form is `unknown`; CRLF warning on `CMakePresets.json` is pre-existing noise and not a package failure; only one CTest target exists (`CMakeLists.txt:32-37`, one global failure counter in `tests/test_main.cpp`); external web truth for XB-0083 is unverifiable offline; write-set overlaps are `docs/AGENT_TASKS.md` shared by 0080/0081/0082, `docs/AGENT_WORKFLOW.md` by 0080/0081, `docs/RESEARCH_REGISTER.md` by 0080/0083.

`observed` — XB-0081 scope gap confirmed: its derived-envelope FORBIDDEN field excludes installed agent files, `opencode.jsonc` and `docs/PORTABLE_SETUP.md`, but the stale VS2022 claims live in `PORTABLE_SETUP.md:59,97` and part of the ownership contradiction lives in `developer.md:14-16`/`tester.md:8-10`; therefore XB-0081 can align prose only and must record these as unresolved, not claim full resolution.

`observed` — tester conclusion: XB-0081 is independently testable first (docs-only, no web, no new agents, no owner gate, objective offline acceptance), with XB-0082 as a weaker second (judgment rather than checkable contradiction). Both installed `developer` (`developer.md:14-16`) and `tester` (`tester.md:8-10`) may edit `tests/**`, while `docs/AGENT_WORKFLOW.md:181,185` (current; PLAN-001-era `:177,181`) frames exclusivity for a future-planned, uninstalled role; no rule was changed.

## PLAN-001 — prioritization and owner decision template (coordinator)

`observed` — ranking, jointly supported by architect and tester: **1. XB-0081**, **2. XB-0080**, **3. XB-0083**, **4. XB-0082**.

- **XB-0081 first:** `[observed]` only package executable by one installed owner with all four write paths; `[documented]` no owner approval for documentation (derived-envelope OWNER_APPROVALS field); `[observed]` offline-checkable against the recorded HIGH finding (SETUP-007 reviewer verdict section); `[observed]` no build/CTest, web, agent installation or product-code risk.
- **XB-0080 second:** `[documented]` lowest content risk and clearest zero product-code exposure (XB-0080 derived envelope); `[observed]` blocked as written by split write set (researcher = register, coordinator = workflow/ledger); `[hypothesis]` schema must allow `unknown`-with-reason so unknown external claims are not forced into false statuses.
- **XB-0083 third:** `[documented]` resolves class-5 evidence gaps (XB-0083 derived envelope); `[observed]` installed `researcher` can write both files, but fresh web permission is `unknown` and external truth cannot be verified offline; does not unblock workflow ownership.
- **XB-0082 fourth:** `[documented]` evaluation only with owner gate before tools/CI (XB-0082 derived envelope); `[observed]` blocked as written by split write set; `[hypothesis]` greatest future risk of false metrics/dependencies, so it should follow the workflow/evidence groundwork.

`hypothesis` — the ordering follows availability and verifiability, not product value; all four are development-only, so interpreter-first, C++20, guest/host separation, existing public interfaces and safe error handling are preserved by *not touching them*. No package here implements a runtime feature.

### Owner decision template — first package XB-0081 (SUPERSEDED pre-approval draft)

`documented` — this template is the historical pre-approval decision request. The owner approved XB-0081; the approval, narrowed scope and execution evidence are recorded in "XB-0081 — owner approval and execution evidence". The `STATUS`, `OPEN_OWNER_DECISIONS`, `OWNER_APPROVALS` and trailing `unknown` statements inside the block below are **superseded** by that approval and are not currently open or unapproved.

```text
PACKAGE_ID:        XB-0081
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
STATUS:            planned — awaiting owner approval (do not start before approval)
GOAL:              Align development documentation with the six installed agents and recorded evidence, without installing specialists or changing any protection rule.
FILES:             docs/AGENT_WORKFLOW.md; docs/ROADMAP.md; docs/AGENT_TASKS.md; docs/RELEASE_CHECKLIST.md
COMPONENTS:        Development-infrastructure documentation only. No include/**, src/**, tests/**, CMakeLists.txt, CMakePresets.json, CI, .opencode/** or opencode.jsonc.
ALLOWED:           Correct old Roles/Standard-flow prose to match installed roles; replace the "read-only CMake/CTest" wording with accurate build-output wording; mark historical ledger snapshots as superseded; add an explicit unresolved-items list.
FORBIDDEN:         Product code/tests/CMake/presets; installed agent files; opencode.jsonc; docs/PORTABLE_SETUP.md; changing deny/protection rules; weakening or deleting evidence; marking SETUP-007/SETUP-006 as pass or closed; claiming a build/CTest result.
OWNERSHIP:         coordinator = sole writer (edit rights at coordinator.md:8-19); reviewer = independent read-only verdict with file:line findings; architect/tester not needed for docs-only work; no specialist agent used.
ACCEPTANCE:        (1) no contradictory test-ownership statement remains in the four FILES; (2) CMake/CTest wording no longer claims "read-only"; (3) superseded ledger snapshots labelled; (4) unresolved items explicitly recorded (agent-file overlap, opencode.jsonc global rules, stale PORTABLE_SETUP.md, SETUP-007/006 status); (5) reviewer verdict recorded by coordinator; (6) pre-existing 26-entry working-tree state preserved; (7) git diff --check exit 0 with only the known CMakePresets.json CRLF warning.
TESTS:             Read-back of all four FILES; git status --porcelain=v1 --untracked-files=all; git diff --check; reviewer review. No build/CTest: documentation-only, declared in the envelope.
REVIEW:            reviewer (independent read-only verdict with file:line findings); coordinator records the verdict; architect and tester not required for documentation-only work.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   Required before start: owner approves the narrowed acceptance (a) and confirms decisions (b) and (c). No approval is granted by this template.
RISKS:             Scope gap — part of the contradiction lives in installed agent files and PORTABLE_SETUP.md, which this package may not edit; must be recorded as unresolved, not silently "fixed". CRLF warning is pre-existing noise. Uncommitted docs must be preserved, not overwritten.
OPEN_OWNER_DECISIONS: (a) approve the narrowed acceptance limited to the four docs, OR explicitly authorize a separate package with agent-file edits (no deny-rule change); (b) keep the "test-engineer exclusive" text as future-planned only, or rewrite it now; (c) confirm the Roadmap:27 product milestone remains blocked and is not auto-started.
```

`documented` — the envelope fields above follow `docs/AGENT_WORKFLOW.md` (`PACKAGE_ID`, `PACKAGE_TYPE`, `PRODUCT_BOUNDARY`, `GOAL`, `FILES`, `ALLOWED`, `FORBIDDEN`, `ACCEPTANCE`, `TESTS`, `REVIEW`, `WORKTREE`, `OWNER_APPROVALS`); `STATUS`, `COMPONENTS`, `OWNERSHIP`, `RISKS` and `OPEN_OWNER_DECISIONS` are additional decision-support fields, not substitutes. On owner approval this template supersedes and narrows the derived XB-0081 envelope (PACKAGE_ID `XB-0081` block under "RESEARCH-007 derived packages"); that entry remains the historical origin. No approval is granted by this template; it is a decision request. `unknown` — whether the owner accepts the narrowed scope or wants agent files included in a later package.

`observed` — independent PLAN-001 review (session ses_dfbc16f28ffe9PrxbVt3RiDabQ): round 1 `NEEDS_CHANGE` with one MEDIUM (three missing envelope fields) and two LOW findings (web-permission line reference/rationale, duplicate XB-0081 envelope framing). All three were corrected by the coordinator in `docs/AGENT_TASKS.md`; final review returned **PASS** with no remaining findings. Reviewer confirmed 26 git entries, `git diff --check` exit 0 (known CRLF warning), and no product/test/CMake/agent-file/rule change. PLAN-001 planning documentation is complete. `documented` — supersession: the earlier "execution remains blocked on owner approval, and no package was started" was accurate when that review closed; the owner has since approved XB-0081 and it was executed (see "XB-0081 — owner approval and execution evidence"). No other package was started.

## RESEARCH-007 evidence — architect (session ses_dfc0f9e12ffdZLo8jWoLhqefBW, read-only, returned)

`observed` — no files changed, no build/CTest run, no web fetch. Key inventory results with file:line references:

- **CLI/core:** `src/app/main.cpp:6-26` accepts only `--profile series_x` / `--help`; `src/core/runtime.cpp:15-38` runs a 4 KiB `TestCpu` synthetic demo and prints `Status: scaffold demo completed (not Xbox emulation)`; returns 0 only for synthetic result 42. `src/core/config.cpp:5-11` marks the profile "illustrative and deliberately not a cycle-accurate hardware model".
- **Decoder:** `src/cpu/x64_instruction_decoder.cpp:10-15,85-189` implements only NOP 0x90, MOV imm 0xB8+r (REX.W), MOV r/m↔r 0x88/89/8A/8B; 8-bit MOV → `UnknownOpcode`; SIB/RIP-relative/displaced → `InvalidEncoding`. Stateless, `noexcept`, status-only, no exceptions (`include/xbridge/cpu/x64_instruction_decoder.hpp:17-23,56-77`).
- **Interpreter:** `src/cpu/x64_instruction_interpreter.cpp:26-148` handles NOP, MOV imm, MOV reg↔reg, reg↔mem for 32/64-bit; validates ranges before access (`:8-24,69-123`); converts `std::out_of_range` to `MemoryAccessError` with no partial state/memory mutation (`:31-40,85-98,106-120`). Tests at `tests/test_main.cpp:861-999`.
- **X64CpuState:** `include/xbridge/cpu/x64_cpu_state.hpp:11-105` — 16 GPRs, RIP, RFLAGS, slice access; no semantics/reset/SIMD/FPU/Xbox properties. `TestCpu` is a separate synthetic ISA (`include/xbridge/cpu/test_cpu.hpp:9-37`).
- **GuestMemory:** `include/xbridge/memory/memory.hpp:8-21`, `src/memory/memory.cpp:9-59` — generic contiguous zero-initialized byte vector, 32-bit offsets, checked half-open ranges, LE 32-bit access; no reservation/permissions/aliasing/paging/Xbox VM semantics.
- **Host/policy:** `include/xbridge/host/capabilities.hpp:7-19` describes only the host PC; `src/host/execution_policy.cpp:22-79` is a pure selector (Vulkan→D3D12→software; interpreter baseline; JIT only opt-in when `cpu_jit_available`). No real detector, backend initialization, renderer, UI, telemetry, or provider. `CpuBackend::Jit` is a policy state, not an implementation (`docs/ARCHITECTURE.md:209-213`).
- **Profiles:** only `SeriesX` exists (`include/xbridge/core/config.hpp:7`); `src/profiles/hardware_profile.cpp:1-2` is a reserved placeholder; `PerformanceProfile` is enum/report only.
- **Tests/build:** one hand-written CTest target `xbridge_tests` (`CMakeLists.txt:32-37`, `tests/test_main.cpp:21-44`); no differential corpus, fuzz, coverage automation, graphics/system tests. CI is `.github/workflows/windows.yml:1-17` (windows-latest, Release, `ctest -C Release`).
- **Staleness:** no current VS2026/agent-count staleness; historical first-pass lines at `AGENT_TASKS.md:77,83` are explicitly superseded by `:85-97`.

The architect supplied a reusable 1–5 classification rubric and decision gates: native x64/JIT = class 3 (class 4 if introduced without evidence/design/tests; owner decision + ADR + interface change); unified memory/aliasing in GuestMemory = class 3/4 (forbidden to put Xbox VM assumptions into generic GuestMemory); D3D12.x/RDNA2/shader translation = class 5 research, class 3 implementation, class 4 for equivalence claims; Xbox console DLLs = class 5 until lawful evidence, class 3 implementation, class 4 for proprietary binaries; function-coverage automation = class 1 workflow / class 2 independent tooling, class 4 if proprietary-dependent. All are development-only here; every adoption needs a separate owner decision and ADR.

## RESEARCH-007 evidence — researcher (session ses_dfc0f9e12ffeTE4B5oClEOf3sI, web research, returned)

`observed` — five files created under `docs/research/` and one `RESEARCH_REGISTER.md` row added; no product/test/CMake/preset/agent-file change; no clone, no binary/dump/key/game asset downloaded; no code copied. Coordinator re-read all five files and confirmed content.

Examined revisions (all sources accessed 2026-11-22): primary `omonkeyz/xbox-emulator` HEAD `65ca6dd07b0b8ca774a036cdc319d1bb9754e6e2`; upstream `drycheetah84/TomBox` HEAD `9a39de888bbdecef1cae84171464dad34176b994`; upstream base named by primary README `7e8064fdcd47bca4afef3ec295668ed40b81c047`. Determined via GitHub API `commits/HEAD` / `commits/7e8064f` and recorded with exact URLs.

Principal findings by area (all labeled in the files): (A) both projects are native-x64 compatibility runtimes, explicitly no CPU emulation — a fundamentally different architecture from XBridge's interpreter baseline (class 3 for XBridge); upstream architecture doc lags its README at the same commit. (B) Zen 2/AVX2/16 GB unified memory are project-authored secondary notes; Xbox aliasing/protection/paging remain `unknown`; XBridge GuestMemory unchanged. (C) project-reported D3D12.x 194/200, RDNA2 768/783, shaders 105,506/105,506 but shader stages 0/8; "translating is not rendering correctly"; no rendered-pixel validation. (D) machine-readable `exports.txt` status vocabulary, "no fake success", 2,142/2,155 console exports, 671/671 imported functions; OS-build-dependent export surfaces. (E) append-only logs, debt ledger, generated progress map/badges, per-function test gate, CI/PR artifact gates, explicit parallel-agent conflict notes — most transferable to XBridge as workflow method (class 1).

License result: primary repo GPL-3.0 (`spdx_id: GPL-3.0`, full GPL v3 text, "GPL-3.0-or-later" in README; earlier releases MIT OR Apache-2.0); upstream LICENSE raw fetch HTTP 404 and API license HTTP 404, no license grant in fetched README. Contradictions documented (current-vs-earlier primary license, upstream doc-vs-README status, status vocabulary mismatch). Conclusion recorded: no code/design/artifact adoption before exact-revision owner/legal review; this is a risk finding, not legal advice.

Coordinator verification: `git status --porcelain=v1 --untracked-files=all` → 26 entries (20 prior + 5 new research files + 1 modified RESEARCH_REGISTER.md); `git diff --check` → exit 0 with the known informational CRLF warning for CMakePresets.json. Research-only package: no build/CTest run or claimed. Known minor typo: `docs/research/RESEARCH-007_OVERVIEW.md:61` reads `cpu-developper` (handed to reviewer for confirmation and correction).

## RESEARCH-007 review round 1 + correction C1 (coordinator record)

`observed` — independent `reviewer` (session ses_dfc0a4d50ffe9TFmP7j4Lppqic) returned **NEEDS_CHANGE** with four light documentation findings (F1 rubric inconsistency, F2 class-5 rationale mismatch, F3 CPU-status-contract class tension, F4 confirmed typo); all other checks passed: factual honesty vs real XBridge (no JIT implementation, no paging/aliasing in GuestMemory, interpreter baseline, one CTest target), evidence quality, license contradiction handling without legal advice, scope/boundary cleanliness, envelope completeness, no unsupported game-compatibility claims. F5 (stale roadmap line) was corrected by the coordinator; F6/F7 are INFO/non-blocking (unpinned live URLs where pinned exist; external truth not reverifiable offline and explicitly `unknown`).

`observed` — `researcher` correction package RESEARCH-007-C1 applied exactly the four findings, no other claims/sources/hashes/labels changed, no web re-fetch. Coordinator re-read all six changed lines and confirmed: `ARCHITECTURE_COMPARISON.md:49` AC-01 `5`→`4`; `:56` AC-08 `5`→`3` with explicit "further public evidence is still required (see class-5 items)"; `:74` AC-23 class-1 clarification; `RECOMMENDATIONS.md:41` REC-10 class-2 clarification; `:82` priority row "Do not implement without ADR" → "Do not implement without approval" with REC-10 = approved package only and REC-11/12 = ADR; `OVERVIEW.md:61` only `cpu-developer` remains.

`observed` — post-correction verification by researcher and coordinator: trailing-whitespace grep over `docs/research/RESEARCH-007_*.md` → no matches; `git diff --check; "EXIT=$LASTEXITCODE"` → known CMakePresets.json CRLF warning, `EXIT=0`; `git status --porcelain=v1 --untracked-files=all` → 26 entries (unchanged). Final independent reviewer confirmation of C1 is running; verdict to be recorded on return.

`observed` — final independent reviewer (same session, follow-up) returned **PASS**. All five check locations verified fixed: AC-01 `4`; AC-08 `3` with evidence note, consistent with REC-12 `3`; both class-1/class-2 clarifications present; priority row "Do not implement without approval" with REC-10 = approved package only and REC-11/12 = ADR; `cpu-developer` typo resolved. Scope re-confirmed: 26 git status entries unchanged, `git diff --check` exit 0 with only the known CMakePresets.json CRLF warning, branch `main`, no commits/publication. External web truth explicitly `unknown` (offline final check). Maximum two correction rounds were not exceeded; one round was used. RESEARCH-007 is marked `pass` in the ledger and roadmap.

## XB-0080 — extended preparation (PLAN-002; implementation NOT started)

`documented` — owner instruction for this round: extended preparation only, no implementation beyond already approved documentation work, no product/architecture/ADR/permission change, no publication, and an explicit owner release required before XB-0080 starts. `documented` — XB-0080 remains `planned`; the derived envelope at `:215-226` stays the authoritative origin (supersession note `:212`). This section records preparation only; its review verdict, if `PASS`, applies to the preparation deliverables and **not** to XB-0080's implementation acceptance criteria, which are unassessed because no implementation was performed.

`observed` — actual scope, taken verbatim from the envelope and not expanded: `PACKAGE_TYPE: FEATURE`, `PRODUCT_BOUNDARY: development-only`, `GOAL` = adopt a "no fake success" evidence-status method as a development-only Xbox capability register, `FILES` = `docs/RESEARCH_REGISTER.md`, `docs/AGENT_WORKFLOW.md`, `docs/AGENT_TASKS.md`, `ALLOWED` = five states (`not investigated` / `investigated-unsupported` / `implemented-unvalidated` / `validated` / `unknown`) plus source and reproduction path per entry, `FORBIDDEN` = product code/tests/CMake, promoting external claims to authoritative Xbox facts, marking a stub as implemented. No additional file was added to the write set.

`observed` — four installed agents ran in parallel, all read-only, all confirmed "edited nothing": `researcher` (session `ses_dfba8b58affe545mD87FsL6iq7`) audited citation completeness and register structure; `architect` (`ses_dfba8b584ffe4eaQP7zanSZAci`) audited technical relevance, ADR/interface boundary and dependencies; `developer` (`ses_dfba8b573ffeaUIbKUibFaPL3V`) audited executability and permissions; `tester` (`ses_dfba8b56fffeZNYmndkgXz392p`) designed the verification plan. No uninstalled specialist role was used or reported as available.

### Audit findings integrated

`observed` — genuine traceability gap, not a form-copying exercise: `docs/RESEARCH_REGISTER.md:3` requests source, date, confidence and reproduction steps, but the table at `:5-6` has only `ID | Topic | Status | Next action`; RESEARCH-007 stores sources and reproduction as prose at `:13`. External status vocabulary (DLL export states at `RESEARCH-007_WORKFLOW_ANALYSIS.md:41-44`) must not be copied; only the rule shape (explicit gaps, evidence, reproduction, no fake success) transfers.

`observed` — capability status and evidence label must remain distinct fields: labels (`documented` / `observed` / `hypothesis` / `unknown`, `RESEARCH_REGISTER.md:15-21`) describe evidence quality; the five proposed states describe investigation/implementation/validation lifecycle. Merging them would allow a `documented` quote to be misread as a validated capability.

`observed` — register conformance gaps: R-001…R-006 (`RESEARCH_REGISTER.md:7-12`) lack structured source, date, finding, label and reproduction; `partial` at `:11` and the compound status at `:13` are outside the new vocabulary; `:13` also groups installed-role status (locally `observed`, `AGENT_WORKFLOW.md:49`) with genuinely external `unknown` items. REC-01/REC-05/REC-07 (`RESEARCH-007_RECOMMENDATIONS.md:21,25,27`) have URL/hash/label but no self-contained per-row date, source ID, concrete finding and reproduction; REC-05 has no quoted claim.

`observed` — no architecture, interface, ADR or product impact under this envelope: no `include/**`, `src/**`, `tests/**`, CMake, preset or CI path is in `FILES`; read-only searches found no schema terms in product code; `docs/DECISIONS.md:1-56` contains no evidence-register ADR. A generator, machine-readable progress artifact, CI gate or external tool belongs to XB-0082 (`AGENT_TASKS.md:241-252`) and needs its own owner gate; it is deliberately not added here.

`observed` — envelope defects to resolve before owner approval: (1) `:226` `OWNER_APPROVALS: None` contradicts `:212` (separate approval needed) and ledger row `:20`; (2) no `OWNER`/`OWNERSHIP` field although `AGENT_WORKFLOW.md:156` requires one owner per package; (3) `ALLOWED` (`:220`) describes only register edits while `FILES` (`:219`) also names `AGENT_WORKFLOW.md`; (4) "existing RESEARCH-007 entries conform" (`:222`) is ambiguous between the single register row `:13` and five research files outside `FILES`; (5) no mapping rule for `partial`/compound statuses; (6) status vocabularies differ between `:220`, `RESEARCH_REGISTER.md:7-13` and `:5`; (7) no supersession rule for the derived envelope (XB-0081 precedent: keep origin, add execution section). Authority order (`AGENT_WORKFLOW.md:5-7`) requires the owner to resolve these, not the coordinator.

### Common package plan (proposed; not approved)

`observed` — one-owner-per-path (`AGENT_WORKFLOW.md:156,218`) plus installed rights make the envelope **not executable as written**: `researcher` may write only `docs/RESEARCH_REGISTER.md` (`researcher.md:8-13`), `coordinator` may write `docs/AGENT_WORKFLOW.md` and `docs/AGENT_TASKS.md` (`coordinator.md:8-19`). Recommended split is justified by permissions and shared-file rules only, not by technical content:

| Subpackage | Owner | Write set | Depends on | Reviewer |
|---|---|---|---|---|
| XB-0080-A — register schema and conformance | researcher (installed) | `docs/RESEARCH_REGISTER.md` only | none (after owner release) | reviewer, read-only |
| XB-0080-B — workflow/ledger integration | coordinator (primary) | `docs/AGENT_WORKFLOW.md`; `docs/AGENT_TASKS.md` | A's schema text final | reviewer, read-only |

`hypothesis` — A then B, with one integrated reviewer verdict, avoids two agents defining the schema in two documents. Alternative (owner grants one installed agent the missing path) requires editing `.opencode/**`, which is denied and owner-only; not done here. Splitting schema, source path and review into three packages would be artificial fragmentation because schema, migration and reviewer check form one acceptance unit.

`documented` — responsibility matrix: coordinator owns envelope, distribution, integration, ledger and roadmap; researcher owns register schema, evidence labels and per-entry provenance; architect provides read-only relevance/ADR/dependency analysis; developer provides read-only feasibility/component analysis; tester provides read-only acceptance and verification design; reviewer provides the independent verdict. Product owners (cpu/memory/gpu/audio/input/platform) and all nine specialists remain uninstalled/planned and are not assigned.

`observed` — dependency overview: XB-0081 is complete (`pass`, `:20`) and must not be reopened; `docs/AGENT_TASKS.md` is shared with XB-0082, `docs/AGENT_WORKFLOW.md` with XB-0081, `docs/RESEARCH_REGISTER.md` with XB-0083 — serialize on those paths. XB-0083 should follow A or stay on current labels to avoid rework; XB-0082 should follow A for taxonomy alignment. Roadmap item 27 remains blocked by SETUP-007 permissions.

### Checkable acceptance criteria (rewritten; implementation status: NOT STARTED)

| # | Criterion | Check | Status |
|---|---|---|---|
| AC-1 | Schema documented | Read-back of `RESEARCH_REGISTER.md` shows all five state tokens verbatim, one use-rule each, mandatory Source (URL/date or exact local path) and Reproduction fields | not started |
| AC-2 | RESEARCH-007 entries conform | All seven register rows use only the five tokens with a mapping note for `partial`/compound; RESEARCH-007 row retains date, URLs, hashes and reproduction; 20 REC rows retain ID/URL/label/class | not started |
| AC-3 | No fake success | Reviewer finds no class-4/5 item (D3D12.X, shader/mesh stages, rendered pixels, JIT/native execution, Xbox paging/aliasing, dumps/keys/firmware, publisher services, game compatibility) marked `validated`; every `validated`/`implemented-unvalidated` row has non-empty Source and Reproduction citing executed package-local evidence | not started |
| AC-4 | Boundary preserved | `git diff` over the three `FILES` only; no product, test, CMake, preset, agent, `opencode.jsonc` or permission change; 26 working-tree entries preserved | not started |
| AC-5 | Review recorded | Independent reviewer verdict with file:line findings recorded by coordinator; no self-acceptance | not started |

### Risks and verification plan

`observed` — principal risk is status inflation (`validated` without reproduction, stub as `implemented-unvalidated`), which would recreate the "fake success" the package prevents; secondary risks are loss of RESEARCH-007 URLs/hashes/date during migration, reintroducing XB-0081's corrected ownership wording, and stale bare `file:line` refs after ledger insertions (mitigate with section anchors). External web truth, license/legal effect and fresh XB-0083 web access stay `unknown`.

`documented` — verification commands for the eventual implementation: `git branch --show-current` (expect `main`); `git status --porcelain=v1 --untracked-files=all` (expect 26 entries); `git diff --check; "EXIT=$LASTEXITCODE"` (expect `EXIT=0`, known CMakePresets.json CRLF warning); `git diff -- docs/RESEARCH_REGISTER.md docs/AGENT_WORKFLOW.md docs/AGENT_TASKS.md`; `git log -3` (expect head `10e5e75` unchanged); read-back of the three files; reviewer check with file:line findings. Build/CTest are not applicable to this docs-only package (`AGENTS.md:82-94` applies to implementation with new product behavior). Shell allowlist covers `git diff*` but not grep/Select-String (`opencode.jsonc:18,20-25`), so verification uses read-back plus `git diff`.

### Open owner decisions

`documented` — (1) resolve the `OWNER_APPROVALS: None` vs `:212` conflict and grant a separate start approval for XB-0080; (2) choose the A/B write-set split or grant one installed agent the missing scoped path; (3) decide whether "RESEARCH-007 entries conform" includes the five research files outside `FILES`; (4) approve the checkable AC-1…AC-5 rewrites; (5) choose a single authoritative location for the status vocabulary (recommended: register); (6) confirm the mapping for `partial` and compound statuses; (7) confirm machine-readable progress stays in XB-0082. No decision was silently chosen by the coordinator.

`observed` — preparation-side evidence: `git branch --show-current` → `main`; `git status --porcelain=v1 --untracked-files=all | Measure-Object` → `26`; `git diff --check; "EXIT=$LASTEXITCODE"` → known CMakePresets.json CRLF warning, `EXIT=0`. Only this ledger section was added in the preparation round; `RESEARCH_REGISTER.md`, `AGENT_WORKFLOW.md`, `ROADMAP.md`, `RELEASE_CHECKLIST.md`, product code, tests, build files, agent files, permissions and `opencode.jsonc` were not changed. External truth remains `unknown`; no web fetch, build or CTest was run.

`observed` — independent review of the preparation (session ses_dfba621b9ffeXnC3S4E42eigU5, read-only) returned **PASS** with no blocking findings. Reviewer re-ran `git branch --show-current` → `main`; `git status --porcelain=v1 --untracked-files=all` → 26 entries (5 modified, 21 untracked), `STATUS_EXIT=0`; `git diff --check; "EXIT=$LASTEXITCODE"` → known CMakePresets.json CRLF warning, `EXIT=0`; `git log -1` → `10e5e7515ea9306dfe0fba45d87b01bed801551f`, no post-preparation commit. All 8 check items passed: true 3-file envelope with no invented scope; faithful integration of the four audits; accurate installed-only responsibility matrix; checkable AC-1..AC-5 correctly marked `not started`; complete unresolved owner decisions with Roadmap item 27 still blocked; honest evidence labels with external truth `unknown`; no shifted anchors; git state confirmed. Verdict explicitly covers the **preparation only** — XB-0080 stays `planned` and AC-1..AC-5 remain NOT STARTED / NOT ASSESSED.

`observed` — five non-blocking reviewer notes recorded without change: (N1) "Roadmap item 27" is established ledger shorthand for `ROADMAP.md:27` (also used at `:113`, `:117`, `:133`, `:363`); (N2) the responsibility matrix at `:439` is labelled `documented`, with the specific A/B mapping being a proposed plan derived from installed role definitions; (N3) `AGENT_WORKFLOW.md` and `AGENT_TASKS.md` are untracked, so `git diff` does not show their content — the read-back step is the effective check; (N4) the four audit transcripts live in OpenCode sessions, not the repo, so integration was verified for internal consistency and anchor accuracy rather than by re-reading transcripts; (N5) the "only this ledger section was added" claim rests on mtime ordering plus unchanged 26-entry status, not cryptographic proof. None affects acceptance; the reviewer required no further change.

## XB-0084 — GTA VI performance-first assessment + Xbox 360 backward-compatibility planning (envelope; implementation not started)

`documented` — owner order in this session authorizes a documentation/assessment package only: comprehensive technical inventory, prioritized plan, six named document categories, updates to `docs/AGENT_TASKS.md` and `docs/ROADMAP.md`, independent reviewer, and an explicit stop before any implementation. The order modifies the earlier "no further packages" instruction for this package only.

```text
PACKAGE_ID:        XB-0084
PACKAGE_TYPE:      RESEARCH
PRODUCT_BOUNDARY:  development-only
STATUS:            in_progress — assessment/planning only; no implementation approved
GOAL:              Produce an evidence-labeled technical inventory and prioritized plan for GTA VI (Series X, performance-first) and Splinter Cell: Double Agent (Xbox 360) as a separate backward-compatibility target, without any product change.
FILES:             docs/research/XB-0084_GTA_VI_COMPATIBILITY_INVENTORY.md
                   docs/research/XB-0084_GTA_VI_PERFORMANCE_PROFILE.md
                   docs/research/XB-0084_XBOX360_SPLINTER_CELL_ANALYSIS.md
                   docs/research/XB-0084_RESEARCH_LICENSE_ASSESSMENT.md
                   docs/research/XB-0084_PRIORITIZED_DEVELOPMENT_PLAN.md
                   docs/research/XB-0084_RISK_DEPENDENCY_OVERVIEW.md
                   docs/AGENT_TASKS.md
                   docs/ROADMAP.md
ALLOWED:           Create the six assessment/planning documents under docs/research/**; record evidence, verdict and open decisions in the ledger; add a roadmap entry. All claims carry documented/observed/hypothesis/unknown labels. Performance section defines measurement methodology only; no invented FPS values.
FORBIDDEN:         Product code, product tests, CMake/presets, public interfaces, new dependencies, architecture or ADR changes, JIT implementation, code copying, proprietary firmware/keys/game data, DRM circumvention, agent files, opencode.jsonc, permission changes, commits/pushes/merges/tags/releases, closing blocked SETUP tasks, starting any follow-up implementation package, claiming game compatibility or playability without reproducible evidence, treating Xbox 360 as the current target or confusing it with original Xbox.
OWNERSHIP:         coordinator = envelope, distribution, integration, ledger/roadmap, final report; researcher = sole writer of the six docs/research/** files; architect/developer/tester = read-only analyses returned to coordinator; reviewer = independent read-only verdict. No uninstalled specialist role used.
ACCEPTANCE:        (1) all six documents exist and are read back; (2) 12 GTA VI component areas each state existing implementation/tests/proven function/gaps/dependencies/compat+perf impact/optimizations/measurement/risks with honest labels; (3) performance strategy defines metrics and methods with no fabricated values; (4) Xbox 360 analysis is separate, does not confuse original Xbox, and answers shared-vs-separate layer; (5) RESEARCH-007 and existing research assessed with license/source provenance and no adoption without approval; (6) phased plan adapted to real dependencies with no false completion claims; (7) risk/dependency overview present; (8) independent reviewer PASS with file:line findings recorded; (9) all 26 pre-existing working-tree entries preserved, plus exactly six new XB-0084 documents; (10) git diff --check EXIT=0 with known CRLF warning; (11) no product/ADR/permission/publication change.
TESTS:             Read-back of all eight FILES; git status --porcelain=v1 --untracked-files=all; git diff --check; independent reviewer review. No build/CTest: documentation-only package.
REVIEW:            reviewer (independent read-only, file:line findings, PASS/NEEDS_CHANGE/BLOCKED); coordinator records verdict; no self-acceptance.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   This order approves assessment/planning only. Not approved and not started: any implementation phase, ADR, interface change, dependency, tool/CI, code adoption, Series S target, or making Xbox 360 a current target.
```

`observed` — write sets are disjoint: researcher owns only `docs/research/**`; coordinator owns only ledger/roadmap; architect, developer, tester and reviewer are read-only. No two agents write the same file. `documented` — ADR-0004 (`DECISIONS.md:18-21`) and `ARCHITECTURE.md:13` keep Xbox Series X as sole target and Xbox 360 as deferred/optional; the owner order asks for *planning* only, which does not change that ADR. `unknown` — whether GTA VI or Splinter Cell: Double Agent technical requirements can be established from public sources at all; external truth and any game-specific performance number are `unknown` until evidence exists.

`documented` — the twelve component areas in ACCEPTANCE (2) are fixed by the owner order and are not an agent-invented taxonomy: (1) CPU instructions/threading; (2) memory/unified memory/bandwidth; (3) GPU / D3D12.x; (4) RDNA2/shaders; (5) shader translation/caching; (6) resources/textures; (7) system libraries/Xbox functions; (8) filesystem/storage; (9) audio; (10) input/controller; (11) CPU/GPU/audio synchronization; (12) scheduling/resource management. Any alternative taxonomy is a cross-check only, not a substitute for these twelve headings. The read-only developer inventory proposed a different 12-module list; it is recorded as `observed` feedback, not adopted as the acceptance taxonomy.

### XB-0084-C1 — citation-only correction package (review round 1)

```text
PACKAGE_ID:        XB-0084-C1
PACKAGE_TYPE:      BUGFIX (documentation citations only)
PRODUCT_BOUNDARY:  development-only
GOAL:              Fix five reviewer-confirmed broken/misleading citations without changing any assessment text or claim.
FILES:             docs/research/XB-0084_GTA_VI_COMPATIBILITY_INVENTORY.md
                   docs/research/XB-0084_GTA_VI_PERFORMANCE_PROFILE.md
                   docs/research/XB-0084_XBOX360_SPLINTER_CELL_ANALYSIS.md
                   docs/research/XB-0084_PRIORITIZED_DEVELOPMENT_PLAN.md
                   docs/research/XB-0084_RISK_DEPENDENCY_OVERVIEW.md
ALLOWED:           Replace exact citation strings only: src/profiles/config.cpp -> src/core/config.cpp; docs/AGENTS.md -> AGENTS.md; AGENTS.md:3-7 -> AGENTS.md:7-12; AGENTS.md:23 -> AGENTS.md:25; define W09-W12 in the risk overview or replace W09-W12/W09 with the inline GTA VI source URLs.
FORBIDDEN:         Any other text, claim, evidence label, heading, metric, phase, product/test/CMake/ADR/interface/permission/publication change; no new files; no deletion of existing findings.
ACCEPTANCE:        All reviewer findings C1-F1..C1-F5 resolved; no substantive sentence changed; read-back confirms exact replacements; reviewer re-review PASS.
TESTS:             Read-back of the five files; git status --porcelain=v1 --untracked-files=all still 32 entries; git diff --check EXIT=0 with known CRLF warning.
REVIEW:            Same independent reviewer, read-only, file:line verdict.
WORKTREE:          Main checkout <PROJECT_ROOT> on E:.
OWNER_APPROVALS:   None; mechanical correction inside the already approved XB-0084 documentation assessment.
```

`observed` — reviewer `ses_dfb7ecd50ffeK1pi553rHarRJ7` verdict `NEEDS_CHANGE`: C1-F1 `src/profiles/config.cpp` does not exist (INVENTORY:166, XBOX360:15, :68; real file `src/core/config.cpp:5-21`); C1-F2 `docs/AGENTS.md` does not exist (INVENTORY:32, PLAN:15; real root `AGENTS.md`); C1-F3 priority list is at `AGENTS.md:7-12`, not `:3-7` (PERFORMANCE:14, RISK:15); C1-F4 no-host-RAM-reservation is at `AGENTS.md:25`, not `:23` (INVENTORY:43, RISK:48); C1-F5 `W09-W12` are undefined in the six docs (RISK:37, :84, :93). Reviewer states all acceptance criteria are substantively met and all defects are low-severity citation issues; no product, ADR, interface, permission or publication change found.

### XB-0084 final result — assessment PASS, no implementation approved

`documented` — Independent reviewer final verdict `PASS` (`ses_dfb7ecd50ffeK1pi553rHarRJ7`, correction round 1). All C1-F1…C1-F5 findings verified resolved with file:line evidence: `INVENTORY:32,43,166`; `PERFORMANCE:14`; `XBOX360:15,68`; `PLAN:15`; `RISK:15,48,89-98`. No substantive text, evidence label, heading, metric, phase or claim changed; no new file beyond the six XB-0084 documents. XB-0084 is accepted for documentation assessment/planning only. Implementation of any phase, ADR/interface/dependency change, code adoption, Series S target, Xbox 360 as a current target, and any performance/compatibility claim without pre-registered evidence remain unapproved.

`observed` — Coordinator verification commands and actual results in the main checkout: `git branch --show-current` → `main`; `git status --porcelain=v1 --untracked-files=all` → **32 entries** (all 26 pre-existing preserved + exactly six new `docs/research/XB-0084_*.md` files); `git diff --check; "EXIT=$LASTEXITCODE"` → `warning: in the working copy of 'CMakePresets.json', CRLF will be replaced by LF the next time Git touches it`, `EXIT=0`; `git log -1 --format="%h %s"` → `10e5e75 Merge pull request #11 from comedius/feat/x64-cpu-instruction-support`; grep for `src/profiles/config.cpp|docs/AGENTS.md|AGENTS.md:3-7|AGENTS.md:23` → `No matches found`; read-back line counts INV 181, PERF 85, X360 86, LIC 113, PLAN 145, RISK 135. Reviewer commands and results are recorded in the review handoff; reviewer edited nothing. No build/CTest was run because the package has no product, test or CMake change and `AGENTS.md` ties build/CTest verification to implementation tasks.

`documented` — The six deliverables are `docs/research/XB-0084_GTA_VI_COMPATIBILITY_INVENTORY.md`, `XB-0084_GTA_VI_PERFORMANCE_PROFILE.md`, `XB-0084_XBOX360_SPLINTER_CELL_ANALYSIS.md`, `XB-0084_RESEARCH_LICENSE_ASSESSMENT.md`, `XB-0084_PRIORITIZED_DEVELOPMENT_PLAN.md`, `XB-0084_RISK_DEPENDENCY_OVERVIEW.md`. All 11 acceptance criteria are met: twelve owner-fixed component areas with ten required fields each; 16 methodology metrics with no invented FPS values; Xbox 360/SKU separation with `BWV6WF6XMFZX` vs `BPQ8RMCDSLRL`, shared host infrastructure vs separate guest layer, and ADR-0004 unchanged; RESEARCH-007/PS5 provenance and license assessment with no adoption approval; dependency-safe ten-phase plan; risk/dependency overview; independent PASS; 26+6 working tree; `git diff --check` EXIT=0; no product/ADR/permission/publication change.

`hypothesis` — The documented plan indicates CPU/memory correctness and evidence contracts as prerequisites before any game-level work. `unknown` — all GTA VI Series X internal requirements and performance values; Xbox 360 emulator internals and title behavior; external truth of web-sourced claims; RESEARCH-007 license compatibility; XBridge project license; D3D12.X equivalence, shader-stage formats and rendered-pixel correctness. No compatibility or playability is claimed for either title.

`documented` — Deliberately not implemented: no product code, tests, CMake/presets, public interfaces, ADRs, dependencies, JIT, native execution, D3D12.X, Xbox 360 module, code adoption, firmware/keys/game data, DRM work, agent files, permissions, `opencode.jsonc`, commits/pushes/merges/tags/releases/PRs, and no follow-up implementation package. The owner must separately approve any such work; this package ends after assessment and independent review.

## Current status addendum (XB-0084 / XB-0085 / XB-0086)

`observed` — this addendum is appended after the XB-0084 final result instead of inserting rows into the status table at `:9-20`. Six XB-0084 research documents cite ledger line numbers at `:469-497`; inserting lines before that range would break those citations. The addendum therefore preserves all historical line anchors. It is the authoritative current-status view; the earlier table remains a historical record.

| ID | Type | Boundary | Status | Responsible | Result / Evidence | Open owner decision |
|---|---|---|---|---|---|---|
| XB-0084 | RESEARCH | development-only | pass | researcher + coordinator + reviewer | Documentation inventory complete: six `docs/research/XB-0084_*.md` documents plus ledger/roadmap records; reviewer round 1 `NEEDS_CHANGE` → correction `XB-0084-C1` → final `PASS`. No product code, tests, CMake/presets, ADR/interface/dependency/permission/publication change; no implementation approved. GTA VI Series X compatibility/performance not demonstrated; Xbox 360 / Splinter Cell: Double Agent remains deferred, planning-only and not a current target. Evidence: "XB-0084 final result" section above. | Separate owner approval required before any implementation, ADR, code adoption, Series S target or Xbox 360 current-target change |
| XB-0085 | RESEARCH | development-only | pass | coordinator + reviewer | Read-only publication audit complete: all 32 entries audited with 13 `READY_FOR_COMMIT`, 18 `NEEDS_REVIEW`, 1 `DO_NOT_PUBLISH`; reviewer round 1 `NEEDS_CHANGE` → correction round 1 → final `PASS`. No publication, no index or reference changes; PR #1 remains open and untouched. No product, test, CMake, preset, agent, permission or publication change. | Owner decision required for any publication action or PR change |
| XB-0086 | BUGFIX | development-only | pass | coordinator + architect + developer + tester + researcher + reviewer | Documentation cleanup complete. Four read-only audits plus independent PASS for the four initially edited files (`AGENT_WORKFLOW.md`, `RELEASE_CHECKLIST.md`, `AGENT_TASKS.md`, `ROADMAP.md`); follow-up `XB-0086-C1` applied the owner-approved scoped permission and completed `docs/PORTABLE_SETUP.md`. Final independent review: round 1 `NEEDS_CHANGE` (one MEDIUM `CMakeCache.txt` citation defect at `:58,:102`) → correction → final `PASS`. No product, test, CMake, preset, build, CTest, permission, ADR, interface, dependency, publication or PR change. | None for XB-0086 itself; publication and all other owner decisions remain separately gated |

## XB-0085 — read-only publication audit (pass)

`documented` — XB-0085 audited all 32 publication-candidate entries: 13 `READY_FOR_COMMIT`, 18 `NEEDS_REVIEW`, 1 `DO_NOT_PUBLISH`. No publication was performed; no index or reference changes were made; PR #1 remains open and untouched. The 32-entry inventory comprised 26 pre-existing entries plus the six XB-0084 documents.

`observed` — Independent review round 1 `NEEDS_CHANGE` → correction round 1 → final reviewer `PASS`. No product code, tests, CMake, presets, agent files, permissions, publication action, index change or reference change was part of this package. The `[x]` VS2026 item in `docs/RELEASE_CHECKLIST.md:47` was independently re-checked during XB-0086 and retained because its build/test evidence is findable at `:93-97`; no build or CTest was rerun.

`unknown` — PR #1 state and the 29-commit divergence from `main` were not independently re-verified by XB-0086 agents; the recorded XB-0085 figures are treated as that package's documented outcome.

`observed` — The open-decision wording in historical rows `:13` and `:18` is superseded by SETUP-005 (`pass`, no open VS2026/VS2022 decision) and by the later installed-role/supersession notes. It is historical evidence only and must not be read as a current owner decision.

`observed` — XB-0086 coordinator edits attempted in `AGENT_WORKFLOW.md`, `RELEASE_CHECKLIST.md`, `AGENT_TASKS.md` and `ROADMAP.md` succeeded. The edit to `docs/PORTABLE_SETUP.md` returned `{"error":{"type":"permission.rejected","message":"Permission denied: edit"}}`. No alternative path, subagent or configuration change was used to bypass it.

## XB-0086 review record — PASS for four edited files, package still in_progress

`documented` — Independent reviewer verdict `PASS` (session `ses_dfb4f6c93ffehZ6NiedpQBdZ0I`) for the four files actually edited: `AGENT_WORKFLOW.md:136`, `RELEASE_CHECKLIST.md:52`, `AGENT_TASKS.md:7,536-556`, `ROADMAP.md:3,27,31-39`. No correctness, evidence-integrity, status-escalation or citation-integrity defect was found. The reviewer explicitly accepted the unchanged `PORTABLE_SETUP.md` as an open owner blocker and stated that this PASS does not close XB-0086.

`observed` — Reviewer verification: branch `main`; HEAD `10e5e7515ea9306dfe0fba45d87b01bed801551f`; 32 working-tree entries; empty index; `git diff --check` `EXIT=0` with the known CRLF warning; `AGENT_TASKS.md:469` anchor unchanged; 13 XB-0084 citations resolve within `:469-497`; trailing-whitespace search returned no output; `Select-String .opencode/agents/coordinator.md -Pattern PORTABLE_SETUP` returned no output. The reviewer edited nothing.

`observed` — Coordinator verification before this record: same branch/HEAD/32-entry/0-staged results; `git diff --check` `EXIT=0`; `git diff --numstat` empty because the edited docs are untracked, so read-back and the reviewer's whitespace search are the applicable evidence for those paths.

`documented` — Remaining owner action: persist one scoped `edit` allow for `docs/PORTABLE_SETUP.md` in `.opencode/agents/coordinator.md` after the existing `docs/RELEASE_CHECKLIST.md` rule, then reload OpenCode so the rule becomes effective. The prepared replacements cover `PORTABLE_SETUP.md:21,30-31,40,58,59,97` and preserve the status separation `observed` / `pending` / `blocked` / `unknown`. XB-0086 remains `in_progress`; no publication step is authorized.

## XB-0086-C1 — completion of Portable Setup documentation (pass)

`documented` — The owner added the scoped `edit` allow for `docs/PORTABLE_SETUP.md` in `.opencode/agents/coordinator.md:20-22` and restarted OpenCode. XB-0086-C1 was limited to `docs/PORTABLE_SETUP.md` and this ledger; no other file was edited by the coordinator in the follow-up. `docs/ROADMAP.md` was intentionally left unchanged.

`observed` — `docs/PORTABLE_SETUP.md` was read back in full (111 lines after correction) and corrected at `:11,:21,:29-32,:39,:41,:56-61,:82-85,:93,:96,:98-109`. It now states the first-machine toolchain (VS2026 Community, C++/C# support, CMake `4.3.1-msvc1`, generator `Visual Studio 18 2026`), the local uncommitted preset switch versus the GitHub `HEAD 10e5e75` still containing VS2022, unchanged `ninja-debug`, first-machine-only historical build/test evidence, planned shared projects on the external SSD, locally required tools on each machine, the open/planned 13-session transfer (3 project + 10 setup, `C:` originals untouched), the failed worktree opening with an existing but unregistered checkout at `E:\XBridge 1.0\worktrees\portability-worktree-test`, and explicit `observed` / `documented` / `planned` / `blocked` / `unknown` separation. No full portability, session-transfer or worktree-success claim is made.

`documented` — Independent reviewer verdict for the fifth file: round 1 `NEEDS_CHANGE` (MEDIUM: wrong `CMakeCache.txt` citation at `:58,:102`) → targeted correction of both occurrences to `:222,254` → final `PASS` (session `ses_dfb38042fffeU3MOxqXVwnB7vM`). The earlier PASS for the four initially edited files (`ses_dfb4f6c93ffehZ6NiedpQBdZ0I`) is retained. This completes XB-0086; the package status in the addendum table above is updated to `pass`.

`observed` — Correction read-back: `grep 'CMakeCache\.txt:(219,251|222,254)' docs/PORTABLE_SETUP.md` → exactly two matches, both `222,254`; the reviewer verified `CMakeCache.txt:222` = `CMAKE_COMMAND:INTERNAL=.../Visual Studio/18/Community/...` and `:254` = `CMAKE_GENERATOR:INTERNAL=Visual Studio 18 2026`. No build, CTest, staging, commit, push, merge, branch change, fetch, reset, stash, clean, PR change or publication was performed.

`observed` — Final checks: branch `main`; HEAD `10e5e7515ea9306dfe0fba45d87b01bed801551f`; 32 working-tree entries before and after the follow-up; 0 staged entries; `git diff --check` `EXIT=0` with the known `CMakePresets.json` CRLF warning. Out-of-scope files were not edited in XB-0086-C1.

`observed` — Supersession: the earlier statements at `:562`, `:564` and `:566` are historical snapshots from before the owner granted the scoped permission; they are superseded by this section. All other historical ledger content is preserved.

## XB-0086-C2 — final status alignment and publication group 0 definition

`documented` — XB-0086-C2 closed the last documentation point: `docs/ROADMAP.md:33` now records XB-0086 as complete with both reviewer PASS session IDs, and `docs/ROADMAP.md:39` marks sequence steps (1)-(2) complete while leaving steps (3)-(5) unchanged. XB-0084 remains a completed assessment/planning package, XB-0085 a completed read-only publication audit, XB-0080-A/B remain `planned`/blocked by owner decisions, and GTA VI implementation remains not approved. No review PASS is treated as implementation, build/CTest, game-compatibility or publication approval.

`observed` — Four read-only audits were completed before any edit: architect (`ses_dfb2cda3cffd9WU33Sw9ZahR2C`) consistency/boundary, developer (`ses_dfb2cda3affe2PAnOs63wbyxT7`) path/file-set, tester (`ses_dfb2cda2affejmuQdijhbKD6sm`) evidence/status, researcher (`ses_dfb2cda21ffeF8oir7SbJQQkvl`) references/citations. All four confirmed baseline branch `main`, HEAD `10e5e7515ea9306dfe0fba45d87b01bed801551f`, 32 working-tree entries with `--untracked-files=all`, 0 staged entries and tracked diff 5 files / 28+ / 2-, plus the two XB-0086 PASS records at `:560` and `:574`. Researcher confirmed all 13 XB-0084 citations still resolve into the stable `:469-497` range; appending after `:580` shifts none of them. The developer's `git status --porcelain=v1` count of 24 entries used directory summarisation without `--untracked-files=all` and is consistent with 32.

`documented` — Publication group 0 is defined here, for the first time in a project document, as exactly these five development-infrastructure files: `docs/AGENT_WORKFLOW.md`, `docs/RELEASE_CHECKLIST.md`, `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`, `docs/PORTABLE_SETUP.md`. `AGENTS.md` is excluded per the owner's XB-0086-C2 instruction because its permanent versioning decision remains open; that specific decision is not recorded in the audited files (`unknown`) and is not resolved here. No other file belongs to group 0. This definition is a documentation/publication grouping only: it does not authorize staging, commit, push, merge, tag, release, PR change, index/reference change, product inclusion, implementation, ADR approval, game-compatibility claim or any build/CTest result. Any publication action requires a separate explicit owner decision.

`observed` — Clarification of the historical row at `:544`: "no ... permission ... change" means no agent-initiated permission change. The only recorded permission action is the owner's scoped `docs/PORTABLE_SETUP.md` edit allow in `.opencode/agents/coordinator.md:20-22` (`:570`), outside the coordinator's write scope. The row itself is preserved unchanged. The same applies to `:558`, `:562`, `:564` and `:566`, which remain historical pre-C1 snapshots already superseded at `:580`.

`observed` — No build, CTest, staging, commit, push, merge, tag, release, branch change, fetch, reset, stash, clean, PR change or publication was performed. No product code, test, CMake, preset, agent configuration or permission file was edited.

`documented` — Independent reviewer verdict (session `ses_dfb25ec0bffeJ2AVA6w21ArW6R`): `PASS` with no blocking or non-blocking findings. Verified: exact ROADMAP replacements at `:33,:39` with neighbouring items and anchors intact; append-only ledger block at `:582-592` with `:469` anchor and all 13 XB-0084 citations resolving; group 0 exactly five files with no extras and no publication authorization; no PASS inflated into build/CTest, implementation, compatibility or publication approval; write scope limited to the two permitted docs. The reviewer re-ran branch `main`, HEAD `10e5e7515ea9306dfe0fba45d87b01bed801551f`, 32 entries, 0 staged, tracked diff 5 files / 28+ / 2- and `git diff --check` `EXIT=0` with the known CRLF warning. This paragraph records that verdict after the fact; the reviewer checked the state before this line was appended and edited nothing.

