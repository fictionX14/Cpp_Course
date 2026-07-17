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
| `std::weak_ptr` | 🟢 | Day 7 | Day 8 | Day 8: `Track`/`Sensor` cycle fix written, compiled, and run for real. Self-caught own bug (`weak_ptr` assignment via `.lock()` — a no-op on a temporary, corrected to direct assignment). Output confirmed both destructors fire, and correctly traced the exact cascade (reverse-declaration-order + member-teardown) explaining why |
| Dangling / use-after-free | 🟢 | Day 5 | Day 8 | GAP CLOSED Day 8: wrote a real heap-lifetime use-after-free, compiled under AddressSanitizer, read the full report precisely (alloc/free/bad-read line numbers). Both flavors (stack-lifetime Day 5-7, heap-lifetime Day 8) now demonstrated and tool-verified |
| Double free | 🟡 | Day 2 | Day 3 | reasoned correctly both times but conflated "UB" with "compile error" once — watch this; last remaining non-🟢 Phase 0 item |

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
