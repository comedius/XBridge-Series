# Agent Task Ledger — Archive 02

> Extracted verbatim from the former `docs/AGENT_TASKS.md`, original lines 754–1520. Historical line references in older records refer to the former monolithic file. Content is preserved; see the index at `docs/AGENT_TASKS.md`.

## RV-P1-C1 — bundled corrections from the failed P1 review (in_progress / needs_owner)

PACKAGE_ID: RV-P1-C1
PACKAGE_TYPE: BUGFIX
PRODUCT_BOUNDARY: development-only
GOAL: Apply only the owner-approved P1 review corrections and prepare the owner-only patches, without starting Package 2.
FILES: `docs/DEVELOPMENT_ENVIRONMENT.md`, `docs/PORTABLE_SETUP.md`, `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`; owner patches for `.gitignore`, `ai/README.md`, `opencode.jsonc`.
ALLOWED: Coordinator edits limited to `docs/*.md` explicitly allowed in `.opencode/agents/coordinator.md:5-55`; read-only inspection of all target files; prepared patches for owner-only files.
FORBIDDEN: No edits to `.gitignore`, `ai/README.md`, `opencode.jsonc`, `.opencode/**`, `AGENTS.md`, `docs/DECISIONS.md`, product code, tests, CMake or presets; no Package 2 envelope, publication group 2, staging, commit, push, merge, tag, PR, history change or permission broadening.
ACCEPTANCE: Every owner-approved P1 point is either applied by an authorized writer or recorded as an `OWNER_DECISION` with a complete patch; fresh independent reviewer verdict is obtained before P1 is called complete; no unapproved file is modified.
TESTS: Documentation-only: exact `git status --porcelain=v1 --untracked-files=all`, `git rev-parse HEAD`, `git branch --show-current`, `git diff --check`, `git diff HEAD --no-ext-diff`, `git diff HEAD --no-ext-diff --stat`, `git diff HEAD --no-ext-diff --name-only` plus full read-back. No product build or CTest for this documentation-only package.
REVIEW: Independent `reviewer` re-check mandatory after all corrections; architect skipped because this is pure documentation care with no interface, module-boundary, dependency or ADR change (`.opencode/agents/coordinator.md:142`). Reviewer prompt must be read-only (`read`/`glob`/`grep` only).
WORKTREE: None; main checkout `D:\XBridge 1.0\XBridge-Series-Github`.
OWNER_APPROVALS: Owner approved the four P1 corrections and the re-review. Owner-only edits for `.gitignore`, `ai/README.md` and `opencode.jsonc` remain `OWNER_DECISION` because the coordinator has `edit:"*" deny` with no allow for those paths.

`observed` — Baseline before coordinator edits: branch `git branch --show-current` → `docs/opencode-publication-group-2`; `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git status --porcelain=v1 --untracked-files=all` → 24 entries (11 modified, 13 untracked). The combined command `git status --short --branch; git rev-parse HEAD` was rejected: `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}`; only exact allowlisted commands are usable. Web policy verification via `fetch("https://opencode.ai/docs/llms.txt")` and `fetch("https://opencode.ai/docs/permissions/")` was rejected: `Permission denied: webfetch`.

`observed` — Finding disposition (reviewer IDs as retained in the approval): RV-P1-01 `.gitignore` backup/debug exclusions → `ACTIONABLE` but blocked, owner patch below; RV-P1-02 mutating Code Mode tools → `ACTIONABLE` but blocked and policy syntax `unknown`, owner patch below; RV-P1-03 machine identity/hardware → `ACTIONABLE`, applied by labeling `owner-provided` in `docs/DEVELOPMENT_ENVIRONMENT.md:3,28,30,54-59` and `docs/PORTABLE_SETUP.md:59`; RV-P1-04 stale/cross references → `ACTIONABLE`, applied in `docs/DEVELOPMENT_ENVIRONMENT.md:163-164,187` and `docs/PORTABLE_SETUP.md:31-33`; RV-P1-05 absent Package 2 envelope/group 2 → `OWNER_DECISION`/`unknown`, documented but not invented; RV-P1-06 `.git/info/exclude` and `/.env.*` side effect → `OWNER_DECISION`, no agent edit and patch deliberately excludes `/.env.*`; RV-P1-07 ledger snapshot discrepancy → `ACTIONABLE`, documented as supersession in `docs/PORTABLE_SETUP.md:59-60,109` and this block; RV-P1-08 `ai/README.md:13` PR instruction → `ACTIONABLE` but blocked, owner patch below; RV-P1-09 finding/severity/category recount → `UNKNOWN` from the retained summary, left for the fresh reviewer to recount rather than fabricated here.

`documented` — `docs/DEVELOPMENT_ENVIRONMENT.md` now separates `observed` path `D:\XBridge 1.0\XBridge-Series-Github` from `owner-provided` machine identity and hardware. Current observed values are VS2026 Community at `C:\Program Files\Microsoft Visual Studio\18\Community`, MSVC toolsets `14.44.35207`/`14.52.37025`, SDKs `10.0.26100.0`/`10.0.28000.0`, CMake/CTest `4.3.1-msvc1` by simple command name through `tester` (exit codes `unknown`), GitHub Desktop `app-3.7.0`, OpenCode resource version `2.4.1`. `E:`, `main`/`10e5e75`/32 entries and VS2022 are marked historical/superseded. Plain `cmake`/`ctest` reachability is `observed`; the former `CommandNotFoundException` is historical. The CRLF warning is `observed`, current exit code `unknown`.

`documented` — `docs/PORTABLE_SETUP.md` now cites `opencode.jsonc:73-109` plus agent frontmatter for unavailable `opencode debug config/agents`, records current observed toolchain values and the `58aa1f60…` branch/HEAD supersession, and marks the XB-0085 32-entry audit as a historical snapshot with PR state not re-verified.

`documented` — Owner patch 1, `.gitignore` (prepared, NOT applied):

```diff
--- a/.gitignore
+++ b/.gitignore
@@ -13,3 +13,7 @@
 *.so
 *.dylib
+
+# Owner-only development artifacts; keep out of publication.
+/.owner-patch-backup/
+/CMakePresets.json.backup
+/debug-*.txt
```

`documented` — Owner patch 2, `ai/README.md` (prepared, NOT applied):

```diff
--- a/ai/README.md
+++ b/ai/README.md
@@ -13 +13 @@
-7. Open a pull request; never merge directly to the protected main branch.
+7. Hand the result to the project owner; the owner performs all Git/GitHub publication actions.
@@ -28 +28 @@
-Step 7 of the workflow above is performed by the project owner. Agents must not create pull requests or perform any other Git/GitHub publication action.
+Step 7 is explicit: agents hand off; the project owner alone may open a pull request or perform any other Git/GitHub publication action.
```

`documented` — Owner patch 3, `opencode.jsonc` (prepared, NOT applied; resource spelling `unknown` because webfetch was denied):

```diff
--- a/opencode.jsonc
+++ b/opencode.jsonc
@@ -151,5 +151,12 @@
     { "action": "tool.use", "resource": "edit:*.env", "effect": "deny" },
     // Mirrors `read:*.env.*`. Whole-value `*` spans `/`; this also denies `.env.example` (side effect).
     // Pattern semantics are owner-documented; no runtime behavior is claimed as tested here.
-    { "action": "tool.use", "resource": "edit:*.env.*", "effect": "deny" }
+    { "action": "tool.use", "resource": "edit:*.env.*", "effect": "deny" },
+
+    // Mutating Code Mode tools; confirm resource spelling against OpenCode V2 before applying.
+    { "action": "tool.use", "resource": "automation.create", "effect": "deny" },
+    { "action": "tool.use", "resource": "automation.delete", "effect": "deny" },
+    { "action": "tool.use", "resource": "opencode.session.move", "effect": "deny" },
+    { "action": "tool.use", "resource": "opencode.worktree_create", "effect": "deny" },
+    { "action": "tool.use", "resource": "opencode.worktree_remove", "effect": "deny" }
   ]
 }
```

`documented` — No `/.env.*` rule is proposed because it would also deny `.env.example` (`docs/DEVELOPMENT_ENVIRONMENT.md:164`); `.git/info/exclude` is local owner territory and is not edited by any agent. `opencode.worktree_list` remains read-only and is not denied.

`documented` — P1 is not complete. After this coordinator-authorized portion, the remaining owner actions are: apply owner patch 1 (`.gitignore`) and owner patch 2 (`ai/README.md`), then trigger a fresh independent reviewer re-check. Owner patch 3 is withdrawn by owner decision and must not be applied. Package 2 remains closed and needs a separate owner-approved envelope.

`OWNER_DECISION` — Owner selected option (c) “Leave unapplied” for RV-P1-02. Classification: `OWNER_DECISION`, not `ACTIONABLE`; no agent correction is required for this finding. The prepared `opencode.jsonc` patch above is NOT applied and is now superseded/withdrawn as written; exact valid `<action>:<value>` strings remain `unknown`, and no permission broadening or further Code-Mode investigation is authorized in RV-P1-C1. RV-P1-02 is recorded as a documented limitation, not as a fix. P1 can be completed only after the owner applies patches 1 and 2, the state is recaptured/read back, and the mandatory independent reviewer re-check passes. Package 2 remains closed.

`observed` — Final coordinator self-check after all authorized edits: `git status --porcelain=v1 --untracked-files=all` → 26 entries (13 modified, 13 untracked); `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git diff --cached --name-only` → no output (empty index). The increase from the 24-entry baseline is exactly the two newly modified tracked docs `docs/AGENT_TASKS.md` and `docs/ROADMAP.md`; `docs/PORTABLE_SETUP.md` became modified and `docs/DEVELOPMENT_ENVIRONMENT.md` remained untracked.

`observed` — Verification commands and actual results: `git diff --check` → only `warning: in the working copy of 'CMakePresets.json', CRLF will be replaced by LF the next time Git touches it`, no error line; the shell tool did not expose an exit code, so the current exit state is `unknown` rather than a newly claimed `EXIT=0`. `git diff HEAD --no-ext-diff --stat` → 14 tracked files changed with the known CRLF warning; the exact insertion/deletion count is command-output evidence in the final coordinator report and is deliberately not frozen here because every appended ledger paragraph changes it. `git diff HEAD --no-ext-diff --name-only` → the 14 tracked paths (six `.opencode/agents/*.md`, `AGENTS.md`, `CMakePresets.json`, `ai/README.md`, `docs/AGENT_TASKS.md`, `docs/ARCHITECTURE.md`, `docs/PORTABLE_SETUP.md`, `docs/ROADMAP.md`, `opencode.jsonc`). `git diff HEAD --no-ext-diff` was executed and its returned diff preview inspected; the changed portions of all four coordinator-written docs were additionally read back in full with `read`. No product code, test, CMake, preset, agent configuration or permission file was edited by the coordinator in this package.

`observed` — `.owner-patch-backup/**`, `CMakePresets.json.backup` and `debug-*.txt` remain untracked and unchanged; none was staged, deleted, moved or overwritten. No build, CTest, staging, commit, push, merge, tag, release, fetch, reset, stash, clean, branch change, PR change or publication was performed. The independent reviewer re-check has not yet been started because the owner-only patches are still unapplied.

`documented` — Policy-syntax clarification after the owner chose “Erst Policy-Syntax klären”. Official OpenCode documentation was read successfully in the in-app browser: `https://opencode.ai/docs/policies/` states that a `tool.use` statement hard-denies a permission check and that the resource has the form `<action>:<value>`; examples include `shell:git push *`, `edit:*.env` and `webfetch:*`; invalid statements are dropped with a server-log warning. `https://opencode.ai/docs/tools/` states that denying `execute` with resource `*` removes Code Mode, that nested built-in and MCP tools still enforce their own permissions, that `worktree_remove` uses the `edit` permission with the worktree directory, and that the other listed worktree utilities request no built-in permission action. `https://opencode.ai/docs/permissions/` lists built-in actions (`read`, `edit`, `glob`, `grep`, `shell`, `subagent`, `skill`, `question`, `webfetch`, `websearch`, `generate_image`, `external_directory`, MCP actions and `execute`) and does not list bare names such as `automation.create`, `opencode.session.move`, `opencode.worktree_create` or `opencode.worktree_remove` as built-in actions.

`observed` — Additional browser inspection of official upstream `dev`-branch source was treated as non-authoritative for the installed OpenCode resource version `2.4.1`. It showed permission checks and tool registration for automation/session/worktree areas but did not establish exact resource strings for the mutating Code-Mode entries. Exact valid `<action>:<value>` resources for targeted denial of `automation.create`, `automation.delete`, `opencode.session.move`, `opencode.worktree_create` and `opencode.worktree_remove` in this installation therefore remain `unknown`.

`documented` — Owner patch 3 above is invalid as written and MUST NOT be applied: its bare Code-Mode tool names do not match the documented `<action>:<value>` resource form. Broad `execute:*` is documented as removing Code Mode, but it is broader than the owner-approved targeted correction and is not authorized by this package. No `opencode.jsonc` edit, permission change or bypass was performed.

`documented` — The RV-P1-02 decision is resolved at `:677`: the owner selected option (c), leave unapplied. No further Code-Mode syntax decision is pending. RV-P1-C1 remains `in_progress / needs_owner` only because owner patches 1 and 2 are not yet applied and the mandatory reviewer re-check has not run; P1 is not complete, and Package 2 remains closed.

## RV-P1-C2 — bundled correction round 1 for RV-P1-C1 review (in_progress)

PACKAGE_ID: RV-P1-C2
PACKAGE_TYPE: BUGFIX
PRODUCT_BOUNDARY: development-only
GOAL: Resolve the two ACTIONABLE findings from the RV-P1-C1 re-review without reopening Package 2.
FILES: `docs/AGENT_TASKS.md` (package record), `docs/ROADMAP.md`, `docs/DEVELOPMENT_ENVIRONMENT.md`.
ALLOWED: Coordinator edits to these three permission-approved `docs/*.md` paths (`.opencode/agents/coordinator.md:9-25`); read-only inspection of cited evidence files.
FORBIDDEN: No edits to `.gitignore`, `ai/README.md`, `opencode.jsonc`, `.opencode/**`, `AGENTS.md`, product code, tests, CMake or presets; no Package 2 envelope, publication group 2, staging, commit, push, merge, tag, PR, history change, permission broadening, build or CTest.
ACCEPTANCE: `docs/ROADMAP.md` no longer instructs applying the withdrawn `opencode.jsonc` patch; `docs/DEVELOPMENT_ENVIRONMENT.md` evidence pointers resolve to claimed content; no other RV-P1-C1 state is reopened; independent reviewer re-check passes.
TESTS: Read-back of the three files plus exact `git status --porcelain=v1 --untracked-files=all`, `git rev-parse HEAD`, `git branch --show-current`, `git diff --check`, `git diff HEAD --no-ext-diff --stat` and `git diff HEAD --no-ext-diff --name-only`. No product build or CTest for this documentation-only package.
REVIEW: Mandatory read-only reviewer re-check after corrections. Architect skipped: pure documentation care with no interface, module-boundary, dependency or ADR change.
WORKTREE: None; main checkout `D:\XBridge 1.0\XBridge-Series-Github`.
OWNER_APPROVALS: RV-P1-02 leave-unapplied decision already recorded at `:677`; no new owner decision required.

`observed` — Independent reviewer session `ses_df2403fb3ffeQJAOnU8s9587C7` returned `NEEDS_CHANGE` with two findings. Classification: RV-P1-REV-01 (HIGH) `ACTIONABLE`; RV-P1-REV-02 (LOW) `ACTIONABLE`; no `OWNER_DECISION`, `NON_BLOCKING` or `UNKNOWN` findings. Case B applies, with one bundled correction package for the single coordinator writer. Root causes: (a) the owner’s RV-P1-02 leave-unapplied decision was not propagated to the roadmap sequence; (b) two stale evidence pointers in `docs/DEVELOPMENT_ENVIRONMENT.md` do not resolve to the claimed content. This is bundled correction round 1; at most two rounds are permitted.

`observed` — Bundled correction round 1 applied by the coordinator in the three allowed docs. `docs/ROADMAP.md:39,41` now states that only patches 1 and 2 may be applied, that patch 3 (`opencode.jsonc`) is withdrawn/invalid/MUST NOT be applied, and that no Code-Mode syntax decision is pending. `docs/DEVELOPMENT_ENVIRONMENT.md:109` now cites `docs/RELEASE_CHECKLIST.md:47` and `docs/AGENT_TASKS.md:92,97` for the CRLF warning instead of `docs/AGENT_WORKFLOW.md`; `:164` now points directly to `docs/AGENT_TASKS.md:677,689-691` for the Code-Mode owner decision/withdrawal instead of the non-existent Section 12 row. A grep for `three owner-only|apply the three|drei owner-only|siehe Abschnitt 12|AGENT_WORKFLOW.md` dokumentiert` returned no matches. Read-back confirmed all four corrected locations.

`observed` — Post-correction verification: `git status --porcelain=v1 --untracked-files=all` → 26 entries (13 modified, 13 untracked), unchanged; `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git diff --check` → only the known `CMakePresets.json` CRLF warning, exit state `unknown`; `git diff HEAD --no-ext-diff --stat` → 14 tracked files; the exact insertion/deletion count is command-output evidence in the coordinator report and is deliberately not frozen here because appended ledger paragraphs change it; `git diff HEAD --no-ext-diff --name-only` → the same 14 tracked paths. No product code, test, CMake, preset, agent configuration, permission, `.gitignore`, `ai/README.md` or `opencode.jsonc` edit; no build, CTest, staging or publication.

`documented` — Independent reviewer follow-up session `ses_df2403fb3ffeQJAOnU8s9587C7` returned `PASS` for the RV-P1-C2 documentation correction state (round 1 of at most two). Round-2 finding classification: RV-P1-C2-REV-01 (LOW) `NON_BLOCKING` — a self-referential grep match at `docs/AGENT_TASKS.md:710`, caused by quoting the search pattern in the verification record; no operative stale text remains. No `ACTIONABLE`, `OWNER_DECISION` or `UNKNOWN` finding was introduced. Case A applies: no correction required, no second review started, existing `PASS` retained. Both RV-P1-REV-01 and RV-P1-REV-02 are closed. The `PASS` covers only the documentation correction state; it does not approve build/test results, implementation, Xbox compatibility, publication or permission changes.

`documented` — RV-P1-C2 is complete, and RV-P1-C1 P1 is complete on the documentation-only state after successful independent re-review. Open items remain separate from defects: `OWNER_DECISION` RV-P1-01 (`.gitignore` patch prepared, not applied), RV-P1-02 (leave unapplied; `opencode.jsonc` patch withdrawn), RV-P1-05 (no Package 2 envelope or publication group 2 defined), RV-P1-06 (`.git/info/exclude` and `/.env.*` side effect), RV-P1-08 (`ai/README.md` patch prepared, not applied). Manual owner action, if desired: apply patches 1 and 2; patch 3 must not be applied. `UNKNOWN` items remain: RV-P1-09 original severity/category recount, exact Code-Mode action strings (decision closed without a fix), current `git diff --check` exit state, and any Package 2 content. NON_BLOCKING RV-P1-C2-REV-01 is recorded for transparency only and deliberately not edited after the PASS.

`observed` — This completion-recording edit occurred after the reviewer PASS and changes no reviewed correction content. Latest command results: `git status --porcelain=v1 --untracked-files=all` → 26 entries; `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git diff --check` → known `CMakePresets.json` CRLF warning only; `git diff HEAD --no-ext-diff --name-only` → the same 14 tracked paths. No build, CTest, staging, commit, push, merge, tag, PR, publication, permission or Package 2 action.

## RV-P1-C3 — owner instruction supersession and Code-Mode security reopened (in_progress / needs_owner)

PACKAGE_ID: RV-P1-C3
PACKAGE_TYPE: BUGFIX
PRODUCT_BOUNDARY: development-only
GOAL: Reopen the Code-Mode security portion of P1 under the owner’s renewed explicit deny decision, verify OpenCode 2.4.1 syntax, and preserve the already reviewed documentation corrections.
FILES: `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`; owner-only patch targets remain `.gitignore` and `opencode.jsonc`.
ALLOWED: Coordinator edits limited to permission-approved `docs/*.md` paths; read-only research and architecture analysis; preparation of complete owner patches.
FORBIDDEN: No edit to `opencode.jsonc`, `.gitignore`, `ai/README.md`, `.opencode/**`, `AGENTS.md`, product code, tests, CMake, presets, permissions or agent configuration; no Package 2 execution, publication, commit, push, merge, tag, PR or history change.
ACCEPTANCE: OpenCode 2.4.1 rule syntax is evidence-labeled; a technically effective deny is either applied by the owner or explicitly stopped for owner decision if unsupported; target `.gitignore` rules are prepared unchanged; `ai/README.md` remains untouched; P1 status reflects the reopened state until a fresh reviewer verdict covers this supersession.
TESTS: Documentation-only: read-back, exact Git status/HEAD/branch/diff checks, security-rule consistency check, backup/debug path presence check. No product build or CTest.
REVIEW: Fresh independent reviewer verdict required after the reopened P1 items are resolved. Architect skipped for this reopening; architect is separately engaged only for the Package 2 proposal.
WORKTREE: None; main checkout `D:\XBridge 1.0\XBridge-Series-Github`.
OWNER_APPROVALS: Owner reaffirmed mutating Code-Mode deny decision and approved the three targeted `.gitignore` rules; `ai/README.md` remains blocked pending a separate owner release.

`OWNER_DECISION` — The new owner instruction explicitly supersedes the earlier option (c) “leave unapplied” for RV-P1-02. The previous withdrawal of the `opencode.jsonc` patch did not cancel the owner’s security decision; mutating Code-Mode tools must again be denied if a supported OpenCode 2.4.1 rule exists. If no reliable technical lock is established, the change is stopped and documented for owner decision rather than simulated with ineffective syntax. The three approved `.gitignore` rules are `/.owner-patch-backup/`, `/CMakePresets.json.backup` and `/debug-*.txt`; `.env.example` must remain available. Owner-approved documentation corrections already reviewed are preserved. `ai/README.md` is not edited.

`observed` — Effective write rights rechecked before any assignment: coordinator, developer, tester and researcher all have `edit` deny for `opencode.jsonc`; coordinator also denies `.gitignore` and `ai/**`, developer denies `ai/**` and all coordination docs, tester and researcher only allow their own scoped docs. No installed agent can apply the owner-only patch, so any resulting config change must be applied by the owner.

`observed` — Read-only `researcher` session `ses_df22fa77affebBDIGkCZ25tdx4` and read-only `architect` session `ses_df22fa779ffePSfY4Burv04Det` were launched in parallel for, respectively, version-specific OpenCode 2.4.1 permission syntax evidence and the Package 2 architecture proposal. Both were instructed not to edit, not to run shell, not to start subagents and not to change provider/model. P1 is reopened until the research result is evaluated and a fresh reviewer verdict is obtained; Package 2 remains prepared-only and not started.

`observed` — Architect session `ses_df22fa779ffePSfY4Burv04Det` completed read-only with no edits, shell calls, subagents or owner questions. It proposed `XB-P2` as a documentation-only FEATURE with coordinator as sole writer, six new docs (`docs/AGENT_CORE_ARCHITECTURE.md`, `docs/AGENT_AUTONOMY.md`, `docs/OPENCODE_AUTOMATION_PLAN.md`, `docs/AUTOMATED_QUALITY_GATES.md`, `docs/GITHUB_AGENT_WORKFLOW.md`, `docs/DESKTOP_MIGRATION_CHECKLIST.md`) plus cross-links from existing coordination docs. The proposal keeps `docs/AGENT_WORKFLOW.md` as central executable workflow, treats Agent Core as development documentation rather than product runtime, uses only the six installed roles, keeps specialists planned, defines a platform-neutral adapter contract with OpenCode now and future/planned Codex/Claude Code/GitHub Copilot mappings, and specifies a 12-step owner-gated automation flow, persistence/recovery, quota pause and token-efficiency rules. It explicitly forbids config/permission/product/publication changes and states Package 2 is not started. Owner approval of the envelope and document structure is required before execution.

`documented` — Read-only researcher session `ses_df22fa77affebBDIGkCZ25tdx4` identified OpenCode v2.4.1 as release commit `6f0514052a3c859894ad01bb14cc14dda1e44b96` (`6f05140`) from `https://api.github.com/repos/anomalyco/opencode/releases/tags/v2.4.1` and `https://github.com/anomalyco/opencode/releases/tag/v2.4.1`. Source evidence from the v2.4.1 tag shows that tool-identity suppression uses top-level `permissions` with `resource: "*", effect: "deny"`, not `policies.tool.use` bare names. `policies.mdx` v2.4.1 states `tool.use` resources are `<action>:<value>`; arbitrary strings in that field are schema-valid but do not match generated `<action>:<resource>` events, so the previously prepared bare-name patch is inert and must remain unapplied. No runtime verification was performed; actual behavior in the installed 2.4.1 session remains `unknown`.

`observed` — Exact v2.4.1 action names source-derived by the researcher: `automation_create`, `automation_delete`, `opencode_session_move`, `opencode_worktree_create`; these use underscores from namespace/name normalization. `opencode_worktree_remove` has no tool-specific action: v2.4.1 registers it with `permission: "edit"`, so only a broad `edit` deny can affect it. Evidence URLs: `https://github.com/anomalyco/opencode/blob/v2.4.1/packages/core/src/tool/runtime.ts`, `.../tool.ts`, `.../tool/plugin/automation.ts`, `.../tool/plugin/opencode.ts`, `.../automation.ts`, and `.../config/plugin/policy.ts`.

`observed` — Agent-rule override audit: grep over `.opencode/agents/**` found no rule for `automation_create`, `automation_delete`, `opencode_session_move`, `opencode_worktree_create` or `opencode_worktree_remove`. Agent rules only reference `edit` with path-scoped allow/deny entries, so none of the four new action denies would be overridden by an installed agent rule. The `worktree_remove` `edit` case remains different because path-scoped `edit` allows exist in several agents.

`documented` — Revised owner patch 3, `opencode.jsonc` (source-supported for v2.4.1, runtime-unverified, NOT applied; replaces the invalid bare-name patch):

```diff
--- a/opencode.jsonc
+++ b/opencode.jsonc
@@ -115,6 +115,13 @@
      { "action": "edit", "resource": "docs/ARCHITECTURE.md", "effect": "ask" },
      { "action": "edit", "resource": "opencode.jsonc", "effect": "ask" },
-     { "action": "edit", "resource": ".opencode/**", "effect": "ask" }
+     { "action": "edit", "resource": ".opencode/**", "effect": "ask" },
+
+     // RV-P1-C3: suppress mutating Code Mode tool identities in OpenCode 2.4.1.
+     // Source-derived action names use underscores; runtime behavior is not locally verified.
+     { "action": "automation_create", "resource": "*", "effect": "deny" },
+     { "action": "automation_delete", "resource": "*", "effect": "deny" },
+     { "action": "opencode_session_move", "resource": "*", "effect": "deny" },
+     { "action": "opencode_worktree_create", "resource": "*", "effect": "deny" }
    ],
```

`documented` — Owner patch 1, `.gitignore`, remains approved, unchanged and NOT applied (`/.owner-patch-backup/`, `/CMakePresets.json.backup`, `/debug-*.txt`); context still matches `.gitignore:13-15`. Owner patch 2, `ai/README.md`, remains prepared but untouched pending a separate owner release. `.env.example` is not affected.

`OWNER_DECISION` — `opencode.worktree_remove` cannot be tool-specifically denied in OpenCode 2.4.1: the tool deliberately uses the built-in `edit` action. A global `edit:* deny` would be broad and is not authorized. Per the owner’s instruction to stop an unreliable change for owner decision, this sub-item is stopped and documented; no broad edit deny was prepared or applied. Awaiting owner decision: accept the documented limitation, or explicitly authorize a broader edit deny with its side effects. **(Pre-decision snapshot — superseded by the owner decision recorded below; retained for history only.)**

`OWNER_DECISION` — Owner selected “Accept limitation” for `opencode.worktree_remove`. The four source-supported tool-identity denies stand as the revised owner patch; no broad `edit:* deny` is authorized or prepared, and the residual `worktree_remove` exposure is a documented limitation rather than a claimed fix. P1 remains blocked until the owner applies the `.gitignore` patch and the revised `opencode.jsonc` patch, after which a fresh independent reviewer verdict on the applied state is required. `ai/README.md` remains untouched pending its separate owner release. Package 2 remains prepared-only and not started.

## RV-P1-C3-C1 — bundled correction for review finding RV-P1-C3-REV-01 (in_progress / needs_re-review)

PACKAGE_ID: RV-P1-C3-C1
PACKAGE_TYPE: BUGFIX
PRODUCT_BOUNDARY: development-only
GOAL: Remove the stale current-state contradiction identified by the independent reviewer so the ledger and roadmap agree on the required owner actions.
FILES: `docs/ROADMAP.md`, `docs/AGENT_TASKS.md`.
ALLOWED: Coordinator edits limited to these permission-approved `docs/*.md` paths.
FORBIDDEN: No config, permission, agent, product, test, CMake or preset edit; no patch applied; no Package 2 execution or publication.
ACCEPTANCE: No current-state line says “only patch 1 and patch 2” or that patch 3 awaits syntax verification; the pre-decision `worktree_remove` snapshot is explicitly superseded; a fresh independent reviewer re-check passes.
TESTS: Documentation-only read-back plus exact Git status/HEAD/branch/diff checks.
REVIEW: Mandatory independent reviewer re-check after the bundled correction.
WORKTREE: None; main checkout.
OWNER_APPROVALS: None new; uses the owner decisions already recorded under RV-P1-C3.

`OWNER_DECISION` — Reviewer verdict `FAIL` (`session ses_df2403fb3ffeQJAOnU8s9587C7`) returned one finding: RV-P1-C3-REV-01 (HIGH) `ACTIONABLE`. Classified `ACTIONABLE`: stale current-state text at `docs/ROADMAP.md:41` and the unsuperseded pre-decision snapshot at `docs/AGENT_TASKS.md:771` could mislead the owner into applying the wrong patch set. Case B applies: one bundled correction package per the single writer (coordinator), grouped by the single root cause (stale current-state after RV-P1-C3 append), then a mandatory independent reviewer re-check. No `OWNER_DECISION`, `NON_BLOCKING` or `UNKNOWN` finding was added by the reviewer.

`observed` — Correction applied: `docs/ROADMAP.md:39,41` now state that syntax verification is complete, that the owner applies patch 1 (`.gitignore`) plus the revised patch 3 (`opencode.jsonc`), that `ai/README.md` stays untouched pending a separate release, that the Code-Mode denies are source-supported but runtime-unverified, that `opencode.worktree_remove` is an accepted limitation, that no broad `edit:* deny` is authorized, and that a fresh applied-state reviewer verdict is required. `docs/AGENT_TASKS.md:771` is marked as a pre-decision snapshot superseded by the decision at `:773`. No config, permission, product or publication file was changed. Re-review pending.

`documented` — Independent reviewer re-check (session `ses_df2403fb3ffeQJAOnU8s9587C7`, round 2) returned `PASS WITH FINDINGS` for `RV-P1-C3-C1`: 0 reviewer findings; prior `RV-P1-C3-REV-01` closed. Classification of this reviewer result: no `ACTIONABLE`, `OWNER_DECISION`, `NON_BLOCKING` or `UNKNOWN` reviewer finding. Case A applies: no correction required and none made, so no further review is started and the existing verdict is retained. The `PASS WITH FINDINGS` covers only the reviewed documentation correction state; it does not approve build/test results, implementation, Xbox compatibility, publication or permission changes.

`documented` — Open items recorded separately from defects under this verdict. `OWNER_DECISION`: (1) owner applies patch 1 (`.gitignore`); (2) owner applies revised patch 3 (`opencode.jsonc`); (3) `ai/README.md` stays untouched pending its separate owner release; (4) `opencode.worktree_remove` limitation accepted, residual exposure not a claimed fix. `UNKNOWN`: (1) runtime behavior of the four v2.4.1 tool-identity denies was not locally verified; (2) current `git diff --check` exit state is not exposed by the shell tool. `NON_BLOCKING`: none.

`documented` — `RV-P1-C3-C1` ist abgeschlossen. P1 ist lokal abgeschlossen, nachdem der Owner Patch 1 (`.gitignore`) und den überarbeiteten Patch 3 (`opencode.jsonc`) angewendet hat und der unabhängige Reviewer im angewendeten Zustand `PASS` erteilt hat (`ses_df1b60181ffek5mOBp1DHBgV5M`); die Publikation bleibt `OWNER_DECISION`. Die vorstehende Blockade ist historisch und wird durch den `XB-P2`-Eintrag unten superseded. Package 2 wurde vom Owner freigegeben und unter `XB-P2` gestartet; der Abschluss wird unten nach dem Review protokolliert.

## XB-P2 — Agentenarchitektur und effizienter Entwicklungsworkflow (pass / locally_complete)

PACKAGE_ID: XB-P2
PACKAGE_TYPE: FEATURE (documentation/architecture only)
PRODUCT_BOUNDARY: development-only
GOAL: Define a scalable, platform-neutral agent architecture and an efficient A–E workflow without runtime code.
FILES: `docs/AGENT_AUTONOMY.md`, `docs/AGENT_CORE_ARCHITECTURE.md`, `docs/OPENCODE_AUTOMATION_PLAN.md`, `docs/AUTOMATED_QUALITY_GATES.md`, `docs/GITHUB_AGENT_WORKFLOW.md`, `docs/DESKTOP_MIGRATION_CHECKLIST.md`; status records in `docs/AGENT_TASKS.md` and `docs/ROADMAP.md`.
ALLOWED: Coordinator as sole writer of the six docs; read-only architect; one bundled independent reviewer; documentation only.
FORBIDDEN: config/permission/agent-file/product/test/CMake/preset/AI README edits; publication; builds/tests for pure docs.
ACCEPTANCE: six complete, cross-consistent docs; A–E workflow binding; platform-neutral core separated from OpenCode adapter; security/owner boundaries; reviewer `PASS`/`NEEDS_CHANGE`/`BLOCKED` without actionable findings.
TESTS: read-back, exact Git status/HEAD/branch/diff checks; no artificial tests.
REVIEW: architect consistency check; one bundled independent reviewer; targeted re-check only for actionable findings.
WORKTREE: none; main checkout.
OWNER_APPROVALS: owner approved XB-P2 with this task; no new decision required for scoped status records.

`observed` — Pre-flight: HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; branch `docs/opencode-publication-group-2`; `git status --porcelain=v1 --untracked-files=all` showed the six new untracked docs plus the pre-existing working-tree entries; `git diff --check` emitted only the pre-existing `CMakePresets.json` CRLF warning. The coordinator created the six documents as sole writer and corrected localized artifacts during self-check. No build/CTest was run because this is a documentation-only package.

`documented` — Read-only architect session `ses_df1adc2afffeIpWUI0iYvcYFga` returned `ARCH NEEDS_CHANGE`: `ARCH-P2-01`, `ARCH-P2-02`, `ARCH-P2-03`, `ARCH-P2-05` classified `ACTIONABLE`; `ARCH-P2-04` classified `NON_BLOCKING`. One bundled correction was applied inside the six permitted docs: the applied-state reviewer session is now cited instead of an unrecorded verdict; the verdict vocabulary is aligned to `PASS`/`NEEDS_CHANGE`/`BLOCKED`; the coordinator write scope is corrected to twelve permitted docs; a logical-to-ledger state mapping was added; `opencode *` checks are marked owner-only. The ledger/roadmap alignment uses the coordinator's existing status-recording right (`docs/AGENT_WORKFLOW.md:173`), not a permission expansion. No config, permission, product, test, CMake, preset or `ai/README.md` file was edited. The final independent re-check is recorded below.

`documented` — Independent reviewer re-check (session `ses_df1a7c9d9ffejVBY4Gp0KjszlL`) returned `PASS`: 0 `ACTIONABLE`, 0 `OWNER_DECISION`, 0 `NON_BLOCKING` and 0 classified `UNKNOWN` findings; all five architect findings were closed, with no over-correction or new contradiction. Classification of this reviewer result: **Case A** — no correction required, none made, no second review. The verdict covers exactly the eight-file XB-P2 review scope; it does not approve build/test results, publication, runtime effectiveness of the four denies, JSONC parser validation, second-PC portability or Xbox compatibility. `XB-P2` is locally complete and awaiting the owner's publication decision.

## XB-0080-A/B — research register and workflow/ledger integration (implementation record)

PACKAGE_ID: XB-0080-A/B
PACKAGE_TYPE: FEATURE (development-only documentation)
PRODUCT_BOUNDARY: development-only
GOAL: Establish one authoritative, conformance-checked research register and connect it consistently to the agent workflow and ledger.
FILES: XB-0080-A = `docs/RESEARCH_REGISTER.md`; XB-0080-B = `docs/AGENT_WORKFLOW.md`, `docs/AGENT_TASKS.md`.
ALLOWED: researcher writes the register only; coordinator writes the workflow and ledger only; one bundled independent review of A+B; append-only status recording in this section.
FORBIDDEN: product code, tests, CMake, presets, build/CTest, CI, generator/tool, `opencode.jsonc`, `.opencode/**`, `AGENTS.md`, `ai/README.md`, `docs/ROADMAP.md`, `docs/DECISIONS.md`, `docs/ARCHITECTURE.md`, research files under `docs/research/**`, permission changes, staging, commit, publication, deletion or move.
ACCEPTANCE: AC-1 schema documented; AC-2 RESEARCH-007 register-level conformance; AC-3 no fake success; AC-4 boundary preserved; AC-5 independent review recorded. Existing AC definitions at the earlier "Checkable acceptance criteria" section retain priority; AC-2 is applied to all eight current register rows because the XB-0084 row postdates the original "seven rows" wording.
TESTS: documentation-only read-back, exact Git status/HEAD/branch/diff checks and `git diff --check`; no artificial tests and no build/CTest.
REVIEW: read-only architect model check before writing; one bundled independent reviewer after A+B; targeted bundled correction only for `ACTIONABLE` findings; reviewer does not edit or launch subagents.
WORKTREE: none; current checkout.
OWNER_APPROVALS: start release, A→B order, split write set and AC framework were supplied with the owner's current order. No new owner decision was created: the one architect-marked `OWNER_DECISION` is resolved by the in-scope option that preserves the six authoritative ledger `Status` tokens and adds fine-grained states as metadata. Publication remains owner-only.

`documented` — Effective write rights were re-verified before release: `researcher.md:8-10` allows `docs/RESEARCH_REGISTER.md`; `researcher.md:11-13` also allows `docs/research/**`, but that path is explicitly excluded by this package's `FORBIDDEN` list and was not edited. `coordinator.md:8-13` allows `docs/AGENT_TASKS.md` and `docs/AGENT_WORKFLOW.md`. Write sets are disjoint and ordered, so no parallel-write conflict exists. The reviewer is read-only. No effective right was expanded or bypassed.

`documented` — Architect model check (`ses_df19301d3ffetTvX1VUvYbMuLZ`) returned `ARCH NEEDS_CHANGE`. Classification by the coordinator: `ARCH-XB0080-01` was marked `OWNER_DECISION` by the architect but is resolved in-scope by keeping `AGENT_TASKS.md:5` and `AUTOMATED_QUALITY_GATES.md:84` authoritative and recording `IMPLEMENTATION_STATE` separately, per `AGENT_CORE_ARCHITECTURE.md:41-54`; the alternative of expanding the authoritative vocabulary is not chosen and would be a new owner decision. `ARCH-XB0080-02` through `ARCH-XB0080-08` are `ACTIONABLE` and were folded into the model before writing: historical XB-0085 publication-audit labels are not mapped; conformance is objective; source/reproduction and `not_applicable` rules are status-dependent and bounded; `documented` requires an authoritative/project-authored qualifier and the former `confidence` field is explicitly replaced; XB-0082 readiness has a fixed key order and token set; the coordinator-rights citation uses `coordinator.md:8-13`. `ARCH-XB0080-09` and `ARCH-XB0080-10` are `NON_BLOCKING`: the register says "pinned commit SHAs" and this section uses section anchors instead of the stale `AGENT_WORKFLOW.md` line references. No architect finding remains open.

`observed` — XB-0080-A handoff (`ses_df1904039ffebaGY8PN7kBewHZ`): sole writer changed `docs/RESEARCH_REGISTER.md` only; no web, shell, build, staging or publication. Read-back by the researcher reported 8 index rows, 8 detail blocks, 184 required-field lines (8 × 23), `Source` and `Reproduction` present in all 8, conformance distribution 8 `conformant` / 0 other, capability distribution 8 `unknown` / 0 `not investigated` / 0 `investigated-unsupported` / 0 `implemented-unvalidated` / 0 `validated`, and all three RESEARCH-007 pinned commit SHAs plus `2026-11-22` present. The coordinator independently read the file and confirmed stable anchors `## 2. Two independent axes`, `## 3. Required fields for every entry`, `## 4. Status-dependent source and reproduction requirements`, `## 5. Explicit mapping of legacy statuses`, `## 6. Conformance model`, `## 7. Compact index`, `## 8. Entry details` and `## 9. Assessment and audit notes`. A→B interface check **passed**: B consumes these section names, not line numbers, and needs no register change.

`observed` — XB-0080-B applied changes: `AGENT_WORKFLOW.md` gained the "Research register and evidence lifecycle" section with entry creation, conformance verification, status axes, the XB-P2 logical-to-ledger mapping, historical XB-0085 label disposition, and the evaluation-only machine-readable readiness convention; this ledger gained this implementation section. No generator, tool, CI job, dependency, percentage display or XB-0082 implementation was added.

```yaml
PACKAGE_ID: XB-0080-A
PARENT_ID: XB-0080
DEPENDENCIES: N/A
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: conformant
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

```yaml
PACKAGE_ID: XB-0080-B
PARENT_ID: XB-0080
DEPENDENCIES: XB-0080-A
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: conformant
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — Pre-review acceptance evidence: AC-1 is met by register §2.2 (five capability tokens with one use rule each), §3 (23 required fields) and §4 (mandatory Source/Reproduction with status-dependent explicit N/A reasons); AC-2 is met by §5 legacy mapping, §7/§8 full-row assessment and §9 provenance note, with RESEARCH-007 date, three pinned commit SHAs, URLs and reproduction pointer preserved and REC-01…REC-20 untouched in the recommendations file; AC-3 is met because §9 reports zero `validated` and zero `implemented-unvalidated` rows and class-4/class-5 items remain `unknown`; AC-4 is subject to the exact diff check recorded below; AC-5 remains pending until the independent reviewer verdict is appended.

`unknown` — The historical ledger table at `:9-20` still lists XB-0080 as `planned`, and `docs/ROADMAP.md` item 11 still says `planned / blocked by open owner decisions`. Both are historical anchors; editing the roadmap and the early table is outside this package's write set. The authoritative current state is this section. External web truth, license/legal effect and runtime behavior of any configuration remain `unknown`; no build or CTest was run for this documentation-only package.

`observed` — Exact verification commands and actual results run by the coordinator after both writes:
- `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`.
- `git branch --show-current` → `docs/opencode-publication-group-2`.
- `git status --porcelain=v1 --untracked-files=all` → 24 entries (17 modified, 7 untracked). Compared with the 22-entry pre-package state, no new untracked file was created by XB-0080-A/B; only the two previously clean tracked files `docs/RESEARCH_REGISTER.md` and `docs/AGENT_WORKFLOW.md` became modified in addition to the already-modified `docs/AGENT_TASKS.md`.
- `git diff --check; "EXIT=$LASTEXITCODE"` → only `warning: in the working copy of 'CMakePresets.json', CRLF will be replaced by LF the next time Git touches it`; `EXIT=0`.
- `git diff --stat -- docs/RESEARCH_REGISTER.md docs/AGENT_WORKFLOW.md docs/AGENT_TASKS.md` → `Permission denied: shell`. This exact form is not in the global exact-command allowlist at `opencode.jsonc:79-84`; it is recorded as a rejected attempt, not as a passed check.
- `git diff -- docs/RESEARCH_REGISTER.md docs/AGENT_WORKFLOW.md docs/AGENT_TASKS.md` → `Permission denied: shell`. Same reason; no result was inferred from it.
- `git diff HEAD --no-ext-diff --name-only` → executed successfully under the effective permission model; output listed 17 modified tracked files, including exactly `docs/RESEARCH_REGISTER.md`, `docs/AGENT_WORKFLOW.md` and `docs/AGENT_TASKS.md` from this package, plus 14 pre-existing modifications. Warning about `CMakePresets.json` CRLF was repeated.
- `git diff HEAD --no-ext-diff --stat` → executed successfully under the effective permission model; 17 tracked files, `942 insertions(+), 47 deletions(-)`. Package-relevant lines: `docs/AGENT_TASKS.md | 278 ++++++++++++++++++++++++++++++++++++`, `docs/AGENT_WORKFLOW.md | 47 +++++++`, `docs/RESEARCH_REGISTER.md | 353 +++++++++++++++++++++++++++++++++++++---`.
- `observed` — Command provenance: these two `git diff HEAD --no-ext-diff ...` forms are permitted by the coordinator agent rule at `coordinator.md:102-109`; they are not listed in the global exact-command allowlist at `opencode.jsonc:79-84`, which falls back to `shell "*" = ask` at `opencode.jsonc:76`. They are therefore recorded as successfully executed under the effective configuration, not as globally allowlisted and not as a saved-approval claim. This replaces the earlier inaccurate "allowlisted alternative" wording without changing any recorded output.
- `git diff --cached --name-only` → no output; the index is empty and nothing was staged.
- Full read-back of `docs/RESEARCH_REGISTER.md` (335 lines), `docs/AGENT_WORKFLOW.md` (new section at `## Research register and evidence lifecycle`) and this ledger section confirmed the applied text, fixed YAML key order and allowed tokens. No build or CTest command was run or claimed.

### XB-0080-C1 — bundled correction package (review round 1)

PACKAGE_ID: XB-0080-C1
PACKAGE_TYPE: BUGFIX (documentation accuracy only)
PRODUCT_BOUNDARY: development-only
GOAL: Correct the two evidence-accuracy defects found by the bundled reviewer without changing schema, status model or acceptance substance.
FILES: `docs/AGENT_TASKS.md` only; coordinator is sole writer.
ALLOWED: replace the inaccurate permission citation and command-provenance wording in the XB-0080-A/B implementation section; add this correction record.
FORBIDDEN: register or workflow content changes, product/test/CMake/preset/config/agent/permission edits, publication, build/CTest.
ACCEPTANCE: reviewer re-check confirms R-01 closed and R-02 wording accurate; no new finding.
TESTS: read-back of the two corrected passages; no build/CTest for documentation.
REVIEW: mandatory independent re-check by the same bundled reviewer after correction.
WORKTREE: none; current checkout.
OWNER_APPROVALS: none; mechanical correction inside the already approved package.

`documented` — Reviewer verdict (`ses_df18cb63dffe3BLVz4tT98Gy9z`) was `NEEDS_CHANGE` with R-01 MEDIUM `ACTIONABLE` and R-02 LOW `NON_BLOCKING`; 0 `OWNER_DECISION`, 0 classified `UNKNOWN`. Classification by the coordinator: R-01 `ACTIONABLE`; R-02 `NON_BLOCKING` but fixed in the same bundle because it is the same evidence-accuracy root cause in adjacent ledger text. Grouping produced one correction package for one writer. No severity was downgraded and no finding was left silently unresolved.

`observed` — Corrections applied: the write-rights sentence now cites `researcher.md:8-10` for the register allow and explicitly states that `researcher.md:11-13` additionally permits `docs/research/**` but is excluded by this package's `FORBIDDEN` list; the rejected `git diff` forms are described as not globally allowlisted; the two successful `git diff HEAD --no-ext-diff ...` forms are described as executed under the effective configuration via `coordinator.md:102-109`, not as globally allowlisted and not as a saved-approval claim. No command output, AC result, schema rule, status token or boundary statement changed. Re-check pending.

### XB-0080-A/B final result — locally complete, publication owner-only

`documented` — Review sequence completed: architect model check `ARCH NEEDS_CHANGE` → all findings folded in before writing → researcher wrote A → coordinator A→B interface check passed → coordinator wrote B → one bundled reviewer round 1 `NEEDS_CHANGE` (`ses_df18cb63dffe3BLVz4tT98Gy9z`: R-01 `ACTIONABLE`, R-02 `NON_BLOCKING`, 0 `OWNER_DECISION`, 0 classified `UNKNOWN`) → one bundled correction `XB-0080-C1` for the single writer → targeted independent re-check `PASS` with zero findings in the same session. Classification of the re-check: R-01 and R-02 closed; no new `ACTIONABLE`, `OWNER_DECISION`, `NON_BLOCKING` or `UNKNOWN` finding; Case A applies to the re-check and no second review was started. The two-round limit was respected.

`documented` — Final acceptance: AC-1, AC-2, AC-3, AC-4 and AC-5 are met. AC-5 is met by the independent reviewer verdict recorded above, not by coordinator self-acceptance. Both subpackages are `STATUS: pass`, `IMPLEMENTATION_STATE: done`, `REVIEW_STATUS: PASS`, `CONFORMANCE_STATUS: conformant` and `PUBLICATION_STATUS: local_only` in the fixed-key blocks above. The verdict covers only the three scoped files and the documented evidence model; it does not approve product changes, build/CTest results, runtime behavior, external web truth, license/legal effect, roadmap edits or publication.

`OWNER_DECISION` — Publication of this working-tree state, including any commit, stage, push, merge, pull request, tag or release, remains exclusively with the project owner. No agent performed any such action. The stale historical XB-0080 row at `:9-20` and roadmap item 11 remain unchanged because those paths are outside this package's write set; a separate owner-approved documentation package would be needed to update them.

`observed` — Final post-correction verification: `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git status --porcelain=v1 --untracked-files=all` → the same 24 entries (17 modified, 7 untracked), with no new untracked file from this package; `git diff --check; "EXIT=$LASTEXITCODE"` → only the pre-existing `CMakePresets.json` CRLF warning, `EXIT=0`; `git diff HEAD --no-ext-diff --stat` → 17 tracked files, `982 insertions(+), 47 deletions(-)`, with package lines `docs/AGENT_TASKS.md | 318 +++++`, `docs/AGENT_WORKFLOW.md | 47 +++++++`, `docs/RESEARCH_REGISTER.md | 353 +++++...---`; `git diff --cached --name-only` → no output. No build or CTest was run or claimed.

## XB-0085-CURRENT — refreshed read-only publication audit (XB-CONTINUATION-01)

PACKAGE_ID: XB-0085-CURRENT
PACKAGE_TYPE: RESEARCH (read-only publication preparation)
PRODUCT_BOUNDARY: development-only
GOAL: Re-evaluate the publication groups against the current HEAD instead of reusing the historical 32-entry snapshot.
FILES: no file is the object of a publication action; this record is appended to `docs/AGENT_TASKS.md` by the coordinator.
ALLOWED: read-only Git inspection and documentation read-back; append-only status recording in this ledger.
FORBIDDEN: staging, commit, push, pull, PR, merge, tag, release, branch/worktree change, history change, deletion, move, permission or config edits, build/CTest, and any use of historical counts as fresh verification.
ACCEPTANCE: every current working-tree entry is assigned to one of the three existing publication-audit labels; the basis is current Git output; remaining owner decisions are collected without being resolved by an agent.
TESTS: exact Git read commands plus read-back; no build or CTest for a read-only audit.
REVIEW: bundled independent review of the continuation work, not a self-accepted publication approval.
WORKTREE: none; current checkout.
OWNER_APPROVALS: all publication actions remain owner-only; no new permission or scope was created.

`observed` — Fresh commands and actual results: `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git status --porcelain=v1 --untracked-files=all` → 25 entries (18 modified, 7 untracked) at the C1 refresh; the original audit snapshot immediately before the tester edit was 24 entries (17 modified, 7 untracked); `git diff --cached --name-only` → no output; `git diff --check; "EXIT=$LASTEXITCODE"` → only the `CMakePresets.json` CRLF warning, `EXIT=0`; `git diff HEAD --no-ext-diff --stat` → 17 tracked files, `982 insertions(+), 47 deletions(-)`; `git log -5 --oneline` → `58aa1f6`, `e8ba492`, `cb04e03`, `7a9753f`, `af625df`. `git diff HEAD --no-ext-diff` was executed; `git diff HEAD --no-ext-diff --numstat` was rejected with `Permission denied: shell` and is recorded as a denied attempt, not bypassed. PR state and divergence from `main` remain `unknown` because agents do not run PR commands.

`observed` — Historical XB-0085 (`docs/AGENT_TASKS.md:546-552`, `:543`, `docs/ROADMAP.md:31`, `docs/PORTABLE_SETUP.md:109`) recorded 32 entries (13 `READY_FOR_COMMIT`, 18 `NEEDS_REVIEW`, 1 `DO_NOT_PUBLISH`) on branch `main` with HEAD `10e5e75`. That is a documented historical snapshot, not a current verification. At the C1 refresh the current tree has 25 entries (18 modified, 7 untracked, including the newly added `tests/test_main.cpp`); the historical per-entry list is not recoverable from the ledger, so the counts below are a fresh assignment to the 25 current entries and do not reuse or transform the historical numbers.

`documented` — Label meanings are preserved from XB-0085 and are publication-audit labels only, not ledger `Status` or `PUBLICATION_STATUS` tokens (`docs/AGENT_WORKFLOW.md:155`): `READY_FOR_COMMIT` = locally complete with an independent PASS for the substantive file state and no file-specific open owner blocker; `NEEDS_REVIEW` = changed but without an independent PASS for the exact current state or only a historical/partial PASS; `DO_NOT_PUBLISH` = private development infrastructure or owner-territory file that agents must not publish and that needs a separate owner decision. Every label still requires the owner's publication decision.

| Current entry | Label | Basis |
|---|---|---|
| `.gitignore` | READY_FOR_COMMIT | Owner patch 1 applied (`:16-19`); reviewer `PASS` in the applied state (`ses_df1b60181ffek5mOBp1DHBgV5M`, recorded at `:798`). |
| `docs/AGENT_WORKFLOW.md` | READY_FOR_COMMIT | XB-P2 eight-file `PASS` (`:819`); XB-0080-A/B `PASS` (`:908`); no edit after those reviews. |
| `docs/RESEARCH_REGISTER.md` | READY_FOR_COMMIT | XB-0080-A/B bundled `PASS` (`:908`); no edit after review. |
| `docs/ROADMAP.md` | READY_FOR_COMMIT | XB-0086-C2 `PASS` (`:594`) and XB-P2 status scope (`:819`); the formerly stale item 11 and sequence step (8) were corrected in C1 (`:35`, `:43`), so no stale XB-0080 anchor remains. The exact post-C1 state is in the bundled review scope. |
| `docs/PORTABLE_SETUP.md` | READY_FOR_COMMIT | XB-0086-C1 fifth-file final `PASS` (`:574`); no later edit by this package. |
| `docs/DEVELOPMENT_ENVIRONMENT.md` | READY_FOR_COMMIT | RV-P1-C2 final `PASS` for the correction state (`:714`); no later edit. |
| `docs/AGENT_AUTONOMY.md`, `docs/AGENT_CORE_ARCHITECTURE.md`, `docs/OPENCODE_AUTOMATION_PLAN.md`, `docs/AUTOMATED_QUALITY_GATES.md`, `docs/GITHUB_AGENT_WORKFLOW.md`, `docs/DESKTOP_MIGRATION_CHECKLIST.md` | READY_FOR_COMMIT | Six XB-P2 files in the eight-file independent `PASS` (`:806-819`). |
| `docs/AGENT_TASKS.md` | NEEDS_REVIEW | Append-only final-result and verification records at `:906-914` postdate the XB-0080-A/B re-check; no new substantive rule, but the exact current file state is not yet covered by a reviewer verdict. Included in the continuation bundled review. |
| `docs/ARCHITECTURE.md` | NEEDS_REVIEW | Modified in the earlier documentation cleanup (`:137`, `:681`); no independent `PASS` for the exact current file state was located. |
| `tests/test_main.cpp` | READY_FOR_COMMIT | Post-CTest refresh: after the owner applied the absolute `ctest.exe` allow, the real VS2026 Debug configure/build/CTest chain returned EXIT=0 (1/1 `xbridge_tests` passed, with the five new cases in that binary); the bundled final reviewer returned `PASS`/Case A for this exact evidence state. The earlier C1 `NEEDS_REVIEW` was based on the failed stale-cache run and is superseded. Publication remains owner-only. |
| `.opencode/agents/{architect,coordinator,developer,researcher,reviewer,tester}.md` | DO_NOT_PUBLISH | Private agent configuration; agents have no edit right there (`coordinator.md:53-55`) and no publication right at all (`docs/GITHUB_AGENT_WORKFLOW.md:9`). |
| `AGENTS.md` | DO_NOT_PUBLISH | Permanent versioning decision explicitly left open by XB-0086-C2 (Abschnitt `XB-0086-C2 — final status alignment and publication group 0 definition` in `docs/AGENT_TASKS.md`; die frühere Zeilenangabe `:588` ist veraltet). |
| `CMakePresets.json` | DO_NOT_PUBLISH | Machine-local VS2026 preset change with a CRLF warning and an excluded backup; not a product-package file, not re-verified by a fresh build in this audit. |
| `ai/README.md` | DO_NOT_PUBLISH | Private owner-territory documentation. The formerly open `OWNER_DECISION` RV-P1-08 (`:716`) is resolved: the three approved wordings at `:3,:13,:28` were applied and recorded at `:2411-2418` (read-back in this pass confirms them). It still stays unpublished because it is private development infrastructure, not because a patch is pending. |
| `opencode.jsonc` | DO_NOT_PUBLISH | Owner-territory permissions/configuration. The revised patch 3 was applied by the owner and independently reviewer-`PASS`ed (`:794`, `docs/DEVELOPMENT_ENVIRONMENT.md:164`); the older withdrawal note (`:677,:689-691`) is historical and superseded. It stays unpublished because it is private development infrastructure, not because a patch was withdrawn. |

`observed` — Counts at the C1 refresh: 12 `READY_FOR_COMMIT`, 3 `NEEDS_REVIEW`, 10 `DO_NOT_PUBLISH` = 25 current entries. The 24-entry snapshot at the start of this continuation was 12/2/10; the added `tests/test_main.cpp` moved to `NEEDS_REVIEW`. This is a fresh classification, not a correction of the historical 13/18/1 figures. `observed` — Post-CTest refresh after the owner-applied absolute CTest rule and the bundled reviewer `PASS`: 13 `READY_FOR_COMMIT`, 2 `NEEDS_REVIEW` (`docs/AGENT_TASKS.md`, `docs/ARCHITECTURE.md`), 10 `DO_NOT_PUBLISH` = the same 25 entries; only the `tests/test_main.cpp` basis changed from failed-verification to executed-PASS. No file was added or removed by this refresh.

`observed` — `XB-CHANGE-COMPLETION-FINAL-01` localized correction: the two stale basis statements at `:948` (claimed `ai/README.md` patch 2 unapplied) and `:949` (claimed `opencode.jsonc` patch 3 withdrawn) were corrected in place to match the applied, reviewer-`PASS`ed state recorded at `:794` and `:2411-2418`. Read-back confirmed `ai/README.md:3,13,28` and `docs/ARCHITECTURE.md:234` carry the approved wording; `docs/DEVELOPMENT_ENVIRONMENT.md:164` marks the old withdrawal note as historical. The `DO_NOT_PUBLISH` labels themselves are unchanged: both files remain private owner-territory development infrastructure regardless of patch state. No other file was changed by this correction; the publication-audit counts at `:951` are historical snapshots and were not recomputed.

`documented` — Recommended publication order, without authorizing any publication action: (1) the 13 `READY_FOR_COMMIT` files as the first group (12 at the C1 refresh plus `tests/test_main.cpp` after the executed CTest PASS), preserving the five-file `publication group 0` definition in the section `XB-0086-C2 — final status alignment and publication group 0 definition` as a subset; (2) `docs/AGENT_TASKS.md` after the continuation bundled review covers `:906-914`; (3) `docs/ARCHITECTURE.md` after a targeted review of its current diff; (4) the `DO_NOT_PUBLISH` set never automatically — each remains owner-territory or owner-decision territory. Dependencies: the ledger must be reviewed before publication because it records the verdicts for the other files; the six XB-P2 documents are self-contained; the private configuration files do not belong to any product package.

`OWNER_DECISION` — Collected decisions: (a) whether to publish the 12-file ready group; (b) `ai/README.md` patch 2 was applied and independently reviewer-`PASS`ed (`:2411-2418`); the remaining decision is only whether to publish this private owner-territory file at all (recommended: keep it unpublished); (c) the permanent versioning decision for `AGENTS.md`; (d) treatment of the local `CMakePresets.json` change and its excluded backup; (e) current PR #1 state, branch strategy and merge into `main`, all `unknown` to agents; (f) worktree cleanup of the unregistered checkout; (g) formerly stale `docs/ROADMAP.md` item 11 and historical XB-0080 table row — C1 corrected the roadmap status/sequence text (`:35`, `:43`); whether the historical ledger table row itself needs a separate owner patch remains an owner call. Recommended default: publish only owner-selected development-infrastructure files, keep configuration and machine-local files out, and perform no merge or PR action until the owner explicitly decides.

`unknown` — Current PR state, 29-commit divergence, license/legal effect of cited research, second-PC portability, interactive TUI dialogs and runtime behavior of configuration denies were not re-verified. No historical result is presented as current verification.

## XB-X64-CORPUS-01 — documented-contract test hardening (XB-CONTINUATION-01)

PACKAGE_ID: XB-X64-CORPUS-01
PACKAGE_TYPE: FEATURE (test-only conformance hardening)
PRODUCT_BOUNDARY: product (test file only; no runtime behavior change)
GOAL: Cover documented x64 decoder/interpreter boundary and failure contracts with the installed tester, without changing product behavior or public interfaces.
FILES: `tests/test_main.cpp` only. Sole writer: installed `tester`; `developer` is explicitly not a writer for this file.
ALLOWED: add tests using the existing `check` harness; preserve every existing test; assert only contracts documented in `docs/ARCHITECTURE.md:115-129,153-163`.
FORBIDDEN: product source/header edits, `CMakeLists.txt`, `CMakePresets.json`, config/agent/docs edits, deletion or weakening of tests, new dependencies, JIT, new opcode families, FLAGS/SIMD/SIB/displacement semantics, and any assertion about `length` on `InvalidEncoding` (undocumented incidental behavior).
ACCEPTANCE: new cases cover REX.W/R/B extended-register selection (R8–R15), REX.X recognized-but-unused, truncated input with REX and missing ModR/M, and every non-`Ok` decode status as a no-op with unchanged RIP/registers/memory and no exception; all pre-existing tests remain; exact build/CTest/diff commands are executed with real results reported.
TESTS: `cmake --preset windows-debug`; `cmake --build --preset windows-debug --config Debug`; `ctest --test-dir build/windows-debug -C Debug --output-on-failure`; `git diff --check`. The single existing CTest target means the full binary runs; no targeted `ctest -R` is claimed.
REVIEW: architect skipped for implementation (no interface, module boundary, dependency or ADR change; read-only technical map already returned `ARCH PASS` bounded); one bundled independent reviewer covers XB-0085-CURRENT, the roadmap correction and this test package.
WORKTREE: none; current checkout.
OWNER_APPROVALS: the owner released this tests-only package immediately in `XB-CONTINUATION-01`; normal envelope approval is therefore recorded. Publication remains owner-only.

`documented` — Authority: `AGENTS.md:84-86` requires boundary/failure tests; `RESEARCH-007_RECOMMENDATIONS.md:24,37,81` assigns installed `tester` for untrusted/truncated input and boundary contracts with no product change. Effective right verified: `tester.md:5-10` allows `tests/**`, and this envelope explicitly names `tester` as the sole writer of `tests/test_main.cpp` as required by `tester.md:81-82`. `developer.md` grants `tests/**` too but is not assigned here, so no parallel write conflict exists.

`documented` — Pre-release findings consolidated: inventory (`ses_df17930f6ffdZyLRjGeHjmrYt8`), architecture (`ses_df17930f6ffeoP4pHBZMRQh0R1`, `ARCH PASS` bounded), feasibility (`ses_df17930f4ffew0lbi0f6doF4Or`) and verification gaps (`ses_df17930f2ffezcfgWyGYPRodh4`). All agree that product-code, Agent Core, GTA VI, RIP-bridge and host-provider implementations require separate owner decisions; only this test-only package was executable now.

`observed` — Writer handoff and exact verification results (`ses_df171e7eeffe4js8Xt34Buic0I`): `tests/test_main.cpp` was the only changed file, additions only — `test_x64_decoder_rex_extended_registers` (`:1216-1307`), `test_x64_interpreter_rex_extended_registers` (`:1309-1388`), `test_x64_decoder_rex_x_recognized_unused` (`:1390-1437`), `test_x64_decoder_truncated_rex_modrm` (`:1439-1455`), `test_x64_interpreter_non_ok_decode_noop` (`:1457-1513`), registrations (`:1622-1626`). `cmake --preset windows-debug` → **exit 1** (`CMakeCache.txt` directory `D:/XBridge 1.0/...` differs from `e:/XBridge 1.0/...` where it was created; source-path mismatch). `cmake --build --preset windows-debug --config Debug` → **exit 1** (same cache error; generator `Visual Studio 18 2026` could not locate `E:/PRO/Programm Daten/Visual Studio/18/Community`). `ctest --test-dir build/windows-debug -C Debug --output-on-failure` → **exit 1** (`Unable to find executable: E:/XBridge 1.0/.../xbridge_tests.exe`; `1/1 Test #1: xbridge_tests ... ***Not Run`; `0% tests passed`). `git diff --check` → only the pre-existing `CMakePresets.json` CRLF warning, no error line; exit code **unknown** because the exit-code-capture variant was denied. `git status --porcelain=v1 --untracked-files=all` → 25 entries (18 modified including `M tests/test_main.cpp`, 7 untracked); `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git diff --cached --name-only` → no output. Denied, not bypassed: `git diff -- tests/test_main.cpp`, `git diff HEAD --no-ext-diff`, `cmake -S . -B build/windows-debug-verify-d -G "Visual Studio 18 2026" -A x64`, exit-code-capture variant. The complete diff was inspected with the allowed `read` tool (`tests/test_main.cpp:1200-1639`): exactly two insertion blocks, no other change.

`observed` — Root cause of the verification failure, confirmed by read-back in C1: `build/windows-debug/CMakeCache.txt:62,198,204,214,263` still contains `E:/XBridge 1.0/XBridge-Series-Github/...` paths from the previous checkout; `CMakePresets.json:5-9` uses `${sourceDir}` with generator `Visual Studio 18 2026`. The tester permission file (`tester.md:20-67`) allowlists only exact command strings and does not permit a fresh configure or an alternate diff form, so the stale cache cannot be repaired by the assigned writer. No build or CTest pass is claimed; compile and runtime results of the new tests are `unknown`.

`observed` — Bundled round-1 reviewer verdict (`ses_df16e3a75ffeJg0HIeGf1y64xC`): `NEEDS_CHANGE`, 3 findings — `XB-CONT-01-R01` HIGH/`ACTIONABLE` (failed verification not recorded, package without status axes), `XB-CONT-01-R02` MEDIUM/`ACTIONABLE` (publication audit omitted the post-tester 25th entry), `XB-CONT-01-R03` LOW/`NON_BLOCKING` (compound envelope tokens). The reviewer confirmed the tests are faithful to documented contracts, non-weakening and confined to the sole tester writer, but never compiled or executed; runtime success is `unknown`. All three findings were grouped into this single bundled correction `XB-CONTINUATION-01-C1`; an independent re-check is mandatory.

```yaml
PACKAGE_ID: XB-X64-CORPUS-01
PARENT_ID: XB-CONTINUATION-01
DEPENDENCIES: N/A
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — Status change basis: the owner applied the exact absolute `ctest.exe` allow at `.opencode/agents/tester.md:71-73` and restarted OpenCode. The tester then ran `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"` → `Start 1: xbridge_tests`, `1/1 Test #1: xbridge_tests ... Passed 0.05 sec`, `100% tests passed, 0 tests failed out of 1`, `Total Test time (real) = 0.12 sec`, `EXIT=0`. Found/executed/passed/failed/Not Run = 1/1/1/0/0. Together with the earlier absolute CMake `--version`, `--fresh --preset windows-debug` and `--build --preset windows-debug --config Debug` results (all `EXIT=0`) and the harness read-back (`tests/test_main.cpp:21-30,1622-1626,1749-1754`), this satisfies the ACCEPTANCE at `:972-973`. Bundled independent reviewer `ses_df1120fd9ffeXNrOLLH95PvD6T` returned **PASS, Case A** (0 `ACTIONABLE`, 0 `OWNER_DECISION`, 0 `NON_BLOCKING`, 0 `UNKNOWN`); no correction round was required. `documented` — `STATUS: pass` is local verification only; `PUBLICATION_STATUS: local_only` and owner-only publication are unchanged. The earlier `needs_owner` entries above are historical snapshots taken when configure/build/CTest failed with zero executed tests.

## XB-CONTINUATION-01 — consolidated open-package overview and dependency plan

PACKAGE_ID: XB-CONTINUATION-01
PACKAGE_TYPE: RESEARCH
PRODUCT_BOUNDARY: development-only
GOAL: Refresh publication state, remove the stale roadmap contradiction, and inventory/prepare the only executable technical package without inventing owner approvals.
FILES: `docs/AGENT_TASKS.md`, `docs/ROADMAP.md`, `tests/test_main.cpp` under separate explicit envelopes.
ALLOWED: append-only status records; one coordinator-owned roadmap correction; one tester-owned test file.
FORBIDDEN: product runtime code, CMake/preset changes, config/agent/permission edits, publication, build claims without execution, new dependencies, JIT, Xbox compatibility claims.
ACCEPTANCE: every open package classified; dependencies and priorities recorded; XB-0085 refreshed; stale roadmap anchor corrected; tests-only package executed with real commands; one bundled independent review; all owner decisions collected once.
TESTS: see XB-X64-CORPUS-01 envelope; documentation-only checks for the ledger/roadmap.
REVIEW: one bundled independent reviewer over all continuation artifacts after the writer reports a stable state.
WORKTREE: none; current checkout.
OWNER_APPROVALS: continuation order received; immediate release of the tests-only package received; all other decisions remain open and are collected below.

`documented` — C1 scope note (bundled correction, reviewer finding `XB-CONT-01-R03`): this coordination envelope uses single allowed tokens `RESEARCH`/`development-only`; the test-only implementation is tracked separately under `XB-X64-CORPUS-01` with its own `product` boundary. The mixed-scope explanation is prose only, so read-back token validation succeeds (`docs/AGENT_WORKFLOW.md:21-22`).

`documented` — Dependency plan (1 = blocks other work, 2 = architecture/interface foundation, 3 = approved technical foundation, 4 = automation/QA, 5 = functional implementation, 6 = integration, 7 = documentation/publication):
1. Owner publication decision and branch strategy block every Git/GitHub action but block no local documentation or test work.
2. `XB-0084` owner decision register and ADR/license gates block all GTA VI implementation, host-provider expansion and any public-interface change.
3. `SETUP-007` blocks specialist installation; installed `developer`/`tester` substitutes require an approved envelope per package; this does not block the current tests-only package.
4. `XB-0082` blocks only automated progress tooling, not the existing document convention.
5. `XB-X64-CORPUS-01` is executable now and strengthens the documented interpreter baseline; it does not unlock product code by itself.
6. `XB-X64-STEP-01` (new x64 single-step façade) and `XB-HOST-CAP-01` (capability-provider seam) are fully prepared but owner-gated because they add public interfaces.
7. XB-0085-CURRENT, roadmap correction and final reporting proceed in parallel with (5) and do not depend on owner decisions.

`observed` — Open-package inventory (authoritative detail remains in the cited records):
| ID | Current state | Dependencies / gate | Next concrete step | Evidence |
|---|---|---|---|---|
| `XB-0082` | `planned`, evaluation-only convention implemented in `docs/AGENT_WORKFLOW.md:157-174` | Owner decision on split write set, taxonomy and any tool/CI/dependency | Owner decides; then approved package | `docs/AGENT_TASKS.md:241-252,310,338,441,461` |
| `XB-0083` | `planned`, not started | Start package; fresh web truth `unknown` | Approve researcher package with exact URLs/access dates | `docs/AGENT_TASKS.md:254-265,311,337` |
| `R-001`…`R-006` | Register rows `unknown`, conformant, no package assigned | Researcher package per row; no product change | Create envelopes if the owner wants evidence | `docs/RESEARCH_REGISTER.md:111-116,122-276` |
| `RESEARCH-007` / `XB-0084` carry-over | Research/planning `pass`; capability `unknown` | Owner/legal/ADR decisions before adoption | Resolve decision registers | `docs/RESEARCH_REGISTER.md:278-328`; `XB-0084_RISK_DEPENDENCY_OVERVIEW.md:108-129` |
| GTA VI implementation | Not approved | Separate owner approval after XB-0084 decisions | Prepare only; no code | `docs/ROADMAP.md:37` |
| `XB-X64-STEP-01` | Prepared, not started | Owner approval of new public interface and installed writer set | Owner decides or chooses private-helper alternative | architect result `ses_df17930f6ffeoP4pHBZMRQh0R1` §3.3 |
| `XB-HOST-CAP-01` | Prepared, not started | Owner approval of provider interface and Windows-probing scope | Owner decides | architect result §3.4 |
| `SETUP-004` | `needs_owner`; owner-only session export/import | Owner executes supported commands and reports | No agent action | `docs/AGENT_TASKS.md:15,142-153` |
| `SETUP-006` | `blocked`; failed worktree opening, unregistered checkout | Owner cleanup/further-attempt decision | No agent action | `docs/AGENT_TASKS.md:17,80-82,101` |
| `SETUP-007` | `blocked`; nine specialists not installed | Owner persists scoped rules and installs agents | No agent action | `docs/AGENT_TASKS.md:18,103-106,272-288` |
| `ai/README.md` patch | Applied and verified under `XB-OWNER-DOC-IMPLEMENTATION-02` (`PERMISSION_STATE: verified`, `REVIEW_STATUS: PASS`) | none (historical F1 framing; authoritative current policy at file end) | Historical snapshot: routine publication is now coordinator-authorized after checks and review | `docs/AGENT_TASKS.md:636-645,2405-2467`; `XB-AUTONOMY-ACTIVATION-03` |
| Publication of all local `pass` work | Historical snapshot: locally complete and unpublished at that time; `XB-AUTONOMY-ACTIVATION-02` was committed and pushed, other packages remain `local_only` | historical F1 framing; authoritative current policy at file end | Historical snapshot: routine publication is now coordinator-authorized after checks and review | `docs/GITHUB_AGENT_WORKFLOW.md:7-9,49`; `XB-AUTONOMY-ACTIVATION-03` |

```yaml
PACKAGE_ID: XB-0082
PARENT_ID: N/A
DEPENDENCIES: N/A
STATUS: needs_owner
IMPLEMENTATION_STATE: needs_owner
REVIEW_STATUS: not_reviewed
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: OWNER-XB0082-SCOPE
PROGRESS_UNITS: N/A
```

`documented` — Single owner-decision list (question → recommendation → alternatives → impact → urgency → work that can continue):
- `OWNER-PUBLICATION`: Which files/groups should be staged, committed and published, and what happens to PR #1? Recommendation: owner selects the 12 `READY_FOR_COMMIT` development documents only; alternatives: publish nothing, publish a narrower group, or publish more after review; impact: irreversible publication state; urgency: high only when owner wants publication; local documentation/test work continues. `documented` — historical F1 framing from `XB-0085`; the authoritative current policy is appended at the end of this file. Routine staging/commit/push/PR/merge of independently reviewed work is coordinator-authorized; PR #1 handling and any tag/release/strategic-branch/history decision remain owner decisions.
- `OWNER-XB0082-SCOPE`: Approve split write set, status taxonomy and any future tool/CI/dependency for XB-0082? Recommendation: keep the current evaluation-only convention and defer tooling; alternatives: approve a generator/tool package; impact: new dependency/CI surface; urgency: medium; all other work continues.
- `OWNER-XB0083-START`: Release a researcher package for authoritative re-verification (R-001…R-006, XB-0083)? Recommendation: approve a scoped read-only package with exact URLs and access dates; alternatives: defer; impact: evidence quality only; urgency: medium; tests and documentation continue.
- `OWNER-XB0084-DECISIONS`: Resolve the seven XB-0084 decision-register items (baseline, license policy, ADRs, native/JIT policy, Xbox 360 scope, performance publication, FINAL_CLEANUP)? Recommendation: decide in the recorded order; alternatives: defer each; impact: gates all product implementation; urgency: medium; tests and cleanups continue.
- `OWNER-X64-STEP`: Approve `XB-X64-STEP-01` public interface or choose a private helper / defer? Recommendation: approve after the current test package produces evidence; alternatives: private-only seam or defer; impact: new public API; urgency: low-medium; tests-only work continues.
- `OWNER-HOST-CAP`: Approve `XB-HOST-CAP-01` interface and Windows-probing scope? Recommendation: deterministic mock provider first, real probing later; alternatives: defer; impact: host/guest boundary; urgency: low; tests-only work continues.
- `OWNER-SETUP-004/006/007`: Execute session transfer, decide worktree cleanup, install specialists? Recommendations: owner runs export/import manually; remove or keep the unregistered checkout by owner choice; keep six installed roles for now and repackage work with installed substitutes; urgency: medium; local documentation/test work continues.
- `OWNER-AI-README`: Apply or permanently reject the prepared `ai/README.md` patch? Recommendation: apply before broad publication; alternatives: leave unapplied and exclude from publication; urgency: low; other work continues.
- `OWNER-X64-VERIFY`: How should the stale `E:`-drive CMake cache in `build/windows-debug/CMakeCache.txt` be handled so the declared VS2026 configure/build/CTest verification of `XB-X64-CORPUS-01` can actually run? Recommendation: owner removes or regenerates the stale cache/build directory outside an agent run, then an approved tester run repeats exactly the three declared commands; alternatives: keep the package `needs_owner` with tests unexecuted, or grant a scoped fresh-configure command; impact: only build outputs under `build/**`, no new dependency and no product-code change; urgency: medium, because it is the only executable package and its compile/runtime result remains `unknown`; all other documentation/publication-preparation work continues unaffected. `observed` — The technical part of this decision is now satisfied: the owner applied the absolute CMake and CTest rules, and the tester's configure/build/CTest chain returned `EXIT=0` with 1/1 tests passed; `XB-X64-CORPUS-01` is `pass` under `XB-CMAKE-VERIFY-RESUME`. The publication, configuration and scope alternatives in this decision remain owner territory and are not re-opened by this local closure.

`unknown` — No owner decision above was silently resolved by an agent. No build, CTest, PR or publication result is treated as current verification unless a later section records the exact executed command and result.

## XB-CONTINUATION-01-C1 — bundled correction and final review result

`observed` — One bundled correction package `XB-CONTINUATION-01-C1` was applied by the coordinator as sole writer of `docs/AGENT_TASKS.md` and `docs/ROADMAP.md`; writer sets stayed disjoint from the tester's `tests/test_main.cpp`. It closed all three round-1 findings by root cause: R01 (record every failed command/exit, denied forms and stale-cache root cause; assign `needs_owner` axes; add `OWNER-X64-VERIFY`; fix roadmap step 8 and the two follow-on stale anchors), R02 (refresh the audit to 25 entries with a `tests/test_main.cpp` NEEDS_REVIEW row and 12/3/10 counts), R03 (single allowed envelope tokens with prose scope note).

`observed` — Mandatory targeted re-check by the same independent reviewer (`ses_df16e3a75ffeJg0HIeGf1y64xC`): verdict `PASS`, **zero findings** (`ACTIONABLE` 0, `OWNER_DECISION` 0, `NON_BLOCKING` 0, `UNKNOWN` 0). Case A applies: no further correction, no second review. The reviewer confirmed R01/R02/R03 resolved and closed, the failed verification honestly recorded, `XB-X64-CORPUS-01` never marked `pass`, the XB-0082 yaml block compliant, and no publication/staging/commit or config/agent/permission/product-source/CMake edit by C1. Reviewer limitation: no shell/git tool in its catalog, so Git facts and raw command output were assessed from the recorded ledger, not independently re-run.

```yaml
PACKAGE_ID: XB-CONTINUATION-01-C1
PARENT_ID: XB-CONTINUATION-01
DEPENDENCIES: N/A
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

`observed` — Final coordinator verification after all edits: `git status --porcelain=v1 --untracked-files=all` → 25 entries (18 modified including `tests/test_main.cpp`, 7 untracked); `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git diff --cached --name-only` → no output; `git diff --check; "EXIT=$LASTEXITCODE"` → only the pre-existing `CMakePresets.json` CRLF warning, `EXIT=0`. No build or CTest was run or claimed by this continuation; no staging, commit, PR, merge, tag, release, worktree or publication action occurred. `STATUS: pass` above applies only to the coordinator's bundled documentation correction and its independent re-check, not to `XB-X64-CORPUS-01`, which remains `needs_owner` under `OWNER-X64-VERIFY` because its declared configure/build/CTest commands failed.

## XB-X64-VERIFY — owner-authorized cache repair and verification attempt

PACKAGE_ID: XB-X64-VERIFY
PACKAGE_TYPE: BUGFIX
PRODUCT_BOUNDARY: development-only
GOAL: Repair or route around the stale E:-drive CMake cache so the five new tests in tests/test_main.cpp are actually compiled and executed, and set XB-X64-CORPUS-01 to pass only on real evidence. This is local build verification with no product change.
FILES: no source file; only generated build outputs under build/** may be created. tests/test_main.cpp is explicitly read-only in this package.
ALLOWED: read-only inspection; backup of the stale cache if permission permits; fresh configuration/build/CTest via allowlisted commands; tester as sole executor.
FORBIDDEN: editing tests/test_main.cpp or any product/CMake/preset/toolchain file; bypassing denied permissions; deletion/move without backup; any Git publication action; config/agent/permission edits; software installation.
ACCEPTANCE: configure, build and CTest actually exit 0 with the xbridge_tests target executed; independent reviewer PASS; status changed only on that evidence.
TESTS: exact allowlisted windows-debug commands plus the independent ninja-debug route; exact commands and exit states recorded.
REVIEW: exactly one bundled independent reviewer after execution; targeted re-check only for real findings.
WORKTREE: none; current checkout.
OWNER_APPROVALS: the owner issued this verification task; publication remains a separate OWNER-PUBLICATION decision.

`observed` — Phase A basis re-read by the coordinator: HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`, branch `docs/opencode-publication-group-2`, 25 working-tree entries, empty index. `build/windows-debug/CMakeCache.txt:2-3,198,204,214,263` still contains `E:/XBridge 1.0/XBridge-Series-Github` and `E:/PRO/Programm Daten/Visual Studio/18/Community`; current environment is `D:` with VS at `C:\Program Files\Microsoft Visual Studio\18\Community` (`docs/DEVELOPMENT_ENVIRONMENT.md:67-70,86-87`). `opencode.jsonc:147-150` hard-denies `Remove-Item`, `Move-Item`, `rm`, `del`; `Copy-Item` is in no agent allowlist. Per the owner's instruction, a denial must be documented, not bypassed; exact command results are pending from the tester.

`observed` — Tester execution handoff (`ses_df1613fe3ffekpwe3unWKpN5ws`). Phase A, all exit 0: `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git status --porcelain=v1 --untracked-files=all` → 25 entries; `git diff --cached --name-only` → empty; `cmake --version` and `ctest --version` → `4.3.1-msvc1` each.

`denied` — Phase B: `Copy-Item -Path build/windows-debug/CMakeCache.txt -Destination build/windows-debug-cache-backup-20261124.txt; "EXIT=$LASTEXITCODE"` → `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}`. No backup created; no retry, no alternate form, no other tool, nothing deleted or moved. Cache repair is therefore not executable by any installed agent; this is the concrete blocker for the owner.

`observed` — Phase D1 (`windows-debug`, exact allowlisted commands, cache untouched): `cmake --preset windows-debug` → **exit 1** (CMakeCache directory `D:/…` differs from `e:/…`; source `D:/…/CMakeLists.txt` does not match `E:/…`); `cmake --build --preset windows-debug --config Debug` → **exit 1** (same cache mismatch; generator `Visual Studio 18 2026` could not find `E:/PRO/Programm Daten/Visual Studio/18/Community`); `ctest --test-dir build/windows-debug -C Debug --output-on-failure` → **exit 8** (`Unable to find executable: E:/…/xbridge_tests.exe`, `1/1 Test #1: xbridge_tests ... ***Not Run`, `0% tests passed, 1 tests failed out of 1`).

`observed` — Phase D2 (`ninja-debug`): the coordinator's Phase-A assumption that this build directory was cache-free was **wrong**. `build/ninja-debug/CMakeCache.txt:2,76,222,277` exists and is stale on `E:` (generator `Ninja`, `CMAKE_MAKE_PROGRAM` at `E:/PRO/…/Ninja/ninja.exe`), independently re-read by the coordinator. `cmake --preset ninja-debug` → **exit 1** (same D:/e: mismatch); `cmake --build --preset ninja-debug` → **exit 1** (`no such file or directory`, build tool `E:/PRO/…/ninja.exe`); `ctest --preset ninja-debug --output-on-failure` → **exit 8** (`Unable to find executable: E:/…/xbridge_tests.exe`, `1/1 ... ***Not Run`, `0% tests passed, 1 tests failed out of 1`). No compiler invocation succeeded; the five new test functions were not compiled or executed.

`observed` — Final Git state byte-identical to Phase A: 25 entries, empty index, `tests/test_main.cpp` not edited by the tester. No build directory was created or changed by the tester; configure/build failed before generation, CTest was read-only. Outcome: **BLOCKED, not passed**. `XB-X64-CORPUS-01` remains `needs_owner`; the status is not changed on this evidence.

`observed` — One bundled independent final review (`ses_df15f87ddffeuFzakoAjTPaMpt`): verdict **`PASS` with zero findings** (`ACTIONABLE` 0, `OWNER_DECISION` 0, `NON_BLOCKING` 0, `UNKNOWN` 0); Case A applies, no correction, no second review. The reviewer independently re-read both caches (stale `E:` paths confirmed), all five test definitions and their single `main()` registrations (intact, harness unchanged), the CTest `***Not Run`/`0%` outputs, the verbatim `Copy-Item` denial plus the hard-deny rules, and the unchanged `needs_owner` axes. Reviewer limitation: no shell/Git tool in its catalog, so Git facts and command re-execution were assessed from file state and recorded output, not re-run.

`documented` — Scope of that `PASS`: it certifies only the honesty and accuracy of the XB-X64-VERIFY record (failure reported, denial documented without bypass, wrong ninja-cache assumption corrected, no status changed on insufficient evidence). It does **not** certify build or CTest success, compile/runtime correctness of the five tests, external truth, or publication. The reviewer's explicit conclusion: `XB-X64-CORPUS-01` may **not** be set to `pass`; its ACCEPTANCE (configure/build/CTest exit 0 with xbridge_tests executed) is unmet (1/1/8, 0 tests executed), so it stays `needs_owner` under `OWNER-X64-VERIFY`.

## XB-CONTINUATION-02 — build-unblock attempt, current verification, and maximum released work

PACKAGE_ID: XB-CONTINUATION-02
PACKAGE_TYPE: RESEARCH
PRODUCT_BOUNDARY: development-only
GOAL: Attempt the owner-authorized build unblock, re-verify XB-X64-CORPUS-01 with current commands, and complete every released nonblocked package with one bundled final review.
FILES: `docs/AGENT_TASKS.md`, `docs/ROADMAP.md` (coordinator only). No product, test, CMake, preset, config or agent file was edited.
ALLOWED: read-only inspection; exact allowlisted tester commands; append-only ledger and roadmap records; complete drafted envelopes for owner-gated packages.
FORBIDDEN: editing `opencode.jsonc` or `.opencode/**`; bypassing denied permissions; product code without interface approval; weakening tests; cache deletion/move; Git publication; build claims without real execution.
ACCEPTANCE: current configure/build/CTest results recorded with exact exit codes; the Package A blocker precisely prepared for the owner; all open packages consolidated; complete D/E envelopes recorded; XB-X64-CORPUS-01 status kept honest; exactly one independent final review.
TESTS: exact allowlisted commands only (current-state results below); no build/CTest pass claimed.
REVIEW: exactly one bundled independent reviewer after all writes; targeted re-check only for real ACTIONABLE findings.
WORKTREE: none; current checkout.
OWNER_APPROVALS: the permission expansion was authorized by the owner in this order but is NOT executable by agents (see Package A below); every other decision remains open as collected at `:1058-1067`.

`observed` — Phase A (one consolidated pass): `git rev-parse HEAD` → `58aa1f60eb5e67cbf8aa884f694fce208b54819f`; `git branch --show-current` → `docs/opencode-publication-group-2`; `git status --porcelain=v1 --untracked-files=all` → 25 entries (18 modified, 7 untracked); `git diff --cached --name-only` → empty; `git diff --check` → only the known `CMakePresets.json` CRLF warning. `CMakePresets.json:5-19` uses `${sourceDir}` with generator `Visual Studio 18 2026` (`:7`) and `Ninja` (`:14`) — no hard-coded toolchain path. Both caches remain stale on `E:` (`windows-debug/CMakeCache.txt:2-3,198,204,214,263`; `ninja-debug/CMakeCache.txt:2,76,222,277`).

### Package A — permission expansion authorized but NOT executable by agents (OWNER_DECISION)

`observed` — The owner authorized a narrowly scoped permission expansion for `Copy-Item`, `cmake --fresh --preset windows-debug`, the debug build, its CTest call and an optional ninja route. Effective rights make this NOT executable by any installed agent: (1) `coordinator.md:50-55` denies `edit` on `opencode.jsonc` and `.opencode/**`; (2) `cmake --fresh` and `Copy-Item` are in no allowlist (`opencode.jsonc:73-96`, `tester.md:23-67`); (3) `opencode.jsonc:147-150` hard-denies `Remove-Item`, `Move-Item`, `rm`, `del` (top-level policies cannot be granted by a rule). No denial was bypassed and no config file was edited.

`denied` — Probe by tester (`ses_df1585811ffeqeFNdU5kAznCmD`): `cmake --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"` → `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}`, before cmake started; no exit code exists. Both caches and both file contents unchanged; no `build/*-cache-backup*` exists; `.owner-patch-backup/` does not exist yet (but is already excluded at `.gitignore:17`). Fresh configuration is therefore impossible for agents in this session.

`documented` — Minimal owner patch (prepared, NOT applied). The following block is illustrative manual-edit guidance, **not** a tool-applicable unified diff: hunk headers and context are abbreviated, so `git apply` is not expected to succeed; the owner inserts the two `+` lines at the cited locations by hand. The owner performs these edits and reloads OpenCode; whether a reload is required for the new rules to take effect is `unknown`.
```diff
--- a/opencode.jsonc
+++ b/opencode.jsonc
@@ -89,3 +89,5 @@
      { "action": "shell", "resource": "cmake --preset windows-debug", "effect": "allow" },
+     { "action": "shell", "resource": "cmake --fresh --preset windows-debug", "effect": "allow" },
+     { "action": "shell", "resource": "Copy-Item -Path build/windows-debug/CMakeCache.txt -Destination .owner-patch-backup/windows-debug-CMakeCache.txt", "effect": "allow" },
--- a/.opencode/agents/tester.md
+++ b/.opencode/agents/tester.md
@@ -46,3 +46,4 @@
      effect: allow
+  - action: shell
+    resource: "cmake --fresh --preset windows-debug"
+    effect: allow
```
`documented` — Owner sequence (outside an agent run): create `.owner-patch-backup/`; back up both `build/windows-debug/CMakeCache.txt` and `build/ninja-debug/CMakeCache.txt` there; apply the patch above; reload; then authorize a fresh tester run of `cmake --fresh --preset windows-debug`, `cmake --build --preset windows-debug --config Debug`, `ctest --test-dir build/windows-debug -C Debug --output-on-failure`. Because `CMakePresets.json` stores no path, a fresh cache should resolve VS2026 from the current install (`C:\Program Files\Microsoft Visual Studio\18\Community`) — `hypothesis`, not verified until the fresh run succeeds. The hard denys for delete/move stay in force; `cmake --fresh` performs its own cache regeneration and needs no shell delete rule.

### Package B — current-state verification of XB-X64-CORPUS-01 (observed, all failed)

`observed` — Tester (`ses_df1585811ffeqeFNdU5kAznCmD`), exact allowlisted commands, current run (not historical), executed with full output:
- `cmake --preset windows-debug; "EXIT=$LASTEXITCODE"` → **exit 1** (`CMakeCache.txt` directory `D:/…` differs from `e:/…`; source `D:/…/CMakeLists.txt` does not match `E:/…`).
- `cmake --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"` → **exit 1** (same mismatch; generator `Visual Studio 18 2026` could not find `E:/PRO/Programm Daten/Visual Studio/18/Community`).
- `ctest --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"` → **exit 8** (`Unable to find executable: E:/…/xbridge_tests.exe`; `1/1 Test #1: xbridge_tests ... ***Not Run`; `0% tests passed, 1 tests failed out of 1`). Found 1 / executed 0 / passed 0 / failed 1 (`Not Run`) / skipped 0.
Both caches were re-read in full and are byte-identical to baseline. `tests/test_main.cpp` was not edited. **No test was compiled or executed.** `XB-X64-CORPUS-01` therefore remains `needs_owner` under `OWNER-X64-VERIFY`; no status change on this evidence.

### Package C — systematic disposition of open documentation/research work

`documented` — C1 `XB-0082`: evaluation-only convention is complete and consistent at `docs/AGENT_WORKFLOW.md:157-174` (fixed key order, allowed tokens, `PROGRESS_UNITS: N/A` unless a declared integer denominator, `STATUS: pass` as the only complete token). No generator, tool, CI, dependency or percentage display was added; a real implementation stays behind `OWNER-XB0082-SCOPE` (`:1045-1055,1060`).
`documented` — C2 `XB-0083` / `R-001`…`R-006`: read-only researcher verification (`ses_df1585811ffcXPy5zYyyRlo8YO`) confirmed the ledger at `:1033-1034` is accurate: all six legacy rows are `unknown`/`conformant` with `no package assigned` and gated by `OWNER-XB0083-START` (`:1061`); R-005's `partial` is a workflow note only. RESEARCH-007 and XB-0084 are complete with independent `PASS` and were not re-researched. No web access, no edits, no new research package started.
`documented` — C3 `XB-0084` decision register: the seven open points are consolidated at `docs/research/XB-0084_RISK_DEPENDENCY_OVERVIEW.md:108-118` (assessment baseline, project license/third-party policy, subsystem ADRs, native/JIT policy, optional Xbox 360 scope, performance-publication protocol, FINAL_CLEANUP timing), with components, alternatives, dependencies, risks and ADR references at `:14,39-46,112-118`. No decision was silently resolved; all remain owner-gated.
`documented` — C4 `SETUP-004/006/007`: remain owner actions (session export/import, worktree cleanup, agent installation/persisted permissions) at `:15,17-18,1039-1041`; installed toolchains exist and nothing was reinstalled. No executable agent step remained.
`documented` — C5 `ai/README.md` patch 2: re-read `:13` still contains "Open a pull request"; the complete patch proposal is unchanged at `:634-649` and remains `OWNER_DECISION` RV-P1-08 (`:716`). Not applied by an agent.
`observed` — C6 `XB-0085-CURRENT`: at the current HEAD the tree still has 25 entries and the audit at `:937-955` applies unchanged: 12 `READY_FOR_COMMIT`, 3 `NEEDS_REVIEW`, 10 `DO_NOT_PUBLISH`; the historical 32-entry (13/18/1) and 24-entry (12/2/10) snapshots remain explicitly labelled historical. No staging, publication or PR action; PR state stays `unknown`.

### Package D/E — complete envelopes for the owner-gated public interfaces (drafts recorded, not implemented)

`documented` — The architect (`ses_df1585811ffdRFFa5eq8f9CcOl`) supplied the missing concrete detail for two envelopes that the ledger previously only referenced (`:1026,1037-1038`). Both are **drafts for the coordinator**, `hypothesis` as designs, explicitly **not** review verdicts, approvals or implementations. No product file was edited.

```text
PACKAGE_ID: XB-X64-STEP-01
PACKAGE_TYPE: FEATURE
PRODUCT_BOUNDARY: product
GOAL: Add a byte-span x64 single-step facade that composes the existing decoder and interpreter so one documented instruction executes per call without replacing either contract.
FILES: include/xbridge/cpu/x64_instruction_interpreter.hpp; src/cpu/x64_instruction_interpreter.cpp; tests/test_main.cpp
ALLOWED: Declare [[nodiscard]] X64ExecuteStatus step_x64_instruction(std::span<const std::uint8_t> bytes, X64CpuState& state, memory::GuestMemory& memory) noexcept in namespace xbridge::cpu; implement it only as decode_x64_instruction(bytes) followed by execute_instruction(decoded, state, memory); add tests using the existing check harness.
FORBIDDEN: JIT; RIP-based fetch; run loops; new decode/execute status values; FLAGS/SIB/SIMD/displacement semantics; changes to TestCpu, GuestMemory, decoder representation, CMakeLists.txt, presets, docs, agent/config files; weakening existing tests.
ACCEPTANCE: The facade returns Ok and advances RIP by DecodedInstruction::length for documented NOP/MOV forms; returns MemoryAccessError with unchanged CPU/memory for out-of-range and >UINT32_MAX addresses; treats every non-Ok decode status as an Ok no-op with unchanged state; all pre-existing tests remain; exact build/CTest/diff commands executed with real results.
TESTS: NOP {0x90}; MOV imm32 {0xB8,0x78,0x56,0x34,0x12}; MOV imm64 with REX.W {0x48,0xB8,...}; MOV reg/reg and reg/mem round trips; 4-/8-byte boundary failures at GuestMemory edge; address 0x100000000ULL -> MemoryAccessError without mutation; non-Ok no-ops for empty span, {0x0F}, {0x89,0x04}, {0x48,0x8B}. Verify: cmake --preset windows-debug; cmake --build --preset windows-debug --config Debug; ctest --test-dir build/windows-debug -C Debug --output-on-failure; git diff --check.
REVIEW: architect interface re-check; tester independent verification; reviewer read-only verdict.
WORKTREE: none
OWNER_APPROVALS: OWNER-X64-STEP (public interface); private test-helper alternative avoids a new public API but still needs an approved envelope.
```
`documented` — Basis: decoder contract `include/xbridge/cpu/x64_instruction_decoder.hpp:17-23,56-77` + `docs/ARCHITECTURE.md:104-139`; interpreter contract `include/xbridge/cpu/x64_instruction_interpreter.hpp:9-24` + `docs/ARCHITECTURE.md:153-163`; no FLAGS/SIB/SIMD/displacement (`docs/ARCHITECTURE.md:170-180`); the span facade is NOT the deferred RIP-fetch bridge (`docs/ROADMAP.md:27,49`). Not implemented: adding a public header symbol needs owner approval (`AGENTS.md:26,63`).

```text
PACKAGE_ID: XB-HOST-CAP-01
PACKAGE_TYPE: FEATURE
PRODUCT_BOUNDARY: product
GOAL: Add a deterministic capability-provider seam so policy consumes immutable host facts through an interface without Windows probing or guest-semantic change.
FILES: include/xbridge/host/capabilities.hpp; include/xbridge/host/execution_policy.hpp; src/host/execution_policy.cpp; tests/test_main.cpp
ALLOWED: Add ICapabilityProvider (virtual ~, [[nodiscard]] capabilities() const noexcept = 0), FixedCapabilityProvider (explicit constructor, immutable HostCapabilities), and select_execution_plan(const ICapabilityProvider&, const ExecutionSettings&) forwarding to the existing HostCapabilities overload; add deterministic provider tests.
FORBIDDEN: Windows/Win32/Vulkan/D3D12 probing; backend initialization; firmware/keys/proprietary assets; new dependencies; CMake/preset changes; changing or removing the existing select_execution_plan overload; guest CPU/memory/GPU/profile changes; JIT; compatibility claims.
ACCEPTANCE: Provider overload yields a plan and report identical to the direct overload for the same facts; FixedCapabilityProvider is repeatable and reads no OS state; existing selector and tests unchanged; no probing or dependency added; exact build/CTest/diff commands executed with real results.
TESTS: Reuse synthetic fixtures at tests/test_main.cpp:1630-1676 for automatic Vulkan selection, software fallback, fallback-disabled Unavailable, manual Direct3D12, JIT available/unavailable and report text; verify repeated immutable facts and unmutated settings. Verify: cmake --preset windows-debug; cmake --build --preset windows-debug --config Debug; ctest --test-dir build/windows-debug -C Debug --output-on-failure; git diff --check.
REVIEW: architect interface/ADR and host/guest-boundary check; tester independent verification; reviewer read-only verdict.
WORKTREE: none
OWNER_APPROVALS: OWNER-HOST-CAP (provider interface and any later Windows-probing scope).
```
`documented` — Basis: `HostCapabilities` is host-only data (`include/xbridge/host/capabilities.hpp:7-19`); pure selector `docs/ARCHITECTURE.md:229-231`, `src/host/execution_policy.cpp:17-93`; ADR-0005/0010/0011 (`docs/DECISIONS.md:23-26,48-56`); no real detector exists (`docs/ADAPTIVE_EXECUTION.md:36-38`, `docs/HOST_HARDWARE_POLICY.md:38-40`, `src/profiles/hardware_profile.cpp:1-2`). Deterministic-provider-first is the recommendation at `:1064`; real Win32/DXGI/Vulkan entry points are `unknown` and excluded.

`documented` — Package F (Agent Core): no new contradiction was found beyond items already recorded at `:121-125` (installed `developer`/`tester` both may write `tests/**`; future-planned `test-engineer` exclusivity; global web denies vs researcher allows resolved by last-match-wins at `docs/AGENT_WORKFLOW.md:95`; SETUP-006/007 still `blocked`). No documented architecture was rewritten; no new dependency, global permission, external automation or publication was added. Package G (consolidation): this single record plus the roadmap note below are the targeted updates; no broad restructuring.

```yaml
PACKAGE_ID: XB-CONTINUATION-02
PARENT_ID: N/A
DEPENDENCIES: OWNER-X64-VERIFY, OWNER-X64-STEP, OWNER-HOST-CAP, OWNER-XB0082-SCOPE, OWNER-XB0083-START, OWNER-XB0084-DECISIONS, OWNER-PUBLICATION
STATUS: needs_owner
IMPLEMENTATION_STATE: needs_owner
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: OWNER-X64-VERIFY
PROGRESS_UNITS: N/A
```
`documented` — Honest outcome: no build, configure, compile, CTest or product-code success was achieved; all executable work consisted of current-state verification (failed as recorded), read-only verification, complete owner patches/envelopes and documentation consolidation. `STATUS: pass` is not claimed. The five new tests remain uncompiled (`unknown` runtime correctness), and every owner decision at `:1058-1067` stays open.

`observed` — Final bundled independent review (`ses_df1550a00ffe1EInCOTAS66xWT`) returned **NEEDS_CHANGE**: 1 `ACTIONABLE` (LOW, `XB-CONT-02-REV-01`: wrong anchor `docs/AGENT_WORKFLOW.md:84` instead of `:95` for last-match-wins), 1 `NON_BLOCKING` (INFO, `XB-CONT-02-REV-02`: patch block not a tool-applicable unified diff), 0 `OWNER_DECISION`, 0 `UNKNOWN`. Case B applied: ONE bundled correction by the coordinator as sole writer of `docs/AGENT_TASKS.md` (both `:84` anchors corrected to `:95` at `:123` and `:1217`; illustrative manual-edit label added before the patch block at `:1146`). No finding was downgraded. Targeted independent re-check (same session) returned **PASS** with 0 `ACTIONABLE`: both anchors resolve to `docs/AGENT_WORKFLOW.md:95`, the patch label is present and consistent with "prepared, NOT applied", envelope/yaml tokens and build/test honesty are unchanged, Roadmap stays consistent, and no new finding was introduced. Two pre-existing stale anchors at `:290`/`:311` were reported `NON_BLOCKING` and not reopened. Case B is complete after one correction round; no second correction round and no further review. This paragraph and the `REVIEW_STATUS: PASS` update are coordinator bookkeeping added after the PASS re-check; they record the verdict and do not change any reviewed substantive artifact.

## XB-X64-VERIFY — controlled Windows build re-run (owner order, current session)

`documented` — Owner ordered exactly one tester command `cmake --fresh --preset windows-debug`, build/CTest only on prior success, no permission bypass, no source/test/CMake/cache/Git changes, independent reviewer assessment. This is a re-run, not a new package envelope.

`observed` — Preparation: HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`, branch `docs/opencode-publication-group-2`, 25 working-tree entries, empty index (unchanged). New rules are present in `opencode.jsonc:92-96` and `.opencode/agents/tester.md:47-61`, so the scoped expansion was applied. **Correction to the coordinator's initial preparation note:** `.owner-patch-backup/` is NOT empty. A direct directory read shows `windows-debug-CMakeCache.txt`, `ninja-debug-CMakeCache.txt`, `opencode.jsonc`, `opencode.jsonc.original`, `tester.md.original` and `.opencode/`; the earlier `glob` result "No files found" was a tool artifact (the directory is excluded at `.gitignore:17`). Both cache backups' lines 2-3 match the source caches. No *new* backup was created by this run; no cache was deleted/moved.

`observed` — tester (`ses_df1354920ffeWaKLHxv3eogAVJ`) ran exactly `cmake --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"` once. It was **not** permission-denied; the command reached PowerShell. Result: `CommandNotFoundException` (`Die Benennung "cmake" wurde nicht als Name eines Cmdlet ... erkannt`), `FullyQualifiedErrorId: CommandNotFoundException`, and `EXIT=` (empty — no native process started, no numeric cmake exit code). Verdict: **configure FAIL**, not PASS, not permission-BLOCKED. No alternate form, absolute path, `cmake.exe`, `Copy-Item`, or permission edit was attempted. Because configure was not successful, build and CTest were correctly NOT run.

`observed` — Independent reviewer (`ses_df1347fdfffe56fDvdKXF6c8L2`) verdict **PASS, Case A**: 0 `ACTIONABLE`, 1 `NON_BLOCKING` (the missing-backup misstatement above, now corrected from the reviewer's direct read), 1 `UNKNOWN` (reviewer has no shell; Git state and `CommandNotFoundException` root cause not independently re-executed). The PASS certifies the honesty and completeness of the **FAIL** record, not build/test success. The five tests at `tests/test_main.cpp:1216,1309,1390,1439,1457` remain uncompiled and unexecuted; `XB-X64-CORPUS-01` stays `needs_owner` under `OWNER-X64-VERIFY`. No regression assessment is possible with zero executed tests (`unknown`).

`documented` — Outcome: configuration **FAIL**, build **not run**, CTest **not run**, 0 tests compiled/executed/passed/failed. Technical blocker: `cmake` is not resolvable on the tester shell PATH in this session (`unknown` why, e.g. plain-PATH resolution vs VS2026-only install); the owner's exact command is now permission-allowed but cannot execute. No publication, staging, commit, or product change occurred.

## XB-CMAKE-PATH-DIAGNOSTIC — read-only shell resolution check

`documented` — Owner ordered six read-only probes only (Get-Command/where.exe/Test-Path/absolute --version/$env:PATH/tester-environment parity), with no config/build/cache/Git action. No permission denial was bypassed or retried.

`observed` — Coordinator: probes 1-4 (`Get-Command cmake -All`, `where.exe cmake`, `Test-Path` on the supplied absolute path, direct absolute `cmake.exe --version`) each returned `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}` (coordinator.md:74-85). Probe 5 (`$env:PATH`) executed; the PATH contains no `Microsoft Visual Studio` or `CMake` entry. Tester (`ses_df12fb8deffeeUczu5oOkAMe5A`): the same probes 1-4 were likewise `permission.rejected` (tester.md:20-22 `shell: * deny` plus exact allows). Allowlisted `cmake --version` executed and returned PowerShell `CommandNotFoundException` with `EXIT=` empty. Tester probe 5 PATH was captured.

`observed` — Correction of a transcription artifact: the PATH string supplied to the tester in the prompt accidentally contained an extra `C:\Program Files (x86)\NVIDIA Corporation\NVIDIA App\NvDLISR`. Direct comparison of the ACTUAL coordinator and tester `$env:PATH` outputs shows them **identical**. The shells therefore show no observed PATH difference; permission rules differ by role (coordinator `cmake * deny`, tester exact cmake allows), which is a permission-layer difference, not an environment difference. Probes 3/4 were denied in both roles, so on-disk existence and absolute-path executability of the supplied `cmake.exe` remain `unknown`; no claim is made for either.

`documented` — Diagnostic result: bare `cmake` is unresolvable in both shells because the VS CMake bin directory is absent from PATH (`observed` for PATH, `hypothesis` for the causal link). The owner-supplied absolute path is `documented` as supplied but was not verifiable under installed-role permissions. No fix was applied; any PATH/permission change requires an owner decision.

## XB-CMAKE-ABSOLUTE-PATH-ENABLEMENT — Phase 1 proposal only (NOT applied)

`documented` — Owner supplied the absolute CMake path `C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` and stated that outside OpenCode `Test-Path` returned True and the version is 4.3.1 (`documented` owner report, not re-verified by an agent). Phase 1 is read-only; Phase 2 requires explicit owner approval; no config file may be edited by the coordinator (`coordinator.md:50-55`).

`observed` — Current rules: `opencode.jsonc:87-100` contains only bare-name CMake/CTest allows and the two `Copy-Item` backup allows; hard denys at `opencode.jsonc:131-155` (git publication, `cmake --install`, install-target builds, `Remove-Item`/`Move-Item`/`rm`/`del`) are unchanged. `tester.md:20-22` has `shell: * deny`, followed by exact allows at `:44-61` for bare CMake/CTest and at `:62-70` for ninja. Last-match-wins (`AGENT_WORKFLOW.md:95`) means agent-level exact allows suffice; a global edit is NOT required.

`documented` — Minimal proposed change (tester.md ONLY, insert after line 61 before the ninja block). YAML double-quoted scalars use `\\` for each literal backslash; the executor must use exactly these strings with the `; "EXIT=$LASTEXITCODE"` suffix only for exit capture:

```diff
--- a/.opencode/agents/tester.md
+++ b/.opencode/agents/tester.md
@@ -59,6 +59,15 @@
   - action: shell
     resource: "ctest --test-dir \"build/windows-debug\" -C Debug --output-on-failure"
     effect: allow
+  - action: shell
+    resource: "& 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin\\cmake.exe' --version"
+    effect: allow
+  - action: shell
+    resource: "& 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin\\cmake.exe' --fresh --preset windows-debug"
+    effect: allow
+  - action: shell
+    resource: "& 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin\\cmake.exe' --build --preset windows-debug --config Debug"
+    effect: allow
   - action: shell
     resource: "cmake --preset ninja-debug"
     effect: allow
```

`documented` — All existing deny rules remain: `tester.md:6-7,11-22,71-73` and global `opencode.jsonc:131-155`; no global PATH change, no global shell/CMake blanket allow, no other agent file, no other scope. `unknown`: whether the permission matcher accepts this `& '<absolute path>'` resource form; if a Phase 3 probe is denied, stop and report the verbatim denial without an alternate form. Bare `ctest` at `tester.md:56-61` is already allowed but its shell resolution is `unknown` (PATH lacks the VS bin); no absolute CTest rule is proposed because no such path was owner-confirmed. Status `needs_owner` pending explicit approval; no execution performed.

## XB-CMAKE-VERIFY-RESUME — absolute CMake/CTest chain (all four steps PASS)

`documented` — Owner confirmed the three absolute CMake rules were applied in `tester.md:62-70` and OpenCode was restarted. Verified by direct read: all three exact allows are present after line 61, with all denies intact (`tester.md:6-22,80-85`; global hard-denys unchanged). No config file was edited by an agent.

`observed` — Tester (`ses_df1199749ffeHGlpnRU2dVa31y`), each command run exactly once, no alternate form, no retry:
1. `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --version; "EXIT=$LASTEXITCODE"` → `cmake version 4.3.1-msvc1`, `EXIT=0` → **PASS**.
2. `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --fresh --preset windows-debug; "EXIT=$LASTEXITCODE"` → `Configuring done (4.8s)`, `Generating done (0.4s)`, build files written to `D:/XBridge 1.0/XBridge-Series-Github/build/windows-debug`, `EXIT=0` → **PASS**. Working caches regenerated; backups untouched.
3. `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --build --preset windows-debug --config Debug; "EXIT=$LASTEXITCODE"` → `xbridge_core.lib`, `xbridge.exe`, `xbridge_tests.exe` built in `build/windows-debug/Debug`, `test_main.cpp` compiled, `EXIT=0` → **PASS**.
4. `ctest --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"` → PowerShell `CommandNotFoundException` for bare `ctest`, `EXIT=` empty, no native process started → **FAIL**, not a permission denial. Found/executed/passed/failed/Not Run = `unknown`; zero tests executed. No absolute CTest form was attempted.

`observed` — Read-only discovery: directory `C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin` contains `cmake.exe`, `cmcldeps.exe`, `cpack.exe`, `ctest.exe`; glob confirms `ctest.exe` at that exact path. No install, config edit or PATH change performed.

`documented` — Minimal CTest permission proposal (NOT applied; coordinator cannot edit `.opencode/**`). Insert ONE exact allow in `.opencode/agents/tester.md` after line 70, before the ninja block:

```diff
--- a/.opencode/agents/tester.md
+++ b/.opencode/agents/tester.md
@@ -68,6 +68,9 @@
   - action: shell
     resource: "& 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin\\cmake.exe' --build --preset windows-debug --config Debug"
     effect: allow
+  - action: shell
+    resource: "& 'C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\CMake\\CMake\\bin\\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure"
+    effect: allow
   - action: shell
     resource: "cmake --preset ninja-debug"
     effect: allow
```

`observed` — The owner subsequently applied that single rule and reloaded OpenCode; direct read-back confirmed it at `.opencode/agents/tester.md:71-73` with all deny rules intact. No agent edited `.opencode/**`.

`documented` — The authorized tester command was exactly `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' --test-dir build/windows-debug -C Debug --output-on-failure; "EXIT=$LASTEXITCODE"`. The earlier condition that `XB-X64-CORPUS-01` may be set to `pass` only on exit 0 with all expected tests executed and passed is now met: CTest `EXIT=0`, 1/1 registered binary `xbridge_tests` passed, and the five new cases are unconditional calls inside that binary. A build PASS alone was never treated as verification. No product/source/test/CMake change occurred.

`observed` — Fourth verification step (`ses_df1134a43ffe1ESP4as9wMhb7V`), exact command above, full output: `Test project D:/XBridge 1.0/XBridge-Series-Github/build/windows-debug`, `Start 1: xbridge_tests`, `1/1 Test #1: xbridge_tests ....................   Passed    0.05 sec`, `100% tests passed, 0 tests failed out of 1`, `Total Test time (real) =   0.12 sec`, `EXIT=0` → **PASS**. Found 1, executed 1, passed 1, failed 0, Not Run 0; no denial, no retry. The bare-`ctest` `CommandNotFoundException` at step 4 above was the earlier failed form and is superseded by this result.

`observed` — Final Git/cache state (`ses_df128f84fffeXUBeeHF44CjYHn`): HEAD `58aa1f60eb5e67cbf8aa884f694fce208b54819f`, branch `docs/opencode-publication-group-2`, 25 entries (18 modified, 7 untracked), empty index, `git diff --check` `EXIT=0` with only the pre-existing `CMakePresets.json` CRLF warning; `xbridge_tests.exe` present in `build/windows-debug/Debug`, CTest registration at root `CMakeLists.txt:32-36`, both `.owner-patch-backup/` cache backups present. Bundled reviewer `ses_df1120fd9ffeXNrOLLH95PvD6T`: **PASS, Case A**, 0 findings.

```yaml
PACKAGE_ID: XB-CMAKE-VERIFY-RESUME
PARENT_ID: XB-CONTINUATION-03
DEPENDENCIES: XB-CMAKE-ABSOLUTE-PATH-ENABLEMENT, OWNER-X64-VERIFY
STATUS: pass
IMPLEMENTATION_STATE: done
REVIEW_STATUS: PASS
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: none
PROGRESS_UNITS: N/A
```

## XB-CONTINUATION-03 — autonomous execution attempt (Phase A BLOCKED)

`documented` — Owner ordered autonomous continuation after the XB-CMAKE-ABSOLUTE-PATH-ENABLEMENT approval. Phase A (CMake verification) has highest priority; all later phases depend on configure/build/CTest success.

`observed` — Step 1: the three approved absolute-path rules are NOT present. `tester.md:44-70` still contains only bare-name CMake/CTest allows; `opencode.jsonc:87-100` likewise. Two Greps for `Program Files`, `cmake.exe`, `Common7` in both files returned "No matches found". The owner's approval was documented but the config edit was never applied. The coordinator may not edit `.opencode/**` or `opencode.jsonc` (`coordinator.md:50-55`); no config file was touched by an agent.

`denied` — Step 2, tester (`ses_df129869dffemAOUJcSCNf9Oem`) ran exactly `& 'C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --version; "EXIT=$LASTEXITCODE"` once: `{"error":{"type":"permission.rejected","message":"Permission denied: shell"}}`. No CMake output, no exit code. Per owner rule 7, no alternate form, no bypass, no retry.

`documented` — Consequence: steps 3–6 (fresh configure, build, CTest, CTest-path investigation) were NOT executed; they are only permitted after a successful preceding step. The five CPU tests in `tests/test_main.cpp` remain uncompiled/unexecuted; `XB-X64-CORPUS-01` stays `needs_owner`. No product code, header, test, CMake, preset, cache, backup, Git or publication action occurred. The previously consolidated C1–C6 documentation work and the D/E draft envelopes were not repeated because no new owner decision exists; repeating them would be redundant.

`documented` — Owner action required before Phase A can continue: apply the already-approved three lines from the XB-CMAKE-ABSOLUTE-PATH-ENABLEMENT proposal at `.opencode/agents/tester.md` after line 61 (before the ninja block), then reload OpenCode. No global/config/PATH expansion is needed beyond that exact diff. Any later CTest PATH issue must be handled by owner-confirmed path or explicit new permission decision; no absolute CTest rule was approved.

`observed` — Historical snapshot only: the owner later applied the three absolute CMake rules and the single absolute CTest rule, and the successor package `XB-CMAKE-VERIFY-RESUME` recorded all four verification steps as `EXIT=0`/PASS with a bundled reviewer `PASS`. The blocked Phase A evidence above is retained for the record; it is superseded by that successor, not re-run or rewritten. This section's axes therefore stay as the snapshot taken at the `CommandNotFoundException`/permission-denial stage.

```yaml
PACKAGE_ID: XB-CONTINUATION-03
PARENT_ID: N/A
DEPENDENCIES: XB-CMAKE-ABSOLUTE-PATH-ENABLEMENT, OWNER-X64-VERIFY
STATUS: needs_owner
IMPLEMENTATION_STATE: needs_owner
REVIEW_STATUS: not_reviewed
CONFORMANCE_STATUS: not_assessed
PUBLICATION_STATUS: local_only
BLOCKER_OWNER_DECISION: OWNER-X64-VERIFY
PROGRESS_UNITS: N/A
```

