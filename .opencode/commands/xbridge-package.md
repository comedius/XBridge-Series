---
description: Create a complete XBridge development package for coordinator review
agent: coordinator
---

Create a development package from: $ARGUMENTS

Use the envelope from `docs/AGENT_WORKFLOW.md` and emit it in this exact field order: PACKAGE_ID, PACKAGE_TYPE, PRODUCT_BOUNDARY, GOAL, FILES, ALLOWED, FORBIDDEN, ACCEPTANCE, TESTS, REVIEW, WORKTREE, OWNER_APPROVALS. Do not implement the package and do not edit product code or agent configuration.

Scope and writers:
- FILES: list the exact paths; state PRODUCT_BOUNDARY (product / development-only / excluded-from-product).
- ALLOWED / FORBIDDEN: state the precise permitted and prohibited changes.
- Assign exactly one writer per file path and confirm that writer has edit permission; no two agents may write the same file. Only installed agents may be writers; do not assign the planned `test-engineer` or other uninstalled specialists.
- ACCEPTANCE: measurable, testable criteria with verifiable success conditions.
- TESTS: targeted tests first, then the full declared package verification; state explicitly if no product build/CTest is needed (pure documentation/configuration).
- OWNER_APPROVALS: name only genuine product, architecture, ADR/license, permission, or release decisions; everything else is routine within the approved envelope.
- REVIEW: state whether the architect is required (interface/multi-module/design work) and that the tester/reviewer plus the review-fix workflow in `docs/AGENT_WORKFLOW.md` apply.

Output only the completed envelope followed by a short open-decisions list; stop without implementing.
