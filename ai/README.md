# AI development workflow

Agents work in the owner-provided checkout; Git-Shell branch changes and worktree commands remain hard-denied. The coordinator publishes only independently reviewed work through the configured Git/GitHub rules.

## Required workflow

1. Read `docs/ARCHITECTURE.md`, `docs/DECISIONS.md`, and relevant headers.
2. Restate the task and list files to be changed.
3. Implement only the assigned scope.
4. Add or update tests.
5. Run CMake build and CTest.
6. Report commands, results, limitations, and unresolved questions.
7. Hand the result to the coordinator; the coordinator publishes only reviewed work through the configured Git/GitHub rules. Subagents never publish.

## Global guardrails

- Do not claim Xbox compatibility unless demonstrated by tests.
- Do not invent undocumented APIs, registers, opcodes, or hardware behavior.
- Do not add firmware, decryption keys, game dumps, or proprietary assets.
- Do not implement bypasses of access controls or security protections.
- Do not modify another agent's module without an approved interface change.
- Keep `TestCpu` separate from future x86-64 code.

## Development infrastructure boundary

The agent system, its prompts, OpenCode configuration, workflow documents and internal automation are development artifacts. They remain in the public development repository and are excluded from a future product package by an explicit allowlist. See `docs/AGENT_WORKFLOW.md` and `docs/RELEASE_CHECKLIST.md` for the package envelope and the final cleanup gate.

Step 7 is explicit: subagents hand off to the coordinator; only the coordinator may open a pull request or perform Git/GitHub publication actions within the configured rules.
