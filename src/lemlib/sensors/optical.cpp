#include "pros/optical.hpp"
#include "pros/optical.h"
#include <iostream>
#include <vector>
#include <cerrno>

namespace pros {
inline namespace v5 {

// initializes the Optical Sensor on the given port
Optical::Optical(const std::uint8_t port) : Device(port) {
    if (pros::c::optical_get_hue(port) == PROS_ERR_F) {
        std::cerr << "Error initializing Optical Sensor on port: "
                  << static_cast<int>(port) << " (errno: " << errno << ")\n";
    }
}

// greturns a list of Optical Sensor objects
std::vector<Optical> Optical::get_all_devices() {
    std::vector<Optical> devices;
    for (std::uint8_t port = 1; port <= 21; ++port) {
        if (pros::c::optical_get_hue(port) != PROS_ERR_F) {
            devices.emplace_back(port);
        }
    }
    return devices;
}

// get the detected color hue
double Optical::get_hue() {
    return pros::c::optical_get_hue(_port);
}

// get the detected color saturation
double Optical::get_saturation() {
    return pros::c::optical_get_saturation(_port);
}

// get the detected color brightness
double Optical::get_brightness() {
    return pros::c::optical_get_brightness(_port);
}

// get the detected proximity value
std::int32_t Optical::get_proximity() {
    return pros::c::optical_get_proximity(_port);
}

// set the PWM value of the white LED on the sensor
std::int32_t Optical::set_led_pwm(uint8_t value) {
    return pros::c::optical_set_led_pwm(_port, value);
}

// get the PWM value of the white LED on the sensor
std::int32_t Optical::get_led_pwm() {
    return pros::c::optical_get_led_pwm(_port);
}

// get the processed RGBC data from the sensor
pros::c::optical_rgb_s_t Optical::get_rgb() {
    return pros::c::optical_get_rgb(_port);
}

// get the raw unprocessed RGBC data from the sensor
pros::c::optical_raw_s_t Optical::get_raw() {
    return pros::c::optical_get_raw(_port);
}

// get the most recent gesture data from the sensor
pros::c::optical_direction_e_t Optical::get_gesture() {
    return pros::c::optical_get_gesture(_port);
}

// get the most recent raw gesture data from the sensor
pros::c::optical_gesture_s_t Optical::get_gesture_raw() {
    return pros::c::optical_get_gesture_raw(_port);
}

// enable gesture detection on the sensor
std::int32_t Optical::enable_gesture() {
    return pros::c::optical_enable_gesture(_port);
}

// disable gesture detection on the sensor
std::int32_t Optical::disable_gesture() {
    return pros::c::optical_disable_gesture(_port);
}

// get the integration time (update rate) of the optical sensor
double Optical::get_integration_time() {
    return pros::c::optical_get_integration_time(_port);
}

// set the integration time (update rate) of the optical sensor
std::int32_t Optical::set_integration_time(double time) {
    return pros::c::optical_set_integration_time(_port, time);
}

// outputs Optical Sensor details
std::ostream& operator<<(std::ostream& os, pros::Optical& optical) {
    auto rgb = optical.get_rgb();
    os << "Optical Sensor [port: " << static_cast<int>(optical.get_port())
       << ", hue: " << optical.get_hue()
       << ", saturation: " << optical.get_saturation()
       << ", brightness: " << optical.get_brightness()
       << ", proximity: " << optical.get_proximity()
       << ", rgb: {red: " << rgb.red
       << ", green: " << rgb.green
       << ", blue: " << rgb.blue << "}]";
    return os;
}

// allows creation of an Optical Sensor using a port number
const Optical literals::operator"" _opt(const unsigned long long int o) {
    return Optical(static_cast<std::uint8_t>(o));
}

}  // namespace v5
}  // namespace pros
