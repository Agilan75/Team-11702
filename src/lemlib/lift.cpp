#include "Lift.hpp"

Lift::Lift(const std::uint8_t port) : motor(port) {
    motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD); 
}

void Lift::move_up() {
    motor.move_voltage(12000); 
}

void Lift::move_down() {
    motor.move_voltage(-12000); 
}    

void Lift::stop() {
    motor.move_voltage(0); 
}
