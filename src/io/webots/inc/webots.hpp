#pragma once

#include <mutex>
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

namespace robo {
namespace io {
class Webots {
public:
    explicit Webots(webots::Robot &robot): 
        robot(robot) {
    };
    ~Webots() = default;

    void MotorSetTorque(webots::Motor &motor, float torque) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "setTorque" << std::endl;
        motor.setTorque(torque);
    }
    void EncoderGetValue(webots::PositionSensor &encoder, float &value, int &time) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "getValue" << std::endl;
        value = encoder.getValue();
        time = robot.step(0);
    };

private:
    webots::Robot &robot;
    std::mutex mtx;

};
}
}

