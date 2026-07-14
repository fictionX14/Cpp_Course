# Skills Tracker

Mastery matrix for every C++ concept we touch. Update at the end of each session.

**Legend:** ⚪ Not started · 🟡 Learning · 🟢 Solid · 🔵 Mastered (cold quiz passed)

## Phase 0 — Memory & Ownership
| Concept | Status | First seen | Last reviewed | Notes |
|---|---|---|---|---|
| Stack vs heap | 🟡 | Day 1 | Day 5 | PERSISTENT gap (fumbled Day 4 AND Day 5): keeps attributing automatic cleanup to raw heap memory. Needs a different reinforcement approach next time, not just re-asking |
| RAII | 🟢 | Day 1 | Day 5 | Day 5: correctly explained full mechanism (stack obj dtor guarantee → calls delete) unprompted — recovered from Day 4's acronym slip |
| `new`/`delete`, leaks | 🟢 | Day 1 | Day 2 | verified leak count/size w/ real tool, understands allocator overhead |
| `std::unique_ptr` | 🟢 | Day 2 | Day 4 | consistent correct reasoning across 2 sessions; interleaved successfully into Day 4 coding rep |
| `std::shared_ptr` | 🟢 | Day 3 | Day 4 | ref-counting mechanism derived correctly AND live-verified via real `use_count()` output Day 4; grasped handle-vs-object distinction unprompted |
| `std::weak_ptr` | ⚪ | — | — | |
| Dangling / use-after-free | 🟡 | Day 5 | Day 5 | timing (dangles at scope-exit, not at use) + UB's "silently works" danger both understood after correction; fix patterns (return by value; return smart ptr by value) reasoned correctly, one w/ syntax slips |
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
