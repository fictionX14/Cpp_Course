# Day 03 — std::shared_ptr  (2026-07-09)
Phase 0 · ~30 min

## Learned
- **`std::unique_ptr` copy is a compile-time error, not a runtime bug.** Corrected an over-answer: copying a `unique_ptr` doesn't "cause" UB at runtime — the copy constructor is `delete`d, so it never compiles. The double-free is prevented, not just risky.
- **Unnamed temporary refresher** (forgotten, re-taught): `ScopeLogger("x");` with no variable name dies at the end of that *statement*, not the enclosing scope. Naming it ties its lifetime to the function, which is why Day 2's exercise needed a named local to prove RAII across an early return.
- **`std::shared_ptr` — reference-counted shared ownership.** Unlike `unique_ptr`, copying is *allowed* and *is* the mechanism: copy → count +1, destroy/reset → count −1, hits 0 → object deleted. No concept of an "original" owner — the count only tracks how many owners exist right now, order doesn't matter.
- **Methodology decision (not a C++ concept, a course-design one):** agreed to add phase-end integration checkpoints + daily interleaving of old material into new coding reps, on top of the existing spaced-repetition review queue. Documented in README.md, Cpp_Competency_Roadmap.md, and new `progress/integration-checkpoints.md`.
- **`.use_count()` on an empty/reset `shared_ptr` is well-defined (returns 0), not UB.** Key distinction: querying a smart pointer's own bookkeeping (`use_count()`, `get()`, boolean check) is always safe on an in-scope `shared_ptr`. UB only appears when *dereferencing* (`*ptr`, `ptr->x`) an empty one — same danger zone as a raw null pointer.

## Q&A / Quiz  → score: strong, one scope-reasoning slip
- Double-free / copy question: partially corrected — right consequence (UB), needed the "compile error, not runtime" correction.
- Temp-object-lifetime: forgotten, successfully re-taught.
- Prediction exercise (`Sensor`/`track_ref`/`use_count()`, reasoned verbally, not yet run): predicted counts of 1, 2, 1 — first two correct; final count reasoning had the wrong variable dying (said `track_ref` was scoped to the inner block and got destroyed; actually `track_ref` is declared in `main()`'s outer scope and survives — it's `a` that dies at the block's end). Numeric answer (1) was still right, by symmetry, but the reasoning was corrected.
- UB question (self-initiated, sharp): "would `.use_count()` on a freed struct be UB?" → answered: not UB, well-defined, returns 0; UB is reserved for dereferencing an empty pointer, not querying it.

## Code written
- None — the `Sensor`/`track_ref`/`use_count()` snippet was designed and predicted on but **explicitly deferred** ("will code out tomorrow"). Carried forward to Day 4.

## Tripped me up
- Swapped which variable's scope ends first (`track_ref` vs `a`) in the prediction exercise — a real scope-tracking slip, not a conceptual gap in ref-counting itself.

## Wins
- Correctly derived the reference-count mechanism unprompted, from just the ownership scenario (no `unique_ptr` analogy given).
- Asked a genuinely sharp follow-up (use_count-on-empty UB question) that surfaced an important, non-obvious C++ distinction.
- Drove a real improvement to the course's own methodology (interleaving + phase checkpoints) — good engineering instinct applied to the learning process itself.

## Flagged for review  → progress/review-queue.md
- `shared_ptr` fundamentals (copy-based ref counting, `use_count()`) — new, review next session.
- Scope-lifetime tracking (which variable dies first in nested blocks) — resurfaced gap, review soon.
- `use_count()` safety vs dereference UB — new, review next session.

## Next session (Day 4)
- Actually run the deferred `Sensor`/`track_ref` snippet — verify predictions against real output.
- Interleave: fold `unique_ptr` back into the same session (per the new interleaving policy) to close that review loop live instead of via a separate warm-up question.
- New material candidate: dangling pointers / use-after-free (natural extension of the UB-on-dereference discussion).
