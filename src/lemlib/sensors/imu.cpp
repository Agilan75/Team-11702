#include "pros/imu.hpp"
#include "pros/imu.h"
#include <iostream>
#include <cerrno>

namespace pros {
inline namespace v5 {

// initalizes the Inertial Sensor on the given port
Imu::Imu(const std::int8_t port) : Device(port) {
    if (pros::c::imu_reset(port) == PROS_ERR) {
        std::cerr << "Error initializing Inertial Sensor on port: "
                  << static_cast<int>(port) << " (errno: " << errno << ")\n";
    }
}

// resets the Inertial Sensor
std::int32_t Imu::reset() const {
    return pros::c::imu_reset(_port);
}

// checks if the sensor is calibrating
bool Imu::is_calibrating() const {
    return pros::c::imu_is_calibrating(_port);
}

// retrieves the current heading of the sensor in degrees
double Imu::get_heading() const {
    return pros::c::imu_get_heading(_port);
}

// retrieves the total rotation of the sensor in degrees
double Imu::get_rotation() const {
    return pros::c::imu_get_rotation(_port);
}

// sets the heading of the sensor to a specific value
std::int32_t Imu::set_heading(const double heading) const {
    return pros::c::imu_set_heading(_port, heading);
}

// sets the rotation of the sensor to a specific value
std::int32_t Imu::set_rotation(const double rotation) const {
    return pros::c::imu_set_rotation(_port, rotation);
}

// retrieves acceleration values on x, y, z axes
imu_accel_s_t Imu::get_accel() const {
    return pros::c::imu_get_accel(_port);
}

// retrieves gyroscope values on x, y, z axes
imu_gyro_s_t Imu::get_gyro_rate() const {
    return pros::c::imu_get_gyro_rate(_port);
}

// retrieves the internal temperature of the sensor in Celsius
double Imu::get_temperature() const {
    return pros::c::imu_get_temperature(_port);
}

// outputs Inertial Sensor details
std::ostream& operator<<(std::ostream& os, const Imu& imu) {
    os << "Inertial Sensor [port: " << static_cast<int>(imu.get_port())
       << ", heading: " << imu.get_heading()
       << ", rotation: " << imu.get_rotation()
       << ", calibrating: " << (imu.is_calibrating() ? "true" : "false")
       << "]";
    return os;
}

// allows creation of an Inertial Sensor using a port number
const Imu literals::operator"" _imu(const unsigned long long int i) {
    return Imu(static_cast<std::int8_t>(i));
}

}  // namespace v5
}  // namespace pros
