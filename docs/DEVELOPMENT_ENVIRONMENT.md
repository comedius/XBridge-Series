# XBridge Series – Entwicklungsumgebung

Diese Datei gehört zur **Entwicklungsinfrastruktur** und ist **nicht Teil des Produktpakets** (`AGENTS.md` §10/§11). Sie beschreibt den technischen Ist-Zustand der Entwicklungsumgebung als Referenz für weitere Entwicklungsaufgaben. `observed`: Der aktive Workspace ist `D:\XBridge 1.0\XBridge-Series-Autonomy`. `owner-provided`: Der hier beschriebene Desktop-PC ist die aktive Entwicklungsumgebung; der frühere Pfad `D:\XBridge 1.0\XBridge-Series-Github` ist **historisch** und nicht mehr aktuell. Ein weiterer, noch nicht geprüfter Rechner ist ein separates zukünftiges Migrationsziel (`planned`/`unknown`); es werden keine Annahmen über ihn getroffen.

Stand der Aufnahme: 2026-11-24.

## 1. Zweck und Geltungsbereich

Dieses Dokument ist die zentrale Referenz für **technische Umgebungsdaten**: installierte Werkzeuge und Versionen, Verzeichnisse, Build-/Testverfahren, Agentenrollen, Berechtigungen und offene Prüfpunkte.

Es ergänzt, ersetzt aber nicht:

| Dokument | Zuständigkeit |
|---|---|
| `AGENTS.md` | Projektregeln, Architekturgrundsätze, C++/Testpflichten, Owner-Kontrolle |
| `docs/AGENT_WORKFLOW.md` | Paket-Envelope, Rollenmodell, Review-Fix-Ablauf, Produkt-/Infrastrukturgrenze |
| `docs/PORTABLE_SETUP.md` | Konkrete Schritte für den zweiten Rechner / externe SSD |
| `docs/AGENT_TASKS.md` | Paketverlauf, Envelopes, Roh-Evidenz mit Kommandos und Ergebnissen |

**Evidenzlabels** (durchgängig in diesem Dokument verwendet):

| Label | Bedeutung |
|---|---|
| `observed` | Im Rahmen der P2-Bestandsaufnahme bzw. dieser Beauftragung direkt aus Dateien/Konfiguration gelesen |
| `owner-confirmed` | Vom Eigentümer zuvor bestätigt und in diesem Auftrag erneut als bestätigt übermittelt; hier nicht erneut per Kommando geprüft |
| `documented` | In vorhandener Projektdokumentation festgehalten |
| `unknown` | Derzeit nicht verifiziert – darf nicht als Tatsache behandelt werden |
| `owner-provided` | Vom Eigentümer als Ist-Zustand angegeben, aber nicht agentenseitig per allowlistetem Kommando verifiziert; keine agentenverifizierte Tatsache |

Zusätzlich werden `planned` (vorgesehen, nicht umgesetzt/geprüft) und `hypothesis` (plausibel, unbelegt) verwendet. Historische Prüfergebnisse werden ausdrücklich als **historisch** gekennzeichnet. Der aktuelle Workspace-Pfad `D:\XBridge 1.0\XBridge-Series-Autonomy` ist `observed`; `D:\XBridge 1.0\XBridge-Series-Github` ist ein **historischer** Checkout-Pfad. Die Rechneridentität („Desktop-PC", Zuordnung zum geplanten zweiten Rechner) und die Hardwarewerte sind `owner-provided`, nicht agentenverifiziert.

**Quellen im Repository:** `AGENTS.md`; `docs/AGENT_WORKFLOW.md`; `docs/PORTABLE_SETUP.md`; `docs/AGENT_TASKS.md`; `docs/ROADMAP.md`; `docs/ARCHITECTURE.md`; `opencode.jsonc`; `.opencode/agents/{coordinator,developer,tester,architect,reviewer,researcher}.md`; `CMakeLists.txt`; `CMakePresets.json`; `CMakePresets.json.backup`; `build/windows-debug/CMakeCache.txt`.

## 2. Aktueller Projektstand und technische Zielplattform

`observed` / `documented`:

- C++20-CMake-Projekt `XBridgeSeries`, Version `0.3.0` (`CMakeLists.txt:1-8`).
- Bibliothek `xbridge_core` mit Modulen `core`, `cpu`, `memory`, `profiles`, `host`; CLI-Demo `xbridge`; ein CTest-Ziel `xbridge_tests` (`CMakeLists.txt:10-37`).
- Synthetische `TestCpu`, generischer `GuestMemory`, `X64CpuState`, deterministische `ExecutionPolicy` mit Mock-Fähigkeiten; Decoder/Interpreter-Subset für x64 NOP/MOV (`docs/ROADMAP.md:5-10`).
- Der aktuelle Stand ist ein **C++20-Grundgerüst mit synthetischer Test-CPU und bietet keine Xbox-Emulation**. Die Demo gibt laut `docs/AGENT_TASKS.md:374` ausdrücklich `Status: scaffold demo completed (not Xbox emulation)` aus.

`documented` (`AGENTS.md` §2, `docs/ROADMAP.md:43-49`):

- Technische Zielplattform ausschließlich **Xbox Series X**. **Xbox Series S bleibt außerhalb des Projektumfangs.**
- Der Interpreter ist die sichere CPU-Ausführungsbasis. **JIT bleibt ausgeschlossen**, solange keine eigene technische Entscheidung mit Belegen, Design und Tests vorliegt.
- Gast-CPU-Semantik bleibt von Host-Fähigkeiten getrennt; `GuestMemory` bleibt generisch und wird nicht als Xbox-Virtual-Memory-System angenommen.
- Aktuelle Phase: Phase 1 „Foundation" / Phase 2 „Agent-assisted development" (`docs/ROADMAP.md:5,12`).

## 3. Betriebssystem und Hardware – bekannte Werte und offene Angaben

| Angabe | Wert | Status / Quelle |
|---|---|---|
| Betriebssystem | Windows 11 Pro, Build 26200 (x64) | `owner-provided` (Edition/Build nicht agentenverifiziert); `observed` (Plattform `win32`). Historische Angabe „Laufwerk `E:`" (`docs/PORTABLE_SETUP.md:56`) gilt nicht für den aktuellen Desktop |
| Aktueller Rechner | `observed`: Workspace `D:\XBridge 1.0\XBridge-Series-Autonomy`; `E:` existiert in dieser Umgebung nicht | Pfad `observed`; `D:\XBridge 1.0\XBridge-Series-Github` und die Bezeichnung „erster Rechner" mit `E:`-Laufwerk sind **historisch** (`docs/PORTABLE_SETUP.md`, `docs/AGENT_TASKS.md`). Rechneridentität als aktive Entwicklungsumgebung `owner-provided`, nicht agentenverifiziert |
| Externe SSD | Gemeinsame Verwendung an zwei Rechnern, jedoch **immer nur an einem Rechner gleichzeitig** | `owner-confirmed` (siehe Abschnitt 11) |
| Host-CPU (Modell) | AMD Ryzen 7 5700X | `owner-provided`, nicht agentenverifiziert (zuvor `unknown`) |
| Host-RAM (Größe) | 16 GB | `owner-provided`, nicht agentenverifiziert (zuvor `unknown`) |
| Host-GPU | NVIDIA GeForce RTX 2060 SUPER | `owner-provided`, nicht agentenverifiziert (zuvor `unknown`) |

Hinweis: Hardwareangaben im Projekt beziehen sich auf das **Gastziel** (Xbox Series X), nicht auf das Entwicklungssystem. Es werden keine Host-Hardwarewerte aus Gast-Annahmen abgeleitet.

## 4. Entwicklungswerkzeuge und Versionen

| Werkzeug | Wert | Status / Quelle |
|---|---|---|
| Visual Studio | **2026 Community**, Generator `Visual Studio 18 2026` | `observed` (`CMakePresets.json:7`, `CMakeCache.txt:254` = `Visual Studio 18 2026`). Der Pfad `.../18/Community` in `CMakeCache.txt:256` ist **historisch** (erster Rechner, `E:`); heute `observed`: `C:\Program Files\Microsoft Visual Studio\18\Community` |
| Installationsverzeichnis VS | `observed`: `C:\Program Files\Microsoft Visual Studio\18\Community` | `observed` (Dateisystem, aktueller Desktop). Der Wert `E:\PRO\Programm Daten\Visual Studio\18\Community` (`CMakeCache.txt:222,226,256`) ist **historisch** |
| MSVC | `observed`: Toolsets `14.44.35207` und `14.52.37025` | Beide Toolsets `observed` (Dateisystem). Compiler `19.52.37029` `owner-provided`, nicht agentenverifiziert (zuvor als `owner-confirmed` geführt) |
| Windows SDK | `observed`: `10.0.26100.0` und `10.0.28000.0` | Beide SDKs `observed` (Dateisystem); zuvor war nur `10.0.28000.0` (`owner-confirmed`) verzeichnet |
| CMake | `4.3.1-msvc1` | `observed`: `cmake --version` → `cmake version 4.3.1-msvc1`, aufgelöst über einfachen Befehlsnamen durch `tester` (Exit-Code nicht exponiert → `unknown`). `CMakeCache.txt:216-220` = 4/3/1 **historisch**. Mindestanforderung `3.20` (`CMakeLists.txt:1`) |
| Ninja | `1.13.2` | `owner-provided` (Version nicht agentenverifiziert; `git/ninja --version` nicht allowlistet); `observed`: VS-bundelte `ninja.exe`, `ninja-debug` nutzt Generator `Ninja` (`CMakePresets.json:14`) |
| CTest | `4.3.1-msvc1` | `observed`: `ctest --version` → `ctest version 4.3.1-msvc1` durch `tester` (Exit-Code nicht exponiert → `unknown`). Pfad `CMakeCache.txt:226` historisch |
| MSBuild | `18.12.3` | `historisch dokumentiert` (`docs/AGENT_TASKS.md:95`, Build-Log des ersten Rechners) |
| Git | `2.56.1`; HTTPS-Zugriff auf `origin` erforderlich | Version `owner-provided`, nicht agentenverifiziert (`git --version` nicht in der Allowlist). `observed`: `git.exe` unter `C:\Program Files\Git\cmd`; Git-Lesekommandos funktionieren über Befehlsnamen |
| GitHub Desktop | `app-3.7.0`, installiert | `observed` (Verzeichnis `C:\Users\Dennis\AppData\Local\GitHubDesktop\app-3.7.0`). Publikation erfolgt nicht über Desktop, sondern koordiniert: der Coordinator darf laut `AGENTS.md` §8 / `AGENT_WORKFLOW.md` committen, pushen und PRs verwalten (siehe §10); die ältere Owner-Only-Aussage ist **historisch**. |
| OpenCode CLI | Ressourcen-Version `2.4.1` | `observed` (`opencode-cli.version`); Laufzeitversion `unknown` (nicht per allowlistetem Kommando geprüft). `2.3.2` (`docs/AGENT_TASKS.md:28,44`) ist **historisch** |
| OpenCode Desktop | installiert | `observed` (Desktop-Anwendung vorhanden); konkrete Version `unknown` |

Compileroptionen `observed` (`CMakeLists.txt:23-27`): MSVC `/W4 /permissive-`, sonst `-Wall -Wextra -Wpedantic`; C++20 explizit erforderlich (`CMAKE_CXX_STANDARD 20`, `CMAKE_CXX_STANDARD_REQUIRED ON`, `CMAKE_CXX_EXTENSIONS OFF`).

## 5. Installations- und Projektverzeichnisse

| Pfad / Konzept | Status / Quelle |
|---|---|
| Projektverzeichnis `D:\XBridge 1.0\XBridge-Series-Autonomy` | `observed` (aktuelles Arbeitsverzeichnis, Git-Kommandos). `D:\XBridge 1.0\XBridge-Series-Github` und der Wert `E:\XBridge 1.0\XBridge-Series-Github` (`CMAKE_HOME_DIRECTORY`, `docs/AGENT_TASKS.md:90`) sind **historisch** |
| VS-Installation `C:\Program Files\Microsoft Visual Studio\18\Community` | `observed` (siehe Abschnitt 4). `E:\PRO\Programm Daten\Visual Studio\18\Community` ist **historisch** |
| `<PROJECT_ROOT>`-Konvention | `documented`: immer das Verzeichnis, das die Projektregeln enthält; Laufwerksbuchstabe ist egal (`docs/PORTABLE_SETUP.md:9`) |
| Werkzeuge | `documented`: lokale Installationen, nicht im Projekt; gefunden über `PATH`, CMake-Erkennung oder lokale Toolchain (`docs/PORTABLE_SETUP.md:11`) |
| Build-Ausgaben | `documented`: unter `build/**`, auf jedem Rechner neu erzeugt, nie zwischen Rechnern kopiert (`docs/PORTABLE_SETUP.md:12,86`) |
| Worktree-Ziel | `observed`: `opencode.jsonc:7-9` → `worktree.directory = "../worktrees"` (relativ zum Checkout, laufwerksunabhängig) |
| OpenCode-Sitzungsdaten | `documented`/`planned`: separater portabler Kontext außerhalb von `<PROJECT_ROOT>`, nicht im Projekt (`docs/PORTABLE_SETUP.md:10,105`) |
| Persönliche Zugangsdaten / API-Keys | `documented`: bewusst lokal, nicht im Projekt; keine harten Benutzerprofil-Pfade in der Projektkonfiguration (`docs/PORTABLE_SETUP.md:94`, `docs/AGENT_TASKS.md:45`) |

## 6. Build-Verzeichnisse und CMake-Presets

`observed` (`CMakePresets.json`, `CMakeCache.txt`):

| Element | Wert |
|---|---|
| Preset-Datei | `CMakePresets.json`, `version: 3` |
| `windows-debug` | Generator `Visual Studio 18 2026`, `architecture: x64`, `binaryDir: ${sourceDir}/build/windows-debug` (`:5-10`) |
| `ninja-debug` | Generator `Ninja`, `binaryDir: ${sourceDir}/build/ninja-debug`, `CMAKE_BUILD_TYPE: Debug` (`:11-19`) |
| Build-Presets | `windows-debug`, `ninja-debug` (`:21-24`) |
| Test-Preset | `ninja-debug` mit `outputOnFailure: true` (`:25-27`) |
| Weitere Build-Ordner | `build/windows-2026-debug`, `build/windows-debug-vs2022` – `historisch dokumentiert` (`docs/AGENT_TASKS.md:90`), generiert, nicht versionsverwaltet |
| `CMakePresets.json.backup` | `historisch dokumentiert`: enthält die alte Zeile `Visual Studio 17 2022` (`:7`), bewusst aufbewahrt, nicht Teil des publizierten Stands (`docs/PORTABLE_SETUP.md:59,102`) |

`git diff --check` wurde in XB-AUTONOMY-ACTIVATION-01 vom Coordinator ausgeführt: `git diff --check; "EXIT=$LASTEXITCODE"` → `EXIT=0` (`observed`, `docs/AGENT_TASKS.md:157,169`). Die früher dokumentierte CRLF-Warnung für `CMakePresets.json` (`docs/RELEASE_CHECKLIST.md:47`, `docs/AGENT_TASKS.md:92,97`) ist **historisch** und nicht blockierend; ältere `unknown`-Angaben zum Exit-Code sind **historisch**.

## 7. Build- und Testverfahren

`documented` (`docs/PORTABLE_SETUP.md:65-88`) – exakte Kommandos:

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug --config Debug
ctest --test-dir build/windows-debug -C Debug --output-on-failure
```

Alternativ Ninja:

```powershell
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

Historische Verifikation (ausschließlich **erster Rechner**, `docs/AGENT_TASKS.md:93-97`):

- Mit den aus dem `CMakeCache.txt` belegten **absoluten VS2026-Executables** unter `E:\PRO\Programm Daten\Visual Studio\18\Community\...\CMake\bin\` liefen Configure (`exit 0`), Debug-Build (`exit 0`, Artefakte `xbridge_core.lib`, `xbridge.exe`, `xbridge_tests.exe`) und CTest (`exit 0`, `1/1 Test`, `100% tests passed, 0 tests failed out of 1`). Der `E:`-Pfad und die Exit-Codes sind **historisch**.
- Diese Ergebnisse sind **historisch** und wurden in dieser Aufnahme **nicht** wiederholt. Sie belegen einen Build/Test des Scaffolds, **keine** Xbox-Emulation.
- `observed` (aktuell): Plain `cmake` und `ctest` sind im Agentenkontext über den einfachen Befehlsnamen erreichbar – der `tester` meldete `cmake version 4.3.1-msvc1` und `ctest version 4.3.1-msvc1`, jeweils ohne Ablehnung; Exit-Codes nicht exponiert (`unknown`). Die historische `CommandNotFoundException` (`docs/AGENT_TASKS.md:92`) ist **superseded**; ihre damalige Ursache bleibt `unknown`. Der Coordinator bleibt für `cmake *`/`ctest *` bewusst gesperrt (`coordinator.md:80-85`), kein Fehler.
- Es existiert genau ein CTest-Ziel `xbridge_tests` (`CMakeLists.txt:32-37`); keine Differenz-, Fuzz-, Coverage- oder Grafiktests (`docs/AGENT_TASKS.md:381`).

## 8. OpenCode-Agenten und ihre Aufgaben

`observed` (Frontmatter) / `documented` (`docs/AGENT_WORKFLOW.md`):

**Installiert und validiert: sechs Rollen.**

| Rolle | Modus | Aufgabe | Schreibumfang (Kurzform) |
|---|---|---|---|
| `coordinator` | primary | Plant Pakete, delegiert, führt Ledger/Planung, steuert Review-Korrekturen | `docs/AGENT_TASKS.md`, `docs/AGENT_WORKFLOW.md`, `docs/ROADMAP.md`, `docs/RELEASE_CHECKLIST.md`, `docs/PORTABLE_SETUP.md`, `docs/DEVELOPMENT_ENVIRONMENT.md` |
| `architect` | subagent | Read-only Architektur-, Interface-, ADR- und Risikoanalyse | keine (`edit:* deny`) |
| `developer` | subagent | Implementiert freigegebene Pakete; Tests nur bei ausdrücklicher Zuweisung als sole writer | `src/**`, `include/**`, `tests/**`, `CMakeLists.txt` |
| `tester` | subagent | Unabhängige Testausführung und Regression | nur `tests/**` |
| `researcher` | subagent | Evidenzgelabelte öffentliche Forschung | `docs/RESEARCH_REGISTER.md`, `docs/research/**`; Web-Tools erlaubt |
| `reviewer` | subagent | Streng read-only Diff-/Vertrags-/Sicherheitsreview | keine (`edit:* deny`) |

**Built-in-Agents** `observed` (`opencode.jsonc:14-71`): `build`, `compaction`, `explore`, `plan`, `summary`, `title` mit `edit/external_directory/shell "*" deny`; `general` mit `disabled: true`.

**Nicht installiert / `planned`:** neun Spezialrollen (`cpu-developer`, `memory-developer`, `gpu-developer`, `audio-developer`, `input-developer`, `platform-developer`, `test-engineer`, `performance-engineer`, `integration-engineer`). Insbesondere `test-engineer` bleibt `planned`; `tests/**` wird derzeit nicht exklusiv von ihm gehalten (`docs/AGENT_WORKFLOW.md`, `docs/ROADMAP.md:18`).

Konfiguration `observed` (`opencode.jsonc:4-13`): `default_agent = coordinator`; `instructions = docs/AGENT_WORKFLOW.md`; `worktree.directory = ../worktrees`. Keine Abhängigkeit von Benutzerprofil oder globalem Agentenverzeichnis (`docs/PORTABLE_SETUP.md:41`).

## 9. Aktuelle Berechtigungen und Sicherheitsgrenzen

`documented` (`docs/AGENT_WORKFLOW.md`, „Permission model"): V2-Modell; globale Regeln werden vor Agentenregeln geladen, **die letzte passende Regel gewinnt**; `edit` umfasst Schreiben/Patchen; `policies` sind nicht über Regel-Freigaben aufhebbare Harte Denys. Shell-Pattern sind **keine** OS-Sandbox, sondern schützen nur das Agentenverhalten.

`observed` (`opencode.jsonc`, Agenten-Frontmatter):

- **Global:** `external_directory "*" ask` (`opencode.jsonc:75`), `shell "*" ask` (`:77`), `question "*" ask` (`:117`), `subagent` erlaubt genau `architect`, `developer`, `tester`, `reviewer`, `researcher` (`:118-122`), `webfetch/websearch "*" deny` (`:123-124`). Ältere Angaben in diesem Abschnitt sind **historisch** und durch XB-AUTONOMY-ACTIVATION-01 superseded.
- **Harte Denys (`opencode.jsonc:142-172`):** destruktive Git-Befehle (u. a. `rebase`, `reset`, `clean`, `checkout`, `switch`, `restore`, `stash`, `revert`, `cherry-pick`, `pull`, `tag`, `worktree`, `git config --global *`), Install-Kommandos, `Remove-Item`/`rm`/`del`/`Move-Item`, `read/edit:.git/**` und `.env`-Zugriffe (inkl. dokumentierter `.env.example`-Seiteneffekt). `git add/commit/push/merge` und `gh auth/pr/repo` sind **keine** harten Denys, sondern für den Coordinator als Shell-Regeln erlaubt (`coordinator.md:125-145`). Mutierende Code-Mode-Werkzeuge sind über vier Regeln erfasst: `automation_create`, `automation_delete`, `opencode_session_move`, `opencode_worktree_create` (`opencode.jsonc:134-139`); die Laufzeitwirkung bleibt `unknown`.
- **Coordinator:** `shell "*" deny`, danach exakte Git-Read-Kommandos einschließlich `git status --short --branch` (`coordinator.md:92-124`) sowie `git add/commit/push/merge`, `gh auth status`, `gh pr *`, `gh repo view *` (`coordinator.md:125-145`). `question "*" allow`, `webfetch/websearch deny`; `opencode *` bleibt bewusst deny.
- **Entwickler/Tester:** exakte Git- sowie CMake-/CTest-Befehle erlaubt; `opencode *` und `shell "*" deny`.
- **Architect/Reviewer:** `edit "*" deny`, alle Shell-Zugriffe deny – strikt read-only.
- **Researcher:** Web-Tools erlaubt, Shell deny.

**Vom Eigentümer ausdrücklich bestätigte Entscheidungen** (in dieser Aufnahme zu dokumentieren, nicht als Problem zu behandeln):

- `plan.question = allow` (`opencode.jsonc:54`) ist **beabsichtigt und freigegeben**.
- `coordinator → edit docs/DEVELOPMENT_ENVIRONMENT.md = allow` (`coordinator.md:23-25`) ist **beabsichtigt und freigegeben**; die Freigabe zur Erstellung dieser Datei erfolgte mit dem vorliegenden Auftrag.

Weitere Begrenzungen `observed`: `opencode debug config` / `opencode debug agents` sind für Agents über `shell "opencode *" deny` blockiert (Owner-Prüfung außerhalb des Agenten erforderlich). `git status --short --branch` ist seit XB-AUTONOMY-ACTIVATION-01 auch im Coordinator erlaubt (`coordinator.md:95-97`) und lieferte `## main...origin/main`, `EXIT=0`; die ältere Blockade-Aussage ist **historisch**.

## 10. Git-, Review- und Veröffentlichungsprozess

`documented` (`AGENTS.md` §4/§6/§8/§9, `docs/AGENT_WORKFLOW.md`):

- Jede Aufgabe folgt: Plan → Implementierung → Selbstprüfung → unabhängiges Review → gebündelte Korrektur → erneutes Review → Abschluss. Maximal zwei Korrekturrunden; danach Eskalation.
- Review-Findings werden klassifiziert als `ACTIONABLE`, `OWNER_DECISION`, `NON_BLOCKING`, `UNKNOWN`; der Reviewer bleibt read-only.
- Pflichtnachweise: exakte Kommandos und reale Ergebnisse, `git diff --check`, vollständige Diff-Inspektion, CTest bei Implementierungsaufgaben.
- `documented` (`AGENTS.md` §8, `docs/AGENT_WORKFLOW.md`, XB-AUTONOMY-ACTIVATION-01): Der **Coordinator** darf nach Tests und unabhängigem Review committen, pushen, mergen und GitHub-PRs erstellen/aktualisieren; Subagenten veröffentlichen nicht. Die ältere Aussage zur ausschließlichen Owner-Publikation ist **historisch** und durch die aktuelle Freigabe superseded. Harte Denys für Historie-Umschreibung, Verwerfen, destruktive Befehle und globale Git-Konfiguration bleiben unverändert. Tatsächlich ausgeführt wurde in diesem Paket kein Publish-Schritt.
- `documented` (Abschnitt `XB-0086-C2 — final status alignment and publication group 0 definition` in `docs/AGENT_TASKS.md`): Publikationsgruppe 0 besteht aus genau `docs/AGENT_WORKFLOW.md`, `docs/RELEASE_CHECKLIST.md`, `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`, `docs/PORTABLE_SETUP.md`; `AGENTS.md` ist wegen einer offenen Versionierungsentscheidung ausgenommen. Die frühere Zeilenangabe `:588` ist durch nachträgliche Ledger-Einträge ungültig geworden und wird nicht mehr verwendet. Dies ist eine Gruppierung, keine Publikationsfreigabe.

`observed` (Arbeitsstand bei Erstellung dieser Datei): Branch `docs/opencode-publication-group-2`, HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`. Nicht publizieren: `.owner-patch-backup/**`, `CMakePresets.json.backup`, `debug-*.txt` (untracked Artefakte; keine pauschale Index-Aufnahme).

`documented`/`unknown`: Ein gesondertes Paket 2 oder eine Publikationsgruppe 2 ist in `docs/AGENT_TASKS.md` nicht definiert; die einzige dort definierte Gruppierung ist Publikationsgruppe 0 (Abschnitt `XB-0086-C2 — final status alignment and publication group 0 definition`). Package 2 bleibt geschlossen und nicht freigegeben; diese Korrekturrunde behandelt ausschließlich die vom Eigentümer freigegebenen P1-Findings.

## 11. Externe SSD und geplanter Einsatz an zwei Rechnern

- `owner-confirmed`: Die externe SSD wird zur gemeinsamen Verwendung an zwei Rechnern genutzt, **jedoch immer nur an einem Rechner gleichzeitig**.
- `documented` (`docs/PORTABLE_SETUP.md`): Ziel ist Öffnen/Bauen/Testen des Projekts von der externen Festplatte auf einem anderen Windows-Rechner ohne festen Laufwerksbuchstaben. `<PROJECT_ROOT>` wird jeweils neu bestimmt; Werkzeuge sind pro Rechner lokal erforderlich, soweit nicht ausdrücklich portabel; `build/**` wird neu erzeugt; bei `detected dubious ownership` wird `safe.directory` **pro Befehl** gesetzt, nie global.
- `documented`/`planned` (`docs/PORTABLE_SETUP.md:10,105`): 13 OpenCode-Sitzungen (3 Projekt + 10 Setup) sollen exportiert/importiert werden; Originale auf `C:` bleiben unangetastet; keine aktive SQLite-Datei wird direkt kopiert. Agents können `opencode session export/import` nicht ausführen.
- `documented`/`blocked` (`docs/AGENT_TASKS.md:17,106`): Worktree-Öffnen auf `E:` meldete `Created worktree could not be opened`; ein vollständiger, aber nicht registrierter Checkout liegt unter `E:\XBridge 1.0\worktrees\portability-worktree-test` – Bereinigung nur mit Owner-Entscheidung.
- `unknown`: Ein erfolgreicher Test am zweiten Rechner liegt **nicht** vor; die Funktionstüchtigkeit des zweiten PCs darf nicht als bestätigt dargestellt werden.

## 12. Bekannte Einschränkungen und offene technische Punkte

| Punkt | Status |
|---|---|
| Aktueller Stand ist Scaffold mit synthetischer Test-CPU, **keine** Xbox-Emulation | `observed` / `documented` |
| Xbox Series S außerhalb des Umfangs; JIT ohne eigene Entscheidung ausgeschlossen | `documented` |
| Zweiter Rechner / vollständige Portabilität | `unknown` – kein bestätigter Test |
| Übertragung der 13 OpenCode-Sitzungen | `planned` / `unknown` – kein Nachweis |
| Worktree-Öffnen auf `E:` | `blocked` – unregistrierter Checkout, Owner-Entscheidung offen |
| MSVC-Toolsets `14.44.35207`, `14.52.37025`; SDKs `10.0.26100.0`, `10.0.28000.0` | `observed` (Dateisystem). Compiler `19.52.37029` und Ninja `1.13.2` `owner-provided`, nicht agentenverifiziert |
| Host-CPU Ryzen 7 5700X, RAM 16 GB, GPU RTX 2060 SUPER | `owner-provided`, nicht agentenverifiziert (zuvor `unknown`) |
| GitHub Desktop, OpenCode Desktop | `observed` installiert (`app-3.7.0` bzw. Desktop-Anwendung); konkrete OpenCode-Laufzeitversion `unknown`. Interaktiver TUI-`ask`-Dialog weiterhin `unknown` |
| `opencode debug config` / `debug agents` im Agentenkontext | `blocked` (`shell "opencode *" deny`) – Owner-Prüfung außerhalb erforderlich |
| `git status --short --branch` für Coordinator | `observed` – erlaubt seit XB-AUTONOMY-ACTIVATION-01, `EXIT=0` |
| Konkrete Modellnamen, MCP-Server / -Authentifizierungen zur Laufzeit | `unknown` – kein `mcp`-Abschnitt in `opencode.jsonc` |
| `debug-agents-full.txt` | `observed`: historischer Vor-Patch-Snapshot – nicht als aktuelle Verifikation verwenden |
| `debug-config-check.txt`, `debug-agents-check.txt` | `unknown` – als Binärdateien nicht als Text interpretiert |
| Plain `cmake`/`ctest` im Agentenkontext | `observed` über einfache Befehlsnamen durch `tester` (`4.3.1-msvc1`); Exit-Codes `unknown`. Frühere `CommandNotFoundException` (`docs/AGENT_TASKS.md:92`) ist **historisch / superseded** |
| `git diff --check` | `observed`: `EXIT=0` in XB-AUTONOMY-ACTIVATION-01; historische CRLF-Warnung für `CMakePresets.json` nicht blockierend |
| Build/CTest in dieser Aufnahme | **nicht ausgeführt** – historische Ergebnisse werden nicht als neue Prüfung ausgegeben |

## 13. Voraussetzungen für die spätere Xbox-Series-X-Emulatorentwicklung

`documented` (`AGENTS.md` §1/§2/§5/§6, `docs/ROADMAP.md:43-61`), allesamt **geplant, nicht bereits vorhanden**:

- C++20-Toolchain und Windows/Visual-Studio-Kompatibilität sind als Basis vorhanden (`observed`); weitere Arbeit bleibt auf dieser Grundlage.
- Interpreter als sichere CPU-Ausführungsbasis erhalten; Dekoder, CPU-Zustand, Interpreter und Speicher bleiben getrennt.
- Echte Host-Fähigkeitserkennung getrennt von Gast-Semantik; dynamische Host-RAM-/VRAM-Verwaltung, keine Reservierung des gesamten RAM.
- Jede neue Verhaltenweise mit Tests, inklusive Grenz- und Fehlerfällen; Build (VS2026 Debug), CTest, `git diff --check` und vollständige Diff-Inspektion vor Abschluss.
- Architektur-, Interface- oder ADR-Änderungen sowie neue Abhängigkeiten benötigen eine eigene Owner-Entscheidung; JIT nur mit separater Evidenz, Design und Tests.
- Keine proprietäre Firmware, Keys, Spieldateien oder geschützten Inhalte; keine DRM-Umgehung; keine fremden Quellen ohne geprüfte Lizenz.
- Erforderliche Spezialrollen (insb. `cpu-developer`, `memory-developer`, `test-engineer`) sind derzeit `planned`/nicht installiert – ihre Installation erfordert eine eigene, begrenzte Owner-Freigabe für `.opencode/agents/**`.
- Prioritätenreihenfolge: Korrektheit → Stabilität → konsistente Frame-Zeiten → Performance → Grafikqualität.

Diese Aufzählung beschreibt **Voraussetzungen**, nicht vorhandene Emulatorfunktionen. Es werden keine Kompatibilitäts- oder Spielbarkeitsaussagen getroffen.
