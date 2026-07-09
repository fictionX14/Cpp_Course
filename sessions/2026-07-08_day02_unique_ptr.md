# Day 02 — std::unique_ptr & Move Semantics Teaser  (2026-07-08)
Phase 0 · ~45 min · branch `day02-unique-ptr`

## Learned
- **`std::unique_ptr`** — the STL's version of the `IntArray` wrapper from Day 1. Owns a heap resource, frees it automatically in its destructor. No manual `delete`, ever.
- **Exclusive ownership, enforced at compile time.** `unique_ptr`'s copy constructor is `delete`d — copying one is a compile error, not just bad practice. This is *stronger* than a human code-review catch: the compiler refuses the program.
- **`std::move`** — how ownership transfers between `unique_ptr`s. Not a byte-copy; it hands off ownership so only one owner (and one destructor call) ever exists at a time. Full depth deferred to Phase 1.
- **`std::shared_ptr`** — teaser only. Reference-counted, multiple owners, frees when the count hits zero. More overhead than `unique_ptr`; default to `unique_ptr` unless ownership is genuinely shared. Full session deferred.
- **`malloc`/`free`** (tangent, user-initiated) — C-style raw allocation, no constructor/destructor hookup, not RAII-compatible. Never mix allocator families (`new`↔`delete`, `new[]`↔`delete[]`, `malloc`↔`free`) — crossing them is UB.
- **Heap allocator overhead** — requested 100 ints (400 bytes) but `leaks` reported 416 leaked bytes. Extra 16 bytes = allocator bookkeeping/alignment (macOS rounds to 16-byte boundaries + stores a size header). You never get exactly what you ask the heap for.
- **Temporary object lifetime** — `ScopeLogger("x");` (unnamed) is destroyed at the end of that *statement*, not the enclosing scope — useless for proving RAII across a function. Must bind to a named local (`ScopeLogger log("x");`) so its lifetime is tied to the function's scope.
- **Member initializer list runs before the constructor body.** `: name_(name)` assigns `name_` before `{}` executes — so `name_` and `name` are already identical inside the body (caught a real duplicate-print bug caused by this).
- **Verified a leak with a real tool**, not just reasoning: `leaks --atExit -- ./day02` on macOS — proved `process_leaky(bool)` leaks exactly once (only the true/early-return branch) while the `unique_ptr` version leaked zero times regardless of input.

## Q&A / Quiz  → score: strong
- Warm-up Q1 (what RAII does + the guarantee): mostly correct — called it a "framework" (it's a pattern/idiom) and said "for every constructor there's a destructor" (true of any class, not the distinguishing trait). Corrected: destructor releases what constructor acquired, guaranteed to run on scope exit including unwinding.
- Warm-up Q2 (invisible exit path): ✅ correct — exceptions/`throw`.
- Warm-up Q3 (stack vs heap responsibility): ✅ correct, plus correctly paired `new`/`delete` vs `new[]`/`delete[]`.
- Double-free prediction (copying a `unique_ptr`): ✅ correctly reasoned UB before being shown the compiler actually forbids it outright.
- ScopeLogger prediction (does the destructor fire on early return): confirmed correct via actual program output.

## Code written
- `code/phase0-memory/day02.cpp` — `process_leaky(bool)` (Day 1's raw-pointer leak, kept as the "before" for comparison) vs `process_fixed(bool)` (`unique_ptr<int[]>` rewrite, zero leaks either branch, verified with `leaks`). `ScopeLogger` RAII class (acquire/release prints) proving dtor fires on both early-return and normal-exit paths.
- Not done this session: a separate `std::vector` rewrite of the leak (exercise was substituted by keeping the raw version as a comparison baseline against `unique_ptr`) — optional follow-up, low priority.

## Tripped me up
- `int` size: said "4 bits" instead of 4 bytes/32 bits — flagged as a real interview trap (bit vs. byte confusion).
- Function name collision (`process`/`process`) — self-diagnosed and fixed before I had to point it out.
- Duplicate print from `name_` + `name` in `ScopeLogger` — root cause (initializer-list timing) was non-obvious; self-navigated most of the way, needed the timing explanation.
- Initial confusion on "why does the leaky output look identical to the fixed output" — good, expected confusion; leaks are silent by design, that's the whole danger.

## Wins
- Independently reasoned through the double-free consequence of copying a `unique_ptr` before being told the answer.
- Compiled and used `leaks` (real tooling) unprompted-quickly once introduced — turned an abstract concept into an observed, measured result (416 bytes, exactly one leak, exactly the predicted function).
- Self-fixed two real bugs (name collision, duplicate print) with minimal hinting.
- Asked a sharp tangential question (malloc/free pairing) that connected cleanly back to why RAII/STL wrappers exist.

## Flagged for review  → progress/review-queue.md
- `std::unique_ptr` (ownership, move, compile-time-enforced exclusivity) — new today, review tomorrow.
- Temporary object lifetime (named vs unnamed) — new today, subtle, review tomorrow.
- Day 1 concepts (stack/heap, RAII, exceptions) — passed cold recall today, interval grows to 3 days.

## Next session (Day 3)
- `std::shared_ptr` proper (not just teaser): reference counting mechanics, `use_count()`, cost vs `unique_ptr`, when shared ownership is actually the right call.
- `std::weak_ptr` — breaking reference cycles.
- Optional cleanup: revisit the `std::vector` rewrite of the Day 1/2 leak example for completeness.
- Coding rep: a small scenario with two objects that need to share ownership of a third (a preview of tracking-system-shaped ownership problems).
