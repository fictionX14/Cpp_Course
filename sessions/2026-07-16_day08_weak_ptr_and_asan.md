# Day 08 — weak_ptr Coded + Heap-Lifetime Dangling Closed via ASan  (2026-07-16)
Phase 0 · ~30–35 min · pure coding day, no new concepts

## Learned
- **RAII quick check:** ctor/dtor pairing for a non-memory resource (`FILE*`) correct (`fopen`/`fclose`), but the "guarantee" piece — that release only fires because the wrapper is a *stack-resident* object — was omitted until prompted. Held the review interval flat rather than growing it.
- **`weak_ptr` cycle fix — written, compiled, verified for real.** Built `Track` (owns `Sensor` via `shared_ptr`) / `Sensor` (observes `Track` via `weak_ptr`).
- **Self-caught a real bug:** wrote `sensor->owner.lock() = tracker;`, intending to link the back-reference. Diagnosed (with a nudge) that `.lock()` is a *read* operation — it returns a temporary `shared_ptr`, and assigning to that temporary is a silent no-op that never touches `sensor->owner`. Corrected to direct assignment: `sensor->owner = tracker;` (`weak_ptr` has its own assignment operator accepting a `shared_ptr` — no `.lock()` needed for linking, only for later safe access).
- **Traced the exact reference-count mechanism** for why this version doesn't leak like the all-`shared_ptr` cycle: `Track`'s count is only touched by real `shared_ptr` owners (`weak_ptr` never counts), so the local `tracker` going out of scope correctly drops it to 0. `Sensor`'s count depends on two real owners (`sensor` local + `tracker->sensor`), both of which disappear by the end of `main()`.
- **Output confirmed and explained precisely:** `"Sensor's owner is still alive"` → `"Track Destroyed"` → `"Sensor Destroyed"`, in that exact order. Traced *why*: reverse-declaration-order destroys the local `sensor` first, but that only drops `Sensor`'s count 2→1 (no print — `tracker->sensor` still holds it). `tracker` destructs next, `Track`'s count hits 0 → prints, and tearing down `Track` also destroys its `sensor` member, dropping `Sensor`'s count 1→0 → prints immediately after, same cascade.
- **Heap-lifetime dangling — the Day 7 gap, closed for real.** Wrote a genuine use-after-free (`new int(42); delete p; std::cout << *p;`), compiled under **AddressSanitizer** (`-fsanitize=address -g`), and read a full ASan report.
- **Corrected a prediction en route:** guessed "it'll crash since it's UB" — flagged precisely: **UB itself never guarantees a crash** (Day 6's silent-failure lesson still holds for an *uninstrumented* build). The reliable, loud failure here comes specifically from ASan being built to catch this exact bug class deterministically — a different claim than "UB crashes."
- **Read the ASan report as a full bug biography:** `previously allocated ... :49` (the `new`), `freed by ... :50` (the `delete`), `READ of size 4 ... :52` (the bad dereference) — three exact line numbers giving the complete lifecycle of the bug, plus the error explicitly named `heap-use-after-free`, plus `ABORTING` confirming deterministic-under-ASan termination.

## Q&A / Quiz  → score: strong, both coding reps landed with real self-correction
- RAII quick check: mechanism correct, guarantee omitted until prompted.
- weak_ptr prediction (do both destructors fire): ✅ correct conclusion, admittedly unsure of the *why* until walked through it — then correctly traced the full cascade from real output.
- ASan prediction (crash vs. something else): partially corrected — right that it fails loud, wrong initial reasoning (attributed to "UB" generally rather than ASan specifically).

## Code written
- `code/phase0-memory/day08.cpp` — `Track`/`Sensor` reference-cycle fix with `weak_ptr` + `.lock()` safety check. Compiled and run; output matched and explained.
- Heap-lifetime use-after-free example (same file or day08b, per session) — compiled under `-fsanitize=address -g`; ASan report obtained and read in full.

## Tripped me up
- Confusing `.lock()` (read/convert-to-shared_ptr) with an assignment mechanism — a real, self-corrected bug, not just a warm-up quiz miss.
- Reflexively attributing "it'll crash" to UB in general rather than to ASan's specific deterministic instrumentation — a precise but important distinction from Day 6.

## Wins
- **Both Day 7 gaps closed in one session, with real compiled/run proof, not just verbal correction:** `weak_ptr` coded end-to-end; heap-lifetime dangling demonstrated and caught live by tooling.
- Self-diagnosed the `.lock()`-as-assignment bug with minimal help — genuine debugging rep, not just following instructions.
- Correctly traced a nontrivial destruction cascade (reverse-declaration-order + member-teardown interacting) against real program output.
- Read a full, unfamiliar ASan report cold and extracted the right information (three line numbers, the bug-class name) without being walked through every section.

## Flagged for review  → progress/review-queue.md
- RAII — interval held flat, not grown; the "guarantee" piece needs to come out unprompted next time.
- Double free — now the **last remaining non-🟢 Phase 0 concept**. One more clean rep and Phase 0's integration checkpoint can be scheduled.

## Next session
- **Double free** — one more targeted rep (ideally compiled/verified, matching today's discipline) to close out the last open Phase 0 item.
- Once double free is solid: **schedule Phase 0's integration checkpoint** — a cumulative problem pulling together RAII, `unique_ptr`, `shared_ptr`, `weak_ptr`, dangling/use-after-free (both flavors), stack/heap, all in one exercise. Nearly every prerequisite is now 🟢.
- After the checkpoint passes: begin Phase 1 (value semantics + STL) per the roadmap.
