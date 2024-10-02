#pragma once

#include <atomic>
#include <barrier>
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/InertialUnit.hpp>
#include <webots/Gyro.hpp>

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
    void inertialGetRollPitchYaw(webots::InertialUnit &inertial, float *angle) {
        const double *angle_tmp = inertial.getRollPitchYaw();
        for (int i = 0; i < 3; i++) {
            angle[i] = angle_tmp[i];
        }
    }
    void gyroGetValues(webots::Gyro &gyro, float *speed) {
        const double *speed_tmp = gyro.getValues();
        for (int i = 0; i < 3; i++) {
            speed[i] = speed_tmp[i];
        }
    }

private:
    int basic_time_step;

};
}
}

