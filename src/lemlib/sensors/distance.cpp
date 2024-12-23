#include "pros/distance.hpp"
#include "pros/distance.h"
#include <iostream>
#include <vector>
#include <cerrno>

namespace pros {
inline namespace v5 {

// initalize distance sesnor on the given port
Distance::Distance(const std::uint8_t port) : Device(port) {
    if (pros::c::distance_get(port) == PROS_ERR) {
        std::cerr << "Error initializing Distance Sensor on port: "
                  << static_cast<int>(port) << " (errno: " << errno << ")\n";
    }
}

// gets the measured distance in millimeters
std::int32_t Distance::get() {
    return pros::c::distance_get(_port);
}

// gets the measured distance in millimeters
std::int32_t Distance::get_distance() {
    return get();  // Alias for `get`
}

// returns a list of Distance Sensor objects
std::vector<Distance> Distance::get_all_devices() {
    std::vector<Distance> devices;
    for (std::uint8_t port = 1; port <= 21; ++port) {
        if (pros::c::distance_get(port) != PROS_ERR) {
            devices.emplace_back(port);
        }
    }
    return devices;
}

// retrieves the confidence level of the distance reading
std::int32_t Distance::get_confidence() {
    return pros::c::distance_get_confidence(_port);
}

// estimates the relative size of the detected object
std::int32_t Distance::get_object_size() {
    return pros::c::distance_get_object_size(_port);
}

// retrieves the object's velocity in meters per second
double Distance::get_object_velocity() {
    return pros::c::distance_get_object_velocity(_port);
}

// outputs Distance Sensor details
std::ostream& operator<<(std::ostream& os, pros::Distance& distance) {
    os << "Distance Sensor [port: " << static_cast<int>(distance.get_port())
       << ", distance: " << distance.get()
       << " mm, confidence: " << distance.get_confidence()
       << ", object size: " << distance.get_object_size()
       << ", object velocity: " << distance.get_object_velocity()
       << "]";
    return os;
}

// creates a Distance Sensor using a port number
const Distance literals::operator"" _dist(const unsigned long long int d) {
    return Distance(static_cast<std::uint8_t>(d));
}

}  // namespace v5
}  // namespace pros
