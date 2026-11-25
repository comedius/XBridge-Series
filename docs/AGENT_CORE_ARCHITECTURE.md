# Plattformneutraler XBridge Agent Core (Architekturgrundlage)

Dieses Dokument gehört zur **Entwicklungsinfrastruktur** und ist **nicht Teil des Produktpakets** (`AGENTS.md` §10). Es beschreibt die **konzeptionelle** Architektur eines plattformneutralen Agent Core. Es ist eine Ergänzung zu `docs/AGENT_WORKFLOW.md`; bei Konflikten gilt dessen Autoritätsordnung.

Stand: 2026-11-24. Freigegeben als Teil von `XB-P2`. Evidenzlabels: `documented` (architektonische Regel), `observed` (im Projekt vorhanden), `planned`/`unknown` (nicht implementiert bzw. nicht verifiziert).

## 1. Zweck und Nichtziele

`documented` — Der Agent Core ist die **plattformneutrale** Beschreibung von Aufgaben, Rollen, Projektzuständen, Übergaben, Persistenz und Qualitätsregeln. Er wird über Adapter auf eine konkrete Ausführungsplattform abgebildet; derzeit ist OpenCode die einzige aktive Plattform.

`documented` — **Nichtziele in XB-P2:** kein neues Laufzeit-Framework, kein Code, kein Dienst, keine API-Schlüssel, keine Plan-Komponente im Produkt, keine neue Abhängigkeit. Der Core ist eine Architektur- und Dokumentationsgrundlage. Jede spätere Implementierung benötigt einen eigenen, vom Owner freigegebenen ADR mit Design und Tests.

`documented` — Der Core verändert weder die Produktarchitektur (`docs/ARCHITECTURE.md`) noch die Prioritätenordnung aus `AGENTS.md` (Korrektheit → Stabilität → Frame-Zeiten → Performance → Grafikqualität).

## 2. Ziele

- Wiederverwendbare, plattformunabhängige Beschreibung von Paketen, Rollen, Zuständen und Qualitätsregeln.
- Klare Trennung zwischen unveränderlicher Core-Logik und plattformspezifischen Adaptern.
- Nachvollziehbare Aufgaben-, Abhängigkeits-, Status- und Ergebnisverwaltung.
- Fehler routing an den zuständigen Agenten; wiederaufnehmbare, abgebrochene Aufgaben.
- Effizienter Token-/Kontextverbrauch durch gebündelte Vorbereitung und eine Abschlussprüfung.
- Keine Bindung an einen Modellanbieter.

## 3. Konzeptmodell

| Konzept | Bedeutung | Aktuelle Entsprechung |
|---|---|---|
| `Task` | freigegebener Arbeitsumfang mit Envelope | Paket in `docs/AGENT_TASKS.md` |
| `Envelope` | verbindliche Felder vor Ausführungsbeginn | `docs/AGENT_WORKFLOW.md` |
| `Role` | Verantwortung mit wirksamen Rechten | sechs installierte Agenten (`docs/AGENT_AUTONOMY.md`) |
| `Writer` | genau ein verantwortlicher Pfadbesitzer | pro Paket namentlich benannt |
| `Artifact` | erzeugtes oder geprüftes Ergebnis | Datei, Review-Verdict, Kommandoprotokoll |
| `Gate` | Freigabe- oder Prüfpunkt | Owner-Gate, Qualitäts-Gate (`docs/AUTOMATED_QUALITY_GATES.md`) |
| `Evidence` | klassifizierte Aussage | `documented`/`observed`/`hypothesis`/`unknown` |
| `Adapter` | Abbildung des Core auf eine Plattform | OpenCode (`docs/OPENCODE_AUTOMATION_PLAN.md`) |

`documented` — Ein Task ist erst ausführbar, wenn der Envelope vollständig ist, der Writer installiert und für jeden Zielpfad berechtigt ist, keine Schreibmenge kollidiert und keine Owner-Entscheidung offen ist.

## 4. Zustandsmodell und Wiederaufnahme

`documented` — Logische Zustände:

```text
planned → prepared → in_progress → self_checked → review → correcting → done
                                      ↘ blocked ↗
                                          ↘ needs_owner
```

- `prepared`: Envelope vollständig; Vorbereitung einmalig gebündelt erfolgt.
- `self_checked`: Writer hat Leserückprüfung und erlaubte Teilprüfungen ausgeführt.
- `review`: eine gebündelte unabhängige Abschlussprüfung läuft.
- `correcting`: genau ein gebündeltes Korrekturpaket pro Writer; nur betroffene Bereiche werden erneut geprüft.
- `blocked`/`needs_owner`: konkreter Grund und benötigte Entscheidung werden im Ledger vermerkt.
- `documented` — Ledger-Abbildung (`docs/AGENT_TASKS.md`): `planned`→`planned`, `prepared`/`self_checked`/`review`/`correcting`→`in_progress`, `done`→`pass`, `blocked`→`blocked`, `needs_owner`→`needs_owner`. `superseded` bleibt ein historischer Ledger-Zustand ohne eigenes logisches Core-Gegenstück.

`documented` — Wiederaufnahme erfolgt aus dem letzten protokollierten Zustand mit dem Handoff-Payload: Package-ID, exakter Writer-Satz, geänderte Dateien, Kommandos und Ergebnisse, Testlage, bewusste Auslassungen, Evidenzlabels und offene Fragen. Es wird kein bereits verifizierter Sachverhalt erneut untersucht, solange keine neue Evidenz vorliegt.

## 5. Logische Core-Komponenten (Verantwortlichkeiten, keine Implementierung)

| Komponente | Verantwortung | Keine Implementierung in XB-P2 |
|---|---|---|
| `Planner` | Envelope, Zielkriterien, Writer-Zuordnung, Risiken | kein Automatismus, keine UI |
| `Router` | Aufgaben an installierte Rollen, Fehler an zuständigen Writer | kein Nachrichtenbus |
| `Executor` | Änderungen im freigegebenen Writer-Satz | kein Runtime-Prozess |
| `Verifier` | erlaubte Prüfungen, Beweisaufnahme | kein Test-Framework |
| `GateKeeper` | Owner-Gates, Stop-Bedingungen, Rechteprüfung | keine Rechteerweiterung |
| `StateStore` | Status, Evidenz, Korrekturrunden | keine eigene Datenbank |
| `AdapterContract` | Abbildung auf Plattformfähigkeiten | kein SDK, kein Client |

`documented` — Diese Tabelle beschreibt **Zuständigkeiten**, keine Klassen, Dateien oder Schnittstellen im Produkt. `observed` — Die heutige Persistenz liegt in `docs/AGENT_TASKS.md` und `docs/ROADMAP.md`; es existiert kein Core-Code.

## 6. Core-/Adapter-Trennung

`documented` — Der Core definiert **was** passiert (Envelope, Rollen, Zustände, Gates, Evidenz, Korrekturlogik). Ein Adapter definiert **wie** eine Plattform das ausführt (Werkzeuge, Berechtigungsmodell, Delegation, Persistenzort, Sitzungen).

Adaptervertrag (konzeptionell, nicht als Code):

1. **Rollenabbildung:** Core-Rollen auf installierte Plattformrollen; nicht installierte Spezialisten werden nie als aktiv gemeldet.
2. **Rechteabbildung:** wirksame Schreib-/Leserechte pro Pfad; Verweigerung stoppt den Task.
3. **Delegation:** genau ein Writer pro Datei; keine Subagenten-Ketten.
4. **Prüfung:** erlaubte Kommandos, echte Ergebnisse, `git diff --check`, Review mit Datei:Zeile-Belegen.
5. **Persistenz:** Status und Evidenz in projekteinternen Dokumenten; Sitzungen/Ruhezustände plattformspezifisch.
6. **Publikationsübergabe:** Subagenten geben Ergebnisse an den Coordinator; der Coordinator veröffentlicht nur geprüfte Pakete über die konfigurierten Git/GitHub-Regeln. Adapter und Subagenten veröffentlichen nicht.
7. **Evidenz:** Core-Labels bleiben unverändert; Adapter darf sie nicht in „erfolgreich" umdeuten.

`documented` — Ein Adapter darf Core-Regeln nicht abschwächen. Wenn eine Plattform eine Core-Regel nicht abbilden kann, wird der Task `blocked`/`needs_owner`, nicht still angepasst.

## 7. Persistenz und Wiederherstellung

- `documented` — Status und Envelope: `docs/AGENT_TASKS.md`; Phasen und Meilensteine: `docs/ROADMAP.md`; verbindliche Ablaufregel: `docs/AGENT_WORKFLOW.md`.
- `observed` — Konfiguration und Agentenprompts liegen im Projekt (`opencode.jsonc`, `.opencode/**`); keine Abhängigkeit von Benutzerprofil oder globalem Agentenverzeichnis (`docs/PORTABLE_SETUP.md:42`).
- `documented` — `build/**` ist generiert und wird auf jedem Rechner neu erzeugt; keine Build-Artefakte als Statusquelle verwenden.
- `documented` — OpenCode-Sitzungsübertragung ist Owner-Aufgabe (`session export/import`); Agents führen sie nicht aus. Originale bleiben unangetastet; keine aktive SQLite-Datei direkt kopieren.
- `observed` — Lokale Sicherungen unter `C:\Users\Dennis\AppData\Local\Temp\opencode\` sind Arbeitspapiere des Owners, keine Projektabhängigkeit und nicht zu publizieren.
- `documented` — Nach Abbruch wird der letzte protokollierte Zustand fortgesetzt; eine widersprüchliche oder fehlende Protokollierung ist ein Blocker, kein Freibrief für erneute Änderungen.

## 8. Plattformabbildung

| Plattform | Status | Anmerkung |
|---|---|---|
| OpenCode | `observed` — aktuell aktiv | Adapter beschrieben in `docs/OPENCODE_AUTOMATION_PLAN.md`; Version `2.4.1` quellenseitig belegt, Laufzeitversion `unknown` |
| Codex | `planned` | architektonisch berücksichtigbar; kein Adapter, keine Aussage über Fähigkeiten |
| Claude Code | `planned` | architektonisch berücksichtigbar; kein Adapter, keine Aussage über Fähigkeiten |
| GitHub Copilot | `planned` | architektonisch berücksichtigbar; kein Adapter, keine Aussage über Fähigkeiten |

`documented` — Es gibt keine Bindung an einen Modellanbieter. Namensnennungen sind mögliche spätere Adapter, keine Gegenwartsfunktion und keine Empfehlung. Ein neuer Adapter benötigt eine eigene Owner-Entscheidung, eigene Rechteprüfung und eigene Verifikation.

## 9. Beziehung zur Produktgrenze

`documented` — Agent Core, Adapter, Prompts, Workflow-Dokumente und Automation sind Entwicklungsinfrastruktur und bleiben im öffentlichen Entwicklungsrepository. Das Produktpaket wird später aus einer expliziten Allowlist gebildet; diese Dokumente werden automatisch ausgeschlossen. Produktcode enthält keine Agenten-, OpenCode-, API-Key- oder KI-Dienstlogik, solange dies nicht durch eine eigene ADR und Owner-Freigabe als Produktfunktion definiert ist.

## 10. Offene Punkte

- `unknown` — Laufzeitverhalten der angewendeten Code-Mode-Denys; JSONC-Validierung im Agentenkontext; interaktive Berechtigungsdialoge; zweiter Rechner.
- `documented` — Keine Implementierung des Core in diesem Paket; spätere Implementierung erfordert ADR und Owner-Freigabe.
