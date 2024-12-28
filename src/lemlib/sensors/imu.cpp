#include "pros/imu.hpp"
#include <iostream>
#include <vector>

#include "pros/adi.hpp"
#include "pros/serial.hpp" 

namespace pros {

Imu Imu::get_imu() {
    // Check ports 1 through 21 to find an IMU
    for (uint8_t port = 1; port <= 21; ++port) {
        if (pros::c::imu_get_status(port) != PROS_ERR) {
            return Imu(port);
        }
    }

    std::cerr << "Error: No IMU sensor found\n";
    return Imu(PROS_ERR);
}

std::int32_t Imu::reset(bool blocking) const {
    return pros::c::imu_reset(this->get_port());
}

std::int32_t Imu::set_data_rate(std::uint32_t rate) const {
    return pros::c::imu_set_data_rate(this->get_port(), rate);
}

std::vector<Imu> Imu::get_all_devices() {
    std::vector<Imu> devices;
    // Iterate through all possible ports
    for (uint8_t port = 1; port <= 21; ++port) {
        if (pros::c::imu_get_status(port) != PROS_ERR) {
            devices.emplace_back(port);
        }
    }
    return devices;
}

double Imu::get_rotation() const {
    return pros::c::imu_get_rotation(get_port());
}

double Imu::get_heading() const {
    return pros::c::imu_get_heading(get_port());
}

pros::quaternion_s_t Imu::get_quaternion() const {
    return pros::c::imu_get_quaternion(get_port());
}

pros::euler_s_t Imu::get_euler() const {
    return pros::c::imu_get_euler(get_port());
}

double Imu::get_pitch() const {
    return pros::c::imu_get_pitch(get_port());
}

double Imu::get_roll() const {
    return pros::c::imu_get_roll(get_port());
}

double Imu::get_yaw() const {
    return pros::c::imu_get_yaw(get_port());
}

pros::imu_gyro_s_t Imu::get_gyro_rate() const {
    return pros::c::imu_get_gyro_rate(get_port());
}

pros::imu_accel_s_t Imu::get_accel() const {
    return pros::c::imu_get_accel(get_port());
}

pros::ImuStatus Imu::get_status() const {
    auto status = pros::c::imu_get_status(get_port());
    return static_cast<pros::ImuStatus>(status);
}

imu_orientation_e_t Imu::get_physical_orientation() const {
    return pros::c::imu_get_physical_orientation(get_port());
}

std::int32_t Imu::tare_rotation() const {
    return pros::c::imu_tare_rotation(get_port());
}

std::int32_t Imu::tare_heading() const {
    return pros::c::imu_tare_heading(get_port());
}

std::int32_t Imu::tare_pitch() const {
    return pros::c::imu_tare_pitch(get_port());
}

std::int32_t Imu::tare_roll() const {
    return pros::c::imu_tare_roll(get_port());
}

std::int32_t Imu::tare_yaw() const {
    return pros::c::imu_tare_yaw(get_port());
}

std::int32_t Imu::tare_euler() const {
    return pros::c::imu_tare_euler(get_port());
}

std::int32_t Imu::tare() const {
    return pros::c::imu_tare(get_port());
}

std::int32_t Imu::set_heading(const double target) const {
    return pros::c::imu_set_heading(get_port(), target);
}

std::int32_t Imu::set_rotation(const double target) const {
    return pros::c::imu_set_rotation(get_port(), target);
}

std::int32_t Imu::set_pitch(const double target) const {
    return pros::c::imu_set_pitch(get_port(), target);
}

std::int32_t Imu::set_roll(const double target) const {
    return pros::c::imu_set_roll(get_port(), target);
}

std::int32_t Imu::set_yaw(const double target) const {
    return pros::c::imu_set_yaw(get_port(), target);
}

std::int32_t Imu::set_euler(const pros::euler_s_t target) const {
    return pros::c::imu_set_euler(get_port(), target);
}

std::ostream& operator<<(std::ostream& os, const pros::Imu& imu) {
    os << "Imu [port: " << static_cast<int>(imu.get_port())
       << ", rotation: " << imu.get_rotation()
       << ", heading: " << imu.get_heading()
       << ", pitch: " << imu.get_pitch()
       << ", roll: " << imu.get_roll()
       << ", yaw: " << imu.get_yaw()
       << ", gyro rate: {" << imu.get_gyro_rate().x << ", " << imu.get_gyro_rate().y << ", " << imu.get_gyro_rate().z << "}"
       << ", accel: {" << imu.get_accel().x << ", " << imu.get_accel().y << ", " << imu.get_accel().z << "}"
       << ", calibrating: " << imu.is_calibrating()
       << "]";
    return os;
}

namespace literals {
const pros::Imu operator"" _imu(const unsigned long long int i) {
    return pros::Imu(static_cast<uint8_t>(i));
}
}  // namespace literals

}  // namespace pros
