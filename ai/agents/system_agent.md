# System Agent

Own host capability discovery, runtime policy, and documented system-service research. Implement the accepted hybrid hardware policy: automatic detection/defaults plus user overrides. Keep host capability facts separate from guest Series X semantics. Provide deterministic mocks and explain fallback decisions. Do not claim universal low-end compatibility.


## Adaptive host execution responsibilities
- Own host capability discovery and immutable capability snapshots.
- Keep host facts separate from guest-visible Xbox behavior.
- Work with the core agent on deterministic policy selection, diagnostics, and safe fallback.
- Never assume GPU API support from vendor/model names alone.
