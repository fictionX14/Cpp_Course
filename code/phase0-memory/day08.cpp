#include <vector>
#include <memory>
#include <iostream>
using namespace std;

struct Track; 

    struct Sensor {

        weak_ptr<Track> owner;
        ~Sensor() { cout << "Sensor Destroyed\n";}
    };

struct Track { 

    shared_ptr<Sensor> sensor;

    ~Track() { cout << "Track Destroyed\n";}

};

// int main() { 

//     // create track struct tracker using shared_ptr
//     shared_ptr<Track> tracker= make_shared<Track>();
//     // create sensor struct sensor using shared_ptr
//     shared_ptr<Sensor> sensor = make_shared<Sensor>(); 

//     // link tracker and sensor 
//     tracker->sensor = sensor;
//     // make sensor the owner such that if tracker destructs, sensor maintains owner ship
//     sensor->owner = tracker; 

//     if(auto locked = sensor->owner.lock()) {
//         cout << "Sensor's owner is still alive\n";
//     } else { 
//         cout << "Sensors owner is gone\n";
//     }
// }

// OUTPUT: 
// Sensor's owner is still alive
// Track Destroyed
// Sensor Destroyed

#include <iostream>

int main() {
    int* p = new int(42);
    delete p;
    // p still holds the old address — nothing cleared it
    std::cout << *p << "\n";
}

// Compiled with AddressSanitizer, shows heap-use-after-free
// g++ -std=c++17 -fsanitize=address -g day08.cpp -o day08
// ./day08
