# XB-0084 — Xbox 360 Splinter Cell: Double Agent analysis

- **PACKAGE_ID:** XB-0084
- **PACKAGE_TYPE:** RESEARCH
- **PRODUCT_BOUNDARY:** development-only
- **ACCESS DATE:** 2026-11-22
- **STATUS:** assessment/planning only; no implementation approved.
- **Evidence labels:** `documented` (explicit in an authoritative source or accepted project document), `observed` (directly seen in code, test output, or a reproducible local check), `hypothesis` (plausible but unverified), `unknown` (not established).
- **Boundary:** Development-only research document. Not product code. No Xbox 360 compatibility, title behavior, emulator implementation, or current-target change is claimed.

## Scope and target identity

- `documented` — The XB-0084 envelope names “Splinter Cell: Double Agent (Xbox 360)” as a separate backward-compatibility target. `docs/AGENT_TASKS.md:478`.
- `documented` — ADR-0004 and the accepted architecture remain unchanged: Xbox Series X is the sole current target; Xbox 360 compatibility is deferred, optional and would require a separate compatibility module. `docs/DECISIONS.md:18-21`; `docs/ARCHITECTURE.md:11-13`.
- `documented` — No Xbox 360 product module, profile, decoder, memory model, graphics path, audio/input path or test suite exists in the local scaffold. Architect and developer handoffs to the coordinator (`ses_dfb8b2c84ffed9nKKaN1zVSUCM`, `ses_dfb8af93bffeqPGOh2dL4DmGmO`); local `CMakeLists.txt:10-37`; `src/core/config.cpp:5-21`.
- `unknown` — No public authoritative source used here establishes Xbox 360 emulator implementation details or the title’s runtime behavior. All such claims remain `unknown`.

## Critical SKU separation

`documented` — Public evidence identifies at least two distinct products with the same or closely related title. They must not be conflated:

| SKU / version | Evidence | Label | Status in this analysis |
|---|---|---|---|
| Original Xbox version, product `BWV6WF6XMFZX` | W05 below describes “Tom Clancy’s Splinter Cell Double Agent” among original Xbox games announced for Xbox One backward compatibility; W04 and W06 concern original Xbox games playable on Xbox 360. | `documented` | Not the XB-0084 target. Must not be used as Xbox 360 SKU evidence. |
| Xbox 360 version, product `BPQ8RMCDSLRL` | W08 below is the Xbox Store listing labelled “Tom Clancy’s Splinter Cell Double Agent (Xbox 360)” and “Xbox One Backward Compatible.” | `documented` | This matches the envelope’s “(Xbox 360)” target wording. |

- `documented` — W04 and W06 are original-Xbox-on-Xbox-360 evidence. They are not evidence about the Xbox 360 SKU’s guest behavior.
- `documented` — W05 is original-Xbox-on-Xbox-One evidence. It is not evidence about the Xbox 360 SKU.
- `unknown` — Whether the original Xbox and Xbox 360 versions share any engine, asset pipeline or behavior is not established here.

## W01–W08 public evidence register

All sources below were accessed on `2026-11-22`. Hashes are recorded when the source exposed one. Direct body fetch failed for some official support pages; where noted, exact text was observed in an official-domain search result and is labeled accordingly.

| ID | Source / hash | Exact claim | Label | Reproduction path |
|---|---|---|---|---|
| W01 | `https://learn.microsoft.com/en-us/windows/win32/dxtecharts/coding-for-multiple-cores?view=windows-11`; access `2026-11-22`; `git_commit_id: 26465e18219d6311381977f154b0a5b7ed53c8d6` | “The Xbox 360 CPU has three processor cores on one chip”; “three CPU cores each with two hardware threads”; “On Xbox 360, you must explicitly assign software threads to a particular hardware thread by using XSetThreadProcessor.” Stack size must be a multiple of 64 KB. | `documented` | Fetch URL; read frontmatter `git_commit_id`; find the quoted phrases. |
| W02 | `https://learn.microsoft.com/en-us/windows/win32/dxtecharts/lockless-programming?view=windows-11`; access `2026-11-22`; `git_commit_id: 26465e18219d6311381977f154b0a5b7ed53c8d6` | “Lockless programming is particularly complex on Xbox 360”; “While the Xbox 360 CPU does not reorder instructions, it does rearrange write operations”; “The Xbox 360 CPU reorders memory operations much more aggressively than do x86 and x64 CPUs”; “On Xbox 360, MemoryBarrier is defined as lwsync.” | `documented` | Fetch URL; read frontmatter hash; find quoted phrases and synchronization-cost table. |
| W03 | `https://learn.microsoft.com/en-us/archive/blogs/nmercer/xbox360?view=windows-11`; access `2026-11-22`; `git_commit_id: 5019655ffa733bb8ab1266cc2a6a7b70a1ecdfa6` | “a custom IBM PowerPC-based CPU (Three symmetrical cores running at 3.2 GHz each, Two hardware threads per core; six hardware threads total) with 512MB of RAM”; “a teraflop of Overall System Floating-Point Performance.” | `documented` | Fetch URL; read frontmatter hash; find the quoted factsheet summary. |
| W04 | `https://news.xbox.com/en-us/2006/12/14/december-06-back-compat-update`; access `2026-11-22`; hash not exposed | December 2006 backward-compatible title update includes “Splinter Cell: Double Agent” in the original-Xbox-on-Xbox-360 list; the list exceeded 300 original Xbox games. | `documented` | Fetch URL; find the December 2006 heading and title list. |
| W05 | `https://news.xbox.com/en-us/2019/06/10/e3-2019-whats-next-xbox-backward-compatability`; access `2026-11-22`; hash not exposed | “Today, another long-awaited wave of Original Xbox games are playable on Xbox One” includes “Tom Clancy’s Splinter Cell Double Agent” linked as product `BWV6WF6XMFZX`; “With the addition of the four Tom Clancy Splinter Cell games from the Original Xbox…”. | `documented` | Fetch URL; find “New Original Xbox Games Announced Today” and quoted lines. |
| W06 | `https://support.xbox.com/en-US/help/xbox-360/games/backward-compatibility/play-original-xbox-games-on-xbox-360`; access `2026-11-22`; hash not exposed; body fetch unavailable, official search result exposed text | “You can play some original Xbox games on your Xbox 360 console”; list includes “Tom Clancy's Splinter Cell® Double Agent™.” | `documented` (official search result) | Search `site:support.xbox.com "Tom Clancy's Splinter Cell® Double Agent™" "Xbox 360"`; open result. |
| W07 | `https://support.xbox.com/en-US/help/games-apps/backward-compatibility/play-xbox-360-games-on-xbox-one`; access `2026-11-22`; hash not exposed; body fetch unavailable, official search result exposed text | “The ability to play digital and disc-based Xbox 360 and original Xbox games on Xbox One has expanded to include Xbox Series X|S and cloud gaming”; “These backward compatible games use an emulator for the older Xbox operating systems…”. | `documented` (official search result) | Search `site:support.xbox.com "expanded to include Xbox Series X|S" "emulator"`; open result. |
| W08 | `https://www.xbox.com/en-US/games/store/tom-clancys-splinter-cell-double-agent/bpq8rmcdslrl`; access `2026-11-22`; hash not exposed | Xbox Store listing for product `BPQ8RMCDSLRL`; official Xbox Wire sale snippets label it “Tom Clancy’s Splinter Cell Double Agent (Xbox 360)” and “Xbox One Backward Compatible.” | `documented` | Search `site:xbox.com/en-US/games/store "Double Agent" "bpq8rmcdslrl"` and `site:news.xbox.com "Double Agent (Xbox 360)" "Xbox One Backward Compatible"`. |

## Architecture facts that are valid for Xbox 360 only

- `documented` — W01–W03 describe an IBM PowerPC-based CPU with three symmetrical cores, two hardware threads per core, six hardware threads total, 512 MB RAM and Xbox 360-specific thread/memory ordering rules.
- `documented` — These are Xbox 360 facts. They are not Xbox Series X facts and must never be used to justify x64, Zen 2, unified-memory or D3D12.x assumptions for the current target.
- `unknown` — No authoritative public source in this audit establishes the Xbox 360 GPU, shader formats, system APIs, storage layout, audio protocol or input protocol needed for the target SKU.

## Shared host infrastructure versus separate guest layer

`hypothesis` — The following separation is a planning conclusion, not implemented behavior:

| Area | Shared host infrastructure? | Separate Xbox 360 guest layer? | Basis |
|---|---|---|---|
| Host capability detection | Yes, only as host-side facts | No | Host facts affect implementation/performance, not guest identity. `docs/ARCHITECTURE.md:15-24`. |
| Deterministic execution-policy selection | Yes, as tested policy/reporting | No | Existing selector is pure and profile-specific; no guest semantics. Architect handoff; `src/host/execution_policy.cpp:17-93`. |
| Diagnostics, status vocabulary, append-only evidence log | Yes, as workflow | No | Class 1 workflow concepts only. `docs/research/RESEARCH-007_ARCHITECTURE_COMPARISON.md:63-72`. |
| Generic byte storage primitive | Limited reuse only | Yes, for guest semantics | `GuestMemory` is generic and must not become an Xbox VM without approved scope. `docs/ARCHITECTURE.md:30-39`; `docs/DECISIONS.md:38-41`. |
| CPU instruction/thread semantics | No | Yes | Xbox 360 PowerPC/SMT facts in W01–W03 differ from x64 Series X work. |
| Memory ordering/protection/layout | No | Yes | W02 documents different memory reordering; no 360 VM model is known. |
| Graphics, shaders, resources, sync | No | Yes | No public authoritative details established here; PS5/PC terms are not Xbox facts. `docs/research/ps5-reference-project-analysis.md:273-280`. |
| System libraries, storage, audio, input | No | Yes | Separate ADRs/evidence required; none exist for 360. `docs/AGENT_WORKFLOW.md:178-179`. |
| Tests and evidence register | Yes, as discipline | Yes, for guest contracts | Shared test discipline, separate target identity/evidence/status. Tester handoff. |

- `documented` — Existing `ProfileKind` is `SeriesX` only; `series_s` is rejected in tests. Developer and architect handoffs; `src/core/config.cpp:5-21`; `tests/test_main.cpp:1284-1289`.
- `hypothesis` — A future Xbox 360 module would need its own profile/ADR, guest contracts, evidence register and tests before any product work.
- `documented` — This document does not create that profile, module or ADR. `docs/AGENT_TASKS.md:477`, `:494`.

## Compatibility and performance posture

- `unknown` — No 360 title compatibility can be asserted. W04–W08 establish availability/backward-compatibility facts only.
- `unknown` — No Xbox 360 emulator internals, performance profile, frame rate or runtime timing for the target SKU were established.
- `documented` — Original Xbox support and Xbox 360 support must remain separate claims. W04–W06 concern original Xbox; W08 concerns Xbox 360.
- `hypothesis` — Earliest lawful research step is a separate evidence register and ADR scope decision, not implementation or title-oriented proving.

## Deliberate omissions and open questions

- `unknown` — Title-specific behavior for SKU `BPQ8RMCDSLRL`.
- `unknown` — Original Xbox versus Xbox 360 engine/asset relationship.
- `unknown` — Backward-compatibility emulator implementation details referenced by W07.
- `unknown` — Xbox 360 graphics, shader, resource, storage, audio, input and synchronization requirements.
- `documented` — ADR-0004 and `docs/ARCHITECTURE.md:13` remain unchanged.
- `documented` — No product code, build, CTest, commit or game/software execution was performed.
