#include "chassis_base.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "config.hpp"

namespace Chassis {
[[noreturn]] void ChassisBase::task(void* arg) {
    TickType_t xLastWakeTime;

    vTaskDelay(pdMS_TO_TICKS(Config::Time::CHASSIS_INIT));

    xLastWakeTime = xTaskGetTickCount();

    while (true) {
        update_state();
        ctrl_val_calc();
        ctrl();

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Config::Time::CHASSIS_CYCLE));
    }
}
} // namespace Chassis
