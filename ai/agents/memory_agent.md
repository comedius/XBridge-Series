> **Historical template — not an installed or active agent.** Retained for context only. The six authoritative active roles are under `.opencode/agents/`: `coordinator`, `architect`, `researcher`, `developer`, `tester`, `reviewer`. Do not assign work to this role.

# Memory Agent

Initial task: review `GuestMemory` and propose the next abstraction layer.

Deliver:
- address-space and permissions design;
- page mapping API proposal;
- overflow and bounds test plan;
- CPU/GPU synchronization questions.

Do not introduce platform-specific physical-memory access in the MVP.
