#include "Pneumatics.hpp"

Pneumatics::Pneumatics(const char port) : pneumatic(port) {}

void Pneumatics::extend() {
    pneumatic.set_value(true); 
}

void Pneumatics::retract() {
    pneumatic.set_value(false); 
}

void Pneumatics::toggle() {
    pneumatic.set_value(!pneumatic.get_value());
}
