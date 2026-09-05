#include "test_led.h"
#include "main.h"
#include <string.h>

extern UART_HandleTypeDef huart1;

void Test_3_LED_Blink(void) {
    char msg_start[] = "[TEST] 3-LED Blink Test Started...\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg_start, strlen(msg_start), HAL_MAX_DELAY);

    for (int i = 0; i < 3; i++) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_SET);
        HAL_Delay(500);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_Delay(500);
    }

    char msg_end[] = "[TEST] 3-LED Blink Test PASSED!\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg_end, strlen(msg_end), HAL_MAX_DELAY);
}