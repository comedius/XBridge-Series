# Agent autonomy and decision boundaries

Dieses Dokument gehört zur **Entwicklungsinfrastruktur** und ist **nicht Teil des Produktpakets** (`AGENTS.md` §10). Es präzisiert `docs/AGENT_WORKFLOW.md`; bei Konflikten gilt dessen Autoritätsordnung: `AGENTS.md` → `docs/DECISIONS.md` (akzeptierte ADRs) → `docs/ARCHITECTURE.md` und weitere Projekt-/Architekturdokumentation → `docs/AGENT_WORKFLOW.md` → Agent-Dateien unter `.opencode/agents/`.

Stand: 2026-11-24. Freigegeben als Teil von `XB-P2`. Evidenzlabels: `documented` (freigegebene Regel oder vorhandene Dokumentation), `observed` (lokal geprüfter Ist-Zustand), `unknown`/`hypothesis` (nicht verifiziert).

## 1. Grundsatz

`documented` — Autonomie ist **aufgabenbezogen**, nicht personenbezogen. Sie wird ausschließlich durch den freigegebenen Paket-Envelope und die wirksamen Berechtigungen bestimmt, nie aus einem Rollen Namen abgeleitet. Ein Agent darf genau das tun, was sein Paket ihm ausdrücklich erlaubt; alles andere ist entweder `OWNER_DECISION` oder gestoppt.

`documented` — Ziel ist maximale Eigenständigkeit innerhalb klarer Grenzen: einmal vollständig vorbereiten, direkt umsetzen, einmal gebündelt prüfen, gezielt korrigieren. Autonomie erhöht nicht das Risiko, sondern verringert unnötige Rückfragen und unnötige Prüfschleifen.

## 2. Autonomiegrade

| Grad | Bezeichnung | Erlaubt | Nicht erlaubt | Typisches Beispiel |
|---|---|---|---|---|
| A0 | `read-only` | `read`, `glob`, `grep`, erlaubte Git-Leseoperationen | jede Dateiänderung | Architect, Reviewer, Vorprüfung |
| A1 | `bounded-doc` | Schreiben **nur** auf namentlich genannte, berechtigte `docs/**`- oder konfigurierte Projektdateien | Produktcode, nicht zugewiesene Pfade | Coordinator bei Dokumentations-/Konfigurationspaketen |
| A2 | `package-write` | Schreiben auf die im Envelope genannten `src/**`, `include/**`, `tests/**`-Pfade | fremde Module, nicht zugewiesene Tests | Developer im freigegebenen Implementierungspaket |
| A3 | `verify-correct` | Ausführen der erlaubten Prüfungen; gebündelte Korrektur im eigenen Writersatz | Abschwächen von Tests, Ausweiten des Writer-Satzes | Tester, gezielte Korrektur durch zuständigen Writer |
| A4 | `owner-only` | — (ausschließlich Owner) | eigenmächtige Rechteerweiterung, ADR-/Lizenzentscheidung, Produktfreigabe, destruktive Aktionen | neue Rollen/Rechte, ADRs, `LICENSE`, `FINAL_CLEANUP` |
| A5 | `bounded-publication` | exakt konfigurierte Git/GitHub-Kommandos nach Review und Diff-Prüfung | Subagenten-Publikation, History-Rewrite, ungeprüfte Änderungen | Coordinator-Commit/Push/Merge/PR |

`documented` — Der Grad wird pro Aufgabe im Envelope festgelegt (Felder `FILES`, `ALLOWED`, `FORBIDDEN`). Ein Wechsel zu einem höheren Grad benötigt eine neue Owner-Freigabe. `observed` — der Coordinator besitzt A1 für die namentlich freigegebenen Dokument- und Konfigurationspfade (`coordinator.md:8-61`, einschließlich `opencode.jsonc` und `.opencode/**`) sowie A5 für exakt konfigurierte Git/GitHub-Kommandos nach Review; `AGENTS.md`, `docs/DECISIONS.md` und Produktcode bleiben für ihn deny.

## 3. Verantwortlichkeiten der installierten Rollen

`documented` (`docs/AGENT_WORKFLOW.md`), `observed` (Agenten-Frontmatter):

| Rolle | Verantwortung | Schreibgrad | Darf nicht |
|---|---|---|---|
| `coordinator` | Envelope, Planung, Writer-Zuordnung, Ergebnis-Konsolidierung, Ledger/Status, koordinierte Publikation | A1/A5, paketabhängig | Produktcode schreiben, Rechte ohne Freigabe erweitern, Subagenten-Duplicate starten |
| `architect` | Architektur-, Interface-, ADR-, Abhängigkeits- und Risikoprüfung | A0 | entscheiden, schreiben, implementieren |
| `developer` | Implementierung freigegebener Pakete | A2, nur zugewiesene Pfade | fremde Module, Konfiguration, unzugewiesene Tests |
| `tester` | unabhängige Verifikation, Build/CTest/Diff | A3, `tests/**` | Produktcode ändern, Tests abschwächen |
| `reviewer` | gebündelte unabhängige Abschlussprüfung | A0 | schreiben, Shell, Web, Subagenten, Owner direkt fragen |
| `researcher` | evidenzgelabelte öffentliche Recherche | A1 auf `docs/RESEARCH_REGISTER.md`, `docs/research/**` | Forschungsergebnisse als Implementierung ausgeben |

`documented` — Neun Spezialrollen (`cpu-developer`, `memory-developer`, `gpu-developer`, `audio-developer`, `input-developer`, `platform-developer`, `test-engineer`, `performance-engineer`, `integration-engineer`) sind **`planned`, nicht installiert**. `test-engineer` darf niemals als installiert gemeldet werden; bis zu einer Owner-Installation gelten die sechs installierten Substitute.

## 4. Owner-Gates

`documented` — Folgende Entscheidungen sind ausschließlich dem Owner vorbehalten und werden von keinem Agenten eigenmächtig getroffen:

1. Entscheidungen, die über bestehende Freigaben hinausgehen (neue Strategie, ADR, Lizenz, Produktfreigabe).
2. Änderungen an `AGENTS.md`, `docs/DECISIONS.md`, `LICENSE` und Agentenberechtigungen; `opencode.jsonc`/`.opencode/**` bleiben im projektautorisierten Coordinator-Rahmen korrigierbar.
3. Installation oder Rechteänderung von Agenten; neue Abhängigkeiten; öffentliche Schnittstellen; Architekturänderungen.
4. Git-/GitHub: Tag, Release, strategische Branch-Entscheidungen und Historie-Umschreibung. Staging, Commit, Push, Merge und PR nimmt der Coordinator nach Review innerhalb der konfigurierten Regeln vor.
5. Destruktive Aktionen: Löschen/Verschieben bestehender Dateien, `git reset`/`clean`, Worktree-Bereinigung.
6. Produktfreigabe, Versions-/Kompatibilitätsaussagen und `FINAL_CLEANUP`.
7. Wahl zwischen widersprüchlichen Anforderungen oder nicht eindeutigen Prioritäten.

`observed` — Die vier globalen Code-Mode-Deny-Regeln wurden vom Owner angewendet; der unabhängige Reviewer erteilte im angewendeten Zustand `PASS` (`ses_df1b60181ffek5mOBp1DHBgV5M`). Ihre **Laufzeitwirkung bleibt `unknown`**. Agenten dürfen sie nicht eigenmächtig ergänzen, aufheben oder umgehen.

## 5. Stop-Bedingungen

`documented` — Die Arbeit wird sofort gestoppt und der Owner informiert, wenn einer der folgenden Punkte eintritt:

- Branch, HEAD oder Projektpfad weicht vom freigegebenen Ausgangszustand ab; vorhandene fremde Änderungen würden überschrieben.
- Die Auftragslage ist widersprüchlich oder eine notwendige Owner-Entscheidung fehlt.
- Die wirksamen Schreibrechte für einen Zielpfad sind verweigert oder nicht sicher bestimmbar. Es wird keine Umgehung versucht.
- Eine Prüfung schlägt fehl und die Ursache ist nicht eindeutig dem zugewiesenen Paket zuordenbar.
- Ein Review-Finding betrifft einen Bereich außerhalb des freigegebenen Writer-Satzes.
- Ein Werkzeug, ein Backup-Ort oder eine externe Ressource ist nicht verfügbar und ein Ersatz würde die Grenzen überschreiten.
- Es werden proprietäre Inhalte, Keys, geschützte Assets oder Lizenzunsicherheiten festgestellt.
- Eine Sicherheitsaussage soll getroffen werden, für die nur `hypothesis` oder `unknown` vorliegt.

`documented` — Ein Stopp ist kein Scheitern des Pakets: Ursache, betroffener Pfad und benötigte Owner-Entscheidung werden dokumentiert; der Zustand bleibt manipulierbar und wiederaufnehmbar.

## 6. Umgang mit Unsicherheiten und Konflikten

- `documented` — Jede technische Aussage trägt einen der Labels `documented`, `observed`, `hypothesis`, `unknown`. `hypothesis` und `unknown` werden nie als Implementierungs- oder Prüfergebnis ausgegeben.
- `documented` — Bei Quellenkonflikten gilt die Autoritätsordnung; wird sie dadurch nicht aufgelöst, stoppt der Agent und übergibt die konkrete Entscheidung dem Owner. Kein stilles Wählen.
- `documented` — Fehlende Daten werden nicht erfunden. Nicht prüfbare Punkte werden als `UNKNOWN` separat von echten Befunden gelistet.
- `documented` — Ein Review-Finding wird klassifiziert als `ACTIONABLE`, `OWNER_DECISION`, `NON_BLOCKING` oder `UNKNOWN`. Nur `ACTIONABLE` löst eine Korrektur aus; `OWNER_DECISION` wird gebündelt an den Owner gegeben.
- `documented` — Bestehende, bereits verifizierte Informationen werden wiederverwendet; erneute Bestandsaufnahmen benötigen einen konkreten Anlass.

## 7. Autonomie im Phasenmodell (A–E)

| Phase | Autonomie | Ergebnis |
|---|---|---|
| A — Einmalige Vorbereitung | A0/A1 | vollständiger Envelope, bekannte Fakten wiederverwendet, Blocker sichtbar |
| B — Direkte Umsetzung | A1/A2/A3, disjunkte Writer | Änderungen ohne Zwischenberichte, parallel nur bei getrennten Pfaden |
| C — Gebündelte Abschlussprüfung | A0 (Reviewer) | ein Verdict über den Gesamtumfang mit Datei:Zeile-Belegen |
| D — Gezielte Korrektur | A1/A2/A3 | nur betroffene Bereiche und direkte Auswirkungen erneut geprüft |
| E — Abschluss | A1/A5 | kompakter Bericht, offene Punkte, Empfehlung; Coordinator publiziert geprüften Stand, Subagenten nicht |

`documented` — Der Übergang von B nach C erfolgt ohne separate Zwischenreviews pro Datei. Ein erfolgreicher Build ersetzt nie die funktionale und sicherheitliche Prüfung.

## 8. Effizienzregeln

- Eine Datei = ein verantwortlicher Writer pro Package; Shared Files sind coordinator-serialisiert.
- Keine unnötigen Zwischenberichte; handoff payload enthält Package-ID, Writer-Satz, geänderte Dateien, exakte Kommandos/Ergebnisse, bewusste Auslassungen, Evidenzlabels und offene Fragen.
- Höchstens zwei gebündelte Korrekturrunden; danach Eskalation an den Owner.
- Kein vollständiger Review-Zyklus für unveränderte, bereits bestandene Bereiche.
- Keine Rechteerweiterung „für grüne Tests"; Shell-Pattern sind keine OS-Sandbox.

## 9. Bekannte Einschränkungen

- `unknown` — Laufzeitverhalten der vier Code-Mode-Denys; JSONC-Parser-Validierung im Agentenkontext; interaktive TUI-`ask`-Dialoge; zweiter Rechner.
- `documented` — Worktree-Erzeugung/-Öffnen ist blockiert (`Created worktree could not be opened`); Bereinigung nur mit Owner-Entscheidung.
- `documented` — Agent Core ist Architektur- und Dokumentationsgrundlage, kein neues Laufzeit-Framework (`docs/AGENT_CORE_ARCHITECTURE.md`).
