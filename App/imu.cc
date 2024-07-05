#include <cmath>
#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"
#include "MahonyAHRS.h"
#include "imu.hpp"
#include "user_lib_cpp.hpp"
#include "config.hpp"

// #warning "DEBUG"
// #include "bsp_usart.h"

namespace Dev {
[[noreturn]] void Imu::task(void* arg) {
    TickType_t xLastWakeTime;

    vTaskDelay(pdMS_TO_TICKS(Config::Time::IMU_INIT));

    while (BMI088_init()) {
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        BMI088_read(gyro_raw, acc_raw, &temp);

        acc[0] = -acc_raw[1];
        acc[1] = acc_raw[0];
        acc[2] = acc_raw[2];
        gyro[0] = -gyro_raw[1] + gyro_offset[0];
        gyro[1] = gyro_raw[0] + gyro_offset[1];
        gyro[2] = gyro_raw[2] + gyro_offset[2];

        MahonyAHRSupdateIMU(quat, gyro[0], gyro[1], gyro[2], acc[0], acc[1], acc[2]);
        calc_angle();

// #warning "DEBUG"
        // if (cali_time_count < CALI_TIME) cali_hook();
        // usart1_printf("%.6f,%.6f,%.6f\r\n", gyro_offset[0], gyro_offset[1], gyro_offset[2]);
        // usart1_printf("%.6f,%.6f,%.6f\r\n", angle[0], angle[1], angle[2]);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Config::Time::IMU_CYCLE));
    }
}

void Imu::calc_angle() {
    angle[0] = atan2f(
        2.0f * (quat[0] * quat[3] + quat[1] * quat[2]),
        2.0f * (quat[0] * quat[0] + quat[1] * quat[1]) - 1.0f
    );
    angle[1] = asinf(-2.0f * (quat[1] * quat[3] - quat[0] * quat[2]));
    angle[2] = atan2f(
        2.0f * (quat[0] * quat[1] + quat[2] * quat[3]),
        2.0f * (quat[0] * quat[0] + quat[3] * quat[3]) - 1.0f
    );
}

void Imu::cali_hook() {
    if (cali_time_count == 0) {
        gyro_offset[0] = 0;
        gyro_offset[1] = 0;
        gyro_offset[2] = 0;
    }

    gyro_offset[0] = gyro_offset[0] - 0.0008f * gyro[0];
    gyro_offset[1] = gyro_offset[1] - 0.0008f * gyro[1];
    gyro_offset[2] = gyro_offset[2] - 0.0008f * gyro[2];

    cali_time_count++;

    if (cali_time_count >= CALI_TIME) {
        if (UserLib::abs<float>(gyro_offset[0]) > GYRO_OFFSET_MAX)
            gyro_offset[0] = 0;
        if (UserLib::abs<float>(gyro_offset[1]) > GYRO_OFFSET_MAX)
            gyro_offset[1] = 0;
        if (UserLib::abs<float>(gyro_offset[2]) > GYRO_OFFSET_MAX)
            gyro_offset[2] = 0;
    }
}

} // namespace Dev
