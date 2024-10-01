#pragma once 

#include <functional>
#include <string>

namespace robo {
namespace vir {
struct Binder {
    std::function<void (float)> setTorque;
    float *angle;
    float *speed;
};

class Motor {
public:
    explicit Motor();
    explicit Motor(const std::string name);
    ~Motor() = default;

    std::function<void (float)> setTorque = [this](float torque) { this->setTorqueDefault(torque); };

    void bind(const Binder &binder);
    float getAngle() const;
    float getSpeed() const;

private:
    const std::string name {"M"};
    float *angle {new float};
    float *speed {new float};

    void setTorqueDefault(float torque);
};
}
}

