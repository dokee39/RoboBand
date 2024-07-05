#pragma once

#include <cstdint>

#include "motor_base.hpp"
#include "user_lib_cpp.hpp"

namespace Motor {

struct PwmDcMotorConfig {
    const uint32_t FBK_PERIOD; // in ms
    const float PULSE_TO_RAD_RATIO;
};

// derived class : DC motor (controlled by PWM & feedback via pulse encoder)
template<typename T_impl>
class Motor<PwmDcMotorConfig, T_impl>: public PwmDcMotorConfig, public MotorBase {
public:
    explicit Motor(const PwmDcMotorConfig& config, T_impl& impl):
        PwmDcMotorConfig(config),
        impl(impl) {}
    ~Motor() override = default;

    bool ctrl() override;
    bool feedback() override;

private:
    T_impl& impl;
    // ctrl_val is pwm_duty, 0 ~ 1
    int32_t ecd_delta = 0;

};

template<typename T_impl>
bool Motor<PwmDcMotorConfig, T_impl>::ctrl() {
    bool ret(true);

    UserLib::abs_limit<float>(ctrl_val, 1.0f);

    ret = impl.msg_out(ctrl_val);

    return ret;
}

template<typename T_impl>
bool Motor<PwmDcMotorConfig, T_impl>::feedback() {
    bool ret(true);
    float speed(0.0f);
    float angle(0.0f);

    impl.msg_in(ecd_delta);

    angle = get_angle() + ecd_delta * PULSE_TO_RAD_RATIO;
    angle = UserLib::rad_format(angle);
    speed = ecd_delta * PULSE_TO_RAD_RATIO / FBK_PERIOD * 1000.0f;

    update(speed, angle);

    return ret;
}
} // namespace Motor
