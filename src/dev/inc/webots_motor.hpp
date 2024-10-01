#pragma once

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

#include "io/webots/inc/webots.hpp"
#include "virtual/inc/motor.hpp"

namespace robo {
namespace dev {
class WebotsMotor {
public:
    explicit WebotsMotor(robo::io::Webots &webots_io, webots::Motor&motor);
    ~WebotsMotor() = default;

    robo::vir::Binder binder {
        .setTorque = [this](float torque) { this->setTorque(torque); },
        .angle= &this->angle,
        .speed = &this->speed,
    };

    void update(const int time_step);
    void setTorque(float torque) {
        webots_io.motorSetTorque(motor, torque);
    }
    void setAngelOffset(float angle_offset_) {
        angle_offset = angle_offset_;
    }
    float getAngle() const {
        return angle - angle_offset;
    }
    float getSpeed() const {
        return speed;
    }

private:
    robo::io::Webots &webots_io;
    webots::Motor &motor;
    webots::PositionSensor &encoder;

    float speed {0.0f};
    float angle {0.0f};
    float angle_last {0.0f};
    float angle_offset {0.0f};
    int time_last {0};

};
}
}

