# Day 07 — Stack/Heap Gate Cleared, weak_ptr Introduced  (2026-07-15)
Phase 0 · ~30–40 min

## Learned
- **Stack/heap — the gate cleared.** Fresh cold check (`double val`, `double* ptr = new double(...)`, `shared_ptr<double> sp = make_shared<double>(...)`), zero hints, zero diagram. Fully correct: universal "names live on stack" stated unprompted, heap correctly limited to only what `new`/`make_shared` created, **no conflation of pointer-variable vs. pointee** (the specific Day 6 opening error) — that distinction is now solid. First fully clean cold pass since the Day 4 regression began. **Graduated to 🟢 in the skills tracker.**
- **Combined review — `shared_ptr`/scope-lifetime/`use_count()`:** a global/local sharing scenario, predicted counts (1, 2, 1) all correct. Causal reasoning on the final count needed one precision fix: `global_ref` doesn't "gain ownership" when `local` goes out of scope — it already became a co-owner earlier at the assignment; the count drops because `local`'s destructor runs, not because the survivor does anything new. Same mechanism as Day 3's `Sensor`/`track_ref`.
- **`use_count()` / dereference safety on an *empty* `shared_ptr`, revisited:** correct conclusions both times (safe to query, unsafe to dereference) but the *reasoning* needed tightening twice: "safe because stable" → corrected to "safe because it never touches the managed object, full or empty, always." "Unsafe because it went out of scope" → corrected to "unsafe because there was never a target in the first place" (same UB category as a null-pointer dereference, not a lifetime-expiry bug).
- **Dangling pointer generalization, partially closed:** the **stack-lifetime** flavor (function returns address of a local) is fully internalized — reused correctly and unprompted. The **heap-lifetime** flavor (use-after-free: `delete p;` without nulling `p`) was asked for directly and **not produced** — gave the stack example again instead. Real, specific gap: the *label* "stack-lifetime vs. heap-lifetime dangling" is understood, but the heap-lifetime *instance* hasn't been independently generated yet.
- **`std::weak_ptr` — new material.** Motivated via a reference-cycle scenario: if `Track` owns `Sensor` via `shared_ptr` and `Sensor` owns `Track` back the same way, both counts stay ≥1 forever even after every external reference is gone — a leak that happens *because* `shared_ptr` was used "correctly." **Correctly reasoned the exact mechanism unprompted**, without being shown it: neither can hit 0 because the other is still holding it up.
- **The fix:** `weak_ptr` observes without owning — doesn't increment the count. Pick one direction of a relationship to be real ownership (`shared_ptr`), the other to be observation (`weak_ptr`).
- **Safe access to a `weak_ptr`'s target:** first instinct was `use_count()`-then-dereference — a real anti-pattern, corrected. The two steps (check, then use) aren't atomic; nothing guarantees the object survives the gap between them. The actual tool is **`.lock()`**: atomically returns either an owning `shared_ptr` (if the target's alive — and holding it keeps it alive for the check's duration) or an empty one (if it's gone). Connected cleanly to Day 3's rule: `.lock()`'s result is just a `shared_ptr`, and boolean-checking a `shared_ptr` is already known-safe.

## Q&A / Quiz  → score: strong, with one real gap surfaced
- Stack/heap cold gate check: ✅ fully clean, unprompted — graduated.
- Combined `shared_ptr`/scope/`use_count` scenario: ✅ all 3 predictions correct, one reasoning imprecision corrected.
- Empty-`shared_ptr` safety (query vs. dereference): ✅ conclusions correct, reasoning needed tightening twice.
- Heap-lifetime dangling example: ❌ gave the stack-lifetime example again — did not independently produce the heap-lifetime flavor. Flagged as a dedicated gap, not just a review-interval reset.
- Reference-cycle mechanism (why neither count hits 0): ✅ correct, unprompted, no hints.
- Safe `weak_ptr` access: ❌ first instinct (`use_count()`-then-dereference) was a real anti-pattern — corrected to `.lock()`.

## Code written
- None this session — entirely verbal/reasoning. **Coding rep deliberately deferred to Day 8** (see below) rather than rushed into the last few minutes.

## Tripped me up
- Producing the heap-lifetime dangling *instance* on demand, despite correctly understanding the stack-lifetime one and the general vocabulary distinguishing the two.
- Reaching for `use_count()` as a "check before use" pattern for `weak_ptr` — a reasonable-sounding instinct that's actually unsafe (non-atomic), vs. the correct atomic `.lock()`.

## Wins
- **Stack/heap fully graduated** — the multi-day regression (Days 4–6) is resolved, confirmed via a genuinely fresh, hint-free scenario.
- Derived the reference-cycle mechanism for `weak_ptr` entirely from first principles, before any explanation was given — strong transfer from the `shared_ptr` ref-counting mental model built over Days 3–4.
- Immediately connected `.lock()`'s return value back to Day 3's "boolean-checking a smart pointer is always safe" rule, once shown it — fast integration of new material with old.

## Flagged for review  → progress/review-queue.md
- **Dangling pointer: heap-lifetime flavor — top priority.** Real, specific gap. Plan: write and compile a genuine use-after-free example, run it under AddressSanitizer to see it caught live (mirrors Day 6's `-Wall` catching the stack-lifetime bug) — this also closes the long-pending "Practice/debug: read basic ASan/UBSan output" item on the Notion roadmap.
- `weak_ptr` — mechanism understood, but zero code written yet. Needs a real coding rep before it can be called solid.
- `use_count()`/dereference reasoning precision — modest gap, keep in rotation.

## Next session (Day 8) — coding rep plan
1. Write the `Track`/`Sensor` reference-cycle fix for real: `Track` owns via `shared_ptr`, `Sensor` observes via `weak_ptr`, with a `.lock()` check before use. Compile and run.
2. Write a genuine heap-lifetime use-after-free example (`delete p;` without nulling, then dereference) and compile it **with AddressSanitizer** (`-fsanitize=address -g`) to watch it get caught live — closes both the Day 7 gap and the outstanding Notion checklist item.
3. If both land cleanly, Phase 0's integration checkpoint becomes realistic to schedule in the next session or two — nearly every Phase 0 concept is now 🟢 or a well-defined 🟡 with a specific closing task, not an open-ended gap.
