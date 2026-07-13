# C++ Competency Roadmap

> Living reference for navigating Chandler's C++ study plan (Anduril — Software Engineer, Robotics Tracking & Fusion / Battlespace Awareness).
> The framework below is logged word-for-word from the coaching session; we tweak as we go.

---

## THE FRAMEWORK (verbatim)

Good — this is the right question, and it's the one that actually controls every other outcome we've talked about. Let me give you a real regimen, honestly calibrated to where you sit.

### Where you actually are right now

Don't lose sight of this: **your algorithmic engine is solid** (8/10 from-scratch, and you own BFS/DFS, hash maps, two pointers, heaps, sorting, bitmask-state). Your gap is **not** "can I code" — it's specifically **C++ as a systems language**: memory/ownership, value semantics, concurrency, and reading/critiquing unfamiliar code. That's a *narrower and more trainable* gap than "learn to program in C++." You're not starting at zero; you're starting at "strong problem-solver who hasn't yet internalized C++'s hard parts."

### Two different finish lines (be honest about which you mean)

- **Assessment-competent** (pass these C++ HackerRanks *confidently*, not by luck): **~6–10 weeks** of daily work.
- **Strong C++ SWE** (hold your own on a real systems/robotics team — comfortable with memory, concurrency, idioms; can build and debug real C++): **~6–12 months** of consistent practice, ideally with real project work in the back half.

They're the same road; the first is a milestone on the way to the second.

### Foundational vs. application: the ratio *shifts*

The key insight: you can't "apply" what you don't yet have, so early weeks lean foundational — **but never pure theory.** Every concept gets learned by writing a *broken* version and a *fixed* version. That single habit double-serves you: it builds real understanding *and* it's literally the code-review MCQ skill.

Here's how the mix moves:

| Phase | Weeks | Foundational : Application | Focus |
|-------|-------|---------------------------|-------|
| **0 — Memory & ownership** | 1–2 | 80 : 20 | Stack/heap, RAII, smart pointers (unique/shared/weak), dangling/use-after-free/double-free |
| **1 — Value semantics + STL** | 3–5 | 70 : 30 | Copy vs move, Rule of 0/3/5, const-correctness, refs vs ptrs; STL containers + complexity + iterator invalidation; lambdas |
| **2 — Concurrency + UB** | 6–8 | 60 : 40 | threads, mutex/lock_guard, atomics, condition_variable, data races/deadlock; overflow, signed/unsigned, lifetime gotchas → **now assessment-ready** |
| **3 — Build & apply** | Mo. 3–4 | 40 : 60 | A real C++ project + DSA-in-C++ cold reps |
| **4 — Depth & domain** | Mo. 5–6+ | 30 : 70 | Reading real codebases + tracking/fusion math (linear algebra, Kalman filters, coordinate frames) |

Notice Phase 2 is the assessment finish line. Phases 3–4 are what turn "passed the test" into "actually strong."

### Daily cadence (the part that determines success)

**60–90 min/day, 5–6 days/week.** You have 3–4 hours, but consistency crushes volume — a sustainable daily habit you keep for 6 months beats a heroic week you burn out on. Bank the extra hours on days you *want* to; don't build the plan around needing them.

A daily block:
- **~15 min** — spaced review of prior concepts, flashcard-style (RAII, move semantics, "what's the bug here"). This is what makes it *stick*.
- **~35 min** — one new foundational concept, learned as **broken code → fixed code**.
- **~25 min** — application: a C++ coding problem cold, or project work.
- **~10 min** — log what tripped you up to `Cpp_Systems_Review.md`.

**Weekly:** one consolidation/review session, and a **short timed mock every 1–2 weeks** (mirror the 6 MCQ + 8 code-review + 1 coding format so the clock stops being scary).

### The multiplier: build something real (Phase 3+)

A small **tracking-themed C++ project** — say a 2D object tracker with a basic Kalman filter, or a spatial data structure — does triple duty: forces the foundations into muscle memory, gives you a portfolio proof point for *any* application, and puts the exact domain vocabulary (state estimation, coordinate frames) in your mouth for interviews. This is what separates you from every other <1-YOE applicant.

---

## ASSESSMENT FORMAT OBSERVED (Chandler's recall — targets for the plan)

The real HackerRank had three question types. Our prep must drill each in its actual format:

1. **Code Review — "find the bug, choose the answer that applies."** Single-answer multiple choice. Skill = *recognition* of the bug (not articulation). Highly gameable → drill bug-class recognition + common distractor patterns. (Maps to daily "broken code → fixed code" habit.)
2. **Coding problem.** Write-from-scratch. Already the strongest section (8/10). Maintain via cold C++ reps of owned patterns.
3. **Inline PR comment on the line(s) that need fixing.** Locate *where* the issue is in a file and comment PR-style. Skill = spot-the-line + concise call-out. Drill by reviewing snippets and pointing to the exact offending line + one-sentence why.

> Note: user's own "Phase 1/2/3" labels above refer to these three assessment sections, not the learning phases in the table. Keeping both framings; will reconcile as we build the week-by-week checklist.

---

## PHASE-END INTEGRATION CHECKPOINTS

Added 2026-07-09 (Day 3). Individual concepts graduate via the spaced-repetition review queue (`progress/review-queue.md`) — that stays the primary retention engine, tuned to test each concept right before it'd be forgotten, not everything every session (doesn't scale, wastes reps on things already solid).

But **working knowledge** — fluently combining *multiple* old concepts at once, the way a real code-review problem does — needs its own mechanism:
- **Daily (lightweight, free):** each session's coding rep quietly weaves in one prior concept alongside the new one, by design — not a bolted-on quiz, just built into the same problem.
- **Phase-end (the real gate):** before advancing to the next phase, a cumulative test/coding problem pulls together *everything* learned in the phase so far. Tracked in `progress/integration-checkpoints.md`. Must pass before the phase is marked done (in both the roadmap checklist and Notion).

## NOTION TRACKER

Built 2026-07-02: standalone Notion page **"C++ Competency Roadmap"** (under Private) — mirrors this file. Structure:
- Goal line + **Phases** checklist (Phase 0–4 as checkboxes).
- **Daily Log** inline database: columns = Date · Concept · Problem · Tripped me up · Done (checkbox). Seeded first row: "Jul 3 — Day 1 / Phase 0 — RAII & smart pointers".
- Workflow: this markdown file = source of truth (maintained every session); Notion = daily dashboard, synced via Chrome extension when coworking.

---

## TODO / NEXT

- [ ] Build the week-by-week checklist under each learning phase.
- [ ] Decide flashcard tool (Anki vs. plain doc) for the ~15-min daily review.
- [ ] Pick the Phase 3 project (leaning: 2D tracker + basic Kalman filter).
- [ ] Reconcile learning-phase numbering vs. assessment-section numbering to avoid confusion.
