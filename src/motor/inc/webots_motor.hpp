#pragma once

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

#include "io/webots/inc/webots.hpp"
#include "motor/inc/motor.hpp"

namespace robo {
namespace motor {
class WebotsMotor {
public:
    explicit WebotsMotor(robo::io::Webots &webots_io, webots::Motor&motor): 
        webots_io(webots_io),
        motor(motor),
        encoder(*motor.getPositionSensor()) {
        encoder.enable(webots_io.robot.getBasicTimeStep());
    }
    ~WebotsMotor() = default;

    Binder binder {
        .setTorque = [this](float torque) { this->setTorque(torque); },
        .angle= &this->angle,
        .speed = &this->speed,
    };

    void Update(const int time_step) {
        int time;
        webots_io.EncoderGetValue(encoder, angle, time);
        speed = (angle - angle_last) / (time - time_last);
        time_last = time;
        angle_last = angle;
    }

    void setTorque(float torque) {
        webots_io.MotorSetTorque(motor, torque);
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

