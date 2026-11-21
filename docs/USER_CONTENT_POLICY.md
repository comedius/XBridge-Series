# User Content Policy

This policy governs project-managed source, tests, releases, installers, and
downloads. It does not grant permission to use XBridge or any third-party
content. The repository's current [LICENSE](../LICENSE) is marked
`UNLICENSED`; consult [Legal Compliance](LEGAL_COMPLIANCE.md) for the release
review requirements.

## No bundled or automatically obtained protected content

XBridge source, test fixtures, installers, release packages, and
project-operated downloads must not include:

- commercial games or game binaries;
- console firmware or other proprietary system files;
- cryptographic keys; or
- copyrighted assets or test content unless maintainers have documented a
  review and explicitly approved the specific use and distribution, with the
  applicable rights and permissions traceable.

XBridge must not automatically obtain, download, extract, decrypt, or install
such content. Build dependencies are separate from console or game content
and must still be reviewed under their own licenses.

## User-provided files

Users are responsible for ensuring they have the rights and permissions
needed for files they choose to use. Possession of a file or device, or
describing software as homebrew, is not treated by this project as proof of
authorization.

User-provided files must remain separate from the repository, ordinary
development tests, issue and pull-request attachments, and public release
artifacts. Do not submit games, firmware, keys, proprietary system files, or
other protected content to project services or contributors. Prefer a
synthetic reproducer and redacted diagnostic output. XBridge must not upload
user-provided content by default; any future feature that stores, transmits,
or transforms it requires separate privacy, security, and legal review.

If protected content is inadvertently submitted, do not process it or copy it
to other project channels, repositories, or downloads. Maintainers must
restrict access or remove it as appropriate, review the incident, and seek
qualified legal advice if legal uncertainty remains.

## Technical protection measures

Project code and documentation must not explain or enable bypassing DRM, copy
protection, access controls, or other technical protection measures. The
project will not provide instructions or tooling to obtain or use protected
content or keys. This policy does not provide a process for extracting or
converting such content.

## Responsibility and limitations

The user's responsibility for files they provide does not automatically
release XBridge, its maintainers, or contributors from their own legal
obligations, and does not make a use authorized. This policy is not legal
advice and makes no blanket statement about the legality of emulation or any
particular use. Legal rights and restrictions may depend on jurisdiction and
the facts; consult qualified legal counsel when uncertain.

See also [Homebrew Support](HOMEBREW_SUPPORT.md) and
[Legal Compliance](LEGAL_COMPLIANCE.md).
