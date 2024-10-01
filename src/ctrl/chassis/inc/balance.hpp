#pragma once

#include <eigen3/Eigen/Eigen>

#include "virtual/inc/motor.hpp"
#include "runner/inc/runner.hpp"

namespace robo {
namespace ctrl {
class Balance {
public:
    explicit Balance():
        runner("balance chassis", [this]() { ctrl_loop(); }) {
    }
    ~Balance() = default;

    robo::run::Runner runner;
    robo::vir::Motor joint_motor[2];
    robo::vir::Motor wheel_motor[2];

    void ctrl_loop();

private:
    Eigen::Vector<float, 8> state_ref;
    Eigen::Vector<float, 8> state_set;
    Eigen::Vector<float, 4> ctrl_vec;
    Eigen::Matrix<float, 8, 4> K;
};
}
}