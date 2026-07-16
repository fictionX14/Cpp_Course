# Skills Tracker

Mastery matrix for every C++ concept we touch. Update at the end of each session.

**Legend:** ⚪ Not started · 🟡 Learning · 🟢 Solid · 🔵 Mastered (cold quiz passed)

## Phase 0 — Memory & Ownership
| Concept | Status | First seen | Last reviewed | Notes |
|---|---|---|---|---|
| Stack vs heap | 🟢 | Day 1 | Day 7 | GRADUATED Day 7: fully clean cold pass (double/ptr/shared_ptr, zero hints) — no conflation, correct cleanup responsibility for all three. Recovered from the Day 4-6 regression via the "name=stack always; only new/make_X=heap" rule |
| RAII | 🟢 | Day 1 | Day 5 | Day 5: correctly explained full mechanism (stack obj dtor guarantee → calls delete) unprompted — recovered from Day 4's acronym slip |
| `new`/`delete`, leaks | 🟢 | Day 1 | Day 2 | verified leak count/size w/ real tool, understands allocator overhead |
| `std::unique_ptr` | 🟢 | Day 2 | Day 4 | consistent correct reasoning across 2 sessions; interleaved successfully into Day 4 coding rep |
| `std::shared_ptr` | 🟢 | Day 3 | Day 4 | ref-counting mechanism derived correctly AND live-verified via real `use_count()` output Day 4; grasped handle-vs-object distinction unprompted |
| `std::weak_ptr` | 🟡 | Day 7 | Day 7 | Correctly reasoned the reference-cycle mechanism unprompted (why neither count can hit 0). First instinct for safe access was `use_count()`-then-dereference (a real anti-pattern — not atomic); corrected to `.lock()`, which returns an empty or owning `shared_ptr` in one atomic step. No code written yet — coding rep planned Day 8 |
| Dangling / use-after-free | 🟡 | Day 5 | Day 7 | Stack-lifetime flavor is rock solid (reused correctly, unprompted, Day 7). Heap-lifetime flavor (use-after-free via `delete` without nulling) still not independently demonstrated — asked for it directly Day 7, gave the stack example again instead. Real gap, not yet closed |
| Double free | 🟡 | Day 2 | Day 3 | reasoned correctly both times but conflated "UB" with "compile error" once — watch this |

## Phase 1 — Value Semantics + STL
| Concept | Status | First seen | Last reviewed | Notes |
|---|---|---|---|---|
| Copy semantics | ⚪ | — | — | |
| Move semantics / rvalue refs | ⚪ | — | — | |
| Rule of 0/3/5 | ⚪ | — | — | |
| const-correctness | ⚪ | — | — | |
| References vs pointers | ⚪ | — | — | |
| STL containers + complexity | ⚪ | — | — | |
| Iterator invalidation | ⚪ | — | — | |
| Lambdas | ⚪ | — | — | |

## Phase 2 — Concurrency + UB
| Concept | Status | First seen | Last reviewed | Notes |
|---|---|---|---|---|
| `std::thread` | ⚪ | — | — | |
| mutex / lock_guard | ⚪ | — | — | |
| atomics | ⚪ | — | — | |
| condition_variable | ⚪ | — | — | |
| data races | ⚪ | — | — | |
| deadlock | ⚪ | — | — | |
| integer overflow / signed-unsigned | ⚪ | — | — | |
| lifetime UB | ⚪ | — | — | |
