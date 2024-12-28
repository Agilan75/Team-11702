#include "pros/vision.hpp"
#include <iostream>
#include <vector>
#include <cerrno>

#include "pros/adi.hpp"
#include "pros/serial.hpp" 

namespace pros {
inline namespace v5 {

Vision::Vision(std::uint8_t port, vision_zero_e_t zero_point) 
    : Device(port, DeviceType::vision) {
    if (pros::c::vision_set_zero_point(port, zero_point) == PROS_ERR) {
        std::cerr << "Error initializing Vision Sensor on port " 
                  << static_cast<int>(port) << " (errno: " << errno << ")\n";
    }
}

std::int32_t Vision::clear_led(void) const {
    return pros::c::vision_clear_led(get_port());
}

vision_signature_s_t Vision::signature_from_utility(const std::int32_t id, const std::int32_t u_min,
                                                    const std::int32_t u_max, const std::int32_t u_mean,
                                                    const std::int32_t v_min, const std::int32_t v_max,
                                                    const std::int32_t v_mean, const float range,
                                                    const std::int32_t type) {
    vision_signature_s_t signature;
    signature.id = id;
    signature.u_min = u_min;
    signature.u_max = u_max;
    signature.u_mean = u_mean;
    signature.v_min = v_min;
    signature.v_max = v_max;
    signature.v_mean = v_mean;
    signature.range = range;
    signature.type = type;
    return signature;
}


vision_color_code_t Vision::create_color_code(const std::uint32_t sig_id1, const std::uint32_t sig_id2,
                                              const std::uint32_t sig_id3, const std::uint32_t sig_id4,
                                              const std::uint32_t sig_id5) const {
    return pros::c::vision_create_color_code(get_port(), sig_id1, sig_id2, sig_id3, sig_id4, sig_id5);
}

std::vector<Vision> Vision::get_all_devices() {
    std::vector<Vision> devices;
    for (uint8_t port = 1; port <= 21; ++port) {
        if (pros::c::vision_get_object_count(port) != PROS_ERR) {
            devices.emplace_back(port);
        }
    }
    return devices;
}

vision_object_s_t Vision::get_by_size(const std::uint32_t size_id) const {
    return pros::c::vision_get_by_size(get_port(), size_id);
}

vision_object_s_t Vision::get_by_sig(const std::uint32_t size_id, const std::uint32_t sig_id) const {
    return pros::c::vision_get_by_sig(get_port(), size_id, sig_id);
}

vision_object_s_t Vision::get_by_code(const std::uint32_t size_id, const vision_color_code_t color_code) const {
    return pros::c::vision_get_by_code(get_port(), size_id, color_code);
}

std::int32_t Vision::get_exposure(void) const {
    return pros::c::vision_get_exposure(get_port());
}

std::int32_t Vision::get_object_count(void) const {
    return pros::c::vision_get_object_count(get_port());
}

vision_signature_s_t Vision::get_signature(const std::uint8_t signature_id) const {
    vision_signature_s_t signature;
    pros::vision_signature_s_t result = pros::c::vision_get_signature(get_port(), signature_id);
    
    if (result.id == 0) {
        std::cerr << "Error retrieving signature for ID " << static_cast<int>(signature_id)
                  << " (errno: " << errno << ")\n";
        return {};
    }
    
    return result;
}


std::int32_t Vision::get_white_balance(void) const {
    return pros::c::vision_get_white_balance(get_port());
}

std::int32_t Vision::read_by_size(const std::uint32_t size_id, const std::uint32_t object_count,
                                  vision_object_s_t* const object_arr) const {
    return pros::c::vision_read_by_size(get_port(), size_id, object_count, object_arr);
}

std::int32_t Vision::read_by_sig(const std::uint32_t size_id, const std::uint32_t sig_id, 
                                 const std::uint32_t object_count, vision_object_s_t* const object_arr) const {
    return pros::c::vision_read_by_sig(get_port(), size_id, sig_id, object_count, object_arr);
}

int32_t Vision::read_by_code(const std::uint32_t size_id, const vision_color_code_t color_code,
                             const std::uint32_t object_count, vision_object_s_t* const object_arr) const {
    return pros::c::vision_read_by_code(get_port(), size_id, color_code, object_count, object_arr);
}

std::int32_t Vision::print_signature(const vision_signature_s_t sig) {
    return pros::c::vision_print_signature(sig);
}

std::int32_t Vision::set_auto_white_balance(const std::uint8_t enable) const {
    return pros::c::vision_set_auto_white_balance(get_port(), enable);
}

std::int32_t Vision::set_exposure(const std::uint8_t exposure) const {
    return pros::c::vision_set_exposure(get_port(), exposure);
}

std::int32_t Vision::set_led(const std::int32_t rgb) const {
    return pros::c::vision_set_led(get_port(), rgb);
}

std::int32_t Vision::set_signature(const std::uint8_t signature_id, vision_signature_s_t* const signature_ptr) const {
    return pros::c::vision_set_signature(get_port(), signature_id, signature_ptr);
}

std::int32_t Vision::set_white_balance(const std::int32_t rgb) const {
    return pros::c::vision_set_white_balance(get_port(), rgb);
}

std::int32_t Vision::set_zero_point(vision_zero_e_t zero_point) const {
    return pros::c::vision_set_zero_point(get_port(), zero_point);
}

std::int32_t Vision::set_wifi_mode(const std::uint8_t enable) const {
    return pros::c::vision_set_wifi_mode(get_port(), enable);
}

Vision Vision::get_vision() {
    for (uint8_t port = 1; port <= 21; ++port) {
        if (pros::c::vision_get_object_count(port) != PROS_ERR) {
            return Vision(port);
        }
    }
    std::cerr << "Error: No Vision Sensor found\n";
    return Vision(PROS_ERR);
}

}  // namespace v5

namespace literals {
const Vision operator"" _vis(const unsigned long long int m) {
    return Vision(static_cast<uint8_t>(m));
}
}  // namespace literals
}  // namespace pros
