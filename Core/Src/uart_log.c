#include "uart_log.h"

void UART_Log_Init(void) {
    // TODO: Send startup log via HAL_UART_Transmit(&huart1, ...)
    const static char init_data[] = "START LOGGING STATE DATA\r\n"; 
    HAL_UART_Transmit_IT(&huart1, (uint8_t *) init_data, sizeof(init_data) - 1);
}

void UART_Log_Process(const SensorData_t *sensor_data) {
    // TODO: Check non-blocking 200ms timer with HAL_GetTick(), sprintf log string, send via huart1
    static uint32_t last_log = 0;
    uint32_t current_time = HAL_GetTick();

    static char log_data[64] = "";
    const char* unit_str = "cm";
    const char* state_str = "UNKNOWN";

    if (current_time - last_log >= LOG_INTERVAL_MS){
        if (huart1.gState == HAL_UART_STATE_READY){

            if (sensor_data->unit == 0) unit_str = "cm";
            else unit_str = "in";

            switch (sensor_data->state) {
                case STATE_SAFE:
                    state_str = "SAFE";
                    break;
                case STATE_WARNING:
                    state_str = "WARNING";
                    break;
                case STATE_DANGER:
                    state_str = "DANGER";
                    break;
                case STATE_OUT_OF_RANGE:
                    state_str = "OUT OF RANGE";
                    break;
                default:
                    state_str = "UNKNOWN";
                    break;
            }

            int log_len = snprintf(log_data, sizeof(log_data), "Dist: %-6.1f %-2s | %-12s\r\n", sensor_data->filtered_distance, unit_str, state_str);
            if (log_len > 0) {
                HAL_UART_Transmit_IT(&huart1, (uint8_t *) log_data, log_len);
            }

            last_log = current_time;
        }
    }
}