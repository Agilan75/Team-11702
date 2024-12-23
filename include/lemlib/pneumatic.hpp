#ifndef PNEUMATICS_HPP
#define PNEUMATICS_HPP

#include "pros/adi.hpp"

class Pneumatics {
private:
    pros::ADIDigitalOut pneumatic;

public:
    /**
     * Constructor for the Pneumatics class.
     * 
     * @param port The port on the ADI where the pneumatic solenoid is connected.
     */
    Pneumatics(const char port);

    /**
     * Extends the pneumatic clamp.
     */
    void extend();

    /**
     * Retracts the pneumatic clamp.
     */
    void retract();

    /**
     * Toggles the state of the pneumatic clamp.
     */
    void toggle();
};

#endif // PNEUMATICS_HPP
