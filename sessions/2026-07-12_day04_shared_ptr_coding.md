# Day 04 — shared_ptr Verified, unique_ptr Interleaved, Regression Catch  (2026-07-12)
Phase 0 · ~30 min

## Learned
- **Backfilled/closed out Day 3's log** (2026-07-09, shared_ptr fundamentals) — was left as a skeleton; retroactively completed at the start of this session so the record is honest.
- **The review system caught two real regressions.** Stack/heap and RAII were both marked 🟢 Solid after Day 2 — today's cold warm-up showed both had drifted:
  - **Stack vs heap:** swapped which one is manual vs. automatic (said heap cleans up automatically "like locally defined variables," said stack requires manual freeing — backwards). Corrected: stack = always automatic, guaranteed by the compiler at scope end; heap = always manual by default, only becomes automatic when wrapped in a stack object's destructor (RAII), which is a technique layered on top, not a property of the heap itself.
  - **RAII acronym:** said "Resource **Allocation** is Initialization" — wrong word, it's **Acquisition**. Flagged as a realistic MCQ distractor (a plausible-sounding wrong term).
  - Both demoted 🟢→🟡 and reset to a 1-day review interval. This is the review system working as designed — "solid" means "solid as of the last time it was tested," not permanent.
- **Exceptions and temp-object-lifetime both held up** on cold recall — intervals grew.
- **Ran the deferred Day 3 coding exercise for real**, interleaved with `unique_ptr` per the new interleaving policy (one file, one owner via `unique_ptr`, one shared scenario via `shared_ptr`). Predictions from Day 3 were verified against actual program output.
- **Key insight, self-surfaced via a great question:** the program only printed "Sensor acquired"/"released" **twice**, not three times, despite three variable names (`owner`, `a`, `track_ref`). Resolved: a variable name is just a *handle*; the number of handles has nothing to do with the number of constructed objects. `track_ref = a;` is a `shared_ptr` **copy** — it doesn't construct a new `Sensor`, it just adds a second handle to the *same* object and bumps the reference count. The `use_count()` jump from 1→2 at that exact line is the proof: if `track_ref` had created its own object, the count would have stayed at 1. This is the entire mechanism of shared ownership, seen firsthand instead of just reasoned about.

## Q&A / Quiz  → score: 2/4 warm-up correct, 2 regressions caught; coding predictions verified
- Stack vs heap: ❌ swapped manual/automatic — corrected, demoted, reset interval.
- RAII: ❌ acronym misnamed ("Allocation" vs "Acquisition") — corrected, demoted, reset interval.
- Exceptions (invisible exit path): ✅ correct.
- Temp-object lifetime: ✅ correct.
- Coding predictions (use_count sequence 1, 2, 1; which variable dies first): verified against real output — matched Day 3's corrected prediction.
- Follow-up ("why only 2 constructions for 3 variable names?"): self-initiated, sharp, fully resolved in-session — handle vs. object distinction.

## Code written
- `code/phase0-memory/day04.cpp` — `unique_ptr` ownership (`createSensor()`), interleaved with the `shared_ptr`/`track_ref`/`use_count()` scenario deferred from Day 3. Compiled and run; output matched corrected predictions.

## Tripped me up
- Stack vs. heap manual/automatic swap — despite being "solid" after Day 1–2, the labels flipped under a fresh cold ask. Worth extra spaced reps, not just re-teaching once.
- RAII acronym precision ("Allocation" vs "Acquisition") — a naming-only slip, mechanism understanding was intact.

## Wins
- Caught and self-corrected the "3 handles = 3 objects" assumption in real time once shown the `use_count()` evidence — didn't just accept the answer, understood *why* the count proved it.
- Successfully closed a multi-day-deferred exercise (Day 3 → Day 4) and validated predictions against real compiled output, same "prove it, don't just reason about it" discipline as Day 2's `leaks` run.

## Flagged for review  → progress/review-queue.md
- Stack vs heap, RAII — reset to 1-day interval given today's regression. Highest priority for tomorrow's warm-up.
- `shared_ptr`, `unique_ptr`, scope-lifetime tracking, `use_count()` safety — all live-verified today, intervals grown.

## Next session (Day 5 — long session, ~60 min planned)
- Priority warm-up: re-test stack/heap and RAII (regression check).
- New material: dangling pointers / use-after-free (deferred from Day 4 due to time).
- Given the longer time budget, possibly also start `std::weak_ptr` (breaking reference cycles) — the last major Phase 0 gap.
- Consider whether Phase 0's integration checkpoint is close enough to schedule.
