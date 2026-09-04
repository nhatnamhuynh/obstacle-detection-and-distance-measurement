#include "test_uart.h"
#include <string.h>

void Test_UART_Terminal(UART_HandleTypeDef *huart) {
    char msg[] = "[TC1] UART functions normally!\r\n";
    HAL_UART_Transmit(huart, (uint8_t *)msg, strlen(msg), 100);
}