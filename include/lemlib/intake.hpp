#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "pros/motors.hpp"

class Intake {
private:
    pros::Motor motor;

public:
    /**
     * Constructor for the Intake class.
     *
     * @param port The port where the intake motor is connected.
     */
    Intake(const std::uint8_t port);

    /**
     * Spins the intake motor forward.
     */
    void spin_forward();

    /**
     * Spins the intake motor in reverse.
     */
    void spin_reverse();

    /**
     * Stops the intake motor.
     */
    void stop();
    
    /**
     * Adjusts the intake speed.
     *
     * @param speed The speed percentage (-100 to 100).
     */
    void set_speed(int speed);
};

#endif // INTAKE_HPP
