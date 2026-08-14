#include "uart_log.h"

void UART_Log_Init(void) {
    // TODO: Send startup log via HAL_UART_Transmit(&huart1, ...)
    static char init_data[] = "START LOGGING STATE DATA\r\n"; 
    HAL_UART_Transmit_IT(&huart1, (uint8_t *) init_data, sizeof(init_data) - 1);
}

void UART_Log_Process(const SensorData_t *sensor_data) {
    // TODO: Check non-blocking 200ms timer with HAL_GetTick(), sprintf log string, send via huart1
    static uint32_t last_log = 0;
    uint32_t current_time = HAL_GetTick();

    static char log_data[64] = "";

    if (current_time - last_log >= LOG_INTERVAL_MS){
        if (huart1.gState == HAL_UART_STATE_READY){
            last_log = current_time;
            int log_len = snprintf(log_data, sizeof(log_data), "Dist: %-6.1f cm | %s\r\n", sensor_data->filtered_distance, sensor_data->state);
            if (log_len > 0) {
                HAL_UART_Transmit_IT(&huart1, (uint8_t *) log_data, log_len);
            }
        }
    }
}