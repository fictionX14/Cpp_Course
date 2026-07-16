#include <vector>
#include <memory>
#include <iostream>
using namespace std;

// dangling pointer issue, x lives on stack and is vulnerable to stack pop when resources are needed
int* getValueBroken() {

    int x = 42;
    return &x;
}

// day06.cpp:10:13: warning: address of stack memory associated with local variable 'x' returned [-Wreturn-stack-address]
//    10 |     return &x;
//       |             ^

// A little better since x is passed by value and assigned the caller, x can be overwritten with no issues
int getValueFixed1() {

    int x = 42;
    return x;
}

// safest since this allocates memory on the heap for the caller and returns the unique pointer with allocated memory 
unique_ptr<int> getValueFixed2() {

    return make_unique<int>(42);
}

int main() { 

    int fixed1 = getValueFixed1();
    cout << fixed1 << endl;

    unique_ptr<int> fixed2 = getValueFixed2(); 
    cout << *fixed2 << endl;

    return 0;
}