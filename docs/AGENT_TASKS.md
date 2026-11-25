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

## RV-P1-C1 — bundled corrections from the failed P1 review (in_progress / needs_owner)

PACKAGE_ID: RV-P1-C1
PACKAGE_TYPE: BUGFIX
PRODUCT_BOUNDARY: development-only
GOAL: Apply only the owner-approved P1 review corrections and prepare the owner-only patches, without starting Package 2.
FILES: `docs/DEVELOPMENT_ENVIRONMENT.md`, `docs/PORTABLE_SETUP.md`, `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`; owner patches for `.gitignore`, `ai/README.md`, `opencode.jsonc`.
ALLOWED: Coordinator edits limited to `docs/*.md` explicitly allowed in `.opencode/agents/coordinator.md:5-55`; read-only inspection of all target files; prepared patches for owner-only files.
FORBIDDEN: No edits to `.gitignore`, `ai/README.md`, `opencode.jsonc`, `.opencode/**`, `AGENTS.md`, `docs/DECISIONS.md`, product code, tests, CMake or presets; no Package 2 envelope, publication group 2, staging, commit, push, merge, tag, PR, history change or permission broadening.
ACCEPTANCE: Every owner-approved P1 point is either applied by an authorized writer or recorded as an `OWNER_DECISION` with a complete patch; fresh independent reviewer verdict is obtained before P1 is called complete; no unapproved file is modified.
TESTS: Documentation-only: exact `git status --porcelain=v1 --untracked-files=all`, `git rev-parse HEAD`, `git branch --show-current`, `git diff --check`, `git diff HEAD --no-ext-diff`, `git diff HEAD --no-ext-diff --stat`, `git diff HEAD --no-ext-diff --name-only` plus full read-back. No product build or CTest for this documentation-only package.
REVIEW: Independent `reviewer` re-check mandatory after all corrections; architect skipped because this is pure documentation care with no interface, module-boundary, dependency or ADR change (`.opencode/agents/coordinator.md:142`). Reviewer prompt must be read-only (`read`/`glob`/`grep` only).
WORKTREE: None; main checkout `D:\XBridge 1.0\XBridge-Series-Github`.
OWNER_APPROVALS: Owner approved the four P1 corrections and the re-review. Owner-only edits for `.gitignore`, `ai/README.md` and `opencode.jsonc` remain `OWNER_DECISION` because the coordinator has `edit:"*" deny` with no allow for those paths.

`observed` — Baseline before coordinator edits: branch `git branch --show-current` → `docs/opencode-publication-group-2`; `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git status --porcelain=v1 --untracked-files=all` → 24 entries (11 modified, 13 untracked). The combined command `git status --short --branch; git rev-parse HEAD` was rejected: `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}`; only exact allowlisted commands are usable. Web policy verification via `fetch("https://opencode.ai/docs/llms.txt")` and `fetch("https://opencode.ai/docs/permissions/")` was rejected: `Permission denied: webfetch`.

`observed` — Finding disposition (reviewer IDs as retained in the approval): RV-P1-01 `.gitignore` backup/debug exclusions → `ACTIONABLE` but blocked, owner patch below; RV-P1-02 mutating Code Mode tools → `ACTIONABLE` but blocked and policy syntax `unknown`, owner patch below; RV-P1-03 machine identity/hardware → `ACTIONABLE`, applied by labeling `owner-provided` in `docs/DEVELOPMENT_ENVIRONMENT.md:3,28,30,54-59` and `docs/PORTABLE_SETUP.md:59`; RV-P1-04 stale/cross references → `ACTIONABLE`, applied in `docs/DEVELOPMENT_ENVIRONMENT.md:163-164,187` and `docs/PORTABLE_SETUP.md:31-33`; RV-P1-05 absent Package 2 envelope/group 2 → `OWNER_DECISION`/`unknown`, documented but not invented; RV-P1-06 `.git/info/exclude` and `/.env.*` side effect → `OWNER_DECISION`, no agent edit and patch deliberately excludes `/.env.*`; RV-P1-07 ledger snapshot discrepancy → `ACTIONABLE`, documented as supersession in `docs/PORTABLE_SETUP.md:59-60,109` and this block; RV-P1-08 `ai/README.md:13` PR instruction → `ACTIONABLE` but blocked, owner patch below; RV-P1-09 finding/severity/category recount → `UNKNOWN` from the retained summary, left for the fresh reviewer to recount rather than fabricated here.

`documented` — `docs/DEVELOPMENT_ENVIRONMENT.md` now separates `observed` path `D:\XBridge 1.0\XBridge-Series-Github` from `owner-provided` machine identity and hardware. Current observed values are VS2026 Community at `C:\Program Files\Microsoft Visual Studio\18\Community`, MSVC toolsets `14.44.35207`/`14.52.37025`, SDKs `10.0.26100.0`/`10.0.28000.0`, CMake/CTest `4.3.1-msvc1` by simple command name through `tester` (exit codes `unknown`), GitHub Desktop `app-3.7.0`, OpenCode resource version `2.4.1`. `E:`, `main`/`10e5e75`/32 entries and VS2022 are marked historical/superseded. Plain `cmake`/`ctest` reachability is `observed`; the former `CommandNotFoundException` is historical. The CRLF warning is `observed`, current exit code `unknown`.

`documented` — `docs/PORTABLE_SETUP.md` now cites `opencode.jsonc:73-109` plus agent frontmatter for unavailable `opencode debug config/agents`, records current observed toolchain values and the `58aa1f60…` branch/HEAD supersession, and marks the XB-0085 32-entry audit as a historical snapshot with PR state not re-verified.

`documented` — Owner patch 1, `.gitignore` (prepared, NOT applied):

```diff
--- a/.gitignore
+++ b/.gitignore
@@ -13,3 +13,7 @@
 *.so
 *.dylib
+
+# Owner-only development artifacts; keep out of publication.
+/.owner-patch-backup/
+/CMakePresets.json.backup
+/debug-*.txt
```

`documented` — Owner patch 2, `ai/README.md` (prepared, NOT applied):

```diff
--- a/ai/README.md
+++ b/ai/README.md
@@ -13 +13 @@
-7. Open a pull request; never merge directly to the protected main branch.
+7. Hand the result to the project owner; the owner performs all Git/GitHub publication actions.
@@ -28 +28 @@
-Step 7 of the workflow above is performed by the project owner. Agents must not create pull requests or perform any other Git/GitHub publication action.
+Step 7 is explicit: agents hand off; the project owner alone may open a pull request or perform any other Git/GitHub publication action.
```

`documented` — Owner patch 3, `opencode.jsonc` (prepared, NOT applied; resource spelling `unknown` because webfetch was denied):

```diff
--- a/opencode.jsonc
+++ b/opencode.jsonc
@@ -151,5 +151,12 @@
     { "action": "tool.use", "resource": "edit:*.env", "effect": "deny" },
     // Mirrors `read:*.env.*`. Whole-value `*` spans `/`; this also denies `.env.example` (side effect).
     // Pattern semantics are owner-documented; no runtime behavior is claimed as tested here.
-    { "action": "tool.use", "resource": "edit:*.env.*", "effect": "deny" }
+    { "action": "tool.use", "resource": "edit:*.env.*", "effect": "deny" },
+
+    // Mutating Code Mode tools; confirm resource spelling against OpenCode V2 before applying.
+    { "action": "tool.use", "resource": "automation.create", "effect": "deny" },
+    { "action": "tool.use", "resource": "automation.delete", "effect": "deny" },
+    { "action": "tool.use", "resource": "opencode.session.move", "effect": "deny" },
+    { "action": "tool.use", "resource": "opencode.worktree_create", "effect": "deny" },
+    { "action": "tool.use", "resource": "opencode.worktree_remove", "effect": "deny" }
   ]
 }
```

`documented` — No `/.env.*` rule is proposed because it would also deny `.env.example` (`docs/DEVELOPMENT_ENVIRONMENT.md:164`); `.git/info/exclude` is local owner territory and is not edited by any agent. `opencode.worktree_list` remains read-only and is not denied.

`documented` — P1 is not complete. After this coordinator-authorized portion, the remaining owner actions are: apply owner patch 1 (`.gitignore`) and owner patch 2 (`ai/README.md`), then trigger a fresh independent reviewer re-check. Owner patch 3 is withdrawn by owner decision and must not be applied. Package 2 remains closed and needs a separate owner-approved envelope.

`OWNER_DECISION` — Owner selected option (c) “Leave unapplied” for RV-P1-02. Classification: `OWNER_DECISION`, not `ACTIONABLE`; no agent correction is required for this finding. The prepared `opencode.jsonc` patch above is NOT applied and is now superseded/withdrawn as written; exact valid `<action>:<value>` strings remain `unknown`, and no permission broadening or further Code-Mode investigation is authorized in RV-P1-C1. RV-P1-02 is recorded as a documented limitation, not as a fix. P1 can be completed only after the owner applies patches 1 and 2, the state is recaptured/read back, and the mandatory independent reviewer re-check passes. Package 2 remains closed.

`observed` — Final coordinator self-check after all authorized edits: `git status --porcelain=v1 --untracked-files=all` → 26 entries (13 modified, 13 untracked); `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git diff --cached --name-only` → no output (empty index). The increase from the 24-entry baseline is exactly the two newly modified tracked docs `docs/AGENT_TASKS.md` and `docs/ROADMAP.md`; `docs/PORTABLE_SETUP.md` became modified and `docs/DEVELOPMENT_ENVIRONMENT.md` remained untracked.

`observed` — Verification commands and actual results: `git diff --check` → only `warning: in the working copy of 'CMakePresets.json', CRLF will be replaced by LF the next time Git touches it`, no error line; the shell tool did not expose an exit code, so the current exit state is `unknown` rather than a newly claimed `EXIT=0`. `git diff HEAD --no-ext-diff --stat` → 14 tracked files changed with the known CRLF warning; the exact insertion/deletion count is command-output evidence in the final coordinator report and is deliberately not frozen here because every appended ledger paragraph changes it. `git diff HEAD --no-ext-diff --name-only` → the 14 tracked paths (six `.opencode/agents/*.md`, `AGENTS.md`, `CMakePresets.json`, `ai/README.md`, `docs/AGENT_TASKS.md`, `docs/ARCHITECTURE.md`, `docs/PORTABLE_SETUP.md`, `docs/ROADMAP.md`, `opencode.jsonc`). `git diff HEAD --no-ext-diff` was executed and its returned diff preview inspected; the changed portions of all four coordinator-written docs were additionally read back in full with `read`. No product code, test, CMake, preset, agent configuration or permission file was edited by the coordinator in this package.

`observed` — `.owner-patch-backup/**`, `CMakePresets.json.backup` and `debug-*.txt` remain untracked and unchanged; none was staged, deleted, moved or overwritten. No build, CTest, staging, commit, push, merge, tag, release, fetch, reset, stash, clean, branch change, PR change or publication was performed. The independent reviewer re-check has not yet been started because the owner-only patches are still unapplied.

`documented` — Policy-syntax clarification after the owner chose “Erst Policy-Syntax klären”. Official OpenCode documentation was read successfully in the in-app browser: `https://opencode.ai/docs/policies/` states that a `tool.use` statement hard-denies a permission check and that the resource has the form `<action>:<value>`; examples include `shell:git push *`, `edit:*.env` and `webfetch:*`; invalid statements are dropped with a server-log warning. `https://opencode.ai/docs/tools/` states that denying `execute` with resource `*` removes Code Mode, that nested built-in and MCP tools still enforce their own permissions, that `worktree_remove` uses the `edit` permission with the worktree directory, and that the other listed worktree utilities request no built-in permission action. `https://opencode.ai/docs/permissions/` lists built-in actions (`read`, `edit`, `glob`, `grep`, `shell`, `subagent`, `skill`, `question`, `webfetch`, `websearch`, `generate_image`, `external_directory`, MCP actions and `execute`) and does not list bare names such as `automation.create`, `opencode.session.move`, `opencode.worktree_create` or `opencode.worktree_remove` as built-in actions.

`observed` — Additional browser inspection of official upstream `dev`-branch source was treated as non-authoritative for the installed OpenCode resource version `2.4.1`. It showed permission checks and tool registration for automation/session/worktree areas but did not establish exact resource strings for the mutating Code-Mode entries. Exact valid `<action>:<value>` resources for targeted denial of `automation.create`, `automation.delete`, `opencode.session.move`, `opencode.worktree_create` and `opencode.worktree_remove` in this installation therefore remain `unknown`.

`documented` — Owner patch 3 above is invalid as written and MUST NOT be applied: its bare Code-Mode tool names do not match the documented `<action>:<value>` resource form. Broad `execute:*` is documented as removing Code Mode, but it is broader than the owner-approved targeted correction and is not authorized by this package. No `opencode.jsonc` edit, permission change or bypass was performed.

`documented` — The RV-P1-02 decision is resolved at `:677`: the owner selected option (c), leave unapplied. No further Code-Mode syntax decision is pending. RV-P1-C1 remains `in_progress / needs_owner` only because owner patches 1 and 2 are not yet applied and the mandatory reviewer re-check has not run; P1 is not complete, and Package 2 remains closed.

## RV-P1-C2 — bundled correction round 1 for RV-P1-C1 review (in_progress)

PACKAGE_ID: RV-P1-C2
PACKAGE_TYPE: BUGFIX
PRODUCT_BOUNDARY: development-only
GOAL: Resolve the two ACTIONABLE findings from the RV-P1-C1 re-review without reopening Package 2.
FILES: `docs/AGENT_TASKS.md` (package record), `docs/ROADMAP.md`, `docs/DEVELOPMENT_ENVIRONMENT.md`.
ALLOWED: Coordinator edits to these three permission-approved `docs/*.md` paths (`.opencode/agents/coordinator.md:9-25`); read-only inspection of cited evidence files.
FORBIDDEN: No edits to `.gitignore`, `ai/README.md`, `opencode.jsonc`, `.opencode/**`, `AGENTS.md`, product code, tests, CMake or presets; no Package 2 envelope, publication group 2, staging, commit, push, merge, tag, PR, history change, permission broadening, build or CTest.
ACCEPTANCE: `docs/ROADMAP.md` no longer instructs applying the withdrawn `opencode.jsonc` patch; `docs/DEVELOPMENT_ENVIRONMENT.md` evidence pointers resolve to claimed content; no other RV-P1-C1 state is reopened; independent reviewer re-check passes.
TESTS: Read-back of the three files plus exact `git status --porcelain=v1 --untracked-files=all`, `git rev-parse HEAD`, `git branch --show-current`, `git diff --check`, `git diff HEAD --no-ext-diff --stat` and `git diff HEAD --no-ext-diff --name-only`. No product build or CTest for this documentation-only package.
REVIEW: Mandatory read-only reviewer re-check after corrections. Architect skipped: pure documentation care with no interface, module-boundary, dependency or ADR change.
WORKTREE: None; main checkout `D:\XBridge 1.0\XBridge-Series-Github`.
OWNER_APPROVALS: RV-P1-02 leave-unapplied decision already recorded at `:677`; no new owner decision required.

`observed` — Independent reviewer session `ses_df2403fb3ffeQJAOnU8s9587C7` returned `NEEDS_CHANGE` with two findings. Classification: RV-P1-REV-01 (HIGH) `ACTIONABLE`; RV-P1-REV-02 (LOW) `ACTIONABLE`; no `OWNER_DECISION`, `NON_BLOCKING` or `UNKNOWN` findings. Case B applies, with one bundled correction package for the single coordinator writer. Root causes: (a) the owner’s RV-P1-02 leave-unapplied decision was not propagated to the roadmap sequence; (b) two stale evidence pointers in `docs/DEVELOPMENT_ENVIRONMENT.md` do not resolve to the claimed content. This is bundled correction round 1; at most two rounds are permitted.

`observed` — Bundled correction round 1 applied by the coordinator in the three allowed docs. `docs/ROADMAP.md:39,41` now states that only patches 1 and 2 may be applied, that patch 3 (`opencode.jsonc`) is withdrawn/invalid/MUST NOT be applied, and that no Code-Mode syntax decision is pending. `docs/DEVELOPMENT_ENVIRONMENT.md:109` now cites `docs/RELEASE_CHECKLIST.md:47` and `docs/AGENT_TASKS.md:92,97` for the CRLF warning instead of `docs/AGENT_WORKFLOW.md`; `:164` now points directly to `docs/AGENT_TASKS.md:677,689-691` for the Code-Mode owner decision/withdrawal instead of the non-existent Section 12 row. A grep for `three owner-only|apply the three|drei owner-only|siehe Abschnitt 12|AGENT_WORKFLOW.md` dokumentiert` returned no matches. Read-back confirmed all four corrected locations.

`observed` — Post-correction verification: `git status --porcelain=v1 --untracked-files=all` → 26 entries (13 modified, 13 untracked), unchanged; `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git diff --check` → only the known `CMakePresets.json` CRLF warning, exit state `unknown`; `git diff HEAD --no-ext-diff --stat` → 14 tracked files; the exact insertion/deletion count is command-output evidence in the coordinator report and is deliberately not frozen here because appended ledger paragraphs change it; `git diff HEAD --no-ext-diff --name-only` → the same 14 tracked paths. No product code, test, CMake, preset, agent configuration, permission, `.gitignore`, `ai/README.md` or `opencode.jsonc` edit; no build, CTest, staging or publication.

`documented` — Independent reviewer follow-up session `ses_df2403fb3ffeQJAOnU8s9587C7` returned `PASS` for the RV-P1-C2 documentation correction state (round 1 of at most two). Round-2 finding classification: RV-P1-C2-REV-01 (LOW) `NON_BLOCKING` — a self-referential grep match at `docs/AGENT_TASKS.md:710`, caused by quoting the search pattern in the verification record; no operative stale text remains. No `ACTIONABLE`, `OWNER_DECISION` or `UNKNOWN` finding was introduced. Case A applies: no correction required, no second review started, existing `PASS` retained. Both RV-P1-REV-01 and RV-P1-REV-02 are closed. The `PASS` covers only the documentation correction state; it does not approve build/test results, implementation, Xbox compatibility, publication or permission changes.

`documented` — RV-P1-C2 is complete, and RV-P1-C1 P1 is complete on the documentation-only state after successful independent re-review. Open items remain separate from defects: `OWNER_DECISION` RV-P1-01 (`.gitignore` patch prepared, not applied), RV-P1-02 (leave unapplied; `opencode.jsonc` patch withdrawn), RV-P1-05 (no Package 2 envelope or publication group 2 defined), RV-P1-06 (`.git/info/exclude` and `/.env.*` side effect), RV-P1-08 (`ai/README.md` patch prepared, not applied). Manual owner action, if desired: apply patches 1 and 2; patch 3 must not be applied. `UNKNOWN` items remain: RV-P1-09 original severity/category recount, exact Code-Mode action strings (decision closed without a fix), current `git diff --check` exit state, and any Package 2 content. NON_BLOCKING RV-P1-C2-REV-01 is recorded for transparency only and deliberately not edited after the PASS.

`observed` — This completion-recording edit occurred after the reviewer PASS and changes no reviewed correction content. Latest command results: `git status --porcelain=v1 --untracked-files=all` → 26 entries; `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git diff --check` → known `CMakePresets.json` CRLF warning only; `git diff HEAD --no-ext-diff --name-only` → the same 14 tracked paths. No build, CTest, staging, commit, push, merge, tag, PR, publication, permission or Package 2 action.

## RV-P1-C3 — owner instruction supersession and Code-Mode security reopened (in_progress / needs_owner)

PACKAGE_ID: RV-P1-C3
PACKAGE_TYPE: BUGFIX
PRODUCT_BOUNDARY: development-only
GOAL: Reopen the Code-Mode security portion of P1 under the owner’s renewed explicit deny decision, verify OpenCode 2.4.1 syntax, and preserve the already reviewed documentation corrections.
FILES: `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`; owner-only patch targets remain `.gitignore` and `opencode.jsonc`.
ALLOWED: Coordinator edits limited to permission-approved `docs/*.md` paths; read-only research and architecture analysis; preparation of complete owner patches.
FORBIDDEN: No edit to `opencode.jsonc`, `.gitignore`, `ai/README.md`, `.opencode/**`, `AGENTS.md`, product code, tests, CMake, presets, permissions or agent configuration; no Package 2 execution, publication, commit, push, merge, tag, PR or history change.
ACCEPTANCE: OpenCode 2.4.1 rule syntax is evidence-labeled; a technically effective deny is either applied by the owner or explicitly stopped for owner decision if unsupported; target `.gitignore` rules are prepared unchanged; `ai/README.md` remains untouched; P1 status reflects the reopened state until a fresh reviewer verdict covers this supersession.
TESTS: Documentation-only: read-back, exact Git status/HEAD/branch/diff checks, security-rule consistency check, backup/debug path presence check. No product build or CTest.
REVIEW: Fresh independent reviewer verdict required after the reopened P1 items are resolved. Architect skipped for this reopening; architect is separately engaged only for the Package 2 proposal.
WORKTREE: None; main checkout `D:\XBridge 1.0\XBridge-Series-Github`.
OWNER_APPROVALS: Owner reaffirmed mutating Code-Mode deny decision and approved the three targeted `.gitignore` rules; `ai/README.md` remains blocked pending a separate owner release.

`OWNER_DECISION` — The new owner instruction explicitly supersedes the earlier option (c) “leave unapplied” for RV-P1-02. The previous withdrawal of the `opencode.jsonc` patch did not cancel the owner’s security decision; mutating Code-Mode tools must again be denied if a supported OpenCode 2.4.1 rule exists. If no reliable technical lock is established, the change is stopped and documented for owner decision rather than simulated with ineffective syntax. The three approved `.gitignore` rules are `/.owner-patch-backup/`, `/CMakePresets.json.backup` and `/debug-*.txt`; `.env.example` must remain available. Owner-approved documentation corrections already reviewed are preserved. `ai/README.md` is not edited.

`observed` — Effective write rights rechecked before any assignment: coordinator, developer, tester and researcher all have `edit` deny for `opencode.jsonc`; coordinator also denies `.gitignore` and `ai/**`, developer denies `ai/**` and all coordination docs, tester and researcher only allow their own scoped docs. No installed agent can apply the owner-only patch, so any resulting config change must be applied by the owner.

`observed` — Read-only `researcher` session `ses_df22fa77affebBDIGkCZ25tdx4` and read-only `architect` session `ses_df22fa779ffePSfY4Burv04Det` were launched in parallel for, respectively, version-specific OpenCode 2.4.1 permission syntax evidence and the Package 2 architecture proposal. Both were instructed not to edit, not to run shell, not to start subagents and not to change provider/model. P1 is reopened until the research result is evaluated and a fresh reviewer verdict is obtained; Package 2 remains prepared-only and not started.

`observed` — Architect session `ses_df22fa779ffePSfY4Burv04Det` completed read-only with no edits, shell calls, subagents or owner questions. It proposed `XB-P2` as a documentation-only FEATURE with coordinator as sole writer, six new docs (`docs/AGENT_CORE_ARCHITECTURE.md`, `docs/AGENT_AUTONOMY.md`, `docs/OPENCODE_AUTOMATION_PLAN.md`, `docs/AUTOMATED_QUALITY_GATES.md`, `docs/GITHUB_AGENT_WORKFLOW.md`, `docs/DESKTOP_MIGRATION_CHECKLIST.md`) plus cross-links from existing coordination docs. The proposal keeps `docs/AGENT_WORKFLOW.md` as central executable workflow, treats Agent Core as development documentation rather than product runtime, uses only the six installed roles, keeps specialists planned, defines a platform-neutral adapter contract with OpenCode now and future/planned Codex/Claude Code/GitHub Copilot mappings, and specifies a 12-step owner-gated automation flow, persistence/recovery, quota pause and token-efficiency rules. It explicitly forbids config/permission/product/publication changes and states Package 2 is not started. Owner approval of the envelope and document structure is required before execution.

`documented` — Read-only researcher session `ses_df22fa77affebBDIGkCZ25tdx4` identified OpenCode v2.4.1 as release commit `6f0514052a3c859894ad01bb14cc14dda1e44b96` (`6f05140`) from `https://api.github.com/repos/anomalyco/opencode/releases/tags/v2.4.1` and `https://github.com/anomalyco/opencode/releases/tag/v2.4.1`. Source evidence from the v2.4.1 tag shows that tool-identity suppression uses top-level `permissions` with `resource: "*", effect: "deny"`, not `policies.tool.use` bare names. `policies.mdx` v2.4.1 states `tool.use` resources are `<action>:<value>`; arbitrary strings in that field are schema-valid but do not match generated `<action>:<resource>` events, so the previously prepared bare-name patch is inert and must remain unapplied. No runtime verification was performed; actual behavior in the installed 2.4.1 session remains `unknown`.

`observed` — Exact v2.4.1 action names source-derived by the researcher: `automation_create`, `automation_delete`, `opencode_session_move`, `opencode_worktree_create`; these use underscores from namespace/name normalization. `opencode_worktree_remove` has no tool-specific action: v2.4.1 registers it with `permission: "edit"`, so only a broad `edit` deny can affect it. Evidence URLs: `https://github.com/anomalyco/opencode/blob/v2.4.1/packages/core/src/tool/runtime.ts`, `.../tool.ts`, `.../tool/plugin/automation.ts`, `.../tool/plugin/opencode.ts`, `.../automation.ts`, and `.../config/plugin/policy.ts`.

`observed` — Agent-rule override audit: grep over `.opencode/agents/**` found no rule for `automation_create`, `automation_delete`, `opencode_session_move`, `opencode_worktree_create` or `opencode_worktree_remove`. Agent rules only reference `edit` with path-scoped allow/deny entries, so none of the four new action denies would be overridden by an installed agent rule. The `worktree_remove` `edit` case remains different because path-scoped `edit` allows exist in several agents.

`documented` — Revised owner patch 3, `opencode.jsonc` (source-supported for v2.4.1, runtime-unverified, NOT applied; replaces the invalid bare-name patch):

```diff
--- a/opencode.jsonc
+++ b/opencode.jsonc
@@ -115,6 +115,13 @@
      { "action": "edit", "resource": "docs/ARCHITECTURE.md", "effect": "ask" },
      { "action": "edit", "resource": "opencode.jsonc", "effect": "ask" },
-     { "action": "edit", "resource": ".opencode/**", "effect": "ask" }
+     { "action": "edit", "resource": ".opencode/**", "effect": "ask" },
+
+     // RV-P1-C3: suppress mutating Code Mode tool identities in OpenCode 2.4.1.
+     // Source-derived action names use underscores; runtime behavior is not locally verified.
+     { "action": "automation_create", "resource": "*", "effect": "deny" },
+     { "action": "automation_delete", "resource": "*", "effect": "deny" },
+     { "action": "opencode_session_move", "resource": "*", "effect": "deny" },
+     { "action": "opencode_worktree_create", "resource": "*", "effect": "deny" }
    ],
```

`documented` — Owner patch 1, `.gitignore`, remains approved, unchanged and NOT applied (`/.owner-patch-backup/`, `/CMakePresets.json.backup`, `/debug-*.txt`); context still matches `.gitignore:13-15`. Owner patch 2, `ai/README.md`, remains prepared but untouched pending a separate owner release. `.env.example` is not affected.

`OWNER_DECISION` — `opencode.worktree_remove` cannot be tool-specifically denied in OpenCode 2.4.1: the tool deliberately uses the built-in `edit` action. A global `edit:* deny` would be broad and is not authorized. Per the owner’s instruction to stop an unreliable change for owner decision, this sub-item is stopped and documented; no broad edit deny was prepared or applied. Awaiting owner decision: accept the documented limitation, or explicitly authorize a broader edit deny with its side effects. **(Pre-decision snapshot — superseded by the owner decision recorded below; retained for history only.)**

`OWNER_DECISION` — Owner selected “Accept limitation” for `opencode.worktree_remove`. The four source-supported tool-identity denies stand as the revised owner patch; no broad `edit:* deny` is authorized or prepared, and the residual `worktree_remove` exposure is a documented limitation rather than a claimed fix. P1 remains blocked until the owner applies the `.gitignore` patch and the revised `opencode.jsonc` patch, after which a fresh independent reviewer verdict on the applied state is required. `ai/README.md` remains untouched pending its separate owner release. Package 2 remains prepared-only and not started.

## RV-P1-C3-C1 — bundled correction for review finding RV-P1-C3-REV-01 (in_progress / needs_re-review)

PACKAGE_ID: RV-P1-C3-C1
PACKAGE_TYPE: BUGFIX
PRODUCT_BOUNDARY: development-only
GOAL: Remove the stale current-state contradiction identified by the independent reviewer so the ledger and roadmap agree on the required owner actions.
FILES: `docs/ROADMAP.md`, `docs/AGENT_TASKS.md`.
ALLOWED: Coordinator edits limited to these permission-approved `docs/*.md` paths.
FORBIDDEN: No config, permission, agent, product, test, CMake or preset edit; no patch applied; no Package 2 execution or publication.
ACCEPTANCE: No current-state line says “only patch 1 and patch 2” or that patch 3 awaits syntax verification; the pre-decision `worktree_remove` snapshot is explicitly superseded; a fresh independent reviewer re-check passes.
TESTS: Documentation-only read-back plus exact Git status/HEAD/branch/diff checks.
REVIEW: Mandatory independent reviewer re-check after the bundled correction.
WORKTREE: None; main checkout.
OWNER_APPROVALS: None new; uses the owner decisions already recorded under RV-P1-C3.

`OWNER_DECISION` — Reviewer verdict `FAIL` (`session ses_df2403fb3ffeQJAOnU8s9587C7`) returned one finding: RV-P1-C3-REV-01 (HIGH) `ACTIONABLE`. Classified `ACTIONABLE`: stale current-state text at `docs/ROADMAP.md:41` and the unsuperseded pre-decision snapshot at `docs/AGENT_TASKS.md:771` could mislead the owner into applying the wrong patch set. Case B applies: one bundled correction package per the single writer (coordinator), grouped by the single root cause (stale current-state after RV-P1-C3 append), then a mandatory independent reviewer re-check. No `OWNER_DECISION`, `NON_BLOCKING` or `UNKNOWN` finding was added by the reviewer.

`observed` — Correction applied: `docs/ROADMAP.md:39,41` now state that syntax verification is complete, that the owner applies patch 1 (`.gitignore`) plus the revised patch 3 (`opencode.jsonc`), that `ai/README.md` stays untouched pending a separate release, that the Code-Mode denies are source-supported but runtime-unverified, that `opencode.worktree_remove` is an accepted limitation, that no broad `edit:* deny` is authorized, and that a fresh applied-state reviewer verdict is required. `docs/AGENT_TASKS.md:771` is marked as a pre-decision snapshot superseded by the decision at `:773`. No config, permission, product or publication file was changed. Re-review pending.

`documented` — Independent reviewer re-check (session `ses_df2403fb3ffeQJAOnU8s9587C7`, round 2) returned `PASS WITH FINDINGS` for `RV-P1-C3-C1`: 0 reviewer findings; prior `RV-P1-C3-REV-01` closed. Classification of this reviewer result: no `ACTIONABLE`, `OWNER_DECISION`, `NON_BLOCKING` or `UNKNOWN` reviewer finding. Case A applies: no correction required and none made, so no further review is started and the existing verdict is retained. The `PASS WITH FINDINGS` covers only the reviewed documentation correction state; it does not approve build/test results, implementation, Xbox compatibility, publication or permission changes.

`documented` — Open items recorded separately from defects under this verdict. `OWNER_DECISION`: (1) owner applies patch 1 (`.gitignore`); (2) owner applies revised patch 3 (`opencode.jsonc`); (3) `ai/README.md` stays untouched pending its separate owner release; (4) `opencode.worktree_remove` limitation accepted, residual exposure not a claimed fix. `UNKNOWN`: (1) runtime behavior of the four v2.4.1 tool-identity denies was not locally verified; (2) current `git diff --check` exit state is not exposed by the shell tool. `NON_BLOCKING`: none.

`documented` — `RV-P1-C3-C1` ist abgeschlossen. P1 ist lokal abgeschlossen, nachdem der Owner Patch 1 (`.gitignore`) und den überarbeiteten Patch 3 (`opencode.jsonc`) angewendet hat und der unabhängige Reviewer im angewendeten Zustand `PASS` erteilt hat (`ses_df1b60181ffek5mOBp1DHBgV5M`); die Publikation bleibt `OWNER_DECISION`. Die vorstehende Blockade ist historisch und wird durch den `XB-P2`-Eintrag unten superseded. Package 2 wurde vom Owner freigegeben und unter `XB-P2` gestartet; der Abschluss wird unten nach dem Review protokolliert.

## XB-P2 — Agentenarchitektur und effizienter Entwicklungsworkflow (pass / locally_complete)

PACKAGE_ID: XB-P2
PACKAGE_TYPE: FEATURE (documentation/architecture only)
PRODUCT_BOUNDARY: development-only
GOAL: Define a scalable, platform-neutral agent architecture and an efficient A–E workflow without runtime code.
FILES: `docs/AGENT_AUTONOMY.md`, `docs/AGENT_CORE_ARCHITECTURE.md`, `docs/OPENCODE_AUTOMATION_PLAN.md`, `docs/AUTOMATED_QUALITY_GATES.md`, `docs/GITHUB_AGENT_WORKFLOW.md`, `docs/DESKTOP_MIGRATION_CHECKLIST.md`; status records in `docs/AGENT_TASKS.md` and `docs/ROADMAP.md`.
ALLOWED: Coordinator as sole writer of the six docs; read-only architect; one bundled independent reviewer; documentation only.
FORBIDDEN: config/permission/agent-file/product/test/CMake/preset/AI README edits; publication; builds/tests for pure docs.
ACCEPTANCE: six complete, cross-consistent docs; A–E workflow binding; platform-neutral core separated from OpenCode adapter; security/owner boundaries; reviewer `PASS`/`NEEDS_CHANGE`/`BLOCKED` without actionable findings.
TESTS: read-back, exact Git status/HEAD/branch/diff checks; no artificial tests.
REVIEW: architect consistency check; one bundled independent reviewer; targeted re-check only for actionable findings.
WORKTREE: none; main checkout.
OWNER_APPROVALS: owner approved XB-P2 with this task; no new decision required for scoped status records.

`observed` — Pre-flight: HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; branch `docs/opencode-publication-group-2`; `git status --porcelain=v1 --untracked-files=all` showed the six new untracked docs plus the pre-existing working-tree entries; `git diff --check` emitted only the pre-existing `CMakePresets.json` CRLF warning. The coordinator created the six documents as sole writer and corrected localized artifacts during self-check. No build/CTest was run because this is a documentation-only package.

`documented` — Read-only architect session `ses_df1adc2afffeIpWUI0iYvcYFga` returned `ARCH NEEDS_CHANGE`: `ARCH-P2-01`, `ARCH-P2-02`, `ARCH-P2-03`, `ARCH-P2-05` classified `ACTIONABLE`; `ARCH-P2-04` classified `NON_BLOCKING`. One bundled correction was applied inside the six permitted docs: the applied-state reviewer session is now cited instead of an unrecorded verdict; the verdict vocabulary is aligned to `PASS`/`NEEDS_CHANGE`/`BLOCKED`; the coordinator write scope is corrected to twelve permitted docs; a logical-to-ledger state mapping was added; `opencode *` checks are marked owner-only. The ledger/roadmap alignment uses the coordinator's existing status-recording right (`docs/AGENT_WORKFLOW.md:173`), not a permission expansion. No config, permission, product, test, CMake, preset or `ai/README.md` file was edited. The final independent re-check is recorded below.

`documented` — Independent reviewer re-check (session `ses_df1a7c9d9ffejVBY4Gp0KjszlL`) returned `PASS`: 0 `ACTIONABLE`, 0 `OWNER_DECISION`, 0 `NON_BLOCKING` and 0 classified `UNKNOWN` findings; all five architect findings were closed, with no over-correction or new contradiction. Classification of this reviewer result: **Case A** — no correction required, none made, no second review. The verdict covers exactly the eight-file XB-P2 review scope; it does not approve build/test results, publication, runtime effectiveness of the four denies, JSONC parser validation, second-PC portability or Xbox compatibility. `XB-P2` is locally complete and awaiting the owner's publication decision.

## XB-0080-A/B — research register and workflow/ledger integration (implementation record)

PACKAGE_ID: XB-0080-A/B
PACKAGE_TYPE: FEATURE (development-only documentation)
PRODUCT_BOUNDARY: development-only
GOAL: Establish one authoritative, conformance-checked research register and connect it consistently to the agent workflow and ledger.
FILES: XB-0080-A = `docs/RESEARCH_REGISTER.md`; XB-0080-B = `docs/AGENT_WORKFLOW.md`, `docs/AGENT_TASKS.md`.
ALLOWED: researcher writes the register only; coordinator writes the workflow and ledger only; one bundled independent review of A+B; append-only status recording in this section.
FORBIDDEN: product code, tests, CMake, presets, build/CTest, CI, generator/tool, `opencode.jsonc`, `.opencode/**`, `AGENTS.md`, `ai/README.md`, `docs/ROADMAP.md`, `docs/DECISIONS.md`, `docs/ARCHITECTURE.md`, research files under `docs/research/**`, permission changes, staging, commit, publication, deletion or move.
ACCEPTANCE: AC-1 schema documented; AC-2 RESEARCH-007 register-level conformance; AC-3 no fake success; AC-4 boundary preserved; AC-5 independent review recorded. Existing AC definitions at the earlier "Checkable acceptance criteria" section retain priority; AC-2 is applied to all eight current register rows because the XB-0084 row postdates the original "seven rows" wording.
TESTS: documentation-only read-back, exact Git status/HEAD/branch/diff checks and `git diff --check`; no artificial tests and no build/CTest.
REVIEW: read-only architect model check before writing; one bundled independent reviewer after A+B; targeted bundled correction only for `ACTIONABLE` findings; reviewer does not edit or launch subagents.
WORKTREE: none; current checkout.
OWNER_APPROVALS: start release, A→B order, split write set and AC framework were supplied with the owner's current order. No new owner decision was created: the one architect-marked `OWNER_DECISION` is resolved by the in-scope option that preserves the six authoritative ledger `Status` tokens and adds fine-grained states as metadata. Publication remains owner-only.

`documented` — Effective write rights were re-verified before release: `researcher.md:8-10` allows `docs/RESEARCH_REGISTER.md`; `researcher.md:11-13` also allows `docs/research/**`, but that path is explicitly excluded by this package's `FORBIDDEN` list and was not edited. `coordinator.md:8-13` allows `docs/AGENT_TASKS.md` and `docs/AGENT_WORKFLOW.md`. Write sets are disjoint and ordered, so no parallel-write conflict exists. The reviewer is read-only. No effective right was expanded or bypassed.

`documented` — Architect model check (`ses_df19301d3ffetTvX1VUvYbMuLZ`) returned `ARCH NEEDS_CHANGE`. Classification by the coordinator: `ARCH-XB0080-01` was marked `OWNER_DECISION` by the architect but is resolved in-scope by keeping `AGENT_TASKS.md:5` and `AUTOMATED_QUALITY_GATES.md:84` authoritative and recording `IMPLEMENTATION_STATE` separately, per `AGENT_CORE_ARCHITECTURE.md:41-54`; the alternative of expanding the authoritative vocabulary is not chosen and would be a new owner decision. `ARCH-XB0080-02` through `ARCH-XB0080-08` are `ACTIONABLE` and were folded into the model before writing: historical XB-0085 publication-audit labels are not mapped; conformance is objective; source/reproduction and `not_applicable` rules are status-dependent and bounded; `documented` requires an authoritative/project-authored qualifier and the former `confidence` field is explicitly replaced; XB-0082 readiness has a fixed key order and token set; the coordinator-rights citation uses `coordinator.md:8-13`. `ARCH-XB0080-09` and `ARCH-XB0080-10` are `NON_BLOCKING`: the register says "pinned commit SHAs" and this section uses section anchors instead of the stale `AGENT_WORKFLOW.md` line references. No architect finding remains open.

`observed` — XB-0080-A handoff (`ses_df1904039ffebaGY8PN7kBewHZ`): sole writer changed `docs/RESEARCH_REGISTER.md` only; no web, shell, build, staging or publication. Read-back by the researcher reported 8 index rows, 8 detail blocks, 184 required-field lines (8 × 23), `Source` and `Reproduction` present in all 8, conformance distribution 8 `conformant` / 0 other, capability distribution 8 `unknown` / 0 `not investigated` / 0 `investigated-unsupported` / 0 `implemented-unvalidated` / 0 `validated`, and all three RESEARCH-007 pinned commit SHAs plus `2026-11-22` present. The coordinator independently read the file and confirmed stable anchors `## 2. Two independent axes`, `## 3. Required fields for every entry`, `## 4. Status-dependent source and reproduction requirements`, `## 5. Explicit mapping of legacy statuses`, `## 6. Conformance model`, `## 7. Compact index`, `## 8. Entry details` and `## 9. Assessment and audit notes`. A→B interface check **passed**: B consumes these section names, not line numbers, and needs no register change.

`observed` — XB-0080-B applied changes: `AGENT_WORKFLOW.md` gained the "Research register and evidence lifecycle" section with entry creation, conformance verification, status axes, the XB-P2 logical-to-ledger mapping, historical XB-0085 label disposition, and the evaluation-only machine-readable readiness convention; this ledger gained this implementation section. No generator, tool, CI job, dependency, percentage display or XB-0082 implementation was added.

```yaml
PACKAGE_ID: XB-0080-A
PARENT_ID: XB-0080
DEPENDENCIES: N/A
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: conformant
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

```yaml
PACKAGE_ID: XB-0080-B
PARENT_ID: XB-0080
DEPENDENCIES: XB-0080-A
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: conformant
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — Pre-review acceptance evidence: AC-1 is met by register §2.2 (five capability tokens with one use rule each), §3 (23 required fields) and §4 (mandatory Source/Reproduction with status-dependent explicit N/A reasons); AC-2 is met by §5 legacy mapping, §7/§8 full-row assessment and §9 provenance note, with RESEARCH-007 date, three pinned commit SHAs, URLs and reproduction pointer preserved and REC-01…REC-20 untouched in the recommendations file; AC-3 is met because §9 reports zero `validated` and zero `implemented-unvalidated` rows and class-4/class-5 items remain `unknown`; AC-4 is subject to the exact diff check recorded below; AC-5 remains pending until the independent reviewer verdict is appended.

`unknown` — The historical ledger table at `:9-20` still lists XB-0080 as `planned`, and `docs/ROADMAP.md` item 11 still says `planned / blocked by open owner decisions`. Both are historical anchors; editing the roadmap and the early table is outside this package's write set. The authoritative current state is this section. External web truth, license/legal effect and runtime behavior of any configuration remain `unknown`; no build or CTest was run for this documentation-only package.

`observed` — Exact verification commands and actual results run by the coordinator after both writes:
- `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`.
- `git branch --show-current` → `docs/opencode-publication-group-2`.
- `git status --porcelain=v1 --untracked-files=all` → 24 entries (17 modified, 7 untracked). Compared with the 22-entry pre-package state, no new untracked file was created by XB-0080-A/B; only the two previously clean tracked files `docs/RESEARCH_REGISTER.md` and `docs/AGENT_WORKFLOW.md` became modified in addition to the already-modified `docs/AGENT_TASKS.md`.
- `git diff --check; "EXIT=$LASTEXITCODE"` → only `warning: in the working copy of 'CMakePresets.json', CRLF will be replaced by LF the next time Git touches it`; `EXIT=0`.
- `git diff --stat -- docs/RESEARCH_REGISTER.md docs/AGENT_WORKFLOW.md docs/AGENT_TASKS.md` → `Permission denied: shell`. This exact form is not in the global exact-command allowlist at `opencode.jsonc:79-84`; it is recorded as a rejected attempt, not as a passed check.
- `git diff -- docs/RESEARCH_REGISTER.md docs/AGENT_WORKFLOW.md docs/AGENT_TASKS.md` → `Permission denied: shell`. Same reason; no result was inferred from it.
- `git diff HEAD --no-ext-diff --name-only` → executed successfully under the effective permission model; output listed 17 modified tracked files, including exactly `docs/RESEARCH_REGISTER.md`, `docs/AGENT_WORKFLOW.md` and `docs/AGENT_TASKS.md` from this package, plus 14 pre-existing modifications. Warning about `CMakePresets.json` CRLF was repeated.
- `git diff HEAD --no-ext-diff --stat` → executed successfully under the effective permission model; 17 tracked files, `942 insertions(+), 47 deletions(-)`. Package-relevant lines: `docs/AGENT_TASKS.md | 278 ++++++++++++++++++++++++++++++++++++`, `docs/AGENT_WORKFLOW.md | 47 +++++++`, `docs/RESEARCH_REGISTER.md | 353 +++++++++++++++++++++++++++++++++++++---`.
- `observed` — Command provenance: these two `git diff HEAD --no-ext-diff ...` forms are permitted by the coordinator agent rule at `coordinator.md:102-109`; they are not listed in the global exact-command allowlist at `opencode.jsonc:79-84`, which falls back to `shell "*" = ask` at `opencode.jsonc:76`. They are therefore recorded as successfully executed under the effective configuration, not as globally allowlisted and not as a saved-approval claim. This replaces the earlier inaccurate "allowlisted alternative" wording without changing any recorded output.
- `git diff --cached --name-only` → no output; the index is empty and nothing was staged.
- Full read-back of `docs/RESEARCH_REGISTER.md` (335 lines), `docs/AGENT_WORKFLOW.md` (new section at `## Research register and evidence lifecycle`) and this ledger section confirmed the applied text, fixed YAML key order and allowed tokens. No build or CTest command was run or claimed.

### XB-0080-C1 — bundled correction package (review round 1)

PACKAGE_ID: XB-0080-C1
PACKAGE_TYPE: BUGFIX (documentation accuracy only)
PRODUCT_BOUNDARY: development-only
GOAL: Correct the two evidence-accuracy defects found by the bundled reviewer without changing schema, status model or acceptance substance.
FILES: `docs/AGENT_TASKS.md` only; coordinator is sole writer.
ALLOWED: replace the inaccurate permission citation and command-provenance wording in the XB-0080-A/B implementation section; add this correction record.
FORBIDDEN: register or workflow content changes, product/test/CMake/preset/config/agent/permission edits, publication, build/CTest.
ACCEPTANCE: reviewer re-check confirms R-01 closed and R-02 wording accurate; no new finding.
TESTS: read-back of the two corrected passages; no build/CTest for documentation.
REVIEW: mandatory independent re-check by the same bundled reviewer after correction.
WORKTREE: none; current checkout.
OWNER_APPROVALS: none; mechanical correction inside the already approved package.

`documented` — Reviewer verdict (`ses_df18cb63dffe3BLVz4tT98Gy9z`) was `NEEDS_CHANGE` with R-01 MEDIUM `ACTIONABLE` and R-02 LOW `NON_BLOCKING`; 0 `OWNER_DECISION`, 0 classified `UNKNOWN`. Classification by the coordinator: R-01 `ACTIONABLE`; R-02 `NON_BLOCKING` but fixed in the same bundle because it is the same evidence-accuracy root cause in adjacent ledger text. Grouping produced one correction package for one writer. No severity was downgraded and no finding was left silently unresolved.

`observed` — Corrections applied: the write-rights sentence now cites `researcher.md:8-10` for the register allow and explicitly states that `researcher.md:11-13` additionally permits `docs/research/**` but is excluded by this package's `FORBIDDEN` list; the rejected `git diff` forms are described as not globally allowlisted; the two successful `git diff HEAD --no-ext-diff ...` forms are described as executed under the effective configuration via `coordinator.md:102-109`, not as globally allowlisted and not as a saved-approval claim. No command output, AC result, schema rule, status token or boundary statement changed. Re-check pending.

### XB-0080-A/B final result — locally complete, publication owner-only

`documented` — Review sequence completed: architect model check `ARCH NEEDS_CHANGE` → all findings folded in before writing → researcher wrote A → coordinator A→B interface check passed → coordinator wrote B → one bundled reviewer round 1 `NEEDS_CHANGE` (`ses_df18cb63dffe3BLVz4tT98Gy9z`: R-01 `ACTIONABLE`, R-02 `NON_BLOCKING`, 0 `OWNER_DECISION`, 0 classified `UNKNOWN`) → one bundled correction `XB-0080-C1` for the single writer → targeted independent re-check `PASS` with zero findings in the same session. Classification of the re-check: R-01 and R-02 closed; no new `ACTIONABLE`, `OWNER_DECISION`, `NON_BLOCKING` or `UNKNOWN` finding; Case A applies to the re-check and no second review was started. The two-round limit was respected.

`documented` — Final acceptance: AC-1, AC-2, AC-3, AC-4 and AC-5 are met. AC-5 is met by the independent reviewer verdict recorded above, not by coordinator self-acceptance. Both subpackages are `STATUS: pass`, `IMPLEMENTATION_STATE: done`, `REVIEW_STATUS: PASS`, `CONFORMANCE_STATUS: conformant` and `PUBLICATION_STATUS: local_only` in the fixed-key blocks above. The verdict covers only the three scoped files and the documented evidence model; it does not approve product changes, build/CTest results, runtime behavior, external web truth, license/legal effect, roadmap edits or publication.

`OWNER_DECISION` — Publication of this working-tree state, including any commit, stage, push, merge, pull request, tag or release, remains exclusively with the project owner. No agent performed any such action. The stale historical XB-0080 row at `:9-20` and roadmap item 11 remain unchanged because those paths are outside this package's write set; a separate owner-approved documentation package would be needed to update them.

`observed` — Final post-correction verification: `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git status --porcelain=v1 --untracked-files=all` → the same 24 entries (17 modified, 7 untracked), with no new untracked file from this package; `git diff --check; "EXIT=$LASTEXITCODE"` → only the pre-existing `CMakePresets.json` CRLF warning, `EXIT=0`; `git diff HEAD --no-ext-diff --stat` → 17 tracked files, `982 insertions(+), 47 deletions(-)`, with package lines `docs/AGENT_TASKS.md | 318 +++++`, `docs/AGENT_WORKFLOW.md | 47 +++++++`, `docs/RESEARCH_REGISTER.md | 353 +++++...---`; `git diff --cached --name-only` → no output. No build or CTest was run or claimed.

## XB-0085-CURRENT — refreshed read-only publication audit (XB-CONTINUATION-01)

PACKAGE_ID: XB-0085-CURRENT
PACKAGE_TYPE: RESEARCH (read-only publication preparation)
PRODUCT_BOUNDARY: development-only
GOAL: Re-evaluate the publication groups against the current HEAD instead of reusing the historical 32-entry snapshot.
FILES: no file is the object of a publication action; this record is appended to `docs/AGENT_TASKS.md` by the coordinator.
ALLOWED: read-only Git inspection and documentation read-back; append-only status recording in this ledger.
FORBIDDEN: staging, commit, push, pull, PR, merge, tag, release, branch/worktree change, history change, deletion, move, permission or config edits, build/CTest, and any use of historical counts as fresh verification.
ACCEPTANCE: every current working-tree entry is assigned to one of the three existing publication-audit labels; the basis is current Git output; remaining owner decisions are collected without being resolved by an agent.
TESTS: exact Git read commands plus read-back; no build or CTest for a read-only audit.
REVIEW: bundled independent review of the continuation work, not a self-accepted publication approval.
WORKTREE: none; current checkout.
OWNER_APPROVALS: all publication actions remain owner-only; no new permission or scope was created.

`observed` — Fresh commands and actual results: `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git status --porcelain=v1 --untracked-files=all` → 25 entries (18 modified, 7 untracked) at the C1 refresh; the original audit snapshot immediately before the tester edit was 24 entries (17 modified, 7 untracked); `git diff --cached --name-only` → no output; `git diff --check; "EXIT=$LASTEXITCODE"` → only the `CMakePresets.json` CRLF warning, `EXIT=0`; `git diff HEAD --no-ext-diff --stat` → 17 tracked files, `982 insertions(+), 47 deletions(-)`; `git log -5 --oneline` → `58aa1f6`, `e8ba492`, `cb04e03`, `7a9753f`, `af625df`. `git diff HEAD --no-ext-diff` was executed; `git diff HEAD --no-ext-diff --numstat` was rejected with `Permission denied: shell` and is recorded as a denied attempt, not bypassed. PR state and divergence from `main` remain `unknown` because agents do not run PR commands.

`observed` — Historical XB-0085 (`docs/AGENT_TASKS.md:546-552`, `:543`, `docs/ROADMAP.md:31`, `docs/PORTABLE_SETUP.md:109`) recorded 32 entries (13 `READY_FOR_COMMIT`, 18 `NEEDS_REVIEW`, 1 `DO_NOT_PUBLISH`) on branch `main` with HEAD `10e5e75`. That is a documented historical snapshot, not a current verification. At the C1 refresh the current tree has 25 entries (18 modified, 7 untracked, including the newly added `tests/test_main.cpp`); the historical per-entry list is not recoverable from the ledger, so the counts below are a fresh assignment to the 25 current entries and do not reuse or transform the historical numbers.

`documented` — Label meanings are preserved from XB-0085 and are publication-audit labels only, not ledger `Status` or `PUBLICATION_STATUS` tokens (`docs/AGENT_WORKFLOW.md:155`): `READY_FOR_COMMIT` = locally complete with an independent PASS for the substantive file state and no file-specific open owner blocker; `NEEDS_REVIEW` = changed but without an independent PASS for the exact current state or only a historical/partial PASS; `DO_NOT_PUBLISH` = private development infrastructure or owner-territory file that agents must not publish and that needs a separate owner decision. Every label still requires the owner's publication decision.

| Current entry | Label | Basis |
|---|---|---|
| `.gitignore` | READY_FOR_COMMIT | Owner patch 1 applied (`:16-19`); reviewer `PASS` in the applied state (`ses_df1b60181ffek5mOBp1DHBgV5M`, recorded at `:798`). |
| `docs/AGENT_WORKFLOW.md` | READY_FOR_COMMIT | XB-P2 eight-file `PASS` (`:819`); XB-0080-A/B `PASS` (`:908`); no edit after those reviews. |
| `docs/RESEARCH_REGISTER.md` | READY_FOR_COMMIT | XB-0080-A/B bundled `PASS` (`:908`); no edit after review. |
| `docs/ROADMAP.md` | READY_FOR_COMMIT | XB-0086-C2 `PASS` (`:594`) and XB-P2 status scope (`:819`); the formerly stale item 11 and sequence step (8) were corrected in C1 (`:35`, `:43`), so no stale XB-0080 anchor remains. The exact post-C1 state is in the bundled review scope. |
| `docs/PORTABLE_SETUP.md` | READY_FOR_COMMIT | XB-0086-C1 fifth-file final `PASS` (`:574`); no later edit by this package. |
| `docs/DEVELOPMENT_ENVIRONMENT.md` | READY_FOR_COMMIT | RV-P1-C2 final `PASS` for the correction state (`:714`); no later edit. |
| `docs/AGENT_AUTONOMY.md`, `docs/AGENT_CORE_ARCHITECTURE.md`, `docs/OPENCODE_AUTOMATION_PLAN.md`, `docs/AUTOMATED_QUALITY_GATES.md`, `docs/GITHUB_AGENT_WORKFLOW.md`, `docs/DESKTOP_MIGRATION_CHECKLIST.md` | READY_FOR_COMMIT | Six XB-P2 files in the eight-file independent `PASS` (`:806-819`). |
| `docs/AGENT_TASKS.md` | NEEDS_REVIEW | Append-only final-result and verification records at `:906-914` postdate the XB-0080-A/B re-check; no new substantive rule, but the exact current file state is not yet covered by a reviewer verdict. Included in the continuation bundled review. |
| `docs/ARCHITECTURE.md` | NEEDS_REVIEW | Modified in the earlier documentation cleanup (`:137`, `:681`); no independent `PASS` for the exact current file state was located. |
| `tests/test_main.cpp` | READY_FOR_COMMIT | Post-CTest refresh: after the owner applied the absolute `ctest.exe` allow, the real VS2026 Debug configure/build/CTest chain returned EXIT=0 (1/1 `xbridge_tests` passed, with the five new cases in that binary); the bundled final reviewer returned `PASS`/Case A for this exact evidence state. The earlier C1 `NEEDS_REVIEW` was based on the failed stale-cache run and is superseded. Publication remains owner-only. |
| `.opencode/agents/{architect,coordinator,developer,researcher,reviewer,tester}.md` | DO_NOT_PUBLISH | Private agent configuration; agents have no edit right there (`coordinator.md:53-55`) and no publication right at all (`docs/GITHUB_AGENT_WORKFLOW.md:9`). |
| `AGENTS.md` | DO_NOT_PUBLISH | Permanent versioning decision explicitly left open by XB-0086-C2 (Abschnitt `XB-0086-C2 — final status alignment and publication group 0 definition` in `docs/AGENT_TASKS.md`; die frühere Zeilenangabe `:588` ist veraltet). |
| `CMakePresets.json` | DO_NOT_PUBLISH | Machine-local VS2026 preset change with a CRLF warning and an excluded backup; not a product-package file, not re-verified by a fresh build in this audit. |
| `ai/README.md` | DO_NOT_PUBLISH | Private owner-territory documentation. The formerly open `OWNER_DECISION` RV-P1-08 (`:716`) is resolved: the three approved wordings at `:3,:13,:28` were applied and recorded at `:2411-2418` (read-back in this pass confirms them). It still stays unpublished because it is private development infrastructure, not because a patch is pending. |
| `opencode.jsonc` | DO_NOT_PUBLISH | Owner-territory permissions/configuration. The revised patch 3 was applied by the owner and independently reviewer-`PASS`ed (`:794`, `docs/DEVELOPMENT_ENVIRONMENT.md:164`); the older withdrawal note (`:677,:689-691`) is historical and superseded. It stays unpublished because it is private development infrastructure, not because a patch was withdrawn. |

`observed` — Counts at the C1 refresh: 12 `READY_FOR_COMMIT`, 3 `NEEDS_REVIEW`, 10 `DO_NOT_PUBLISH` = 25 current entries. The 24-entry snapshot at the start of this continuation was 12/2/10; the added `tests/test_main.cpp` moved to `NEEDS_REVIEW`. This is a fresh classification, not a correction of the historical 13/18/1 figures. `observed` — Post-CTest refresh after the owner-applied absolute CTest rule and the bundled reviewer `PASS`: 13 `READY_FOR_COMMIT`, 2 `NEEDS_REVIEW` (`docs/AGENT_TASKS.md`, `docs/ARCHITECTURE.md`), 10 `DO_NOT_PUBLISH` = the same 25 entries; only the `tests/test_main.cpp` basis changed from failed-verification to executed-PASS. No file was added or removed by this refresh.

`observed` — `XB-CHANGE-COMPLETION-FINAL-01` localized correction: the two stale basis statements at `:948` (claimed `ai/README.md` patch 2 unapplied) and `:949` (claimed `opencode.jsonc` patch 3 withdrawn) were corrected in place to match the applied, reviewer-`PASS`ed state recorded at `:794` and `:2411-2418`. Read-back confirmed `ai/README.md:3,13,28` and `docs/ARCHITECTURE.md:234` carry the approved wording; `docs/DEVELOPMENT_ENVIRONMENT.md:164` marks the old withdrawal note as historical. The `DO_NOT_PUBLISH` labels themselves are unchanged: both files remain private owner-territory development infrastructure regardless of patch state. No other file was changed by this correction; the publication-audit counts at `:951` are historical snapshots and were not recomputed.

`documented` — Recommended publication order, without authorizing any publication action: (1) the 13 `READY_FOR_COMMIT` files as the first group (12 at the C1 refresh plus `tests/test_main.cpp` after the executed CTest PASS), preserving the five-file `publication group 0` definition in the section `XB-0086-C2 — final status alignment and publication group 0 definition` as a subset; (2) `docs/AGENT_TASKS.md` after the continuation bundled review covers `:906-914`; (3) `docs/ARCHITECTURE.md` after a targeted review of its current diff; (4) the `DO_NOT_PUBLISH` set never automatically — each remains owner-territory or owner-decision territory. Dependencies: the ledger must be reviewed before publication because it records the verdicts for the other files; the six XB-P2 documents are self-contained; the private configuration files do not belong to any product package.

`OWNER_DECISION` — Collected decisions: (a) whether to publish the 12-file ready group; (b) `ai/README.md` patch 2 was applied and independently reviewer-`PASS`ed (`:2411-2418`); the remaining decision is only whether to publish this private owner-territory file at all (recommended: keep it unpublished); (c) the permanent versioning decision for `AGENTS.md`; (d) treatment of the local `CMakePresets.json` change and its excluded backup; (e) current PR #1 state, branch strategy and merge into `main`, all `unknown` to agents; (f) worktree cleanup of the unregistered checkout; (g) formerly stale `docs/ROADMAP.md` item 11 and historical XB-0080 table row — C1 corrected the roadmap status/sequence text (`:35`, `:43`); whether the historical ledger table row itself needs a separate owner patch remains an owner call. Recommended default: publish only owner-selected development-infrastructure files, keep configuration and machine-local files out, and perform no merge or PR action until the owner explicitly decides.

`unknown` — Current PR state, 29-commit divergence, license/legal effect of cited research, second-PC portability, interactive TUI dialogs and runtime behavior of configuration denies were not re-verified. No historical result is presented as current verification.

## XB-X64-CORPUS-01 — documented-contract test hardening (XB-CONTINUATION-01)

PACKAGE_ID: XB-X64-CORPUS-01
PACKAGE_TYPE: FEATURE (test-only conformance hardening)
PRODUCT_BOUNDARY: product (test file only; no runtime behavior change)
GOAL: Cover documented x64 decoder/interpreter boundary and failure contracts with the installed tester, without changing product behavior or public interfaces.
FILES: `tests/test_main.cpp` only. Sole writer: installed `tester`; `developer` is explicitly not a writer for this file.
ALLOWED: add tests using the existing `check` harness; preserve every existing test; assert only contracts documented in `docs/ARCHITECTURE.md:115-129,153-163`.
FORBIDDEN: product source/header edits, `CMakeLists.txt`, `CMakePresets.json`, config/agent/docs edits, deletion or weakening of tests, new dependencies, JIT, new opcode families, FLAGS/SIMD/SIB/displacement semantics, and any assertion about `length` on `InvalidEncoding` (undocumented incidental behavior).
ACCEPTANCE: new cases cover REX.W/R/B extended-register selection (R8–R15), REX.X recognized-but-unused, truncated input with REX and missing ModR/M, and every non-`Ok` decode status as a no-op with unchanged RIP/registers/memory and no exception; all pre-existing tests remain; exact build/CTest/diff commands are executed with real results reported.
TESTS: `cmake --preset windows-debug`; `cmake --build --preset windows-debug --config Debug`; `ctest --test-dir build/windows-debug -C Debug --output-on-failure`; `git diff --check`. The single existing CTest target means the full binary runs; no targeted `ctest -R` is claimed.
REVIEW: architect skipped for implementation (no interface, module boundary, dependency or ADR change; read-only technical map already returned `ARCH PASS` bounded); one bundled independent reviewer covers XB-0085-CURRENT, the roadmap correction and this test package.
WORKTREE: none; current checkout.
OWNER_APPROVALS: the owner released this tests-only package immediately in `XB-CONTINUATION-01`; normal envelope approval is therefore recorded. Publication remains owner-only.

`documented` — Authority: `AGENTS.md:84-86` requires boundary/failure tests; `RESEARCH-007_RECOMMENDATIONS.md:24,37,81` assigns installed `tester` for untrusted/truncated input and boundary contracts with no product change. Effective right verified: `tester.md:5-10` allows `tests/**`, and this envelope explicitly names `tester` as the sole writer of `tests/test_main.cpp` as required by `tester.md:81-82`. `developer.md` grants `tests/**` too but is not assigned here, so no parallel write conflict exists.

`documented` — Pre-release findings consolidated: inventory (`ses_df17930f6ffdZyLRjGeHjmrYt8`), architecture (`ses_df17930f6ffeoP4pHBZMRQh0R1`, `ARCH PASS` bounded), feasibility (`ses_df17930f4ffew0lbi0f6doF4Or`) and verification gaps (`ses_df17930f2ffezcfgWyGYPRodh4`). All agree that product-code, Agent Core, GTA VI, RIP-bridge and host-provider implementations require separate owner decisions; only this test-only package was executable now.

`observed` — Writer handoff and exact verification results (`ses_df171e7eeffe4js8Xt34Buic0I`): `tests/test_main.cpp` was the only changed file, additions only — `test_x64_decoder_rex_extended_registers` (`:1216-1307`), `test_x64_interpreter_rex_extended_registers` (`:1309-1388`), `test_x64_decoder_rex_x_recognized_unused` (`:1390-1437`), `test_x64_decoder_truncated_rex_modrm` (`:1439-1455`), `test_x64_interpreter_non_ok_decode_noop` (`:1457-1513`), registrations (`:1622-1626`). `cmake --preset windows-debug` → **exit 1** (`CMakeCache.txt` directory `D:/XBridge 1.0/...` differs from `e:/XBridge 1.0/...` where it was created; source-path mismatch). `cmake --build --preset windows-debug --config Debug` → **exit 1** (same cache error; generator `Visual Studio 18 2026` could not locate `E:/PRO/Programm Daten/Visual Studio/18/Community`). `ctest --test-dir build/windows-debug -C Debug --output-on-failure` → **exit 1** (`Unable to find executable: E:/XBridge 1.0/.../xbridge_tests.exe`; `1/1 Test #1: xbridge_tests ... ***Not Run`; `0% tests passed`). `git diff --check` → only the pre-existing `CMakePresets.json` CRLF warning, no error line; exit code **unknown** because the exit-code-capture variant was denied. `git status --porcelain=v1 --untracked-files=all` → 25 entries (18 modified including `M tests/test_main.cpp`, 7 untracked); `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git diff --cached --name-only` → no output. Denied, not bypassed: `git diff -- tests/test_main.cpp`, `git diff HEAD --no-ext-diff`, `cmake -S . -B build/windows-debug-verify-d -G "Visual Studio 18 2026" -A x64`, exit-code-capture variant. The complete diff was inspected with the allowed `read` tool (`tests/test_main.cpp:1200-1639`): exactly two insertion blocks, no other change.

`observed` — Root cause of the verification failure, confirmed by read-back in C1: `build/windows-debug/CMakeCache.txt:62,198,204,214,263` still contains `E:/XBridge 1.0/XBridge-Series-Github/...` paths from the previous checkout; `CMakePresets.json:5-9` uses `${sourceDir}` with generator `Visual Studio 18 2026`. The tester permission file (`tester.md:20-67`) allowlists only exact command strings and does not permit a fresh configure or an alternate diff form, so the stale cache cannot be repaired by the assigned writer. No build or CTest pass is claimed; compile and runtime results of the new tests are `unknown`.

`observed` — Bundled round-1 reviewer verdict (`ses_df16e3a75ffeJg0HIeGf1y64xC`): `NEEDS_CHANGE`, 3 findings — `XB-CONT-01-R01` HIGH/`ACTIONABLE` (failed verification not recorded, package without status axes), `XB-CONT-01-R02` MEDIUM/`ACTIONABLE` (publication audit omitted the post-tester 25th entry), `XB-CONT-01-R03` LOW/`NON_BLOCKING` (compound envelope tokens). The reviewer confirmed the tests are faithful to documented contracts, non-weakening and confined to the sole tester writer, but never compiled or executed; runtime success is `unknown`. All three findings were grouped into this single bundled correction `XB-CONTINUATION-01-C1`; an independent re-check is mandatory.

```yaml
PACKAGE_ID: XB-X64-CORPUS-01
PARENT_ID: XB-CONTINUATION-01
DEPENDENCIES: N/A
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — Status change basis: the owner applied the exact absolute `ctest.exe` allow at `.opencode/agents/tester.md:71-73` and restarted OpenCode. The tester then ran `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"` → `Start 1: xbridge_tests`, `1/1 Test #1: xbridge_tests ... Passed 0.05 sec`, `100% tests passed, 0 tests failed out of 1`, `Total Test time (real) = 0.12 sec`, `EXIT=0`. Found/executed/passed/failed/Not Run = 1/1/1/0/0. Together with the earlier absolute CMake `--version`, `--fresh --preset windows-debug` and `--build --preset windows-debug --config Debug` results (all `EXIT=0`) and the harness read-back (`tests/test_main.cpp:21-30,1622-1626,1749-1754`), this satisfies the ACCEPTANCE at `:972-973`. Bundled independent reviewer `ses_df1120fd9ffeXNrOLLH95PvD6T` returned **PASS, Case A** (0 `ACTIONABLE`, 0 `OWNER_DECISION`, 0 `NON_BLOCKING`, 0 `UNKNOWN`); no correction round was required. `documented` — `STATUS: pass` is local verification only; `PUBLICATION_STATUS: local_only` and owner-only publication are unchanged. The earlier `needs_owner` entries above are historical snapshots taken when configure/build/CTest failed with zero executed tests.

## XB-CONTINUATION-01 — consolidated open-package overview and dependency plan

PACKAGE_ID: XB-CONTINUATION-01
PACKAGE_TYPE: RESEARCH
PRODUCT_BOUNDARY: development-only
GOAL: Refresh publication state, remove the stale roadmap contradiction, and inventory/prepare the only executable technical package without inventing owner approvals.
FILES: `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`, `tests/test_main.cpp` under separate explicit envelopes.
ALLOWED: append-only status records; one coordinator-owned roadmap correction; one tester-owned test file.
FORBIDDEN: product runtime code, CMake/preset changes, config/agent/permission edits, publication, build claims without execution, new dependencies, JIT, Xbox compatibility claims.
ACCEPTANCE: every open package classified; dependencies and priorities recorded; XB-0085 refreshed; stale roadmap anchor corrected; tests-only package executed with real commands; one bundled independent review; all owner decisions collected once.
TESTS: see XB-X64-CORPUS-01 envelope; documentation-only checks for the ledger/roadmap.
REVIEW: one bundled independent reviewer over all continuation artifacts after the writer reports a stable state.
WORKTREE: none; current checkout.
OWNER_APPROVALS: continuation order received; immediate release of the tests-only package received; all other decisions remain open and are collected below.

`documented` — C1 scope note (bundled correction, reviewer finding `XB-CONT-01-R03`): this coordination envelope uses single allowed tokens `RESEARCH`/`development-only`; the test-only implementation is tracked separately under `XB-X64-CORPUS-01` with its own `product` boundary. The mixed-scope explanation is prose only, so read-back token validation succeeds (`docs/AGENT_WORKFLOW.md:21-22`).

`documented` — Dependency plan (1 = blocks other work, 2 = architecture/interface foundation, 3 = approved technical foundation, 4 = automation/QA, 5 = functional implementation, 6 = integration, 7 = documentation/publication):
1. Owner publication decision and branch strategy block every Git/GitHub action but block no local documentation or test work.
2. `XB-0084` owner decision register and ADR/license gates block all GTA VI implementation, host-provider expansion and any public-interface change.
3. `SETUP-007` blocks specialist installation; installed `developer`/`tester` substitutes require an approved envelope per package; this does not block the current tests-only package.
4. `XB-0082` blocks only automated progress tooling, not the existing document convention.
5. `XB-X64-CORPUS-01` is executable now and strengthens the documented interpreter baseline; it does not unlock product code by itself.
6. `XB-X64-STEP-01` (new x64 single-step façade) and `XB-HOST-CAP-01` (capability-provider seam) are fully prepared but owner-gated because they add public interfaces.
7. XB-0085-CURRENT, roadmap correction and final reporting proceed in parallel with (5) and do not depend on owner decisions.

`observed` — Open-package inventory (authoritative detail remains in the cited records):
| ID | Current state | Dependencies / gate | Next concrete step | Evidence |
|---|---|---|---|---|
| `XB-0082` | `planned`, evaluation-only convention implemented in `docs/AGENT_WORKFLOW.md:157-174` | Owner decision on split write set, taxonomy and any tool/CI/dependency | Owner decides; then approved package | `docs/AGENT_TASKS.md:241-252,310,338,441,461` |
| `XB-0083` | `planned`, not started | Start package; fresh web truth `unknown` | Approve researcher package with exact URLs/access dates | `docs/AGENT_TASKS.md:254-265,311,337` |
| `R-001`…`R-006` | Register rows `unknown`, conformant, no package assigned | Researcher package per row; no product change | Create envelopes if the owner wants evidence | `docs/RESEARCH_REGISTER.md:111-116,122-276` |
| `RESEARCH-007` / `XB-0084` carry-over | Research/planning `pass`; capability `unknown` | Owner/legal/ADR decisions before adoption | Resolve decision registers | `docs/RESEARCH_REGISTER.md:278-328`; `XB-0084_RISK_DEPENDENCY_OVERVIEW.md:108-129` |
| GTA VI implementation | Not approved | Separate owner approval after XB-0084 decisions | Prepare only; no code | `docs/ROADMAP.md:37` |
| `XB-X64-STEP-01` | Prepared, not started | Owner approval of new public interface and installed writer set | Owner decides or chooses private-helper alternative | architect result `ses_df17930f6ffeoP4pHBZMRQh0R1` §3.3 |
| `XB-HOST-CAP-01` | Prepared, not started | Owner approval of provider interface and Windows-probing scope | Owner decides | architect result §3.4 |
| `SETUP-004` | `needs_owner`; owner-only session export/import | Owner executes supported commands and reports | No agent action | `docs/AGENT_TASKS.md:15,142-153` |
| `SETUP-006` | `blocked`; failed worktree opening, unregistered checkout | Owner cleanup/further-attempt decision | No agent action | `docs/AGENT_TASKS.md:17,80-82,101` |
| `SETUP-007` | `blocked`; nine specialists not installed | Owner persists scoped rules and installs agents | No agent action | `docs/AGENT_TASKS.md:18,103-106,272-288` |
| `ai/README.md` patch | Applied and verified under `XB-OWNER-DOC-IMPLEMENTATION-02` (`PERMISSION_STATE: verified`, `REVIEW_STATUS: PASS`) | none (historical F1 framing; authoritative current policy at file end) | Historical snapshot: routine publication is now coordinator-authorized after checks and review | `docs/AGENT_TASKS.md:636-645,2405-2467`; `XB-AUTONOMY-ACTIVATION-03` |
| Publication of all local `pass` work | Historical snapshot: locally complete and unpublished at that time; `XB-AUTONOMY-ACTIVATION-02` was committed and pushed, other packages remain `local_only` | historical F1 framing; authoritative current policy at file end | Historical snapshot: routine publication is now coordinator-authorized after checks and review | `docs/GITHUB_AGENT_WORKFLOW.md:7-9,49`; `XB-AUTONOMY-ACTIVATION-03` |

```yaml
PACKAGE_ID: XB-0082
PARENT_ID: N/A
DEPENDENCIES: N/A
STATUS: needs_owner
IMPLEMENTATION_STATE: needs_owner
REVIEW_STATUS: not_reviewed
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: OWNER-XB0082-SCOPE
PROGRESS_UNITS: N/A
```

`documented` — Single owner-decision list (question → recommendation → alternatives → impact → urgency → work that can continue):
- `OWNER-PUBLICATION`: Which files/groups should be staged, committed and published, and what happens to PR #1? Recommendation: owner selects the 12 `READY_FOR_COMMIT` development documents only; alternatives: publish nothing, publish a narrower group, or publish more after review; impact: irreversible publication state; urgency: high only when owner wants publication; local documentation/test work continues. `documented` — historical F1 framing from `XB-0085`; the authoritative current policy is appended at the end of this file. Routine staging/commit/push/PR/merge of independently reviewed work is coordinator-authorized; PR #1 handling and any tag/release/strategic-branch/history decision remain owner decisions.
- `OWNER-XB0082-SCOPE`: Approve split write set, status taxonomy and any future tool/CI/dependency for XB-0082? Recommendation: keep the current evaluation-only convention and defer tooling; alternatives: approve a generator/tool package; impact: new dependency/CI surface; urgency: medium; all other work continues.
- `OWNER-XB0083-START`: Release a researcher package for authoritative re-verification (R-001…R-006, XB-0083)? Recommendation: approve a scoped read-only package with exact URLs and access dates; alternatives: defer; impact: evidence quality only; urgency: medium; tests and documentation continue.
- `OWNER-XB0084-DECISIONS`: Resolve the seven XB-0084 decision-register items (baseline, license policy, ADRs, native/JIT policy, Xbox 360 scope, performance publication, FINAL_CLEANUP)? Recommendation: decide in the recorded order; alternatives: defer each; impact: gates all product implementation; urgency: medium; tests and cleanups continue.
- `OWNER-X64-STEP`: Approve `XB-X64-STEP-01` public interface or choose a private helper / defer? Recommendation: approve after the current test package produces evidence; alternatives: private-only seam or defer; impact: new public API; urgency: low-medium; tests-only work continues.
- `OWNER-HOST-CAP`: Approve `XB-HOST-CAP-01` interface and Windows-probing scope? Recommendation: deterministic mock provider first, real probing later; alternatives: defer; impact: host/guest boundary; urgency: low; tests-only work continues.
- `OWNER-SETUP-004/006/007`: Execute session transfer, decide worktree cleanup, install specialists? Recommendations: owner runs export/import manually; remove or keep the unregistered checkout by owner choice; keep six installed roles for now and repackage work with installed substitutes; urgency: medium; local documentation/test work continues.
- `OWNER-AI-README`: Apply or permanently reject the prepared `ai/README.md` patch? Recommendation: apply before broad publication; alternatives: leave unapplied and exclude from publication; urgency: low; other work continues.
- `OWNER-X64-VERIFY`: How should the stale `E:`-drive CMake cache in `build/windows-debug/CMakeCache.txt` be handled so the declared VS2026 configure/build/CTest verification of `XB-X64-CORPUS-01` can actually run? Recommendation: owner removes or regenerates the stale cache/build directory outside an agent run, then an approved tester run repeats exactly the three declared commands; alternatives: keep the package `needs_owner` with tests unexecuted, or grant a scoped fresh-configure command; impact: only build outputs under `build/**`, no new dependency and no product-code change; urgency: medium, because it is the only executable package and its compile/runtime result remains `unknown`; all other documentation/publication-preparation work continues unaffected. `observed` — The technical part of this decision is now satisfied: the owner applied the absolute CMake and CTest rules, and the tester's configure/build/CTest chain returned `EXIT=0` with 1/1 tests passed; `XB-X64-CORPUS-01` is `pass` under `XB-CMAKE-VERIFY-RESUME`. The publication, configuration and scope alternatives in this decision remain owner territory and are not re-opened by this local closure.

`unknown` — No owner decision above was silently resolved by an agent. No build, CTest, PR or publication result is treated as current verification unless a later section records the exact executed command and result.

## XB-CONTINUATION-01-C1 — bundled correction and final review result

`observed` — One bundled correction package `XB-CONTINUATION-01-C1` was applied by the coordinator as sole writer of `docs/AGENT_TASKS.md` and `docs/ROADMAP.md`; writer sets stayed disjoint from the tester's `tests/test_main.cpp`. It closed all three round-1 findings by root cause: R01 (record every failed command/exit, denied forms and stale-cache root cause; assign `needs_owner` axes; add `OWNER-X64-VERIFY`; fix roadmap step 8 and the two follow-on stale anchors), R02 (refresh the audit to 25 entries with a `tests/test_main.cpp` NEEDS_REVIEW row and 12/3/10 counts), R03 (single allowed envelope tokens with prose scope note).

`observed` — Mandatory targeted re-check by the same independent reviewer (`ses_df16e3a75ffeJg0HIeGf1y64xC`): verdict `PASS`, **zero findings** (`ACTIONABLE` 0, `OWNER_DECISION` 0, `NON_BLOCKING` 0, `UNKNOWN` 0). Case A applies: no further correction, no second review. The reviewer confirmed R01/R02/R03 resolved and closed, the failed verification honestly recorded, `XB-X64-CORPUS-01` never marked `pass`, the XB-0082 yaml block compliant, and no publication/staging/commit or config/agent/permission/product-source/CMake edit by C1. Reviewer limitation: no shell/git tool in its catalog, so Git facts and raw command output were assessed from the recorded ledger, not independently re-run.

```yaml
PACKAGE_ID: XB-CONTINUATION-01-C1
PARENT_ID: XB-CONTINUATION-01
DEPENDENCIES: N/A
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — Final coordinator verification after all edits: `git status --porcelain=v1 --untracked-files=all` → 25 entries (18 modified including `tests/test_main.cpp`, 7 untracked); `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git diff --cached --name-only` → no output; `git diff --check; "EXIT=$LASTEXITCODE"` → only the pre-existing `CMakePresets.json` CRLF warning, `EXIT=0`. No build or CTest was run or claimed by this continuation; no staging, commit, PR, merge, tag, release, worktree or publication action occurred. `STATUS: pass` above applies only to the coordinator's bundled documentation correction and its independent re-check, not to `XB-X64-CORPUS-01`, which remains `needs_owner` under `OWNER-X64-VERIFY` because its declared configure/build/CTest commands failed.

## XB-X64-VERIFY — owner-authorized cache repair and verification attempt

PACKAGE_ID: XB-X64-VERIFY
PACKAGE_TYPE: BUGFIX
PRODUCT_BOUNDARY: development-only
GOAL: Repair or route around the stale E:-drive CMake cache so the five new tests in tests/test_main.cpp are actually compiled and executed, and set XB-X64-CORPUS-01 to pass only on real evidence. This is local build verification with no product change.
FILES: no source file; only generated build outputs under build/** may be created. tests/test_main.cpp is explicitly read-only in this package.
ALLOWED: read-only inspection; backup of the stale cache if permission permits; fresh configuration/build/CTest via allowlisted commands; tester as sole executor.
FORBIDDEN: editing tests/test_main.cpp or any product/CMake/preset/toolchain file; bypassing denied permissions; deletion/move without backup; any Git publication action; config/agent/permission edits; software installation.
ACCEPTANCE: configure, build and CTest actually exit 0 with the xbridge_tests target executed; independent reviewer PASS; status changed only on that evidence.
TESTS: exact allowlisted windows-debug commands plus the independent ninja-debug route; exact commands and exit states recorded.
REVIEW: exactly one bundled independent reviewer after execution; targeted re-check only for real findings.
WORKTREE: none; current checkout.
OWNER_APPROVALS: the owner issued this verification task; publication remains a separate OWNER-PUBLICATION decision.

`observed` — Phase A basis re-read by the coordinator: HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`, branch `docs/opencode-publication-group-2`, 25 working-tree entries, empty index. `build/windows-debug/CMakeCache.txt:2-3,198,204,214,263` still contains `E:/XBridge 1.0/XBridge-Series-Github` and `E:/PRO/Programm Daten/Visual Studio/18/Community`; current environment is `D:` with VS at `C:\Program Files\Microsoft Visual Studio\18\Community` (`docs/DEVELOPMENT_ENVIRONMENT.md:67-70,86-87`). `opencode.jsonc:147-150` hard-denies `Remove-Item`, `Move-Item`, `rm`, `del`; `Copy-Item` is in no agent allowlist. Per the owner's instruction, a denial must be documented, not bypassed; exact command results are pending from the tester.

`observed` — Tester execution handoff (`ses_df1613fe3ffekpwe3unWKpN5ws`). Phase A, all exit 0: `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git status --porcelain=v1 --untracked-files=all` → 25 entries; `git diff --cached --name-only` → empty; `cmake --version` and `ctest --version` → `4.3.1-msvc1` each.

`denied` — Phase B: `Copy-Item -Path build/windows-debug/CMakeCache.txt -Destination build/windows-debug-cache-backup-20261124.txt; "EXIT=$LASTEXITCODE"` → `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}`. No backup created; no retry, no alternate form, no other tool, nothing deleted or moved. Cache repair is therefore not executable by any installed agent; this is the concrete blocker for the owner.

`observed` — Phase D1 (`windows-debug`, exact allowlisted commands, cache untouched): `cmake --preset windows-debug` → **exit 1** (CMakeCache directory `D:/…` differs from `e:/…`; source `D:/…/CMakeLists.txt` does not match `E:/…`); `cmake --build --preset windows-debug --config Debug` → **exit 1** (same cache mismatch; generator `Visual Studio 18 2026` could not find `E:/PRO/Programm Daten/Visual Studio/18/Community`); `ctest --test-dir build/windows-debug -C Debug --output-on-failure` → **exit 8** (`Unable to find executable: E:/…/xbridge_tests.exe`, `1/1 Test #1: xbridge_tests ... ***Not Run`, `0% tests passed, 1 tests failed out of 1`).

`observed` — Phase D2 (`ninja-debug`): the coordinator's Phase-A assumption that this build directory was cache-free was **wrong**. `build/ninja-debug/CMakeCache.txt:2,76,222,277` exists and is stale on `E:` (generator `Ninja`, `CMAKE_MAKE_PROGRAM` at `E:/PRO/…/Ninja/ninja.exe`), independently re-read by the coordinator. `cmake --preset ninja-debug` → **exit 1** (same D:/e: mismatch); `cmake --build --preset ninja-debug` → **exit 1** (`no such file or directory`, build tool `E:/PRO/…/ninja.exe`); `ctest --preset ninja-debug --output-on-failure` → **exit 8** (`Unable to find executable: E:/…/xbridge_tests.exe`, `1/1 ... ***Not Run`, `0% tests passed, 1 tests failed out of 1`). No compiler invocation succeeded; the five new test functions were not compiled or executed.

`observed` — Final Git state byte-identical to Phase A: 25 entries, empty index, `tests/test_main.cpp` not edited by the tester. No build directory was created or changed by the tester; configure/build failed before generation, CTest was read-only. Outcome: **BLOCKED, not passed**. `XB-X64-CORPUS-01` remains `needs_owner`; the status is not changed on this evidence.

`observed` — One bundled independent final review (`ses_df15f87ddffeuFzakoAjTPaMpt`): verdict **`PASS` with zero findings** (`ACTIONABLE` 0, `OWNER_DECISION` 0, `NON_BLOCKING` 0, `UNKNOWN` 0); Case A applies, no correction, no second review. The reviewer independently re-read both caches (stale `E:` paths confirmed), all five test definitions and their single `main()` registrations (intact, harness unchanged), the CTest `***Not Run`/`0%` outputs, the verbatim `Copy-Item` denial plus the hard-deny rules, and the unchanged `needs_owner` axes. Reviewer limitation: no shell/Git tool in its catalog, so Git facts and command re-execution were assessed from file state and recorded output, not re-run.

`documented` — Scope of that `PASS`: it certifies only the honesty and accuracy of the XB-X64-VERIFY record (failure reported, denial documented without bypass, wrong ninja-cache assumption corrected, no status changed on insufficient evidence). It does **not** certify build or CTest success, compile/runtime correctness of the five tests, external truth, or publication. The reviewer's explicit conclusion: `XB-X64-CORPUS-01` may **not** be set to `pass`; its ACCEPTANCE (configure/build/CTest exit 0 with xbridge_tests executed) is unmet (1/1/8, 0 tests executed), so it stays `needs_owner` under `OWNER-X64-VERIFY`.

## XB-CONTINUATION-02 — build-unblock attempt, current verification, and maximum released work

PACKAGE_ID: XB-CONTINUATION-02
PACKAGE_TYPE: RESEARCH
PRODUCT_BOUNDARY: development-only
GOAL: Attempt the owner-authorized build unblock, re-verify XB-X64-CORPUS-01 with current commands, and complete every released nonblocked package with one bundled final review.
FILES: `docs/AGENT_TASKS.md`, `docs/ROADMAP.md` (coordinator only). No product, test, CMake, preset, config or agent file was edited.
ALLOWED: read-only inspection; exact allowlisted tester commands; append-only ledger and roadmap records; complete drafted envelopes for owner-gated packages.
FORBIDDEN: editing `opencode.jsonc` or `.opencode/**`; bypassing denied permissions; product code without interface approval; weakening tests; cache deletion/move; Git publication; build claims without real execution.
ACCEPTANCE: current configure/build/CTest results recorded with exact exit codes; the Package A blocker precisely prepared for the owner; all open packages consolidated; complete D/E envelopes recorded; XB-X64-CORPUS-01 status kept honest; exactly one independent final review.
TESTS: exact allowlisted commands only (current-state results below); no build/CTest pass claimed.
REVIEW: exactly one bundled independent reviewer after all writes; targeted re-check only for real ACTIONABLE findings.
WORKTREE: none; current checkout.
OWNER_APPROVALS: the permission expansion was authorized by the owner in this order but is NOT executable by agents (see Package A below); every other decision remains open as collected at `:1058-1067`.

`observed` — Phase A (one consolidated pass): `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git status --porcelain=v1 --untracked-files=all` → 25 entries (18 modified, 7 untracked); `git diff --cached --name-only` → empty; `git diff --check` → only the known `CMakePresets.json` CRLF warning. `CMakePresets.json:5-19` uses `${sourceDir}` with generator `Visual Studio 18 2026` (`:7`) and `Ninja` (`:14`) — no hard-coded toolchain path. Both caches remain stale on `E:` (`windows-debug/CMakeCache.txt:2-3,198,204,214,263`; `ninja-debug/CMakeCache.txt:2,76,222,277`).

### Package A — permission expansion authorized but NOT executable by agents (OWNER_DECISION)

`observed` — The owner authorized a narrowly scoped permission expansion for `Copy-Item`, `cmake --fresh --preset windows-debug`, the debug build, its CTest call and an optional ninja route. Effective rights make this NOT executable by any installed agent: (1) `coordinator.md:50-55` denies `edit` on `opencode.jsonc` and `.opencode/**`; (2) `cmake --fresh` and `Copy-Item` are in no allowlist (`opencode.jsonc:73-96`, `tester.md:23-67`); (3) `opencode.jsonc:147-150` hard-denies `Remove-Item`, `Move-Item`, `rm`, `del` (top-level policies cannot be granted by a rule). No denial was bypassed and no config file was edited.

`denied` — Probe by tester (`ses_df1585811ffeqeFNdU5kAznCmD`): `cmake --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"` → `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}`, before cmake started; no exit code exists. Both caches and both file contents unchanged; no `build/*-cache-backup*` exists; `.owner-patch-backup/` does not exist yet (but is already excluded at `.gitignore:17`). Fresh configuration is therefore impossible for agents in this session.

`documented` — Minimal owner patch (prepared, NOT applied). The following block is illustrative manual-edit guidance, **not** a tool-applicable unified diff: hunk headers and context are abbreviated, so `git apply` is not expected to succeed; the owner inserts the two `+` lines at the cited locations by hand. The owner performs these edits and reloads OpenCode; whether a reload is required for the new rules to take effect is `unknown`.
```diff
--- a/opencode.jsonc
+++ b/opencode.jsonc
@@ -89,3 +89,5 @@
      { "action": "shell", "resource": "cmake --preset windows-debug", "effect": "allow" },
+     { "action": "shell", "resource": "cmake --fresh --preset windows-debug", "effect": "allow" },
+     { "action": "shell", "resource": "Copy-Item -Path build/windows-debug/CMakeCache.txt -Destination .owner-patch-backup/windows-debug-CMakeCache.txt", "effect": "allow" },
--- a/.opencode/agents/tester.md
+++ b/.opencode/agents/tester.md
@@ -46,3 +46,4 @@
      effect: allow
+  - action: shell
+    resource: "cmake --fresh --preset windows-debug"
+    effect: allow
```
`documented` — Owner sequence (outside an agent run): create `.owner-patch-backup/`; back up both `build/windows-debug/CMakeCache.txt` and `build/ninja-debug/CMakeCache.txt` there; apply the patch above; reload; then authorize a fresh tester run of `cmake --fresh --preset windows-debug`, `cmake --build --preset windows-debug --config Debug`, `ctest --test-dir build/windows-debug -C Debug --output-on-failure`. Because `CMakePresets.json` stores no path, a fresh cache should resolve VS2026 from the current install (`C:\Program Files\Microsoft Visual Studio\18\Community`) — `hypothesis`, not verified until the fresh run succeeds. The hard denys for delete/move stay in force; `cmake --fresh` performs its own cache regeneration and needs no shell delete rule.

### Package B — current-state verification of XB-X64-CORPUS-01 (observed, all failed)

`observed` — Tester (`ses_df1585811ffeqeFNdU5kAznCmD`), exact allowlisted commands, current run (not historical), executed with full output:
- `cmake --preset windows-debug; "EXIT=$LASTEXITCODE"` → **exit 1** (`CMakeCache.txt` directory `D:/…` differs from `e:/…`; source `D:/…/CMakeLists.txt` does not match `E:/…`).
- `cmake --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"` → **exit 1** (same mismatch; generator `Visual Studio 18 2026` could not find `E:/PRO/Programm Daten/Visual Studio/18/Community`).
- `ctest --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"` → **exit 8** (`Unable to find executable: E:/…/xbridge_tests.exe`; `1/1 Test #1: xbridge_tests ... ***Not Run`; `0% tests passed, 1 tests failed out of 1`). Found 1 / executed 0 / passed 0 / failed 1 (`Not Run`) / skipped 0.
Both caches were re-read in full and are byte-identical to baseline. `tests/test_main.cpp` was not edited. **No test was compiled or executed.** `XB-X64-CORPUS-01` therefore remains `needs_owner` under `OWNER-X64-VERIFY`; no status change on this evidence.

### Package C — systematic disposition of open documentation/research work

`documented` — C1 `XB-0082`: evaluation-only convention is complete and consistent at `docs/AGENT_WORKFLOW.md:157-174` (fixed key order, allowed tokens, `PROGRESS_UNITS: N/A` unless a declared integer denominator, `STATUS: pass` as the only complete token). No generator, tool, CI, dependency or percentage display was added; a real implementation stays behind `OWNER-XB0082-SCOPE` (`:1045-1055,1060`).
`documented` — C2 `XB-0083` / `R-001`…`R-006`: read-only researcher verification (`ses_df1585811ffcXPy5zYyyRlo8YO`) confirmed the ledger at `:1033-1034` is accurate: all six legacy rows are `unknown`/`conformant` with `no package assigned` and gated by `OWNER-XB0083-START` (`:1061`); R-005's `partial` is a workflow note only. RESEARCH-007 and XB-0084 are complete with independent `PASS` and were not re-researched. No web access, no edits, no new research package started.
`documented` — C3 `XB-0084` decision register: the seven open points are consolidated at `docs/research/XB-0084_RISK_DEPENDENCY_OVERVIEW.md:108-118` (assessment baseline, project license/third-party policy, subsystem ADRs, native/JIT policy, optional Xbox 360 scope, performance-publication protocol, FINAL_CLEANUP timing), with components, alternatives, dependencies, risks and ADR references at `:14,39-46,112-118`. No decision was silently resolved; all remain owner-gated.
`documented` — C4 `SETUP-004/006/007`: remain owner actions (session export/import, worktree cleanup, agent installation/persisted permissions) at `:15,17-18,1039-1041`; installed toolchains exist and nothing was reinstalled. No executable agent step remained.
`documented` — C5 `ai/README.md` patch 2: re-read `:13` still contains "Open a pull request"; the complete patch proposal is unchanged at `:634-649` and remains `OWNER_DECISION` RV-P1-08 (`:716`). Not applied by an agent.
`observed` — C6 `XB-0085-CURRENT`: at the current HEAD the tree still has 25 entries and the audit at `:937-955` applies unchanged: 12 `READY_FOR_COMMIT`, 3 `NEEDS_REVIEW`, 10 `DO_NOT_PUBLISH`; the historical 32-entry (13/18/1) and 24-entry (12/2/10) snapshots remain explicitly labelled historical. No staging, publication or PR action; PR state stays `unknown`.

### Package D/E — complete envelopes for the owner-gated public interfaces (drafts recorded, not implemented)

`documented` — The architect (`ses_df1585811ffdRFFa5eq8f9CcOl`) supplied the missing concrete detail for two envelopes that the ledger previously only referenced (`:1026,1037-1038`). Both are **drafts for the coordinator**, `hypothesis` as designs, explicitly **not** review verdicts, approvals or implementations. No product file was edited.

```text
PACKAGE_ID: XB-X64-STEP-01
PACKAGE_TYPE: FEATURE
PRODUCT_BOUNDARY: product
GOAL: Add a byte-span x64 single-step facade that composes the existing decoder and interpreter so one documented instruction executes per call without replacing either contract.
FILES: include/xbridge/cpu/x64_instruction_interpreter.hpp; src/cpu/x64_instruction_interpreter.cpp; tests/test_main.cpp
ALLOWED: Declare [[nodiscard]] X64ExecuteStatus step_x64_instruction(std::span<const std::uint8_t> bytes, X64CpuState& state, memory::GuestMemory& memory) noexcept in namespace xbridge::cpu; implement it only as decode_x64_instruction(bytes) followed by execute_instruction(decoded, state, memory); add tests using the existing check harness.
FORBIDDEN: JIT; RIP-based fetch; run loops; new decode/execute status values; FLAGS/SIB/SIMD/displacement semantics; changes to TestCpu, GuestMemory, decoder representation, CMakeLists.txt, presets, docs, agent/config files; weakening existing tests.
ACCEPTANCE: The facade returns Ok and advances RIP by DecodedInstruction::length for documented NOP/MOV forms; returns MemoryAccessError with unchanged CPU/memory for out-of-range and >UINT32_MAX addresses; treats every non-Ok decode status as an Ok no-op with unchanged state; all pre-existing tests remain; exact build/CTest/diff commands executed with real results.
TESTS: NOP {0x90}; MOV imm32 {0xB8,0x78,0x56,0x34,0x12}; MOV imm64 with REX.W {0x48,0xB8,...}; MOV reg/reg and reg/mem round trips; 4-/8-byte boundary failures at GuestMemory edge; address 0x100000000ULL -> MemoryAccessError without mutation; non-Ok no-ops for empty span, {0x0F}, {0x89,0x04}, {0x48,0x8B}. Verify: cmake --preset windows-debug; cmake --build --preset windows-debug --config Debug; ctest --test-dir build/windows-debug -C Debug --output-on-failure; git diff --check.
REVIEW: architect interface re-check; tester independent verification; reviewer read-only verdict.
WORKTREE: none
OWNER_APPROVALS: OWNER-X64-STEP (public interface); private test-helper alternative avoids a new public API but still needs an approved envelope.
```
`documented` — Basis: decoder contract `include/xbridge/cpu/x64_instruction_decoder.hpp:17-23,56-77` + `docs/ARCHITECTURE.md:104-139`; interpreter contract `include/xbridge/cpu/x64_instruction_interpreter.hpp:9-24` + `docs/ARCHITECTURE.md:153-163`; no FLAGS/SIB/SIMD/displacement (`docs/ARCHITECTURE.md:170-180`); the span facade is NOT the deferred RIP-fetch bridge (`docs/ROADMAP.md:27,49`). Not implemented: adding a public header symbol needs owner approval (`AGENTS.md:26,63`).

```text
PACKAGE_ID: XB-HOST-CAP-01
PACKAGE_TYPE: FEATURE
PRODUCT_BOUNDARY: product
GOAL: Add a deterministic capability-provider seam so policy consumes immutable host facts through an interface without Windows probing or guest-semantic change.
FILES: include/xbridge/host/capabilities.hpp; include/xbridge/host/execution_policy.hpp; src/host/execution_policy.cpp; tests/test_main.cpp
ALLOWED: Add ICapabilityProvider (virtual ~, [[nodiscard]] capabilities() const noexcept = 0), FixedCapabilityProvider (explicit constructor, immutable HostCapabilities), and select_execution_plan(const ICapabilityProvider&, const ExecutionSettings&) forwarding to the existing HostCapabilities overload; add deterministic provider tests.
FORBIDDEN: Windows/Win32/Vulkan/D3D12 probing; backend initialization; firmware/keys/proprietary assets; new dependencies; CMake/preset changes; changing or removing the existing select_execution_plan overload; guest CPU/memory/GPU/profile changes; JIT; compatibility claims.
ACCEPTANCE: Provider overload yields a plan and report identical to the direct overload for the same facts; FixedCapabilityProvider is repeatable and reads no OS state; existing selector and tests unchanged; no probing or dependency added; exact build/CTest/diff commands executed with real results.
TESTS: Reuse synthetic fixtures at tests/test_main.cpp:1630-1676 for automatic Vulkan selection, software fallback, fallback-disabled Unavailable, manual Direct3D12, JIT available/unavailable and report text; verify repeated immutable facts and unmutated settings. Verify: cmake --preset windows-debug; cmake --build --preset windows-debug --config Debug; ctest --test-dir build/windows-debug -C Debug --output-on-failure; git diff --check.
REVIEW: architect interface/ADR and host/guest-boundary check; tester independent verification; reviewer read-only verdict.
WORKTREE: none
OWNER_APPROVALS: OWNER-HOST-CAP (provider interface and any later Windows-probing scope).
```
`documented` — Basis: `HostCapabilities` is host-only data (`include/xbridge/host/capabilities.hpp:7-19`); pure selector `docs/ARCHITECTURE.md:229-231`, `src/host/execution_policy.cpp:17-93`; ADR-0005/0010/0011 (`docs/DECISIONS.md:23-26,48-56`); no real detector exists (`docs/ADAPTIVE_EXECUTION.md:36-38`, `docs/HOST_HARDWARE_POLICY.md:38-40`, `src/profiles/hardware_profile.cpp:1-2`). Deterministic-provider-first is the recommendation at `:1064`; real Win32/DXGI/Vulkan entry points are `unknown` and excluded.

`documented` — Package F (Agent Core): no new contradiction was found beyond items already recorded at `:121-125` (installed `developer`/`tester` both may write `tests/**`; future-planned `test-engineer` exclusivity; global web denies vs researcher allows resolved by last-match-wins at `docs/AGENT_WORKFLOW.md:95`; SETUP-006/007 still `blocked`). No documented architecture was rewritten; no new dependency, global permission, external automation or publication was added. Package G (consolidation): this single record plus the roadmap note below are the targeted updates; no broad restructuring.

```yaml
PACKAGE_ID: XB-CONTINUATION-02
PARENT_ID: N/A
DEPENDENCIES: OWNER-X64-VERIFY, OWNER-X64-STEP, OWNER-HOST-CAP, OWNER-XB0082-SCOPE, OWNER-XB0083-START, OWNER-XB0084-DECISIONS, OWNER-PUBLICATION
STATUS: needs_owner
IMPLEMENTATION_STATE: needs_owner
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: OWNER-X64-VERIFY
PROGRESS_UNITS: N/A
```
`documented` — Honest outcome: no build, configure, compile, CTest or product-code success was achieved; all executable work consisted of current-state verification (failed as recorded), read-only verification, complete owner patches/envelopes and documentation consolidation. `STATUS: pass` is not claimed. The five new tests remain uncompiled (`unknown` runtime correctness), and every owner decision at `:1058-1067` stays open.

`observed` — Final bundled independent review (`ses_df1550a00ffe1EInCOTAS66xWT`) returned **NEEDS_CHANGE**: 1 `ACTIONABLE` (LOW, `XB-CONT-02-REV-01`: wrong anchor `docs/AGENT_WORKFLOW.md:84` instead of `:95` for last-match-wins), 1 `NON_BLOCKING` (INFO, `XB-CONT-02-REV-02`: patch block not a tool-applicable unified diff), 0 `OWNER_DECISION`, 0 `UNKNOWN`. Case B applied: ONE bundled correction by the coordinator as sole writer of `docs/AGENT_TASKS.md` (both `:84` anchors corrected to `:95` at `:123` and `:1217`; illustrative manual-edit label added before the patch block at `:1146`). No finding was downgraded. Targeted independent re-check (same session) returned **PASS** with 0 `ACTIONABLE`: both anchors resolve to `docs/AGENT_WORKFLOW.md:95`, the patch label is present and consistent with "prepared, NOT applied", envelope/yaml tokens and build/test honesty are unchanged, Roadmap stays consistent, and no new finding was introduced. Two pre-existing stale anchors at `:290`/`:311` were reported `NON_BLOCKING` and not reopened. Case B is complete after one correction round; no second correction round and no further review. This paragraph and the `REVIEW_STATUS: PASS` update are coordinator bookkeeping added after the PASS re-check; they record the verdict and do not change any reviewed substantive artifact.

## XB-X64-VERIFY — controlled Windows build re-run (owner order, current session)

`documented` — Owner ordered exactly one tester command `cmake --fresh --preset windows-debug`, build/CTest only on prior success, no permission bypass, no source/test/CMake/cache/Git changes, independent reviewer assessment. This is a re-run, not a new package envelope.

`observed` — Preparation: HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`, branch `docs/opencode-publication-group-2`, 25 working-tree entries, empty index (unchanged). New rules are present in `opencode.jsonc:92-96` and `.opencode/agents/tester.md:47-61`, so the scoped expansion was applied. **Correction to the coordinator's initial preparation note:** `.owner-patch-backup/` is NOT empty. A direct directory read shows `windows-debug-CMakeCache.txt`, `ninja-debug-CMakeCache.txt`, `opencode.jsonc`, `opencode.jsonc.original`, `tester.md.original` and `.opencode/`; the earlier `glob` result "No files found" was a tool artifact (the directory is excluded at `.gitignore:17`). Both cache backups' lines 2-3 match the source caches. No *new* backup was created by this run; no cache was deleted/moved.

`observed` — tester (`ses_df1354920ffeWaKLHxv3eogAVJ`) ran exactly `cmake --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"` once. It was **not** permission-denied; the command reached PowerShell. Result: `CommandNotFoundException` (`Die Benennung "cmake" wurde nicht als Name eines Cmdlet ... erkannt`), `FullyQualifiedErrorId: CommandNotFoundException`, and `EXIT=` (empty — no native process started, no numeric cmake exit code). Verdict: **configure FAIL**, not PASS, not permission-BLOCKED. No alternate form, absolute path, `cmake.exe`, `Copy-Item`, or permission edit was attempted. Because configure was not successful, build and CTest were correctly NOT run.

`observed` — Independent reviewer (`ses_df1347fdfffe56fDvdKXF6c8L2`) verdict **PASS, Case A**: 0 `ACTIONABLE`, 1 `NON_BLOCKING` (the missing-backup misstatement above, now corrected from the reviewer's direct read), 1 `UNKNOWN` (reviewer has no shell; Git state and `CommandNotFoundException` root cause not independently re-executed). The PASS certifies the honesty and completeness of the **FAIL** record, not build/test success. The five tests at `tests/test_main.cpp:1216,1309,1390,1439,1457` remain uncompiled and unexecuted; `XB-X64-CORPUS-01` stays `needs_owner` under `OWNER-X64-VERIFY`. No regression assessment is possible with zero executed tests (`unknown`).

`documented` — Outcome: configuration **FAIL**, build **not run**, CTest **not run**, 0 tests compiled/executed/passed/failed. Technical blocker: `cmake` is not resolvable on the tester shell PATH in this session (`unknown` why, e.g. plain-PATH resolution vs VS2026-only install); the owner's exact command is now permission-allowed but cannot execute. No publication, staging, commit, or product change occurred.

## XB-CMAKE-PATH-DIAGNOSTIC — read-only shell resolution check

`documented` — Owner ordered six read-only probes only (Get-Command/where.exe/Test-Path/absolute --version/$env:PATH/tester-environment parity), with no config/build/cache/Git action. No permission denial was bypassed or retried.

`observed` — Coordinator: probes 1-4 (`Get-Command cmake -All`, `where.exe cmake`, `Test-Path` on the supplied absolute path, direct absolute `cmake.exe --version`) each returned `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}` (coordinator.md:74-85). Probe 5 (`$env:PATH`) executed; the PATH contains no `Microsoft Visual Studio` or `CMake` entry. Tester (`ses_df12fb8deffeeUczu5oOkAMe5A`): the same probes 1-4 were likewise `permission.rejected` (tester.md:20-22 `shell: * deny` plus exact allows). Allowlisted `cmake --version` executed and returned PowerShell `CommandNotFoundException` with `EXIT=` empty. Tester probe 5 PATH was captured.

`observed` — Correction of a transcription artifact: the PATH string supplied to the tester in the prompt accidentally contained an extra `C:\Program Files (x86)\NVIDIA Corporation\NVIDIA App\NvDLISR`. Direct comparison of the ACTUAL coordinator and tester `$env:PATH` outputs shows them **identical**. The shells therefore show no observed PATH difference; permission rules differ by role (coordinator `cmake * deny`, tester exact cmake allows), which is a permission-layer difference, not an environment difference. Probes 3/4 were denied in both roles, so on-disk existence and absolute-path executability of the supplied `cmake.exe` remain `unknown`; no claim is made for either.

`documented` — Diagnostic result: bare `cmake` is unresolvable in both shells because the VS CMake bin directory is absent from PATH (`observed` for PATH, `hypothesis` for the causal link). The owner-supplied absolute path is `documented` as supplied but was not verifiable under installed-role permissions. No fix was applied; any PATH/permission change requires an owner decision.

## XB-CMAKE-ABSOLUTE-PATH-ENABLEMENT — Phase 1 proposal only (NOT applied)

`documented` — Owner supplied the absolute CMake path `C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` and stated that outside OpenCode `Test-Path` returned True and the version is 4.3.1 (`documented` owner report, not re-verified by an agent). Phase 1 is read-only; Phase 2 requires explicit owner approval; no config file may be edited by the coordinator (`coordinator.md:50-55`).

`observed` — Current rules: `opencode.jsonc:87-100` contains only bare-name CMake/CTest allows and the two `Copy-Item` backup allows; hard denys at `opencode.jsonc:131-155` (git publication, `cmake --install`, install-target builds, `Remove-Item`/`Move-Item`/`rm`/`del`) are unchanged. `tester.md:20-22` has `shell: * deny`, followed by exact allows at `:44-61` for bare CMake/CTest and at `:62-70` for ninja. Last-match-wins (`AGENT_WORKFLOW.md:95`) means agent-level exact allows suffice; a global edit is NOT required.

`documented` — Minimal proposed change (tester.md ONLY, insert after line 61 before the ninja block). YAML double-quoted scalars use `\\` for each literal backslash; the executor must use exactly these strings with the `; "EXIT=$LASTEXITCODE"` suffix only for exit capture:

```diff
--- a/.opencode/agents/tester.md
+++ b/.opencode/agents/tester.md
@@ -59,6 +59,15 @@
   - action: shell
     resource: "ctest --test-dir \"build/windows-debug\" -C Debug --output-on-failure"
     effect: allow
+  - action: shell
+    resource: "& 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin\\cmake.exe' --version"
+    effect: allow
+  - action: shell
+    resource: "& 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin\\cmake.exe' --fresh --preset windows-debug"
+    effect: allow
+  - action: shell
+    resource: "& 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin\\cmake.exe' --build --preset windows-debug --config Debug"
+    effect: allow
   - action: shell
     resource: "cmake --preset ninja-debug"
     effect: allow
```

`documented` — All existing deny rules remain: `tester.md:6-7,11-22,71-73` and global `opencode.jsonc:131-155`; no global PATH change, no global shell/CMake blanket allow, no other agent file, no other scope. `unknown`: whether the permission matcher accepts this `& '<absolute path>'` resource form; if a Phase 3 probe is denied, stop and report the verbatim denial without an alternate form. Bare `ctest` at `tester.md:56-61` is already allowed but its shell resolution is `unknown` (PATH lacks the VS bin); no absolute CTest rule is proposed because no such path was owner-confirmed. Status `needs_owner` pending explicit approval; no execution performed.

## XB-CMAKE-VERIFY-RESUME — absolute CMake/CTest chain (all four steps PASS)

`documented` — Owner confirmed the three absolute CMake rules were applied in `tester.md:62-70` and OpenCode was restarted. Verified by direct read: all three exact allows are present after line 61, with all denies intact (`tester.md:6-22,80-85`; global hard-denys unchanged). No config file was edited by an agent.

`observed` — Tester (`ses_df1199749ffeHGlpnRU2dVa31y`), each command run exactly once, no alternate form, no retry:
1. `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --version; "EXIT=$LASTEXITCODE"` → `cmake version 4.3.1-msvc1`, `EXIT=0` → **PASS**.
2. `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"` → `Configuring done (4.8s)`, `Generating done (0.4s)`, build files written to `D:/XBridge 1.0/XBridge-Series-Github/build/windows-debug`, `EXIT=0` → **PASS**. Working caches regenerated; backups untouched.
3. `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"` → `xbridge_core.lib`, `xbridge.exe`, `xbridge_tests.exe` built in `build/windows-debug/Debug`, `test_main.cpp` compiled, `EXIT=0` → **PASS**.
4. `ctest --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"` → PowerShell `CommandNotFoundException` for bare `ctest`, `EXIT=` empty, no native process started → **FAIL**, not a permission denial. Found/executed/passed/failed/Not Run = `unknown`; zero tests executed. No absolute CTest form was attempted.

`observed` — Read-only discovery: directory `C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin` contains `cmake.exe`, `cmcldeps.exe`, `cpack.exe`, `ctest.exe`; glob confirms `ctest.exe` at that exact path. No install, config edit or PATH change performed.

`documented` — Minimal CTest permission proposal (NOT applied; coordinator cannot edit `.opencode/**`). Insert ONE exact allow in `.opencode/agents/tester.md` after line 70, before the ninja block:

```diff
--- a/.opencode/agents/tester.md
+++ b/.opencode/agents/tester.md
@@ -68,6 +68,9 @@
   - action: shell
     resource: "& 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin\\cmake.exe' --build --preset windows-debug --config Debug"
     effect: allow
+  - action: shell
+    resource: "& 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin\\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure"
+    effect: allow
   - action: shell
     resource: "cmake --preset ninja-debug"
     effect: allow
```

`observed` — The owner subsequently applied that single rule and reloaded OpenCode; direct read-back confirmed it at `.opencode/agents/tester.md:71-73` with all deny rules intact. No agent edited `.opencode/**`.

`documented` — The authorized tester command was exactly `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"`. The earlier condition that `XB-X64-CORPUS-01` may be set to `pass` only on exit 0 with all expected tests executed and passed is now met: CTest `EXIT=0`, 1/1 registered binary `xbridge_tests` passed, and the five new cases are unconditional calls inside that binary. A build PASS alone was never treated as verification. No product/source/test/CMake change occurred.

`observed` — Fourth verification step (`ses_df1134a43ffe1ESP4as9wMhb7V`), exact command above, full output: `Test project D:/XBridge 1.0/XBridge-Series-Github/build/windows-debug`, `Start 1: xbridge_tests`, `1/1 Test #1: xbridge_tests ....................   Passed    0.05 sec`, `100% tests passed, 0 tests failed out of 1`, `Total Test time (real) =   0.12 sec`, `EXIT=0` → **PASS**. Found 1, executed 1, passed 1, failed 0, Not Run 0; no denial, no retry. The bare-`ctest` `CommandNotFoundException` at step 4 above was the earlier failed form and is superseded by this result.

`observed` — Final Git/cache state (`ses_df128f84fffeXUBeeHF44CjYHn`): HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`, branch `docs/opencode-publication-group-2`, 25 entries (18 modified, 7 untracked), empty index, `git diff --check` `EXIT=0` with only the pre-existing `CMakePresets.json` CRLF warning; `xbridge_tests.exe` present in `build/windows-debug/Debug`, CTest registration at root `CMakeLists.txt:32-36`, both `.owner-patch-backup/` cache backups present. Bundled reviewer `ses_df1120fd9ffeXNrOLLH95PvD6T`: **PASS, Case A**, 0 findings.

```yaml
PACKAGE_ID: XB-CMAKE-VERIFY-RESUME
PARENT_ID: XB-CONTINUATION-03
DEPENDENCIES: XB-CMAKE-ABSOLUTE-PATH-ENABLEMENT, OWNER-X64-VERIFY
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

## XB-CONTINUATION-03 — autonomous execution attempt (Phase A BLOCKED)

`documented` — Owner ordered autonomous continuation after the XB-CMAKE-ABSOLUTE-PATH-ENABLEMENT approval. Phase A (CMake verification) has highest priority; all later phases depend on configure/build/CTest success.

`observed` — Step 1: the three approved absolute-path rules are NOT present. `tester.md:44-70` still contains only bare-name CMake/CTest allows; `opencode.jsonc:87-100` likewise. Two Greps for `Program Files`, `cmake.exe`, `Common7` in both files returned "No matches found". The owner's approval was documented but the config edit was never applied. The coordinator may not edit `.opencode/**` or `opencode.jsonc` (`coordinator.md:50-55`); no config file was touched by an agent.

`denied` — Step 2, tester (`ses_df129869dffemAOUJcSCNf9Oem`) ran exactly `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --version; "EXIT=$LASTEXITCODE"` once: `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}`. No CMake output, no exit code. Per owner rule 7, no alternate form, no bypass, no retry.

`documented` — Consequence: steps 3–6 (fresh configure, build, CTest, CTest-path investigation) were NOT executed; they are only permitted after a successful preceding step. The five CPU tests in `tests/test_main.cpp` remain uncompiled/unexecuted; `XB-X64-CORPUS-01` stays `needs_owner`. No product code, header, test, CMake, preset, cache, backup, Git or publication action occurred. The previously consolidated C1–C6 documentation work and the D/E draft envelopes were not repeated because no new owner decision exists; repeating them would be redundant.

`documented` — Owner action required before Phase A can continue: apply the already-approved three lines from the XB-CMAKE-ABSOLUTE-PATH-ENABLEMENT proposal at `.opencode/agents/tester.md` after line 61 (before the ninja block), then reload OpenCode. No global/config/PATH expansion is needed beyond that exact diff. Any later CTest PATH issue must be handled by owner-confirmed path or explicit new permission decision; no absolute CTest rule was approved.

`observed` — Historical snapshot only: the owner later applied the three absolute CMake rules and the single absolute CTest rule, and the successor package `XB-CMAKE-VERIFY-RESUME` recorded all four verification steps as `EXIT=0`/PASS with a bundled reviewer `PASS`. The blocked Phase A evidence above is retained for the record; it is superseded by that successor, not re-run or rewritten. This section's axes therefore stay as the snapshot taken at the `CommandNotFoundException`/permission-denial stage.

```yaml
PACKAGE_ID: XB-CONTINUATION-03
PARENT_ID: N/A
DEPENDENCIES: XB-CMAKE-ABSOLUTE-PATH-ENABLEMENT, OWNER-X64-VERIFY
STATUS: needs_owner
IMPLEMENTATION_STATE: needs_owner
REVIEW_STATUS: not_reviewed
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: OWNER-X64-VERIFY
PROGRESS_UNITS: N/A
```

## XB-OWNER-DECISION-CONSOLIDATION — konsolidierte Owner-Entscheidungsvorlage

**development infrastructure — nicht Teil des Produktpakets** (`AGENTS.md` §10, `docs/AGENT_WORKFLOW.md` Product/Development boundary). Der Abschnitt steht bewusst am Dateiende: er ist append-only und verschiebt keine historischen `file:line`-Anker. Stand: 2026-11-24. Status der *Vorlage*: `needs_owner`. Es wurde **keine** Owner-Entscheidung getroffen, kein Paket freigegeben und keine Implementierung gestartet. Alle Empfehlungen sind Vorschläge des Coordinators.

Evidenzlabels: `documented` = in autoritativer Quelle bzw. akzeptierter Projektdatei explizit vorhanden; `observed` = direkt in Datei, Testausgabe oder reproduzierbarer lokaler Prüfung gesehen; `hypothesis` = plausibel, unverifiziert; `unknown` = nicht festgestellt.

Grundlage (vollständig gelesen, keine Untersuchung wiederholt): `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`, `docs/AGENT_WORKFLOW.md`, `docs/DECISIONS.md`, `docs/ARCHITECTURE.md`, `docs/RESEARCH_REGISTER.md`, `docs/research/XB-0084_*.md`, `docs/GITHUB_AGENT_WORKFLOW.md` sowie vorhandene Task-Envelopes. Zwei parallele read-only Voranalysen (`researcher` `ses_df10c47dfffeTWiVzecRqjJGjt`, `architect` `ses_df10c3052ffegFp6o77RBKousd`) lieferten Inventar und Architekturfolgen; beide schrieben nichts und führten keine Shell-/Git-/Build-Kommandos aus.

### 0. Leseregeln

1. `documented` — Autoritätskette: `AGENTS.md` → akzeptierte ADRs → `ARCHITECTURE.md`/`RESEARCH_REGISTER.md` → `AGENT_WORKFLOW.md` → Agentendateien; Widersprüche nicht stillschweigend auflösen (`AGENT_WORKFLOW.md:5-13`).
2. `documented` — Nur sechs Rollen installiert: `coordinator`, `architect`, `developer`, `tester`, `reviewer`, `researcher`. Alle Spezialisten inkl. `test-engineer` sind `planned` (`AGENT_WORKFLOW.md:36-49,227-249`). Kein Plan setzt einen nicht installierten Spezialisten als Writer ein.
3. `documented` — Zwingende Owner-Gates: öffentliche Schnittstellen, ADRs, neue Abhängigkeiten/Tool/CI, Lizenz/Recht, `.opencode/**`/`opencode.jsonc`/Berechtigungen, `CMakePresets.json`, Git/GitHub-Publikation, Spezialisteninstallation, Session-Export/Import, Worktree-Bereinigung (`AGENT_WORKFLOW.md:61,92-102,176-202,272,288-292`; `GITHUB_AGENT_WORKFLOW.md:7-9,55-60`).
4. `documented` — Nur `STATUS: pass` gilt als abgeschlossen (`AGENT_WORKFLOW.md:157-174`).
5. `observed` — Historische Snapshots bleiben erhalten und werden hier nicht als aktueller Zustand ausgegeben; konkrete Widersprüche stehen in §3.

### 1. Entscheidungsmatrix

| ID | Gruppe | Ursprungskennung | Art | Empfehlung in einem Satz | Aktueller Status |
|---|---|---|---|---|---|
| A1 | Architektur | `OWNER-XB0082-SCOPE` | zwingend | Evaluierung-only-Konvention bestätigen, Generator/Tool/CI weiterhin nicht freigeben. | `needs_owner` (`:1045-1055`) |
| A2 | Architektur | `OWNER-X64-STEP` | zwingend | `XB-X64-STEP-01` mit öffentlicher Fassade freigeben (Testevidenz liegt vor), ohne RIP-Fetch/JIT. | vorbereitet (`:1185-1199`) |
| A3 | Architektur | XB-0084 Punkt 3 | zwingend | ADRs nur einzeln mit eigener Evidenz freigeben, keinen Sammel-ADR akzeptieren. | offen (`RISK:114`) |
| A4 | Architektur | XB-0084 Punkt 4 | zwingend | Interpreter-Baseline behalten; JIT/Native weiterhin ohne eigene ADR/Tests nicht untersuchen. | offen (`RISK:115`) |
| A5 | Architektur | XB-0084 Punkt 5 | zwingend | Xbox 360 optional und deferred lassen; ADR-0004 unverändert. | offen (`RISK:116`) |
| B1 | Forschung | `OWNER-XB0083-START` | zwingend | Scoped read-only Researcher-Paket für R-001…R-006 mit exakten URLs/Daten freigeben. | `planned` (`:254-265,1033`) |
| B2 | Forschung | XB-0084 Punkt 1 | zwingend | XB-0084 als Evidenzbaseline akzeptieren — ohne Implementierungsfreigabe. | offen (`RISK:112`) |
| C1 | Host/Hardware | `OWNER-HOST-CAP` | zwingend | Nur den deterministischen Provider-Seam freigeben; echtes Windows-Probing separat entscheiden. | vorbereitet (`:1201-1215`) |
| D1 | Setup | `SETUP-004` | zwingend, Owner-only | Owner führt `session export/import` manuell aus und meldet echte Ergebnisse. | `needs_owner` (`:15`) |
| D2 | Setup | `SETUP-006` / XB-0085(f) | zwingend, Owner-only | Verzeichnis vorerst behalten, keine weiteren Öffnungsversuche. | `blocked` (`:17`) |
| D3 | Setup | `SETUP-007` | zwingend, Owner-only | Sechs installierte Rollen behalten, keine neun Spezialisten installieren. | `blocked` (`:18`) |
| D4 | Setup | `SETUP-002`/`003` Rest | informativ | Mit F1/D2 zusammen erledigen; keine optionalen C:-Artefakte übertragen. | `needs_owner` (`:13-14`) |
| E1 | Dokumentation | `OWNER-AI-README` / RV-P1-08 | zwingend | Patch 2 vor jeder breiteren Publikation anwenden. | vorbereitet (`:634-649`) |
| E2 | Dokumentation | XB-0085(c) | bedingt | `AGENTS.md` privat/unveröffentlicht lassen; Versionsentscheidung zurückstellen. | offen (Abschnitt `XB-0086-C2 — final status alignment and publication group 0 definition`, `:950`) |
| E3 | Dokumentation | XB-0085(d) | bedingt | `CMakePresets.json` rein lokal lassen, Backup unangetastet. | offen (`:951`) |
| E4 | Dokumentation | RV-P1-06 / “group 2” | informativ | `.git/info/exclude` Owner-Territorium; “publication group 2” nicht erfinden. | `unknown` (`:613,716`) |
| F1 | Veröffentlichung | `OWNER-PUBLICATION` / XB-0085 | zwingend | Nur vom Owner ausgewählte development-only Dateien publizieren; Zählung klären (§3.1). | `needs_owner` (`:1059,1179`) |
| F2 | Veröffentlichung | PR #1 / Branch | zwingend | PR #1 und Branch vorerst unangetastet; kein Merge ohne Freigabe. | `unknown` (`:959(e)`) |
| F3 | Veröffentlichung | XB-0084 Punkt 6 | zwingend vor Messpublikation | Keine Performance-Werte publizieren; Protokoll präregistrieren. | offen (`RISK:117`) |
| F4 | Veröffentlichung | XB-0084 Punkt 2 | zwingend, rechtlich | Projektlizenz klären; bis dahin `UNLICENSED`, keine Third-Party-Übernahme. | offen (`RISK:113`) |
| F5 | Veröffentlichung | XB-0084 Punkt 7 | zwingend, bedingt | `FINAL_CLEANUP` erst nach Feature-Complete plus separater Freigabe. | offen (`RISK:118`) |
| F6 | Veröffentlichung | `:959(g)` / XB-0080-Historienzeile | zwingend | Historische Ledger-Zeile als Snapshot belassen; Addendum bleibt autoritativ, kein Agenten-Patch. | offen (`:959(g)`) |

**Nicht erneut zu öffnen (`observed`):** `OWNER-X64-VERIFY` (technischer Teil erfüllt; `XB-X64-CORPUS-01` und `XB-CMAKE-VERIFY-RESUME` `STATUS: pass`, `:1001,1292-1336`); VS2026-vs-VS2022-Preset (SETUP-005 `pass`, `:16,88-98,554`); RV-P1-01/02, Code-Mode-Patch-3, `edit:* deny`, XB-P2, XB-0081, XB-0080-A/B, XB-0084-Assessment, RESEARCH-007, XB-0086/C1/C2, RV-P1-C2/C3-C1, XB-CONTINUATION-01-C1/-02-Review und `XB-X64-CORPUS-01`. Diese sind abgeschlossen und werden nicht wieder aufgerollt.

### 2. Die sieben XB-0084-Entscheidungen

| Punkt (`RISK_DEPENDENCY_OVERVIEW.md:112-118`) | Hier |
|---|---|
| 1. Assessment als Evidenzbaseline | B2 |
| 2. Projektlizenz / Third-Party-Adoption | F4 |
| 3. ADRs: Frame Pacing, GPU-Timeline, Resource Lifetime, Shader-Cache, Streaming/Residency, Audio-Clock, deterministisches Scheduling | A3 |
| 4. Native Execution / JIT / Relinker | A4 |
| 5. Optionales Xbox-360-Modul | A5 |
| 6. Performance-Publikation / Protokoll | F3 |
| 7. `FINAL_CLEANUP`-Zeitpunkt | F5 |

### 3. Zu klärende Quellenwidersprüche (nicht eigenmächtig korrigiert)

#### 3.1 Publikationszählung 12/3/10 vs. 13/2/10
`observed` — `:955` und `ROADMAP.md:45(5)` enthalten die Post-CTest-Aktualisierung **13** `READY_FOR_COMMIT` / **2** `NEEDS_REVIEW` / **10** `DO_NOT_PUBLISH` (25 Einträge), während `:1059` noch “12 `READY_FOR_COMMIT`” empfiehlt und `:1179` denselben 25-Eintrags-Stand als “12/3/10 ... applies unchanged” beschreibt. Der Coordinator hat das nicht stillschweigend aufgelöst. Empfehlung in **F1**: 13/2/10 als maßgeblich bestätigen und die älteren Formulierungen als historische Snapshots kennzeichnen; Dateien bleiben bis zur Owner-Entscheidung unverändert.

#### 3.2 Weitere historische Widersprüche
`observed` — `:477` (`XB-0084 in_progress`) ist durch `:524-530,542` superseded; `:872,912` und `ROADMAP.md:28` nennen XB-0080 `planned`, obwohl `:906-914` `pass` vermerkt; `:1090,1117,1121,1223-1231,1350` enthalten `needs_owner`/Blocker-Aussagen aus der Zeit vor dem erfolgreichen absoluten CMake/CTest-Lauf. `R-005` bleibt Capability-Status `unknown`; `partial` ist nur ein Workflow-Vermerk (`RESEARCH_REGISTER.md:37,83-88,115`).

### 4. Entscheidungen im Detail

Schema je Entscheidung: (1) Kennung/Ziel, (2) Status, (3) erledigt, (4) fehlt, (5) Optionen, (6) Auswirkungen, (7) Risiken/Abhängigkeiten, (8) Empfehlung, (9) autonom nach Freigabe.

#### Gruppe A — Architektur und technische Entwicklung

##### A1 — XB-0082: Status-Taxonomie, Write-Set, Tool/CI
1. **Kennung/Ziel:** `OWNER-XB0082-SCOPE`; entscheiden, ob die evaluation-only Konvention reicht oder ein Generator/Tool/CI mit Abhängigkeiten freigegeben wird (`:241-252,1045-1055,1060`).
2. **Status:** `needs_owner` (`:1049`); Paket `planned`; Konvention dokumentiert (`AGENT_WORKFLOW.md:157-174`).
3. **Erledigt:** Konvention mit festen Keys/Tokens und `PROGRESS_UNITS: N/A` (`:1174`); kein Generator, Tool, CI, Dependency oder Prozentanzeige; XB-0080-A/B lieferten geteilte Write-Sets mit Reviewer-`PASS` (`:827-830,908`); Vorbereitungs-Review `PASS` (`:465`).
4. **Fehlt:** Owner-Bestätigung “evaluation-only reicht” oder Freigabe eines eigenen Tool-Pakets.
5. **Optionen:** (a) bestätigen, Tooling deferren; (b) Generator-/Tool-Paket mit eigener Freigabe für Abhängigkeit/CI; (c) installiertem Agenten fehlenden Pfad per `.opencode/**`-Änderung geben — Owner-territorium.
6. **Auswirkungen:** (a) kein Interface-, ADR-, Dependency- oder CI-Effekt; (b) neue Dependency-/CI-Oberfläche; Zähler dürfen nie als Fähigkeit ausgegeben werden (`RESEARCH_REGISTER.md:16-45`, `:449`).
7. **Risiken/Abhängigkeiten:** geteilter Pfad `docs/AGENT_TASKS.md` serialisieren (`:441`); XB-0080-A/B Taxonomie ist Voraussetzung (abgeschlossen).
8. **Empfehlung:** **(a)** — Konvention bestätigen, kein Tool, keine CI, keine neue Abhängigkeit.
9. **Nach Freigabe autonom:** Coordinator dokumentiert die Bestätigung; kein Build/CTest (dokumentations-only); Reviewer liest die Änderung; Abschluss mit `git diff --check` und PASS-Vermerk. Kein Generator wird geschrieben.

##### A2 — XB-X64-STEP-01: öffentliche Single-Step-Fassade
1. **Kennung/Ziel:** `OWNER-X64-STEP`; `[[nodiscard]] X64ExecuteStatus step_x64_instruction(std::span<const std::uint8_t>, X64CpuState&, memory::GuestMemory&) noexcept` als Komposition aus Decoder + Interpreter (`:1185-1199`).
2. **Status:** vorbereitetes Draft-Envelope, `hypothesis` als Design, nicht gestartet (`:1183,1037`).
3. **Erledigt:** Signatur (`:1191`), Acceptance (`:1193`), Testmatrix (`:1194`), Boundaries (`:1199`); `XB-X64-CORPUS-01` liefert echte CTest-Evidenz (`STATUS: pass`, `:992-1001`).
4. **Fehlt:** Owner-Entscheidung öffentliche API **oder** privater Helper **oder** Deferral; privater Helper braucht eigenes Envelope.
5. **Optionen:** (a) öffentliche Fassade gemäß Draft; (b) privater Test-Helper ohne neue public API; (c) defer.
6. **Auswirkungen:** (a) neue öffentliche Header-Funktion in `include/xbridge/cpu/**` — Owner-Gate (`AGENT_WORKFLOW.md:272`, `AGENTS.md:26,63`); keine Änderung an `TestCpu`, `GuestMemory`, Decoder-Repräsentation oder Status-Enums; (b) kein public API, aber `tests/**` betroffen; (c) kein Effekt.
7. **Risiken/Abhängigkeiten:** Fassade ist **nicht** der deferred RIP-Fetch-Bridge (`ROADMAP.md:27,49`); keine FLAGS/SIB/SIMD/Displacement-Semantik; kein JIT; `tests/test_main.cpp` geteilter Pfad mit C1.
8. **Empfehlung:** **(a)** — Envelope freigeben; die Bedingung “nach Testevidenz” ist erfüllt. Architektur-Check und Review bleiben verbindlich.
9. **Nach Freigabe autonom:** Architect Interface-Re-Check; `developer` sole writer für `include/xbridge/cpu/x64_instruction_interpreter.hpp` und `src/cpu/x64_instruction_interpreter.cpp`; `tester` sole writer für `tests/test_main.cpp` (Rechte vor Freigabe gegen `developer.md:9-15`/`tester.md:9-10` prüfen); §5-Verifikation; Reviewer gebündelt.

##### A3 — XB-0084 ADR-Set (Punkt 3)
1. **Kennung/Ziel:** ADRs für Frame Pacing, GPU-Timeline, Resource Lifetime, Shader-Cache-Keys/-Invalidierung, Streaming/Residency, Audio-Clock, deterministisches Scheduling (`RISK:114`).
2. **Status:** offen; XB-0084 hat keine ADRs akzeptiert (`:526,534`).
3. **Erledigt:** Anforderungen/Phasenabhängigkeiten im `XB-0084_PRIORITIZED_DEVELOPMENT_PLAN.md:59-66,95-129`; ADR-0005/0006/0007/0010/0011 bestehen (`DECISIONS.md:23-37,48-56`).
4. **Fehlt:** Owner-Entscheidung je ADR; keine pauschale Sammelannahme vorhanden.
5. **Optionen:** (a) einzelne ADRs mit Evidenz freigeben; (b) alle sieben jetzt akzeptieren; (c) defer bis konkrete Evidenz/Backend-Verträge vorliegen.
6. **Auswirkungen:** ADRs legen Verträge für Grafik-, Shader-, Ressourcen-, Audio- und Integrationsphasen fest und ändern spätere Schnittstellen/Testmatrizen; Agenten dürfen ADRs nicht selbst akzeptieren (`AGENT_WORKFLOW.md:272`, `RESEARCH_REGISTER.md:9-10`).
7. **Risiken/Abhängigkeiten:** `hypothesis` — ohne Evidenz würden Metriken oder Backend-Verhalten erfunden (`RISK:37-38,50-51`); abhängig von B1/B2 und der Kette `RISK:57-76`.
8. **Empfehlung:** **(c)** — zurückstellen; jedes ADR erhält später ein eigenes Paket mit Evidenz, Architekturprüfung und Owner-Freigabe.
9. **Nach Freigabe autonom:** nur Ledger-/Roadmap-Vermerk; keine Implementierung. Ein eventuales ADR-Paket wird nicht aus dieser Vorlage abgeleitet.

##### A4 — Native Execution / JIT / Relinker (Punkt 4)
1. **Kennung/Ziel:** entscheiden, ob eine JIT-/Native-Untersuchung jemals erlaubt wird (`RISK:115`).
2. **Status:** offen; aktuelle Regel verbietet JIT ohne Evidenz, Design, Tests und Freigabe (`ARCHITECTURE.md:76-95,209-213`).
3. **Erledigt:** Interpreter ist dokumentierte Referenzimplementierung (`ARCHITECTURE.md:141-168`); Klassifizierung Klasse 3/4 mit Owner-Gate (`:384`); keine JIT-Datei, kein Pfadwechsel.
4. **Fehlt:** Policy-Entscheidung (dauerhaftes Verbot vs. qualifizierte spätere Untersuchung).
5. **Optionen:** (a) Interpreter-only beibehalten; (b) spätere Untersuchung nur mit eigenem ADR, Design, Tests, Paket; (c) JIT jetzt freigeben — widerspricht dokumentierter Architektur und ist nicht vorbereitet.
6. **Auswirkungen:** (a) sichere Baseline; (b) neuer ADR-/Schnittstellen-/Testkomplex, kein Laufzeitpfadwechsel ohne Safe-Point-Design (`HOST_HARDWARE_POLICY.md:20-28`); (c) hohes Risiko für Guest-Semantik.
7. **Risiken/Abhängigkeiten:** hängt an A3 und B2; `hypothesis` — ein JIT-Start würde die dokumentierte Reihenfolge überspringen (`RISK:75`).
8. **Empfehlung:** **(a)** — weiterhin Interpreter-only; spätere Prüfung nur als **(b)** mit vollständig neuem Paket und Owner-ADR.
9. **Nach Freigabe autonom:** nur Ledger-Vermerk; keine Codeänderung. Ein JIT-Paket wird nicht hieraus abgeleitet.

##### A5 — Optionales Xbox-360-Modul (Punkt 5)
1. **Kennung/Ziel:** entscheiden, ob Xbox 360 eigener Modulscope wird (`RISK:116`).
2. **Status:** offen; ADR-0004 nennt Xbox 360 deferred/optional/isolated (`DECISIONS.md:18-21`; `ARCHITECTURE.md:11-13`).
3. **Erledigt:** SKU-Trennung `BWV6WF6XMFZX` (Original Xbox) vs. `BPQ8RMCDSLRL` (Xbox 360) dokumentiert (`RISK:42`; `:530`); kein Modul, kein Profil, keine Kompatibilitätsbehauptung.
4. **Fehlt:** Owner-Entscheidung über optionalen Scope und eigene ADR-/Evidenz-/Testgrenze.
5. **Optionen:** (a) deferred lassen, ADR-0004 unverändert; (b) isoliertes Zukunftspaket mit separatem ADR; (c) Xbox 360 jetzt als Ziel — ausgeschlossen durch ADR-0004.
6. **Auswirkungen:** (a) kein Scope-Effekt; (b) eigener Guest-Layer, keine Übertragung von Series-X-Ergebnissen; (c) ADR-Änderung und Zielumfangserweiterung, die diese Vorlage nicht vornimmt.
7. **Risiken/Abhängigkeiten:** falsche SKU-Evidenz erzeugt falsche Kompatibilitätsaussagen (`RISK:42-43`); letzter Schritt der Kette `RISK:57-72`.
8. **Empfehlung:** **(a)** — Xbox 360 bleibt deferred; keine ADR-Änderung.
9. **Nach Freigabe autonom:** Ledger-Vermerk; keine neuen Pakete.

#### Gruppe B — Forschung und Evaluation

##### B1 — XB-0083: autoritative Re-Verifikation, R-001…R-006
1. **Kennung/Ziel:** `OWNER-XB0083-START`; projektautorene CPU/Grafik-Behauptungen (Zen 2, AVX2, Windows-x64-ABI, D3D12.x, Shader/Mesh-Formate) gegen autoritative Microsoft-/AMD-Quellen prüfen (`:254-265`).
2. **Status:** `planned`, nicht gestartet (`:1033,1061`); R-001…R-006 `unknown`/`conformant`, kein Paket zugeordnet (`RESEARCH_REGISTER.md:111-116`; `:1175`).
3. **Erledigt:** vollständiges Envelope (`:254-265`); Register-Zeilen konform; read-only Researcher bestätigte die sechs Legacy-Zeilen; RESEARCH-007 und XB-0084 `pass`.
4. **Fehlt:** Owner-Startfreigabe mit exakten URLs und Zugriffsdaten; frische Webwahrheit bleibt `unknown`.
5. **Optionen:** (a) scoped read-only Researcher-Paket freigeben; (b) defer.
6. **Auswirkungen:** nur Evidenzqualität; keine Produkt-/ADR-/Interface-Änderung ohne separaten Owner-Schritt; nicht verifizierte Punkte bleiben `unknown` (`:261`).
7. **Risiken/Abhängigkeiten:** andere-Konsolen-Evidenz darf nicht als Xbox-Fakt gelten (`RESEARCH_REGISTER.md:12-14`); keine Codeübernahme; `docs/RESEARCH_REGISTER.md` wird vom Coordinator serialisiert.
8. **Empfehlung:** **(a)** — Paket mit fester URL-/Datumsliste freigeben; `unknown` bleibt `unknown`.
9. **Nach Freigabe autonom:** `researcher` sole writer für `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md` und `docs/RESEARCH_REGISTER.md` (Rechte `researcher.md:9-13` vor Freigabe gegenlesen); Coordinator nur Ledger; `architect` nur bei Architekturberührung; Reviewer `PASS`; kein Build/CTest. Abschluss: konforme Registerfelder, korrekte Labels, `git diff --check` EXIT=0.

##### B2 — XB-0084 Assessment als Evidenzbaseline (Punkt 1)
1. **Kennung/Ziel:** XB-0084 als Baseline akzeptieren oder ablehnen (`RISK:112`).
2. **Status:** Assessment `pass`/Final-Reviewer-`PASS`, Implementierung nicht freigegeben (`:526,542`; `ROADMAP.md:29,37`).
3. **Erledigt:** sechs Dokumente, 11 Acceptance-Kriterien, Review `NEEDS_CHANGE` → C1 → `PASS` (`:522-530`); 32 Einträge (26+6) erhalten, `git diff --check` EXIT=0 (`:528`).
4. **Fehlt:** explizite Owner-Annahme als *Evidenzbaseline*, verschieden von Implementierungsfreigabe.
5. **Optionen:** (a) akzeptieren; (b) ablehnen und neue Evidenz verlangen; (c) als Implementierungsfreigabe missverstehen — nicht gedeckt.
6. **Auswirkungen:** (a) erlaubt weitere Planung, kein Implementierungspaket; (b) Planung verliert ihre Grundlage; (c) überspringt ADR/Tests/Evidence.
7. **Risiken/Abhängigkeiten:** `hypothesis` — jeder Pfeil der Kette `RISK:57-76` braucht ein eigenes Paket; GTA-VI-Interna, FPS und D3D12.X bleiben `unknown` (`RISK:120-129`).
8. **Empfehlung:** **(a)** — als Evidenzbaseline akzeptieren, ohne Implementierungs-, ADR- oder Kompatibilitätsfreigabe.
9. **Nach Freigabe autonom:** Ledger-/Roadmap-Vermerk; keine Implementierungspakete hieraus ableiten.

#### Gruppe C — Host- und Hardware-Funktionen

##### C1 — XB-HOST-CAP-01: deterministischer Capability-Provider
1. **Kennung/Ziel:** `OWNER-HOST-CAP`; `ICapabilityProvider`, `FixedCapabilityProvider`, `select_execution_plan`-Overload ohne echtes Probing (`:1201-1215`).
2. **Status:** vollständiges Draft-Envelope, nicht gestartet (`:1038,1183`).
3. **Erledigt:** Acceptance (`:1209`), Testwiederverwendung (`:1210`), Reviewpflicht (`:1211`), ADR-Basis ADR-0005/0010/0011 (`:1215`); keine echten Detektoren (`ADAPTIVE_EXECUTION.md:36-38`).
4. **Fehlt:** Owner-Freigabe des Provider-Interfaces **und** getrennte Entscheidung über späteres Windows-Probing.
5. **Optionen:** (a) nur deterministischen Seam freigeben; (b) Seam + Probe-Scope jetzt; (c) defer.
6. **Auswirkungen:** (a) neue öffentliche Host-Header, bestehender Overload und Report unverändert; (b) Win32/DXGI/Vulkan-Einstiegspunkte `unknown`, neue Abhängigkeiten und Host-/Guest-Grenzen (`:1208,1215`); (c) kein Effekt.
7. **Risiken/Abhängigkeiten:** ADR-0005/0008/0010/0011 schützen Host-/Guest-Trennung (`DECISIONS.md:23-41,48-56`); kein Runtime-Pfadwechsel; `tests/test_main.cpp` geteilter Pfad mit A2.
8. **Empfehlung:** **(a)** — Provider-Seam freigeben; echtes Probing bleibt eigene spätere Owner-Entscheidung.
9. **Nach Freigabe autonom:** Architect Interface-/ADR-/Host-Guest-Check; `developer` sole writer für `include/xbridge/host/capabilities.hpp`, `include/xbridge/host/execution_policy.hpp`, `src/host/execution_policy.cpp`; `tester` sole writer für `tests/test_main.cpp` (Rechte vor Freigabe prüfen); §5-Verifikation; Reviewer gebündelt. Abschluss nur bei identischem Plan/Report zum Direkt-Overload und PASS.

#### Gruppe D — Setup und Automatisierung

##### D1 — SETUP-004: Sitzungsübertragung
1. **Kennung/Ziel:** 13 OpenCode-Sitzungen über `session export/import` übertragen (drei Projekt-, zehn Setup-Sitzungen) (`:15,142-153`).
2. **Status:** `needs_owner`, nicht abgeschlossen.
3. **Erledigt:** Envelope mit `OWNER_APPROVALS: Already granted` (`:153`); keine Agenten-Shell `opencode *`, kein Code-Mode-Tool (`:76-77`); portable State leer (`:99`).
4. **Fehlt:** Owner führt die supported Commands manuell aus und meldet echte Resultate.
5. **Optionen:** (a) Owner führt Export/Import aus; (b) dauerhaft verzichten; (c) Kopie einer aktiven SQLite-Datei — ausgeschlossen.
6. **Auswirkungen:** nur Portabilität/Verlauf; kein Produkt-, ADR- oder Interface-Effekt.
7. **Risiken/Abhängigkeiten:** Originale auf C: nie verändern; keine aktive DB direkt kopieren (`AGENT_WORKFLOW.md:186-190`).
8. **Empfehlung:** **(a)** — Owner führt die Befehle aus; Agents dokumentieren nur gemeldete Ergebnisse.
9. **Nach Freigabe autonom:** Coordinator schreibt den gemeldeten Outcome ins Ledger; Reviewer prüft, dass nichts Ungemeldetes behauptet wird; kein Build/CTest.

##### D2 — SETUP-006 / XB-0085(f): Worktree-Rest
1. **Kennung/Ziel:** Entscheidung über nicht registriertes Checkout `portability-worktree-test` und weitere Öffnungsversuche (`:17,80-82,168-179,959(f)`).
2. **Status:** `blocked`.
3. **Erledigt:** `worktree_create` → `Created worktree could not be opened`; `worktree_list` unverändert; vollständiger Checkout vorhanden; keine Löschung, kein Konfigurations-Workaround (`:80-82,101,174`).
4. **Fehlt:** Owner-Entscheidung behalten vs. entfernen und ob ein weiterer Versuch erlaubt ist.
5. **Optionen:** (a) behalten, keine weiteren Versuche; (b) Bereinigung über supportete Funktion nach Freigabe; (c) erneuter Versuch mit separater Freigabe.
6. **Auswirkungen:** rein lokaler Setup-Zustand; kein Produkt-/Publikationseffekt; kein automatisches Entfernen durch Agents.
7. **Risiken/Abhängigkeiten:** nur OpenCode-Worktree-Funktionen, nie Git-Shell (`GITHUB_AGENT_WORKFLOW.md:29-35`); `worktree_remove` historisch nicht im aktuellen Tool-Set — `unknown`.
8. **Empfehlung:** **(a)** — vorerst behalten, keine spekulativen Änderungen.
9. **Nach Freigabe autonom:** Ledger-Eintrag; keine Agenten-Bereinigung ohne ausdrücklichen Auftrag.

##### D3 — SETUP-007: Spezialisteninstallation / Berechtigungen
1. **Kennung/Ziel:** neun Spezialrollen installieren oder mit sechs installierten Rollen weiterarbeiten (`:18,181-192,272-288,1065`).
2. **Status:** `blocked`; Reviewer `NEEDS_CHANGE` (`:137`); sechs Dateien installiert (`:272`).
3. **Erledigt:** alle neun Spezialisten `Unknown agent`; sechs Schreibversuche `Permission denied: edit` (`:105,278-286`); XB-0081 korrigierte Doku-Konflikte (`:117,133`); `test-engineer` bleibt `planned`.
4. **Fehlt:** Owner persistiert minimale Regeln in `coordinator.md` **oder** entscheidet endgültig, keine Spezialisten zu installieren; SETUP-007-Reviewfindings bleiben sichtbar.
5. **Optionen:** (a) sechs Rollen behalten, Arbeit mit installierten Substituten paketieren; (b) gezielt einzelne Spezialisten mit Owner-persistierten Regeln installieren; (c) alle neun — derzeit nicht durchführbar.
6. **Auswirkungen:** (b) würde Write-Scopes `src/cpu/**`, `src/memory/**`, `src/host/**` und Test-Ownership verschieben (`AGENT_WORKFLOW.md:231-243`); (a) ändert nichts an Rechten, verlangt aber pro Paket explizite Writer-Benennung.
7. **Risiken/Abhängigkeiten:**Konversations-Freigabe erzeugt keine Berechtigung (`:103-106`); blockiert den RIP-Single-Step-Meilenstein (`ROADMAP.md:27`) — A2 löst ihn mit installiertem `developer`.
8. **Empfehlung:** **(a)** — keine Spezialisten installieren; `test-engineer` bleibt `planned`; Pakete mit installierten Writern ausführen.
9. **Nach Freigabe autonom:** Coordinator benennt pro Paket genau einen Writer je Pfad und prüft effektive Rechte vor Freigabe; kein `edit` an `.opencode/**`.

##### D4 — SETUP-002/003 Restpunkte (informativ)
1. **Kennung/Ziel:** unversionierte Änderungen prüfen, optionale C:-Artefakte und ursprünglichen Setup-Worktree klären (`:13-14`).
2. **Status:** `needs_owner` (historische Setup-Zeilen), VS2026-Teil `pass`.
3. **Erledigt:** Portable Setup übertragen und validiert (`:40-60`); Speicherinventar vollständig, nichts kopiert (`:62-64`); VS2026-Frage geschlossen (`:16,554`).
4. **Fehlt:** Owner-Review der lokalen Änderungen, Entscheidung zu optionalen Artefakten und zum ursprünglichen Worktree.
5. **Optionen:** (a) mit F1/D2 zusammen erledigen, keine C:-Artefakte übertragen; (b) alles separat entscheiden.
6. **Auswirkungen:** rein lokal; keine Produkt-/Publikationswirkung.
7. **Risiken/Abhängigkeiten:** überschneidet sich mit F1 und D2; nicht doppelt entscheiden.
8. **Empfehlung:** **(a)** — in F1/D2 aufgehen lassen; keine eigenen Pakete.
9. **Nach Freigabe autonom:** nur Ledger-Verweise; keine Dateioperationen durch Agents.

#### Gruppe E — Dokumentation

##### E1 — AI README Patch 2 (`OWNER-AI-README` / RV-P1-08)
1. **Kennung/Ziel:** `ai/README.md:13,28` von “Open a pull request” auf Owner-Handoff umstellen (`:634-649,1066`).
2. **Status:** Patch vorbereitet, nicht angewendet; Datei `DO_NOT_PUBLISH` (`:952,1178`).
3. **Erledigt:** exakter Patch vorhanden (`:636-648`); RV-P1-08 dokumentiert (`:613,716`); mehrfach neu gelesen, unverändert (`:1178`).
4. **Fehlt:** Owner wendet an oder lehnt dauerhaft ab.
5. **Optionen:** (a) anwenden; (b) nicht anwenden und Datei von Publikation ausschließen.
6. **Auswirkungen:** (a) Dokumentation stimmt mit `GITHUB_AGENT_WORKFLOW.md:7-9,49` überein; kein Produkt-/ADR-Effekt; (b) Widerspruch bleibt lokal sichtbar.
7. **Risiken/Abhängigkeiten:** `ai/**` Owner-Territorium (`GITHUB_AGENT_WORKFLOW.md:59`); Agenten dürfen nicht editieren.
8. **Empfehlung:** **(a)** — vor jeder breiteren Publikation anwenden.
9. **Nach Freigabe autonom:** Owner wendet Patch an; Reviewer prüft angewendeten Zustand read-only; Coordinator aktualisiert Ledger; kein Build/CTest.

##### E2 — `AGENTS.md` dauerhafte Versionierung (XB-0085(c))
1. **Kennung/Ziel:** klären, ob `AGENTS.md` dauerhaft versioniert/publiziert wird (Abschnitt `XB-0086-C2 — final status alignment and publication group 0 definition`, `:950`, `:959(c)`).
2. **Status:** offen/`unknown`; von publication group 0 ausgeschlossen.
3. **Erledigt:** group 0 exakt fünf Dateien, Reviewer `PASS` bestätigt “no extras” (Abschnitt `XB-0086-C2 — final status alignment and publication group 0 definition`, `:594`); `AGENTS.md` `DO_NOT_PUBLISH` (`:950`).
4. **Fehlt:** Owner-Entscheidung zur dauerhaften Versionierung; Optionen in keiner Quelle enumeriert.
5. **Optionen:** (a) privat/unveröffentlicht lassen; (b) publizieren — Umfang, Produktpaketbedeutung und Vereinbarkeit mit Infrastruktur-Ausschluss `unknown`; nur Owner.
6. **Auswirkungen:** (a) erhält Trennung private Agentregeln vs. Produktpaket; (b) würde Autoritätskette und Paketdefinition berühren.
7. **Risiken/Abhängigkeiten:** `AGENTS.md` shared/coordinator-serialisiert; keine Agentenänderung ohne Owner.
8. **Empfehlung:** **(a)** — `AGENTS.md` bleibt privat und unveröffentlicht; Versionsentscheidung zurückstellen.
9. **Nach Freigabe autonom:** nur Ledger-/Roadmap-Vermerk; keine Dateiänderung.

##### E3 — `CMakePresets.json` lokale Änderung und Backup (XB-0085(d))
1. **Kennung/Ziel:** Behandlung der maschinenlokalen VS2026-Änderung und des ausgeschlossenen Backups (`:951,959(d)`).
2. **Status:** VS2026-Änderung selbst `pass` (`:16,88-98,554`); Treatment offen.
3. **Erledigt:** absolute VS2026-Executables verifiziert, Configure/Build/CTest EXIT=0 (`:1292-1323`); Backup unangetastet (`.owner-patch-backup/`); CRLF-Warnung bekannt.
4. **Fehlt:** Owner entscheidet lokal vs. normalisiert vs. publiziert.
5. **Optionen:** (a) lokal lassen, `DO_NOT_PUBLISH`; (b) publizieren/normalisieren — `unknown`, Owner-only; kein Agent hat Edit-Recht (`AGENT_WORKFLOW.md:192-196`).
6. **Auswirkungen:** (a) kein Produkt-/API-/ADR-Effekt; (b) Maschinenpfade könnten ins Repository gelangen; CRLF-/Backup-Policy ungeklärt.
7. **Risiken/Abhängigkeiten:** VS2026-vs-VS2022 ist **geschlossen** und wird nicht erneut aufgerollt; `ninja-debug`-Cache veraltet, aber out of scope (`ROADMAP.md:43`).
8. **Empfehlung:** **(a)** — rein lokal, Backup unangetastet, nicht publizieren.
9. **Nach Freigabe autonom:** Ledger-Vermerk; Tester nur die bereits erlaubten absoluten Commands; keine Preset-Änderung durch Agents.

##### E4 — RV-P1-06 / “publication group 2” (informativ)
1. **Kennung/Ziel:** `.git/info/exclude`-Nebeneffekt und Bezeichnung “publication group 2” (`:613,716`).
2. **Status:** `unknown`/informativ; RV-P1-C2 selbst abgeschlossen.
3. **Erledigt:** `/.env.*`-Regel bewusst nicht vorgeschlagen, weil sie `.env.example` mitdeny würde (`:673`); `XB-P2` als Package 2 freigegeben und mit `PASS` abgeschlossen (`:798,819`).
4. **Fehlt:** keine Definition von “publication group 2” gefunden; `.git/info/exclude` für Agents nicht lesbar (harter Deny).
5. **Optionen:** (a) als historischen Hinweis vermerken, keine Definition erfinden; (b) Owner definiert Gruppe 2 separat.
6. **Auswirkungen:** (a) keine; (b) nur Publikationsgruppierung.
7. **Risiken/Abhängigkeiten:** Branchname `publication-group-2` ist kein Gruppen-Envelope (`GITHUB_AGENT_WORKFLOW.md:19`).
8. **Empfehlung:** **(a)** — informativ führen, keine neue Gruppe erfinden.
9. **Nach Freigabe autonom:** lediglich Hinweis im Ledger; keine Aktion.

#### Gruppe F — Veröffentlichung

##### F1 — XB-0085 Publikationsaudit: Dateien/Gruppen und Zählung
1. **Kennung/Ziel:** `OWNER-PUBLICATION`; festlegen, welche Dateien/Gruppen gestaget, committet und publiziert werden (`:916-961,1059`; `ROADMAP.md:45(5)`).
2. **Status:** Audit `pass`, Readiness `needs_owner`; 25 Einträge aktuell.
3. **Erledigt:** Labels definiert (`:935`); group 0 exakt fünf Dateien (Abschnitt `XB-0086-C2 — final status alignment and publication group 0 definition`); empfohlene Reihenfolge (`:957`); Reviews für `.gitignore`, Workflow, Register, Roadmap, P2-Docs, XB-0080-A/B und Testpaket; Post-CTest `tests/test_main.cpp` `READY_FOR_COMMIT` (`:948`); `git diff --check` EXIT=0 (nur CRLF-Warnung).
4. **Fehlt:** Owner-Auswahl der Gruppen; Klärung §3.1; PR #1 (F2).
5. **Optionen:** (a) nur vom Owner ausgewählte READY_FOR_COMMIT development-only Dateien; (b) nichts oder engere Gruppe; (c) mehr nach gezieltem Review (`docs/AGENT_TASKS.md` und `docs/ARCHITECTURE.md` brauchen eigene Verdicts); (d) Konfiguration/Maschinenlokales publizieren — widerspricht `AGENT_WORKFLOW.md:78-90,100`.
6. **Auswirkungen:** irreversible externe Zustände; keine Produkt-, ADR-, Interface- oder Kompatibilitätsaussage; `PUBLICATION_STATUS: local_only` bleibt bis Owner-Handlung.
7. **Risiken/Abhängigkeiten:** Ledger zuerst reviewed, da es Verdicts anderer Dateien trägt (`:957`); `ai/README.md` vor breiter Publikation patchen (E1); Agents leaden/staging/committen nie (`GITHUB_AGENT_WORKFLOW.md:7-9`).
8. **Empfehlung:** **(a)** — group 0 zuerst, danach das nach Review freigegebene Ledger, `docs/ARCHITECTURE.md` nur nach gezieltem Review; `DO_NOT_PUBLISH`-Set vollständig ausschließen; 13/2/10 als maßgeblich bestätigen (§3.1).
9. **Nach Freigabe autonom:** **keine** Agenten-Publikation. Agents bereiten höchstens eine vom Owner genehmigte Dateiliste vor und dokumentieren die ausgeführte Owner-Handlung; Reviewer read-only; Abschluss erst mit vom Owner gemeldeten Resultaten.

##### F2 — PR #1, Branch-Strategie, Merge nach `main`
1. **Kennung/Ziel:** `OWNER-PUBLICATION` Teil (e): PR-Status, Branchstrategie und Merge (`:959(e),1059`; `GITHUB_AGENT_WORKFLOW.md:13-19`).
2. **Status:** `unknown` (Agents prüfen keine PRs); Branch `docs/opencode-publication-group-2` Arbeitsstand, keine Freigabe.
3. **Erledigt:** keine Agenten-PR-/Merge-/Branchaktion; historisch PR #1 offen, 29-Commit-Divergenz nicht aktuell verifiziert (`:543,552`).
4. **Fehlt:** Owner-verifizierter PR-Zustand und Merge-Entscheidung.
5. **Optionen:** (a) unverändert lassen, kein Merge; (b) PR-Check und Entscheidung durch Owner; (c) Merge/Branchwechsel durch Agents — ausgeschlossen (harte Denys).
6. **Auswirkungen:** (a) lokaler Stand bleibt; (b) irreversible externe Zustände; (c) Policy-Verstoß.
7. **Risiken/Abhängigkeiten:** hängt an F1 und E1; Branchwechsel/Reset/Merge benötigen Owner-Entscheidung (`GITHUB_AGENT_WORKFLOW.md:17`).
8. **Empfehlung:** **(a)** — PR #1 und Branch vorerst unangetastet; jede Änderung nur nach Owner-Check.
9. **Nach Freigabe autonom:** Owner führt alle Git/GitHub-Aktionen aus; Agents dokumentieren nur, keine Statusbehauptung über ungeprüfte PRs.

##### F3 — Performance-Publikationsprotokoll (XB-0084 Punkt 6)
1. **Kennung/Ziel:** entscheiden, ob und unter welchem präregistrierten Protokoll Performance-Ergebnisse publiziert werden (`RISK:117`).
2. **Status:** offen; keine Messwerte vorhanden.
3. **Erledigt:** “30 FPS” ist owner-desired Target, keine Messung (`RISK:102`); GTA-VI-Anforderung und XBridge-Erreichbarkeit `unknown`/`hypothesis`; Methodikmetriken und Präregistrierungspflicht dokumentiert (`RISK:100-106`).
4. **Fehlt:** Owner-Entscheidung über Publikationserlaubnis und Protokoll.
5. **Optionen:** (a) keine Publikation, Protokoll vor jeder Messung präregistrieren; (b) Publikation nach festem Protokoll; (c) Werte jetzt veröffentlichen — nicht möglich (`unknown`).
6. **Auswirkungen:** (a) keine irreführenden Claims; (b) braucht Warm-up, N, Seeds, Perzentilalgorithmus, Frame-Grenze, Refresh, Stutter-Schwelle, Ausschlüsse, Instrumentation-Overhead (`RISK:105-106`); Zähler nie als Fähigkeit (`:51`).
7. **Risiken/Abhängigkeiten:** abhängig von B2/A3 und echter reproduzierbarer Telemetrie; kein Frame-Loop/Backend/lawful Workload vorhanden (`RISK:103`).
8. **Empfehlung:** **(a)** — keine Performance-Werte publizieren; Protokoll vor der ersten Messung präregistrieren.
9. **Nach Freigabe autonom:** kein Messlauf hieraus; ein späteres Messpaket braucht eigenen Envelope, Tester-Verifikation und Reviewer-`PASS`.

##### F4 — Projektlizenz und Third-Party-Adoption (XB-0084 Punkt 2)
1. **Kennung/Ziel:** Projektlizenz wählen und Third-Party-Übernahmepolitik festlegen (`RISK:113`).
2. **Status:** offen; Repository `UNLICENSED` (`LEGAL_COMPLIANCE.md:61-69`; `RISK:41`).
3. **Erledigt:** RESEARCH-007 Lizenz `observed`, rechtliche Wirkung `unknown` (`RISK:39-40`); keinerlei Code/Text/Artefakt übernommen; ADR-0009 verbietet proprietäre Inhalte (`DECISIONS.md:43-46`).
4. **Fehlt:** Owner-/Rechtsentscheidung zu Lizenz und Adoption Policy.
5. **Optionen:** (a) `UNLICENSED` beibehalten, keine Adoption; (b) konkrete Lizenz wählen und File-/Commit-/Dependency-Audit erlauben; (c) Code jetzt übernehmen — rechtlich ungeklärt.
6. **Auswirkungen:** (a) Publikations-/Verteilungsrechte bleiben nicht angenommen; (b) neue Legal-/Compliance-Oberfläche, ADR und Audit nötig; (c) Verstoß gegen `AGENTS.md` Evidence-/Legalregeln.
7. **Risiken/Abhängigkeiten:** harte Gates: keine Firmware, Keys, Spieldateien, DRM-Umgehung, unlizenzierter Code (`AGENTS.md:39-45`, `RESEARCH_REGISTER.md:11`, `RISK:16`).
8. **Empfehlung:** **(a)** — `UNLICENSED` beibehalten, keine Third-Party-Übernahme bis separater Owner-/Rechtsfreigabe.
9. **Nach Freigabe autonom:** nur Ledger-Vermerk; keine Codeübernahme, kein Dependency-File, keine Publikation hieraus.

##### F5 — `FINAL_CLEANUP`-Zeitpunkt (XB-0084 Punkt 7)
1. **Kennung/Ziel:** entscheiden, ob und wann `FINAL_CLEANUP` zulässig ist (`RISK:118`).
2. **Status:** offen; nicht Teil von XB-0084.
3. **Erledigt:** Regel dokumentiert: erst nach Feature-Complete **und** expliziter Owner-Freigabe; gefilterter Produktbaum, keine Löschung von Agentkonfiguration/Dokumentation (`AGENTS.md` §10, `AGENT_WORKFLOW.md:76-90`, `RISK:76`).
4. **Fehlt:** Feature-Complete-Status und separate Owner-Freigabe — beides nicht gegeben.
5. **Optionen:** (a) zurückstellen; (b) Planung ohne Filtern als eigenes Doku-Paket; (c) jetzt ausführen — ausgeschlossen.
6. **Auswirkungen:** (a) Entwicklungsinfrastruktur bleibt vollständig; (b) nur Doku-Aufwand; (c) unzulässige Bereinigung.
7. **Risiken/Abhängigkeiten:** hängt an B2/A3/A4/A5/F4 und Feature-Complete; kein `FINAL_CLEANUP` aus XB-0084 ableiten.
8. **Empfehlung:** **(a)** — zurückstellen.
9. **Nach Freigabe autonom:** kein Cleanup; Ledger-Vermerk genügt.

##### F6 — Historische XB-0080-Ledgerzeile: separater Owner-Patch? (`:959(g)`)
1. **Kennung/Ziel:** `:959(g)`; klären, ob die historische Tabellezeile zu XB-0080 im Ledger (`:9-20`, `planned`/`blocked`-Snapshot) selbst einen separaten Owner-Patch braucht. Das Roadmap-Seitige wurde von C1 korrigiert (`:35,43`); die Ledgerzeile blieb bewusst als historischer Snapshot, um `file:line`-Anker stabil zu halten (`:536-544`). Nicht zu verwechseln mit dem abgeschlossenen Paket XB-0080-A/B selbst (`:906-914`, `pass`) — dieses wird hier **nicht** wieder geöffnet.
2. **Status:** offen/`needs_owner` (`:959(g)`); das Addendum `:536-544` ist dokumentiert als aktuell autoritativ für Status von XB-0084/0085/0086 und hält die früheren Zeilen als Geschichte.
3. **Erledigt:** Roadmap-Text korrigiert (`:35,43`); terminales Status-Addendum verzeichnet XB-0080-A/B als `pass` (`:906-914`); Final-Reviewer-`PASS` (`:910`); `XB-0084`-Addendum-Regel zum Erhalt der Anker (`:536`). Kein Agent hat die historische Zeile geändert.
4. **Fehlt:** explizite Owner-Entscheidung, ob die historische Zeile unverändert bleibt oder gepatcht wird.
5. **Optionen:** (a) als historischen Snapshot belassen, Addendum bleibt autoritativ; (b) separaten Owner-Patch auf die Zeile anwenden; (c) Owner-defined alternative.
6. **Auswirkungen:** (a) keine Statusänderung, Anker bleiben stabil, Ledger bleibt publizierbar nach Review; (b) ändert einen historischen Anker und damit die Stabilität älterer `file:line`-Zitate — Publikationsrelevanz, da das Ledger vor anderen Dateien reviewed wird (`:957`); (c) nach Owner-Vorgabe.
7. **Risiken/Abhängigkeiten:** hängt an F1 (Publikationsauswahl und Ledger-Review); Patchen der Zeile würde die in dieser Vorlage und in XB-0084/0085/0086 verwendeten Anker verschieben — deshalb nur durch Owner.
8. **Empfehlung:** **(a)** — Zeile als historischen Snapshot belassen; Addendum `:536-544` bleibt die autoritative aktuelle Statusquelle. Kein Agenten-Patch.
9. **Nach Freigabe autonom:** Coordinator vermerkt die Owner-Entscheidung im Ledger; kein Patch der historischen Zeile durch Agents; Reviewer prüft nur read-only, dass kein Anker verschoben wurde; `git diff --check`.

### 5. Autonomie nach Owner-Freigabe — Umsetzungsplan

Dies ist die vorbereitete Umsetzung, **nicht** deren Freigabe. Writer-Zuweisungen sind vor Paketstart erneut gegen effektive Rechte zu prüfen (`developer.md:9-15`, `tester.md:9-10`, `researcher.md:9-13`; `architect.md`/`reviewer.md` `edit: * = deny`). Rechte werden nicht erweitert, Denials nicht umgangen.

#### 5.1 Abhängigkeiten und Wellen

```text
Owner-Freigabe (dieser Abschnitt)
  ├─ Welle 1a: A2 (XB-X64-STEP-01)   [developer: product; tester: tests]
  ├─ Welle 1b: B1 (XB-0083)          [researcher: docs/research + register]
  ├─ Welle 1c: D1/D2/D3/F1/F2        [Owner führt aus; Agents nur Ledger]
  ├─ Welle 2:  C1 (XB-HOST-CAP-01)   [developer: host; tester: tests] — nach A2
  ├─ Welle 2:  A1, B2, E1..E4, D4, F3..F5 [Doku/Ledger, serialisiert]
  └─ Welle 3:  A3/A4/A5              [nur nach eigener ADR-/Evidenzfreigabe]
```

`observed` — A2 und C1 schreiben beide `tests/test_main.cpp` und dürfen **nicht** parallel laufen. B1 schreibt `docs/RESEARCH_REGISTER.md` und `docs/research/**`; der Coordinator schreibt `docs/AGENT_TASKS.md`/`docs/ROADMAP.md`. Alle Ledger-/Roadmap-Schreibzugriffe sind coordinator-serialisiert (`AGENT_WORKFLOW.md` Parallel change rules). `architect` und `reviewer` sind read-only und können parallel prüfen.

#### 5.2 Agentenzuweisung (installed roles only)

| Paket | Writer (exact) | Read-only Prüfer |
|---|---|---|
| A2 `XB-X64-STEP-01` | `developer`: `include/xbridge/cpu/x64_instruction_interpreter.hpp`, `src/cpu/x64_instruction_interpreter.cpp`; `tester`: `tests/test_main.cpp` | `architect` (Interface), `tester` (Verifikation), `reviewer` (final) |
| B1 `XB-0083` | `researcher`: `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md`, `docs/RESEARCH_REGISTER.md`; `coordinator`: Ledger | `architect` nur bei Architekturberührung, `reviewer` |
| C1 `XB-HOST-CAP-01` | `developer`: `include/xbridge/host/capabilities.hpp`, `include/xbridge/host/execution_policy.hpp`, `src/host/execution_policy.cpp`; `tester`: `tests/test_main.cpp` | `architect` (Interface/ADR/Host-Guest), `tester`, `reviewer` |
| A1, B2, D4, E2, E3, E4, F3, F4, F5 | `coordinator`: nur `docs/AGENT_TASKS.md` / `docs/ROADMAP.md` | `reviewer` (read-only Doku-Prüfung) |
| E1, F1, F2, D1, D2, D3 | **kein Agent-Writer**; Owner appliziert/entscheidet; `coordinator` dokumentiert nur gemeldete Resultate | `reviewer` prüft Ehrlichkeit der Aufzeichnung |
| A3, A4, A5 | `coordinator`: nur Ledger-Vermerk der Owner-Policy; keine ADR-Schreibung ohne neues Paket | `reviewer` |

Kein nicht installierter Spezialist wird als Writer eingesetzt; kein Paket erhält zwei Writer für denselben Pfad.

#### 5.3 Technische Anforderungen

`documented` — aus den Envelopes: C++20; Decoder/Interpreter/Memory-Verantwortung getrennt; keine JIT-, RIP-Fetch-, FLAGS/SIB/SIMD/Displacement- oder Game-Kompatibilitätsbehauptung; `GuestMemory` generisch; Host-Fakten ändern nie Guest-Semantik; keine neuen Abhängigkeiten, CMake-/Preset-Änderungen oder Agent-/Konfigurationsdateien (`AGENTS.md`, `ARCHITECTURE.md`, `:1191-1193,1207-1209`).

`observed` — Bare `cmake`/`ctest` sind in der PowerShell nicht auffindbar (`CommandNotFoundException`). Sie bleiben jedoch in `tester.md:44-61` als `effect: allow` **konfiguriert** (nicht verboten); nur die exakten absoluten Formen in `tester.md:62-73` lösen in dieser Shell auf und wurden tatsächlich erfolgreich ausgeführt (`EXIT=0`, `:1292-1323`). Der Zusatz `; "EXIT=$LASTEXITCODE"` dient nur der Exit-Erfassung. `unknown` — ob bare Formen auf einer anderen Maschine/Shell auflösen, wurde nicht geprüft.

#### 5.4 Build- und Testkriterien (A2, C1)

Exakt zu führen, keine Alternativform, kein Retry ohne neuen Befund:

```powershell
& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"
& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"
& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"
git diff --check; "EXIT=$LASTEXITCODE"
```

`documented` — A2: `Ok` und RIP-Vorschub für dokumentierte NOP/MOV, `MemoryAccessError` ohne Zustandsänderung bei Grenzverletzung und Adresse `0x100000000ULL`, Non-`Ok`-Decodes als `Ok`-No-op, alle bestehenden Tests erhalten (`:1193-1194`). C1: Provider-Overload liefert identischen Plan und Report wie Direkt-Overload, `FixedCapabilityProvider` repeatable und OS-unabhängig, bestehender Selektor unverändert, kein Probing, keine Dependency (`:1209-1210`). Reine Doku-Pakete: kein Build/CTest, nur Read-back, `git diff --check`, Reviewer.

#### 5.5 Review-Kriterien

1. Self-check des Writers mit exakten Kommandos und echten Exit-Codes.
2. `architect` nur bei Interface/ADR-/Modulgrenze (A2, C1; B1 nur bei Architekturberührung).
3. `tester` independent verification ohne Abschwächung von Tests.
4. `reviewer` **eine** gebündelte read-only Abschlussprüfung mit `PASS`/`NEEDS_CHANGE`/`BLOCKED` und `file:line`-Findings.
5. Findings klassifizieren als `ACTIONABLE`, `OWNER_DECISION`, `NON_BLOCKING`, `UNKNOWN`; Case A ohne Korrektur, Case B in **einem** gebündelten Korrekturpaket pro Writer, danach zwingend Re-Review; maximal zwei Runden, dann `BLOCKED` plus Owner-Eskalation.
6. Kein `PASS` ohne tatsächlich ausgeführte Kommandos und Resultate; kein Downgrade eines schweren Findings.

#### 5.6 Konkrete Abschlussbedingungen

Ein freigegebenes Paket gilt nur bei Erfüllung aller Punkte als `STATUS: pass`:

- exakte Kommandos und echte Ausgaben/Exit-Codes im Ledger recorded;
- Writer-Satz und geänderte Dateien stimmen mit dem Envelope überein, keine fremden Pfade;
- `git diff --check` EXIT=0 (bekannte CRLF-Warnung zulässig, dokumentiert);
- neue Tests für neues Verhalten vorhanden oder für Doku-Pakete begründet nicht nötig;
- Reviewer-`PASS` mit `file:line`-Belegen; offene `OWNER_DECISION`/`UNKNOWN` separat gelistet;
- keine Stage, Commit, Push, Merge, Tag, Release, PR, Publication, kein Worktree-/Rechte-/Konfigurationswechsel;
- `PUBLICATION_STATUS: local_only`, solange F1/F2 nicht vom Owner ausgeführt wurden.

Nach Freigabe dieser Vorlage sind **keine weiteren routinemäßigen Owner-Rückfragen** nötig. Unterbrechung nur bei echter neuer Blockade, Sicherheitsrisiko, widersprüchlicher Anforderung oder explizit vorbehaltener Entscheidung (ADR, Lizenz, Interface, Publikation, Rechte). Normale technische Detailentscheidungen innerhalb genehmigter Envelopes treffen die Agenten selbst.

### 6. Evidenzabgrenzung dieser Vorlage

- `documented` — alle Kennungen, Statuswerte, Envelope-Felder, ADR- und Rechtegrenzen, Review- und Publikationsregeln mit den genannten Ankern.
- `observed` — Review-Verdikte, CTest-EXIT 0 mit 1/1 `xbridge_tests`, 25 Working-Tree-Einträge (13/2/10 nach Post-CTest), `git diff --check` EXIT=0 mit CRLF-Warnung, installed-role-Status.
- `hypothesis` — XB-0084-Abhängigkeitskette und Design-Drafts A2/C1; keine Implementierung, keine Xbox-Kompatibilität.
- `unknown` — PR #1 aktuell, externe Webwahrheit, Lizenzwirkung, JIT-Machbarkeit, echte Host-Probing-Einstiegspunkte, Code-Mode-Laufzeitverhalten, zweite-PC-Portabilität, “publication group 2”, Inhalte von R-001…R-006.

Diese Vorlage hat keine Produkt-, Test-, CMake-, Preset-, Konfigurations-, Agent-, Backup-, Cache- oder Git-Veröffentlichungsänderung vorgenommen. Alle lokalen Änderungen bleiben erhalten.

`documented` — Review-Verlauf dieser Vorlage (Paket `XB-OWNER-DECISION-CONSOLIDATION`): erster unabhängiger Reviewer-Durchgang (`reviewer` `ses_df106a318ffeTp8G69Wv56J08k`) → `NEEDS_CHANGE` mit 2 `ACTIONABLE` (RV-1 MEDIUM: fehlende ID/Details für den offenen Owner-Call `:959(g)`; RV-2 LOW: Zeile `:1711` setzte „konfiguriert" mit „auffindbar" gleich), 0 `OWNER_DECISION`, 0 `NON_BLOCKING`, 1 `UNKNOWN` (RV-3: Session-Nebenläufigkeit und Byte-Identität der Anker 1-1363 nicht aus Dateiinhalt nachprüfbar — nicht blockierend). Klassifizierung: **Case B**. Gebündeltes Korrekturpaket `XB-OWNER-DECISION-CONSOLIDATION-C1`, einziger Writer = Coordinator, einziger Pfad = `docs/AGENT_TASKS.md`: (1) RV-1 → neue Entscheidung F6 mit Matrixzeile und neun Feldern, Kennung zitiert `:959(g)`, historische XB-0080-Zeile bleibt Snapshot, XB-0080-A/B wird nicht wieder geöffnet; (2) RV-2 → Zeile über bare `cmake`/`ctest` trennt nun `effect: allow` in `tester.md:44-61` von Shell-Auffindbarkeit und verweist auf die ausgeführten absoluten Formen `tester.md:62-73`. RV-3 benötigt keine Korrektur; die Coordinator-`git diff --check`- und Read-back-Nachweise bleiben maßgeblich. Re-Review durch unabhängigen Reviewer ist nach diesem Paket zwingend. `documented` — Re-Review (`reviewer` `ses_df1032857ffeqES2hbBGj9KSrq`, Runde 1 von 2): **`PASS`**, ACTIONABLE 0, OWNER_DECISION 0, NON_BLOCKING 1 (F6 ist nicht in der §5-Wellenplan-Aufzählung „F3..F5" bzw. der §5.2-Zuordnungstabelle geführt; vom Reviewer ausdrücklich als kein Änderungsbedarf eingestuft, da F6 Nach-Freigabe-Aktion in `:1686` eigenständig beschrieben ist und §5 provisional ist), UNKNOWN 1 (RV-3 bleibt nicht aus Dateiinhalt prüfbar). Beide ACTIONABLE-Findings aus Runde 1 gegen ihre Erfolgskriterien aufgelöst (`RV-1` → F6 mit neun Feldern `:1406,1677-1686`; `RV-2` → korrigierte Zeile `:1723`), keine Regression. Case A: kein weiterer Review, kein weiterer Fix. Der `PASS` bescheinigt ausschließlich die Dokumentationskorrektur und deren Read-back; die Vorlage bleibt `needs_owner` ohne Owner-Entscheidung und ohne Publikation.

```yaml
PACKAGE_ID: XB-OWNER-DECISION-CONSOLIDATION
PARENT_ID: N/A
DEPENDENCIES: XB-0082, XB-0083, XB-0084, SETUP-004, SETUP-006, SETUP-007, XB-X64-STEP-01, XB-HOST-CAP-01, XB-0085-CURRENT
STATUS: needs_owner
IMPLEMENTATION_STATE: needs_owner
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: OWNER-PUBLICATION
PROGRESS_UNITS: N/A
```

## XB-TECHNICAL-DELEGATION — technische Entscheidungen und Umsetzungspakete

**development infrastructure — nicht Teil des Produktpakets.** Der Owner hat am 2026-11-24 die fachlich-technische Entscheidungsverantwortung für die 22 Einträge A1–F6 delegiert und zugleich die Owner-Grenzen ausdrücklich bestehen gelassen: Git/Publikation/PR/Branch, Lizenz/Recht, fremder Code, Agentenrechte/Sicherheitsrichtlinien, nicht wiederherstellbare Löschungen und Änderungen außerhalb des genehmigten Bereichs. F1 bleibt vollständig gesperrt; die technische Delegation ist keine Publikationsfreigabe. `documented` — Owner-Delegationsnachricht; `observed` — Rechte- und Quellstand aus den genannten Dateien.

### TD.0 Einmalige Abhängigkeits- und Rechteprüfung (kein erneutes Review)

`observed` — effektive Schreibrechte je konkretem Agenten und Pfad (Globalregeln `opencode.jsonc:110-165` vor Agentregeln, letzte passende Regel gewinnt; harte `policies` nicht überstimmbar):

| Agent | Pfad | Ergebnis | Beleg |
|---|---|---|---|
| `developer` | `include/**`, `src/**`, `tests/**`, `CMakeLists.txt` | **ALLOW** | `developer.md:8-19`; globale Denys `.git/**`, `.env*`, Shell-Destruktiva und `policies` `opencode.jsonc:131-165` treffen nicht zu |
| `tester` | `tests/**` | **ALLOW** | `tester.md:8-10`; alle anderen Edits `deny` (`tester.md:5-7`) |
| `researcher` | `docs/RESEARCH_REGISTER.md`, `docs/research/**`, Web | **ALLOW** | `researcher.md:8-19` (letzte Regel gewinnt gegen globale Web-Denys `opencode.jsonc:112-113`); Shell `deny` (`researcher.md:26-37`) |
| `coordinator` | `docs/AGENT_TASKS.md`, `docs/ROADMAP.md` | **ALLOW** | `coordinator.md:8-16`; keine Produktpfade |
| `architect`, `reviewer` | — | read-only | `architect.md`/`reviewer.md` `edit: * = deny` |

Jeder Writer wird pro Pfad genau einmal benannt; `tests/test_main.cpp` ist zwischen A2 und C1 serialisiert. Es werden keine Rechte erweitert, keine Spezialrollen installiert und keine Worktrees angelegt (harte Denys `opencode.jsonc:127-128`, `policies` `git worktree *`).

### TD.1 Unter Delegation getroffene technische Entscheidungen

`documented` — Ausgangspunkt sind die Empfehlungen der Vorlage `:1385-1406`; abweichend davon wird nur eine technisch begründete, nachvollziehbar dokumentierte Wahl getroffen:

| Eintrag | Entscheidung | Art der Wirkung |
|---|---|---|
| A1 | **(a)** evaluation-only-Konvention bestätigt; kein Generator, Tool, CI, Dependency, keine Prozentanzeige | Ledger; kein Build |
| A2 | **(a)** öffentliche Fassade `step_x64_instruction` freigegeben; Testevidenz liegt vor; kein RIP-Fetch/JIT | Produktcode + Tests |
| A3 | **(c)** zurückgestellt; sieben ADRs einzeln mit eigener Evidenz, kein Sammel-ADR | Ledger; ADRs bleiben Owner-kontrolliert |
| A4 | **(a)** Interpreter-only; JIT/Native nur mit neuem ADR, Design, Tests und Paket | Ledger; keine Codeänderung |
| A5 | **(a)** Xbox 360 deferred; ADR-0004 unverändert | Ledger; keine Scope-Erweiterung |
| B1 | **(a)** scoped read-only Researcher-Paket mit exakten URLs/Daten | `docs/research/**` + Register |
| B2 | **(a)** XB-0084 als Evidenzbaseline akzeptiert, nicht als Implementierungsfreigabe | Ledger |
| C1 | **(a)** nur deterministischer Provider-Seam; echtes Windows-Probing bleibt separat | Produktcode + Tests |
| D1–D3 | keine Agentenaktion: Session-Export, Worktree, Rechte/Spezialisten bleiben Owner-kontrolliert | Ledger-Status quo |
| D4, E4, F6 | informativ/historisch: keine eigene Gruppe erfinden, keine C:-Artefakte, keine Anker-Verschiebung | Ledger |
| E1 | Patch 2 ist `ai/**` = Owner-Territorium (`developer.md:35-37`); **kein Agenten-Edit**, vollständiger Owner-Patch in TD.6 | Owner-Patch vorbereitet |
| E2, E3, F1, F2 | Publikation/Konfiguration/PR bleibt Owner-kontrolliert; `PUBLICATION_STATUS: local_only` | Keine Agentenaktion |
| F3 | keine Performance-Werte, kein Messlauf ohne präregistriertes Protokoll | Ledger |
| F4 | `UNLICENSED`, keine Third-Party-Übernahme, keine Lizenzänderung durch Agents | Ledger; rechtliche Gate bleibt Owner |
| F5 | `FINAL_CLEANUP` zurückgestellt (kein Feature-Complete, keine separate Freigabe) | Ledger |

Abweichung von der Empfehlung: keine. Die Empfehlungen sind mit der dokumentierten Architektur und den harten Gates vereinbar; eine objektiv bessere Alternative wurde nicht gefunden (`hypothesis` für künftige ADR-Entscheidungen, nicht für die hier umgesetzten Pakete).

### TD.2 Paket XB-X64-STEP-01 (A2)

```text
PACKAGE_ID:        XB-X64-STEP-01
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  product
GOAL:              Eine Byte-Span-Fassade ergänzen, die Decoder und Interpreter zu genau einem dokumentierten x64-Schritt komponiert, ohne deren Verträge zu ersetzen.
FILES:             include/xbridge/cpu/x64_instruction_interpreter.hpp; src/cpu/x64_instruction_interpreter.cpp; tests/test_main.cpp
ALLOWED:           [[nodiscard]] X64ExecuteStatus step_x64_instruction(std::span<const std::uint8_t>, X64CpuState&, memory::GuestMemory&) noexcept in namespace xbridge::cpu; Implementierung ausschließlich decode_x64_instruction(bytes) gefolgt von execute_instruction(decoded, state, memory); Tests im vorhandenen check-Harness. Writer: developer (Produktpfade), tester (tests/test_main.cpp) — genau ein Writer je Pfad.
FORBIDDEN:         JIT, RIP-Fetch, Run-Loops, neue Statuswerte, FLAGS/SIB/SIMD/Displacement, Änderungen an TestCpu/GuestMemory/Decoder-Repräsentation/CMake/Presets/Doku/Agent-Konfiguration, Schwächung bestehender Tests.
ACCEPTANCE:        Ok und RIP-Vorschub um DecodedInstruction::length für NOP/MOV; MemoryAccessError ohne Zustands-/Speicheränderung bei Grenzverletzung und 0x100000000ULL; jeder Non-Ok-Decode wird Ok-No-op ohne Zustandsänderung; alle bestehenden Tests bleiben; exakte Kommandos mit echten Exit-Codes.
TESTS:             NOP {0x90}; MOV imm32 {0xB8,0x78,0x56,0x34,0x12}; MOV imm64 mit REX.W; MOV reg/reg und reg/mem; 4-/8-Byte-Grenzen; Adresse 0x100000000ULL; Non-Ok {}/ {0x0F}/ {0x89,0x04}/ {0x48,0x8B}. Siehe TD.5.
REVIEW:            architect (Interface) und tester (Verifikation) vor der einen gebündelten Abschlussprüfung durch reviewer.
WORKTREE:          none
OWNER_APPROVALS:   Erfüllt — technische Delegation vom 2026-11-24 für A2; öffentliche API ist Owner-Gate und wurde hier technisch freigegeben.
```

### TD.3 Paket XB-0083-AUTHORITATIVE-EVIDENCE (B1)

```text
PACKAGE_ID:        XB-0083-AUTHORITATIVE-EVIDENCE
PACKAGE_TYPE:      RESEARCH
PRODUCT_BOUNDARY:  development-only
GOAL:              Die sechs projektautorenen CPU/Grafik-Behauptungen R-001..R-006 gegen autoritative öffentliche Microsoft-/AMD-Quellen prüfen und nicht Verifiziertes explizit unknown lassen.
FILES:             docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md (neu); docs/RESEARCH_REGISTER.md
ALLOWED:           researcher Web-Recherche mit exakter URL, Zugriffsdatum und Evidence-Label; Registerfelder nach docs/RESEARCH_REGISTER.md:46-72 (23 Felder, feste Reihenfolge) aktualisieren; Conformance-Status gemäß :90-105 selbst bewerten. Writer: researcher (beide Pfade).
FORBIDDEN:         Evidenz anderer Konsolen als Xbox-Fakt, NDA/privates Material, proprietäre Assets, Codeübernahme, Produkt-/ADR-/Interface-Änderung, unbelegte Kompatibilitäts-/Performance-Claims, Shell-Kommandos.
ACCEPTANCE:        Jede Behauptung hat autoritative URL oder bleibt unknown; keine Verwechslung von project-authored mit authoritative; Registerzeilen R-001..R-006 bleiben konform; git diff --check EXIT=0.
TESTS:             Read-back, Registerkonformität (23 Felder/erlaubte Tokens), Reviewer; kein Build/CTest (development-only).
REVIEW:            architect nur bei Architekturberührung; reviewer in der einen gebündelten Abschlussprüfung.
WORKTREE:          none
OWNER_APPROVALS:   Erfüllt — technische Delegation für B1; keine Produktfreigabe.
```

### TD.4 Paket XB-HOST-CAP-01 (C1)

```text
PACKAGE_ID:        XB-HOST-CAP-01
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  product
GOAL:              Einen deterministischen Capability-Provider-Seam ergänzen, damit die Policy Host-Fakten über ein Interface konsumiert, ohne Windows-Probing oder Guest-Semantikänderung.
FILES:             include/xbridge/host/capabilities.hpp; include/xbridge/host/execution_policy.hpp; src/host/execution_policy.cpp; tests/test_main.cpp
ALLOWED:           ICapabilityProvider (virtueller Destruktor, [[nodiscard]] capabilities() const noexcept = 0), FixedCapabilityProvider (explicit, unveränderliche HostCapabilities) und select_execution_plan(const ICapabilityProvider&, const ExecutionSettings&) als Forwarding auf die vorhandene HostCapabilities-Overload; deterministische Provider-Tests. Writer: developer (Hostpfade), tester (tests/test_main.cpp) — nach A2 serialisiert.
FORBIDDEN:         Win32/Vulkan/D3D12-Probing, Backend-Initialisierung, Firmware/Keys/proprietäre Assets, neue Abhängigkeiten, CMake-/Preset-Änderungen, Entfernen/Ändern der bestehenden Overload, Guest-CPU/-Memory/-GPU/-Profile-Änderungen, JIT, Kompatibilitätsclaims.
ACCEPTANCE:        Provider-Overload liefert Plan und Report identisch zur Direkt-Overload; FixedCapabilityProvider repeatable und OS-unabhängig; bestehender Selektor und bestehende Tests unverändert; kein Probing, keine Dependency; exakte Kommandos mit echten Exit-Codes.
TESTS:             Synthetische Fixtures (automatische Vulkan-Wahl, Software-Fallback, Fallback deaktiviert -> Unavailable, manuelles D3D12, JIT verfügbar/nicht, Reporttext); wiederholte unveränderliche Fakten; unmutierte Settings. Siehe TD.5.
REVIEW:            architect (Interface/ADR/Host-Guest), tester (Verifikation) vor der einen gebündelten Abschlussprüfung durch reviewer.
WORKTREE:          none
OWNER_APPROVALS:   Erfüllt — technische Delegation für C1; echtes Windows-Probing ist weiterhin separate Owner-Entscheidung.
```

### TD.5 Reihenfolge und Verifikation

```text
Welle 1a (parallel, disjoint):  XB-X64-STEP-01/developer  |  XB-0083/researcher
Welle 1b (nach A2):             tests/test_main.cpp/tester (A2) + A2-Verifikation
Welle 2 (nach A2):              XB-HOST-CAP-01/developer -> tests/test_main.cpp/tester (C1) + C1-Verifikation
Welle 3 (coordinator, serialisiert): Ledger-/Roadmap-Vermerke A1,A3-A5,B2,D1-D4,E1-E4,F1-F6
Abschluss:                      EINE gebündelte unabhängige Abschlussprüfung (reviewer) über A2+B1+C1+Ledger
```

`observed` — Bare `cmake`/`ctest` sind in dieser PowerShell nicht auffindbar; die absoluten VS2026-Formen sind in `tester.md:62-73` erlaubt und waren zuvor erfolgreich (`EXIT=0`). Exakt zu führen (kein Retry ohne neuen Befund):

```powershell
& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"
& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"
& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"
git diff --check; "EXIT=$LASTEXITCODE"
```

Der Reviewer erhält **eine** Prüfung mit `PASS`/`NEEDS_CHANGE`/`BLOCKED` und `file:line`-Findings; Findings werden als `ACTIONABLE`/`OWNER_DECISION`/`NON_BLOCKING`/`UNKNOWN` klassifiziert, Case B wird in **einem** gebündelten Korrekturpaket pro Writer behoben und einmal nachgeprüft. Keine Zwischenreviews, keine wiederholte Bestandsaufnahme, kein Build-Erfolg allein als Qualitätsnachweis.

### TD.6 Vollständiger Owner-Patch für E1 (nicht angewendet)

`documented` — Kanonisch ist ausschließlich der review-validierte Patch bei `:636-645`; die frühere Kurzfassung „for review and publication" war eine historische Paraphrase und ist nicht mehr maßgeblich. `observed` — `ai/README.md:13,28` enthalten weiterhin „Open a pull request"; `GITHUB_AGENT_WORKFLOW.md:7-9,49` verlangt Owner-Handoff; kein Agent hat Edit-Recht auf `ai/**` (`developer.md:35-37`, `coordinator.md` ohne `ai/**`).

```text
ai/README.md:13   "Open a pull request" -> "Hand the result to the project owner; the owner performs all Git/GitHub publication actions."
ai/README.md:28   "Step 7 of the workflow above is performed by the project owner. Agents must not create pull requests or perform any other Git/GitHub publication action." -> "Step 7 is explicit: agents hand off; the project owner alone may open a pull request or perform any other Git/GitHub publication action."
```

`observed` — Zusätzliche, nicht stillschweigend aufgelöste Quellenabweichung: `docs/ROADMAP.md:31` nennt für dasselbe XB-0085-Audit 32 Einträge mit 13 `READY_FOR_COMMIT` / 18 `NEEDS_REVIEW` / 1 `DO_NOT_PUBLISH`, während `docs/AGENT_TASKS.md:955,1764` 25 Einträge mit 13/2/10 und `:1059,1179` 12/3/10 nennen. Diese technische Delegation ändert weder Audit noch Zählung: F1 ist Publikation und damit vollständig Owner-kontrolliert; die Varianten bleiben `unknown` und werden dem Owner nicht als technische Entscheidung vorgelegt.

### TD.7 Statusaufnahme A2 — `XB-X64-STEP-01` (verifiziert, Review ausstehend)

`observed` — Writer `developer` (`ses_df0f881cbffejueQE5dT1G2SFD`) änderte genau `include/xbridge/cpu/x64_instruction_interpreter.hpp` (`#include <span>` + Deklaration) und `src/cpu/x64_instruction_interpreter.cpp` (Komposition `decode_x64_instruction(bytes)` → `execute_instruction(decoded, state, memory)`). Self-check `git diff --check; "EXIT=$LASTEXITCODE"` → nur vorbestehende CRLF-Warnung zu `CMakePresets.json`, `EXIT=0`. Keine Tests durch den Developer; keine Produkt-, Doku-, CMake-, Preset- oder Konfigurationsänderung darüber hinaus.

`observed` — Writer `tester` (`ses_df0f76f57ffecHFrmusk5RAS80`) änderte genau `tests/test_main.cpp`: neue Funktion `test_x64_step_instruction_facade()` mit NOP, MOV imm32, MOV imm64/REX.W, MOV reg/reg, MOV reg/mem, 4-/8-Byte-Grenzfehlern, Adresse `0x100000000ULL` und vier Non-Ok-No-ops; registriert in `main()` nach `test_x64_interpreter_non_ok_decode_noop()`. Kein bestehender Test geschwächt. Exakte Kommandos und echte Resultate:

```text
& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"
  → Configuring/Generating done, Build files written to build/windows-debug, EXIT=0
& '...\cmake.exe' --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"
  → xbridge_core.lib, xbridge.exe, xbridge_tests.exe gebaut, EXIT=0
& '...\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"
  → 1/1 Test #1: xbridge_tests ... Passed 0.03 sec; 100% tests passed, 0 failed; EXIT=0
git diff --check; "EXIT=$LASTEXITCODE"
  → nur vorbestehende CRLF-Warnung zu CMakePresets.json, EXIT=0
```

`observed` — Unabhängiger Vertragscheck des Testers: Deklaration und Signatur stimmen, Status-Enum unverändert (`Ok`, `MemoryAccessError`), kein JIT/RIP-Fetch/Loop/FLAGS/SIB/SIMD/Displacement. `unknown` aus Sicht des Testers: formale Owner-Genehmigung `OWNER-X64-STEP` und einzelne `[PASS]`-Zeilen (CTest zeigt nur bei Fehler Output; `main()` gibt bei `failures > 0` Return 1 — im Quellcode gesehen). Die technische Freigabe liegt mit der Owner-Delegation vom 2026-11-24 vor (TD.1). A2 ist damit implementiert und verifiziert, aber **nicht** abgeschlossen, solange die eine gebündelte Abschlussprüfung aussteht.

### TD.8 Statusaufnahme C1 — `XB-HOST-CAP-01` (verifiziert, Review ausstehend)

`observed` — Writer `developer` (`ses_df0f4f10dffezMb4t48iiWXqW1`) änderte genau `include/xbridge/host/capabilities.hpp` (`ICapabilityProvider`, `FixedCapabilityProvider final`, `<utility>`), `include/xbridge/host/execution_policy.hpp` (neue Overload-Deklaration) und `src/host/execution_policy.cpp` (Forward `return select_execution_plan(provider.capabilities(), settings);`). Technische Entscheidungen: `const HostCapabilities&`-Rückgabe, um die `noexcept`-Zusage trotz `std::string`-Member nicht zu verletzen; `const`-Member und `final` für Unveränderlichkeit; Konstruktor bewusst nicht `noexcept` (mögliche Allokation). Self-check `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0`, zwei CRLF-Warnungen (`CMakePresets.json`, `src/host/execution_policy.cpp`), keine Whitespace-Fehler; Index leer, nichts gestaget.

`observed` — Writer `tester` (`ses_df0f76f57ffecHFrmusk5RAS80`, fortgesetzte Session) änderte genau `tests/test_main.cpp`: neue Funktion `test_host_capability_provider()` mit Identitäts-/Unveränderlichkeitschecks des Providers und sechs synthetischen Fixture-Fällen (Vulkan auto, Software-Fallback, Fallback deaktiviert → Unavailable, manuelles D3D12, JIT verfügbar/nicht), jeweils Plan- und Reportgleichheit zur Direkt-Overload, Wiederholbarkeit und unveränderte `ExecutionSettings`; registriert in `main()` nach `test_cpu_program_lifecycle()`. A2-Tests blieben erhalten. Exakte Kommandos und echte Resultate:

```text
& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"
  → Configuring/Generating done, Build files written to build/windows-debug, EXIT=0
& '...\cmake.exe' --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"
  → execution_policy.cpp, xbridge_core.lib, xbridge.exe, xbridge_tests.exe gebaut, EXIT=0
& '...\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"
  → 1/1 Test #1: xbridge_tests ... Passed 0.02 sec; 100% tests passed, 0 failed; EXIT=0
git diff --check; "EXIT=$LASTEXITCODE"
  → zwei CRLF-Warnungen (CMakePresets.json, src/host/execution_policy.cpp), EXIT=0
```

`observed` — Unabhängiger Vertragscheck des Testers: reiner Forward zur bestehenden Overload, bestehender Selektor und Reporttext unverändert, kein Probing/Backend-Init/Guest-Semantik/JIT. `unknown` aus Sicht des Testers: formale Owner-Genehmigung `OWNER-HOST-CAP` (hier durch Delegation TD.1 erfüllt), echte Win32/DXGI/Vulkan-Einstiegspunkte (ausgeschlossen), einzelne `[PASS]`-Zeilen. C1 ist implementiert und verifiziert, aber **nicht** abgeschlossen, solange die eine gebündelte Abschlussprüfung aussteht.

### TD.9 Statusaufnahme B1 — `XB-0083-AUTHORITATIVE-EVIDENCE` (verifiziert, Review ausstehend)

`observed` — Writer `researcher` (`ses_df0f881cbffduCZPAOqd6QgpWI`) änderte genau `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md` (neu) und `docs/RESEARCH_REGISTER.md` (R-001…R-006 Detailblöcke, kompakter Index `:111-116`, Audit-Notizen `:336-337`). Kein Shell-/Git-/CMake-/CTest-Kommando durch den Researcher; kein Produktcode, Test, ADR, Interface, Ledger, Roadmap, Konfiguration oder Preset berührt. Exakte URLs mit Zugriffsdatum 2026-11-24 sind im Dokument `:18-42` inventarisiert; sechs 23-Feld-Datensätze und eine Section-5-Konformitätstabelle liegen vor. Der Researcher zeichnet `conformant` für alle sechs Zeilen auf — das ist **kein** Reviewer-Verdict; die unabhängige Prüfung läuft.

`documented` — Autoritative, aber bewusst eng gefasste Befunde: `https://www.xbox.com/en-US/consoles/xbox-series-x` (Zen-2-/RDNA-2-Hardwareangaben, Speicher, NVMe), Windows-x64-ABI (`learn.microsoft.com/.../x64-calling-convention`, `.../x64-software-conventions`), öffentliches D3D12/Mesh-Shader/DXIL/DirectStorage. `unknown` bleiben Guest-CPUID/AVX2 und Guest-ABI (R-001), D3D12.X (R-002), Xbox-Shaderformat und Übersetzungskorrektheit (R-003), Guest-Virtual-Memory (R-004), Xbox-Speicher-API und Host-Mapping (R-005), Xbox-Systemdienste (R-006). Alle sechs Capability-Status bleiben `unknown`; `partial` bei R-005 bleibt reiner Workflow-Vermerk. HTTP-402/404-/Timeout-Quellen sind als unbrauchbar dokumentiert, nicht als Evidenz verwendet. Kein GDK/NDA-, Fremdkonsolen-, DRM- oder Proprietärmaterial; keine Codeübernahme, keine Kompatibilitäts- oder Performancebehauptung.

`observed` — Coordinator-`git diff --check; "EXIT=$LASTEXITCODE"` nach allen Paketen: nur zwei CRLF-Warnungen (`CMakePresets.json`, `src/host/execution_policy.cpp`), `EXIT=0`. B1 ist implementiert und read-back-geprüft, aber **nicht** abgeschlossen, solange die eine gebündelte Abschlussprüfung aussteht.

### TD.10 Abschluss — eine gebündelte unabhängige Abschlussprüfung, `PASS` (Case A)

`documented` — Reviewer (`ses_df0ee9835ffe1x6g1inSAdFk2L`) prüfte A2 + B1 + C1 + TD.0–TD.9 einmalig und read-only; kein zweites Review, keine Korrekturrunde. Verdikt **`PASS`**, Case A: `ACTIONABLE 0`, `OWNER_DECISION 0`, `NON_BLOCKING 3`, `UNKNOWN 1`, keine Schwereinstufung HIGH/MEDIUM. `documented` — mit `file:line` belegte Kernbefunde: A2-Deklaration `include/xbridge/cpu/x64_instruction_interpreter.hpp:31-34`, reine Komposition `src/cpu/x64_instruction_interpreter.cpp:150-157`, Boundary-/Negative-Tests `tests/test_main.cpp:1515-1693`, Registrierung `:1917`; B1-Konformität `docs/RESEARCH_REGISTER.md:124-276` (23 Felder, erlaubte Tokens, `:129,155,181,207,233,259` Capability `unknown`, `:145,171,197,223,249,275` `conformant`) und Quelleninventar `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md:18-42`; C1-Seam `include/xbridge/host/capabilities.hpp:24-47`, Forward `src/host/execution_policy.cpp:95-98`, Tests `tests/test_main.cpp:1759-1866` und Registrierung `:1920`. Bestätigt: ADR-0005/0008/0010/0011, Host/Guest-Trennung, kein Probing/JIT/Proprietärmaterial/Agenten-Text in Produktpfaden, keine Testschwächung, Writer-Satz stimmt mit den Envelopes überein.

`observed` — Offene Punkte, die kein `ACTIONABLE` sind: FINAL-01 LOW `NON_BLOCKING` (zusätzliche CRLF-Warnung `src/host/execution_policy.cpp`; `git diff --check` `EXIT=0`, success criterion erfüllt); FINAL-03 LOW `NON_BLOCKING` (Axen waren zum Review-Zeitpunkt `not_reviewed`/`planned`/`0/4` — hiermit aktualisiert); FINAL-04 LOW `NON_BLOCKING` (E1-Owner-Patch bewusst nicht angewendet, `ai/**` für Agents gesperrt). FINAL-02 LOW `UNKNOWN`: der Reviewer hat kein Shell-Recht und konnte die vier Kommandos nicht selbst wiederholen; er stützt sich auf die aufgezeichneten Tester-Resultate plus direktes Quellenlesen. Die Erfolgskriterien sind durch die Testerläufe `:1917-1924` und `:1936-1943` erfüllt.

`documented` — Status der Einzelpakete: `XB-X64-STEP-01` (A2) `STATUS: pass`, `XB-0083-AUTHORITATIVE-EVIDENCE` (B1) `STATUS: pass`, `XB-HOST-CAP-01` (C1) `STATUS: pass`, jede mit `REVIEW_STATUS: PASS` und `PUBLICATION_STATUS: local_only`. Kein Paket überschreitet die Owner-Grenzen; F1/F2, Lizenz (F4), ADRs (A3), Rechte/Spezialisten (D3), Worktree/Session (D1/D2) und `FINAL_CLEANUP` (F5) bleiben unangetastet und Owner-kontrolliert.

```yaml
PACKAGE_ID: XB-TECHNICAL-DELEGATION
PARENT_ID: XB-OWNER-DECISION-CONSOLIDATION
DEPENDENCIES: XB-X64-STEP-01, XB-0083-AUTHORITATIVE-EVIDENCE, XB-HOST-CAP-01
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: conformant
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: 4/4
```

## XB-NEXT-TECHNICAL-WAVE — `XB-X64-RIP-STEP-01`

**development infrastructure — nicht Teil des Produktpakets.** Fortsetzung unter der technischen Delegation des Owners; alle Owner-Grenzen aus TD.1 bleiben in Kraft. `documented` — Auswahlbegründung: `docs/ROADMAP.md:27,53` nennt die RIP-basierte Single-Step-Brücke als nächsten CPU-Meilenstein; sie war nur durch die nicht installierten Spezialisten (`SETUP-007`) vertagt und ist mit den installierten Rollen ausführbar. Sie baut auf der verifizierten A2-Fassade (TD.7, Review `PASS`) auf und benötigt keinen JIT, keine Abhängigkeit und kein Proprietärmaterial. Der kontrollierte Run-Loop bleibt ein **eigenes späteres** Paket.

```text
PACKAGE_ID:        XB-X64-RIP-STEP-01
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  product
GOAL:              Einen RIP-basierten Single-Step-Fetch ergänzen, der genau eine dokumentierte x64-Instruktion aus GuestMemory holt und über die bestehende Fassade ausführt, ohne Run-Loop oder JIT.
FILES:             include/xbridge/cpu/x64_instruction_interpreter.hpp; src/cpu/x64_instruction_interpreter.cpp; tests/test_main.cpp
ALLOWED:           [[nodiscard]] X64ExecuteStatus step_x64_at_rip(X64CpuState& state, memory::GuestMemory& memory) noexcept in namespace xbridge::cpu; boundierter Fetch von höchstens 15 Bytes ab state.rip via read8 in ein lokales std::array; Validierung rip <= UINT32_MAX und rip < memory.size(); count = min(15, memory.size() - rip), span deckt exakt count Bytes ohne Null-Padding; read8 ist nicht noexcept und muss gegen std::out_of_range abgesichert werden (Return MemoryAccessError vor jedem Zustands-/Speicherzugriff); Aufruf von step_x64_instruction(span, state, memory) und Rückgabe dessen Status; eigener Kommentar für die Brücke, bestehender A2-Kommentar unverändert. Writer: developer (Produktpfade), tester (tests/test_main.cpp) — genau ein Writer je Pfad.
FORBIDDEN:         Run-Loop, JIT, RIP-relative Operand-Adressierung (der Decoder lehnt mod==00/rm==101 weiterhin ab), Änderung an Decoder, execute_instruction, A2-Fassade, TestCpu, GuestMemory, Status-Enums, FLAGS/SIB/SIMD/Displacement, CMake/Presets/Doku/Konfiguration, Schwächung bestehender Tests, neue Abhängigkeiten.
ACCEPTANCE:        Ok und RIP-Vorschub entsprechend der ausgeführten Instruktion für NOP/MOV-Programme im GuestMemory; aufeinanderfolgende Schritte führen aufeinanderfolgende Instruktionen aus; MemoryAccessError ohne CPU-/Speicheränderung bei rip > UINT32_MAX und rip >= memory.size(); Instruktion am letzten Byte ist ausführbar, der nächste Schritt liefert MemoryAccessError ohne Mutation; Non-Ok-Decodes liefern Ok mit **unverändertem CPU-Zustand einschließlich RIP** (kein length-basierter Vorschub außerhalb des dokumentierten Ok-Pfads), die Brücke übersetzt keinen Status; exakte Kommandos mit echten Exit-Codes.
TESTS:             Programm {NOP, MOV imm32, NOP} ab Adresse 0x100 mit drei Schritten; MOV imm64 mit REX.W; MOV reg/mem über den Fetch; NOP am letzten Byte und danach MemoryAccessError; rip > UINT32_MAX und rip == memory.size() ohne Mutation; Non-Ok-Opcode 0x0F als Ok-No-op mit unverändertem RIP; alle bestehenden A2-/Interpreter-Tests bleiben.
REVIEW:            architect (Interface/Modulgrenze) vor Implementierung; tester (unabhängige Verifikation); EINE gebündelte Abschlussprüfung durch reviewer.
WORKTREE:          none
OWNER_APPROVALS:   Keine — technische Delegation vom 2026-11-24; öffentliche Schnittstelle liegt im delegierten Umfang.
```

`documented` — Vertragsgrenzen für die Implementierung: der 15-Byte-Fetch ist ausschließlich ein Lese-Fenster, kein Guest-Adressierungsmodus; bei `rip > UINT32_MAX` oder leerem Restbereich darf **kein** Zustand und **kein** Byte verändert werden; die 15 ist eine Implementierungs-Obergrenze ohne x86-Architekturbehauptung (`unknown`); ein Non-Ok-Decode wird von der bestehenden Fassade bewusst als `Ok`-No-op mit **unverändertem RIP** behandelt und darf vom Bridge-Paket weder in einen neuen Status noch in einen length-basierten Vorschub übersetzt werden; ein Run-Loop, der bei `length == 0` hängen bleiben könnte, ist ausgeschlossen — ein späterer Loop muss RIP vor/nach jedem Schritt vergleichen und bei ausbleibendem Fortschritt mit Diagnose stoppen.

`documented` — Architect-Vorprüfung (`ses_df0e4bc64ffe4MT1MOflX4eyhz`, read-only): `ARCH NEEDS_CHANGE` auf Envelope-Ebene, **kein** Codefehler. `ACTIONABLE HIGH`: `GuestMemory::read8` ist nicht `noexcept` und wirft `std::out_of_range` (`src/memory/memory.cpp:22-39`) — die Brücke muss vorvalidieren, den Fetch gegen `std::out_of_range` absichern und vor jeder Zustands-/Speicheränderung `MemoryAccessError` zurückgeben. `ACTIONABLE HIGH`: die alte Envelope-Fassung „Non-Ok mit length-basiertem RIP-Vorschub" widersprach Implementierung und Doku (`src/cpu/x64_instruction_interpreter.cpp:31-37`, `docs/ARCHITECTURE.md:162-163`, `tests/test_main.cpp:1671-1692`) — korrigiert zu `Ok` mit unverändertem RIP. `ACTIONABLE MEDIUM`: kurzes Fetch-Fenster ohne Null-Padding, `count = min(15, size - rip)`; NOP am letzten Byte führt RIP auf `size`, nächster Schritt `MemoryAccessError`. `ACTIONABLE LOW`: eigener Kommentar für die Brücke, A2-Kommentar unverändert. Beides ist in ALLOWED/ACCEPTANCE/TESTS oben eingearbeitet. `OWNER_DECISION MEDIUM` (Architect): `docs/ROADMAP.md:27` nennt die Brücke noch als vertagten Meilenstein — `documented` — Coordinator-Entscheidung unter der technischen Delegation TD.1: diese Terminnotiz ist veraltet und wird durch die freigegebene Envelope sowie die Owner-Delegation vom 2026-11-24 ersetzt; kein Owner-Eingriff nötig, kein Reel der Delegation. `NON_BLOCKING` (2): Run-Loop-Spin-Gefahr und fehlende autoritative Herleitung der 15 — beide als Constraints oben vermerkt.

`observed` — Writer `developer` (`ses_df0e20eb0ffe60iuPgHoQV0cFh`) änderte genau die zwei Produktpfade: `include/xbridge/cpu/x64_instruction_interpreter.hpp` (neue Deklaration `step_x64_at_rip`, eigener Kommentar; A2-Kommentar byte-for-byte unverändert) und `src/cpu/x64_instruction_interpreter.cpp` (`#include <algorithm>`, `#include <array>`, Implementierung). Verifikation vor allen Reads: `rip > UINT32_MAX` und `rip >= memory.size()` → `MemoryAccessError` ohne Mutation; `count = min(15, size - rip)`, exakt `count` Bytes via `read8` in `std::array<uint8_t,15>`, kein Zero-Padding; Fetch-Loop in `try/catch(std::out_of_range)`, direkter Return von `step_x64_instruction(span, state, memory)` ohne Statusübersetzung. Self-check `git diff --check; "EXIT=$LASTEXITCODE"` → zwei bekannte CRLF-Warnungen, `EXIT=0`. Keine Tests, kein Build/CTest, keine Doku-/Ledger-/Konfigurationsänderung durch den Developer; `tests/test_main.cpp` bleibt dem Tester vorbehalten. `unknown`: Kompilat-/CTest-Ergebnis erst nach dem Tester.

```yaml
PACKAGE_ID: XB-X64-RIP-STEP-01
PARENT_ID: XB-TECHNICAL-DELEGATION
DEPENDENCIES: XB-X64-STEP-01
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: 1/1
```

`observed` — Read-only abort diagnosis (`XB-TEST-ABORT-INVESTIGATION`): `grep` nach `abort|std::abort|assert|_ASSERTE` in `src/**`, `include/**`, `tests/**` fand keine Laufzeit-Abbruchfunktion; die beiden `static_assert` in `tests/test_main.cpp:60,62` sind Compile-Time-Typprüfungen. `tests/test_main.cpp:23-30` zählt nur `failures`; `main()` gibt bei Fehlern `1` zurück (`:2199-2204`). Die konkrete ungeschützte Wurfstelle ist `tests/test_main.cpp:1914-1915`: `memory::GuestMemory memory(256);` erlaubt Adressen `0..255`, `memory.load(0x100, {7 Bytes})` liegt eine Adresse und 7 Bytes hinter dem Ende. `src/memory/memory.cpp:22-28` wirft dort `std::out_of_range`; der Aufruf steht ohne `try/catch`, `check_throws` oder `check_does_not_throw`, und die Funktion war im diagnostischen Arbeitsstand erster Aufruf in `main()` (`:2029`). `hypothesis` — die entwichene Ausnahme erreicht `std::terminate()` → CRT-`abort()` → der vom Owner gemeldete MSVC-Debug-Dialog; kein Stack-Trace und kein Dump vorhanden.

`observed` — Tester-Evidenz aus `ses_df0f76f57ffecHFrmusk5RAS80`, ohne eigene Wiederholung durch den Coordinator: Configure `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"` → `EXIT=0`. Build `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"` → `EXIT=0`; Ausgabe enthielt `x64_instruction_interpreter.cpp`, `test_main.cpp` und `xbridge_tests.exe`. CTest `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"` mit registriertem RIP-Test: `***Failed 968.29 sec`, `0% tests passed, 1 tests failed out of 1`, `EXIT=8`; zweiter Lauf `***Failed 944.34 sec`, `EXIT=8`; später diagnostic first-call-Lauf `***Failed 324.40 sec`, `0 [PASS]-Zeilen`, `EXIT=8`. Mit temporär entferntem RIP-Testaufruf: `1/1 Test #1: xbridge_tests ... Passed 0.03 sec`, `100% tests passed`, `EXIT=0`. Der Tester beobachtete den Dialog nicht selbst; der Owner berichtete ihn. `git diff --check` wurde für dieses Paket nicht ausgeführt → `unknown`. `observed` — Arbeitsstand vor Korrektur: vollständige Testfunktion vorhanden, Single-Call am Anfang von `main()`, geplante Registrierung nach `test_host_capability_provider()` (`:2078`) noch nicht wiederhergestellt; keine Produkt-, CMake-, Preset- oder Konfigurationsänderung durch den Tester; keine weaken/skip/delete existing tests.

`documented` — Klassifizierung nach Tester- und Coordinator-Evidenz: `ACTIONABLE HIGH` = ungeschützte one-past-end `load` bei `:1914-1915`; `ACTIONABLE MEDIUM` = diagnostische Single-Registrierung bei `:2029` statt nach `:2078`; `ACTIONABLE LOW` = fehlendes `git diff --check` für dieses Paket. `Coordinator decision`: finale RIP-Erwartung `0x107` ist korrekt (NOP 1 + MOV imm32 5 + NOP 1); ein abweichender älterer `0x104`-Text ist durch die korrigierte Envelope ersetzt. `OWNER_DECISION` = Freigabe der Änderung trotz zuvor gesetztem Änderungs-Stopp; erteilt am 2026-11-24 durch die Antwort „Tester-Korrektur freigeben (Recommended)". `NON_BLOCKING` = `EXIT=8` ohne `[FAIL]`-Zeile, nicht selbst beobachteter Dialog und bekannte CRLF-Warnungen. `UNKNOWN` = ob beide ursprünglich gemeldeten Abbrüche exakt derselben Zeile entsprachen; keine Exit-Codes, Stack-Traces oder Dumps dieser Ereignisse.

## Correction — `XB-X64-RIP-STEP-01-CORR-01`

**development infrastructure — nicht Teil des Produktpakets.** `documented` — Ein gebündeltes Korrekturpaket pro Writer nach ACTIONABLE-Finding; kein Architect nötig, weil es eine lokale Testkorrektur ohne Interface-, Modulgrenzen-, Dependency- oder ADR-Änderung ist.

```text
PACKAGE_ID:        XB-X64-RIP-STEP-01-CORR-01
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  product
GOAL:              Die bestätigte ungeschützte std::out_of_range-Wurfstelle im RIP-Bridge-Test entfernen und die geplante Testregistrierung wiederherstellen.
FILES:             tests/test_main.cpp
ALLOWED:           GuestMemory(256) -> GuestMemory(256+256) im ersten Block von test_x64_step_at_rip_bridge(); Single-Call test_x64_step_at_rip_bridge() von main()-Start auf die Position direkt nach test_host_capability_provider() verschieben; asserted RIP 0x107 unverändert lassen.
FORBIDDEN:         Produktcode, CMake, Presets, Doku, Ledger, Konfiguration, Agent-Dateien, andere Testfälle, Schwächung/Skipping/Deletion bestehender Tests, Build-Artefakt-Bereinigung, Git-Operationen, automatische Retry-Loops.
ACCEPTANCE:        Genau zwei erlaubte Edits; alle bestehenden Tests erhalten; Configure/Build/CTest/diff-check mit echten Exit-Codes; CTest 1/1 xbridge_tests mit EXIT=0; danach EIN unabhängiger Reviewer-Re-Check.
TESTS:             Absolute VS2026 cmake.exe --fresh --preset windows-debug; absolute cmake.exe --build --preset windows-debug --config Debug; absolute ctest.exe --test-dir build/windows-debug -C Debug --output-on-failure; git diff --check; "EXIT=$LASTEXITCODE" nach jedem Befehl.
REVIEW:            reviewer, EIN Re-Check nach stabilem Tester-Report; keine Schleife ohne neue ACTIONABLE-Findings.
WORKTREE:          none
OWNER_APPROVALS:   Owner-Antwort 2026-11-24: „Tester-Korrektur freigeben (Recommended)".
```

`observed` — Rechtecheck vor Freigabe: `.opencode/agents/tester.md:5-10` = `edit * deny`, danach `edit tests/** allow`; globale `opencode.jsonc:115-165` enthält keine harte Policy gegen `tests/**`; Coordinator hat `tester` in diesem Paket ausdrücklich als alleinigen Writer von genau `tests/test_main.cpp` benannt. `developer` und `coordinator` schreiben diesen Pfad nicht.

`observed` — Writer `tester` (`ses_df0f76f57ffecHFrmusk5RAS80`) meldet genau die zwei freigegebenen Edits: `tests/test_main.cpp:1914` = `memory::GuestMemory memory(512);`, `state.rip`, alle Assertions inklusive finalem `0x107` unverändert; kein Aufruf am Anfang von `main()`, stattdessen genau ein Aufruf direkt nach `test_host_capability_provider();` (`:2077`). Alle vorbestehenden Tests erhalten; keine Produkt-, CMake-, Preset-, Doku-, Ledger-, Konfigurations- oder Agentendatei durch diesen Writer geändert; keine Git-Operation, keine Bereinigung, keine Retries.

`observed` — Verifikation durch den Tester mit realen Exit-Codes:
- `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"` → `Configuring done (2.7s)`, `Generating done (0.3s)`, `EXIT=0`.
- `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"` → `xbridge_core.lib`, `xbridge.exe`, `test_main.cpp`, `xbridge_tests.exe`, `EXIT=0`.
- `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"` → `1/1 Test #1: xbridge_tests .................... Passed 0.03 sec`, `100% tests passed, 0 tests failed out of 1`, `Total Test time (real) = 0.06 sec`, `EXIT=0`.
- `git diff --check; "EXIT=$LASTEXITCODE"` → nur die bekannten CRLF-Warnungen für `CMakePresets.json` und `src/host/execution_policy.cpp`, `EXIT=0`.

`observed` — Damit ist die ACTIONABLE one-past-end Wurfstelle behoben und der vorherige CTest-Abbruch im aktuellen Binary nicht mehr reproduziert. `unknown` bleibt die Kausalität zu den beiden früheren, nur vom Owner gemeldeten Dialogen ohne Exit-Code oder Stack-Trace. Der einzige Reviewer-Re-Check läuft in `ses_df0b6d8abffeH5UQW4HfJLpzjN`.

`observed` — Unabhängiger Reviewer-Re-Check (`ses_df0b6d8abffeH5UQW4HfJLpzjN`, read-only, kein Build/CTest-Re-Run): **PASS**. Positiv verifiziert: `tests/test_main.cpp:1914` = `GuestMemory(512)`, Load bei `:1915` nun in Range nach `src/memory/memory.cpp:22-28,55-58`; keine Wurfstelle im ersten Block; Single-Call bei `:2077` direkt nach `:2076`, kein Start-Call; vollständiger Testkörper `:1868-2022` mit allen geforderten Fällen erhalten; finales `0x107` konsistent mit NOP 1 + MOV imm32 5 + NOP 1 und `src/cpu/x64_instruction_interpreter.cpp:46-60`; keine Produkt-/CMake-/Preset-/Config-/Doku-/Ledger-Änderung im Korrekturpaket. Tester-Exit-Codes als Paketevidenz akzeptiert; `xbridge_tests.exe` im Build-Verzeichnis vorhanden.

`documented` — Reviewer-Klassifizierung: `ACTIONABLE 0`, `OWNER_DECISION 0`, `NON_BLOCKING 2`, `UNKNOWN 1`, keine Findings über LOW. `XB-X64-CORR-01-U1` (`UNKNOWN`): kein unabhängiger Byte-Vergleich zwischen Quelle und Binary, weil der Reviewer read-only ohne Build/CTest-Re-Run arbeitete; kein Korrekturbedarf. `XB-X64-CORR-01-R1` (`NON_BLOCKING`): `std::terminate`/`abort`-Kette bleibt `hypothesis` (`docs/AGENT_TASKS.md:2015`); kein Korrekturbedarf. `XB-X64-CORR-01-R2` (`NON_BLOCKING`): bekannte CRLF-Warnungen, `EXIT=0`; kein Korrekturbedarf. **Case A** — keine ACTIONABLE-Findings, keine Korrektur nach Review, kein zweiter Review.

```yaml
PACKAGE_ID: XB-X64-RIP-STEP-01-CORR-01
PARENT_ID: XB-X64-RIP-STEP-01
DEPENDENCIES: XB-X64-RIP-STEP-01
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: 1/1
```

## XB-CHANGE-COMPLETION — einmalige Gesamtplanung

**development infrastructure — nicht Teil des Produktpakets.** `observed` — Der aktuelle Bestand wurde mit `git status --porcelain=v1 --untracked-files=all` neu erfasst: 31 Einträge, 23 modified und 8 untracked, Branch `docs/opencode-publication-group-2`, HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`, Index leer. `git diff HEAD --no-ext-diff --stat` → 23 getrackte Dateien, `3013 insertions(+), 49 deletions(-)`. Damit ist die ältere Zählung (24/25/26/32 Einträge) historisch, nicht aktuell.

`observed` — Der einmalige read-only Architektur-/Abhängigkeitscheck `ses_df09ca07effeO8w2ng4UYLmpx4` ordnet alle 31 Einträge elf Gruppen zu: G1 CPU-Step-Produktpfad (3 Dateien, pass/reviewer PASS); G2 Host-Capability-Seam (3, pass/reviewer PASS); G3 Workflow/Ledger/Roadmap (3, gemischt historisch/absgeschlossen, Restkorrektur); G4 Produktarchitektur-Dokument (1, `NEEDS_REVIEW`, Owner-Pfad); G5 Setup/Portabilität (2, abgeschlossen); G6 Agentenarchitektur/Automatisierung (6, XB-P2 PASS); G7 Research/Register (2, PASS); G8 Agentenkonfiguration (7, Owner-Territorium); G9 Root-Agentenregeln (1, Owner-Entscheidung); G10 Build-Preset/Ignore-Regeln (2, Owner-Entscheidung/CRLF bekannt); G11 `ai/README.md` (1, E1 Owner-Patch nicht angewendet). Sechs Einträge sind Produktcode/Tests, 25 gehören zur Entwicklungsinfrastruktur oder Owner-Konfiguration.

`documented` — Abhängigkeitsreihenfolge: Owner-Konfiguration/Root-Regeln/Build-Preset → Workflow/Ledger → Research-Schema → Architektur/Produktverträge → gemeinsame Tests → Dokumentationsschichten → Owner-Publikation. Nicht erneut geöffnet: A2, B1, C1, RIP-Paket und Korrektur, SETUP-005, XB-0080-A/B, XB-0081, XB-0084-Assessment, XB-0085, XB-0086, RV-P1-Reihe, XB-P2, XB-CONTINUATION-01/-02, XB-X64-CORPUS-01 und XB-CMAKE-VERIFY-RESUME. Keine neue Roadmap-Funktion, kein JIT, keine Abhängigkeit, keine Veröffentlichung.

`documented` — Klassifizierung des Architektur-Eingangs: `ACTIONABLE` = veraltete Roadmap-Aussagen bei `docs/ROADMAP.md:27,28,45,49,53` und fehlende aktuelle 31-Eintrags-Notiz; `ACTIONABLE`/Owner-Pfad = Architekturdokument `docs/ARCHITECTURE.md:165-168,229-231` beschreibt die implementierten öffentlichen Produkt-Schnittstellen nicht; `OWNER_DECISION` = E1/E2/E3, F1, SETUP-004/006/007, Publikation und die Edit-Route für `docs/ARCHITECTURE.md`; `NON_BLOCKING` = CRLF-Warnungen und stabile Register-Selbstkonsistenz; `UNKNOWN` = Laufzeitwirkung der Code-Mode-Denys und Herkunft alter Zählungs-Snapshots. Keine ACTIONABLE-Produkt-code-Findings.

```text
PACKAGE_ID:        XB-CHANGE-COMPLETION
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  development-only
GOAL:              Alle 31 bestehenden lokalen Changes ohne neue reguläre Feature-Entwicklung in einen konsistenten, überprüften Abschlusszustand bringen.
FILES:             sieben Arbeitsgruppen; genaue Pfade in den Unterpaketen
ALLOWED:           Fertigstellung, Korrektur, Integration, Absicherung und Dokumentation bereits vorhandener Changes; coordinator-serialisierte Doku-Pfade; installierte Writer mit genau einem Writer je Pfad.
FORBIDDEN:         neue Roadmap-Features, JIT, neue Abhängigkeiten, Stage/Commit/Push/Merge/Rebase/Tag/PR/Release/Publication, destruktive Bereinigung, Rechteerweiterung, Agent-/KI-Inhalt in Produktcode, Wiederholung abgeschlossener Pakete.
ACCEPTANCE:        alle 31 Einträge einem Paket mit Abschlussstatus zugeordnet; alle umsetzbaren Korrekturen angewandt; finaler Build/CTest/diff-check; umfassendes unabhängiges Review ohne offene umsetzungsrelevante Findings; echte Blocker als OWNER_DECISION dokumentiert.
TESTS:             finaler Debug-Build und CTest durch tester mit den protokollierten absoluten VS2026-Pfaden; git diff --check; Dokumentations-Read-back; kein Build für reine Doku-Korrekturen.
REVIEW:            umfangreiche unabhängige Abschlussprüfung nach allen Korrekturen; zusätzliche gezielte Reviews für Doku-Korrekturpakete.
WORKTREE:          none
OWNER_APPROVALS:   nur für owner-territoriale Pfade/Entscheidungen; keine technische Routineentscheidung.
```

```yaml
PACKAGE_ID: XB-CHANGE-COMPLETION
PARENT_ID: XB-TECHNICAL-DELEGATION
DEPENDENCIES: XB-X64-RIP-STEP-01-CORR-01, XB-TECHNICAL-DELEGATION
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: 6/6
```

`documented` — Sechs objektive Arbeitseinheiten der Gesamtplanung: (1) Bestandsaufnahme/Abhängigkeitsplan — erledigt; (2) Roadmap/Ledger-Aktualisierung — umgesetzt, Read-back, `git diff --check` und Reviewer-Re-Check `PASS`; (3) Architekturdokumentationspfad — agentenseitig abgeschlossen, weil der vollständige Patch vorbereitet und als Owner-Entscheidung dokumentiert ist; die Anwendung selbst bleibt owner-only und wird nicht als erledigt gezählt; (4) Owner-Territorium und informative Blocker dokumentiert — erledigt; (5) abschließender Build/CTest/diff-check — erledigt, Tester-`PASS` mit echten Exit-Codes; (6) umfassendes unabhängiges Abschlussreview — erledigt, Reviewer-Re-Check `PASS` mit `ACTIONABLE 0`. `PROGRESS_UNITS: 6/6` zählt die sechs agentenseitig abgeschlossenen Einheiten. Die in `:2077` genannten Owner-Entscheidungen betreffen Publikation, ADR, Lizenz, Rechte, Session/Worktree und owner-territoriale Patche; sie blockieren nicht die agentenausführbare technische Dokumentationsarbeit dieses Eintrags. Deshalb bleibt `BLOCKER_OWNER_DECISION: none` für dieses Paket, während die Owner-Punkte separat geführt werden.

`observed` — Abschlusszuordnung aller 31 aktuellen Working-Tree-Einträge. Statusangaben sind lokale Paketstatuswerte; `needs_owner` bedeutet nicht, dass eine Agentenkorrektur fehlt.

| Working-tree entry | Paket / Gruppe | Abschlussstatus | Restarbeit aus Agentensicht |
|---|---|---|---|
| `.gitignore` | G10 / RV-P1-C3 | `pass` | keine |
| `.opencode/agents/architect.md` | G8 / SETUP-007, RV-P1-C3 | `pass` (lokaler Dateizustand) | SETUP-007-Rollout owner-only |
| `.opencode/agents/coordinator.md` | G8 / SETUP-007, RV-P1-C3 | `pass` (lokaler Dateizustand) | SETUP-007-Rollout owner-only |
| `.opencode/agents/developer.md` | G8 / SETUP-007, RV-P1-C3 | `pass` (lokaler Dateizustand) | SETUP-007-Rollout owner-only |
| `.opencode/agents/researcher.md` | G8 / SETUP-007, RV-P1-C3 | `pass` (lokaler Dateizustand) | SETUP-007-Rollout owner-only |
| `.opencode/agents/reviewer.md` | G8 / SETUP-007, RV-P1-C3 | `pass` (lokaler Dateizustand) | SETUP-007-Rollout owner-only |
| `.opencode/agents/tester.md` | G8 / SETUP-007, RV-P1-C3 | `pass` (lokaler Dateizustand) | SETUP-007-Rollout owner-only |
| `AGENTS.md` | G9 / E2 | `needs_owner` | E2: dauerhafte Versionierung/Publikation |
| `CMakePresets.json` | G10 / SETUP-005 | `pass` (lokal verifiziert) | E3: Behandlung der lokalen VS2026-Änderung und des Backups; CRLF `NON_BLOCKING` |
| `ai/README.md` | G11 / E1 / XB-OWNER-DOC-IMPLEMENTATION-02 | `pass` (lokal angewendet, Reviewer `PASS`) | keine; Publikation bleibt owner-only (F1) |
| `docs/AGENT_TASKS.md` | G3 / XB-CHANGE-COMPLETION | `pass` | keine; finale Verifikation und Abschlussreview protokolliert |
| `docs/AGENT_WORKFLOW.md` | G3 / XB-0081, RV-P1 | `pass` | keine |
| `docs/ARCHITECTURE.md` | G4 / XB-OWNER-DOC-IMPLEMENTATION-02 | `pass` (lokal angewendet, Reviewer `PASS`) | keine; Publikation bleibt owner-only (F1) |
| `docs/PORTABLE_SETUP.md` | G5 / XB-0086-C1, RV-P1-C1/C2 | `pass` | SETUP-004/006 bleiben owner-only |
| `docs/RESEARCH_REGISTER.md` | G7 / B1 | `pass` | keine |
| `docs/ROADMAP.md` | G3 / XB-CHANGE-COMPLETION-ALIGN-01 | `pass` | keine |
| `include/xbridge/cpu/x64_instruction_interpreter.hpp` | G1 / A2, RIP | `pass` | keine |
| `include/xbridge/host/capabilities.hpp` | G2 / C1 | `pass` | keine |
| `include/xbridge/host/execution_policy.hpp` | G2 / C1 | `pass` | keine |
| `opencode.jsonc` | G8 / RV-P1-C3 | `pass` (lokaler Dateizustand) | Laufzeitwirkung einzelner Denys `unknown`; SETUP-007 owner-only |
| `src/cpu/x64_instruction_interpreter.cpp` | G1 / A2, RIP | `pass` | keine |
| `src/host/execution_policy.cpp` | G2 / C1 | `pass` | CRLF `NON_BLOCKING` |
| `tests/test_main.cpp` | G1/G2 / A2, C1, RIP-CORR, CORPUS-01 | `pass` | keine; finaler CTest `PASS` protokolliert |
| `docs/AGENT_AUTONOMY.md` | G6 / XB-P2 | `pass` | keine |
| `docs/AGENT_CORE_ARCHITECTURE.md` | G6 / XB-P2 | `pass` | keine |
| `docs/AUTOMATED_QUALITY_GATES.md` | G6 / XB-P2 | `pass` | keine |
| `docs/DESKTOP_MIGRATION_CHECKLIST.md` | G6 / XB-P2 | `pass` | keine |
| `docs/DEVELOPMENT_ENVIRONMENT.md` | G5/G6 / XB-0086-C1, XB-P2 | `pass` | keine |
| `docs/GITHUB_AGENT_WORKFLOW.md` | G6 / XB-P2 | `pass` | keine |
| `docs/OPENCODE_AUTOMATION_PLAN.md` | G6 / XB-P2 | `pass` | keine |
| `docs/research/XB-0083_AUTHORITATIVE_EVIDENCE.md` | G7 / B1 | `pass` | keine |

`documented` — Konsolidierte Owner-Only-Liste dieses Abschlusses: E1 (`ai/README.md`-Patch anwenden oder verwerfen), E2 (`AGENTS.md`-Versionierung), E3 (`CMakePresets.json`/Backup-Behandlung), F1 (Publikationsgruppierung/Zählung; aktuell 31 Einträge, ältere 24/25/26/32 nur historisch), `XB-CHANGE-COMPLETION-ARCH-DOC` (Patch anwenden oder scoped Coordinator-Edit-Regel gewähren), SETUP-004 (Session-Export/Import), SETUP-006 (Worktree-Cleanup), SETUP-007 (Spezialisteninstallation/Rechte), alle Stage-/Commit-/Push-/Merge-/PR-/Tag-/Release-Vorgänge, ADR-/Lizenz-/Abhängigkeits-/JIT-/Kompatibilitätsentscheidungen. Keine dieser Positionen wird von Agenten ausgeführt oder als erledigt markiert.

## Wave 1 — `XB-CHANGE-COMPLETION-ALIGN-01`

```text
PACKAGE_ID:        XB-CHANGE-COMPLETION-ALIGN-01
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Veraltete Roadmap- und Statusaussagen an den tatsächlich abgeschlossenen 31-Eintrags-Stand anpassen.
FILES:             docs/ROADMAP.md; docs/AGENT_TASKS.md
ALLOWED:           docs/ROADMAP.md:27,28,45,49,53 auf pass-Status korrigieren; aktuelle 31-Eintrags-Notiz und historische Snapshots im Ledger ergänzen; alte Zählungen ausdrücklich als historisch markieren.
FORBIDDEN:         Produktcode, Tests, CMake, Presets, Konfiguration, Agent-Dateien, ai/**, AGENTS.md, docs/ARCHITECTURE.md, Publikation, Wiederöffnung abgeschlossener Reviews.
ACCEPTANCE:        keine Aussage mehr, dass A2/C1/B1/RIP-blockiert/planned seien; 31-Eintrags-Stand dokumentiert; alte Zahlen als historisch gekennzeichnet; read-back und git diff --check mit echten Exit-Codes; EIN Reviewer-Check dieses Diffs.
TESTS:             Read-back der geänderten Abschnitte; git diff --check; "EXIT=$LASTEXITCODE". Kein Build/CTest für reine Dokumentation.
REVIEW:            reviewer, read-only, EIN Check nach stabilem Writer-Report.
WORKTREE:          none
OWNER_APPROVALS:   keine; beide Pfade liegen im bestätigten Coordinator-Schreibrecht.
```

`documented` — Rechtecheck: `coordinator.md:8-43` erlaubt `docs/AGENT_TASKS.md` und `docs/ROADMAP.md`; `developer` und `tester` schreiben diese Pfade nicht; kein Konflikt mit Owner-Wave. EIN Writer: coordinator.

`observed` — Writer `coordinator` änderte genau die freigegebenen Stellen in `docs/ROADMAP.md`: `:27` = RIP-Brücke lokal abgeschlossen mit Reviewer `PASS`, Run-Loop weiterhin nicht begonnen; `:28` = XB-0080-A/B und XB-0083 als abgeschlossen, XB-0082 evaluation-only/owner-gated, alte Zählungen historisch; `:45` = aktueller Bestand 31 Einträge (23 modified, 8 untracked), F1 weiterhin Owner-Entscheidung; `:49` = A2/C1/B1 implementiert und reviewer-PASS, alte Fehlschläge historisch; `:53` = RIP-Brücke abgeschlossen, Run-Loop zukünftig. Read-back `docs/ROADMAP.md:24-55` bestätigt diese Zeilen. Keine Produkt-, Test-, CMake-, Preset-, Konfigurations-, Agenten-, `ai/**`- oder `AGENTS.md`-Änderung; kein Build/CTest, da reine Dokumentation.

`observed` — `git diff --check; "EXIT=$LASTEXITCODE"` → nur die bekannten CRLF-Warnungen für `CMakePresets.json` und `src/host/execution_policy.cpp`, `EXIT=0`. Der gezielte Reviewer-Check läuft in `ses_df09503abffe7nOmUyU2fK714J`.

`observed` — Reviewer `ses_df09503abffe7nOmUyU2fK714J` gab **NEEDS_CHANGE** für Wave 1: `XB-ALIGN-REV-01` (MEDIUM, `ACTIONABLE`) = veraltete Top-Level-Metadaten `PROGRESS_UNITS: 1/6` und Einheit (2) „offen“ sowie nicht abgeglichenes `BLOCKER_OWNER_DECISION: none`; `XB-ALIGN-REV-02` (LOW, `ACTIONABLE`) = unzulässige Evidence-Kennzeichnung `` `proposed` `` im damaligen `:2126` (historische Zeilennummer, nicht aktuell). `OWNER_DECISION 0`, `NON_BLOCKING 0`, `UNKNOWN 0`. **Case B**: beide Findings haben dieselbe Root Cause (unvollständige Ledger-Nachführung des eigenen Doku-Diffs) und werden in EINEM gebündelten Korrekturpaket für den einzigen Writer korrigiert; kein Finding wurde downgraded.

## Bundled correction — `XB-CHANGE-COMPLETION-ALIGN-01-CORR-01`

```text
PACKAGE_ID:        XB-CHANGE-COMPLETION-ALIGN-01-CORR-01
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Die beiden ACTIONABLE Review-Findings zur Ledger-Metadatenkonsistenz und Evidence-Taxonomie in einem Bund schließen.
FILES:             docs/AGENT_TASKS.md
ALLOWED:           PROGRESS_UNITS auf den tatsächlichen Stand setzen; Arbeitseinheit (2) als umgesetzt mit pending re-check beschreiben; BLOCKER_OWNER_DECISION: none explizit begründen; `proposed` durch erlaubte Evidence-Kennzeichnung ersetzen; einen sichtbaren Tippfehler im selben neu ergänzten Absatz korrigieren.
FORBIDDEN:         Produktcode, Tests, CMake, Presets, Konfiguration, Agent-Dateien, ai/**, AGENTS.md, docs/ARCHITECTURE.md, Publikation, Rechteerweiterung, Wiederholung abgeschlossener Pakete.
ACCEPTANCE:        Read-back zeigt keine veraltete „offen“-Aussage für Einheit (2), gültige YAML-Tokens, nur erlaubte Evidence-Labels; EIN erneuter Reviewer-Check.
TESTS:             Read-back der korrigierten Abschnitte; git diff --check; "EXIT=$LASTEXITCODE". Kein Build/CTest für reine Dokumentation.
REVIEW:            reviewer, read-only, EIN Re-Check nach stabilem Writer-Report.
WORKTREE:          none
OWNER_APPROVALS:   keine; docs/AGENT_TASKS.md liegt im bestätigten Coordinator-Schreibrecht.
```

`observed` — Writer `coordinator` setzte `PROGRESS_UNITS: 2/6`, änderte Einheit (2) zu „umgesetzt … Reviewer-Re-Check … pending“, begründete `BLOCKER_OWNER_DECISION: none` ausdrücklich gegenüber den separaten Owner-Punkten, ersetzte `` `proposed` `` durch `documented` und korrigierte „öffentlichenprodukt-Schnittstellen“ zu „öffentlichen Produkt-Schnittstellen“. Kein anderer Pfad wurde durch diese Korrektur berührt. Read-back `docs/AGENT_TASKS.md:2094-2141` bestätigt die geänderten Stellen (Zeilennummern als Snapshot zum Korrekturzeitpunkt); `git diff --check; "EXIT=$LASTEXITCODE"` → nur die bekannten CRLF-Warnungen, `EXIT=0`. Der obligatorische Re-Check läuft in derselben Reviewer-Session `ses_df09503abffe7nOmUyU2fK714J`.

`observed` — Re-Check derselben Reviewer-Session: **PASS**, 0 Findings, `ACTIONABLE 0`, `OWNER_DECISION 0`, `NON_BLOCKING 0`, `UNKNOWN 0`, keine Severity. Beide vorherigen ACTIONABLE-Findings sind closed; YAML-Token, Evidence-Labels, Write-Scope und Roadmap-Inhalt bleiben konsistent; kein Over-Correction. **Case A** für die Korrektur: kein weiterer Fix, kein dritter Review.

```yaml
PACKAGE_ID: XB-CHANGE-COMPLETION-ALIGN-01
PARENT_ID: XB-CHANGE-COMPLETION
DEPENDENCIES: XB-CHANGE-COMPLETION
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: 1/1
```

```yaml
PACKAGE_ID: XB-CHANGE-COMPLETION-ALIGN-01-CORR-01
PARENT_ID: XB-CHANGE-COMPLETION-ALIGN-01
DEPENDENCIES: XB-CHANGE-COMPLETION-ALIGN-01
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: 1/1
```

## Owner patch — `XB-CHANGE-COMPLETION-ARCH-DOC`

`documented` — `docs/ARCHITECTURE.md:165-168` und `:229-231` beschreiben die implementierten öffentlichen Produkt-Schnittstellen nicht mehr aktuell. Kein installierter Agent hat Schreibrecht auf diesen Pfad (`coordinator.md:5-7` ohne `docs/ARCHITECTURE.md`-Allow; `developer.md:20-22` explizit deny; `opencode.jsonc:119` `ask`, aber agent-seitig nicht freigegeben). Der folgende Patch ist vorbereitet, **nicht angewendet** und erfordert eine Owner-Entscheidung: Owner wendet ihn an oder gewährt eine explizit scoped Coordinator-Edit-Regel mit anschließendem Reload. Kein Agent erweitert Rechte oder simuliert eine bereits erfolgte Korrektur.

```diff
--- a/docs/ARCHITECTURE.md
+++ b/docs/ARCHITECTURE.md
@@ -168 +168,4 @@
-- Funktion: `execute_instruction(const DecodedInstruction&, X64CpuState&, GuestMemory&) noexcept`
+- Funktionen:
+  - `execute_instruction(const DecodedInstruction&, X64CpuState&, GuestMemory&) noexcept` führt eine bereits dekodierte Instruktion aus.
+  - `step_x64_instruction(std::span<const std::uint8_t>, X64CpuState&, GuestMemory&) noexcept` dekodiert höchstens eine Instruktion aus einem expliziten Byte-Puffer und führt sie aus; Non-Ok-Dekodierungen bleiben `Ok`-No-ops mit unverändertem RIP.
+  - `step_x64_at_rip(X64CpuState&, GuestMemory&) noexcept` liest höchstens 15 Bytes ab `state.rip` aus `GuestMemory` und reicht sie an `step_x64_instruction(...)` weiter. Die 15 ist ein lokales Lese-Limit, keine RIP-relative Guest-Adressierung; kein Status-Übersetzen und kein Run-Loop.
@@ -231 +234 @@
-- A future `HostCapabilities` service should return immutable, testable facts (CPU features/core counts, memory budgets, available graphics APIs/extensions/features, device limits, driver identity, and storage information). A separate `BackendSelector` applies policy and returns the selected backend plus a human-readable decision report. Detection must not be mixed into guest CPU/GPU emulation code. Provide deterministic mock capability sets for tests.
+The repository now contains a deterministic, host-only seam: `HostCapabilities` is immutable test data; `ICapabilityProvider` exposes it read-only; `FixedCapabilityProvider` stores a supplied set for deterministic tests; `select_execution_plan(provider, settings)` forwards to the pure `HostCapabilities` overload without platform probing or initialization. A future platform-specific detector may implement the same seam, but real OS/API probing is not implemented and remains a separate owner decision. `BackendSelector` remains a design concept; current selection is `select_execution_plan(...)`. Detection must not be mixed into guest CPU/GPU emulation code.
```

`documented` — Der E1-Patch für `ai/README.md:13,28` bleibt unverändert bei `:636-645` vorbereitet und ist ebenfalls owner-only; er wurde nicht angewendet. `unknown` — ob der Owner einen der beiden Patche anwenden oder als historische Owner-Entscheidung bestehen lassen möchte.

## Finale Verifikation — `XB-CHANGE-COMPLETION`, Einheit (5)

`observed` — Unabhängiger Tester `ses_df08f5fafffeEthpWBafDZarF6`, ausschließlich Lese-/Verifikationsumfang, keine Dateiänderung, kein Test-Downgrade, keine Publikation. Exakte Befehle und Resultate:

- `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"` → `Configuring done (8.6s)`, `Generating done (0.5s)`, `Build files have been written to: D:/XBridge 1.0/XBridge-Series-Github/build/windows-debug`, `EXIT=0`.
- `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"` → `xbridge_core.lib`, `xbridge.exe`, `xbridge_tests.exe` in `build/windows-debug/Debug`, `EXIT=0`.
- `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"` → `1/1 Test #1: xbridge_tests .................... Passed 0.47 sec`, `100% tests passed, 0 tests failed out of 1`, `Total Test time (real) = 0.52 sec`, `EXIT=0`.
- `git diff --check` → nur die beiden bekannten CRLF-Warnungen für `CMakePresets.json` und `src/host/execution_policy.cpp`, keine Fehlerzeilen. Der numerische Exit-Code wurde vom Shell-Tool in dieser Form nicht ausgewiesen → `unknown`; ein alternativer Befehl wurde nicht verwendet.

`observed` — Damit sind Configure, Build und CTest der vollständigen Debug-Konfiguration erneut erfolgreich; die drei bekannten langsamen CTest-Abbrüche des RIP-Pakets sind im aktuellen Binary nicht reproduziert. `documented` — Einheit (5) ist abgeschlossen; Einheit (6), die umfassende unabhängige Abschlussprüfung, ist ebenfalls abgeschlossen (siehe Korrektur- und Re-Check-Einträge am Dateiende).

`observed` — Umfassendes Abschlussreview `ses_df08e146dffemBeffD2SS4fMlj`: **NEEDS_CHANGE**. `XB-COMPLETION-REV-01` (MEDIUM, `ACTIONABLE`) = vorbereitete Owner-Patche hatten ungültige Unified-Diff-Hunk-Metadaten: E1 `@@ -10,7 +10,7 @@`/`@@ -26,3 +26,3 @@` entsprachen nicht den Body-Zeilen und der aktuellen Zielzeile `ai/README.md:28`; ARCH `@@ -231 +235 @@` war um eins versetzt. `XB-COMPLETION-REV-02` (LOW, `NON_BLOCKING`) = zwei historische Zeilenverweise im Ledger zeigten nicht mehr auf die beschriebenen Inhalte. `OWNER_DECISION 0`, `UNKNOWN 0`. Kein Produktcode-, Test-, Publikations- oder Rechtefinding. **Case B**: ein ACTIONABLE-Finding mit gemeinsamer Root Cause (Patch-Metadaten nicht gegen aktuellen Dateizustand validiert) wird in EINEM Bund korrigiert; das LOW/NON_BLOCKING-Finding wird im selben Ledger-Pfad mit niedrigem Risiko mitbereinigt, nicht downgraded.

## Bundled correction — `XB-CHANGE-COMPLETION-CORR-01`

```text
PACKAGE_ID:        XB-CHANGE-COMPLETION-CORR-01
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Die ACTIONABLE Patch-Metadatenkorrektur und die zwei veralteten historischen Zeilenverweise in einem Bund im Ledger abschließen.
FILES:             docs/AGENT_TASKS.md
ALLOWED:           E1-Hunks auf gültige Ein-Zeilen-Hunks `@@ -13 +13 @@` und `@@ -28 +28 @@` korrigieren; ARCH-Hunk auf `@@ -231 +234 @@` korrigieren; historische Zeilenverweise als Snapshot kennzeichnen; Review-Ergebnis und Writer-Report dokumentieren.
FORBIDDEN:         Produktcode, Tests, CMake, Presets, Konfiguration, Agent-Dateien, ai/**, AGENTS.md, docs/ARCHITECTURE.md, Publikation, Rechteerweiterung, Wiederholung abgeschlossener Pakete.
ACCEPTANCE:        Read-back zeigt gültige Hunk-Köpfe und genau treffende Minus-Zeilen; historische Verweise als Snapshot markiert; kein Owner-Territorium editiert; git diff --check ohne Fehlerzeilen; EIN erneuter Reviewer-Check.
TESTS:             Read-back der korrigierten Abschnitte; git diff --check. Kein Build/CTest für reine Ledger-Dokumentation; finale Produktverifikation ist bereits mit EXIT=0 protokolliert.
REVIEW:            reviewer, read-only, EIN Re-Check nach stabilem Writer-Report.
WORKTREE:          none
OWNER_APPROVALS:   keine; docs/AGENT_TASKS.md liegt im bestätigten Coordinator-Schreibrecht.
```

`observed` — Writer `coordinator` ersetzte die E1-Hunk-Köpfe durch `@@ -13 +13 @@` und `@@ -28 +28 @@`, wodurch die Minus-Zeilen exakt `ai/README.md:13` und `:28` entsprechen; der ARCH-Hunk wurde auf `@@ -231 +234 @@` korrigiert (Netto-+3 aus dem ersten Hunk). Die zwei veralteten Verweise sind jetzt ausdrücklich als historische Zeilennummern/Snapshots gekennzeichnet; die durch die kürzeren Patchblöcke verschobenen Verweise wurden auf `:636-645` und `:2219-2231` aktualisiert. Kein Owner-Territorium, Produktcode, Test oder Konfigurationspfad wurde berührt. Read-back der korrigierten Abschnitte bestätigt die Hunk-Köpfe und Minus-Zeilen; `git diff --check; "EXIT=$LASTEXITCODE"` → nur die bekannten CRLF-Warnungen, `EXIT=0`. Der obligatorische Re-Check läuft in derselben Reviewer-Session `ses_df08e146dffemBeffD2SS4fMlj`.

`observed` — Re-Check derselben Reviewer-Session: **PASS**. Beide vorherigen Findings sind closed: die E1- und ARCH-Hunk-Köpfe sind gültig, die Minus-Zeilen treffen die aktuellen Zielzeilen byte-genau; Klassifizierung: `ACTIONABLE 0`, `OWNER_DECISION 0`, `UNKNOWN 0`. Ein neues LOW/`NON_BLOCKING`-Finding betrifft vier ältere Ledger-Einträge, die noch Patchbereiche `:634-648`/`:634-649` zitieren; diese Bereiche enthalten den aktuellen Block `:636-645` weiterhin und sind daher nicht fehlerhaft, sondern nur historisch ungenau. **Case A**: kein `ACTIONABLE`, keine Korrektur, kein dritter Review. Der Hinweis bleibt als `NON_BLOCKING` dokumentiert, ohne eine Owner-Entscheidung oder Produktarbeit auszulösen.

```yaml
PACKAGE_ID: XB-CHANGE-COMPLETION-CORR-01
PARENT_ID: XB-CHANGE-COMPLETION
DEPENDENCIES: XB-CHANGE-COMPLETION
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: 1/1
```

`documented` — Abschluss des Gesamtpakets: alle sechs Arbeitseinheiten erledigt, `STATUS: pass`, `REVIEW_STATUS: PASS`, `PUBLICATION_STATUS: local_only`. Alle 31 Working-Tree-Einträge sind zugeordnet, alle agentenausführbaren Korrekturen angewandt, finale Build-/CTest-/Diff-Prüfung und umfassendes Review protokolliert. Nicht erledigt und ausdrücklich owner-only: E1, E2, E3, F1, `XB-CHANGE-COMPLETION-ARCH-DOC`, SETUP-004/006/007 und jede Git-/GitHub-Publikation.

## `XB-OWNER-DOC-IMPLEMENTATION-01` — Inhaltsfreigabe, Rechteprüfung, technische Blockade

`documented` — Der Projekteigner hat mit `XB-OWNER-DOC-IMPLEMENTATION-01` beide vorbereiteten Dokumentationsänderungen ausdrücklich **inhaltlich** freigegeben und die direkte Umsetzung verlangt: `docs/ARCHITECTURE.md` (bisheriger Kanon `:2219-2231`) und `ai/README.md` (Zeilen 13 und 28; bisheriger Kanon `:636-645`). Zusätzlich sind zwei redaktionelle Präzisierungen freigegeben, die in den bisherigen Patchblöcken **noch nicht** enthalten sind: (a) `ai/README.md:3` → „Agents work in the owner-provided checkout; branch changes and worktrees require owner action."; (b) `docs/ARCHITECTURE.md:231` → „`HostCapabilities` is immutable test data" wird zu „treated as immutable by this seam". `observed` — Der Block `:636-645` enthält nur die Hunks `@@ -13 +13 @@` und `@@ -28 +28 @@`, keinen Hunk für Zeile 3; `:2230` enthält weiterhin „is immutable test data". Für beide Präzisierungen ist daher vor der Anwendung ein korrigierter vollständiger Patchtext zu erstellen; die Freigabe deckt diese Arbeit inhaltlich ab, aber ein Agent darf sie wegen der unten dokumentierten Rechte nicht selbst ausführen.

`observed` — Wirksame Schreibrechte, rein lesend geprüft: `coordinator.md:5-7` setzt `edit "*" = deny`; die anschließenden Allows betreffen ausschließlich `docs/AGENT_TASKS.md`, `docs/AGENT_WORKFLOW.md`, `docs/ROADMAP.md`, `docs/RELEASE_CHECKLIST.md`, `docs/PORTABLE_SETUP.md`, `docs/DEVELOPMENT_ENVIRONMENT.md`, `docs/AGENT_AUTONOMY.md`, `docs/AGENT_CORE_ARCHITECTURE.md`, `docs/OPENCODE_AUTOMATION_PLAN.md`, `docs/AUTOMATED_QUALITY_GATES.md`, `docs/GITHUB_AGENT_WORKFLOW.md` und `docs/DESKTOP_MIGRATION_CHECKLIST.md`. Weder `docs/ARCHITECTURE.md` noch `ai/**` sind freigegeben. `developer.md:20-22` verweigert `docs/ARCHITECTURE.md`, `:35-37` verweigert `ai/**`. `opencode.jsonc:119` setzt `docs/ARCHITECTURE.md` auf `ask`; nach `docs/AGENT_WORKFLOW.md` („global rules are loaded before agent rules; the last matching rule wins") greift die spätere Agenten-`*`-Regel als deny. Die Konfigurationsdateien sind gegenüber der zuvor protokollierten Prüfung unverändert; eine scoped Coordinator-Edit-Regel wurde nicht hinterlegt.

`documented` — Eine Inhaltsfreigabe ersetzt keine technische Edit-Berechtigung. Agenten erweitern keine Rechte und umgehen keine Verweigerung. Deshalb wurden `docs/ARCHITECTURE.md` und `ai/README.md` in diesem Paket **nicht** verändert; beide Patche bleiben bei `:636-645` und `:2219-2231` vorbereitet und review-validiert. Der einzige zulässige Writer `coordinator` hat ausschließlich `docs/AGENT_TASKS.md` bearbeitet: TD.6 wurde auf den kanonischen E1-Wortlaut vereinheitlicht und dieser Abschnitt angefügt. `OWNER_DECISION` — der Eigner wendet die beiden bestehenden Patche `:636-645` und `:2219-2231` sowie die zwei freigegebenen, noch nicht eingearbeiteten Präzisierungen (`ai/README.md:3` und „treated as immutable by this seam" in `docs/ARCHITECTURE.md`) selbst an oder lässt einen korrigierten vollständigen Patchtext erstellen und anwenden; alternativ hinterlegt er eine explizit scoped, nur diese beiden Pfade betreffende Coordinator-Edit-Regel in `.opencode/agents/coordinator.md` und lädt die Sitzung neu. Erst danach kann ein Writer die Änderungen technisch umsetzen. Dies ist kein erneutes Einholen einer Inhaltsfreigabe, sondern die Meldung eines technischen Rechteblockers.

`observed` — Kein Build und kein CTest in diesem Paket; reine Dokumentations- und Rechteprüfung. Es wurden keine Git-Publikations-, Staging- oder Historienoperationen ausgeführt. Die einzige Git-Operation war die Leseprüfung `git diff --check; "EXIT=$LASTEXITCODE"` → nur die bekannten CRLF-Warnungen für `CMakePresets.json` und `src/host/execution_policy.cpp`, `EXIT=0`. Die Minus-Zeilen beider Patche treffen weiterhin die aktuellen Zielzeilen, bestätigt durch den Re-Check `ses_df08e146dffemBeffD2SS4fMlj` (`PASS`, `ACTIONABLE 0`). Eine erneute vollständige Projektinventur oder Wiederholung abgeschlossener Build-/Testprüfungen wurde nicht durchgeführt.

`observed` — Reviewer `ses_df08097a4ffeujiVO6MPEKfwrl`: `NEEDS_CHANGE`; `ACTIONABLE 2` (F-1 MEDIUM: neue Sektion stellte die zwei freigegebenen Präzisierungen fälschlich als bereits im Kanon enthalten dar; F-2 LOW: pauschale Aussage „kein Git-Kommando" widersprach der protokollierten Pflichtprüfung `git diff --check`). Klassifikation: **Case B**, beide Findings nach gemeinsamer Ursache „Ledger-Text der neuen Sektion" gebündelt, kein Finding downgraded. `OWNER_DECISION 1` (Rechteblocker) bleibt unverändert offen und ist kein Korrekturgegenstand.

```text
PACKAGE_ID:        XB-OWNER-DOC-IMPLEMENTATION-01-CORR-01
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Ledger-Aussagen zur Inhaltsfreigabe und zur Verifikation mit dem tatsächlichen Patch-/Befundstand abgleichen.
FILES:             docs/AGENT_TASKS.md (sole writer: coordinator; explizit erlaubt durch coordinator.md:8-10)
ALLOWED:           Präzisierung der Absätze bei :2286, :2290, :2292; Aufnahme des gebündelten Korrekturprotokolls.
FORBIDDEN:         Bearbeitung von docs/ARCHITECTURE.md, ai/README.md, Produktcode, Headern, Tests, CMake, Konfiguration, ADRs, .opencode/**, opencode.jsonc; Rechteerweiterung; Git-Publikation; erneute Inhaltsfreigabe.
ACCEPTANCE:        Beide Präzisierungen ausdrücklich als nicht eingearbeitet und mit exaktem Zieltext protokolliert; OWNER_DECISION nennt beide beim Namen; git diff --check ohne Fehlerzeilen; EIN erneuter Reviewer-Check.
TESTS:             Kein Build/CTest für reine Ledger-Dokumentation; git diff --check. Finale Produktverifikation bleibt unverändert protokolliert.
REVIEW:            reviewer, read-only, EIN Re-Check nach stabilem Writer-Report.
WORKTREE:          none
OWNER_APPROVALS:   keine für diese Ledger-Korrektur; die Inhaltsfreigabe des Owners liegt vor, der Rechteblocker bleibt OWNER_DECISION.
```

`observed` — Writer `coordinator` trennte in `:2286` den geprüften Kanon (nur Zeilen 13/28 bzw. alter ARCH-Wortlaut) von den zwei neu freigegebenen, noch nicht eingearbeiteten Präzisierungen (`ai/README.md:3`, „treated as immutable by this seam") und nannte den Bedarf eines korrigierten vollständigen Patchtextes. Die `OWNER_DECISION` in `:2290` wurde um beide Präzisierungen beim Namen erweitert. `:2292` unterscheidet jetzt zwischen fehlenden Build-/CTest-/Publikationsoperationen und der tatsächlich ausgeführten Leseprüfung `git diff --check; "EXIT=$LASTEXITCODE"` mit den zwei CRLF-Warnungen und `EXIT=0`. Keine Datei außer `docs/AGENT_TASKS.md` wurde verändert; die Zielpatche bleiben nicht angewendet.

`observed` — Re-Check derselben Reviewer-Session: **PASS** (**Case A**). Beide vorherigen `ACTIONABLE`-Findings sind gegen ihre Erfolgskriterien geschlossen; `ACTIONABLE 0`, `NON_BLOCKING 0`, `UNKNOWN 0`. Keine neue Korrektur und kein drittes Review. `OWNER_DECISION 1` bleibt offen; der in `:2290` dokumentierte Rechteblocker erhält die Kennung `OWNER-DOC-EDIT-RIGHTS`.

```yaml
PACKAGE_ID: XB-OWNER-DOC-IMPLEMENTATION-01
PARENT_ID: XB-CHANGE-COMPLETION
DEPENDENCIES: XB-CHANGE-COMPLETION
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — Der frühere Blocker `OWNER-DOC-EDIT-RIGHTS` wurde in `XB-OWNER-DOC-IMPLEMENTATION-02` durch die Owner-Anwendung der scoped Regel und den nachfolgenden Neustart gelöst; beide Dokumentationsänderungen sind dort umgesetzt. Die historischen Review-Ergebnisse dieses Pakets bleiben unverändert.

`documented` — Abschlussstatus: Die zulässige Ledger-Arbeit ist abgeschlossen und reviewer-geprüft (`PASS`); die technische Umsetzung der beiden Dokumentationspatche ist durch verifizierte Schreibrechte blockiert und bleibt `local_only`. `docs/ARCHITECTURE.md` und `ai/README.md` sind unverändert, alle sonstigen lokalen Änderungen wurden erhalten. Keine Git-Publikation, kein Stage/Commit/Push/Merge/Rebase/Reset/Branch-Wechsel/Tag, kein Build und kein CTest in diesem Paket.

## `XB-OWNER-PERMISSION-FIX-01` — Scoped Dokumentationsrechte

`documented` — Der Owner hat ausdrücklich eine eng begrenzte Schreibberechtigung für genau `docs/ARCHITECTURE.md` und `ai/README.md` genehmigt. Nach `docs/AGENT_WORKFLOW.md` („Not automatable by agents: editing `.opencode/**` or `opencode.jsonc` ... require one owner action each") bleiben Konfigurationsdateien für Agenten read-only; die Genehmigung berechtigt nicht den Agenten, sie selbst zu schreiben. Bis zur Anwendung durch den Owner ist `PERMISSION_STATE: blocked` und die Dokumentation darf nicht umgesetzt werden (Freigabebedingung: „Erst wenn die Berechtigungen nachweislich korrekt sind").

`observed` — Vollständige Rechteprüfung, rein lesend:
- Global `opencode.jsonc:74-129`: Shell `*` = ask (`:77`), keine globale `edit "*"`-Regel; `edit` für `.git/**` deny (`:115-116`), `AGENTS.md`/`docs/DECISIONS.md`/`docs/ARCHITECTURE.md`/`opencode.jsonc`/`.opencode/**` = ask (`:117-121`). Keine globale Regel für `ai/**`. Harte `policies` für Git-/GitHub-Publikation und Installation (`:131-150`) sind nicht agent-scoped und nicht durch eine Permission-Regel aufhebbar.
- `coordinator.md:5-7` `edit "*" = deny`; Allows nur `:8-43`; Denys `:44-55`; Subagent-Allow `:56-70`; Shell-Deny/`opencode *`-Deny `:74-85`; Web-Deny `:116-121`. Kein Allow für die zwei Zielpfade.
- `developer.md:5-7` deny-all; Allows `:8-19`; explizite Denys `docs/ARCHITECTURE.md` `:20-22` und `ai/**` `:35-37`.
- `architect.md:5-7`, `reviewer.md:5-7`, `tester.md:5-7`, `researcher.md:5-7` deny-all; nur eng begrenzte eigene Allows (`tests/**`, `docs/RESEARCH_REGISTER.md`, `docs/research/**`). Kein Target-Pfad dabei.
- Vorrang nach `AGENT_WORKFLOW.md:94-96`: globale Regeln zuerst, Agentenregeln danach, letzte passende Regel gewinnt; `deny` überschreibt gespeicherte Freigaben; Top-Level-`policies` sind harte Denys. Damit blockiert für `coordinator` die eigene `edit "*"`-Regel `:5-7`; die globale `ask`-Regel `:119` für `docs/ARCHITECTURE.md` wird von der späteren Agentenregel überschrieben. Für `ai/README.md` greift ebenfalls die Agenten-`*`-Regel.

`documented` — Minimale sichere Anpassung: **nur** `.opencode/agents/coordinator.md`, zwei pfadgenaue `edit`-Allows nach dem bestehenden Allow-Block. `opencode.jsonc` bleibt unverändert: eine globale `allow`-Regel würde die Rechte unnötig auf weitere Rollen ausweiten und ist nicht erforderlich, weil die spätere Agentenregel Vorrang hat. Keine Änderung an `developer.md`, `architect.md`, `reviewer.md`, `tester.md`, `researcher.md`, an Git-Regeln oder an `policies`.

```diff
--- a/.opencode/agents/coordinator.md
+++ b/.opencode/agents/coordinator.md
@@ -41,6 +41,12 @@
   - action: edit
     resource: "docs/DESKTOP_MIGRATION_CHECKLIST.md"
     effect: allow
+  - action: edit
+    resource: "docs/ARCHITECTURE.md"
+    effect: allow
+  - action: edit
+    resource: "ai/README.md"
+    effect: allow
   - action: edit
     resource: "docs/DECISIONS.md"
     effect: deny
```

`documented` — Nach Owner-Anwendung sind folgende Negativprüfungen zwingend und aktuell **nicht** erfüllt: Read-back der zwei Allows; `docs/ARCHITECTURE.md` und `ai/README.md` für `coordinator` beschreibbar; **keine zusätzlichen** `docs/**`- oder `ai/**`-Pfade über die bereits bestehende Allowlist `coordinator.md:8-43` hinaus beschreibbar, während alle vorbestehenden Denys `:44-55` unverändert in Kraft bleiben; Git-/Publikationsregeln unverändert; Syntax von `opencode.jsonc` und YAML-Frontmatter gültig. Keine dieser Aussagen wird vor der tatsächlichen Anwendung als `verified` ausgegeben.

`observed` — `docs/ARCHITECTURE.md:168,231` und `ai/README.md:3,13,28` bleiben in diesem Paket unverändert; keine Git-Operation außer der Leseprüfung `git diff --check; "EXIT=$LASTEXITCODE"` (bekannte CRLF-Warnungen, `EXIT=0`). Kein Build, kein CTest, keine Rechteerweiterung, keine Publikation.

`observed` — Reviewer `ses_df07a2e3bffeJ58ycZirnYdPVB`: `NEEDS_CHANGE`; `ACTIONABLE 1` (`XB-PERM-REV-02`, LOW: Negativprüfung behauptete pauschal, alle anderen `docs/**`-/`ai/**`-Pfade seien gesperrt, obwohl `coordinator.md:8-43` bereits mehrere `docs/**`-Pfade erlaubt). `OWNER_DECISION 1` (`XB-PERM-REV-01`, BLOCKER: erwarteter, noch nicht angewendeter Owner-Patch). Klassifikation: **Case B**, kein Finding downgraded; der Rechteblocker ist kein Korrekturgegenstand.

```text
PACKAGE_ID:        XB-OWNER-PERMISSION-FIX-01-CORR-01
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Negativprüfungs-Formulierung mit der tatsächlichen Coordinator-Allowlist abgleichen.
FILES:             docs/AGENT_TASKS.md (sole writer: coordinator; explizit erlaubt durch coordinator.md:8-10)
ALLOWED:           Präzisierung des Negativprüfungs-Absatzes und Aufnahme des Korrekturprotokolls.
FORBIDDEN:         Änderung von .opencode/**, opencode.jsonc, docs/ARCHITECTURE.md, ai/README.md, Produktcode, Tests, CMake, ADRs; Rechteerweiterung; Git-Publikation.
ACCEPTANCE:        Keine Behauptung, bereits erlaubte docs-Pfade seien gesperrt; vorbestehende Denys weiterhin genannt; git diff --check ohne Fehlerzeilen; EIN erneuter Reviewer-Check.
TESTS:             Kein Build/CTest für reine Ledger-Dokumentation; git diff --check.
REVIEW:            reviewer, read-only, EIN Re-Check nach stabilem Writer-Report.
WORKTREE:          none
OWNER_APPROVALS:   keine für diese Ledger-Korrektur; der Rechte-Patch bleibt OWNER_DECISION.
```

`observed` — Writer `coordinator` ersetzte die pauschale Sperr-Aussage durch die präzise Formulierung „keine zusätzlichen `docs/**`- oder `ai/**`-Pfade über die bestehende Allowlist `coordinator.md:8-43` hinaus" und nannte die unverändert bleibenden Denys `:44-55`. Keine Datei außer `docs/AGENT_TASKS.md` wurde verändert; der vorbereitete Owner-Patch blieb unverändert; die Zielpatche bleiben nicht angewendet. Verifikation dieser Korrekturwelle: `git diff --check; "EXIT=$LASTEXITCODE"` → nur die bekannten CRLF-Warnungen für `CMakePresets.json` und `src/host/execution_policy.cpp`, `EXIT=0`.

`observed` — Re-Check derselben Reviewer-Session: **PASS** (**Case A**). `XB-PERM-REV-02` ist gegen sein Erfolgskriterium geschlossen; `ACTIONABLE 0`, `UNKNOWN 0`. Ein `NON_BLOCKING`-Hinweis betraf die fehlende Diff-Check-Zeile im Korrekturblock; sie wurde soeben mit exaktem Befehl und `EXIT=0` ergänzt. `OWNER_DECISION 1` (`XB-PERM-REV-01`) bleibt offen und ist kein Korrekturdefekt.

```yaml
PACKAGE_ID: XB-OWNER-PERMISSION-FIX-01
PARENT_ID: XB-OWNER-DOC-IMPLEMENTATION-01
DEPENDENCIES: XB-OWNER-DOC-IMPLEMENTATION-01
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — Der vorbereitete Patch wurde vom Owner unverändert angewendet und die Sitzung neu gestartet; die wirksamen Allows sind in `XB-OWNER-DOC-IMPLEMENTATION-02` verifiziert. Die historischen Review-Ergebnisse dieses Pakets bleiben unverändert.

`documented` — Abschlussstatus: Die zulässige Ledger-Arbeit und der minimal vorbereitete Owner-Patch sind reviewer-geprüft (`PASS`). Die Rechte selbst sind nicht angewendet und daher nicht `verified`; die Dokumentationsdateien bleiben unverändert. Der Owner wendet genau den Patch bei `:2343-2359` an, lädt die Sitzung neu und bestätigt die Negativprüfungen bei `:2361`; erst danach dürfen die freigegebenen Dokumentationsänderungen umgesetzt werden. Keine Git-Publikation, kein Build und kein CTest in diesem Paket.

## `XB-OWNER-DOC-IMPLEMENTATION-02` — Rechteprüfung und Umsetzung

`observed` — Wirksame Rechteprüfung nach dem Owner-Neustart: `.opencode/agents/coordinator.md:44-46` erlaubt `edit` für `docs/ARCHITECTURE.md`, `:47-49` für `ai/README.md`. Beide exakten Allows stehen nach dem bisherigen Allow-Block `:8-43` und vor den Denys `:50-61`; danach folgt keine weitere `edit`-Regel (`:62+` betrifft `subagent`, `question`, `shell`, `webfetch`, `websearch`). Nach `AGENT_WORKFLOW.md:94-96` werden globale Regeln zuerst geladen und die letzte passende Regel gewinnt; die globale `ask`-Regel `opencode.jsonc:119` für `docs/ARCHITECTURE.md` wird damit durch den Agenten-Allow überschrieben. Der Wirksamkeitsnachweis stützt sich nicht nur auf die Regelstellung: alle vier freigegebenen Edits wurden nach dem Neustart tatsächlich ausgeführt und waren erfolgreich.

`observed` — Negativprüfungen, konfigurationsbasiert; bewusst **keine** Schreibversuche auf gesperrte Pfade: `coordinator.md:5-7` (`edit "*" = deny`) bleibt erhalten; für andere `docs/**`-Pfade existiert nur die bestehende Allowlist `:8-43`, kein `docs/**`-Wildcard; für andere `ai/**`-Pfade existiert kein Allow, sondern ausschließlich der exakte Eintrag `ai/README.md`. `developer.md:20-22,35-37` Denys für `docs/ARCHITECTURE.md` und `ai/**` bleiben unverändert; `architect.md:5-7`, `reviewer.md:5-7`, `tester.md:5-7`, `researcher.md:5-7` bleiben deny-all mit ihren engen Eigen-Allows. `opencode.jsonc:115-121` und die harten Publikations-`policies` `:131-150` sind unverändert; `coordinator.md:92-121` erlaubt weiterhin nur die gelisteten Lese-Git-Kommandos; `:80-91` enthält weiterhin die Shell-Deny-Regeln. Es wurden keine neuen Rechte erteilt.

`observed` — Umgesetzte Änderungen:
- `docs/ARCHITECTURE.md:168-171`: öffentliche Schnittstelle mit `execute_instruction(...)`, `step_x64_instruction(...)` und `step_x64_at_rip(...)` dokumentiert; die 15 Bytes als lokales Lese-Limit, keine RIP-relative Adressierung, kein Run-Loop.
- `docs/ARCHITECTURE.md:234`: Host-Seam als Implementierungsstand beschrieben; „treated as immutable by this seam"; keine echte OS-/API-Erkennung implementiert; `BackendSelector` bleibt Designkonzept; Auswahl über `select_execution_plan(...)`; keine JIT-Behauptung, das bestehende „Kein JIT" bei `:182` blieb unverändert.
- `ai/README.md:3,13,28`: exakt die drei freigegebenen Wortlaute übernommen; alle anderen Zeilen unverändert.

`observed` — Verifikation: Read-back aller fünf Zielstellen; `git diff --check; "EXIT=$LASTEXITCODE"` → nur die bekannten CRLF-Warnungen für `CMakePresets.json` und `src/host/execution_policy.cpp`, `EXIT=0`. Kein Build und kein CTest (reine Dokumentation, keine technische Abhängigkeit). Keine Git-Aktion außer der Leseprüfung; kein Stage, Commit, Push, Merge, Rebase, Reset, Branch-Wechsel, Tag, PR oder Publication.

`documented` — Die zuvor offenen Blocker `OWNER-DOC-EDIT-RIGHTS` und `XB-PERM-REV-01` sind damit gelöst; historische Review-Ergebnisse früherer Pakete wurden nicht verändert. Der E1-Kanon `:636-645` und die TD.6-Historie `:1895-1902` bleiben konsistent; die dort als „noch nicht eingearbeitet" geführten Präzisierungen sind jetzt tatsächlich umgesetzt.

```yaml
PACKAGE_ID: XB-OWNER-DOC-IMPLEMENTATION-02
PARENT_ID: XB-OWNER-DOC-IMPLEMENTATION-01
DEPENDENCIES: XB-OWNER-PERMISSION-FIX-01
STATUS: in_progress
IMPLEMENTATION_STATE: self_checked
REVIEW_STATUS: review_pending
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — Reviewer `ses_df066ad52ffeXzANmbEyXVS5m1`: `NEEDS_CHANGE`; `ACTIONABLE 1` (`DOCREV-02-01`, LOW: `coordinator.md:80-91` wurde als Bereich der Lese-Git-Allows zitiert, enthält aber nur Shell-Denys). `OWNER_DECISION 0`, `NON_BLOCKING 0`, `UNKNOWN 0`. Klassifikation: **Case B**, kein Finding downgraded. Alle übrigen sechs Prüfpunkte des Reviews waren inhaltlich `PASS`.

```text
PACKAGE_ID:        XB-OWNER-DOC-IMPLEMENTATION-02-CORR-01
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Falsche Zeilenangabe der Lese-Git-Allows im Ledger korrigieren.
FILES:             docs/AGENT_TASKS.md (sole writer: coordinator; explizit erlaubt durch coordinator.md:8-10)
ALLOWED:           Korrektur des Zitats bei :2409 und Aufnahme des Korrekturprotokolls.
FORBIDDEN:         Änderung von .opencode/**, opencode.jsonc, docs/ARCHITECTURE.md, ai/README.md, Produktcode, Tests, CMake, ADRs; Rechteerweiterung; Git-Publikation.
ACCEPTANCE:        Zitat zeigt :92-121 als Lese-Git-Allows und :80-91 als Shell-Denys; git diff --check ohne Fehlerzeilen; EIN erneuter Reviewer-Check.
TESTS:             Kein Build/CTest für reine Ledger-Dokumentation; git diff --check.
REVIEW:            reviewer, read-only, EIN Re-Check nach stabilem Writer-Report.
WORKTREE:          none
OWNER_APPROVALS:   keine für diese Ledger-Korrektur.
```

`observed` — Writer `coordinator` ersetzte das Zitat durch `coordinator.md:92-121` für die Lese-Git-Allows und wies `:80-91` ausdrücklich als Shell-Deny-Bereich aus. Keine Datei außer `docs/AGENT_TASKS.md` wurde verändert; die umgesetzten Dokumentationspatche blieben unberührt. Verifikation dieser Korrekturwelle: `git diff --check; "EXIT=$LASTEXITCODE"` → nur die bekannten CRLF-Warnungen für `CMakePresets.json` und `src/host/execution_policy.cpp`, `EXIT=0`.

`observed` — Re-Check derselben Reviewer-Session: **PASS** (**Case A**). `DOCREV-02-01` ist gegen sein Erfolgskriterium geschlossen; `ACTIONABLE 0`, `OWNER_DECISION 0`, `UNKNOWN 0`. Der `NON_BLOCKING`-Hinweis zur fehlenden Diff-Check-Zeile wurde mit exaktem Befehl und `EXIT=0` ergänzt. Kein drittes Review.

```yaml
PACKAGE_ID: XB-OWNER-DOC-IMPLEMENTATION-02
PARENT_ID: XB-OWNER-DOC-IMPLEMENTATION-01
DEPENDENCIES: XB-OWNER-PERMISSION-FIX-01
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`documented` — Abschluss der Change-Aufarbeitung: Rechte wirksam verifiziert, beide freigegebenen Dokumentationsänderungen vollständig umgesetzt, Ledger konsistent, abschließendes Review `PASS` mit `ACTIONABLE 0`. Keine Produktcode-, Header-, Test-, CMake-, ADR- oder Konfigurationsänderung durch Agenten in diesem Paket; keine Git-Publikation, kein Build und kein CTest. Alle Änderungen bleiben lokal, uncommitted und unpubliziert.

## `XB-CHANGE-COMPLETION-AND-INTEGRATION-01` — Konsolidierte Change-Aufarbeitung und Integrationsübergabe

```text
PACKAGE_ID:        XB-CHANGE-COMPLETION-AND-INTEGRATION-01
PACKAGE_TYPE:      BUGFIX (Dokumentations-/Statusangleichung; keine Produktänderung)
PRODUCT_BOUNDARY:  development-only
GOAL:              Offenen Change-Bestand einmalig inventorieren, veraltete Statusaussagen angleichen und die ausschließlich owner-kontrollierte Publikation vorbereiten.
FILES:             docs/AGENT_TASKS.md, docs/ROADMAP.md (sole writer: coordinator; coordinator.md:9-16)
ALLOWED:           Statusangleichung veralteter Gegenwartsaussagen; Konsolidierungs-, review- und Übergabeprotokoll.
FORBIDDEN:         Produktcode/Header/Tests/CMake/Presets/ADR/Konfiguration; Rechteerweiterung; Stage/Commit/Push/Merge/PR/Tag/Release/Branchwechsel/Historie; erneutes Aufrollen abgeschlossener Pakete; Umformulierung historischer Snapshots.
ACCEPTANCE:        Inventory vollständig; Kategorien A-E je Change; veraltete Gegenwartsaussagen korrigiert; ein unabhängiges Review; Git-Integration als Owner-Schritt vorbereitet, nicht ausgeführt.
TESTS:             Kein Build/CTest (reine Doku-Statusangleichung, keine technische Abhängigkeit); git diff --check; EIN umfassendes Review.
REVIEW:            reviewer, read-only, ein Durchgang; Case-B-Korrektur mit gezieltem Re-Check nur bei echtem ACTIONABLE.
WORKTREE:          none
OWNER_APPROVALS:   F1/E2/E3, SETUP-004/006/007, XB-0082/0084-Gates, alle Git-/GitHub-Publikationsaktionen.
```

`observed` — Ausgangslage (einmalig gelesen, nicht erneut untersucht): Branch `docs/opencode-publication-group-2`, HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`, Working Tree 31 Einträge (23 modified, 8 untracked), Index leer. Alle technischen Pakete (`XB-X64-STEP-01`, `XB-X64-RIP-STEP-01` inkl. Korrektur, `XB-HOST-CAP-01`, `XB-0080-A/B`, `XB-0081`, `XB-0083`, `XB-X64-CORPUS-01`, `XB-CMAKE-VERIFY-RESUME`, `XB-OWNER-DOC-IMPLEMENTATION-01/02` und die jeweiligen Korrekturen) sind lokal `pass` mit Reviewer-`PASS`; sie werden nicht erneut geprüft.

`observed` — Konsolidiertes Inventur und Kategorisierung (A notwendig / B entwicklungsfördernd / C optional / D zurückstellen / E Owner-Entscheidung):

| Change | Kategorie | Implementierungslage | Agentenrest | Owner-Entscheidung |
|---|---|---|---|---|
| E2 — `AGENTS.md` Versionierung | E | pass (lokal), Publication offen | keine | dauerhafte Versionierung/Publikation |
| E3 — `CMakePresets.json` + Backup | E | VS2026-Änderung pass, Treatment offen | keine | lokal vs. normalisiert vs. publiziert |
| F1 — Publikationsaudit/Zählung | E | Audit pass (25/31 Einträge, historische Varianten) | keine | Auswahl/Gruppierung/Zählung, PR #1 |
| SETUP-004 — Session-Transfer | E | needs_owner, kein Agententool | keine | Owner führt export/import manuell aus |
| SETUP-006 — Worktree-Rest | E | blocked, kein Agententool | keine | Bereinigung/weiterer Öffnungsversuch |
| SETUP-007 — Spezialisten/Rechte | E | blocked, sechs Rollen installiert | keine | Installation + persistierte Rechte |
| XB-0082 (Scope/Taxonomie/Tool) | E | evaluation-only umgesetzt | keine | Scope, Tool/CI/Abhängigkeit |
| XB-0084-Gates (A3-A5, B2, F3-F6) | E | Bewertung/Planung pass | keine | ADR, Lizenz, Performance, Xbox-360, FINAL_CLEANUP |
| GTA-VI-Implementierung | E | nicht freigegeben | keine | eigenes Freigabepaket |
| PR #1 / aktueller PR-Status | E/unknown | historisch offen, nicht prüfbar | keine | weiterverwenden oder neuen PR anlegen |
| Veraltete Gegenwartsstatus (E1/ARCH) | B | **hier umgesetzt** | keine | keine (Publikation bleibt F1) |

`documented` — Keine weiteren agentenausführbaren Changes gefunden. Die `needs_owner`-YAML-Blöcke `:1041-1052`, `:1215-1226`, `:1348-1359` und `:1768-1779` sind Owner-Entscheidungspakete bzw. historische Snapshots, keine offenen Implementierungen. Der doppelte `PACKAGE_ID`-Block `XB-OWNER-DOC-IMPLEMENTATION-02` (Zwischenstand `:2420-2431`, final `pass` `:2454-2465`) bleibt als historischer Verlauf erhalten; der finale Block ist maßgeblich (`NON_BLOCKING`, kein erneutes Review). Der E1-Kanon `:636-645` und die TD.6-Historie `:1895-1902` bleiben unverändert konsistent.

`observed` — Phase B/C: Es wurde genau ein agentenausführbares Paket gebildet und umgesetzt — die Statusangleichung veralteter Gegenwartsaussagen. Geändert: `:1038` (`ai/README.md`-Patch jetzt `Applied and verified`), `:2118` (`ai/README.md` `pass`), `:2121` (`docs/ARCHITECTURE.md` `pass`), `docs/ROADMAP.md:39` (`ai/README.md` lokal geändert und geprüft; Publikation owner-only). Historische Snapshots (`:1574`, `:1895`, `:2233`, `:2384`, `:2403` u. a.) wurden nicht umgeschrieben. Keine Produkt-, Header-, Test-, CMake-, Preset-, ADR-, Konfigurations- oder Rechteänderung.

`documented` — Phase D: Kein Build und kein CTest in diesem Paket (reine Statusangleichung ohne technische Abhängigkeit). Die letzten echten Verifikationsergebnisse bleiben `documented` und werden nicht wiederholt: Configure `--fresh --preset windows-debug` `EXIT=0`, Build `--build --preset windows-debug --config Debug` `EXIT=0`, `ctest --test-dir build/windows-debug -C Debug --output-on-failure` `1/1 Passed`, `100% tests passed`, `EXIT=0` (`XB-CMAKE-VERIFY-RESUME`, `XB-CHANGE-COMPLETION`); `git diff --check` `EXIT=0` mit den zwei bekannten CRLF-Warnungen.

`observed` — Reviewer `ses_df057b6f5ffeuAFnBrGNg741lG`: `NEEDS_CHANGE`; `ACTIONABLE 1` (`INTEG-REV-01`, LOW: `docs/ROADMAP.md:45` enthielt im als *current* gekennzeichneten Abschnitt noch „`ai/README.md` remains untouched pending a separate owner release", während `:39` und die Datei selbst den angewendeten Zustand zeigen). `OWNER_DECISION 0`, `NON_BLOCKING 0`, `UNKNOWN 0`. Klassifikation: **Case B**, kein Finding downgraded. Alle übrigen sechs Prüfpunkte waren inhaltlich `PASS`.

```text
PACKAGE_ID:        XB-CHANGE-COMPLETION-AND-INTEGRATION-01-CORR-01
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Veraltete Gegenwartsaussage zu ai/README.md in ROADMAP.md:45 angleichen.
FILES:             docs/ROADMAP.md (sole writer: coordinator; coordinator.md:14-16)
ALLOWED:           Ersetzung der veralteten Aussage in :45; Korrekturprotokoll in docs/AGENT_TASKS.md.
FORBIDDEN:         Produktcode/Tests/CMake/Presets/ADR/Konfiguration/Rechte; Git-Publikation; Umformulierung historischer Snapshots.
ACCEPTANCE:        Keine present-tense Aussage in ROADMAP.md beschreibt ai/README.md als unangewendet; :39 und :45 konsistent; git diff --check EXIT=0; EIN gezielter Re-Check.
TESTS:             Kein Build/CTest (reine Doku-Korrektur); git diff --check.
REVIEW:            reviewer, read-only, EIN Re-Check nach stabilem Writer-Report.
WORKTREE:          none
OWNER_APPROVALS:   keine für diese Ledger-/Roadmap-Korrektur.
```

`observed` — Writer `coordinator` ersetzte in `docs/ROADMAP.md:45` „`ai/README.md` remains untouched pending a separate owner release" durch „`ai/README.md` was locally applied and reviewer-PASSed under `XB-OWNER-DOC-IMPLEMENTATION-02`; publication remains an owner decision". Keine Datei außer `docs/ROADMAP.md` (plus Korrekturprotokoll in `docs/AGENT_TASKS.md`) verändert; die Konsolidierungsinventur blieb unberührt. Verifikation: `git diff --check; "EXIT=$LASTEXITCODE"` → nur die bekannten CRLF-Warnungen, `EXIT=0`.

`documented` — Phase E (nicht ausgeführt): Alle Git-/GitHub-Publikationsaktionen sind hart gesperrt (`opencode.jsonc:131-154`: commit/push/merge/rebase/reset/clean/checkout/switch/restore/stash/revert/cherry-pick/pull/tag/worktree/config-gh/gh = `deny`) und owner-only (`AGENTS.md` §8, `docs/GITHUB_AGENT_WORKFLOW.md:9,17,49`). Kein Agent führt Stage, Commit, Push, PR, Merge, Tag oder Release aus. Vorbereitete Owner-Schritte: (1) Review-Set paketbezogen prüfen, (2) gezielt stagen — kein `git add .`, (3) Commit mit paketbezogener Nachricht, (4) Branch `docs/opencode-publication-group-2` pushen, (5) vorhandenen PR weiterverwenden oder neuen PR nach etabliertem Workflow anlegen (PR-Status `unknown`, da Agents keine PR-Prüfung ausführen), (6) nach erfolgreichem Review in den Zielbranch mergen und Merge-Status verifizieren. Kein Agent behauptet, dass einer dieser Schritte bereits stattgefunden hat.

`observed` — Re-Check derselben Reviewer-Session: **PASS** (**Case A**). `INTEG-REV-01` ist gegen sein Erfolgskriterium geschlossen; `ACTIONABLE 0`, `OWNER_DECISION 0`, `NON_BLOCKING 0`, `UNKNOWN 0`. Kein drittes Review.

```yaml
PACKAGE_ID: XB-CHANGE-COMPLETION-AND-INTEGRATION-01
PARENT_ID: XB-CHANGE-COMPLETION
DEPENDENCIES: XB-OWNER-DOC-IMPLEMENTATION-02, XB-CMAKE-VERIFY-RESUME
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`documented` — Abschluss: Einmalige Inventur abgeschlossen, der einzige agentenausführbare Rest (veraltete Gegenwartsstatus zu E1/ARCH) umgesetzt, ein umfassendes Review mit einer gebündelten Korrektur und gezieltem Re-Check (`PASS`, `ACTIONABLE 0`). Alle sechs genannten offenen Punkte bleiben Owner-Entscheidungen; alle Git-/GitHub-Publikationsaktionen sind owner-only und nicht ausgeführt. Kein Produktcode-, Header-, Test-, CMake-, Preset-, ADR-, Konfigurations- oder Rechteänderung; kein Build und kein CTest in diesem Paket. Alle Änderungen bleiben lokal, uncommitted und unpubliziert. Paket 2 des regulären Entwicklungsplans ist agentenseitig startbereit; die Publikation erfordert eine Owner-Entscheidung.

## XB-AUTONOMY-ACTIVATION-03 — ledger publication-policy alignment

```text
PACKAGE_ID:        XB-AUTONOMY-ACTIVATION-03
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Remove the residual contradiction between the ledger's present-tense "publication is owner-only / no agent action" cells and the current coordinator publication authority established by XB-AUTONOMY-ACTIVATION-02.
FILES:             docs/AGENT_TASKS.md (sole writer: coordinator)
ALLOWED:           Append one authoritative current-policy clarification; mark three historical inventory/owner-decision cells as historical; record envelope, evidence, reviewer verdict and Git results in the same file.
FORBIDDEN:         Product code, tests, CMake/presets, ADRs, LICENSE, agent files, opencode.jsonc, permissions, other documentation files, history rewriting, destructive Git commands, new publication actions beyond the already-pushed activation-02 commits, build/CTest/function tests.
ACCEPTANCE:        The three live-looking cells in the open-package inventory and the consolidated owner-decision list are explicitly labeled historical; one authoritative current-policy section with a global supersession statement is appended; the policy cites AGENTS.md §8, docs/AGENT_WORKFLOW.md and docs/AGENT_AUTONOMY.md A4/A5; independent reviewer PASS; git diff --check EXIT=0.
TESTS:             Read-back of the three edited cells and the appended section; literal grep for live owner-only publication phrasing; git diff --check. No build/CTest for pure documentation.
REVIEW:            Architect skipped: single development-only documentation file with no interface, module boundary, dependency or ADR change. Independent reviewer mandatory; bundled correction rounds limited to two.
WORKTREE:          Main checkout D:\XBridge 1.0\XBridge-Series-Autonomy on branch main.
OWNER_APPROVALS:   No new owner decision: factual documentation alignment inside the coordinator's confirmed write right; policy and authority are unchanged.
```

`observed` — effective write right verified before assignment: `.opencode/agents/coordinator.md:5-7` denies `edit "*"` but `:8-10` grants `edit` on `docs/AGENT_TASKS.md`; under the documented precedence (global rules first, later matching agent rule wins) the exact allow governs this path. No global `edit` rule in `opencode.jsonc:126-139` covers `docs/AGENT_TASKS.md`, and no hard policy at `:142-172` denies it. Sole writer: coordinator. Reviewer is read-only.

`observed` — baseline in the main checkout before any edit: `git branch --show-current` → `main`; `git rev-parse HEAD` → `8c863ec86ed587d70afea59c36998f3696eef4f4`; `git status --short --branch` → `## main...origin/main` (clean, in sync); `git status --porcelain=v1 --untracked-files=all` → no output.

`documented` — authoritative current publication policy: routine staging, commit, push, merge and PR of completed, independently reviewed project changes are performed by the coordinator after the required checks and review (`AGENTS.md` §8; `docs/AGENT_WORKFLOW.md` "Standard flow" item 9 and "Permission model"; `docs/AGENT_AUTONOMY.md` A4/A5 grades). Within publication routing only, tag, release, strategic branch decisions and history rewriting remain owner decisions. PR #1 handling remains an owner decision because the GitHub CLI is unavailable and PR state is `unknown`. This statement does not change any non-publication owner gate: `SETUP-004` session export/import, `SETUP-006` worktree cleanup, `SETUP-007` specialist installation, and all ADR, license, public-interface, destructive-operation and product-release decisions remain current owner decisions (`docs/AGENT_WORKFLOW.md:190,202,292`; `docs/AGENT_AUTONOMY.md:39,47`; `AGENTS.md` §3, §7). This is a policy statement only: it does not publish any additional package.

`documented` — publication-scoped supersession: all other **publication-related** `owner-only` / `publication remains owner-only` statements in this ledger are historical per-package records from packages completed before this alignment. They are preserved verbatim and are superseded for current-state interpretation of routine publication by the policy above; they are not rewritten as new facts. The only cells annotated in this package are the `ai/README.md` patch row and the `Publication of all local pass work` row in the open-package inventory (`:1198-1199`) and the `OWNER-PUBLICATION` bullet (`:1215`). Non-publication owner gates listed above are explicitly excluded from this supersession and remain current.

`observed` — executed publication evidence for the preceding package: commit `71faef915996ed4e459cf318111a80daa4719013` (`13 files changed, 229 insertions(+), 64 deletions(-)`) and bookkeeping commit `8c863ec86ed587d70afea59c36998f3696eef4f4`; `git push` returned `To https://github.com/comedius/XBridge-Series.git` / `8339582..71faef9  main -> main` and `71faef9..8c863ec  main -> main`, both `EXIT=0`. `gh auth status` returned `CommandNotFoundException`, `EXIT=1`; remote write authentication is `observed` via Git credentials, PR state remains `unknown`.

`documented` — deliberate omissions: no product code, tests, CMake, preset, ADR, license, agent or permission change; no new build, CTest or product function test; no additional commit, push, PR or merge in this package; historical per-package records were labeled rather than rewritten.

```yaml
PACKAGE_ID: XB-AUTONOMY-ACTIVATION-03
PARENT_ID: XB-AUTONOMY-ACTIVATION-02
DEPENDENCIES: XB-AUTONOMY-ACTIVATION-02
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: published
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

### XB-AUTONOMY-ACTIVATION-03-C1 — bundled review correction (round 1 of 2)

```text
PACKAGE_ID:        XB-AUTONOMY-ACTIVATION-03-C1
PACKAGE_TYPE:      BUGFIX
PRODUCT_BOUNDARY:  development-only
GOAL:              Close both ACTIONABLE reviewer findings by scoping the policy and supersession statements to publication and by recording the post-edit Git verification in the ledger.
FILES:             docs/AGENT_TASKS.md (sole writer: coordinator)
ALLOWED:           Narrow the policy/supersession wording with an explicit non-publication carve-out; name the three annotated cells; append exact post-edit verification commands and results; ledger record.
FORBIDDEN:         Product code, tests, CMake/presets, ADRs, LICENSE, agent files, opencode.jsonc, permissions, other documentation files, history rewriting, destructive Git commands, severity downgrade, build/CTest/function tests, new publication action.
ACCEPTANCE:        Read-back shows the publication qualifier, the non-publication carve-out, the three named annotated cells and the exact post-edit diff-check/diff-stat/status results; independent reviewer re-check PASS; git diff --check EXIT=0.
TESTS:             Read-back and git diff --check. No build/CTest for pure documentation.
REVIEW:            Mandatory independent re-check; round 1 of 2.
WORKTREE:          Main checkout D:\XBridge 1.0\XBridge-Series-Autonomy on branch main.
OWNER_APPROVALS:   Covered by the XB-AUTONOMY-ACTIVATION-03 envelope; no new owner decision.
```

`documented` — finding classification from reviewer `ses_debe49b3cffeoUlxKbSev18hbO`, verdict `NEEDS_CHANGE`: `ARC-03-01` (MEDIUM, `ACTIONABLE`) and `ARC-03-02` (MEDIUM, `ACTIONABLE`) are corrected in this single bundle; `ARC-03-03` (`UNKNOWN`, reviewer tooling limitation) needs no writer correction and remains recorded as a limitation. `OWNER_DECISION` 0, `NON_BLOCKING` 0. No severity was downgraded.

`documented` — root causes: (A) a publication-scoped A5 authority change was generalized into an all-owner-decision supersession; corrected by narrowing the policy and supersession paragraphs to publication and adding an explicit carve-out for `SETUP-004`/`006`/`007` and ADR/license/public-interface/destructive/product-release gates; (B) the package recorded baseline and previous-package Git facts but omitted its own post-edit verification; corrected by appending the exact commands and results below.

`observed` — post-edit verification reported by the coordinator and not independently re-executed by the reviewer. Initial writer state (three cells plus appended section): `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0`; `git diff HEAD --no-ext-diff --stat` → `docs/AGENT_TASKS.md | 48 +++++++++++++++++++++++++++++++++++++++++++++---`, `1 file changed, 45 insertions(+), 3 deletions(-)`; `git status --porcelain=v1 --untracked-files=all` → ` M docs/AGENT_TASKS.md` only; `git branch --show-current` → `main`; `git rev-parse HEAD` → `8c863ec86ed587d70afea59c36998f3696eef4f4`. The two C1 wording replacements were made in place and temporarily produced the same `45 insertions(+), 3 deletions(-)`; appending this C1 record increased the final state to `git diff HEAD --no-ext-diff --stat` → `docs/AGENT_TASKS.md | 71 ++++++++++++++++++++++++++++++++++++++++++++++++++---`, `1 file changed, 68 insertions(+), 3 deletions(-)`. Final re-verification: `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0`; `git diff HEAD --no-ext-diff --name-only` → `docs/AGENT_TASKS.md`; `git status --porcelain=v1 --untracked-files=all` → ` M docs/AGENT_TASKS.md` only. No build, CTest, product function test or additional publication action was performed.

`observed` — mandatory independent re-check (`ses_debe49b3cffeoUlxKbSev18hbO`): **PASS**, Case A. `ARC-03-01` (MEDIUM, `ACTIONABLE`) and `ARC-03-02` (MEDIUM, `ACTIONABLE`) are closed against their success criteria; `ACTIONABLE` 0, `OWNER_DECISION` 0, `NON_BLOCKING` 0, `UNKNOWN` 1 (`ARC-03-03`: reviewer has no shell and `.git/**` reads are hard-denied, so Git facts remain writer-reported, not independently re-executed). No new finding, no over-correction and no severity downgrade. One bundled correction round used; the reviewer explicitly states that no further review is needed.

`documented` — completion: package status `pass` in the parent YAML block above; the current-policy alignment and the three historical cell labels are accepted. Deliberately unchanged: all other historical publication rows, non-publication owner gates, product code, tests, CMake/presets, ADRs, LICENSE, agent files, `opencode.jsonc`, permissions, and all prior review verdicts. No build, CTest or product function test was run. Commit and push are the remaining authorized publication step, recorded in a separate bookkeeping update after execution.

```yaml
PACKAGE_ID: XB-AUTONOMY-ACTIVATION-03-C1
PARENT_ID: XB-AUTONOMY-ACTIVATION-03
DEPENDENCIES: XB-AUTONOMY-ACTIVATION-03
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: published
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — publication bookkeeping added after the reviewer `PASS`: `git commit -m "docs(agents): align ledger publication policy with coordinator authority"` → `[main d385701] docs(agents): align ledger publication policy with coordinator authority`, `1 file changed, 85 insertions(+), 3 deletions(-)`; `git push` → `To https://github.com/comedius/XBridge-Series.git` / `8c863ec..d385701  main -> main`. Full hash `d385701642f5d756c4f6e0262893ca605009cc52` (`git rev-parse HEAD`); `PUBLICATION_STATUS: published` is set only after this confirmed push. No merge, tag, PR, history operation, build or CTest was performed.

## XB-AUTONOMY-GH-ENABLE-01 — GitHub CLI inspection enablement

```text
PACKAGE_ID:        XB-AUTONOMY-GH-ENABLE-01
PACKAGE_TYPE:      FEATURE
PRODUCT_BOUNDARY:  development-only
GOAL:              Enable coordinator-side GitHub CLI inspection of PR #1 after the owner selected "Enable gh inspection for me", by installing gh and adding the minimum scoped shell rights.
FILES:             .opencode/agents/coordinator.md, opencode.jsonc, docs/AGENT_TASKS.md (all coordinator-serialized; coordinator sole writer)
ALLOWED:           Add narrow exact shell allows for the installed gh absolute path; record owner decisions, exact commands, actual results and the pending authentication gate in this ledger.
FORBIDDEN:         Product code, tests, CMake/presets, ADRs, LICENSE, hard-policy or global deny changes, blanket wildcards, secret/credential extraction or storage, PR mutation, merge, tag, history rewrite, build/CTest/function tests.
ACCEPTANCE:        gh.exe installed and executable via absolute path; net-new rights limited to three gh inspection allows; discovery/install allows removed after use; owner authentication recorded as pending; independent reviewer PASS; git diff --check EXIT=0.
TESTS:             Exact command read-back (where.exe gh, winget --version, winget install, Get-Item, gh auth status, rejected --git-credential attempt); git diff --check. No build/CTest for configuration-only work.
REVIEW:            Architect skipped: permission/tooling configuration with no product interface, module boundary, dependency or ADR change; coordinator is authorized for these config paths. Independent reviewer mandatory.
WORKTREE:          Main checkout D:\XBridge 1.0\XBridge-Series-Autonomy on branch main.
OWNER_APPROVALS:   Owner selected "Enable gh inspection for me" and then "You run gh auth login (Recommended)"; no broader authorization was inferred.
```

`observed` — baseline before any edit: `git rev-parse HEAD` → `f753f541dbcc5ff946ce206d6780156a254ecb2c`; `git branch --show-current` → `main`; `git status --porcelain=v1 --untracked-files=all` → no output. The name-based allows `gh auth status`, `gh pr *`, `gh repo view *` were already present in `opencode.jsonc:105-107` and `.opencode/agents/coordinator.md:137-145` and were effective (they executed rather than being denied); the only blocker was the missing binary.

`observed` — install sequence with exact commands and results: `gh auth status` → `Die Benennung "gh" wurde nicht als Name eines Cmdlet ... erkannt`, `CommandNotFoundException`, `EXIT=1`. `where.exe gh` → `INFORMATION: Es konnten keine Dateien mit dem angegebenen Muster gefunden werden`, `WHERE_EXIT=1`. `winget --version` → `v1.29.380`, `WINGET_EXIT=0`. `winget install --id GitHub.cli -e --accept-source-agreements --accept-package-agreements` → `Found GitHub CLI [GitHub.cli] Version 2.105.0`, `Successfully verified installer hash`, `Successfully installed`. `Get-Item 'C:\Program Files\GitHub CLI\gh.exe'` → file present (`Length 41905976`, `LastWriteTime 17.11.2026 23:55`); `Get-Item 'C:\Users\Dennis\AppData\Local\Programs\GitHub CLI\gh.exe'` → `ItemNotFoundException`, `EXIT=1`.

`observed` — the current OpenCode shell process inherited a pre-install `PATH`, so name-based `gh` remained unresolved after installation. Three absolute-path allows were added to both configuration files: `& 'C:\Program Files\GitHub CLI\gh.exe' auth status`, `& 'C:\Program Files\GitHub CLI\gh.exe' pr *`, `& 'C:\Program Files\GitHub CLI\gh.exe' repo view *`. These are effective this session: `& 'C:\Program Files\GitHub CLI\gh.exe' auth status` executed and returned `You are not logged into any GitHub hosts`, `EXIT=1` (a runtime result, not a permission denial).

`observed` — least-privilege cleanup: the temporary discovery/install allows (`where.exe gh`, `winget --version`, exact `winget install ...`) and a failed `auth login --git-credential` allow were removed after use. `--git-credential` returned `unknown flag` with the v2.105.0 usage text, `EXIT=1`; no credential was extracted, read or stored at any point. Net-new rights are exactly the three absolute-path gh inspection allows in each file.

`documented` — pending owner action: authentication must be performed by the owner via `gh auth login` in their own terminal (browser/device flow) or by a token they set themselves; the coordinator neither handles nor stores credentials. Until authentication succeeds, PR #1 state remains `unknown`; no PR inspection, mutation, merge or close has been performed. This package enables inspection only; it does not decide PR #1 disposition.

`documented` — deliberate omissions: no product code, tests, CMake/presets, ADR, LICENSE, hard-policy or global deny change; no blanket wildcard; no secret handling; no build, CTest or product function test; no PR action, merge, tag or history operation.

`observed` — self-check taken before this ledger record was appended: `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0`; `git diff HEAD --no-ext-diff --stat` → `.opencode/agents/coordinator.md | 9 +++++++++`, `opencode.jsonc | 3 +++`, `2 files changed, 12 insertions(+)`; status → exactly those two modified files. The final-state verification after this record is appended is given below.

```yaml
PACKAGE_ID: XB-AUTONOMY-GH-ENABLE-01
PARENT_ID: XB-AUTONOMY-ACTIVATION-03
DEPENDENCIES: XB-AUTONOMY-ACTIVATION-03
STATUS: in_progress
IMPLEMENTATION_STATE: self_checked
REVIEW_STATUS: review_pending
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — independent review (`ses_debd0c873ffedqwX0ErUF3I2ZH`): **PASS**, Case A. Classification: `ACTIONABLE` 0, `OWNER_DECISION` 0, `NON_BLOCKING` 2, `UNKNOWN` 1. Severity: INFO 1, LOW 2, no MEDIUM/HIGH/CRITICAL. No severity was downgraded and no prior finding was reopened. The reviewer confirmed the net-new rights are exactly the three scoped absolute-path gh allows in each config file, the temporary discovery/install/failed login allows are gone, no hard policy or global deny was weakened, the coordinator owns all three paths, the envelope is complete, YAML tokens/order are valid, pending authentication is honestly recorded, PR #1 is not claimed, and no product code, tests, CMake/presets, ADR, LICENSE, secret handling, build, CTest or PR mutation appears in the diff.

`documented` — disposition of non-blocking findings: `GH-02` (LOW, `NON_BLOCKING`) noted that the earlier self-check snapshot understated the final change set; this is resolved by the final-state verification below and by making the snapshot boundary explicit above. `GH-03` (LOW, `NON_BLOCKING`) noted that `& '…gh.exe' pr *` also matches mutating subcommands; the allow exactly mirrors the pre-existing name-based `gh pr *` (`coordinator.md:141`, `opencode.jsonc:106`) and therefore broadens nothing, and the workflow prohibition on PR mutation remains in force regardless of capability. No PR action was performed. `GH-01` (`UNKNOWN`, INFO) is the reviewer's tooling limitation: no shell and hard-denied `.git/**` reads, so all Git/install/runtime facts remain writer-reported, not independently re-executed.

`observed` — final-state verification after this ledger record: `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0`; `git diff HEAD --no-ext-diff --stat` → per-file totals `.opencode/agents/coordinator.md` 9 insertions, `docs/AGENT_TASKS.md` 65 insertions, `opencode.jsonc` 3 insertions; summary `3 files changed, 77 insertions(+)`. Figures are the final state after this ledger record; replacing this line in place keeps the line count unchanged. `git diff HEAD --no-ext-diff --name-only` → `.opencode/agents/coordinator.md`, `docs/AGENT_TASKS.md`, `opencode.jsonc`; `git status --porcelain=v1 --untracked-files=all` → exactly those three modified files. `git rev-parse HEAD` → `f753f541dbcc5ff946ce206d6780156a254ecb2c`; `git branch --show-current` → `main`. No build, CTest or product function test was run.

`documented` — completion: enablement is accepted; authentication by the owner and PR #1 disposition remain pending owner actions and are not part of this package verdict. Commit and push are the remaining authorized publication step, recorded in a separate bookkeeping update after execution.

```yaml
PACKAGE_ID: XB-AUTONOMY-GH-ENABLE-01
PARENT_ID: XB-AUTONOMY-ACTIVATION-03
DEPENDENCIES: XB-AUTONOMY-ACTIVATION-03
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: published
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — publication bookkeeping added after the reviewer `PASS`: `git commit -m "feat(agents): enable scoped GitHub CLI inspection"` → `[main b9044e1] feat(agents): enable scoped GitHub CLI inspection`, `3 files changed, 77 insertions(+)`; `git push` → `To https://github.com/comedius/XBridge-Series.git` / `f753f54..b9044e1  main -> main`. Full hash `b9044e166192036ab2894fb433d3fe49fb8b19b6` (`git rev-parse HEAD`); `PUBLICATION_STATUS: published` is set on the authoritative final YAML block only after this confirmed push. The earlier YAML block in this section is a pre-review snapshot and remains unchanged. No merge, tag, PR, history operation, build or CTest was performed.

`observed` — PR #1 state resolved after the owner completed `gh auth login`: `& 'C:\Program Files\GitHub CLI\gh.exe' auth status` → `github.com`, `✓ Logged in to github.com account comedius (keyring)`, `Active account: true`, `Git operations protocol: https`, `Token scopes: 'gist', 'read:org', 'repo', 'workflow'`. `& 'C:\Program Files\GitHub CLI\gh.exe' pr view 1 --repo comedius/XBridge-Series` → `title: Harden Windows build and test configuration`, `state: CLOSED`, `author: comedius`, `number: 1`, `url: https://github.com/comedius/XBridge-Series/pull/1`, `additions: 108`, `deletions: 11`, `auto-merge: disabled`. `& 'C:\Program Files\GitHub CLI\gh.exe' pr view 1 --repo comedius/XBridge-Series --json number,title,state,closedAt,mergedAt,mergeCommit,url,baseRefName,headRefName` → `{"baseRefName":"main","closedAt":"2026-11-25T08:03:23Z","headRefName":"feature/build-test-hardening","mergeCommit":null,"mergedAt":null,"number":1,"state":"CLOSED",...}`. `observed` — PR #1 is closed without merge (`mergedAt` and `mergeCommit` are null); the earlier `unknown` statement in the enablement envelope is superseded by this read-only observation. No PR #1 mutation, reopen, merge or close was performed; no owner decision on PR #1 remains necessary because its state is already final.

`observed` — repository-wide PR inventory via `& 'C:\Program Files\GitHub CLI\gh.exe' pr list --repo comedius/XBridge-Series --state all --limit 30`: PRs 18, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3 and 2 are `MERGED`; PR 17 (`docs(memory): define guest address-space architecture baseline`) is `DRAFT`; PR 1 is `CLOSED` without merge. No open non-draft pull request was found. This inventory is read-only evidence only; it does not authorize any PR action, and PR #17 draft status and its disposition remain an owner matter, not part of any completed package.
