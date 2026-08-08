#include "uart_log.h"

void UART_Log_Init(void) {
    // TODO: Send startup log via HAL_UART_Transmit(&huart1, ...)
}

void UART_Log_Process(const SensorData_t *sensor_data) {
    // TODO: Check non-blocking 200ms timer with HAL_GetTick(), sprintf log string, send via huart1
}