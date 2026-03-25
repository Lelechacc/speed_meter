#include "GPS.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "ui.h"
#include "GPS_Transform.h"

#define GPS_BUF_LEN 128
#define GPS_DMA_BUF_LEN 16

static uint8_t usart2_dma_buffer[GPS_DMA_BUF_LEN];
static char gps_buffer[GPS_BUF_LEN];
static uint8_t gps_buffer_index;
static uint32_t last_tick_count;
static float distance = 0.0f;
static float ride_time_s = 0;

extern osThreadId_t gps_taskHandle;
extern ui_t *ui;

float gps_datatype_transform(char *lat_or_lon, char dir)
{
    if (!lat_or_lon)
    {
        return 0.0f;
    }
    if (dir != 'N' && dir != 'S' && dir != 'E' && dir != 'W')
    {
        return 0.0f;
    }
    if (strlen(lat_or_lon) < 10)
    {
        return 0.0f;
    }

    float result = 0.0f;
    if (dir == 'N' || dir == 'S')
    {
        //
        char temp[3];
        temp[0] = lat_or_lon[0];
        temp[1] = lat_or_lon[1];
        temp[2] = 0;
        result += atof(temp);
        result += atof(lat_or_lon + 2) / 60;
        if (dir == 'S')
        {
            result = -result;
        }
    }
    else
    {
        //
        char temp[4];
        temp[0] = lat_or_lon[0];
        temp[1] = lat_or_lon[1];
        temp[2] = lat_or_lon[2];
        temp[3] = 0;
        result += atof(temp);
        result += atof(lat_or_lon + 3) / 60;
        if (dir == 'W')
        {
            result = -result;
        }
    }
    return result;
}

float latitude = 0.0f, longitude = 0.0f;
int fs = 0, sa_num = 0;
float velocity = 0.0f;

void updata_pos_async_cb(void *arg)
{
    // ui_update_satellite(ui, sa_num);
    lv_subject_set_int(&ui->data.gps_sat_num, sa_num);
    GPS_Transform(latitude, longitude, &latitude, &longitude);
    // ui_update_pos(ui, latitude, longitude);
    lv_subject_set_float(&ui->data.latitude, latitude);
    lv_subject_set_float(&ui->data.longitude, longitude);
}

void updata_speed_async_cb(void *arg)
{
    // ui_update_speed(ui, velocity);
    lv_subject_set_float(&ui->data.speed, velocity);
}

void updata_distance_async_cb(void *arg)
{
    // ui_update_distance(ui, distance);
    lv_subject_set_float(&ui->data.distance, distance);
}

void updata_ride_time_async_cb(void *arg)
{
    // ui_update_ride_time(ui, ride_time_s);
    lv_subject_set_int(&ui->data.hour, ride_time_s / 60);
    lv_subject_set_int(&ui->data.minute, (uint32_t)ride_time_s % 60);
}

void updata_status_async_cb(void *arg)
{

    int status = (int)arg;
    // ui_update_status(ui, status);

    lv_subject_set_pointer(&ui->data.run_status, (void *)run_status_text[status]);
}

void gps_task_func(void *argument)
{
    printf("GPS task started\n");
    GPS_Init();
    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        char *line_end = strstr(gps_buffer, "\n");
        printf("gpsbuff:%s\n", gps_buffer);
        if (line_end == NULL)
        {
            // vTaskDelay(1000);
            continue;
        }

        line_end[0] = '\0';

        if (strstr(gps_buffer, "GGA"))
        {

            char *latitude_str = NULL, *longitude_str = NULL;

            char *token = strtok(gps_buffer, ",");
            uint8_t field_index = 0;
            while (token)
            {
                switch (field_index)
                {
                case 2:
                    latitude_str = token;
                    break;
                case 3:
                    latitude = gps_datatype_transform(latitude_str, token[0]);
                    break;
                case 4:
                    longitude_str = token;
                    break;
                case 5:
                    longitude = gps_datatype_transform(longitude_str, token[0]);
                    break;
                case 6:
                    fs = token[0] - '0';
                    break;
                case 7:
                    sa_num = atoi(token);
                    break;
                default:
                    break;
                }
                if (field_index == 7)
                {
                    break;
                }
                token = strtok(NULL, ",");
                field_index++;
            }
            if (fs != 0)
            {
                lv_async_call(updata_pos_async_cb, NULL);
            }
        }
        else if (strstr(gps_buffer, "VTG"))
        {
            char *token = strtok(gps_buffer, ",");
            uint8_t field_index = 0;

            while (token)
            {
                switch (field_index)
                {
                case 7:
                    velocity = atof(token);
                    break;
                default:
                    break;
                }
                if (field_index == 7)
                {
                    break;
                }
                token = strtok(NULL, ",");
                field_index++;
            }

            uint32_t current_tick = xTaskGetTickCount();
            lv_async_call(updata_speed_async_cb, NULL);

            float seconds = (float)(current_tick - last_tick_count) / configTICK_RATE_HZ;
            last_tick_count = current_tick;
            uint8_t status;
            if (velocity > 0.5f)
            {
                distance += velocity * seconds / 3600.0f;
                lv_async_call(updata_distance_async_cb, NULL);
                ride_time_s += seconds;

                lv_async_call(updata_ride_time_async_cb, NULL);
                status = 1;
                lv_async_call(updata_status_async_cb, (void *)status);
            }
            else
            {
                status = 0;
                lv_async_call(updata_status_async_cb, (void *)status);
            }
        }
        line_end++;
        memmove(gps_buffer, line_end, gps_buffer_index - (line_end - gps_buffer));
        gps_buffer_index -= line_end - gps_buffer;
        // vTaskDelay(1000);
    }
}

void GPS_UART2_RecvCallback(UART_HandleTypeDef *huart)
{
    if (gps_buffer_index <= GPS_BUF_LEN - GPS_DMA_BUF_LEN / 2)
    {
        memcpy(gps_buffer + gps_buffer_index, usart2_dma_buffer + GPS_DMA_BUF_LEN / 2, GPS_DMA_BUF_LEN / 2);
        gps_buffer_index += GPS_DMA_BUF_LEN / 2;
    }
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(gps_taskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
void GPS_UART2_HalfRecvCallback(UART_HandleTypeDef *huart)
{
    if (gps_buffer_index <= GPS_BUF_LEN - GPS_DMA_BUF_LEN / 2)
    {
        memcpy(gps_buffer + gps_buffer_index, usart2_dma_buffer, GPS_DMA_BUF_LEN / 2);
        gps_buffer_index += GPS_DMA_BUF_LEN / 2;
    }
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(gps_taskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void GPS_Init(void)
{
    HAL_UART_RegisterCallback(&huart2, HAL_UART_RX_HALFCOMPLETE_CB_ID, GPS_UART2_HalfRecvCallback);
    HAL_UART_RegisterCallback(&huart2, HAL_UART_RX_COMPLETE_CB_ID, GPS_UART2_RecvCallback);
    HAL_UART_Receive_DMA(&huart2, usart2_dma_buffer, sizeof(usart2_dma_buffer));
}
