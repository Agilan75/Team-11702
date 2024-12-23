#include "pros/rotation.hpp"
#include "pros/rotation.h"
#include <iostream>
#include <vector>
#include <cerrno>

namespace pros {
inline namespace v5 {

// initializes the rotation sensor on the given port
Rotation::Rotation(const std::int8_t port) : Device(port) {
    if (pros::c::rotation_set_reversed(port, false) == PROS_ERR) {
        std::cerr << "Error initializing rotation sensor on port: " 
                  << static_cast<int>(port) << " (errno: " << errno << ")\n";
    }
}

// resets the rotation sensor's current position to the absolute angle
std::int32_t Rotation::reset() {
    return pros::c::rotation_reset(_port);
}

// sets the sensor's refresh interval in milliseconds
std::int32_t Rotation::set_data_rate(std::uint32_t rate) const {
    return pros::c::rotation_set_data_rate(_port, rate);
}

// sets the rotation sensor's position to the specified value
std::int32_t Rotation::set_position(std::uint32_t position) const {
    return pros::c::rotation_set_position(_port, position);
}

// resets the rotation sensor's position to zero
std::int32_t Rotation::reset_position() const {
    return pros::c::rotation_reset_position(_port);
}

// returns a list of all connected rotation sensors
std::vector<Rotation> Rotation::get_all_devices() {
    std::vector<Rotation> devices;
    for (std::int8_t port = 1; port <= 21; ++port) {
        if (pros::c::rotation_get_position(port) != PROS_ERR) {
            devices.emplace_back(port);
        }
    }
    return devices;
}

// retrieves the current position of the sensor in ticks
std::int32_t Rotation::get_position() const {
    return pros::c::rotation_get_position(_port);
}

// retrieves the current velocity of the sensor in centidegrees/second
std::int32_t Rotation::get_velocity() const {
    return pros::c::rotation_get_velocity(_port);
}

// retrieves the current angle of the sensor in centidegrees
std::int32_t Rotation::get_angle() const {
    return pros::c::rotation_get_angle(_port);
}

// sets the sensor's direction as reversed or normal
std::int32_t Rotation::set_reversed(bool value) const {
    return pros::c::rotation_set_reversed(_port, value);
}

// reverses the direction of the rotation sensor
std::int32_t Rotation::reverse() const {
    return set_reversed(true);
}

// retrieves the reversed state of the rotation sensor
std::int32_t Rotation::get_reversed() const {
    return pros::c::rotation_get_reversed(_port);
}

// outputs rotation sensor details in a readable format
std::ostream& operator<<(std::ostream& os, const Rotation& rotation) {
    os << "Rotation [port: " << static_cast<int>(rotation.get_port())
       << ", position: " << rotation.get_position()
       << ", velocity: " << rotation.get_velocity()
       << ", angle: " << rotation.get_angle()
       << ", reversed: " << (rotation.get_reversed() ? "true" : "false")
       << "]";
    return os;
}

// allows creation of a rotation sensor using a port number
const Rotation literals::operator"" _rot(const unsigned long long int r) {
    return Rotation(static_cast<std::int8_t>(r));
}

}  // namespace v5
}  // namespace pros
