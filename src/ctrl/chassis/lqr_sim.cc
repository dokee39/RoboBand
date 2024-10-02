#include "ctrl/chassis/inc/lqr_sim.hpp"

namespace robo {
namespace ctrl {
LqrSim::LqrSim():
    runner("LQR simulation", [this]() { ctrlLoop(); }, cycle_ms) {
    K << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
}

void LqrSim::ctrlLoop() {
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
    ctrl_vec = K * (state_set - state_ref); 

    wheel_motor[0].setTorque(ctrl_vec(0));
    wheel_motor[1].setTorque(ctrl_vec(1));
    joint_motor[0].setTorque(ctrl_vec(2));
    joint_motor[1].setTorque(ctrl_vec(3));
}
}
}
