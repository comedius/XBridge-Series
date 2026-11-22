# Portable Entwicklungsumgebung (zweiter Windows-Rechner)

Dieses Dokument gehört zur **Entwicklungsinfrastruktur** und ist nicht Teil des Produktpakets.

Ziel: Das Projekt von der externen Festplatte auf einem anderen Windows-Rechner öffnen, die Agenten nutzen, neu bauen und testen – ohne Abhängigkeit vom bisherigen Laptop, dessen Windows-Konto oder einem festen Laufwerksbuchstaben.

## 1. Grundsatz

- `<PROJECT_ROOT>` ist immer das Verzeichnis, in dem diese Datei liegt. Der Laufwerksbuchstabe ist egal.
- Projekt- und projektspezifische Entwicklungsdaten, die dauerhaft benötigt werden, liegen unter `<PROJECT_ROOT>`. OpenCode-Sitzungsdaten sind davon getrennt: Sie sollen in einem separaten portablen Kontext außerhalb des eigentlichen Projektverzeichnisses verwaltet werden, nicht unter `<PROJECT_ROOT>`. Vorgesehen sind insgesamt 13 Sitzungen – drei projektspezifische und zehn für die portable Einrichtung. `planned`/`unknown`: Diese Sitzungen sind noch nicht vollständig übertragen oder eingerichtet; ein entsprechender Nachweis liegt nicht vor. Die Originaldaten auf `C:` bleiben unangetastet und sind weder verändert noch gelöscht (siehe Abschnitt 8).
- Werkzeuge (OpenCode, Git, CMake, CTest, Visual Studio, Compiler) sind lokale Installationen und gehören **nicht** ins Projekt. Sie werden über `PATH`, CMake-Erkennung oder die lokale Toolchain gefunden. Komponenten und erforderliche Entwicklungswerkzeuge müssen auf beiden Rechnern jeweils lokal vorhanden sein, soweit sie nicht ausdrücklich portabel sind.
- `build/**` ist generiert und wird auf jedem Rechner neu erzeugt. Build-Artefakte werden niemals zwischen Rechnern kopiert.

## 2. Projekt von der externen Festplatte öffnen

1. Externe Festplatte anschließen und den tatsächlichen Projektpfad notieren (Beispiel `<LAUFWERK>:\...\XBridge-Series-Github`).
2. In diesem Verzeichnis arbeiten. Keine Annahme, dass es `E:` ist.
3. Falls Git wegen `detected dubious ownership` die Arbeit verweigert: `safe.directory` **pro Befehl** setzen, z. B. `git -c safe.directory=<PROJECT_ROOT> status`. Keine globale Git-Konfiguration ändern.
4. Prüfen, dass `.git` intakt ist (`git status --short --branch`). Die Historie wird nicht umgeschrieben.

Hinweis zu Worktrees: `opencode.jsonc` enthält `"worktree": { "directory": "../worktrees" }`. Das ist relativ zum Projekt-Checkout und daher laufwerksunabhängig; neue Worktrees entstehen neben dem Projekt auf demselben Laufwerk. Die Option ist als `Config.Worktree` dokumentiert („Directory defaults for local worktree creation"); die in älteren Fassungen genannte Versionsangabe 2.3.2 stammt aus `docs/AGENT_TASKS.md:13` und wurde in XB-0086 nicht erneut abgefragt (`unknown`). `documented`: Die portabel zu haltenden Projektdaten liegen unter `<PROJECT_ROOT>`; Werkzeuge und OpenCode-Sitzungen sind nur eingeschränkt portabel (siehe Abschnitt 8).

## 3. OpenCode lokal starten

Voraussetzung: OpenCode ist auf dem Zielrechner installiert und über `PATH` erreichbar. Falls nicht, den lokal installierten CLI-Pfad verwenden (hier `<OPENCODE_CLI>` genannt). Kein Projektpfad darf dabei fest verdrahtet werden.

```powershell
Set-Location '<PROJECT_ROOT>'
opencode --version
opencode debug paths
# 'opencode debug config' und 'opencode debug agents' sind nicht in der Allowlist (opencode.jsonc:41-47)
# und daher nur in einer interaktiven TUI-Sitzung zu verwenden.
```

Erwartung:

- `default_agent = coordinator`
- `coordinator` ist `primary`; `architect`, `developer`, `tester`, `reviewer`, `researcher` sind `subagent`.
- Konfigurationsquellen enthalten `<PROJECT_ROOT>\opencode.jsonc` und `<PROJECT_ROOT>\.opencode`. Für automatisierte Prüfungen ist `opencode debug paths` die allowlistete Variante.

Die sechs installierten Rollen, ihre Prompts und die Befehle liegen unter `<PROJECT_ROOT>\.opencode\agents\` und `<PROJECT_ROOT>\.opencode\commands\`. Es gibt keine Abhängigkeit von einem Benutzerprofil oder einem globalen Agentenverzeichnis. Historische Prompt-Dateien unter `<PROJECT_ROOT>\ai\agents\` sind keine installierten OpenCode-Rollen; die neun Spezialisten bleiben `planned`.

## 4. Projektregeln laden

```powershell
Set-Location '<PROJECT_ROOT>'
opencode run --agent coordinator --format json "Quote the first line of AGENTS.md and docs/AGENT_WORKFLOW.md. Do not edit files."
```

Erwartung: Die Antwort stammt aus `<PROJECT_ROOT>`, nicht aus einem alten Laptop-Pfad.

Falls `ask`-Berechtigungen in nicht-interaktiven Läufen automatisch abgelehnt werden: Die TUI interaktiv nutzen oder nur die in `opencode.jsonc` ausdrücklich erlaubten Lese-/Build-Befehle verwenden. Keine Berechtigungen nur für grüne Tests erweitern.

## 5. Voraussetzungen auf dem Zielrechner

- Windows x64 (auf jedem Rechner lokal).
- Git mit HTTPS-Zugriff auf `origin`, falls Klonen/Fetchen nötig ist (lokal installiert).
- `documented` (erster Rechner): Visual Studio 2026 **Community** mit installierter Entwicklungsunterstützung für C++ und C#, Generator `Visual Studio 18 2026`, CMake `4.3.1-msvc1` und CTest. `observed`: `CMakePresets.json:7`, `CMakePresets.json.backup:7` und `build/windows-debug/CMakeCache.txt:222,254` belegen Community-Pfad und Generator; die C++-/C#- und CMake-Versionsangaben stammen aus der vom Owner bestätigten Erst-Rechner-Konfiguration und wurden in diesem dokumentarischen Auftrag nicht erneut abgefragt (`unknown` für eine erneute lokale Prüfung).
- `observed` (lokaler Arbeitsstand): `windows-debug` ist auf `Visual Studio 18 2026` (x64) umgestellt; `ninja-debug` nutzt weiterhin `Ninja` (`CMakePresets.json:14`). `documented`: Beim dokumentierten Vergleichszeitpunkt war der GitHub-Stand `10e5e75` (historischer Vergleichsstand, nicht der aktuelle HEAD) noch auf `Visual Studio 17 2022`; die lokale Umstellung auf VS2026 war damals ein unveröffentlichter Arbeitsstand. Diese Angabe beschreibt ausschließlich den damaligen Vergleich und ist keine Aussage über den heutigen HEAD. `CMakePresets.json.backup` bewahrt die alte Zeile und ist nicht Teil des veröffentlichten Stands.
- `planned`: Auf dem zweiten Rechner müssen dieselben Werkzeuge und Komponenten lokal vorhanden sein, soweit sie nicht ausdrücklich portabel sind. Installation und Konfiguration dort sind noch nicht vollständig verifiziert (`unknown`).
- Schreibrechte auf der externen Festplatte für neu erzeugte Build-Verzeichnisse.

Fehlende Werkzeuge müssen über die Versionsabfragen (`cmake --version`, `ctest --version`, `git --version`, `opencode --version`) erkannt und gemeldet werden. Es gibt keine festen Installationspfade in Projektdateien.

## 6. Frisch bauen und testen

```powershell
Set-Location '<PROJECT_ROOT>'
cmake --preset windows-debug
cmake --build --preset windows-debug --config Debug
ctest --test-dir build/windows-debug -C Debug --output-on-failure
```

Alternativ mit Ninja:

```powershell
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

Regeln und historische Nachweise:

- `observed` (erster Rechner): Die den obigen Preset-Kommandos entsprechenden Schritte `cmake --preset windows-debug`, `cmake --build --preset windows-debug --config Debug` und `ctest --test-dir "build/windows-debug" -C Debug --output-on-failure` wurden in der aufgezeichneten Verifikation mit den in `docs/AGENT_TASKS.md:93-97` verzeichneten absoluten VS2026-Executables ausgeführt und lieferten jeweils Exit `0`; CTest meldete `1/1 Test` und `100% tests passed, 0 tests failed out of 1`.
- `documented`: Diese Nachweise betreffen ausschließlich den ersten Rechner. In XB-0086-C1 wurde kein neuer Build und kein CTest ausgeführt. `unknown`: Ob dieselben Befehle auf dem zweiten Rechner funktionieren; die Toolchain ist dort vor Ort zu prüfen.
- `build/**` wird auf jedem Rechner neu erzeugt. Vorherige Build-Ordner nicht übernehmen.
- Nach dem Konfigurieren prüfen, dass `CMAKE_HOME_DIRECTORY` auf `<PROJECT_ROOT>` zeigt und keine alten Pfade aus `C:/` oder einem anderen Laufwerk enthalten sind.
- Build, CTest und `git diff --check` sind immer mit exakten Kommandos und echten Ergebnissen zu protokollieren.

## 7. Umgang mit der externen Festplatte beim Rechnerwechsel

1. Festplatte sauber trennen, nicht während eines Builds oder Git-Schreibvorgangs.
2. Auf dem anderen Rechner denselben relativen Aufbau verwenden; `<PROJECT_ROOT>` neu bestimmen. `planned`: Gemeinsame Projekte auf der externen SSD; installierte Werkzeuge und nicht ausdrücklich portable Komponenten bleiben auf jedem Rechner lokal erforderlich.
3. Keine persönlichen Zugangsdaten oder API-Schlüssel ins Projekt kopieren. Diese bleiben bewusst lokal (z. B. unter dem Benutzerprofil) und werden vom Projekt nicht benötigt.
4. Keine Build-Artefakte, keine Logs, keine generierten Cache-Dateien mitnehmen.
5. Nach dem Wechsel die Prüfungen aus Abschnitt 3–6 erneut ausführen. `unknown`: Ein bestandener Zweitrechner-Test liegt weiterhin nicht vor.

## 8. Bekannte Einschränkungen / Status

Kennzeichnungen: `observed` = direkt geprüftes Ergebnis; `documented` = bisherige Entscheidung oder belegte Projektangabe; `planned` = vorgesehener Schritt; `blocked` = derzeit nicht erfolgreich durchführbar; `unknown` = nicht überprüft oder nicht nachgewiesen.

- **VS2026 vs. VS2022:** `observed` (lokaler Arbeitsstand): `CMakePresets.json:7` = `Visual Studio 18 2026` (x64), `CMakePresets.json.backup:7` = alter Wert `Visual Studio 17 2022`, `ninja-debug` unverändert (`CMakePresets.json:14`). `documented` (erster Rechner): Visual Studio 2026 Community, C++/C#-Entwicklungsunterstützung, CMake `4.3.1-msvc1`; `build/windows-debug/CMakeCache.txt:222,254` belegt Community-Pfad und Generator. `documented`: Der GitHub-Stand `10e5e75` ist ein historischer Vergleichsstand (nicht der aktuelle HEAD) und enthielt zum dokumentierten Vergleichszeitpunkt noch VS2022, während der lokale Stand bereits VS2026 nutzte; diese Angabe ist keine Aussage über den heutigen HEAD. `blocked`: keine offene Owner-Entscheidung zur Preset-Frage mehr; SETUP-005 ist `pass`. `unknown`: erneute Toolchain-Prüfung auf dem zweiten Rechner und die Versionsangaben aus Sicht der Agents.
- **Build-/Testnachweise:** `observed` (erster Rechner, `docs/AGENT_TASKS.md:93-97`): Configure, Debug-Build und CTest mit den dort verzeichneten absoluten VS2026-Executables liefen mit Exit `0`; CTest `100% tests passed, 0 tests failed out of 1`. `documented`: nur erster Rechner; in XB-0086-C1 kein neuer Build und kein CTest. `unknown`: zweiter Rechner.
- **Zweiter Rechner / Portabilität:** `planned`: gemeinsame Projekte auf der externen SSD und identischer relativer Aufbau. `documented`: Werkzeuge und Komponenten sind pro Rechner lokal erforderlich, soweit nicht ausdrücklich portabel. `blocked`/`unknown`: Installation und Konfiguration auf dem zweiten Rechner sind nicht vollständig verifiziert; eine vollständige Portabilität der Entwicklungsumgebung ist nicht nachgewiesen.
- **OpenCode-Sitzungen:** `documented`/`planned`: 13 vorhandene Sitzungen sollen exportiert und importiert werden – drei Projektsitzungen in den Hauptcheckout, zehn Setup-Sitzungen in einen separaten, dauerhaften Portable-Setup-Kontext. Die Originale auf `C:` bleiben unangetastet; keine aktive SQLite-Datei wird direkt kopiert. `blocked`: Agenten dürfen `opencode *` nicht in der Shell ausführen; `session export/import` steht nicht in der globalen Allowlist und hat kein Code-Mode-Tool. `unknown`: tatsächlicher Erfolg der Übertragung; der geprüfte portable Zustandsordner war leer. Es wird nicht behauptet, dass Sitzungen bereits übertragen wurden.
- **Worktree-Erzeugung und -Öffnung:** `observed`: Die getestete Erzeugung bzw. das Öffnen war nicht erfolgreich; OpenCode meldete `Created worktree could not be opened`. Ein Checkout existiert unter `E:\XBridge 1.0\worktrees\portability-worktree-test`. `documented`: Dieser ist nicht als regulärer Git-Worktree registriert. `blocked`: eine erfolgreiche portable Worktree-Verwaltung ist nicht nachgewiesen. Der vorhandene Checkout wird weder gelöscht noch verändert; die Bereinigung erfordert eine Owner-Entscheidung.
- **Interaktive Berechtigungen:** `unknown`: Ein interaktiver TUI-`ask`-Dialog wurde nicht geprüft; nicht-interaktive Läufe lehnen `ask` ab. `opencode debug config` und `opencode debug agents` sind nicht allowlistet (siehe Abschnitt 3).
- **XB-0085 / Publikationsprüfung:** `documented`: 32 Einträge wurden geprüft – 13 `READY_FOR_COMMIT`, 18 `NEEDS_REVIEW`, 1 `DO_NOT_PUBLISH`. Es erfolgte keine Veröffentlichung, keine Index-/Referenzänderung; PR #1 bleibt offen und unberührt. Dies ist kein Freigabe- oder Implementierungsbeschluss.
- **Shell-Pattern sind keine Sandbox:** `documented`: Die Regeln in `opencode.jsonc` schützen das Agentenverhalten, nicht den Host-Account.

Siehe auch `docs/RELEASE_CHECKLIST.md`, `docs/AGENT_WORKFLOW.md` und `docs/AGENT_TASKS.md`.
