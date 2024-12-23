#include "pros/vision.hpp"
#include "pros/vision.h"
#include <iostream>
#include <vector>
#include <cerrno>

namespace pros {
inline namespace v5 {

// initializes the Vision Sensor on the given port
Vision::Vision(const std::int8_t port) : Device(port) {
    if (pros::c::vision_clear_led(port) == PROS_ERR) {
        std::cerr << "Error initializing vision sensor on port: "
                  << static_cast<int>(port) << " (errno: " << errno << ")\n";
    }
}

// retrieves the largest detected object with the given signature
vision_object_s_t Vision::get_by_sig(const std::uint8_t signature_id) const {
    return pros::c::vision_get_by_sig(_port, 0, signature_id);
}

// retrieves the largest detected object (ignoring signature)
vision_object_s_t Vision::get_largest_object() const {
    return pros::c::vision_get_by_size(_port, 0);
}

// changes the Vision Sensor's LED color
std::int32_t Vision::set_led(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) const {
    return pros::c::vision_set_led(_port, r, g, b);
}

// resets the Vision Sensor's LED to its default state
std::int32_t Vision::clear_led() const {
    return pros::c::vision_clear_led(_port);
}

// defines a new signature for object detection
std::int32_t Vision::set_signature(const std::uint8_t sig_id, const vision_signature_s_t& signature) const {
    return pros::c::vision_set_signature(_port, sig_id, &signature);
}

// retrieves the signature configuration for the given ID
vision_signature_s_t Vision::get_signature(const std::uint8_t sig_id) const {
    vision_signature_s_t signature;
    if (pros::c::vision_get_signature(_port, sig_id, &signature) == PROS_ERR) {
        std::cerr << "Error retrieving signature for ID: " << static_cast<int>(sig_id) << " (errno: " << errno << ")\n";
    }
    return signature;
}

// returns all objects currently detected by the Vision Sensor
std::vector<vision_object_s_t> Vision::get_objects() const {
    std::vector<vision_object_s_t> objects;
    std::int32_t count = pros::c::vision_get_object_count(_port);
    for (std::int32_t i = 0; i < count; ++i) {
        objects.push_back(pros::c::vision_get_by_size(_port, i));
    }
    return objects;
}

// retrieves the number of currently detected objects
std::int32_t Vision::get_object_count() const {
    return pros::c::vision_get_object_count(_port);
}

// clears all detected objects from the Vision Sensor's memory
std::int32_t Vision::clear_objects() const {
    return pros::c::vision_clear_objects(_port);
}

// adjusts the Vision Sensor's brightness level
std::int32_t Vision::set_brightness(const std::uint8_t brightness) const {
    return pros::c::vision_set_brightness(_port, brightness);
}

// retrieves the current brightness level of the Vision Sensor
std::uint8_t Vision::get_brightness() const {
    return pros::c::vision_get_brightness(_port);
}

// adjusts the Vision Sensor's white balance setting
std::int32_t Vision::set_white_balance(const vision_white_balance_mode_e_t wb_mode) const {
    return pros::c::vision_set_white_balance(_port, wb_mode);
}

// retrieves the current white balance setting of the Vision Sensor
vision_white_balance_mode_e_t Vision::get_white_balance() const {
    return pros::c::vision_get_white_balance(_port);
}

//outputs Vision Sensor details
std::ostream& operator<<(std::ostream& os, const Vision& vision) {
    os << "Vision Sensor [port: " << static_cast<int>(vision.get_port())
       << ", object count: " << pros::c::vision_get_object_count(vision.get_port())
       << "]";
    return os;
}

// allows creation of a Vision Sensor using a port number
const Vision literals::operator"" _vis(const unsigned long long int v) {
    return Vision(static_cast<std::int8_t>(v));
}

}  // namespace v5
}  // namespace pros
