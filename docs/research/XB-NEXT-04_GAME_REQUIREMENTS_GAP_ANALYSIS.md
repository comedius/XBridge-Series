# XB-NEXT-04 — game-installation requirements gap analysis

Development infrastructure; not part of a product package. Target: Xbox Series X only. `observed` — Date of all local reads in this package: 2026-11-26.

## 1. Scope and baseline

- `documented (project-authored)` — Package `XB-NEXT-04` is a documentation-only `RESEARCH` package with write access limited to the two `docs/research/XB-NEXT-04_*.md` files and `docs/RESEARCH_REGISTER.md` (`docs/AGENT_TASKS.md:4089-4103`).
- `documented (project-authored)` — The approved goal is to compare the supplied game-installation reports with current XBridge source, tests and documentation across areas A-G and to record an evidence-labeled, file-referenced gap matrix; no implementation is authorized (`docs/AGENT_TASKS.md:4095-4099`).
- `documented (project-authored)` — Product code, tests, build configuration, JIT, binary rewriting, code patching, arbitrary guest-code execution, real-game execution, proprietary assets, DRM/decryption, new dependencies, license changes, compatibility/playability/performance claims and publication are forbidden (`docs/AGENT_TASKS.md:4098`; `README.md:21-22`).
- `documented (project-authored)` — XBridge is an early foundation/research scaffold and does not currently run Xbox games or Xbox executables (`README.md:5`); its present components are foundations and test models that do not establish Xbox hardware compatibility (`README.md:9-14`).
- `documented (project-authored)` — The reviewed XB-NEXT-DEVELOPMENT-03 foundation records a hybrid direction, fixed compiled-in synthetic native routines, a loader-private bounded PE32+/AMD64 parser for synthetic images, thirteen neutral service ID tokens and evidence-labeled string candidates; it does not establish real-game feasibility (`docs/ARCHITECTURE.md:54-61`).
- `observed` — Direct local reads of `D:\xbox\Spiel Daten\report\report.json` and `D:\xbox\Spiel Daten\report\report.md` succeeded; no external-directory permission block occurred.
- `observed` — `report.json` was read in full through line 2539 and `report.md` was read in full through line 70.
- `observed` — No web source, build, CTest, game execution, guest execution, decryption, commit, push, pull request, merge or publication was performed by this researcher for XB-NEXT-04.
- `documented (project-authored)` — Evidence labels follow `docs/RESEARCH_REGISTER.md:16-45`; general x86-64/Windows/PE facts are never promoted to Xbox Series X guest facts (`docs/RESEARCH_REGISTER.md:12-14`).

Report aliases used below:

- `RPT-JSON` = `D:\xbox\Spiel Daten\report\report.json`
- `RPT-MD` = `D:\xbox\Spiel Daten\report\report.md`

## 2. Executive findings

- `observed` — The supplied analyzer reports 26 files, 106,600,137,801 bytes and 6 analysed PE files; `.rpf` and `.ttpf` dominate the byte total (`RPT-JSON:2-34`; `RPT-MD:3-15`).
- `observed` — The main executable `game_scarlett_master.exe` is reported as 64-bit `machine=0x8664`, `subsystem=2`, OS `6.0`, with a 120,181,760-byte file, 184,995,840-byte `size_of_image`, entry RVA `0x56a1ec8` and 68 imported DLLs (`RPT-MD:28`; `RPT-JSON:965-979`).
- `observed` — The report records Xbox/GDK-facing import names across memory, graphics, audio, HTTP, input and container-streaming families, including `XMemAlloc`, `D3D12XboxCreateDevice`, `XGCreateTextureComputer`, `XDspActivate`, `HCHttpCallPerformAsync`, `gameinput.dll!ord1` and `DStorageGetFactory` (`RPT-JSON:1247-1259,1324-1328,1452-1455,1445-1450,1474-1512,1439-1440,1465-1467`).
- `unknown` — Import names and export names establish dependency strings in the supplied analyzer output only; they do not establish implemented XBridge behavior, guest ABI, API semantics or executability.
- `observed` — The report classifies `XboxBvhBuilder.dll` as an invalid PE with the quoted error `kein gueltiges PE: 'DOS Header magic not found.'` (`RPT-MD:25`; `RPT-JSON:753-756`).
- `observed` — The report's container result is the quoted `Container (verschluesselt/gepackt?): 0` and an empty `containers` array (`RPT-MD:5`; `RPT-JSON:140`).
- `documented (project-authored)` — Current XBridge has generic and synthetic seams — a read-only analyzer, bounded synthetic PE loader, generic guest memory, 13 neutral service ID tokens, a narrow x64 interpreter and fixed synthetic native routines — but no Xbox-specific service implementation (`include/xbridge/analysis/game_intelligence.hpp:11-13,35-61`; `include/xbridge/loader/executable_loader.hpp:15-17,46-52`; `include/xbridge/loader/pe_image.hpp:8-10,42-59,98-116`; `docs/ARCHITECTURE.md:30-38,54-61,150-191`; `include/xbridge/runtime/system_services.hpp:7-28`; `include/xbridge/execution/native_execution_backend.hpp:10-33`).
- `documented (project-authored)` — The repository's current scope explicitly excludes executing Xbox executables, loading firmware/keys, implementing Xbox security and shipping or decrypting commercial game content (`docs/ARCHITECTURE.md:63-69`).
- `documented (project-authored)` — The requirements matrix contains 44 distinct requirements: implemented 0, partial 9, planned 3, missing 27, unknown 5 (`docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12`).
- `hypothesis` — The supplied report is useful as an evidence inventory for future, separately approved research packages; it is not a feasibility proof for native, hybrid or interpreter execution.

## 3. Area A — PE loader/modules

- `observed` — The report lists 26 files, 6 PE files, 1 `.exe` at 120,181,760 bytes, 5 `.dll` files at 2,085,393 bytes and a `MicrosoftGame.Config` marker (`RPT-JSON:2-34,137-139`; `RPT-MD:3-19`).
- `observed` — The main exe reports `machine=0x8664`, `characteristics=0x22`, `subsystem=2`, OS `6.0`, `image_base=0x140000000`, entry RVA `0x56a1ec8`, `size_of_image=184,995,840`, `dll_characteristics=0x8160`, `checksum_ok=true` and `signed=false` (`RPT-JSON:965-979,1787-1797`).
- `observed` — Main exe sections include `.text` 99,875,174/99,875,328 bytes with flags `0x60000020`, `.rdata` 9,047,052/9,047,552, `.data` 71,222,168/6,434,304, `.pdata` 3,591,084/3,591,168, `.tls` 5,249/5,632, `.reloc` 1,221,456/1,221,632, `.retplne` 228/512 with flags `0x0`, `.voltbl` 1,828/2,048 with flags `0x0` and two `.xbld` sections at 1,721/2,048 and 220/512 bytes (`RPT-JSON:979-1060`).
- `observed` — Other valid PE files report `Bink2Scarlett.dll` with `high_entropy_sections=[".data"]` and `signed=false`, `XCurl.dll` with `signed=true`, `amd_fidelityfx_loader_xbox.dll` with `signed=false` and `amd_fidelityfx_upscaler_xbox.dll` with `signed=false` (`RPT-MD:23-28`; `RPT-JSON:404-407,745-750,874-879,955-962`).
- `observed` — The report records .reloc sections in all five valid PE files (`RPT-JSON:237-243,474-480,814-820,946-952,1045-1050`) and TLS flags for the main exe and the FFX upscaler (`RPT-JSON:1792,959`).
- `observed` — `XboxBvhBuilder.dll` is reported as size 884,224 bytes with the quoted error `kein gueltiges PE: 'DOS Header magic not found.'` (`RPT-JSON:109-110,753-756`; `RPT-MD:25`).
- `documented (project-authored)` — XBridge's `ExecutableLoader` is explicitly a state machine for bounded synthetic PE images and records an entry point without executing it (`include/xbridge/loader/executable_loader.hpp:15-17,24-30,46-52`).
- `documented (project-authored)` — XBridge's PE metadata model records machine, magic, image base, entry point, sections, relocations, imports, dependencies and exports for bounded synthetic PE32+/AMD64 test images (`include/xbridge/loader/pe_image.hpp:8-10,98-116`).
- `documented (project-authored)` — XBridge's read-only analyzer records file presence, size, PE fields, imports, exports, relocation types, uncertainties and labeled indicators, and explicitly refuses compatibility or executability verdicts (`include/xbridge/analysis/game_intelligence.hpp:11-13,16-30,35-61`).
- `documented (project-authored)` — The reviewed loader scope handles only `ABSOLUTE` and `DIR64` relocations and rejects other types before writes (`docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:53-55`; `docs/ARCHITECTURE.md:58`).
- `unknown` — The semantics of `.xbld`, `.retplne` and `.voltbl` are not established by the report or by XBridge documentation.
- `unknown` — The cause of the invalid local `XboxBvhBuilder.dll`, and whether it is related to the imported name `xboxbvhbuilder.dll`, remains unresolved.
- `documented (project-authored)` — Area A status classification is 7 requirements: partial 4, missing 1, unknown 2; no implemented or planned requirement in this area (`docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,18-24`).

## 4. Area B — XMem/memory

- `observed` — The main exe imports 12 `xmem.dll` symbols, including `XMemAlloc`, `XMemFree`, `XMemAllocDefault`, `XMemVirtualAlloc`, `XMemAllocatePhysicalPages`, `XMemMapPhysicalPages`, `XMemMapPhysicalPagesScatter`, `XMemGetWorkingSetStatistics`, `XMemIsToolsMemAvailable` and `XMemTransferMemory` (`RPT-JSON:1247-1259`; `RPT-MD:54`).
- `observed` — `Bink2Scarlett.dll` imports `XMemFree` and `XMemAlloc` (`RPT-JSON:266-269`).
- `observed` — The main exe's single reported export is `__XMemAllocationHooks__` (`RPT-JSON:1788-1791`).
- `observed` — The main exe imports generic `VirtualAlloc`, `VirtualFree` and `VirtualQueryEx` (`RPT-JSON:1261-1265`) and `acphal.dll` APU-address helpers including `ApuAlloc`, `ApuIsVirtualAddressValid`, `ApuMapApuAddress` and `ApuMapVirtualAddress` (`RPT-JSON:1525-1534`).
- `documented (project-authored)` — XBridge's `GuestMemory` scaffold is generic: 32-bit byte offsets, bounded half-open ranges, `std::out_of_range`/`std::length_error` failures and no reservation, permissions, committed/unmapped state, aliasing or host mapping (`docs/ARCHITECTURE.md:30-38`).
- `documented (project-authored)` — The address-space seam preserves existing signatures and explicitly does not model Xbox virtual memory (`docs/ARCHITECTURE.md:194-196`).
- `documented (project-authored)` — XBridge's runtime defines a neutral `ServiceId::VirtualMemory`, not an Xbox function signature or behavior (`include/xbridge/runtime/system_services.hpp:7-28`; `docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:45,49`).
- `documented (project-authored)` — The `xmem` family is recorded as a public research lead only, and no external source is adopted (`docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:49`).
- `unknown` — XMem signatures, physical-page semantics, working-set statistics behavior, hook semantics and guest-address ownership are not established by the report.
- `documented (project-authored)` — Area B status classification is 5 requirements: missing 3, partial 1, planned 1; no implemented or unknown requirement in this area (`docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,30-34`).

## 5. Area C — x64 ABI/TLS/SEH/CRT/runtime

- `observed` — All five valid PE files report `machine=0x8664` and `64bit=True` (`RPT-MD:23-28`).
- `observed` — The main exe reports `tls=true`, a `.tls` section, a `.pdata` section and C++-runtime imports from `msvcp140.dll` and `vcruntime140.dll` with mangled symbols such as `??0?$basic_iostream...`, `__CxxFrameHandler3`, `__C_specific_handler` and `__RTDynamicCast` (`RPT-JSON:1005-1027,1094-1208,1576-1603,1792`).
- `observed` — The main exe imports CRT startup and exception symbols including `_initterm`, `_seh_filter_exe`, `__p___argc`, `__p___argv` and `_register_thread_local_exe_atexit_callback` (`RPT-JSON:1635-1661`).
- `observed` — The main exe imports unwind helpers `RtlCaptureContext`, `RtlLookupFunctionEntry` and `RtlVirtualUnwind` (`RPT-JSON:1427-1431`).
- `documented (project-authored)` — XBridge models `X64CpuState` as storage for 16 GPR slots, `RIP` and `RFLAGS`, with no CPU reset behavior, SIMD/FPU state or Xbox-specific properties (`docs/ARCHITECTURE.md:89-97`).
- `documented (project-authored)` — The decoder supports NOP and narrow MOV forms with explicit error statuses, and the interpreter supports NOP and register/immediate or register/register or register/memory MOV with bounded memory access (`docs/ARCHITECTURE.md:106-122,150-179`).
- `documented (project-authored)` — Current interpreter limitations include no 8-bit MOV, SIB, displacement, FLAGS execution, SIMD/FPU, privileged instructions, Xbox-specific CPU properties, JIT or thrown decode exceptions (`docs/ARCHITECTURE.md:182-192`).
- `documented (project-authored)` — Native execution is limited to fixed, trusted, compiled-in `NativeRoutineId` routines; no caller-supplied pointer, byte buffer or RVA is accepted (`include/xbridge/execution/native_execution_backend.hpp:10-33`).
- `documented (project-authored)` — AVX2/BMI/FMA IDs are gating metadata in the synthetic native proof, not emitted instructions or guest requirements (`docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:33-39`).
- `unknown` — Guest CPUID leaves, AVX2/BMI/FMA requirements, guest ABI, TLS initialization order/callback semantics, exception and interrupt injection, syscall surface and privileged-instruction behavior remain unresolved.
- `documented (project-authored)` — Area C status classification is 6 requirements: partial 3, missing 3; no implemented, planned or unknown requirement in this area (`docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,40-45`).

## 6. Area D — D3D12.X/XG/FFX/BVH graphics

- `observed` — The main exe imports `d3d12_xs.dll` symbols `D3D12ConfigureVirtualMemoryX`, `D3D12SerializeVersionedRootSignature` and `D3D12XboxCreateDevice` (`RPT-JSON:1324-1328`; `RPT-MD:42`).
- `observed` — The main exe imports `xg_xs.dll` symbols `XGCreateTextureComputer` and `XGSuggestSwizzleMode` (`RPT-JSON:1452-1455`; `RPT-MD:51`).
- `observed` — The main exe imports seven C++-mangled `xboxbvhbuilder.dll` symbols including `BuildRaytracingAccelerationStructureBatched2X`, `RehydrateRaytracingAccelerationStructureBatched2X`, `CopyRaytracingAccelerationStructure` and `Set64BitMortonCodes`, with `D3D12XBOX_*` and XDXR type names in the mangled text (`RPT-JSON:1456-1463`; `RPT-MD:48`).
- `observed` — The main exe imports `amd_fidelityfx_loader_xbox.dll` symbols `ffxCreateContext`, `ffxDestroyContext`, `ffxDispatch` and `ffxQuery`; the loader DLL exports those plus `ffxConfigure` (`RPT-JSON:866-873,1468-1472`; `RPT-MD:33`).
- `observed` — `amd_fidelityfx_upscaler_xbox.dll` reports 0 imports and 0 exports, has `.xbld` and `.reloc` sections and a PDB path containing `api\xbox\Gaming.Xbox.Scarlett.x64\Release\...` for the loader DLL (`RPT-JSON:875-877,930-952,955-962`).
- `documented (project-authored)` — XBridge requires separate guest command/API interpretation, guest resource/synchronization modeling, shader translation, host backend and optional software/reference layers, and must not assume PC D3D12 equals Xbox-specific graphics interfaces (`docs/ARCHITECTURE.md:42-52,231-233`).
- `documented (project-authored)` — The reviewed graphics position is that D3D12-to-D3D12 is a research approach, not an accepted equivalence, and that D3D12.X semantics, shader formats, memory model and synchronization remain unresolved (`docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:59-61`).
- `unknown` — Guest command encoding, shader binary format, D3D12.X method layout, BVH semantics, swizzle algorithms and rendering correctness are not established by the supplied report.
- `documented (project-authored)` — Area D status classification is 6 requirements: missing 3, planned 2, unknown 1; no implemented requirement in this area (`docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,51-56`).

## 7. Area E — Bink/chad/xapu/Media Foundation audio/video

- `observed` — The main exe imports 23 `bink2scarlett.dll` symbols including `BinkOpenWithOptions`, `BinkDoFrame`, `BinkDoFrameAsyncMulti`, `BinkDoFrameAsyncWait`, `BinkGetFrameBuffersInfo`, `BinkRegisterFrameBuffers`, `BinkSetSoundSystem`, `BinkSetVolume` and `BinkStartAsyncThread` (`RPT-JSON:1536-1559`; `RPT-MD:36`).
- `observed` — `Bink2Scarlett.dll` exports 74 Bink functions, including `BinkCopyToBuffer`, `BinkGetGPUDataBuffersInfo`, `BinkOpenXAudio2`, `BinkSetOSFileCallbacks` and `BinkSetMemory` (`RPT-JSON:351-403`).
- `observed` — `Bink2Scarlett.dll` imports `xaudio2_9.dll!ord1` (`RPT-JSON:297-299`).
- `observed` — The main exe imports `chad.dll` symbols `XDspActivate`, `XDspConnectWithMaximumStreamLimit`, `XDspDeactivate`, `XDspDisconnect` and `XDspSubmitCommand` (`RPT-JSON:1445-1450`; `RPT-MD:39`).
- `observed` — The main exe imports `xapu.dll` symbols `XApuConnect`, `XApuEnqueueCommand`, `XApuDequeueResult`, `XApuGetResultCount` and `XApuDisconnect` (`RPT-JSON:1518-1523`).
- `observed` — The main exe imports `mfplat.dll` symbols `MFStartup`, `MFShutdown`, `MFCreateAttributes`, `MFCreateMediaType`, `MFCreateSourceResolver`, `MFGetSystemTime` and `mfreadwrite.dll!MFCreateSourceReaderFromMediaSource` (`RPT-JSON:1399-1408`).
- `documented (project-authored)` — XBridge defines only neutral `ServiceId::Audio` and has no Xbox DSP, APU, codec or media-source implementation (`include/xbridge/runtime/system_services.hpp:7-28`).
- `unknown` — Codec formats, frame-buffer layouts, DSP graph semantics, APU command formats and Media Foundation behavior are not established by the report.
- `documented (project-authored)` — Area E status classification is 5 requirements: missing 5; no implemented, partial, planned or unknown requirement in this area (`docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,62-66`).

## 8. Area F — config identity/HTTP/Winsock/input/window/crypto/registry

- `observed` — `MicrosoftGame.Config` reports `Name=RockstarGamesInc.28872443D20A7`, publisher `CN=3164F9C2-2C28-4832-BBE3-A642A4EF0054`, `Version=1.0.2.1`, executable `game_scarlett_master.exe`, `Id=GTAO`, `TargetDeviceFamily=Scarlett`, `OverrideDisplayName=Grand Theft Auto VI`, `IsDevOnly=false`, `TitleId=7952987A` and `MSAAppId=000000004060F900` (`RPT-JSON:141-177`).
- `observed` — The main exe imports 35 `libhttpclient.gdk.dll` `HC*` symbols including `HCInitialize`, `HCHttpCallCreate`, `HCHttpCallPerformAsync`, `HCHttpCallResponseGetStatusCode` and `HCTraceSetEtwEnabled` (`RPT-JSON:1474-1512`; `RPT-MD:45`).
- `observed` — `XCurl.dll` and the main exe import `winhttp.dll` symbols including `WinHttpOpen`, `WinHttpConnect`, `WinHttpOpenRequest`, `WinHttpSendRequest`, `WinHttpReceiveResponse`, `WinHttpReadData` and `WinHttpCloseHandle` (`RPT-JSON:591-606,1410-1425`).
- `observed` — The main exe imports `ws2_32.dll` symbols including `WSAStartup`, `socket`, `bind`, `connect`, `listen`, `accept`, `send`, `recv`, `getaddrinfo` and `gethostbyname` (`RPT-JSON:1349-1374`).
- `observed` — The main exe imports `gameinput.dll!ord1` (`RPT-JSON:1439-1440`).
- `observed` — The main exe imports `ext-ms-win-rtcore-ntuser-window-ansi-l1-1-0.dll!CreateWindowExA`, `ext-ms-win-rtcore-ntuser-message-ansi-l1-1-0.dll!PeekMessageA`, `DispatchMessageA`, `TranslateMessage` and `ShowWindow` (`RPT-JSON:1334-1347`).
- `observed` — The main exe imports `bcrypt.dll!BCryptGenRandom`, `crypt32.dll!CertOpenStore`, `advapi32.dll!RegOpenKeyW` and `RegQueryValueExW` (`RPT-JSON:608-614,1329-1333,1565-1568`).
- `observed` — The main exe imports process/thread/synchronization API-set families including `CreateThread`, `GetCurrentProcessId`, `SetThreadPriority`, `EnterCriticalSection`, `CreateEventW`, `WaitForSingleObject` and `CreateSemaphoreW` (`RPT-JSON:1063-1079,1271-1288,1298-1307,1432-1434`).
- `documented (project-authored)` — XBridge's neutral service IDs include `System`, `GuestProcess`, `Thread`, `Synchronization`, `File`, `Storage`, `Input`, `Audio`, `Graphics`, `Timing` and `ErrorLogging`; these are interface-only markers with no Xbox function signature or host API call (`include/xbridge/runtime/system_services.hpp:7-28`).
- `documented (project-authored)` — The neutral registry distinguishes `NotRegistered`, `Unsupported` and `Available` and does not turn an unknown service into a successful no-op (`docs/architecture/XB_NEXT_03_EXECUTION_STRATEGY_DRAFT.md:43-49`).
- `unknown` — Xbox system-service identities, title-identity enforcement, HTTP/Winsock service semantics, input protocol, window/message behavior and crypto/registry behavior are not established by the report.
- `documented (project-authored)` — Area F status classification is 9 requirements: missing 8, partial 1; no implemented, planned or unknown requirement in this area (`docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,72-80`).

## 9. Area G — RPF/TTPF/DStorage containers/streaming

- `observed` — The report lists 6 `.rpf` files totalling 95,384,243,044 bytes and 11 `.ttpf` files totalling 11,092,809,957 bytes (`RPT-JSON:5-33`; `RPT-MD:9-10`).
- `observed` — The largest reported files include `levels.rpf` at 61,512,488,669 bytes, `audio.rpf` at 17,349,454,239 bytes, `movies.rpf` at 6,311,149,581 bytes and `pack\core_bvh4\core_bvh4.rpf` at 5,847,771,069 bytes (`RPT-JSON:35-55`).
- `observed` — The report records the quoted `Container (verschluesselt/gepackt?): 0` and an empty `containers` array (`RPT-MD:5`; `RPT-JSON:140`).
- `observed` — The main exe imports `xcrdapi.dll!DStorageGetFactory` (`RPT-JSON:1465-1467`).
- `observed` — The report lists `Catalog000.bin` at 815,104 bytes and one `.alignmentchunk` file at 1 byte (`RPT-JSON:22-25,113-115`).
- `documented (project-authored)` — XBridge's analyzer is read-only, never writes, moves, extracts, uploads or executes source files, and records uncertainty rather than a platform verdict (`include/xbridge/analysis/game_intelligence.hpp:11-13,24-30,35-61`).
- `documented (project-authored)` — XBridge has a neutral `ServiceId::Storage`, but no RPF/TTPF reader, chunk decoder or DirectStorage-family implementation is documented in the cited repository files (`include/xbridge/runtime/system_services.hpp:7-28`).
- `documented (project-authored)` — Prior register evidence leaves retail container internals, chunk format and encryption construction `unknown` (`docs/RESEARCH_REGISTER.md:361`).
- `unknown` — The meaning of the analyzer's zero encrypted/packed classification, `.rpf`/`.ttpf` internal layout, index semantics and streaming behavior remain unresolved.
- `documented (project-authored)` — Area G status classification is 6 requirements: missing 4, unknown 2; no implemented, partial or planned requirement in this area (`docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12,86-91`).

## 10. Status-count summary

- `documented (project-authored)` — The matrix records 44 requirements across A-G (`docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:12`).

| Status | Count | `documented (project-authored)` meaning in this package |
|---|---:|---|
| implemented | 0 | No Xbox-facing requirement has repository evidence of completed behavior. |
| partial | 9 | Generic or synthetic seams exist but do not supply the Xbox-facing requirement. |
| planned | 3 | Repository documents record a direction or research lead; no behavior is implemented. |
| missing | 27 | No cited repository source or document supplies the named requirement. |
| unknown | 5 | Available report and repository evidence is insufficient to classify the requirement. |

- `documented (project-authored)` — No status is justified by an import name, export name, header, neutral service ID, stub, test-only synthetic routine or documentation phrase alone (`docs/AGENT_TASKS.md:4097-4099`; `docs/research/XB-NEXT-04_REQUIREMENTS_MATRIX.md:93-95`).

## 11. Review decisions for review-only files

| File | Decision | Justification with exact location |
|---|---|---|
| `README.md` | No change | `documented (project-authored)` — It already states early-foundation status, no Xbox game/executable execution, no compatibility/playability/performance claims and no proprietary assets (`README.md:5,14,21-22`); the package envelope marks it review-only and forbids unrelated changes (`docs/AGENT_TASKS.md:4096,4098`). |
| `docs/RESEARCH_REGISTER.md` | Changed by the researcher to add 7 conformant entries and update derived audit counts | `documented (project-authored)` — The package requires register entries for A-G (`docs/AGENT_TASKS.md:4096-4099`); the researcher has scoped edit permission for this path (`.opencode/agents/researcher.md:8-10`); each added entry uses the 23 fields in `docs/RESEARCH_REGISTER.md:46-72` and is assessed against `docs/RESEARCH_REGISTER.md:90-105`; Section 9 audit counts were synchronized with the seven added entries (`docs/RESEARCH_REGISTER.md:737-745`). No existing detail row was altered and reviewer verification is not claimed. |

## 12. Conclusion

- `documented (project-authored)` — The supplied analyzer output is an evidence inventory of PE metadata, import names, config identity and container-like extensions, not a proof of Xbox behavior or XBridge capability.
- `hypothesis` — Future separately approved packages could use the matrix to select lawful synthetic studies for generic seams; no such study is authorized here.
- `unknown` — Real-game feasibility, guest semantics, container internals, graphics formats, service signatures and performance remain unresolved.
- `documented (project-authored)` — This package records analysis only; no product code, tests, build configuration, ADR, execution path, compatibility claim, performance claim or publication was produced.
