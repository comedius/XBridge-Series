# Automatisierte Qualitäts-Gates und Abschlussprüfung

Dieses Dokument gehört zur **Entwicklungsinfrastruktur** und ist **nicht Teil des Produktpakets** (`AGENTS.md` §10). Es präzisiert die Qualitäts- und Reviewregeln von `docs/AGENT_WORKFLOW.md` und `AGENTS.md`. Bei Konflikten gilt die Autoritätsordnung des Workflow-Dokuments.

Stand: 2026-11-24. Freigegeben als Teil von `XB-P2`. Evidenzlabels: `documented` (verbindliche Regel), `observed` (reales Ergebnis), `unknown` (nicht verifiziert).

## 1. Grundsatz

`documented` — Qualität wird nicht zugunsten von Geschwindigkeit reduziert. Ein erfolgreicher Build ist **allein kein Nachweis funktionaler Korrektheit**. Jede Aussage über Ergebnisse stützt sich auf exakte Kommandos und tatsächliche Resultate; historische Ergebnisse werden als historisch gekennzeichnet.

`documented` — Es gibt **eine** gebündelte unabhängige Abschlussprüfung pro Paket, keine Zwischenreviews pro Datei. Nach konkreten Findings wird nur der betroffene Bereich und dessen direkte Auswirkungen erneut geprüft.

## 2. Qualitätsbereiche der Abschlussprüfung

| Nr. | Bereich | Prüffrage | Mindestevidence |
|---|---|---|---|
| 1 | Vollständigkeit | Deckt das Ergebnis den ursprünglichen Auftrag? | Envelope-Abgleich mit Datei:Zeile |
| 2 | Technische/funktionale Korrektheit | Tut das Ergebnis, was spezifiziert ist? | Tests, Laufzeit- oder Lesenachweis; kein Build allein |
| 3 | Architektur/Schnittstellen | Konsistent mit `docs/ARCHITECTURE.md`, ADRs, Modulgrenzen? | `architect`-Beleg oder Begründung des Entfalls |
| 4 | Codequalität/Wartbarkeit/Modularität | Klare Verantwortlichkeiten, keine versteckte Kopplung? | Diff-Inspektion, Datei:Zeile |
| 5 | Stabilität/Regressionen | Werden bestehende Verhalten unverändert gelassen? | CTest/Regression oder Begründung „nicht relevant" |
| 6 | Sicherheit/Berechtigungen | Rechte eingehalten, keine Umgehung, keine Produkt-/Infrastrukturvermischung? | Rechteprüfung, Produktgrenze, Hard-Denys |
| 7 | Dokumentation/Projektstruktur | Konsistent mit bestehender Dokumentation, keine Doppelregeln? | Querverweise, Widerspruchsfreiheit |
| 8 | Build und Tests | Nur bei relevanter Änderung ausgeführt? | exakte Kommandos und echte Exit-Ergebnisse |

`documented` — Der Reviewer bewertet den Gesamtumfang in einem Verdict: `PASS`, `NEEDS_CHANGE` oder `BLOCKED` (`docs/AGENT_WORKFLOW.md:217`, `reviewer.md:49`). Historische Ledger-Einträge mit `PASS WITH FINDINGS`/`FAIL` bleiben als vergangene Aufzeichnungen erhalten, definieren aber kein neues Review-Vokabular. Jeder Befund enthält ID, Schweregrad, Datei:Zeile, Evidenzlabel, technische Wirkung, Korrekturanforderung mit Erfolgskriterium und Klassifikation.

## 3. Build- und Testanforderungen

`documented` (`AGENTS.md` §6) — Für Implementierungspakete: neue Verhalten mit Tests, Grenz- und Fehlerfälle, bestehende Tests erhalten, VS2026-Debug-Build, CTest, `git diff --check`, vollständige Diff-Inspektion, Protokoll aller Kommandos und Ergebnisse.

`observed` — Dokumentierte Kommandos:

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug --config Debug
ctest --test-dir build/windows-debug -C Debug --output-on-failure
git diff --check
```

`documented` — Für reine Dokumentations-/Konfigurationspakete: **kein** Build und **keine** künstlichen Tests. Prüfung erfolgt über Leserückprüfung, `git status`, `git diff --check` und Review. `observed` — Die bekannte CRLF-Warnung für `CMakePresets.json` ist vorbestehend und separat zu behandeln; der Exit-Code ist in der aktuellen Prüfgrundlage `unknown`.

`documented` — Fehlgeschlagene Kommandos werden nicht als „bestanden" gemeldet. Bei Fehler: exakten Fehler dokumentieren, nur relevante Ursache korrigieren, betroffene Prüfung erneut ausführen.

## 4. Unabhängige Abschlussprüfung

`documented` — Der Reviewer ist read-only: `edit:* deny`, Shell/Web/Subagent/Question verweigert (`reviewer.md:4-31`). Er verwendet ausschließlich `read`, `glob`, `grep` und erhält Git-Ergebnisse vom Coordinator. Er darf keine Datei ändern, keine Subagenten starten und den Owner nicht direkt fragen.

`documented` — Der Auftrag nennt den vollständigen Envelope, den Writer-Satz, die geänderten Dateien, die exakten Kommandos und Ergebnisse sowie die bewussten Auslassungen. Ohne diese Evidenz ist der Review nicht belastbar; fehlende Prüfbarkeit wird als `UNKNOWN` gemeldet, nicht als Patch-Mangel.

`documented` — Für die Architekturprüfung wird der `architect` nur bei Architektur-, Interface-, Modul-, Abhängigkeits- oder Designfragen eingesetzt. Bei reiner Dokumentationspflege kann er entfallen; der Entfall wird im Bericht begründet.

## 5. Regressionen

- `documented` — Bestehende Tests, Schnittstellen und Dokumentationsaussagen dürfen nicht verschlechtert oder still geändert werden.
- `documented` — Änderungen an `TestCpu`, `GuestMemory` oder `ExecutionPolicy` nur bei ausdrücklicher Paketfreigabe.
- `documented` — Bei Dokumentationspaketen bedeutet Regression: widersprüchliche Querverweise, veraltete Zustandsangaben oder doppelte Regeln. Diese sind wie technische Regressionsbefunde zu behandeln.
- `observed` — Vorbestehende Arbeitsbaumänderungen anderer Pakete sind keine Regressionen des aktuellen Pakets; sie werden als Kontext protokolliert.

## 6. Findings und gezielte Korrektur

`documented` — Nach jedem Review-Ergebnis:

1. Jeden Befund klassifizieren: `ACTIONABLE`, `OWNER_DECISION`, `NON_BLOCKING`, `UNKNOWN`.
2. **Case A:** kein `ACTIONABLE` und keine Korrektur → kein neues Review; bestehendes Verdict behalten; `OWNER_DECISION` und `UNKNOWN` separat listen.
3. **Case B:** mindestens ein `ACTIONABLE` → nach Root Cause gruppieren, Duplikate zusammenführen, **ein** gebündeltes Korrekturpaket pro Writer, dann zwingend unabhängige Nachprüfung.
4. Maximal zwei gebündelte Korrekturrunden; danach `BLOCKED` mit Eskalation und gesammelten Owner-Entscheidungen.
5. Kein Severity-Downgrade, um ein `PASS` zu erreichen. Kein Review-Finding an den Reviewer zurückgeben; Änderungen laufen immer über einen Writer oder einen vollständigen Owner-Patch.

`documented` — Die Nachprüfung beschränkt sich auf die korrigierten Bereiche und ihre direkten Auswirkungen; bereits bestandene, unveränderte Bereiche werden nicht erneut geprüft.

## 7. Abnahmekriterien

Ein Paket darf nur als lokal abgeschlossen gemeldet werden, wenn:

- alle Envelope-Punkte erfüllt oder als `OWNER_DECISION` ausgewiesen sind;
- der Reviewer `PASS` ohne offene `ACTIONABLE`-Befunde erteilt hat (`NEEDS_CHANGE`/`BLOCKED` bleiben offen);
- alle relevanten Prüfungen tatsächlich ausgeführt und mit echten Ergebnissen protokolliert sind;
- `git diff --check` ausgeführt und das Ergebnis documented ist;
- keine Datei außerhalb des freigegebenen Writer-Satzes durch dieses Paket geändert wurde;
- offene `UNKNOWN`-Punkte separat und ohne Erfolgsbehauptung gelistet sind;
- keine Veröffentlichung durch Agenten erfolgte.

`documented` — Statuswerte im Ledger: `planned`, `in_progress`, `needs_owner`, `pass`, `blocked`, `superseded`. Ein `pass` ohne protokollierte Kommandos und Ergebnisse ist unzulässig.

## 8. Was nicht als Nachweis gilt

- Ein Build ohne CTest oder funktionale Prüfung.
- Historische Exit-Codes ohne erneute Ausführung (`historisch`, nicht `observed`).
- Meldungen von Spezialrollen, die nicht installiert sind.
- `hypothesis` oder `unknown` als Implementierungsaussage.
- Shell-Pattern als OS-Sandbox; sie schützen nur das Agentenverhalten.
- Eine Rechteerweiterung „nur für die Prüfung".

## 9. Spezialfall Dokumentationspakete

`documented` — Für `XB-P2` gilt: sechs neue Dokumente, Coordinator als alleiniger Writer, Architect nur für Architekturkonsistenz, eine gebündelte Review-Prüfung, keine Produkt-/Test-/CMake-/Konfigurationsänderung, kein Build und kein CTest. Abnahme erfolgt über Vollständigkeit, Widerspruchsfreiheit, Autoritätsordnung, Produktgrenze und Review-Verdict.
