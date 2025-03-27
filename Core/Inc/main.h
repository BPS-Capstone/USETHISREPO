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
#include "stm32h7xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
int copyReadBuffer(uint8_t * buf);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BRUSHLESS_PWM_Pin GPIO_PIN_7
#define BRUSHLESS_PWM_GPIO_Port GPIOA
#define BRUSHLESS_DIR_Pin GPIO_PIN_4
#define BRUSHLESS_DIR_GPIO_Port GPIOC
#define BRUSHLESS_FEEDBACK_Pin GPIO_PIN_5
#define BRUSHLESS_FEEDBACK_GPIO_Port GPIOC
#define VALVE_12V_Pin GPIO_PIN_7
#define VALVE_12V_GPIO_Port GPIOE
#define STPR_DIR_Pin GPIO_PIN_10
#define STPR_DIR_GPIO_Port GPIOE
#define STPR_STEP_Pin GPIO_PIN_11
#define STPR_STEP_GPIO_Port GPIOE
#define STPR_SLEEP_Pin GPIO_PIN_12
#define STPR_SLEEP_GPIO_Port GPIOE
#define STPR_RST_Pin GPIO_PIN_13
#define STPR_RST_GPIO_Port GPIOE
#define STPR_MS3_Pin GPIO_PIN_14
#define STPR_MS3_GPIO_Port GPIOE
#define STPR_MS2_Pin GPIO_PIN_15
#define STPR_MS2_GPIO_Port GPIOE
#define STPR_MS1_Pin GPIO_PIN_10
#define STPR_MS1_GPIO_Port GPIOB
#define STPR_EN_Pin GPIO_PIN_11
#define STPR_EN_GPIO_Port GPIOB
#define EN24V_Pin GPIO_PIN_12
#define EN24V_GPIO_Port GPIOB
#define EN5V_Pin GPIO_PIN_13
#define EN5V_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
uint32_t get_time_ms();
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
