#include "ctrl/balance_sys/balance_sys.hpp"
#include "util/util.hpp"

using namespace robo::util;
namespace robo {
namespace ctrl {
BalanceSys::BalanceSys(const toml::table &config, const std::string &name):
    Ctrl(name, getValue<int>(config, "cycle_ms")),
    cycle_ms(getValue<int>(config, "cycle_ms")),
    K(Eigen::Map<Eigen::Matrix<float, 4, 10>>(getArray<float, 4 * 10>(config, "K").data())),
    radius_wheel(getValue<float>(config, "radius_wheel")) {
}

void BalanceSys::ctrlLoop() {
    state_set << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
    state_ref << state_ref(0) + (wheel_motor[0].getAngle() + wheel_motor[1].getAngle()) * radius_wheel / 2,
                 (wheel_motor[0].getSpeed() + wheel_motor[1].getSpeed()) / 2,
                 imu.getAngle(robo::vir::Imu::Yaw),
                 imu.getSpeed(robo::vir::Imu::Yaw),
                 joint_motor[0].getAngle(),
                 joint_motor[0].getSpeed(),
                 joint_motor[1].getAngle(),
                 joint_motor[1].getSpeed(),
                 imu.getAngle(robo::vir::Imu::Pitch),
                 imu.getSpeed(robo::vir::Imu::Pitch);
    // TODO rad_format phi
    ctrl_vec = K * (state_set - state_ref); 

    wheel_motor[0].setTorque(ctrl_vec(0));
    wheel_motor[1].setTorque(ctrl_vec(1));
    joint_motor[0].setTorque(ctrl_vec(2));
    joint_motor[1].setTorque(ctrl_vec(3));

    // DEBUG
    /*struct Data {*/
    /*    int32_t id;*/
    /*    float t;*/
    /*    float d[9];*/
    /*} __attribute__((packed));*/
    /*Data d = { 1, 0, { state_ref(0), state_ref(2), state_ref(4), state_ref(6), state_ref(8), ctrl_vec(0), ctrl_vec(1), ctrl_vec(2), ctrl_vec(3) } };*/
}
}
}
