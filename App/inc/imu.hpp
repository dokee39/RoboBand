#pragma once

#include <cstdint>
#include "FreeRTOS.h"
#include "BMI088driver.h"

namespace Dev {
class Imu {
public:
    explicit Imu() = default;
    ~Imu() = default;

    float gyro[3] = { 0.0f };
    float gyro_raw[3] = { 0.0f };
    float gyro_offset[3] = { 0.017172f, 0.015918f, -0.007618f };
    float acc[3] = { 0.0f };
    float acc_raw[3] = { 0.0f };
    float angle[3] = { 0.0f }; // roll pitch yaw
    float temp = 0.0f;

    [[noreturn]] void task(void* arg);

private:
    float quat[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
    const float GYRO_OFFSET_MAX = 0.05f;
    uint32_t cali_time_count = 0;
    const uint32_t CALI_TIME = 10000;

    void calc_angle();
    void cali_hook();
};
} // namespace Dev
