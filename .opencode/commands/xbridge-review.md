---
description: Orchestrate the XBridge review and review-fix workflow through the coordinator
agent: coordinator
---

Orchestrate the full review-fix workflow for: $ARGUMENTS

Orchestrate through yourself (the coordinator); do not perform the review in your own voice. Delegate the actual read-only review to the `reviewer` subagent using the checklist in `docs/AGENT_WORKFLOW.md` and the finding format in `.opencode/agents/reviewer.md`. The reviewer must remain read-only and must not launch subagents.

After EVERY reviewer result, classify each finding as `ACTIONABLE`, `OWNER_DECISION`, `NON_BLOCKING`, or `UNKNOWN`. Then apply exactly one of two cases:
- Case A — no correction required: the reviewer reported no `ACTIONABLE` finding and no correction was made. Do not start another review. Keep the existing reviewer verdict and list open `OWNER_DECISION` and `UNKNOWN` items separately.
- Case B — at least one correction required: group all `ACTIONABLE` findings by root cause and merge duplicates; create ONE bundled correction package per writer; have the responsible writer perform the correction; then an independent reviewer re-check is mandatory.
- Follow review → bundled correction → re-review, limited to two bundled correction rounds; then mark the package `BLOCKED` with all owner decisions collected.
- Never downgrade a severe finding to reach `PASS`. If permissions prevent a fix, prepare a complete owner patch and never claim it is fixed.

Report the final verdict and recorded evidence after the required independent re-check, or — when no correction and therefore no re-check was required — immediately after classification and adoption of the result. Do not modify product code; record results in `docs/AGENT_TASKS.md`.
