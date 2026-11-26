# Development Infrastructure and Future Product Packaging

## Purpose and status

This document defines the boundary between the public XBridge development repository and any future end-user product package. It records the current project state and the packaging rules that must be applied when a distributable product is eventually prepared.

**Status:** policy and planning documentation only. This document does not create a release, a product package, an installer, a CMake install target, or permission to distribute the project.

## 1. Current project architecture

XBridge is an experimental C++20 project investigating the engineering foundations required for an Xbox Series X system emulator on Windows x64.

The repository currently contains:

- a CMake-based C++20 project;
- a command-line application and one CTest test target;
- a generic guest-memory scaffold and a synthetic `TestCpu`;
- a separate x64 CPU-state model, a limited instruction decoder, and a reference interpreter for a small documented instruction subset;
- host-capability and execution-policy scaffolding;
- architecture, research, development, agent-workflow, and verification documentation.

These are foundation and research components. The project is **not a working Xbox emulator** and does not currently run Xbox games or Xbox executables. No compatibility, gameplay, or performance claim follows from a successful build or test run.

The current target is Xbox Series X only. Series S is not a separate target. Older Xbox generations remain deferred and optional. The interpreter-first baseline excludes JIT. Proprietary firmware, encryption keys, commercial game binaries, protected assets, and DRM-circumvention functionality are outside the repository's stated scope.

The detailed technical contracts and implementation status remain in [ARCHITECTURE.md](ARCHITECTURE.md), [README.md](../README.md), and the relevant subsystem documents. This policy document does not supersede those technical specifications.

## 2. Public development repository

The XBridge source and development repository is public. The following project-related development materials may be stored and committed here when they contain no private personal data, secrets, or credentials:

- `.opencode/**` and `opencode.jsonc`;
- `AGENTS.md`, `ai/**`, agent definitions, prompts, and task instructions;
- development workflows, review procedures, automation, and verification guidance;
- architecture decisions, research notes, technical inventories, and development ledgers;
- build and test configuration needed to develop and verify the source repository.

Their presence in the public development repository does **not** make them part of the end-user product. Conversely, their exclusion from a future product package does **not** mean they must be hidden from this public repository.

Development infrastructure should remain available in the repository for ongoing development, maintenance, and post-release fixes unless a separate owner-approved repository change says otherwise.

## 3. Future product-package boundary

A future product package must be created from an explicit, reviewed allowlist. It must not be produced by copying the entire repository and attempting to remove unwanted files afterward.

The allowlist does not yet exist as an approved release manifest. Until it is approved, no file or directory should be represented as officially included in a distributable product.

### 3.1 Candidate product contents

Subject to implementation, license review, and explicit release approval, a future allowlist may include only the files needed to build, run, configure, document, and support the end-user application, for example:

- approved application and runtime source or compiled binaries;
- required public headers and runtime resources;
- explicitly approved third-party runtime dependencies and their notices;
- end-user configuration templates that contain no secrets;
- end-user documentation, such as installation, usage, troubleshooting, and accessibility information;
- the final license, notices, and attribution materials approved for distribution.

This is a candidate structure, not an assertion that these artifacts or dependencies are currently complete or distributable.

### 3.2 Development-only material excluded from the product

Unless a future owner-approved allowlist explicitly identifies a specific item as necessary for users, exclude:

- `.opencode/**`, `opencode.jsonc`, `AGENTS.md`, and `ai/**`;
- agent definitions, internal prompts, task ledgers, orchestration instructions, and internal review workflows;
- internal research registers, evidence logs, development notes, and architecture-workflow records not intended as end-user documentation;
- temporary prototypes, debug helpers, local logs, generated reports, caches, and local development artifacts;
- `build/**` and other machine-specific build output;
- `.kilo/**` and other local tool state;
- developer-only scripts and automation that are not required by the released application.

Exclusion from the product package must not delete these materials from the public development repository.

### 3.3 Secrets and private data

Never commit or package:

- passwords, access tokens, API keys, private signing keys, or credentials;
- personal access tokens, private connection strings, or machine-specific authentication state;
- private personal data or unrelated user files;
- local environment files or logs that may contain any of the above.

Use local secret storage and appropriately scoped credentials. Before publication or packaging, inspect changed files and generated artifacts for accidental disclosure. A statement that a task did not intentionally include credentials is not a substitute for an actual content review.

## 4. Dependency and AI-service boundary

The development agent system is a development aid, not automatically a product feature.

The end-user product must not depend on OpenCode, agent orchestration, development prompts, or external AI services merely because those tools are used during development. Any proposed AI-powered end-user feature requires a separate architecture decision, explicit project-owner approval, documented data flows, security and privacy review, and inclusion in the product allowlist.

The current repository's agent and automation files therefore remain development-only unless that separate decision is made.

## 5. License and distribution gate

The repository currently declares `UNLICENSED` in [LICENSE](../LICENSE). That declaration does not grant open-source reuse or redistribution permission.

A distributable product must not be published until the project owner has selected and approved the applicable license and a legal review has addressed the project's own code, third-party dependencies, notices, and redistribution obligations. Do not infer distribution rights from the repository being publicly readable.

## 6. Required packaging workflow

Before creating a release package:

1. Obtain explicit owner approval for the release scope and product allowlist.
2. Review the current architecture and identify the actual application entry points, runtime files, resources, and dependencies.
3. Create the product tree from the allowlist; do not copy the development repository wholesale.
4. Verify that development-only files, local state, build output, secrets, credentials, and private data are absent.
5. Verify license, attribution, and third-party notices for every included component.
6. Build and test from the filtered product tree, not only from the development checkout.
7. Run a product smoke test and confirm that the application starts without development agents, OpenCode, or unapproved AI services.
8. Record exact commands, tool versions, results, artifact contents, and hashes.
9. Obtain final owner sign-off before publication.

A failed or unexecuted gate must remain open. Do not describe a planned package, checklist, or successful development build as a completed product release.

## 7. Ownership and change control

Changes to the product boundary, inclusion of development infrastructure in a release, introduction of an AI product feature, licensing, or publication scope require an explicit project-owner decision. Routine documentation maintenance may clarify this policy but must not silently convert a candidate item into an approved product artifact.

Related documents:

- [Architecture overview](ARCHITECTURE.md)
- [Project completion checklist](RELEASE_CHECKLIST.md)
- [Development environment](DEVELOPMENT_ENVIRONMENT.md)
- [Repository README](../README.md)
