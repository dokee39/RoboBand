#ifdef USE_WEBOTS

#include "io/webots.hpp"

namespace robo {
namespace io {
Webots::Webots():
    time((int)(robot.getTime() * 1000)),
    basic_time_step((int)robot.getBasicTimeStep()){
}
Webots::~Webots() {
    if (sync_point != nullptr) {
        delete sync_point;
    }
}

int Webots::step() {
    if (sync_point == nullptr) {
        sync_point = new std::barrier<>(bind_runner_num + 1);
    }
    for (auto update: updaters) {
        update();
    }
    ;
    time = robot.step(basic_time_step) == -1 ? -1 : (int)(robot.getTime() * 1000);
    if (time != -1) {
        sync_point->arrive_and_wait();
    }
    return time;
}

void Webots::motorSetTorque(webots::Motor &motor, const float torque) {
    if (time != -1) {
        motor.setTorque(torque);
    }
}

void Webots::encoderGetValue(webots::PositionSensor &encoder, float &value, int &time_) {
    if (time != -1) {
        value = encoder.getValue();
        time_ = time;
    }
};

void Webots::inertialGetRollPitchYaw(webots::InertialUnit &inertial, float *angle) {
    if (time != -1) {
        const double *angle_tmp = inertial.getRollPitchYaw();
        for (int i = 0; i < 3; i++) {
            angle[i] = angle_tmp[i];
        }
    }
}

void Webots::gyroGetValues(webots::Gyro &gyro, float *speed) {
    if (time != -1) {
        const double *speed_tmp = gyro.getValues();
        for (int i = 0; i < 3; i++) {
            speed[i] = speed_tmp[i];
        }
    }
}
}
}

#endif

