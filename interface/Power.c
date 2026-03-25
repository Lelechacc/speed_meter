#include "Power.h"
#include "gpio.h"
#include "adc.h"
#include "Key.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include "ui.h"

extern ui_t *ui;
extern SemaphoreHandle_t lvgl_mutex;
bool isCharging;
float voltage;

void updata_battery_async_cb(void *arg)
{
    // 这个函数会在 LVGL 任务中安全执行，不需要加 mutex 锁
    // 计算百分比并限制在 0-100 之间
    uint8_t per = (uint8_t)(voltage * 100.0f / 4.2f);
    if(per > 100) per = 100;
    
    // 调用你 ui.c 里的那个函数
    ui_update_battery(ui, isCharging, per);
    // ui_update_battery(ui, isCharging, (uint8_t)(voltage * 100.0f / 4.2f));

}

void Power_Off(void *arg)
{
    vTaskDelay(2);
    HAL_GPIO_WritePin(POWER_EN_GPIO_Port, POWER_EN_Pin, GPIO_PIN_RESET);
}

void power_task_func(void *arg)
{
    Power_Init();
    printf("Power function start\n");

    for (;;)
    {
        isCharging = Power_IsCharging();
       voltage = Power_GetBatteryVoltage();

        // printf("isCharging: %d, voltage: %f\n", isCharging, voltage); 
        // if (xSemaphoreTake(lvgl_mutex, pdMS_TO_TICKS(10)) == pdTRUE)
        // {
        //     ui_update_battery(ui, isCharging, (uint8_t)(voltage * 100.0f / 4.2f));
        //     xSemaphoreGive(lvgl_mutex);
        // }

        // 使用lvgl的方法也能解决线程安全问题 => 延后到下一次timer_hander之前执行
        lv_async_call(updata_battery_async_cb, NULL);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void Power_Init()
{
    HAL_ADC_Start(&hadc1);
    vTaskDelay(pdMS_TO_TICKS(1000));
    Key_RegisterCallback(KEY_POWER, Power_Off, NULL);
}

bool Power_IsCharging()
{
    return HAL_GPIO_ReadPin(CHG_DET_GPIO_Port, CHG_DET_Pin) == GPIO_PIN_RESET;
}

float Power_GetBatteryVoltage()
{
    uint32_t val = HAL_ADC_GetValue(&hadc1);
    return ((float)(val * 2)) / 4096.0f * 3.3f;
}
