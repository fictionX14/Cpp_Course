# Day 06 — Stack/Heap Recovery + Compiled Dangling-Pointer Fixes  (2026-07-14)
Phase 0

## Learned
- **New reinforcement format for stack/heap** (after two straight verbal fumbles): given a concrete snippet (`int a`, `int* b = new int(20)`, `unique_ptr<int> c = make_unique<int>(30)`) and asked to label each variable, rather than answered an abstract "who's responsible" question. This surfaced a *more precise* diagnosis of the confusion than before.
- **Opening fumble revealed the real root cause:** mislabeled a plain `int` (`a`) as heap, and — the more important catch — **conflated a pointer *variable* with what it points to** (said `b` "lives on heap" when the pointer itself is a stack local; only the `int(20)` it targets is heap). This is a different, more specific issue than the "does the heap clean itself up" framing from Days 4–5.
- **The decisive rule, taught explicitly:** a variable's *name* is always on the stack — no exceptions, regardless of type. The only thing that can be on the heap is memory created by an explicit `new`/`new[]`/`make_unique`/`make_shared`/`malloc` call, reached *through* a stack-resident name. Reinforced with a text diagram (stack column of names + arrows to heap-column targets where applicable).
- **Real transfer confirmed twice, cold, no help:** (1) correctly restated the rule unprompted ("new or new[] or make"); (2) applied it cleanly to a brand-new type (`std::string s`, `std::string* p = new std::string(...)`) with zero prompting or diagram in sight — distinguished `s` (stack, no allocation) from `p` (stack pointer) from the heap-allocated string target, and correctly named `delete` as the fix. This is the first *clean, unprompted, novel-example* pass since the Day 4 regression began.
- **Dangling pointer timing, re-tested:** correct on *when* (before the caller receives the pointer), but the *reasoning* wrongly invoked "heap not being allocated properly" — for an example (`int x = 42; return &x;`) that has zero heap involvement. Corrected and **generalized**: dangling isn't inherently a heap concept — it just means the pointer outlived its pointee, which can happen via **stack-lifetime ending** (today's example) or **heap-lifetime ending** (classic use-after-free). Same danger, two different triggers, now distinguishable using the stack/heap rule just re-established.
- **UB's silent-failure danger:** cleanly correct this time — "temporarily passes, fails later when the memory is needed elsewhere." Interval grows.
- **Closed Day 5's deferred coding rep — actually compiled and run, not just reasoned through.** Wrote `getValueBroken()` (the bug), `getValueFixed1()` (return by value), `getValueFixed2()` (return `unique_ptr` by value).
- **Self-inflicted, self-fixed bug:** first run printed a raw address (`0x7f80d9705dc0`) instead of `42` — caused by `std::cout << fixed2` (printing the `unique_ptr` itself) instead of `std::cout << *fixed2` (dereferencing it). Same "a pointer's value is an address" rule from Day 5, now caught in real code.
- **Compiler-as-code-reviewer, seen live:** compiling with `-Wall` produced `warning: address of stack memory associated with local variable 'x' returned [-Wreturn-stack-address]` on `getValueBroken()` — the exact bug from Day 5, flagged by the compiler with zero test cases or tooling. Established `-Wall -Wextra` as a default-on compile habit going forward — a real, free chunk of the code-review section is "read what the compiler already told you."

## Q&A / Quiz  → score: rocky start, strong finish
- Stack/heap applied check (a/b/c labeling): ❌ opening attempt — `a` mislabeled heap, `b`'s pointer-vs-pointee conflated. Corrected via rule + diagram.
- Stack/heap rule restatement: ✅ correct, unprompted.
- Stack/heap applied to a **novel type** (string, no diagram): ✅ fully correct, no help — the real recovery signal.
- Dangling pointer timing: ✅ conclusion correct, ❌ reasoning incorrectly invoked "heap" for a heap-free example — corrected + generalized.
- UB's silent-danger: ✅ correct.
- Coding rep: both fixes eventually compiled correctly; one self-made bug (print pointer vs. dereference) self-diagnosed after a nudge; observed and correctly interpreted the compiler warning.

## Code written
- `code/phase0-memory/day06.cpp` — `getValueBroken()` (uncalled, deliberately triggers `-Wreturn-stack-address`), `getValueFixed1()` (by-value return), `getValueFixed2()` (`unique_ptr` by-value return). Compiled and run successfully after self-fixing a dereference bug; separately compiled with `-Wall` to observe the compiler warning.

## Tripped me up
- Conflating a pointer *variable's* storage location with its *pointee's* storage location — a sharper, more specific version of the ongoing stack/heap confusion.
- Reflexively invoking "heap" as the explanation for a dangling-pointer bug that was actually pure stack-lifetime — a sign the stack/heap distinction, while improving, isn't fully automatic yet.
- Printed a smart pointer directly instead of dereferencing it — a real, easy-to-make bug, self-caught.

## Wins
- **First clean, unprompted, novel-example pass on stack/heap since the Day 4 regression began** — applied the rule to a type (`std::string`) that had never been used in any prior stack/heap exercise.
- Actually compiled and ran code instead of stopping at verbal reasoning — closed Day 5's carryover for real.
- Watched the compiler independently confirm a bug reasoned about two sessions ago, and understood *why* the warning fires (`-Wreturn-stack-address` = exactly the stack-lifetime rule, machine-enforced).

## Flagged for review  → progress/review-queue.md
- **Stack vs heap — still the top-priority item.** Needs a fully clean COLD pass tomorrow (no diagram, no hints) before `weak_ptr` starts, per the two-consecutive-clean-review gate. Today counts as one recovery, not yet one clean pass.
- Dangling pointer timing — reasoning slip (heap invoked where none existed), reset for review.
- UB silent-danger — interval grows, held cleanly.

## Next session
- **Gate check:** cold, hint-free re-test of stack vs heap. If clean → begin `std::weak_ptr` (last major Phase 0 concept) and Phase 0's integration checkpoint comes into view. If not clean → hold and iterate on reinforcement format again.
- Adopt `-Wall -Wextra` as default compile flags for all future coding reps.
