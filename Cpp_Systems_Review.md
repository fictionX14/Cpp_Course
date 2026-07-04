# C++ Systems Review — Living Doc

> **Purpose:** the running review/flashcard doc for the Anduril HackerRank (2nd attempt, ~2026-07-01).
> Grows every prep session. Items marked **🔑 CLARIFIED** are things I specifically asked to nail down.
> **How to use:** each morning, read the "Rule" + "Red flag" lines, then cover the code and re-derive the bug.

---

## The assessment (why this doc is weighted the way it is)

| # | Section | Qs | What it is |
|---|---------|----|-----------|
| 1 | General Questions | 6 | concept MCQs (recall) |
| 2 | **C++ Code Review** | **8** | read code, spot the bug — **biggest section** |
| 3 | Problem Solving (Intermediate) | 1 | write code (Capable-Models style) |

**14 of 15 questions are recognition-based.** This doc targets Sections 1 & 2. Section 3 lives in the coding notes.

---

## ⭐ THE BIG IDEA: RAII

> **RAII = Resource Acquisition Is Initialization.** Acquire a resource in a **constructor**, release it in a **destructor**, and let **scope** do the cleanup automatically. Whenever the object goes out of scope — normal exit, early `return`, or exception — the destructor runs and the resource is freed. You physically cannot forget.

This one idea is the answer to most of the Code Review section:

| Problem | Raw way (bug-prone) | RAII way (safe) |
|---------|--------------------|-----------------|
| memory leak | `new` / `delete` by hand | `std::vector`, `std::unique_ptr` |
| double-free | own a raw `int*` + destructor | `std::vector` |
| lock never released | `m.lock()` / `m.unlock()` | `std::lock_guard` |

---

## 1. Memory & object lifetime

### Stack vs heap (foundation)
- A **local variable lives on the stack** and is **destroyed the instant its function returns.**
- `new` puts data on the **heap**; it lives until you `delete` it (or a RAII type does).

### 🔑 CLARIFIED — pointer syntax (`p`, `*p`, `&p`)
```cpp
int  x = 42;    // x at address 0x200, holds 42
int* p = &x;    // p at address 0x500, holds 0x200
```
| Expr | Means | Value |
|------|-------|-------|
| `*p` | the **value** at the address p holds (dereference) | `42` |
| `p`  | the **address it points to** (what a pointer stores) | `0x200` |
| `&p` | the address of **the pointer variable itself** | `0x500` (an `int**`) |

- Mental model: `p` = "where's the data?", `*p` = "what's the data?", `&p` = "where does the pointer live?"
- Trap: `&p` is NOT the address of the data — plain `p` already is that. `&p` goes one level *up*.
- `*` and `&` are inverses: `*(&x) == x`, `&(*p) == p`.

### Bug: memory leak (`new` with no `delete`)
```cpp
auto* r = new std::vector<int>(100);   // ... never deleted → leak
```
**Fix:** return by value, or use `std::unique_ptr` / no `new` at all.
**Red flag:** a raw `new` whose matching `delete` you can't find on every exit path.

### Bug: dangling reference (returning a reference/pointer to a local)
```cpp
const std::string& getName(int id) {
    std::string name = "sensor_" + std::to_string(id);
    return name;   // name dies when the function returns → UB
}
```
- Returning a *pointer* to `name` has the **same** bug — it points at the dead local.
**Fix:** return **by value**:
```cpp
std::string getName(int id) { return "sensor_" + std::to_string(id); }
```
**Red flag (memorize):** *never return a reference or pointer to a local variable.*

### 🔑 CLARIFIED — shallow copy → double free (Rule of 3 / Rule of 0)
```cpp
class Buffer {
    int* data; size_t size;
public:
    Buffer(size_t n) : data(new int[n]), size(n) {}
    ~Buffer() { delete[] data; }      // custom destructor, but NO copy ctor/assign
};
Buffer a(10);
Buffer b = a;     // 💥
```
- A pointer is a **sticky note holding an address**, not the array itself.
- The auto-generated copy does a **shallow copy**: it copies the *address* (`b.data = a.data = 0x100`), so both objects point at the **same** array.
- At scope end both destructors run `delete[] 0x100` → the second is a **double free** → crash/UB.
- Test of understanding: after `Buffer b = a;`, `b.data[0] = 999;` also changes `a.data[0]` — same memory (aliasing).

**Fix A — Rule of Zero (preferred, what the test wants):** let a container own the memory.
```cpp
class Buffer {
    std::vector<int> data;          // vector deep-copies itself
public:
    Buffer(size_t n) : data(n) {}
    // no destructor, no copy ctor, no copy assignment needed
};
```
**Fix B — Rule of 3 (if forced to hold a raw `int*`):** custom destructor ⇒ also write copy ctor + copy assignment, both **deep** copies.
```cpp
Buffer(const Buffer& o) : data(new int[o.size]), size(o.size) {
    std::copy(o.data, o.data + size, data);
}
Buffer& operator=(const Buffer& o) {
    if (this == &o) return *this;   // self-assignment guard
    delete[] data;                  // free old array (else leak)
    size = o.size; data = new int[size];
    std::copy(o.data, o.data + size, data);
    return *this;
}
```
- Copy assignment is fussier than the copy ctor because the target **already owns** an array: guard self-assignment, free the old array first.
**Red flag (classic question):** a class with a **custom destructor but no copy ctor / copy assignment** → shallow copy → double free.
- *Rule of 5* = Rule of 3 + move ctor + move assignment (for performance). Recognition-level only; `std::vector`/`unique_ptr` already do moves for you — don't hand-write them for this test.

---

## 2. Concurrency

### Bug: data race
```cpp
int counter = 0;
void worker() { for (int i=0;i<100000;++i) counter++; }   // run on 4 threads
// expected 400000, sometimes less
```
- `counter++` is read-modify-write (3 steps), **not atomic**; threads interleave → lost updates.
**Fix:** `std::mutex` + `std::lock_guard`, or `std::atomic<int> counter`.
**Red flag:** shared mutable variable written by multiple threads with no lock/atomic.

### Bug: lock never released
```cpp
std::mutex m;
void update(std::map<int,int>& store, int k, int v) {
    m.lock();
    if (v < 0) return;   // 🚨 early return holds the lock forever
    store[k] = v;
    m.unlock();
}
```
- The early `return` skips `m.unlock()` → next `update()` call blocks forever (**deadlock**).
**Fix (RAII):**
```cpp
std::lock_guard<std::mutex> guard(m);   // locks now, unlocks in destructor on ANY exit
```
**Red flag:** manual `m.lock()` / `m.unlock()` with a `return`/`throw`/branch between them.

---

## Code-review red-flag checklist (scan for these)
- [ ] `new` / `new[]` without a guaranteed matching `delete` / `delete[]` on every path
- [ ] returning a reference or pointer to a LOCAL (non-static) variable
- [ ] custom destructor but missing copy ctor / copy assignment (Rule of 3)
- [ ] `delete` vs `delete[]` mismatch (array allocated with `new[]` must use `delete[]`)
- [ ] shared mutable state across threads with no mutex/atomic
- [ ] `m.lock()` with an exit path before `m.unlock()` (use `lock_guard`)
- [ ] use-after-free / double-free / dangling pointer
- [ ] base class destructor NOT virtual → derived destructor skipped on `delete Base*`
- [ ] dereferencing a `unique_ptr` / raw pointer AFTER it was moved from (null dereference)
- [ ] large object passed by value when only read → use `const T&`
- [ ] two mutexes acquired in opposite order across threads → deadlock
- [ ] `cv.wait(lock)` with no predicate → spurious wakeup bug
- [ ] `i <= v.size()` off-by-one (should be `<`) + signed/unsigned loop comparison

---

## Session 2 additions — pointers deep dive

### 🔑 CLARIFIED — `*` means two different things depending on context
```cpp
int* p = arr;   // DECLARATION: * is part of the TYPE (int*)
                // the = assigns to p, not to *p
                // p gets the address of arr[0]

*p = 5;         // EXPRESSION: * is the dereference operator
                // "go to the address p holds, write 5 there"
```
- In a declaration, the entire `int*` is the type. The variable is `p`. The `= arr` sets p to an address.
- In an expression, `*p` actively follows the arrow to the value.
- Same symbol, two jobs. Declaration = passive label. Expression = active operation.

### 🔑 CLARIFIED — why `int p = arr` doesn't work
- `arr` decays to an **address** (e.g. `0x100`). An address is not an integer value.
- `int p` is a box for values. `int* p` is a box for addresses. Types must match.
- The `*` in the type is what gives `p` the ability to dereference, do pointer arithmetic, and access the array.

### 🔑 CLARIFIED — pointer arithmetic moves by element, not by 1
```cpp
int arr[] = {1, 2, 3, 4, 5};
int* p = arr;    // p = 0x100, *p = 1
p += 2;          // p = 0x108 (moved 2 ints forward), *p = 3
```
- `p += 2` moves forward by `2 * sizeof(int)` bytes — lands on arr[2].
- UB only if you go past the end. Within bounds = defined behavior.
- `p[n]` is just syntactic sugar for `*(p + n)`.

### Plain int copy vs pointer — key distinction
```cpp
int val = 7;
int* p = &val;
int q = *p;     // q is a COPY of the value at *p
q = 99;         // only changes q, not val
std::cout << val;  // prints 7
```
- `*p` gives you the value — assigning it to a plain `int` makes a copy. The link is broken.
- To modify `val` through a function, pass `int*` or `int&`, not `int`.

### Uninitialized pointer = guaranteed UB
```cpp
int* p;         // garbage address
*p;             // UB — dereferencing unknown memory
```
Always initialize: `int* p = &x;` or `int* p = nullptr;` (then check before use).

### Complete pointer reference table
```cpp
int x = 42;
int* p = &x;
```
| Expression | Meaning | Value |
|------------|---------|-------|
| `x` | the int | `42` |
| `&x` | x's address | `0x200` |
| `p` | address p holds (= &x) | `0x200` |
| `*p` | value at that address | `42` |
| `&p` | p's own address | `0x500` |
- `*p` and `x` refer to the **same memory** — modifying one modifies the other.
- `&p` is NOT the address of x — it's where the pointer variable itself lives.

---

## Session 2 additions (rapid-fire)

### 🔑 CLARIFIED — `new`/`delete` must match `new[]`/`delete[]`
```cpp
int* arr = new int[50];
delete arr;          // 🚨 wrong: array needs delete[]
```
- `delete` on `new[]` memory is **undefined behavior** (not a clean "leak") — may free wrong amount / corrupt heap.
- Pairing rule: `new` ↔ `delete`, `new[]` ↔ `delete[]`. Never cross them.
**Red flag:** scalar `delete` on a pointer that was allocated with `new[]`.

### Dangling pointer: cause vs. crime scene
```cpp
int* makeCounter() { int count = 0; return &count; }   // cause: address of a local
// ...
*c = 5;   // crime scene: dereferencing the dangling pointer = UB
```
- On "select all that apply," **both** apply: "returns address of a local" AND "the later dereference is UB." Tick the cause *and* the consequence.

### 🔑 CLARIFIED — thread join ordering
- `join()` can only run on a thread that already exists/is running. Pattern: **create (starts) → do work → `join()` (wait to finish).** You cannot join before creating.

### 🔑 CLARIFIED — `std::unique_ptr` (RAII smart pointer)
```cpp
auto buf = std::make_unique<int[]>(100);   // owns a heap int[100]
buf[0] = 42;                               // use like a raw array
// freed automatically when buf goes out of scope — no delete needed
```
- The RAII replacement for raw `new`/`delete`. Destructor frees automatically.
- **Single owner**: a `unique_ptr` cannot be copied (two owners → double free) — only **moved** (`std::move`).
- Reach for this (or `std::vector`) instead of raw `new` — it's the answer the Code Review section wants.

### Test-taking habit
- On "select all that apply," check **each** option independently: "is this statement, on its own, true of this code?" Under-selecting (knowing the idea but not ticking the box) was the main miss.

---

## Session 3 additions — mixed concepts quiz

### Classes and structs basics (flagged for addition)
```cpp
struct Point { int x; int y; };    // members public by default
class Point { int x; int y; };    // members private by default
```
- Only real difference between `struct` and `class` in C++ is default access.
- **Constructor** = runs when object is created (acquires resources).
- **Destructor** = runs when object goes out of scope (releases resources). Prefixed with `~`.
- If you own a raw pointer in a class: you need a destructor AND copy ctor AND copy assignment (Rule of 3) — or use `std::vector`/`unique_ptr` and need none (Rule of 0).

### 🔑 CLARIFIED — a class can have multiple bugs simultaneously
```cpp
class Sensor {
    int* readings;
public:
    Sensor(int n) : readings(new int[n]) {}
    void reset() { readings = new int[count]; }  // leak: old array not deleted
    // missing: ~Sensor, copy ctor, copy assignment
};
```
Three separate bugs: (1) no destructor → leak on death, (2) reset() leaks old array, (3) Rule of 3 violation. All three are independently true — tick all on "select all that apply."

### 🔑 CLARIFIED — passing address vs data race are separate concerns
```cpp
void process(int* total, int n) { *total += i; }
std::thread t1(process, &sum, 100);
```
- `&sum` correctly passes the address of `sum` → that statement is TRUE and tick-able.
- The data race is about two threads writing through that address simultaneously without a lock.
- Cause (correct address passing) and effect (data race) are both simultaneously true.

### 🔑 CLARIFIED — read the full execution path before ticking a box
```cpp
void update(int idx, int val) {
    m.lock();
    if (idx >= size) return;   // guard fires BEFORE the write
    data[idx] = val;           // only reached when idx is valid
    m.unlock();
}
```
- "Out-of-bounds write if idx >= size" is FALSE — the return prevents it.
- The actual bugs: early return skips unlock (b), Rule of 3 (c), use lock_guard (e).
- Always trace execution before assuming a write/read actually happens.

### Wrong delete = UB (not just a leak)
```cpp
char* data = new char[n];
delete data;    // 💥 UB — should be delete[]
```
- Wrong delete → undefined behavior in the destructor.
- Cause (wrong delete) and effect (UB) are both separately tick-able.

---

## Session 3 additions — malloc/free, class lifecycle, shared_ptr

### `malloc`/`free` vs `new`/`delete`
| | `malloc`/`free` (C) | `new`/`delete` (C++) |
|-|--------------------|--------------------|
| Calls constructor/destructor? | ❌ No | ✅ Yes |
| Type-aware? | ❌ Returns `void*`, must cast | ✅ Returns typed pointer |
| On failure | Returns `nullptr` | Throws `std::bad_alloc` |
| Use in C++? | Avoid | Prefer |

Pairing rules (cross any = UB):
- `malloc` → `free`
- `new` → `delete`
- `new[]` → `delete[]`

**Red flag:** `malloc` paired with `delete`/`delete[]` or `new`/`new[]` paired with `free`.

### Class lifecycle
```cpp
class Sensor {
    int* data;
public:
    Sensor(int n) : data(new int[n]) {}  // constructor: runs on creation, acquires
    ~Sensor() { delete[] data; }         // destructor: runs on scope exit, releases
};
// Sensor s(10) → constructor → use → scope ends → destructor (automatic)
```
- No destructor written = compiler generates one that does nothing = raw pointer leaks.
- Constructor + destructor = RAII. Destructor fires on ANY exit (return, exception, scope end).

### 🔑 CLARIFIED — `shared_ptr` always cleans up when last owner dies
```cpp
void process() {
    auto p = std::make_shared<int>(42);  // count = 1
    auto q = p;                          // count = 2
    p.reset();                           // count = 1 — q still holds it
    std::cout << *q;                     // safe, prints 42
}                                        // q dies → count = 0 → freed automatically
```
- `shared_ptr` leaks ONLY in a reference cycle (count stuck > 0). A plain local going out of scope always frees.
- `p.reset()` drops p's ownership — does NOT free if another owner exists.
- Memory is freed exactly when the last `shared_ptr` to it is destroyed.

### Quiz performance milestone (session 3)
Went 4/5 with clean sweeps on malloc/destructor mismatch, correct-code decoys, lock_guard, Rule of 3, thread safety, and `delete` vs `delete[]`. Only miss: `shared_ptr` cleanup mechanics (closed above). Under-selecting and execution-tracing habits now solid.

---

## Session 4 additions — move semantics, general trivia, the deletion rule

### Move semantics — recognition level
```cpp
vector<int> a = {1,2,3};
vector<int> b = a;              // COPY — duplicates data, both persist independently
vector<int> c = std::move(a);   // MOVE — steals a's internal pointer; a left empty (NOT destroyed, just empty)
```
- `std::move(a)` doesn't move anything itself — it casts `a` to an rvalue, telling the compiler "safe to steal from."
- After a move, the source variable still exists (not a dangling pointer) — just emptied. Using it isn't UB, just logically wrong if you expect old data.
- lvalue = has a name, persists (`a`, `x`). rvalue = temporary, about to vanish (`5`, `a+b`, a function's return value).
- Red flag: `ClassName&&` in a constructor/assignment signature = move version. Single `&` = copy version.

### 🔑 CLARIFIED — declaring a move op auto-deletes copy ops (compiler-protected, not a Rule-of-3 bug)
```cpp
class Matrix {
    int* data;
public:
    Matrix(int n) : data(new int[n]) {}
    Matrix(Matrix&& other) noexcept : data(other.data) { other.data = nullptr; }  // move ctor
    ~Matrix() { delete[] data; }
    // NO copy ctor/assignment written —
    // compiler DELETES them (doesn't silently generate a dangerous shallow copy)
};
Matrix a(10);
Matrix b = a;   // compile error — copy ctor is deleted, caught at compile time
```
- **This is the opposite danger from the classic Rule of 3 bug.** Classic case: destructor only, no copy ops → compiler SILENTLY GENERATES a broken shallow copy → runtime double-free. This case: move op declared → compiler DELETES copy ops → compile-time error, no runtime bug at all.
- Don't flag "missing copy ctor/assignment" as a bug when a move constructor or move assignment is present — it's protected.
- Member not mentioned in an initializer list (e.g. a `std::mutex` member omitted from a move ctor's init list) is simply default-constructed in the new object — doesn't require the member itself to be movable.

### General-section trivia (Section 1 material)
**Integer overflow:**
```cpp
int x = INT_MAX; x = x + 1;   // signed overflow = undefined behavior
                                // commonly wraps to INT_MIN on real systems, NOT guaranteed by standard
```
**Integer division — destination type doesn't matter, only operand types:**
```cpp
int a=10, b=3;
double result = a / b;          // 3 — int/int truncates BEFORE assignment, double doesn't fix it
double result2 = (double)a / b; // 3.333... — cast an operand BEFORE the division
```
**Iterator invalidation:**
```cpp
for (auto it = v.begin(); it != v.end(); ++it)
    if (*it == 3) v.erase(it);   // erase invalidates it; ++it on invalidated iterator = UB
// fix: it = v.erase(it);  (don't also ++it that iteration)
```
**STL complexity:**
| Container | Access | Insert/erase end | Insert/erase middle | Search |
|-|-|-|-|-|
| vector | O(1) | O(1) amortized | O(n) | O(n) |
| unordered_map/set | O(1) avg | O(1) avg | O(1) avg | O(1) avg |
| map/set | O(log n) | O(log n) | O(log n) | O(log n) |
- `unordered_map` = hash-based, NOT sorted, O(1) avg. `map` = tree-based, sorted, O(log n). Use unordered_map unless you need sorted order.

### 🔑 CLARIFIED — array parameters always decay to pointers (no "pass by value" for raw arrays)
```cpp
void modify(int arr[], int size) { arr[0] = 100; }   // int arr[] IS int* arr
int main() {
    int data[5] = {1,2,3,4,5};
    modify(data, 5);
    cout << data[0];   // 100 — original IS modified
}
```
- `int arr[]` as a parameter type is identical to `int* arr` — always decays to a pointer.
- There is no "pass by value" for a raw C-style array parameter; the function can always modify the caller's data.
- Repeated misconception (hit twice — first with `fill()`, again here) — drill this until automatic.

### 🔑 CLARIFIED — non-copyable member auto-deletes the containing class's copy ops too
```cpp
class SafeCounter {
    int count = 0;
    std::mutex m;     // mutex has deleted copy ctor/assignment
public:
    void increment() { m.lock(); count++; }   // also missing m.unlock() entirely — separate bug
};
SafeCounter a;
SafeCounter b = a;   // compile error — m can't be copied, so SafeCounter can't either
```
- Rule: if a class contains ANY member with deleted copy operations (mutex, unique_ptr, etc.), the compiler automatically deletes the containing class's copy ctor/assignment too.
- Before flagging "Rule of 3 violation," check: does this class own something (mutex, unique_ptr) that already blocks copying at compile time? If so, no Rule of 3 violation — compiler-protected, not a runtime bug.
- Extends the session-4 move-constructor-deletes-copy-ops lesson to a second trigger: non-copyable members, not just declared move operations.

### atomic vs mutex — when to use which
```cpp
std::atomic<int> counter(0);
void worker() { for (int i=0;i<1000;i++) counter++; }   // always correct, prints exactly 2000
```
- `atomic<T>` — single simple variable, one operation at a time (increment, flag, simple read/write). Lighter weight than a mutex, often a single CPU instruction.
- `mutex` — protecting a multi-step critical section or multiple variables that must change together.
- Don't "upgrade" a correct atomic solution to a mutex — that's unnecessary overhead, not a fix.

### 🔑 CLARIFIED — `arr[i]` already IS the dereference, never stack `*` on top
```cpp
arr[i]        // means EXACTLY the same as:
*(arr + i)    // 100% interchangeable — [] already contains a dereference

*arr[i]       // WRONG — parses as *(arr[i]): dereferences the INT at arr[i], compile error
```
- Never write `*arr[i]` to "make sure" a value is set through a pointer — `arr[i] = 0` already is the dereferencing write.
- Rule: `p[n]` and `*(p + n)` are always the same operation. Pick one, never stack both.

### 🔑 CLARIFIED (2nd pass) — non-copyable member auto-deletes copy ops, side-by-side
```cpp
class SafeCounter { std::mutex m; };                  // mutex: copy ctor deleted by std lib
class Resource { std::unique_ptr<int> data; };         // unique_ptr: copy ctor deleted by std lib
// both containing classes get their OWN copy ctor auto-deleted too — compile error on copy attempt,
// NOT a Rule of 3 violation, NOT a runtime double-free.
```
- Diagnostic question before flagging "Rule of 3 violation": does this class own a `mutex`, `unique_ptr`,
  or anything else non-copyable? If yes → compiler already blocks copying, no danger. Only flag Rule of 3
  when the class owns something copyable-but-dangerous (a raw pointer).
- This is the SAME underlying mechanism as the move-constructor-deletes-copy-ops lesson — two different
  triggers (declaring a move op, OR holding a non-copyable member), same protective outcome.

### 🔑 CLARIFIED — `Stack s2 = s1;` invokes the copy CONSTRUCTOR, not copy assignment
```cpp
Stack s2 = s1;   // s2 doesn't exist yet → COPY CONSTRUCTOR fires (auto-generated, shallow)
s2 = s1;         // s2 already exists → COPY ASSIGNMENT fires instead
```
- Rule: initialization-with-copy (`Type x = y;` or `Type x(y);`) → copy constructor. Assignment to an existing object (`x = y;`) → copy assignment. Different functions, different timing.

### 🔑 CLARIFIED — unique_ptr by-value function parameters need std::move at the CALL SITE
```cpp
void addPtr(std::unique_ptr<int> p) { ptrs.push_back(std::move(p)); }  // fine inside
auto p1 = std::make_unique<int>(42);
addPtr(p1);              // ❌ compile error — tries to COPY p1 into the parameter
addPtr(std::move(p1));   // ✅ correct — explicitly moves p1 into the parameter
```
- Any time a `unique_ptr` (or other move-only type) crosses a function boundary by value, the CALL SITE needs `std::move()`, not just the function body.
- Classic Code Review pattern: spotting a missing `std::move` at a call site passing a move-only type by value.

---

## The min-cost-per-state pattern — reference solution

This is the "Capable Models" family: n models/workers, each with a cost and a 2-bit capability
(`"10"` = A only, `"01"` = B only, `"11"` = both). Find min cost so total A-coverage ≥ k AND
total B-coverage ≥ k. Built across sessions 3-4 with heavy scaffolding — index math (`prefix[x-1]`
= sum of cheapest x elements) was the sticking point. Use this as the canonical template.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int minCost(int n, vector<int> costs, vector<string> caps, int k) {
    vector<int> onlyA, onlyB, both;

    // Step 1: bucket by capability
    for (int i = 0; i < n; i++) {
        if (caps[i] == "10") onlyA.push_back(costs[i]);
        if (caps[i] == "01") onlyB.push_back(costs[i]);
        if (caps[i] == "11") both.push_back(costs[i]);
    }

    // Step 2: cheapest first
    sort(onlyA.begin(), onlyA.end());
    sort(onlyB.begin(), onlyB.end());
    sort(both.begin(),  both.end());

    // Step 3: prefix sums — prefix[i] = cost of cheapest (i+1) elements
    vector<int> prefixA, prefixB, prefixBoth;
    if (!onlyA.empty()) { prefixA.push_back(onlyA[0]);
        for (int i = 1; i < (int)onlyA.size(); i++) prefixA.push_back(prefixA[i-1] + onlyA[i]); }
    if (!onlyB.empty()) { prefixB.push_back(onlyB[0]);
        for (int i = 1; i < (int)onlyB.size(); i++) prefixB.push_back(prefixB[i-1] + onlyB[i]); }
    if (!both.empty())  { prefixBoth.push_back(both[0]);
        for (int i = 1; i < (int)both.size(); i++) prefixBoth.push_back(prefixBoth[i-1] + both[i]); }

    // Step 4: try every split — x = how many "both" models to buy (0..k)
    int ans = INT_MAX;
    for (int x = 0; x <= k; x++) {
        int need = k - x;   // still-needed onlyA and onlyB count

        if (x    > (int)both.size())  continue;  // not enough "both" models
        if (need > (int)onlyA.size()) continue;  // not enough onlyA models
        if (need > (int)onlyB.size()) continue;  // not enough onlyB models

        int costBoth = (x    == 0) ? 0 : prefixBoth[x - 1];
        int costA    = (need == 0) ? 0 : prefixA[need - 1];
        int costB    = (need == 0) ? 0 : prefixB[need - 1];

        ans = min(ans, costBoth + costA + costB);
    }

    return (ans == INT_MAX) ? -1 : ans;
}
```

**Common bugs hit while building this (watch for these in future variants):**
- Indexing into an empty vector (`vec[0]` on a vector with no elements yet) — always `push_back`, never index-assign on a vector that hasn't grown there yet.
- Copy-paste sort/loop bugs — sorting or summing the wrong bucket because the code was duplicated and one reference wasn't updated.
- Looping bound copy-paste — using `onlyA.size()` as the bound for a loop that should use `both.size()`.
- Special-casing `x==0` or `x==1` with separate `if` blocks instead of one general formula that naturally handles every value of x, including 0.
- `prefix[count - 1]` is the index for "cheapest `count` elements" — easy to get the off-by-one backwards.

---

---

## Session 5 additions — assessment day mock (2026-07-01)

### 🔑 CLARIFIED — virtual destructor: base class MUST declare it virtual

```cpp
class Base {
public:
    ~Base() {}   // NOT virtual
};
class Derived : public Base {
    int* data;
public:
    Derived() : data(new int[10]) {}
    ~Derived() { delete[] data; }
};

Base* obj = new Derived();
delete obj;   // 💥 only Base::~Base() runs — Derived::~Derived() NEVER called → leak
```

- When destructor is non-virtual, `delete` binds to the **static type** (`Base*`), not the runtime type.
- `Derived`'s destructor is silently skipped. Anything `Derived` owns leaks. UB.
- **Rule: any class meant to be inherited from needs `virtual ~ClassName() {}`.**
- Red flag: base class with a non-virtual destructor + derived class owning resources.

### 🔑 CLARIFIED — returning a local `unique_ptr` by value is SAFE

```cpp
std::unique_ptr<int> getVal() {
    auto p = std::make_unique<int>(99);
    return p;   // ✅ SAFE — compiler moves (or elides entirely via NRVO)
}
auto a = getVal();   // a owns the int — no double-free, no dangling
```

- Looks like a copy but isn't. Returning a local `unique_ptr` triggers the move constructor.
- **Don't flag this as a bug.** The bug is copying a `unique_ptr` after you have it, not returning one.
- Classic distractor: `return p;` from a function looks suspicious but is idiomatic C++.

### 🔑 CLARIFIED — use-after-move: source is null, dereference is UB

```cpp
auto p = std::make_unique<int>(0);
use(std::move(p));       // ✅ move at call site is correct
std::cout << *p << "\n"; // 💥 p is now null — dereferencing is UB
```

- `std::move(p)` at the call site is valid. No compile error there.
- The bug is using `p` AFTER the move — it's null. UB on dereference.
- Two separately tick-able answers: "call site move is correct (a)" AND "dereference after move is UB (b)."

### 🔑 CLARIFIED — static local variables: returning a reference is SAFE

```cpp
std::string& getName() {
    std::string name = "Tracker";
    return name;   // 💥 dangling — local dies on return
}

std::string& getTag() {
    static std::string tag = "Sensor";
    return tag;    // ✅ safe — static local lives for program duration
}
```

- `static` locals are initialized once and live until the program exits — not destroyed on function return.
- Returning a reference/pointer to a `static` local is a legitimate pattern (unusual but defined).
- Only flag as dangling if the local is NON-static.

### Deadlock from lock ordering (two mutexes, opposite order)

```cpp
// Thread 1: lock A then B
// Thread 2: lock B then A
// → each holds one, waits for the other → deadlock
```

- **Fix: always acquire multiple mutexes in the SAME ORDER across all threads.**
- Or use `std::lock(mtxA, mtxB)` which acquires both atomically (deadlock-safe).
- Deadlock ≠ data race. Deadlock = threads stuck waiting. Data race = unsynchronized memory access.

### Pass large objects by `const&`, not by value

```cpp
void process(std::string data) { ... }        // 💥 copies the entire string each call
void process(const std::string& data) { ... } // ✅ read-only ref, zero copy
```

- Red flag: function takes a large object (string, vector, struct) by value when it only reads it.
- Fix: `const T&` for read-only. `T&` for read-write. `T` by value only when you need your own copy.

### 🔑 CLARIFIED — `weak_ptr`: non-owning observer, breaks cycles

```cpp
struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;   // ✅ non-owning — doesn't bump ref count
};
```

- `weak_ptr` holds a reference to a `shared_ptr`-managed object **without owning it**.
- Doesn't increment the ref count → resource can be freed even if `weak_ptr` still exists.
- To USE the resource: call `.lock()` → returns a `shared_ptr` (null if already freed).
- **Primary purpose:** break `shared_ptr` cycles. Cycle = two shared_ptrs pointing at each other → ref counts never reach 0 → leak. Replace one with `weak_ptr` → cycle broken.
- `weak_ptr` ≠ `shared_ptr` (multiple owners) ≠ `unique_ptr` (transfer ownership). It's a **watcher only**.

### 🔑 CLARIFIED — unsigned overflow is DEFINED; signed overflow is UB

```cpp
int x = INT_MAX; x++;          // 💥 signed overflow = UB (may wrap, may not)
int y = INT_MIN; y = -y;       // 💥 -INT_MIN overflows signed → UB
unsigned int u = 0; u--;       // ✅ unsigned wraps to UINT_MAX — DEFINED behavior
```

- C++ standard guarantees unsigned arithmetic wraps mod 2^N.
- Signed overflow has no guaranteed behavior — the compiler can assume it never happens.
- Trick question pattern: "which of these is UB?" → signed operations that overflow; NOT unsigned.

### `condition_variable` — always use the predicate form

```cpp
// WRONG — vulnerable to spurious wakeups:
cv.wait(lock);

// CORRECT:
cv.wait(lock, []{ return ready; });   // re-checks ready on every wakeup
```

- OS can wake a `cv.wait()` even when nothing called `notify` (spurious wakeup).
- Without predicate: worker proceeds even if condition is still false.
- With predicate lambda: wakeup → recheck → sleep again if still false. Safe.
- `condition_variable` REQUIRES `unique_lock`, not `lock_guard`.

### Merge intervals — sort + scan against result.back()

```cpp
vector<vector<int>> merge(vector<vector<int>>& intervals) {
    sort(intervals.begin(), intervals.end());   // sort by start time

    vector<vector<int>> result;
    result.push_back(intervals[0]);

    for (int i = 1; i < (int)intervals.size(); i++) {
        if (intervals[i][0] <= result.back()[1]) {
            // overlaps — extend end of last merged interval
            result.back()[1] = max(result.back()[1], intervals[i][1]);
        } else {
            // no overlap — new interval
            result.push_back(intervals[i]);
        }
    }
    return result;
}
```

- Key: compare each interval against `result.back()` (last merged), NOT the previous input interval.
- `max()` on the end handles containment: `[1,10]` absorbing `[2,6]` correctly keeps end=10.
- Pattern: sort → initialize with first → scan → extend or append.

### Test-taking habit — false positives cost more than under-selecting

Mock 2 pattern: selecting options you weren't sure about inflated wrong answers across Section 2.
**Rule for the real test:** if you cannot say in one sentence exactly WHY an answer is wrong, do NOT select it. An unselected correct answer loses fewer points than a wrongly selected distractor in most scoring rubrics.

---

## To do / review list (flagged by user)
- [ ] Memory allocation implementations in depth: `new`/`delete`, `malloc`/`free`, when to use which
- [ ] Class/struct basics: access modifiers, constructor/destructor lifecycle diagram
- [ ] Section 3 coding: min-cost-per-state / bitmask (Capable Models) + C++ variants

## To cover next (open items)
- Smart pointers, remaining: `shared_ptr` (ref-counted, multiple owners), `weak_ptr`, ownership transfer with `std::move`
- Move semantics / lvalue vs rvalue (recognition-level)
- `const` correctness
- General-section C++ trivia: integer overflow, `/` integer division, iterator invalidation, STL container complexity
- Section 3 coding: min-cost-per-state / bitmask (Capable Models) + variants
