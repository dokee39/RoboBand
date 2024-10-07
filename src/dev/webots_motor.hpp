#pragma once
#ifdef USE_WEBOTS

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

#include "virtual/motor.hpp"
#include "dev/dev.hpp"

namespace robo {
namespace dev {
class WebotsMotor: public Dev<robo::io::Webots> {
public:
    explicit WebotsMotor(robo::io::Webots &webots_io, const std::string &motor_name);
    ~WebotsMotor() = default;

    robo::vir::MotorBinder binder {
        .setTorque = [this](float torque) { this->setTorque(torque); },
        .angle= &this->angle,
        .speed = &this->speed,
    };

    void setTorque(float torque) {
        webots_io.motorSetTorque(motor, torque);
    }
    void setAngelOffset(float angle_offset_) {
        angle_offset = angle_offset_;
    }
    float getAngle() const {
        return angle;
    }
    float getSpeed() const {
        return speed;
    }

private:
    webots::Motor &motor;
    webots::PositionSensor &encoder;

    float speed {0.0f};
    float angle {0.0f};
    float angle_last {0.0f};
    float angle_offset {0.0f};
    int time_last {0};

    virtual void update() override;

};
}
}

#endif

