# Review Queue (Spaced Repetition)

Concepts scheduled to resurface. After a review: **nailed it** → grow the interval (1d → 3d → 7d → 14d → 30d); **fumbled** → reset to 1d.

Warm-up each session = everything with a **Next review** date ≤ today.

| Concept | Last reviewed | Interval | Next review | Result last time |
|---|---|---|---|---|
| Stack vs heap | 2026-07-12 | 1d | 2026-07-13 | FUMBLED Day 4 — swapped manual(heap) vs automatic(stack); reset interval |
| RAII (acquire/release, dtor-on-unwind) | 2026-07-12 | 1d | 2026-07-13 | FUMBLED Day 4 — acronym misnamed "Allocation"; reset interval |
| Exception = invisible exit path | 2026-07-12 | 7d | 2026-07-19 | nailed cold recall again Day 4 — interval grows |
| `std::unique_ptr` (ownership, move, no-copy) | 2026-07-12 | 7d | 2026-07-19 | live-verified in Day 4 coding rep — interval grows |
| Temporary object lifetime (named vs unnamed) | 2026-07-12 | 7d | 2026-07-19 | nailed cold recall Day 4 — interval grows |
| `std::shared_ptr` (copy-based ref counting, use_count) | 2026-07-12 | 3d | 2026-07-15 | live-verified via real program output Day 4 |
| Scope-lifetime tracking (nested block ordering) | 2026-07-12 | 3d | 2026-07-15 | correctly resolved in coding rep Day 4 |
| use_count() safety vs dereference UB | 2026-07-12 | 3d | 2026-07-15 | applied correctly in Day 4 discussion |
