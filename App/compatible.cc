#include "compatible.h"
#include "FreeRTOS.h"
#include "task.h"
#include "config.hpp"
#include "led_task.h"
#include "double_wheel_balance_chassis.hpp"
#include "robot_ctrl.hpp"

#warning DEBUG
#include "bsp_usart.h"

void main_entry(void) {
    TaskHandle_t xCreatedLedTask;

    xTaskCreate(
        led_task,
        "led task",
        configMINIMAL_STACK_SIZE * 1,
        NULL,
        (tskIDLE_PRIORITY + 1),
        &xCreatedLedTask
    );

    usart_sbus_init();
    // auto chassis = new Chassis::Chassis(Config::Chassis::DoubleWheelBalance);
    // auto robot = new Robot::RobotCtrl(chassis);
    // UNUSED(robot);

    /* Start scheduler */
    vTaskStartScheduler();
}
