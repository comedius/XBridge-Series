# OpenCode-Automatisierungsplan (aktueller Adapter)

Dieses Dokument gehört zur **Entwicklungsinfrastruktur** und ist **nicht Teil des Produktpakets** (`AGENTS.md` §10). Es beschreibt die Abbildung des plattformneutralen Agent Core (`docs/AGENT_CORE_ARCHITECTURE.md`) auf die aktuelle Plattform OpenCode. Verbindlich bleibt `docs/AGENT_WORKFLOW.md`; bei Konflikten gilt dessen Autoritätsordnung.

Stand: 2026-11-24. Freigegeben als Teil von `XB-P2`. Evidenzlabels: `documented` (Regel), `observed` (im Projekt gelesen), `unknown` (nicht verifiziert), `planned` (vorgesehen, nicht aktiv).

## 1. Rolle von OpenCode

`observed` — OpenCode ist die aktuelle Ausführungsplattform. Konfiguration: `default_agent = coordinator`, `instructions = docs/AGENT_WORKFLOW.md`, `worktree.directory = ../worktrees` (`opencode.jsonc:4-13`). Installiert und validiert sind sechs Rollen: `coordinator` (primary) sowie `architect`, `developer`, `tester`, `reviewer`, `researcher` (subagent). Neun Spezialrollen bleiben `planned`.

`documented` — OpenCode ist ein **Adapter**, nicht der Core. Regeln werden im Core beschrieben und hier auf OpenCode-Mechanismen abgebildet. Eine Plattformfunktion darf eine Core-Regel nicht aufheben; ist das nicht möglich, stoppt der Task.

`unknown` — Die lokal installierte OpenCode-Laufzeitversion wurde nicht per erlaubtem Kommando geprüft. Die Quellenbasis `2.4.1` / Commit `6f05140` ist `documented`; Version und Laufzeitverhalten sind zu unterscheiden.

## 2. Abbildung Core → OpenCode

| Core-Konzept | OpenCode-Entsprechung | Status |
|---|---|---|
| Paket/Envelope | `docs/AGENT_WORKFLOW.md`, `docs/AGENT_TASKS.md` | `documented` |
| Rolle | `.opencode/agents/*.md` mit Frontmatter | `observed` |
| Rechte | V2 `permissions` + harte `policies` in `opencode.jsonc` | `observed` |
| Writer-Satz | Agent-`edit`-Regeln + Paket-Envelope | `documented` |
| Delegation | `coordinator` → installierte Subagenten | `observed` |
| Prüfung | erlaubte Git-/CMake-/CTest-Kommandos, Reviewer | `observed` |
| Persistenz | Ledger, Roadmap, Working Tree | `observed` |
| Publikation | ausschließlich Owner | `documented` |

`observed` — Global gilt `shell "*" ask` mit expliziten Allow-Ausnahmen; `question`/`subagent`/`webfetch`/`websearch` sind global `deny`, werden aber für einzelne Rollen durch spätere Agent-Regeln aufgehoben („letzte passende Regel gewinnt"). Harte `policies` sind nicht durch Regeln aufhebbar. Shell-Pattern sind keine OS-Sandbox.

`observed` — Der Owner hat vier globale Denys angewendet: `automation_create`, `automation_delete`, `opencode_session_move`, `opencode_worktree_create` (jeweils `resource: "*"`, `effect: "deny"`). Der unabhängige Reviewer bestätigte den angewendeten Zustand mit `PASS` (`ses_df1b60181ffek5mOBp1DHBgV5M`). **Laufzeitwirkung `unknown`**; die Regeln sind eine zusätzliche Schutzschicht, keine vollständige Sperre mutierender Funktionen. Ein Tool-spezifischer Deny für `opencode.worktree_remove` existiert in 2.4.1 nicht; ein pauschales `edit:* deny` ist nicht freigegeben.

## 3. Coordinator-Orchestrierung

`documented` — Der Coordinator ist der einzige Router. Er verteilt nicht nach Rollenwunsch, sondern nach freigegebenem Envelope und wirksamen Rechten. Vor jeder Writer-Zuordnung prüft er die effektiven Rechte des konkreten Agenten für den exakten Pfad; bei Unsicherheit wird nichts freigegeben.

`documented` — Ablauf pro Paket:

1. Owner-Freigabe mit Envelope; echte Blocker vorab klären.
2. Einmalige kompakte Vorbereitung: Zustand, Abhängigkeiten, vorhandene Dokumentation, Rechte. Bekanntes wiederverwenden.
3. Vollständige Aufgaben-/Dateizuordnung; genau ein Writer pro Pfad.
4. Direkte Umsetzung im freigegebenen Writersatz; keine Zwischenberichte.
5. Selbstprüfung des Writers: Leserückprüfung, erlaubte Teilprüfungen, exakte Kommandoprotokolle.
6. Architect nur bei Architektur-, Interface-, Modul-, Abhängigkeits- oder Designfragen; bei reiner Dokumentationspflege kann er entfallen (Begründung im Bericht).
7. **Eine** gebündelte unabhängige Abschlussprüfung durch den Reviewer über den Gesamtumfang.
8. Findings klassifizieren: `ACTIONABLE`, `OWNER_DECISION`, `NON_BLOCKING`, `UNKNOWN`.
9. Case A (kein `ACTIONABLE`): Verdict behalten, kein zweites Review. Case B: ein gebündeltes Korrekturpaket pro Writer, dann gezielte Nachprüfung.
10. Kompakter Abschlussbericht mit Ergebnissen, Risiken, Owner-Entscheidungen und nächster Empfehlung.
11. Owner führt Publikation und nächste Freigabe aus.

`documented` — Maximal zwei gebündelte Korrekturrunden; danach `BLOCKED` mit Eskalation. Kein vollständiger Review-Zyklus für unveränderte, bereits bestandene Bereiche.

## 4. Agentenverteilung

| Aufgabe | Agent | Bedingung |
|---|---|---|
| Planung, Envelope, Status, Konsolidierung | `coordinator` | Envelope freigegeben |
| Architektur-/Schnittstellen-/ADR-/Risikoprüfung | `architect` | nur bei architektonischer Relevanz; read-only |
| Produktcode-Implementierung | `developer` | Pfade und Tests im Envelope namentlich zugewiesen |
| Build/CTest/Regression | `tester` | nur wenn technische Prüfung nötig; keine künstlichen Tests |
| Öffentliche Recherche | `researcher` | nur bei tatsächlichem Informationsbedarf; Web nur hier |
| Unabhängige Abschlussprüfung | `reviewer` | read-only, ein gebündelter Auftrag |

`documented` — Parallele Arbeit ist erlaubt, wenn die Write-Sets disjunkt sind und keine Schnittstellenkollision besteht. Shared Files (`CMakeLists.txt`, `CMakePresets.json`, `opencode.jsonc`, `.opencode/**`, `AGENTS.md`) sind coordinator-serialisiert. Subagenten starten keine weiteren Subagenten.

## 5. Aufgabenübergabe (Handoff-Payload)

`documented` — Jede Übergabe enthält: Package-ID, exakter Writer-Satz, geänderte Dateien, exakte Kommandos und reale Ergebnisse, Test-/Verifikationslage, bewusste Auslassungen, Evidenzlabels, offene Fragen und den letzten Zustand aus dem Zustandsmodell. Der Owner ist nicht das Transportmittel zwischen Agenten.

`observed` — Reviewer erhalten Git-Ergebnisse vom Coordinator, weil ihre Agent-Datei alle Shell-Zugriffe verweigert (`reviewer.md:14-31`). Ein Review-Auftrag nennt ausdrücklich `read`/`glob`/`grep` als einzige Werkzeuge und verlangt `PASS`/`NEEDS_CHANGE`/`BLOCKED` mit Datei:Zeile-Belegen (`AGENT_WORKFLOW.md:217`, `reviewer.md:49`).

## 6. Fehler- und Blockerrouting

| Ereignis | Route | Ergebnis |
|---|---|---|
| Schreibrecht verweigert/unklar | Coordinator → Owner | kein Write, `OWNER_DECISION`, kein Umweg |
| Branch/HEAD/Arbeitsstand abweichend | Coordinator → Owner | STOPP vor Änderung |
| Kommando fehlgeschlagen | zuständiger Writer | exakten Fehler dokumentieren, nur relevante Ursache fixen |
| Review `ACTIONABLE` | Coordinator → verantwortlicher Writer | gebündelte Korrektur, gezielte Nachprüfung |
| Review `OWNER_DECISION` | Coordinator → Owner | gebündelt, kein Agent-Eingriff |
| Review `UNKNOWN` | Coordinator → Bericht | separat listen, nicht als Patch-Fehler zählen |
| Werkzeug nicht vorhanden | Coordinator → Bericht | `NICHT VERIFIZIERT`, keine Installation |
| Worktree-Fehler | Coordinator → Owner | kein Git-Shell-Workaround, keine Bereinigung ohne Freigabe |
| Unsichere Sicherheitsaussage | Coordinator → Owner | `hypothesis`/`unknown` bleibt sichtbar |

`documented` — Ein Fehler führt nie zu einer Rechteerweiterung, einer Testabschwächung oder einem stillen Wechsel des Writers.

## 7. Effizienzregeln für die Umsetzung

- Einmal vorbereiten, nicht mehrfach inventoriesieren; bereits verifizierte Fakten tragen ihre Quelle.
- Keine separaten Zwischenreviews je Dokument/Datei; eine gebündelte Abschlussprüfung.
- Keine unnötigen Statusmeldungen; Ergebnisse werden im Handoff-Payload gebündelt.
- Parallele Leseanalysen (z. B. Architect und Researcher) nur, wenn sie keine Schreibmengen teilen.
- Kein Build/CTest für reine Dokumentationspakete; keine künstlichen Tests.

## 8. Zukünftige Automatisierung

`planned` — Weitere Adapter (Codex, Claude Code, GitHub Copilot) sind architektonisch berücksichtigbar, aber nicht installiert, nicht konfiguriert und nicht geprüft. Ein Adapterwechsel oder eine neue Automatisierungsstufe benötigt eine eigene Owner-Entscheidung, eigene Rechteprüfung und eigene Verifikation. Es gibt keine Annahme über Fähigkeiten, Modelle oder Verfügbarkeit dieser Plattformen.

`documented` — Nicht automatisierbar durch Agents: `opencode session export/import`, Änderungen an `.opencode/**`/`opencode.jsonc`, Installation und Rechteänderungen. Diese Aufgaben werden gebündelt an den Owner gegeben. Interaktive TUI-`ask`-Dialoge bleiben `unknown`.

## 9. Bekannte Einschränkungen

- `unknown` — Laufzeitverhalten der vier Denys; JSONC-Parser im Agentenkontext; installierte Laufzeitversion; zweiter Rechner.
- `documented` — Worktree-Öffnen blockiert; `opencode.worktree_remove` ungeschützt; `.env.example`-Seiteneffekt in `opencode.jsonc` bleibt bestehen.
