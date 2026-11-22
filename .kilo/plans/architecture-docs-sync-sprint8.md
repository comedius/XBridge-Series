# Plan: docs/ARCHITECTURE.md an Sprint 8 Part 2 anpassen

**Ziel:** Dokumentation in `docs/ARCHITECTURE.md` an die tatsächliche x64-MOV-Decoder- und Interpreter-Implementierung anpassen. Nur Dokumentation, kein Code.

---

## Abweichung: Dokumentation vs. Code

| Aspekt | Doku (ARCHITECTURE.md Zeilen 93–124) | Tatsächlich implementierter Code |
|--------|--------------------------------------|----------------------------------|
| `X64DecodeStatus` | `{Ok, EmptyInput, UnknownOpcode}` (3 Werte) | `{Ok, EmptyInput, UnknownOpcode, InvalidEncoding, TruncatedInput}` (5 Werte) |
| `DecodedInstruction` Felder | `{status, opcode, length}` (3 Felder) | `{status, opcode, length, operand_size, rex, dest_reg, src_reg, memory_operand, displacement, immediate, immediate_size}` (11 Felder) |
| Unterstützte Opcodes | Nur NOP (0x90) | NOP + MOV (imm, reg-reg, reg-mem, mem-reg) |
| REX-Präfixe | Nicht unterstützt | REX.W, REX.R, REX.B unterstützt |
| Interpreter | Nicht dokumentiert | `execute_instruction()` mit `X64ExecuteStatus { Ok, MemoryAccessError }` |
| Adressierungsformen | Nicht dokumentiert | Einfache Register-Indirekt (mod=00, rm≠100,101); SIB/RIP-rel. → `InvalidEncoding` |

---

## Aufgaben (reihenfolgefrei)

1. **Decoder-Abschnitt ersetzen** (ARCHITECTURE.md Zeilen 93–124):
   - `X64DecodeStatus` auf 5 Werte erweitern (`InvalidEncoding`, `TruncatedInput` hinzufügen)
   - `DecodedInstruction` auf 11 Felder erweitern (alle auflisten)
   - Unterstützte Opcodes: NOP (0x90), MOV imm (`0xB8+reg`), MOV reg-reg (`0x89/0x8b`, mod=11), MOV reg-mem/mem-reg (`0x89/0x8b`, mod=00, rm≠100,101)
   - REX-Präfixe dokumentieren: W (64-bit), R (reg-Erweiterung R8–R15), B (r/m-Erweiterung R8–R15), X (erkannt, nicht verwendet)
   - Fehlerbehandlung: alle 5 Statuswerte ohne Exception
   - `mod=00, rm=100` → `InvalidEncoding` (SIB), `mod=00, rm=101` → `InvalidEncoding` (RIP-relative)
   - 8-bit MOV (`0x88`, `0x8a`) → `UnknownOpcode`

2. **Interpreter-Abschnitt einfügen** (nach Decoder-Abschnitt):
   - `X64ExecuteStatus { Ok, MemoryAccessError }`
   - `execute_instruction(DecodedInstruction, X64CpuState&, GuestMemory&) noexcept`
   - NOP: RIP um `instruction.length` vorantreiben
   - MOV imm: Register setzen (32-bit zero-extends auf 64-bit)
   - MOV reg-reg: Quellregister in Zielregister kopieren
   - MOV reg-mem/mem-reg: Little-Endian Lade-/Speicherzugriff
   - Bereichsprüfung vor jedem Speicherzugriff (`is_address_range_valid`)
   - `MemoryAccessError` bei Out-of-Bounds, keine Zustandsänderung
   - Nicht-ausführbare Dekodierergebnisse → No-op

3. **Einschränkungen aktualisieren**:
   - "Keine REX-Präfixe" → entfernen (REX wird unterstützt)
   - "Keine weiteren Opcodes neben NOP" → entfernen (MOV unterstützt)
   - "Keine Instruktionsausführung (kein Interpreter...)" → entfernen (Interpreter existiert)
   - Neue Einschränkungen: keine SIB, kein Displacement, keine 8-bit MOV, kein SIMD/FPU, kein JIT, keine Xbox-spezifischen Eigenschaften

4. **Ausblick-Abschnitt** (Zeilen 122–124): Entfernen oder anpassen (REX, MOV und Interpreter sind bereits implementiert)

---

## Explizit ausgeschlossen

- Keine Änderungen an Headern, Quelldateien, Tests oder CMake
- Keine Commits, Pushes oder Merges
- Keine neuen Architektur-Ebenen oder Abhängigkeiten
- TestCpu, GuestMemory, ExecutionPolicy bleiben unverändert
- SIB, RIP-relative Adressierung, Displacement, 8-bit MOV nicht als implementiert darstellen

---

## Validierung

1. `git diff --check` muss sauber sein
2. `git diff` muss zeigen, dass nur `docs/ARCHITECTURE.md` geändert wurde
3. Build und CTest müssen weiterhin bestehen (keine Code-Änderung)
4. Dokumentation muss konsistent mit bestehenden Tests in `tests/test_main.cpp` sein (insbesondere `test_x64_instruction_decoder()` und Interpreter-Tests)

---

## Offene Fragen

Keine. Der Plan basiert auf dem tatsächlichen Quellcode und ist implementierungsbereit.