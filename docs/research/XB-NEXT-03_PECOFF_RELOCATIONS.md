# XB-NEXT-DEVELOPMENT-03-R0 — PE32+/AMD64 headers and base relocations

Development infrastructure; not part of a product package. Access date for every URL and local file read in this document: **2026-11-25**. Target: **Xbox Series X only**. This document is public-source evidence for a future strict bounds checker/loader; it does not implement a loader, read a game file, decrypt content or patch binaries.

All external text was treated as untrusted data. Evidence labels follow `docs/RESEARCH_REGISTER.md` Sections 2–6. The Microsoft PE/COFF facts here are **general Windows PE/COFF facts**, not Xbox Series X behavior.

## 1. Source inventory

| ID | Exact URL / local path | Class | Precise section or excerpt used | Reproduction actually performed |
|---|---|---|---|---|
| S0 | `docs/AGENT_TASKS.md:3649-3660` | project-authored | Package `XB-NEXT-DEVELOPMENT-03-R0`; no implementation, proprietary material or unsupported Xbox claim authorized | Local file read, 2026-11-25 |
| S1 | `https://learn.microsoft.com/en-us/windows/win32/debug/pe-format` | authoritative (general PE/COFF only) | Canonical Microsoft PE Format specification; page metadata `ms.date: 2026-09-10` | `webfetch` and direct `fetch`, 2026-11-25 |
| S2 | `https://raw.githubusercontent.com/MicrosoftDocs/win32/docs/desktop-src/Debug/pe-format.md` | authoritative (general PE/COFF only) | `COFF File Header`, `Optional Header`, `Optional Header Data Directories`, `Section Table`, `The .reloc Section (Image Only)`, `Base Relocation Block`, `Base Relocation Types` | Direct `fetch`, 2026-11-25; searched exact anchors below |
| S3 | exact local path `docs/research/XB-NEXT-02_GAME_INTELLIGENCE_FORMATS.md:29-87` | project-authored | Prior PE/COFF field and strict-bounds evidence; no Xbox-specific file-format claim | Local file read, 2026-11-25 |
| S4 | `docs/RESEARCH_REGISTER.md:331-355` | project-authored | Prior `R-XB-NEXT-02-GI` row; capability status `unknown` and no implementation claim | Local file read, 2026-11-25 |

`observed` — S2 returned the quoted content when fetched on 2026-11-25. `unknown` — external truth after that date and any authenticated GDK material were not re-checked.

## 2. PE32+/AMD64 header fields in scope

All rows in this table are `documented (authoritative, general PE/COFF only)` S1/S2. Values are field facts; they are not proof that any Xbox Series X file is readable or executable.

| # | Field | Location / size | Documented meaning or check |
|---|---|---|---|
| H01 | `e_magic` / `e_lfanew` | DOS header; signature offset at file offset `0x3c` | `MZ`/`0x5A4D` and file offset of the PE signature (prior corroboration S3). |
| H02 | PE signature | at `e_lfanew` | `PE\0\0` begins the NT headers. |
| H03 | `Machine` | COFF header offset 0, 2 bytes | `IMAGE_FILE_MACHINE_AMD64 = 0x8664` means `x64`. This is an architecture field, not an Xbox compatibility verdict. |
| H04 | `NumberOfSections` | COFF header offset 2, 2 bytes | Number of section-table entries; the specification notes a Windows loader limit of 96 sections. |
| H05 | `SizeOfOptionalHeader` | COFF header offset 16, 2 bytes | Must bound all optional-header and data-directory reads. |
| H06 | `Characteristics` | COFF header offset 18, 2 bytes | Includes flags such as `RELOCS_STRIPPED 0x0001`, `EXECUTABLE_IMAGE 0x0002`, `32BIT_MACHINE 0x0100`, `DLL 0x2000`. |
| H07 | Optional `Magic` | optional-header offset 0, 2 bytes | `0x20b` = PE32+; `0x10b` = PE32; `0x107` = ROM image. PE32+ allows a 64-bit address space while limiting image size to 2 GB. |
| H08 | `AddressOfEntryPoint` | optional-header offset 16, 4 bytes | RVA from image base; zero is valid when no entry point is present. |
| H09 | `ImageBase` | PE32+ optional-header offset 24, 8 bytes | Preferred base address; the specification describes it as a multiple of 64 K. |
| H10 | `SectionAlignment` | optional-header offset 32, 4 bytes | Alignment of sections in memory; must be greater than or equal to `FileAlignment`; default is architecture page size. |
| H11 | `FileAlignment` | optional-header offset 36, 4 bytes | Raw-data alignment; documented as a power of 2 between 512 and 64 KiB inclusive; if `SectionAlignment` is below architecture page size, `FileAlignment` must equal `SectionAlignment`. |
| H12 | `SizeOfImage` / `SizeOfHeaders` | optional-header offsets 56/60, 4 bytes | Bounds for the in-memory image and headers. |
| H13 | `Subsystem` | optional-header offset 68, 2 bytes | `IMAGE_SUBSYSTEM_XBOX = 14` is a defined field value; this package does not claim retail Series X use. |
| H14 | `DllCharacteristics` | optional-header offset 70, 2 bytes | Includes `HIGH_ENTROPY_VA 0x0020`, `DYNAMIC_BASE 0x0040`, `NX_COMPAT 0x0100`, `NO_SEH 0x0400`, `APPCONTAINER 0x1000`, `GUARD_CF 0x4000`. |
| H15 | `NumberOfRvaAndSizes` | PE32+ optional-header offset 108, 4 bytes | Number of data-directory entries; must bound lookup of directory index 5. |
| H16 | Data directories | PE32+ optional-header offset 112; each entry 8 bytes | `VirtualAddress` is an RVA and `Size` is bytes. Base Relocation Table is directory index **5**. |
| H17 | Section table | after optional header using `SizeOfOptionalHeader`; each entry 40 bytes | `Name` (8), `VirtualSize`, `VirtualAddress`, `SizeOfRawData`, `PointerToRawData`, pointer/count fields and `Characteristics`. |

`hypothesis` (engineering inference from S1/S2/S3): a future reader should first validate the DOS/PE extent, then the full COFF + optional header, then `NumberOfSections × 40`, then every directory and section extent with overflow-safe arithmetic. No reader was implemented or run.

## 3. Base relocation structure

### 3.1 Documented layout

`documented (authoritative, general PE/COFF only)` S1/S2:

- The Base Relocation Table field in the optional-header data directories gives the number of bytes in the table.
- The table is divided into blocks; each block represents one 4 KiB page and must start on a 32-bit boundary.
- Each block starts with `Page RVA` (4 bytes) and `Block Size` (4 bytes). `Block Size` includes those fields and all following Type/Offset entries.
- Each following entry is a 16-bit WORD: the high 4 bits are `Type`, the low 12 bits are an offset from the block's `Page RVA`.
- The loader computes the target VA from the image base plus `Page RVA` plus the entry offset and applies the difference between preferred and actual base.
- `ABSOLUTE (0)` means the relocation is skipped and may be used as padding.
- `DIR64 (10)` applies the difference to the 64-bit field at the offset.
- `HIGHADJ (4)` occupies two slots; it is outside the defined handling scope in this document.

### 3.2 In-scope handling boundary

| Type | Value | In scope for a future strict loader? | Required behavior if encountered |
|---|---:|---|---|
| `IMAGE_REL_BASED_ABSOLUTE` | 0 | Yes | Skip the entry; no write, no target bounds failure caused by the padding entry itself. |
| `IMAGE_REL_BASED_DIR64` | 10 | Yes | Validate that the computed 8-byte target lies entirely in a writable mapped extent; apply the base delta with overflow-safe arithmetic. This is a requirement candidate, not implemented behavior. |
| `IMAGE_REL_BASED_HIGH` | 1 | No | Unsupported: report explicitly; do not silently reinterpret as `LOW`, `DIR64` or a no-op. |
| `IMAGE_REL_BASED_LOW` | 2 | No | Unsupported: report explicitly; do not silently correct. |
| `IMAGE_REL_BASED_HIGHLOW` | 3 | No | Unsupported: report explicitly; do not treat as a 64-bit fixup. |
| `IMAGE_REL_BASED_HIGHADJ` | 4 | No | Unsupported: report explicitly; it occupies two slots and must not be advanced as one slot. |
| Machine-specific / reserved types | 5–9 and any value not in {0, 10} | No | Unsupported: report the numeric type and block/offset; do not guess machine semantics. |

`documented (authoritative)` S2 supports all type values and descriptions. The in/out-of-scope decision is `documented (project-authored)` S0: the package explicitly limits the future handling scope to `DIR64 (10)` and `ABSOLUTE (0)`. `hypothesis`: unsupported types should produce a typed error with block index and entry index; no error taxonomy was approved and no reader exists.

## 4. Strict checks and malformed-input risks

| # | Rule / risk | Evidence label and source |
|---|---|---|
| C01 | Before directory index 5 is read, check `SizeOfOptionalHeader` covers the PE32+ directory array position (`112 + 5 × 8` bytes from optional-header start) and `NumberOfRvaAndSizes > 5`. | `documented (authoritative)` S2 for the fields; `hypothesis` for the concrete check expression |
| C02 | Validate `SectionAlignment ≥ FileAlignment`; `FileAlignment` power-of-two in `[512, 64 KiB]`; if `SectionAlignment` is below architecture page size, require `FileAlignment == SectionAlignment`. | `documented (authoritative)` S2; no implementation |
| C03 | Do not assume a data-directory RVA points to a section start or that conventional section names exist; map RVAs through the section table with explicit containment. | `documented (authoritative)` S2; prior corroboration S3 |
| C04 | Check `Block Size ≥ 8`, block alignment on a 32-bit boundary, and that the entire block fits inside the relocation directory size; ensure `(Block Size - 8)` is even. | `hypothesis` inference from `documented (authoritative)` S2 block layout |
| C05 | Compute relocation target VA with overflow-safe arithmetic; for `DIR64`, require 8 writable bytes inside a mapped extent. | `hypothesis` engineering requirement; `documented (authoritative)` S2 for the field semantics |
| C06 | `ABSOLUTE` padding must be skipped without silently consuming an adjacent entry incorrectly; `HIGHADJ` must never be treated as a one-slot type. | `documented (authoritative)` S2; scope decision `documented (project-authored)` S0 |
| C07 | Truncated relocation directory, zero directory size with nonzero RVA, overlapping sections, missing null terminators in import/export strings and certificate-directory RVAs are malformed-input risks; certificate entries use a file pointer, not an RVA. | `documented (project-authored)` S3 for prior field facts; `hypothesis` for the risk framing |

## 5. Deliberate boundaries

- `documented (project-authored)` S0/S4: no loader, analyzer, test, product code, ADR or public interface was created; capability status remains `unknown`.
- `unknown`: whether any retail Series X executable has a directly visible PE32+ header or base relocation directory; `documented (project-authored)` S3 records XVC encryption as a packaging workflow but it was not re-fetched by this package.
- `unknown`: whether any real guest needs base relocations in a future native execution worker; no binary corpus was examined.
- `documented (authoritative, general only)` S2: PE is not architecture-specific and the spec is not guaranteed complete; x64 field values are indicators, not compatibility proof.
- No proprietary firmware, key, game file or protected asset was accessed; no decryption or DRM circumvention was attempted.

## 6. Conclusion and reproduction

`documented (authoritative, general PE/COFF only)`: PE32+/AMD64 header fields, section/alignment rules and relocation block layout are explicitly specified by Microsoft. `documented (project-authored)`: only `DIR64 (10)` and `ABSOLUTE (0)` are within the defined scope for future handling in this research package; all other relocation types remain unsupported and must be reported rather than silently corrected. `hypothesis`: the listed strict checks are candidate requirements for a future bounded reader. `unknown`: Xbox-specific file visibility and real-binary relocation requirements.

Reproduction: locally read S0, S3 and S4; `webfetch`/direct `fetch` S1/S2 on 2026-11-25 and search the anchors `COFF File Header`, `SizeOfOptionalHeader`, `NumberOfRvaAndSizes`, `SectionAlignment`, `FileAlignment`, `The .reloc Section (Image Only)`, `Base Relocation Block`, `Base Relocation Types`, `IMAGE_REL_BASED_ABSOLUTE`, `IMAGE_REL_BASED_DIR64`. No build, CTest, file corpus, game run or capability validation was performed.
