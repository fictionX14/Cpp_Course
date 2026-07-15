# Skills Tracker

Mastery matrix for every C++ concept we touch. Update at the end of each session.

**Legend:** ⚪ Not started · 🟡 Learning · 🟢 Solid · 🔵 Mastered (cold quiz passed)

## Phase 0 — Memory & Ownership
| Concept | Status | First seen | Last reviewed | Notes |
|---|---|---|---|---|
| Stack vs heap | 🟡 | Day 1 | Day 6 | Day 6: applied-format check still opened with a fumble (mislabeled plain `int` as heap, conflated pointer-var with pointee) — but the precise rule ("name=stack always; only new/make_X=heap") + a diagram produced TWO clean cold transfers to a novel type (string), zero help. Real recovery signal. NOT yet graduated — needs one fully clean review with no hints before advancing to weak_ptr |
| RAII | 🟢 | Day 1 | Day 5 | Day 5: correctly explained full mechanism (stack obj dtor guarantee → calls delete) unprompted — recovered from Day 4's acronym slip |
| `new`/`delete`, leaks | 🟢 | Day 1 | Day 2 | verified leak count/size w/ real tool, understands allocator overhead |
| `std::unique_ptr` | 🟢 | Day 2 | Day 4 | consistent correct reasoning across 2 sessions; interleaved successfully into Day 4 coding rep |
| `std::shared_ptr` | 🟢 | Day 3 | Day 4 | ref-counting mechanism derived correctly AND live-verified via real `use_count()` output Day 4; grasped handle-vs-object distinction unprompted |
| `std::weak_ptr` | ⚪ | — | — | |
| Dangling / use-after-free | 🟡 | Day 5 | Day 6 | Day 6: both fix patterns actually COMPILED and RUN for real (verified `leaks`-style, matching Day 2 discipline); self-caught + self-fixed a `unique_ptr` print-address-vs-dereference bug; observed the compiler's own `-Wreturn-stack-address` warning catch the exact Day 5 bug live. Timing reasoning had one slip (invoked "heap" for a pure stack-lifetime case) — corrected, generalized dangling into stack-lifetime vs heap-lifetime flavors |
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
