#include <vector>
#include <memory>
#include <iostream>
using namespace std;

// Only 2 Sensor objects were ever constructed — because track_ref never creates one.

// Trace the constructor calls, not the variable names:

// owner = createSensor() → 1st Sensor constructed (owned exclusively by owner, via unique_ptr).
// a = std::make_shared<Sensor>() → 2nd Sensor constructed (owned by a, via shared_ptr).
// track_ref = a; → no constructor call at all. This is a shared_ptr copy, not a new object. Copying a shared_ptr doesn't build a new Sensor — it just makes track_ref point at the same Sensor a already owns, and bumps the reference count.

struct Sensor{
    Sensor() {cout << "Sensor acquired" << endl;}

    ~Sensor() {cout << "Sensor released" << endl;}
};

unique_ptr<Sensor> createSensor() { 
    return make_unique<Sensor>(); 
}


int main() { 

    unique_ptr<Sensor> owner = createSensor();
    cout << "unique_ptr owns it, no sharing possible\n";

    shared_ptr<Sensor> track_ref;

    {
        shared_ptr<Sensor> a = make_shared<Sensor>(); 
        cout << "count: " << a.use_count() << endl;
        track_ref = a;
        cout << "count: " << a.use_count() << endl;

    }
    
    cout << "count after inner scope ends: " << track_ref.use_count() << endl; 

    return 0;
}