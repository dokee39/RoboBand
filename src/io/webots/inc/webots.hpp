#pragma once

#include <mutex>
#include <atomic>
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
    ~Webots() = default;

    webots::Robot robot;
    std::atomic<int> time_step;
    std::atomic<int> bind_num {0};
    std::atomic<int> task_wait_cnt {0};

    int Step() {
        std::lock_guard<std::mutex> lock(mtx);
        while (task_wait_cnt != 0);
        task_wait_cnt = bind_num.load();
        time_step = robot.step(basic_time_step);
        return time_step;
    }

    void MotorSetTorque(webots::Motor &motor, const float torque) {
        std::lock_guard<std::mutex> lock(mtx);
        motor.setTorque(torque);
    }

    void EncoderGetValue(webots::PositionSensor &encoder, float &value, int &time) {
        std::lock_guard<std::mutex> lock(mtx);
        value = encoder.getValue();
        time = robot.step(0);
    };

private:
    int basic_time_step;
    std::mutex mtx;

};
}
}

