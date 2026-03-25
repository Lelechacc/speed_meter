#include "Key.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
/* --- 在 Key.c 顶部添加 --- */
#include "lvgl.h" // 必须包含这个，否则不认识 lv_async_call
#include "ui_conf.h"
extern void ui_toggle_theme_wrapper(void *arg);
//  1. 声明我们刚写的模拟走路启停函数
extern void ui_toggle_simulation_wrapper(void *arg);
//  1. 新增：声明水滴按键的包装函数
extern void ui_map_center_wrapper(void *arg);

//  2. 声明你的全局 UI 指针 (你在主程序里肯定定义了这个 ui_t 结构体变量)
extern ui_t *ui;
extern void ui_toggle_theme_wrapper(void *arg); // 声明我们在 ui.c 里写的函数
static GPIO_TypeDef *KEY_GPIO_PORTS[] = {
    PWR_DET_GPIO_Port, KEY1_GPIO_Port, KEY2_GPIO_Port, KEY3_GPIO_Port, KEY4_GPIO_Port, KEY5_GPIO_Port};
static uint16_t KEY_PINS[] = {
    PWR_DET_Pin, KEY1_Pin, KEY2_Pin, KEY3_Pin, KEY4_Pin, KEY5_Pin};

typedef struct
{
    uint8_t Key_Status[KEY_DEBOUNCE_INTERVAL / KEY_SCAN_INTERVAL];
    Key_Function_t Key_Callbacks[KEY_MAX];
    void *Key_Function_Arg[KEY_MAX];
    uint8_t Key_Status_Index;
    uint8_t Key_Press_State; //
} Key_Driver_t;

static Key_Driver_t key_driver;

void key_task_func(void *argument)
{
    printf("Key task started\n");
    Key_Init();
    for (;;)
    {
        int current_tick = xTaskGetTickCount();
        uint8_t current_status = 0;
        for (size_t i = KEY_POWER; i < KEY_MAX; i++)
        {
            current_status |= (HAL_GPIO_ReadPin(KEY_GPIO_PORTS[i], KEY_PINS[i]) << i);
        }
        key_driver.Key_Status[key_driver.Key_Status_Index++] = current_status;
        if (key_driver.Key_Status_Index >= KEY_DEBOUNCE_INTERVAL / KEY_SCAN_INTERVAL)
        {
            key_driver.Key_Status_Index = 0;
        }
        uint8_t all_released = 0xFF;
        uint8_t all_pressed = 0x00;
        for (size_t i = 0; i < KEY_DEBOUNCE_INTERVAL / KEY_SCAN_INTERVAL; i++)
        {
            all_released &= key_driver.Key_Status[i];
            all_pressed |= key_driver.Key_Status[i];
        }

        for (size_t i = KEY_POWER; i < KEY_MAX; i++)
        {
            uint8_t key_mask_bit = (1 << i);
            if ((all_pressed & key_mask_bit) == 0)
            {
                if ((key_driver.Key_Press_State & key_mask_bit) == 0)
                {
                    printf("Key %u pressed\n", i);
                    if (i == 4) // 4 对应的就是数组里的 KEY4
                    {
                        // 发送异步工单给 UI 任务
                        lv_async_call(ui_toggle_theme_wrapper, NULL);
                    }
                    else if (i == 3)
                    {
                        // 异步调用，把 ui_ptr 传进去，安全启停模拟！
                        lv_async_call(ui_toggle_simulation_wrapper, ui);
                    }
                    else if (i == 2)
                    {
                        lv_async_call(ui_map_center_wrapper, ui);
                    }
                }
                key_driver.Key_Press_State |= key_mask_bit;
            }
            if (all_released & key_mask_bit)
            {
                if (key_driver.Key_Press_State & key_mask_bit)
                {
                    printf("Key %u released\n", i);
                    if (key_driver.Key_Callbacks[i])
                    {
                        key_driver.Key_Callbacks[i](key_driver.Key_Function_Arg[i]);
                    }
                }
                key_driver.Key_Press_State &= ~key_mask_bit;
            }
        }
        vTaskDelay(KEY_SCAN_INTERVAL - (xTaskGetTickCount() - current_tick));
    }
}

void Key_Init()
{
    memset(&key_driver, 0, sizeof(key_driver));
}

void Key_RegisterCallback(Key_ID_t key, Key_Function_t callback, void *arg)
{
    key_driver.Key_Callbacks[key] = callback;
    key_driver.Key_Function_Arg[key] = arg;
}
