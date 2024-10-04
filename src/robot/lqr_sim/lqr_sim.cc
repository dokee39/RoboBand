#ifdef USE_WEBOTS

#include "robot/lqr_sim/lqr_sim.hpp"
#include "util/util.hpp"

using namespace robo::util;
namespace robo {
LqrSim::LqrSim(const toml::table &config):
    balance_sys(getTable(getTable(config, "ctrl"), "balance_sys")), 
    imu(webots_io, getValue<std::string>(config, "inertial"), getValue<std::string>(config, "gyro")),
    wheel_motor{robo::dev::WebotsMotor(webots_io, util::getValue<std::string>(config, "wheel_motor_1")),
                robo::dev::WebotsMotor(webots_io, util::getValue<std::string>(config, "wheel_motor_2"))},
    joint_motor{robo::dev::WebotsMotor(webots_io, util::getValue<std::string>(config, "joint_motor_1")),
                robo::dev::WebotsMotor(webots_io, util::getValue<std::string>(config, "joint_motor_2"))} {
    ctrls.push_back(&balance_sys);
    bindVirtualDev();
    devInit();
}

void LqrSim::bindVirtualDev() {
    balance_sys.wheel_motor[0].bind(wheel_motor[0].binder);
    balance_sys.wheel_motor[1].bind(wheel_motor[1].binder);
    balance_sys.joint_motor[0].bind(joint_motor[0].binder);
    balance_sys.joint_motor[1].bind(joint_motor[1].binder);
}

void LqrSim::devInit() {

}
}
#endif

