#pragma once 

#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

namespace robo {
namespace motor {
struct Binder {
    std::function<void (float)> setTorque;
    float *angle;
    float *speed;
};

class Motor {
public:
    explicit Motor() {
        *angle = 0.0f;
        *speed = 0.0f;
    }
    explicit Motor(const std::string name): name(name) {
        *angle = 0.0f;
        *speed = 0.0f;
    }
    ~Motor() = default;

    std::function<void (float)> setTorque = [this](float torque) { this->setTorqueDefault(torque); };

    void bind(const Binder &binder) {
        delete angle;
        delete speed;
        setTorque = binder.setTorque;
        angle = binder.angle;
        speed = binder.speed;
    }

    float getAngle() const {
        return *angle;
    }
    float getSpeed() const {
        return *speed;
    }

private:
    const std::string name {"M"};
    float *angle {new float};
    float *speed {new float};

    void setTorqueDefault(float torque) {
        std::cout << std::setprecision(3) << "[Motor<" + name + ">] set torque: " << torque << "Nm." << std::endl;
    }
};
}
}

