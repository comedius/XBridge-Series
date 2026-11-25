# Agent Task Ledger — Archive 03

> Extracted verbatim from the former `docs/AGENT_TASKS.md`, original lines 1521–2224. Historical line references in older records refer to the former monolithic file. Content is preserved; see the index at `docs/AGENT_TASKS.md`.

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

