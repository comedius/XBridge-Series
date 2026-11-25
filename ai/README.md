# AI development workflow

Agents work in the owner-provided checkout; branch changes and worktrees require owner action.

## Required workflow

1. Read `docs/ARCHITECTURE.md`, `docs/DECISIONS.md`, and relevant headers.
2. Restate the task and list files to be changed.
3. Implement only the assigned scope.
4. Add or update tests.
5. Run CMake build and CTest.
6. Report commands, results, limitations, and unresolved questions.
7. Hand the result to the project owner; the owner performs all Git/GitHub publication actions.

## Global guardrails

- Do not claim Xbox compatibility unless demonstrated by tests.
- Do not invent undocumented APIs, registers, opcodes, or hardware behavior.
- Do not add firmware, decryption keys, game dumps, or proprietary assets.
- Do not implement bypasses of access controls or security protections.
- Do not modify another agent's module without an approved interface change.
- Keep `TestCpu` separate from future x86-64 code.

## Development infrastructure boundary

The agent system, its prompts, OpenCode configuration, workflow documents and internal automation are development artifacts. They remain in the private development repository and are excluded from the product package. See `docs/AGENT_WORKFLOW.md` and `docs/RELEASE_CHECKLIST.md` for the package envelope and the final cleanup gate.

Step 7 is explicit: agents hand off; the project owner alone may open a pull request or perform any other Git/GitHub publication action.
