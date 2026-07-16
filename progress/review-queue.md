# Review Queue (Spaced Repetition)

Concepts scheduled to resurface. After a review: **nailed it** → grow the interval (1d → 3d → 7d → 14d → 30d); **fumbled** → reset to 1d.

Warm-up each session = everything with a **Next review** date ≤ today.

| Concept | Last reviewed | Interval | Next review | Result last time |
|---|---|---|---|---|
| Stack vs heap | 2026-07-15 | 3d | 2026-07-18 | GATE CLEARED Day 7 — fully clean cold pass, fresh variables (double/ptr/shared_ptr), zero hints, zero conflation of pointer-var vs pointee. First clean pass since Day 4 regression began |
| RAII (acquire/release, dtor-on-unwind) | 2026-07-13 | 3d | 2026-07-16 | mechanism correctly explained Day 5 (unique_ptr on stack, dtor guarantee) — interval grows |
| Exception = invisible exit path | 2026-07-12 | 7d | 2026-07-19 | nailed cold recall again Day 4 — interval grows |
| `std::unique_ptr` (ownership, move, no-copy) | 2026-07-12 | 7d | 2026-07-19 | live-verified in Day 4 coding rep — interval grows |
| Temporary object lifetime (named vs unnamed) | 2026-07-12 | 7d | 2026-07-19 | nailed cold recall Day 4 — interval grows |
| `std::shared_ptr` (copy-based ref counting, use_count) | 2026-07-15 | 7d | 2026-07-22 | Day 7: all 3 predicted counts correct (global/local sharing scenario) — interval grows |
| Scope-lifetime tracking (nested block ordering) | 2026-07-15 | 7d | 2026-07-22 | Day 7: numbers correct, but causal reasoning on count-decrease slightly muddled ("gains ownership" vs. "other owner destroyed") — corrected, still counts as a pass |
| use_count() safety vs dereference UB | 2026-07-15 | 3d | 2026-07-18 | Day 7: correct conclusions (use_count safe, `*sp` unsafe on empty) but reasoning imprecise both times ("stable structure" instead of "never touches managed object"; "went out of scope" instead of "never had a target") — modest interval growth |
| Dangling pointer: stack-lifetime flavor | 2026-07-15 | 7d | 2026-07-22 | reused correctly and unprompted Day 7 — solid |
| Dangling pointer: heap-lifetime flavor (use-after-free via delete) | 2026-07-15 | 1d | 2026-07-16 | GAP — asked directly for this Day 7, gave the stack-lifetime example again instead. Needs a dedicated rep, ideally compiled + run under AddressSanitizer |
| UB's "silently appears to work" danger (vs. "always crashes") | 2026-07-14 | 3d | 2026-07-17 | nailed cleanly Day 6 — interval grows |
| `std::weak_ptr` (reference cycles, `.lock()` vs `use_count()`) | 2026-07-15 | 1d | 2026-07-16 | learned Day 7 — mechanism reasoned correctly, `.lock()` needed correction; no code written yet |
