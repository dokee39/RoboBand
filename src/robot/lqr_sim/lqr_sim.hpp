#ifdef USE_WEBOTS
#pragma once

#include "robot/robot.hpp"
#include "ctrl/balance_sys/balance_sys.hpp"
#include "dev/webots_imu.hpp"
#include "dev/webots_motor.hpp"

namespace robo {
class LqrSim: public Robot {
public:
    explicit LqrSim(const toml::table &config);
    ~LqrSim() override = default;

private:
    ctrl::BalanceSys balance_sys;

    robo::dev::WebotsImu imu;
    robo::dev::WebotsMotor wheel_motor[2];
    robo::dev::WebotsMotor joint_motor[2];
    
    void bindVirtualDev() override;
    void devInit() override;
};
}

#endif

