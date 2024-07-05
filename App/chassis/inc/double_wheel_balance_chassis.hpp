#pragma once

#include "chassis_base.hpp"
#include "imu.hpp"
#include "double_wheel_balance_chassis_config.hpp"

using namespace Config::Chassis::DoubleWheelChassis;

namespace Chassis {
template<>
class Chassis<Config::Chassis::DoubleWheelBalanceChassisConfig>: public ChassisBase {
public:
    explicit Chassis(const Config::Chassis::DoubleWheelBalanceChassisConfig&);
    ~Chassis() override = default;

    void init() override;

    Dev::Imu imu;

private:
    void update_state() override;
    void ctrl_val_calc() override;
    void ctrl() override;

    template<CTRL_METHOD_t T>
    void ctrl_val_calc_();

    /* Z[0] : displacement
     * Z[1] : vx (speed)
     * Z[2] : tilt_angle
     * Z[3] : tilt_speed
     * Z[4] : yaw_angle
     * Z[5] : yaw_speed
     * U[0] : left motor control_val
     * U[1] : right motor control_val
    */
    Lqr::Lqr<6, 2> lqr;
    std::vector<Pid::Pid> pid_motors;
    Pid::Pid pid_tilt;
    Pid::Pid pid_vx;
    Pid::Pid pid_yaw;
    
    float displacement = 0.0f;
    float& yaw;
    float yaw_set;
    float& tilt_angle;
    float& tilt_speed;
};
} // namespace Chassis
