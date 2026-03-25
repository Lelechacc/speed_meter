/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_1
#define KEY2_GPIO_Port GPIOC
#define KEY3_Pin GPIO_PIN_2
#define KEY3_GPIO_Port GPIOC
#define BAT_DET_Pin GPIO_PIN_1
#define BAT_DET_GPIO_Port GPIOA
#define GPS_TX_Pin GPIO_PIN_2
#define GPS_TX_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_3
#define GPS_RX_GPIO_Port GPIOA
#define ST_DC_Pin GPIO_PIN_4
#define ST_DC_GPIO_Port GPIOA
#define ST_SCK_Pin GPIO_PIN_5
#define ST_SCK_GPIO_Port GPIOA
#define ST_RST_Pin GPIO_PIN_6
#define ST_RST_GPIO_Port GPIOA
#define ST_DATA_Pin GPIO_PIN_7
#define ST_DATA_GPIO_Port GPIOA
#define KEY5_Pin GPIO_PIN_4
#define KEY5_GPIO_Port GPIOC
#define ST_CS_Pin GPIO_PIN_0
#define ST_CS_GPIO_Port GPIOB
#define ST_BL_Pin GPIO_PIN_1
#define ST_BL_GPIO_Port GPIOB
#define CTP_SCL_Pin GPIO_PIN_10
#define CTP_SCL_GPIO_Port GPIOB
#define CTP_SDA_Pin GPIO_PIN_11
#define CTP_SDA_GPIO_Port GPIOB
#define CTP_RST_Pin GPIO_PIN_12
#define CTP_RST_GPIO_Port GPIOB
#define CTP_INT_Pin GPIO_PIN_13
#define CTP_INT_GPIO_Port GPIOB
#define CTP_INT_EXTI_IRQn EXTI15_10_IRQn
#define POWER_EN_Pin GPIO_PIN_14
#define POWER_EN_GPIO_Port GPIOB
#define CHG_DET_Pin GPIO_PIN_15
#define CHG_DET_GPIO_Port GPIOB
#define BLE_TX_Pin GPIO_PIN_6
#define BLE_TX_GPIO_Port GPIOC
#define BLE_RX_Pin GPIO_PIN_7
#define BLE_RX_GPIO_Port GPIOC
#define SD_CD_Pin GPIO_PIN_8
#define SD_CD_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_9
#define DEBUG_TX_GPIO_Port GPIOA
#define DEBUG_RX_Pin GPIO_PIN_10
#define DEBUG_RX_GPIO_Port GPIOA
#define KEY4_Pin GPIO_PIN_15
#define KEY4_GPIO_Port GPIOA
#define PWR_DET_Pin GPIO_PIN_3
#define PWR_DET_GPIO_Port GPIOB
#define LIS_INT_Pin GPIO_PIN_5
#define LIS_INT_GPIO_Port GPIOB
#define LIS_INT_EXTI_IRQn EXTI9_5_IRQn
#define MEMS_SCL_Pin GPIO_PIN_6
#define MEMS_SCL_GPIO_Port GPIOB
#define MEMS_SDA_Pin GPIO_PIN_7
#define MEMS_SDA_GPIO_Port GPIOB
#define LSM_INT1_Pin GPIO_PIN_8
#define LSM_INT1_GPIO_Port GPIOB
#define LSM_INT1_EXTI_IRQn EXTI9_5_IRQn
#define LSM_INT2_Pin GPIO_PIN_9
#define LSM_INT2_GPIO_Port GPIOB
#define LSM_INT2_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
