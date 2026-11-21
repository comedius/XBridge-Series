# Homebrew Support

## Purpose and scope

XBridge is an independent Xbox Series X emulator project. Its long-term
compatibility goal is not limited to homebrew: it may include self-developed
homebrew and commercial Xbox Series X software where technically and legally
permissible. Homebrew is the preferred basis for early development and
reproducible tests; it is not an exclusive project purpose or a claim that
commercial software is supported.

For this policy, *homebrew* means independently developed software and test
programs. Calling a program homebrew does not, by itself, establish its legal
status, grant rights to its assets, or show that it is compatible with
XBridge.

## Preferred development material

Use original, self-developed test programs and fixtures wherever practical.
Keep their source and provenance clear, and avoid proprietary firmware, keys,
commercial game files, and other protected system or user content. Public
documentation and original test cases may inform research, but technical
assumptions not established by evidence remain open questions.

Independently developed homebrew programs may still contain third-party
libraries, assets, or other protected components. Check their provenance and
usage rights separately.

The current [architecture](ARCHITECTURE.md) describes `TestCpu` as a
synthetic instruction set and marks executing Xbox executables as a
non-goal for version 0.1.0. It does not document working Xbox homebrew
execution. These project rules do not change that implementation status.

## How to describe support

Project documentation and release notes must distinguish:

- **Implemented and tested:** functionality present in XBridge and verified
  by identified, reproducible tests.
- **Planned:** a stated future objective with no claim of present
  implementation.
- **Unknown:** behavior or compatibility not established by available
  evidence.

Do not describe a homebrew program, API, or commercial title as supported
without relevant implementation and test evidence. Successful tests with
synthetic programs or test fixtures do not establish compatibility with real
Xbox hardware, Xbox software, or commercial games; documentation must not
imply otherwise.

## Boundaries

Homebrew support does not authorize or require distributing protected
content. XBridge will not bundle or automatically obtain games, firmware,
keys, or proprietary system files, and will not implement or document
circumvention of DRM or other technical protection measures. See
[Legal Compliance](LEGAL_COMPLIANCE.md) and
[User Content Policy](USER_CONTENT_POLICY.md).

## Legal uncertainty

This document states project policy, not legal advice. Neither the term
*homebrew* nor the user's responsibility for supplied files automatically
resolves legal questions or releases the project from its own obligations.
Consult qualified legal counsel when the applicable rights or restrictions
are uncertain.
