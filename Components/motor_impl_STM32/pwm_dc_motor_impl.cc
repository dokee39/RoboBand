#include <cstdint>

#include "pwm_dc_motor_impl.hpp"

namespace Motor {
MotorImpl<PwmDcMotorImplConfig>::MotorImpl(
    const PwmDcMotorImplConfig& config_impl
):
    PwmDcMotorImplConfig(config_impl) {
    // timer would not restart since there ia a judge inside HAL driver
    HAL_TIM_PWM_Start(&htim_pwm, TIM_PWM_CHANNEL_A);
    HAL_TIM_PWM_Start(&htim_pwm, TIM_PWM_CHANNEL_B);
    __HAL_TIM_SetCompare(&htim_pwm, TIM_PWM_CHANNEL_A, CCR_VAL_MAX);
    __HAL_TIM_SetCompare(&htim_pwm, TIM_PWM_CHANNEL_B, CCR_VAL_MAX);
    HAL_TIM_Encoder_Start(&htim_ecd, TIM_ECD_CHANNEL_A);
    HAL_TIM_Encoder_Start(&htim_ecd, TIM_ECD_CHANNEL_B);
}

bool MotorImpl<PwmDcMotorImplConfig>::msg_out(const float& pwm_duty) {
    bool ret(true);
    int32_t ccr(OUT_DIR * pwm_duty * CCR_VAL_MAX);

    if (ccr < 0) {
        __HAL_TIM_SetCompare(&htim_pwm, TIM_PWM_CHANNEL_B, CCR_VAL_MAX);
        __HAL_TIM_SetCompare(&htim_pwm, TIM_PWM_CHANNEL_A, CCR_VAL_MAX + ccr);
    } else {
        __HAL_TIM_SetCompare(&htim_pwm, TIM_PWM_CHANNEL_A, CCR_VAL_MAX);
        __HAL_TIM_SetCompare(&htim_pwm, TIM_PWM_CHANNEL_B, CCR_VAL_MAX - ccr);
    }

    return ret;
}

bool MotorImpl<PwmDcMotorImplConfig>::msg_in(int32_t& ecd_delta) {
    bool ret(true);
    switch (ECD_TYPE) {
        case TIM_INT16: {
            int16_t ecd = __HAL_TIM_GetCounter(&htim_ecd);
            ecd_delta = IN_DIR * static_cast<int32_t>(ecd);
        } break;
        case TIM_INT32: {
            int32_t ecd = __HAL_TIM_GetCounter(&htim_ecd);
            ecd_delta = IN_DIR * ecd;
        } break;
        default:
            break;
    }
    __HAL_TIM_SetCounter(&htim_ecd, 0);

    return ret;
}
} // namespace Motor
