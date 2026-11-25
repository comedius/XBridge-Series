# GitHub-/Git-Workflow für Agenten und Owner

Dieses Dokument gehört zur **Entwicklungsinfrastruktur** und ist **nicht Teil des Produktpakets** (`AGENTS.md` §10). Es beschreibt Branch-, Parallelarbeits-, Worktree-, Konflikt-, Review- und Veröffentlichungsregeln als Ergänzung zu `docs/AGENT_WORKFLOW.md`. Bei Konflikten gilt dessen Autoritätsordnung.

Stand: 2026-11-24. Freigegeben als Teil von `XB-P2`. Evidenzlabels: `documented` (Regel), `observed` (lokal geprüft), `unknown` (nicht verifiziert), `historisch` (vergangener Snapshot).

## 1. Eigentum an Git-Operationen

`documented` (`AGENTS.md` §8, `docs/AGENT_WORKFLOW.md`) — **Alle** Git-/GitHub-Publikationsaktionen führt der Projektowner aus: Staging, Commit, Push, Pull Request, Merge, Tag, Release. Agents dürfen weder Historie umschreiben noch Änderungen verwerfen, Branches wechseln oder Worktrees per Git-Shell erstellen/entfernen. Harte `policies` in `opencode.jsonc` blockieren die entsprechenden Shell-Kommandos; Agent-Regeln dürfen sie nicht aufheben.

`observed` — Erlaubte Git-Leseoperationen des Coordinator umfassen u. a. `git status --porcelain=v1 --untracked-files=all`, `git diff --check`, `git rev-parse HEAD`, `git branch --show-current`. `git status --short` und `git diff -- .gitignore opencode.jsonc` wurden in jüngsten Läufen nicht ausgeführt bzw. abgewiesen; Ergebnisse sind nicht zu erfinden.

## 2. Branch-Strategie

`observed` — Aktiver Branch: `docs/opencode-publication-group-2`; HEAD: `58aa1f60eb5e67cbf8aa884f694fce208b54819f` (Stand der XB-P2-Vorbereitung). Der Branch ist Arbeitsstand, keine Publikationsfreigabe.

`documented` — Agents arbeiten im vom Owner bereitgestellten Checkout und wechseln keinen Branch. Ein neuer Branch, ein Wechsel, ein Reset oder ein Merge benötigt eine Owner-Entscheidung. `historisch`: frühere Stände (`main`, `10e5e75`, 32 Einträge) sind Snapshots und keine Aussage über den aktuellen HEAD (`docs/PORTABLE_SETUP.md:60,109`).

`documented` — Publikationsgruppe 0 ist in `docs/AGENT_TASKS.md:588` definiert (fünf Dokumente: `AGENT_WORKFLOW.md`, `RELEASE_CHECKLIST.md`, `AGENT_TASKS.md`, `ROADMAP.md`, `PORTABLE_SETUP.md`). Die Bezeichnung `publication-group-2` ist der aktuelle Branchname; eine eigenständige Definition von Gruppe 2 liegt nicht vor und wird von Agents nicht erfunden. PR #1 war `historisch` offen; der aktuelle PR-Status ist `unknown`, da Agents keine PR-Prüfung ausführen.

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
5. `documented` — Owner entscheidet über Staging, Commit, Push, PR, Merge, Tag und Release. Agents führen keinen dieser Schritte aus.

`observed` — Der lokale Ignore-Schutz (`/.owner-patch-backup/`, `/CMakePresets.json.backup`, `/debug-*.txt`) wurde vom Owner angewendet und im angewendeten Zustand vom Reviewer geprüft (`PASS`, `ses_df1b60181ffek5mOBp1DHBgV5M`); die zuvor unversionierten Backup-/Debug-Artefakte erscheinen nicht mehr im Git-Status. Sie werden nicht gelöscht oder verschoben. `unknown` — `.git/info/exclude` ist für Agenten nicht lesbar (harter Deny) und bleibt Owner-Territorium.

## 7. Owner-Freigaben

`documented` — Folgende Punkte benötigen ausdrückliche Owner-Entscheidung, bevor etwas passiert:

- Paketfreigabe und Envelope-Änderungen.
- Branches, Merge, PR, Tag, Release, Historie, Staging und Commit.
- Änderungen an `opencode.jsonc`, `.opencode/**`, `AGENTS.md`, `ai/**` und Rechten.
- Worktree-Bereinigung und weitere Worktree-Versuche nach Fehlern.
- Produktfreigabe, `FINAL_CLEANUP`, neue Abhängigkeiten, ADRs und Schnittstellenänderungen.
- Umgang mit `ai/README.md` (weiterhin separat, nicht freigegeben).

## 8. Protokollierung

`documented` — Ledger-Einträge enthalten ID, Typ, Grenze, Status, Verantwortliche, Ergebnis/Evidence und offene Owner-Entscheidung. `documented` — Ohne protokollierte Kommandos und echte Ergebnisse darf kein Paket als `pass` geführt werden. `observed` — Arbeitsstand und HEAD werden bei jedem Paket erneut gelesen und nicht aus historischen Einträgen übernommen.

## 9. Bekannte Einschränkungen

- `unknown` — PR-Status, Laufzeitverhalten der vier Code-Mode-Denys, zweiter Rechner, interaktive TUI-Dialoge.
- `documented` — Keine Agent-Publikation; keine Worktree-Bereinigung ohne Owner; keine Umgehung harter Policies.
