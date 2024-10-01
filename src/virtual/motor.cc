#include <iomanip>
#include <iostream>

#include "virtual/inc/motor.hpp"

namespace robo {
namespace vir {
Motor::Motor() {
    *angle = 0.0f;
    *speed = 0.0f;
}

Motor::Motor(const std::string name): name(name) {
    *angle = 0.0f;
    *speed = 0.0f;
}

void Motor::bind(const Binder &binder) {
    delete angle;
    delete speed;
    setTorque = binder.setTorque;
    angle = binder.angle;
    speed = binder.speed;
}

inline float Motor::getAngle() const {
    return *angle;
}
inline float Motor::getSpeed() const {
    return *speed;
}

void Motor::setTorqueDefault(float torque) {
    std::cout << std::setprecision(3) << "[Motor<" + name + ">] set torque: " << torque << "Nm." << std::endl;
}
}
}
