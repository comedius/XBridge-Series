# Sprint 0: buildable foundation

## Task A — Build verification
Owner: Build/CI agent
- Verify MSVC and Ninja builds.
- Add Windows CI workflow.
- Confirm CTest runs in CI.

Acceptance: clean configure/build/test on Windows.

## Task B — Memory tests
Owner: Memory agent
- Add tests for write8/read8, load, zero-sized memory, exact-end access, and out-of-range writes.
- Keep behavior deterministic.

Acceptance: all boundary tests pass.

## Task C — TestCpu hardening
Owner: CPU agent
- Add invalid-register, unknown-opcode, truncated-program, and instruction-limit tests.
- Do not call it x86-64.

Acceptance: failures are explicit exceptions and regression-tested.

## Task D — Research register
Owner: Research agent
- Populate entries with authoritative public references and evidence labels.
- Distinguish documented facts from inference.

Acceptance: every claim has a source or is marked unknown.
