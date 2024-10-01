#pragma once

#include <atomic>
#include <barrier>
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

namespace robo {
namespace io {
class Webots {
public:
    explicit Webots();
    ~Webots();

    webots::Robot robot;
    std::atomic<int> time_step;
    std::atomic<int> bind_tasks_num {0};
    std::barrier<> *sync_point;

    int step();
    void motorSetTorque(webots::Motor &motor, const float torque) {
        motor.setTorque(torque);
    }
    void encoderGetValue(webots::PositionSensor &encoder, float &value, int &time) {
        value = encoder.getValue();
        time = robot.step(0);
    };

private:
    int basic_time_step;

};
}
}

