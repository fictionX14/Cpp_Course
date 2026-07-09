# Skills Tracker

Mastery matrix for every C++ concept we touch. Update at the end of each session.

**Legend:** ⚪ Not started · 🟡 Learning · 🟢 Solid · 🔵 Mastered (cold quiz passed)

## Phase 0 — Memory & Ownership
| Concept | Status | First seen | Last reviewed | Notes |
|---|---|---|---|---|
| Stack vs heap | 🟢 | Day 1 | Day 2 | cold-recalled correctly incl. new/delete pairing |
| RAII | 🟢 | Day 1 | Day 2 | cold-recalled w/ minor phrasing fix; proved it live w/ `leaks` tool |
| `new`/`delete`, leaks | 🟢 | Day 1 | Day 2 | verified leak count/size w/ real tool, understands allocator overhead |
| `std::unique_ptr` | 🟡 | Day 2 | Day 2 | ownership, `make_unique`, move-only enforced at compile time; reasoned double-free risk unprompted |
| `std::shared_ptr` | ⚪ | Day 2 (teaser) | — | full session pending Day 3 |
| `std::weak_ptr` | ⚪ | — | — | |
| Dangling / use-after-free | ⚪ | — | — | |
| Double free | ⚪ | — | — | |

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
