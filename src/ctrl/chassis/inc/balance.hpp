#pragma once

#include <eigen3/Eigen/Eigen>

#include "virtual/motor.hpp"
#include "virtual/imu.hpp"
#include "runner/inc/runner.hpp"

namespace robo {
namespace ctrl {
class Balance {
public:
    explicit Balance();
    ~Balance() = default;

    robo::run::Runner runner;
    robo::vir::Imu imu;
    robo::vir::Motor joint_motor[2];
    robo::vir::Motor wheel_motor[2];

    void ctrlLoop();

private:
    Eigen::Vector<float, 8> state_ref;
    Eigen::Vector<float, 8> state_set;
    Eigen::Vector<float, 4> ctrl_vec;
    Eigen::Matrix<float, 8, 4> K;
};
}
}
