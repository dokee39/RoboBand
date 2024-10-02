#pragma once

#include <eigen3/Eigen/Eigen>

#include "virtual/motor.hpp"
#include "virtual/imu.hpp"
#include "runner/inc/runner.hpp"

namespace robo {
namespace ctrl {
class LqrSim {
public:
    explicit LqrSim();
    ~LqrSim() = default;

    int cycle_ms {1};
    robo::run::Runner runner;
    robo::vir::Imu imu;
    robo::vir::Motor wheel_motor[2];
    robo::vir::Motor joint_motor[2];

    void ctrlLoop();

private:
    Eigen::Vector<float, 10> state_ref;
    Eigen::Vector<float, 10> state_set;
    Eigen::Vector<float, 4> ctrl_vec;
    Eigen::Matrix<float, 4, 10> K;
    float radius_wheel {0.15f};
};
}
}
