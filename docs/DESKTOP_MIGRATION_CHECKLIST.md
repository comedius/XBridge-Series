# Desktop-Entwicklungsumgebung und Migrations-Checkliste

Dieses Dokument gehört zur **Entwicklungsinfrastruktur** und ist **nicht Teil des Produktpakets** (`AGENTS.md` §10). Es beschreibt die erforderliche Desktop-Umgebung sowie die Prüfungen für den aktuellen Rechner und einen künftigen zweiten Windows-Rechner. Verbindliche Quellen bleiben `docs/DEVELOPMENT_ENVIRONMENT.md` (technischer Ist-Zustand) und `docs/PORTABLE_SETUP.md` (Schritte für den zweiten Rechner).

Stand: 2026-11-24. Freigegeben als Teil von `XB-P2`. Evidenzlabels: `observed` (direkt geprüft), `owner-provided` (vom Owner angegeben, nicht agentenverifiziert), `documented` (Projektdokumentation), `unknown` (nicht verifiziert), `historisch` (alter Snapshot).

## 1. Grundsatz

`documented` — `<PROJECT_ROOT>` ist stets das Verzeichnis mit den Projektregeln; der Laufwerksbuchstabe ist unerheblich. Werkzeuge sind lokal installiert und gehören nicht ins Projekt. `build/**` ist generiert und wird auf jedem Rechner neu erzeugt. Keine Abhängigkeit von Benutzerprofil, globalem Agentenverzeichnis oder festem Installationspfad in Projektdateien.

`documented` — Maschinenidentität und Hardwarewerte des aktuellen Desktops sind `owner-provided`, nicht agentenverifiziert (`docs/DEVELOPMENT_ENVIRONMENT.md:3,28-30,54-59`). `observed` ist der aktuelle Workspace `D:\XBridge 1.0\XBridge-Series-Autonomy`; `D:\XBridge 1.0\XBridge-Series-Github` und `E:`-Pfade sind **historisch**. Über den zweiten Rechner werden keine Annahmen getroffen.

## 2. Aktueller Desktop (Ist-Zustand)

| Element | Wert / Status |
|---|---|
| Betriebssystem | Windows 11 Pro, Build 26200 (x64) — `owner-provided`; Plattform `win32` `observed` |
| Hosting-CPU / RAM / GPU | Ryzen 7 5700X / 16 GB / RTX 2060 SUPER — `owner-provided`, nicht agentenverifiziert |
| Visual Studio | 2026 Community, Generator `Visual Studio 18 2026` — `observed` (`CMakePresets.json:7`) |
| Installationspfad VS | `C:\Program Files\Microsoft Visual Studio\18\Community` — `observed`; `E:\…` `historisch` |
| CMake / CTest | `4.3.1-msvc1` über einfachen Befehlsnamen — `observed` durch `tester`, Exit-Codes `unknown` |
| MSVC / SDK | Toolsets `14.44.35207`, `14.52.37025`; SDKs `10.0.26100.0`, `10.0.28000.0` — `observed` |
| Ninja | `1.13.2` — `owner-provided`; VS-bundelte `ninja.exe` `observed` |
| Git | `2.56.1` — `owner-provided`; `git.exe` und Leseoperationen `observed` |
| GitHub Desktop | `app-3.7.0` installiert — `observed`; Publikation erfolgt koordiniert durch den Coordinator, nicht über Desktop |
| OpenCode | Ressourcenversion `2.4.1` — `observed`; Laufzeitversion `unknown`; Desktop installiert, Version `unknown` |

`historisch`: `E:`-Pfade, `CMakePresets.json.backup:7` mit `Visual Studio 17 2022` und ältere Build-Ordner sind vergangene Snapshots, keine aktuelle Konfiguration. `documented`: `windows-debug` unverändert auf VS2026, `ninja-debug` auf Ninja.

## 3. Repository- und Projektstruktur

`observed` — Das Projekt enthält `CMakeLists.txt`, `CMakePresets.json`, `AGENTS.md`, `docs/**`, `src/**`, `include/**`, `tests/**`, `opencode.jsonc` und `.opencode/agents/**`. Sechs installierte Rollen; neun Spezialrollen `planned`. `ai/README.md` ist Coordinator-Dokumentation und wird hier abgeglichen; Produktcode bleibt unberührt.

`documented` — Produkt und Infrastruktur sind getrennt: `.opencode/**`, `opencode.jsonc`, `AGENTS.md`, `ai/**`, Workflow-/Ledger-Dokumente und temporäre Artefakte gehören ins öffentliche Entwicklungsrepository und werden später per expliziter Allowlist aus dem Produktpaket ausgeschlossen (`docs/ARCHITECTURE.md:72-74`, `docs/RELEASE_CHECKLIST.md`).

`observed` — Lokale Arbeitspapiere unter `C:\Users\Dennis\AppData\Local\Temp\opencode\` sind Owner-Sicherungen, keine Projektabhängigkeit und nicht zu publizieren. Der Ignorierschutz für `.owner-patch-backup/`, `CMakePresets.json.backup` und `debug-*.txt` ist angewendet; die Dateien bleiben erhalten.

## 4. OpenCode-Einrichtung

`documented`/`observed`:

1. OpenCode auf dem Zielrechner lokal installieren und über `PATH` oder einen lokalen CLI-Pfad aufrufen; keine festen Pfade in Projektdateien.
2. Im `<PROJECT_ROOT>` arbeiten: `Set-Location '<PROJECT_ROOT>'`.
3. Prüfen (alle `opencode *`-Befehle nur durch den Owner außerhalb eines Agenten; alle Rollen verweigern `shell "opencode *"`): `opencode --version`, `opencode debug paths`, `opencode debug config`, `opencode debug agents`.
4. Erwartung: `default_agent = coordinator`; `instructions = docs/AGENT_WORKFLOW.md`; `worktree.directory = ../worktrees`; sechs installierte Rollen.
5. Rechte nicht erweitern, um Prüfungen grün zu machen. V2-Regeln: letzte passende Regel gewinnt; harte `policies` sind nicht aufhebbar.

`observed` — Der Owner hat vier globale Denys für mutierende Code-Mode-Tool-Identitäten angewendet; der unabhängige Reviewer bestätigte den angewendeten Zustand mit `PASS` (`ses_df1b60181ffek5mOBp1DHBgV5M`). **Laufzeitwirkung `unknown`**; keine Agentenänderung daran. `opencode.worktree_remove` bleibt ungeschützt; ein pauschales `edit:* deny` ist nicht freigegeben.

`documented` — 13 OpenCode-Sitzungen (drei Projekt, zehn Setup) sollen ausschließlich mit `session export`/`session import` übertragen werden. Agents dürfen das nicht; Originale auf `C:` bleiben unangetastet; keine aktive SQLite-Datei direkt kopieren. `unknown`: erfolgreiche Übertragung liegt nicht vor.

## 5. Verifikation der Entwicklungsumgebung

`observed`/`documented` — Lesende Prüfungen im aktuellen Checkout:

```powershell
Set-Location "D:\XBridge 1.0\XBridge-Series-Autonomy"
git status --porcelain=v1 --untracked-files=all
git rev-parse HEAD
git branch --show-current
git diff --check
```

`documented` — Build/Test nur in Implementierungspaketen und durch den `tester` (der Coordinator ist für `cmake *`/`ctest *` gesperrt):

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug --config Debug
ctest --test-dir build/windows-debug -C Debug --output-on-failure
```

Alternativ `ninja-debug` gemäß `docs/PORTABLE_SETUP.md:75-81`. `historisch`: Exit `0` und `100% tests passed, 0 tests failed out of 1` wurden nur auf dem ersten Rechner mit absoluten VS2026-Executables protokolliert; keine aktuelle Wiederholung. `observed`: `git diff --check; "EXIT=$LASTEXITCODE"` lieferte in `XB-AUTONOMY-ACTIVATION-01` `EXIT=0`; die ältere CRLF-Warnung für `CMakePresets.json` ist historisch.

`documented` — Jede Prüfung mit exaktem Kommando und echtem Ergebnis protokollieren; keine historischen Ergebnisse als neue Verifikation ausgeben.

## 6. Checkliste zweiter Rechner / externe SSD

- [ ] `documented` — Externe SSD nur an einem Rechner gleichzeitig verwenden; sauber trennen, nicht während Build/Git-Schreibvorgang.
- [ ] `documented` — `<PROJECT_ROOT>` neu bestimmen; keine Annahme `E:`; keine alten Build-Ordner übernehmen.
- [ ] `documented` — Bei `detected dubious ownership`: `safe.directory` **pro Kommando** setzen, nie global.
- [ ] `documented` — Git mit HTTPS-Zugriff auf `origin`, falls nötig; keine globalen Git-Konfigurationsänderungen durch Agents.
- [ ] `documented` — Windows x64, Visual Studio 2026 Community mit C++/C#-Komponenten, CMake/CTest, Ninja lokal vorhanden; keine festen Installationspfade im Projekt.
- [ ] `unknown` — Werkzeugversionen auf dem zweiten Rechner durch den Owner vor Ort prüfen (`cmake --version`, `ctest --version`, `git --version`, `opencode --version`); keine Installation durch Agents.
- [ ] `documented` — Schreibrechte auf der externen Festplatte für neu erzeugte `build/**`.
- [ ] `documented` — Nach Wechsel Abschnitte 4–5 erneut ausführen; `CMAKE_HOME_DIRECTORY` muss auf `<PROJECT_ROOT>` zeigen.
- [ ] `documented` — Keine Zugangsdaten/API-Schlüssel, keine Build-Artefakte, Logs oder generierten Caches mitnehmen.
- [ ] `unknown` — Vollständiger Zweitrechner-Test liegt nicht vor; Portabilität nicht bestätigt.

## 7. Bekannte Blockaden und offene Punkte

| Punkt | Status |
|---|---|
| Zweiter Rechner / Portabilität | `unknown` — kein bestätigter Test |
| Sitzungsübertragung (13 Sitzungen) | `planned`/`blocked` — Owner-Aufgabe, kein Nachweis |
| Worktree-Öffnen | `blocked` — `Created worktree could not be opened`, Owner-Entscheidung offen |
| Interaktive TUI-`ask`-Dialoge | `unknown` — nicht geprüft |
| Installierte OpenCode-Laufzeitversion | `unknown` — nicht per allowlistetem Kommando geprüft |
| JSONC-Parser im Agentenkontext | `unknown` — kein zugängliches Werkzeug, keine Installation |
| Laufzeitverhalten der vier Code-Mode-Denys | `unknown` — kein kontrollierter Laufzeitcheck |
| `ai/README.md` | `documented` — Coordinator-Dokumentation, in `XB-AUTONOMY-ACTIVATION-02` mit der Publikationsregel abgeglichen; kein Produktcode |

## 8. Gesamtstatus

`documented` — Diese Checkliste beschreibt Voraussetzungen und Prüfungen, **keine** bestätigte Xbox-Emulation und keine bestätigte zweite Entwicklungsumgebung. `observed` — Der aktuelle Checkout ist einsatzfähig für die dokumentierten Leseprüfungen; Build-/CTest-Erfolge sind `historisch`. `unknown` bleibt dort stehen, wo keine echte Evidenz vorliegt.
