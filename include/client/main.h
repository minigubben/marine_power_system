/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f0xx_hal.h"

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
#if defined(NODE_BOARD_INPUT_PANEL)
#define RS485_TX_EN_Pin GPIO_PIN_7
#define RS485_TX_EN_GPIO_Port GPIOA
#define Output_2_Pin GPIO_PIN_8
#define Output_2_GPIO_Port GPIOA
#define Input_1_Pin GPIO_PIN_11
#define Input_1_GPIO_Port GPIOA
#define Output_1_Pin GPIO_PIN_12
#define Output_1_GPIO_Port GPIOA
#define Output_6_Pin GPIO_PIN_1
#define Output_6_GPIO_Port GPIOB
#define Input_5_Pin GPIO_PIN_2
#define Input_5_GPIO_Port GPIOB
#define Input_6_Pin GPIO_PIN_0
#define Input_6_GPIO_Port GPIOB
#define Output_5_Pin GPIO_PIN_10
#define Output_5_GPIO_Port GPIOB
#define Input_4_Pin GPIO_PIN_11
#define Input_4_GPIO_Port GPIOB
#define Output_4_Pin GPIO_PIN_12
#define Output_4_GPIO_Port GPIOB
#define Input_3_Pin GPIO_PIN_13
#define Input_3_GPIO_Port GPIOB
#define Output_3_Pin GPIO_PIN_14
#define Output_3_GPIO_Port GPIOB
#define Input_2_Pin GPIO_PIN_15
#define Input_2_GPIO_Port GPIOB
#elif defined(NODE_BOARD_OUTPUT_8CH)
#define Output_1_Pin GPIO_PIN_13
#define Output_1_GPIO_Port GPIOC
#define Output_8_Pin GPIO_PIN_15
#define Output_8_GPIO_Port GPIOA
#define Output_7_Pin GPIO_PIN_4
#define Output_7_GPIO_Port GPIOB
#define Output_6_Pin GPIO_PIN_5
#define Output_6_GPIO_Port GPIOB
#define Output_5_Pin GPIO_PIN_6
#define Output_5_GPIO_Port GPIOB
#define Output_4_Pin GPIO_PIN_7
#define Output_4_GPIO_Port GPIOB
#define Output_3_Pin GPIO_PIN_8
#define Output_3_GPIO_Port GPIOB
#define Output_2_Pin GPIO_PIN_9
#define Output_2_GPIO_Port GPIOB
#define RS485_TX_EN_Pin GPIO_PIN_10
#define RS485_TX_EN_GPIO_Port GPIOB
#else
#error "Define NODE_BOARD_INPUT_PANEL or NODE_BOARD_OUTPUT_8CH in the PlatformIO environment"
#endif

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
