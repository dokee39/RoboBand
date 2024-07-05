#pragma once

#include <cstdint>

// config files
#include "double_wheel_balance_chassis_config.hpp"

namespace Config {
namespace Time {
    const uint32_t LED_CYCLE(10);
    const uint32_t IMU_INIT(7);
    const uint32_t IMU_CYCLE(1);
    const uint32_t CHASSIS_INIT(100);
    const uint32_t CHASSIS_CYCLE(1);
    const uint32_t UPDATE_SET_INIT(50);
    const uint32_t UPDATE_SET_CYCLE(1);
} // namespace Time

namespace Priority {
    const uint32_t IMU(7);
    const uint32_t UPDATE_SET(6);
    const uint32_t CHASSIS(5);
} // namespace Priority

namespace RC {
    const float VX_SET_RATIO(2.0 / 700.0f);
    const float VY_SET_RATIO(0.0f);
    const float WZ_SET_RATIO(0.0f);
} // namespace RC
} // namespace Config
