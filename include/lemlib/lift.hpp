#ifndef LIFT_HPP
#define LIFT_HPP

#include "pros/motors.hpp"

class Lift {
private:
    pros::Motor motor; 

public:
    /**
     * Constructor for the Lift class.
     *
     * @param port The port where the lift motor is connected.
     */
    Lift(const std::uint8_t port);

    /**
     * Moves the lift up.
     */
    void move_up();

    /**
     * Moves the lift down.
     */
    void move_down();

    /**
     * Stops the lift motor.
     */
    void stop();
};

#endif // LIFT_HPP
