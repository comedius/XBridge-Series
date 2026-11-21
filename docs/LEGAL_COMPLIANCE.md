# Legal Compliance and Release Policy

This document describes XBridge project rules and review requirements. It is
not legal advice, does not grant rights to use or distribute XBridge, and does
not determine whether a particular emulator, file, or use is lawful.

## Project scope and status

- **Documented current state:** XBridge is an independent Windows emulator
  project targeting Xbox Series X. The current [architecture](ARCHITECTURE.md)
  identifies executing Xbox executables, loading Xbox firmware or keys, and
  shipping or decrypting commercial game content as non-goals for version
  0.1.0. Its `TestCpu` is a synthetic test instruction set, not an Xbox or
  x86-64 CPU.
- **Planned goal:** Longer-term compatibility work may include self-developed
  homebrew and commercial Xbox Series X software, but only where technically
  and legally permissible. This goal does not state that either category is
  currently supported.
- **Project rule:** Keep emulator code and user-provided content separate.
  Protected games, firmware, keys, proprietary system files, and unauthorized
  copyrighted test assets must not be added to the repository, installers,
  release packages, test fixtures, or project-operated downloads.

The project's technical scope and these rules are not legal conclusions about
emulation or any particular activity. Applicable requirements can depend on
the jurisdiction, the content, its source, and the specific conduct.

## Independent development and protected content

XBridge must be developed as an independent implementation. Contributors must
not copy or incorporate code, assets, documentation, binaries, or other
protected material unless the project has verified permission and satisfied
the applicable license terms. Public documentation may inform independent
implementation, but it does not establish that every interface or technique
may be used in every context.

Keep user data outside project source, build inputs, tests, release artifacts,
and project-managed download locations. Do not request or accept proprietary
game data, firmware, system files, or cryptographic keys as ordinary issue,
pull-request, test, or support attachments. Prefer original synthetic test
programs and other material whose provenance and permitted use are clear.

## Technical protection measures

XBridge will not implement, facilitate, or document bypassing DRM, copy
protection, access controls, or other technical protection measures. The
project will not provide instructions or tooling to acquire, decrypt, extract,
or use protected content or keys. This exclusion applies to source code,
documentation, automated workflows, and project-operated downloads.

## Third-party code and licenses

Before adding or distributing third-party code or assets, identify their
origin, exact version or commit, applicable license and notices, and the
intended manner of use. Review relevant dependencies, submodules, generated
or vendored files, build options, and bundled assets. Preserve required
attribution and notices and assess applicable source-distribution, patent,
trademark, and other obligations. Do not assume that licenses are compatible
or that a dependency's presence in another emulator authorizes its use here.

The repository's current [LICENSE](../LICENSE) is marked `UNLICENSED` and says
that it grants no permission. This marking is not a general permission to use,
modify, or redistribute XBridge code. Until the project license is clarified,
those rights must not be promised or assumed on the project's behalf. The
current license text also calls for a license choice and review before
accepting third-party contributions or publishing binaries. This policy does
not change that status.
Choosing and reviewing an appropriate XBridge license, with qualified legal
advice, is a project prerequisite before public source or binary release.

## Review before public development and release

The project distinguishes three activities:

- **Public development work** includes public discussions, design notes, and
  progress reports. It is not, by itself, a grant of rights to use, modify,
  or redistribute repository code.
- **Source-code publication** means making XBridge source available to the
  public. It is separate from public discussion and requires a clarified,
  reviewed project license before the project offers or represents rights in
  that code.
- **Executable binary distribution** means providing XBridge binaries,
  installers, or downloads. It is a separate release activity and requires
  review of the included code, dependencies, and packaged material.

Making the source repository public is source-code publication, not merely
public discussion. Public development work, source publication, and binary
distribution must not be treated as interchangeable permissions or releases.

Before public development work that discloses code, protected material, or
other content with unresolved rights, maintainers must review the relevant
risks. Before source publication or executable binary distribution,
maintainers must review all applicable items below:

1. code and contribution provenance, including generated and vendored files;
2. project and third-party licenses, notices, dependency versions, and
   distribution obligations;
3. all packaged executables, assets, test data, sample files, and installer
   or download behavior;
4. documentation and release claims, so planned or unknown capabilities are
   not presented as implemented or tested;
5. whether user-provided content is kept separate and whether any future
   feature would transmit, store, or transform it; and
6. unresolved copyright, licensing, contract, privacy, or technical-protection
   questions relevant to the intended release and jurisdictions.

The results and decisions of each review must be documented. Any unresolved
legal or licensing obstacle applicable to a proposed source publication or
binary distribution blocks that release until it is resolved. Record the
review and obtain advice from qualified legal counsel when needed. A
user-content notice or disclaimer does not automatically release XBridge, its
maintainers, or contributors from their own obligations.

## No legal advice or blanket assurances

Nothing in this document promises that emulators, XBridge, or a particular
use, copy, or distribution is legal or illegal. It is not a substitute for
jurisdiction-specific advice from qualified legal counsel. Anyone facing a
legal uncertainty should obtain that advice before proceeding.

Related project rules: [Homebrew Support](HOMEBREW_SUPPORT.md),
[User Content Policy](USER_CONTENT_POLICY.md), and
[ADR-0009: No proprietary firmware or game assets](DECISIONS.md#adr-0009-no-proprietary-firmware-or-game-assets).
