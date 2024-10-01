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
    explicit Webots():
        time_step(robot.step(0)),
        basic_time_step((int)robot.getBasicTimeStep()){
    }
    ~Webots() {
        delete sync_point;
    }

    webots::Robot robot;
    std::atomic<int> time_step;
    std::atomic<int> bind_tasks_num {0};
    std::barrier<> *sync_point;

    int Step() {
        if (sync_point == nullptr) {
            sync_point = new std::barrier<>(bind_tasks_num + 1);
        }
        time_step = robot.step(basic_time_step);
        if (time_step == -1) {
            delete sync_point;
        } else {
            sync_point->arrive_and_wait();
        }
        return time_step;
    }

    void MotorSetTorque(webots::Motor &motor, const float torque) {
        motor.setTorque(torque);
    }

    void EncoderGetValue(webots::PositionSensor &encoder, float &value, int &time) {
        value = encoder.getValue();
        time = robot.step(0);
    };

private:
    int basic_time_step;

};
}
}

