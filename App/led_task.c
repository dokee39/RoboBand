#include <math.h>

#include "FreeRTOS.h"
#include "task.h"
#include "led_task.h"
#include "ws2812.h"

#define LED_FLOW_PERIOD pdMS_TO_TICKS(1200)
#define LED_BRIGHTNESS 16

void led_task(void* argument) {
    TickType_t xLastWakeTime;
    uint32_t time = 0;
    uint8_t r, b, g;

    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        time = xTaskGetTickCount();
        time %= LED_FLOW_PERIOD;
        r = (sin(((float)time / LED_FLOW_PERIOD) * 2 * 3.1416f) + 1) * LED_BRIGHTNESS;
        b = (sin(((float)time / LED_FLOW_PERIOD + 0.3333f) * 2 * 3.1416f) + 1) * LED_BRIGHTNESS;
        g = (sin(((float)time / LED_FLOW_PERIOD + 0.6667f) * 2 * 3.1416f) + 1) * LED_BRIGHTNESS;
        WS2812_Ctrl(r, b, g);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));
    }
}
