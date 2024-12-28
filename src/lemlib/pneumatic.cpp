#include "pneumatic.hpp"
#include <iostream>
#include <vector>
#include <cerrno>

#include "pros/adi.hpp"
#include "pros/serial.hpp" 

Pneumatics::Pneumatics(const char port) : pneumatic(port) {}

void Pneumatics::extend() {
    pneumatic.set_value(true); 
}

void Pneumatics::retract() {
    pneumatic.set_value(false); 
}
