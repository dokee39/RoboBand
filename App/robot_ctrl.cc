#include "robot_ctrl.hpp"
#include "FreeRTOS.h"
#include "task.h"
#include "config.hpp"
#include "remoter.h"
#include "bsp_usart.h"

extern remoter_t remoter;

static void robot_update_set_task(void* arg)
{
    static_cast<Robot::RobotCtrl*>(arg)->update_set_task(nullptr);
}

namespace Robot {
RobotCtrl::RobotCtrl(Chassis::ChassisBase* chassis):
    chassis(chassis),
    chassis_set(chassis->set) {
    TaskHandle_t xCreatedUpdateSetTask = *(new TaskHandle_t);

    usart_sbus_init();

    xTaskCreate(
        robot_update_set_task,
        "update set task",
        configMINIMAL_STACK_SIZE * 8,
        static_cast<void*>(this),
        (tskIDLE_PRIORITY + Config::Priority::UPDATE_SET),
        &xCreatedUpdateSetTask
    );
}

[[noreturn]] void RobotCtrl::update_set_task(void* arg) {
    TickType_t xLastWakeTime;

    vTaskDelay(pdMS_TO_TICKS(Config::Time::UPDATE_SET_INIT));

    xLastWakeTime = xTaskGetTickCount();

    while (true) {
        float vx_set = remoter.rc.ch[1] * Config::RC::VX_SET_RATIO;
        float vy_set = -remoter.rc.ch[0] * Config::RC::VY_SET_RATIO;
        float wz_set = -remoter.rc.ch[2] * Config::RC::WZ_SET_RATIO;
        chassis->set.vx = vx_set;
        chassis->set.vy = vy_set;
        chassis->set.wz = wz_set;

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Config::Time::UPDATE_SET_CYCLE));
    }
}
} // namespace Robot
