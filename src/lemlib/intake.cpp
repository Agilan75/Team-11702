#include "Intake.hpp"

Intake::Intake(const std::uint8_t port) : motor(port) {
    motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST); 
}

void Intake::spin_forward() {
    motor.move_voltage(12000); 
}

void Intake::spin_reverse() {
    motor.move_voltage(-12000); 
}
void Intake::stop() {
    motor.move_voltage(0); 
}

void Intake::set_speed(int speed) {
    motor.move_velocity(speed);
}