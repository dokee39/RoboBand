#pragma once

#include <toml++/toml.hpp>
#include <eigen3/Eigen/Eigen>

#include "ctrl/ctrl.hpp"
#include "virtual/motor.hpp"
#include "virtual/imu.hpp"

namespace robo {
namespace ctrl {
class BalanceSys: public Ctrl{
public:
    explicit BalanceSys(const toml::table &config, const std::string &name);
    ~BalanceSys() override = default;

    int cycle_ms;
    robo::vir::Imu imu;
    robo::vir::Motor wheel_motor[2];
    robo::vir::Motor joint_motor[2];

    void ctrlLoop() override;

private:
    Eigen::Vector<float, 10> state_ref;
    Eigen::Vector<float, 10> state_set;
    Eigen::Vector<float, 4> ctrl_vec;
    Eigen::Matrix<float, 4, 10> K;
    float radius_wheel;
};
}
}
