# Problem-Solving Framework (run this when you freeze)

When you open a problem and don't know where to start, run these steps **in order**. Do NOT jump straight to coding.

1. **READ & RESTATE** — read it twice. Say it back in your own words. What's the *input*? What's the *output*?
2. **ASSUMPTIONS / EDGE CASES** — write them as comments before coding: empty input? duplicates? negatives? already sorted? size limits? what do I return when there's no answer? (Auto-graders hide test cases exactly here.)
3. **EXAMPLE BY HAND** — walk the given example manually, on paper/in your head. Make sure you understand why the expected output is what it is.
4. **BRUTE FORCE FIRST** — write the simplest correct solution, even if slow. A working O(n²) that passes most tests beats an unfinished "optimal" one. **Partial credit is per-test-case.**
5. **TEST** — run against the example + your edge cases. Fix bugs.
6. **OPTIMIZE (only if time + needed)** — find the bottleneck, apply a pattern: hash map (lookups), two pointers / sliding window (arrays), sorting, heap, BFS/DFS.
7. **TRIAGE the exam** — 90 min / 3 problems ≈ 30 min each. Stuck >~20 min? Lock in partial credit and MOVE ON. Come back if time remains.

Golden rule: **Correct-but-slow that runs > elegant-but-broken that doesn't.**
