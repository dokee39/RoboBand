#pragma once

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

#include "motor.hpp"

namespace robo {
namespace motor {
class WebotsMotor {
public:
    explicit WebotsMotor(webots::Motor *const motor, const int time_step): 
        motor(motor),
        encoder(motor->getPositionSensor()) {
        encoder->enable(time_step);
    }
    ~WebotsMotor();

    Binder binder {
        .setTorque = [this](float torque) { this->setTorque(torque); },
        .angle= &this->angle,
        .speed = &this->speed,
    };

    void Update(const int time_step) {
        angle_last = angle;
        angle = encoder->getValue();
        speed = (angle - angle_last) / time_step;
    }

    void setTorque(float torque) {
        motor->setTorque(torque);
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
    webots::Motor *const motor;
    webots::PositionSensor *const encoder;

    float speed {0.0f};
    float angle {0.0f};
    float angle_last {0.0f};
    float angle_offset {0.0f};
};
}
}

