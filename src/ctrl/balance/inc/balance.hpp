#pragma once

#include <eigen3/Eigen/Eigen>
#include "motor/inc/motor.hpp"

namespace robo {
namespace ctrl {
class Balance {
public:
    explicit Balance();
    ~Balance() = default;

    robo::motor::Motor joint_motor[2];
    robo::motor::Motor wheel_motor[2];

private:
    Eigen::Vector<float, 8> state_ref;
    Eigen::Vector<float, 8> state_set;
    Eigen::Vector<float, 4> ctrl_vec;
    Eigen::Matrix<float, 8, 4> K;
};
}
}
