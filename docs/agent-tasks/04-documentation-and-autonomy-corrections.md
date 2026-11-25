# Agent Task Ledger — Archive 04

> Extracted verbatim from the former `docs/AGENT_TASKS.md`, original lines 2225–2791. Historical line references in older records refer to the former monolithic file. Content is preserved; see the index at `docs/AGENT_TASKS.md`.

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

