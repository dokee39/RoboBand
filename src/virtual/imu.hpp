#pragma once

#include <iostream>
#include <string>

namespace robo {
namespace vir {
struct ImuBinder {
    float *angle;
    float *speed;
};

class Imu {
public:
    explicit Imu(const std::string &name): name(name) {}
    explicit Imu() = default;
    ~Imu() {
        if (!isBound()) {
            delete[] angle;
            delete[] speed;
        }
    }

    enum ImuId {
        Roll = 0,
        Pitch = 1,
        Yaw = 2
    };

    bool isBound() const {
        return is_bound;
    }
    void bind(const ImuBinder &binder) {
        if (!isBound()) {
            delete[] angle;
            delete[] speed;
        } else {
            std::clog << "[WARNING] [Imu<" + name + ">] Binding repeatedly!" << std::endl;
        }
        angle = binder.angle;
        speed = binder.speed;
        is_bound = true;
    }
    float getAngle(ImuId id) const {
        return angle[id];
    }
    float getSpeed(ImuId id) const {
        return speed[id];
    }

private:
    const std::string name {"IMU"};
    bool is_bound {false};
    float *angle {new float[3] {0.0f}};
    float *speed {new float[3] {0.0f}};

};
}
}
