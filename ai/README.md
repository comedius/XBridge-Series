# AI development workflow

This file describes the **installed** agent system for the XBridge development repository. It is development infrastructure and is excluded from the future product package. The authoritative rules are `AGENTS.md`, `docs/AGENT_WORKFLOW.md`, `.opencode/agents/*.md` and `opencode.jsonc`.

## Installed agents

Exactly six agents are installed. No other specialist role is installed or active.

| Agent | Mode | Responsibility | Writes |
|---|---|---|---|
| `coordinator` | primary | Package envelopes, delegation, tracking, independent publication | Development documents, configuration and the ledger paths listed in its agent file |
| `architect` | subagent | Read-only architecture, interface, module-boundary, dependency, ADR and risk analysis | Nothing |
| `researcher` | subagent | Evidence-labeled public-source research | `docs/RESEARCH_REGISTER.md`, `docs/research/**` |
| `developer` | subagent | Product implementation of an approved package | `src/**`, `include/**`, `CMakeLists.txt`, and `tests/**` only when explicitly assigned as sole writer |
| `tester` | subagent | Independent verification and test additions | `tests/**` only when explicitly assigned as sole writer |
| `reviewer` | subagent | Strictly read-only diff, contract, test and evidence review | Nothing |

Planned specialist roles such as `cpu-developer`, `test-engineer` or `integration-engineer` are **not installed**. Work assigned to those future roles is performed by an installed substitute under an approved envelope.

## Required workflow

1. Read `docs/ARCHITECTURE.md`, `docs/DECISIONS.md`, the relevant headers, implementations, tests and documentation.
2. Check branch, status and existing local changes before editing; never overwrite or discard unknown changes.
3. The coordinator emits a complete package envelope with one sole writer per exact file path.
4. Implement only the assigned scope. Product changes go to an installed writer; subagents never publish.
5. Add tests for new behavior, including boundary and failure cases, when the package grants the test write path.
6. Run the package's declared CMake build and CTest commands after the writer reports a stable state.
7. An independent reviewer returns `PASS`, `NEEDS_CHANGE` or `BLOCKED`. Every `ACTIONABLE` finding is corrected in one bundled round per writer and re-checked; at most two rounds are allowed.
8. The coordinator records commands and actual results in `docs/AGENT_TASKS.md`, then commits and publishes authorized work.

## Git and GitHub workflow

- The coordinator performs routine staging, commit, push, merge and authorized PR management **after** required checks and independent review. Subagents never publish.
- Before a commit: run `git diff --check`, inspect the complete diff and confirm the staged set contains only the declared files.
- Destructive operations are hard-denied: history rewrites, `git reset`, `git clean`, `git checkout`, `git switch`, `git restore`, `git stash`, `git pull`, `git tag`, `git worktree`, global Git configuration changes and forced branch or stash manipulation are not available to agents.
- GitHub CLI inspection is limited to read-only PR commands plus explicitly named, owner-authorized PR actions. PR state must be observed, not assumed.
- No secrets, credentials, personal data or private local paths are published.

## Global guardrails

- Xbox Series X only; Xbox Series S and Xbox 360 are out of scope. C++20, interpreter-first, no JIT in the current baseline.
- Do not claim Xbox compatibility, playability or performance without evidence. Mark claims `documented`, `observed`, `hypothesis` or `unknown`.
- Do not add firmware, decryption keys, game content, proprietary assets or protection/DRM circumvention.
- `LICENSE` stays `UNLICENSED` until an explicit owner decision changes it.
- Keep `TestCpu` separate from future x86-64 code and keep guest CPU semantics separate from host capabilities.
- Do not add dependencies, agents, prompts, API keys, AI service calls or development-only text to product code or product tests.

## Development infrastructure boundary

This repository is public. Agent prompts, OpenCode configuration, workflow documents, research and automation may be stored and published here. They are **not** automatically part of a future end-user product distribution; that package must be produced from an explicit reviewed allowlist. See `docs/AGENT_WORKFLOW.md` and `docs/RELEASE_CHECKLIST.md` for the package envelope and final cleanup gate.

Legacy role files under `ai/agents/` are historical templates only. They are not installed agents and are not part of the active workflow.
