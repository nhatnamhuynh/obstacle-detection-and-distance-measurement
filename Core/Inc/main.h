/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

// FSM
typedef enum {
  STATE_OUT_OF_RANGE = 0,
  STATE_SAFE,
  STATE_WARNING,
  STATE_DANGER,
} SystemState_t;

//Unit transformation
typedef enum {
  UNIT_CM = 0,
  UNIT_INCH
} DistanceUnit_t;

typedef struct {
  float filtered_distance;       // after filtering (moving average) (cm)
  DistanceUnit_t unit;
  SystemState_t state;
} SensorData_t;
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

// Hardware configuration (macro mapping)
// HC-SR04 sensor
#define TRIG_PIN GPIO_PIN_1   // PA1
#define TRIG_PORT GPIOA
#define ECHO_PIN GPIO_PIN_0   // PA0 (TIM2_CH1)
#define ECHO_PORT GPIOA

// LED
#define LED_GREEN_PIN GPIO_PIN_3   // PB3 (Safe)
#define LED_GREEN_PORT GPIOB
#define LED_YELLOW_PIN GPIO_PIN_4  // PB4 (Warning)
#define LED_YELLOW_PORT GPIOB
#define LED_RED_PIN GPIO_PIN_5     // PB5 (Danger)
#define LED_RED_PORT GPIOB

// Button and PWM Buzzer
#define BTN_UNIT_PIN GPIO_PIN_2   // PA2 (EXTI2)
#define BTN_UNIT_PORT GPIOA
#define BUZZER_PIN GPIO_PIN_8   // PB8 (TIM4_CH3)
#define BUZZER_PORT GPIOB

// Communication Protocol (UART, I2C), (No need to define Port as they are fixed in STM32F103C8T6)
#define I2C_SCL_PIN GPIO_PIN_6   // PB6 (I2C1_SCL)
#define I2C_SDA_PIN GPIO_PIN_7   // PB7 (I2C1_SDA)
#define UART_TX_PIN GPIO_PIN_9   // PA9 (USART1_TX)
#define UART_RX_PIN GPIO_PIN_10  // PA10 (USART1_RX)

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
