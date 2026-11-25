# GitHub-/Git-Workflow für Agenten und Owner

Dieses Dokument gehört zur **Entwicklungsinfrastruktur** und ist **nicht Teil des Produktpakets** (`AGENTS.md` §10). Es beschreibt Branch-, Parallelarbeits-, Worktree-, Konflikt-, Review- und Veröffentlichungsregeln als Ergänzung zu `docs/AGENT_WORKFLOW.md`. Bei Konflikten gilt dessen Autoritätsordnung.

Stand der Regelabstimmung: 2026-11-25 (`XB-AUTONOMY-ACTIVATION-02`); ältere Snapshots bleiben als `historisch` markiert. Evidenzlabels: `documented` (Regel), `observed` (lokal geprüft), `unknown` (nicht verifiziert), `historisch` (vergangener Snapshot).

## 1. Eigentum an Git-Operationen

`documented` (`AGENTS.md` §8, `docs/AGENT_WORKFLOW.md`, `.opencode/agents/coordinator.md`) — Der **Coordinator** führt Staging, Commit, Push, Merge und Pull-Request-Aktionen für geprüfte Pakete über die konfigurierten Shell-Regeln aus; **Subagenten veröffentlichen nicht**. Harte `policies` in `opencode.jsonc` blockieren weiterhin Historie-Umschreibung, Verwerfen von Änderungen, Branch-/Worktree-Kommandos per Git-Shell, globale Git-Konfiguration und destruktive Befehle; Agent-Regeln dürfen sie nicht aufheben. Tag/Release und strategische Branch-Entscheidungen bleiben Owner-Entscheidungen.

`observed` — Erlaubte Git-Leseoperationen des Coordinator umfassen `git status --porcelain=v1 --untracked-files=all`, `git status --short --branch`, `git diff --check`, `git diff HEAD --no-ext-diff [--stat|--name-only]`, `git show --no-ext-diff`, `git rev-parse HEAD`, `git log -5 --oneline` und `git branch --show-current`. `git status --short --branch` lieferte `## main...origin/main`, `EXIT=0`; `git diff --check` lieferte `EXIT=0`. Nicht ausgeführte oder abgewiesene Kommandos bleiben `unknown`/`historisch` und werden nicht als Ergebnis ausgegeben.

## 2. Branch-Strategie

`observed` — Aktiver Branch: `main`; Ausgangs-HEAD dieses Pakets: `833958233cd3864cc04c716a4236207090e00987`. Der Branch `docs/opencode-publication-group-2` mit HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f` ist **historisch**.

`documented` — Agents arbeiten im vom Owner bereitgestellten Checkout. Git-Shell-Branchwechsel, Resets und Worktree-Kommandos bleiben harte Denys; der Coordinator darf bereits geprüfte, abgeschlossene Arbeit gemäß `AGENTS.md` §8 mergen. `historisch`: `10e5e75`, 32-Eintrags-Zählungen und der alte Publikationsgruppen-Branch sind Snapshots, keine aktuelle Aussage.

`documented`/`historisch` — Publikationsgruppe 0 war im Abschnitt `XB-0086-C2 — final status alignment and publication group 0 definition` von `docs/AGENT_TASKS.md` definiert (fünf Dokumente: `AGENT_WORKFLOW.md`, `RELEASE_CHECKLIST.md`, `AGENT_TASKS.md`, `ROADMAP.md`, `PORTABLE_SETUP.md`). Die frühere Zeilenangabe `:588` ist durch nachträgliche Ledger-Einträge ungültig geworden und wird nicht mehr verwendet. Sie ist keine aktuelle Freigabebedingung. PR #18 ist in `main` integriert; der aktuelle PR-Status ist `unknown`, weil `gh` im Agentenkontext nicht verfügbar ist.

## 3. Parallele Agentenarbeit

`documented` — Parallele Arbeit ist nur bei disjunkten Write-Sets und ohne Schnittstellenkollision erlaubt. Pro Datei existiert genau ein verantwortlicher Writer pro Paket. Shared Files (`CMakeLists.txt`, `CMakePresets.json`, `opencode.jsonc`, `.opencode/**`, `AGENTS.md`) sind coordinator-serialisiert und werden nie von zwei Agenten gleichzeitig bearbeitet.

`observed` — Installiert sind sechs Rollen; neun Spezialrollen bleiben `planned`. Ein nicht installierter Spezialist wird nicht als Writer eingesetzt und nicht als aktiv gemeldet. `test-engineer` bleibt ausdrücklich `planned`.

`documented` — Ergebnisübergaben enthalten Package-ID, exakten Writer-Satz, geänderte Dateien, Kommandos und Ergebnisse, Testlage, Auslassungen, Evidenzlabels und offene Fragen. Es gibt keine Zwischenberichte ohne neuen Sachverhalt.

## 4. Worktrees

`documented` — Worktrees werden ausschließlich über die unterstützten OpenCode-Funktionen erzeugt, geprüft und entfernt (`worktree_create`, `worktree_list`, `worktree_remove`). Git-Shell-Kommandos für Worktrees sind untersagt. Konfiguriert ist `worktree.directory = ../worktrees` (`opencode.jsonc:7-9`), relativ zum Checkout und damit laufwerksunabhängig.

`observed` — Die getestete Erzeugung meldete `Created worktree could not be opened`; ein Checkout wurde angelegt, aber nicht von `worktree_list` registriert. Ein Setup-Worktree wird niemals automatisch gemergeht oder entfernt. Die Bereinigung des zurückgebliebenen Verzeichnisses erfordert eine Owner-Entscheidung.

`documented` — Bei erneutem Auftreten des Fehlers wird dieser Teil sofort gestoppt und dokumentiert; es werden keine spekulativen Konfigurationsänderungen und keine globalen Git-Einstellungen vorgenommen. Worktrees über mehrere Laufwerke können `dubious ownership` erzeugen; `safe.directory` wird nur pro Kommando gesetzt, nie global.

## 5. Konfliktbehandlung

`documented` — Konflikte werden nicht durch blindes Übernehmen beider Versionen gelöst. Bei überschneidenden Write-Sets, widersprüchlichen Anforderungen oder unklarer Autorität stoppt der Coordinator, dokumentiert die konkrete Stelle und bittet den Owner um eine Entscheidung. Bestehende Benutzeränderungen werden nie überschrieben oder verworfen.

`documented` — Ein technischer Konflikt (z. B. Fehlschlag einer Prüfung) wird an den verantwortlichen Writer zurückgeleitet; ein Rechte- oder Freigabekonflikt geht an den Owner. Keine Rechteerweiterung zur Konfliktauflösung.

## 6. Review- und Veröffentlichungsablauf

1. `documented` — Writer beendet die Arbeit, führt Selbstprüfung aus und meldet exakte Kommandos/Ergebnisse.
2. `documented` — Architect nur bei architektonischer Relevanz; Reviewer führt **eine** gebündelte Abschlussprüfung durch.
3. `documented` — Findings klassifizieren; Case A ohne Korrektur behalten, Case B gebündelt korrigieren und gezielt nachprüfen (maximal zwei Runden).
4. `documented` — Coordinator erstellt kompakten Abschlussbericht und protokolliert Status in `docs/AGENT_TASKS.md` / `docs/ROADMAP.md`.
5. `documented` — Der Coordinator führt Staging, Commit, Push, Merge und PR nach Review aus; Subagenten führen keinen dieser Schritte aus. Tag, Release und Strategieentscheidungen bleiben beim Owner.

`observed` — Der lokale Ignore-Schutz (`/.owner-patch-backup/`, `/CMakePresets.json.backup`, `/debug-*.txt`) wurde vom Owner angewendet und im angewendeten Zustand vom Reviewer geprüft (`PASS`, `ses_df1b60181ffek5mOBp1DHBgV5M`); die zuvor unversionierten Backup-/Debug-Artefakte erscheinen nicht mehr im Git-Status. Sie werden nicht gelöscht oder verschoben. `unknown` — `.git/info/exclude` ist für Agenten nicht lesbar (harter Deny) und bleibt Owner-Territorium.

## 7. Owner-Freigaben

`documented` — Folgende Punkte benötigen ausdrückliche Owner-Entscheidung, bevor etwas passiert:

- Paketfreigabe und Envelope-Änderungen.
- Tag, Release, Strategie für Branches/Merge und jede Historie-Umschreibung. Routine-Staging/-Commit/-Push/-Merge/-PR des geprüften Pakets liegen beim Coordinator.
- Änderungen an `opencode.jsonc`, `.opencode/**`, `AGENTS.md`, `ai/**` und Rechten.
- Worktree-Bereinigung und weitere Worktree-Versuche nach Fehlern.
- Produktfreigabe, `FINAL_CLEANUP`, neue Abhängigkeiten, ADRs und Schnittstellenänderungen.
- `ai/README.md` ist Coordinator-Dokumentation und wurde in `XB-AUTONOMY-ACTIVATION-02` abgeglichen; keine Produktfreigabe.

## 8. Protokollierung

`documented` — Ledger-Einträge enthalten ID, Typ, Grenze, Status, Verantwortliche, Ergebnis/Evidence und offene Owner-Entscheidung. `documented` — Ohne protokollierte Kommandos und echte Ergebnisse darf kein Paket als `pass` geführt werden. `observed` — Arbeitsstand und HEAD werden bei jedem Paket erneut gelesen und nicht aus historischen Einträgen übernommen.

## 9. Bekannte Einschränkungen

- `unknown` — PR-Status ohne `gh`, Laufzeitverhalten der vier Code-Mode-Denys, zweiter Rechner, interaktive TUI-Dialoge.
- `documented` — Keine Subagenten-Publikation; Coordinator-Publikation nur nach Review; keine Worktree-Bereinigung ohne Owner; keine Umgehung harter Policies.
