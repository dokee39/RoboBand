#pragma once

#include "tim.h"
#include "motor_base.hpp"
#include <cstdint>

namespace Motor {
struct PwmDcMotorImplConfig {
    enum TIM_TYPE_e {
        TIM_INT16 = 0,
        TIM_INT32 = 1,
    };
    static const uint32_t TIM_ECD_CHANNEL_A = TIM_CHANNEL_1;
    static const uint32_t TIM_ECD_CHANNEL_B = TIM_CHANNEL_2;

    const int OUT_DIR;
    const int IN_DIR;

    const TIM_TYPE_e CCR_TYPE;
    const uint32_t CCR_VAL_MAX;
    TIM_HandleTypeDef& htim_pwm;
    const uint32_t TIM_PWM_CHANNEL_A;
    const uint32_t TIM_PWM_CHANNEL_B;

    const TIM_TYPE_e ECD_TYPE;
    TIM_HandleTypeDef& htim_ecd;
};

template<>
class MotorImpl<PwmDcMotorImplConfig>: public PwmDcMotorImplConfig {
public:
    explicit MotorImpl(const PwmDcMotorImplConfig& config_impl);
    ~MotorImpl() = default;

    bool msg_out(const float& pwm_duty);
    bool msg_in(int32_t& ecd_delta);

private:
    MotorImpl() = delete; // must init

};
} // namespace Motor
