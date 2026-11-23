---
description: Independently run the required XBridge verification commands
agent: tester
---

Independently verify: $ARGUMENTS

Run only the package's declared checks. For each check, record the exact command as executed, the real exit code, and the actual result/output; if a command cannot start, report that fact rather than inferring a result. Use `documented`, `observed`, `hypothesis`, `unknown` labels.

Sequence:
- Confirm package scope and acceptance criteria, and that the writer has finished; do not run tests in parallel on files being edited.
- Run targeted tests for the changed behavior first, then the full declared verification (build, CTest, `git diff --check`).
- Skip product build/CTest for pure documentation or configuration packages and state that explicitly.
- After a correction, re-run the affected tests first, then the full declared verification again.

Do not change production code, do not edit build files, and do not weaken, skip, or delete tests to obtain a green result. Report out-of-scope defects to the coordinator with exact file:line locations. See `docs/AGENT_WORKFLOW.md` for the central workflow.
