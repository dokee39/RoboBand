#pragma once

#include "pwm_dc_motor.hpp"
#include "pwm_dc_motor_impl.hpp"
#include "matrix.h"
#include "tim.h"
#include "lqr_controller.hpp"
#include "pid_controller.hpp"

namespace Config::Chassis {
namespace DoubleWheelChassis {

enum CTRL_METHOD_t {
    CTRL_LQR = 0,
    CTRL_PID = 1,
};

const CTRL_METHOD_t CTRL_METHOD(CTRL_PID);
const float WHEEL_DIAMETER(0.03f);
const float TILT_ANGLE_SET(0.04f);
const float TILT_ANGLE_LAZY(0.40f);
const float MOTOR_CTRL_VAL_MAX(1.0f);

const typename Motor::PwmDcMotorImplConfig MOTOR_IMPL_LEFT {
    .OUT_DIR = 1,
    .IN_DIR = 1,

    .CCR_TYPE = Motor::PwmDcMotorImplConfig::TIM_INT16,
    .CCR_VAL_MAX = 2500,
    .htim_pwm = htim3,
    .TIM_PWM_CHANNEL_A = TIM_CHANNEL_1,
    .TIM_PWM_CHANNEL_B = TIM_CHANNEL_2,

    .ECD_TYPE = Motor::PwmDcMotorImplConfig::TIM_INT16,
    .htim_ecd = htim3,
};

const typename Motor::PwmDcMotorImplConfig MOTOR_IMPL_RIGHT {
    .OUT_DIR = -1,
    .IN_DIR = -1,

    .CCR_TYPE = Motor::PwmDcMotorImplConfig::TIM_INT32,
    .CCR_VAL_MAX = 2500,
    .htim_pwm = htim3,
    .TIM_PWM_CHANNEL_A = TIM_CHANNEL_3,
    .TIM_PWM_CHANNEL_B = TIM_CHANNEL_4,

    .ECD_TYPE = Motor::PwmDcMotorImplConfig::TIM_INT32,
    .htim_ecd = htim3,
};

const typename Motor::PwmDcMotorConfig MOTOR { 
    .FBK_PERIOD = 1,
    .PULSE_TO_RAD_RATIO = 1.0f / 30.0f / 500.0f * (2.0f * 3.1415926f) / 2.0f // 1 / reduction ratio / pulse per revolution * 2 pi / 2 (timer) 
};

const typename Lqr::LqrConfig<6, 2> LQR {
    .K = Matrixf<2, 6>((float[12]) { 0.00f, 6.9958f, -208.3373f, -20.2779f, -0.13f, -0.05f, 
                                     0.00f, 6.9958f, -208.3373f, -20.2279f, 0.13f, 0.05f }),
    .Umax = Matrixf<1, 2>((float[2]) { MOTOR_CTRL_VAL_MAX, MOTOR_CTRL_VAL_MAX }),
};

const typename Pid::PidConfig PID_MOTOR_LEFT {
    .kp = 0.4f,
    .ki = 0.02f,
    .kd = 0.0f, 
    .max_iout = 0.8f,
    .max_out = MOTOR_CTRL_VAL_MAX,
};

const typename Pid::PidConfig PID_MOTOR_RIGHT {
    .kp = 0.4f,
    .ki = 0.02f,
    .kd = 0.0f, 
    .max_iout = 0.8f,
    .max_out = MOTOR_CTRL_VAL_MAX,
};

const typename Pid::PidConfig PID_TILT_ANGLE {
    .kp = 120.0f,
    .ki = 3.5f,
    .kd = 0.0f,
    .max_iout = 10.0f,
    .max_out = 50.0f,
};

const typename Pid::PidConfig PID_VX {
    .kp = 0.050f,
    .ki = 0.0010f,
    .kd = 0.0f, 
    .max_iout = 0.08f,
    .max_out = 0.15f,
};

const typename Pid::PidConfig PID_YAW {
    .kp = 0.0f,
    .ki = 0.0f,
    .kd = 0.0f, 
    .max_iout = 0.0f,
    .max_out = 0.0f,
};

} // namespace DoubleWheelBalance    

struct DoubleWheelBalanceChassisConfig {
    const Motor::PwmDcMotorImplConfig MOTOR_IMPL_LEFT;
    const Motor::PwmDcMotorImplConfig MOTOR_IMPL_RIGHT;
    const Motor::PwmDcMotorConfig MOTOR;
    const Lqr::LqrConfig<6, 2> LQR;
    const Pid::PidConfig PID_MOTOR_LEFT;
    const Pid::PidConfig PID_MOTOR_RIGHT;
    const Pid::PidConfig PID_TILT_ANGLE;
    const Pid::PidConfig PID_VX;
    const Pid::PidConfig PID_YAW;
};

const typename Config::Chassis::DoubleWheelBalanceChassisConfig DoubleWheelBalance {
    .MOTOR_IMPL_LEFT = DoubleWheelChassis::MOTOR_IMPL_LEFT,
    .MOTOR_IMPL_RIGHT = DoubleWheelChassis::MOTOR_IMPL_RIGHT,
    .MOTOR = DoubleWheelChassis::MOTOR,
    .LQR = DoubleWheelChassis::LQR,
    .PID_MOTOR_LEFT = DoubleWheelChassis::PID_MOTOR_LEFT,
    .PID_MOTOR_RIGHT = DoubleWheelChassis::PID_MOTOR_RIGHT,
    .PID_TILT_ANGLE = DoubleWheelChassis::PID_TILT_ANGLE,
    .PID_VX = DoubleWheelChassis::PID_VX,
    .PID_YAW = DoubleWheelChassis::PID_YAW,
};

} // namespace Config::Chassis
