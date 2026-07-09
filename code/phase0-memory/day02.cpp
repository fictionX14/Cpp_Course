#include <vector>
#include <memory>
#include <iostream>
using namespace std;

// pointer practice to ensure destructors are ran once resources are no longer in scope 

// case #1 error prone due to potential early exit before call to delete[] data;
void process_leaky(bool errorOccurred) {
    int* data = new int[100];
    if (errorOccurred) return; 

    delete[] data;
}

// case #2 using unique pointer which has built in constructor and destructor functions when resource goes out of scope
void process_fixed(bool errorOccurred) { 
    unique_ptr<int[]> data  = make_unique<int[]>(100);

    if (errorOccurred) return;

}

// case #3 using a class to prove RAII fires automatically in cases where there is intent 

class ScopeLogger {
public: 
    ScopeLogger(string name) : name_(name){
        cout << name_ << " acquired\n";
    }
    ~ScopeLogger() {
        cout << name_ << " released\n";
    }
private: 
    string name_;
};

void demo(bool errorOccurred) {

    ScopeLogger log("demo resource status: ");
    if (errorOccurred) return;
    cout << "doing work...\n";
}

int main() { 

    // cout << "returned" << endl;
    // process_leaky(true);
    // cout << "returned" << endl;
    // process_fixed(true);
    // cout << "returned" << endl;

    demo(false);

    return 0;
}
// HOW TO CHECK FOR MEMORY LEAKS AND WHERE THEY ARE STEMMING FROM - USE: leaks --atExit -- ./day02

// HERE WE WILL NOTICE THAT function: process_leaky exits prematurely and leaks memory 

// int is 4 bytes, not 4 bits — 4 bits would only hold values 0–15! (1 byte = 8 bits, so 4 bytes = 32 bits, which matches int's usual range.) This mix-up matters — bit vs. byte confusion is a classic trap in memory-sizing and bit-shift questions, worth locking in now.

// math should be: 100 ints × 4 bytes = 400 bytes. The report says 416. Where'd the extra 16 come from?

// The allocator's bookkeeping overhead. When you ask malloc/new for memory, you never get exactly what you requested — the heap allocator has to:

// Round up to its internal bucket/alignment size (macOS's allocator rounds to 16-byte boundaries), and
// Store metadata alongside your data — a small header recording the block's size, so that when you (or the destructor) eventually calls free/delete, the allocator knows how much memory to reclaim.
// So your 400 usable bytes sit inside a 416-byte block — 400 for you, 16 for the allocator's own bookkeeping. This is a real, general truth: 
// you never get exactly what you ask the heap for. It's part of why heap allocation has overhead compared to the stack, and why many tiny allocations are more wasteful than one larger one (a preview of a Phase 1 performance idea).

// (base) chandlerpickett@Chandlers-MacBook-Pro phase0-memory % leaks --atExit -- ./day02
// day02(11610) MallocStackLogging: could not tag MSL-related memory as no_footprint, so those pages will be included in process footprint - (null)
// day02(11610) MallocStackLogging: recording malloc (and VM allocation) stacks using lite mode
// returned
// returned
// returned
// Process:         day02 [11610]
// Path:            /Users/USER/Documents/*/day02
// Load Address:    0x1044ae000
// Identifier:      day02
// Version:         0
// Code Type:       X86-64
// Platform:        macOS
// Parent Process:  leaks [11609]
// Target Type:     live task

// Date/Time:       2026-07-08 22:53:36.188 -0500
// Launch Time:     2026-07-08 22:53:35.131 -0500
// OS Version:      macOS 15.6.1 (24G90)
// Report Version:  7
// Analysis Tool:   /usr/bin/leaks

// Physical footprint:         872K
// Physical footprint (peak):  872K
// Idle exit:                  untracked
// ----

// leaks Report Version: 4.0, multi-line stacks
// Process 11610: 185 nodes malloced for 16 KB
// Process 11610: 1 leak for 416 total leaked bytes.

// STACK OF 1 INSTANCE OF 'ROOT LEAK: <malloc in process_leaky(bool)>':
// 5   dyld                               0x7ff8174ef530 start + 3056
// 4   day02                                 0x1044ae67b main + 59
// 3   day02                                 0x1044ae51a process_leaky(bool) + 26
// 2   libc++abi.dylib                    0x7ff81784e37b operator new(unsigned long) + 52
// 1   libsystem_malloc.dylib             0x7ff8176b9b32 _malloc + 87
// 0   libsystem_malloc.dylib             0x7ff8176d09d9 _malloc_zone_malloc_instrumented_or_legacy + 115 
// ====
//     1 (416 bytes) ROOT LEAK: <malloc in process_leaky(bool) 0x7f949b8040f0> [416]
