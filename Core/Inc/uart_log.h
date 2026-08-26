#ifndef __UART_LOG_H
#define __UART_LOG_H

#include "main.h"

#define LOG_INTERVAL_MS 200

void UART_Log_Init(void);
void UART_Log_Process(const SensorData_t *sensor_data);

#endif /* __UART_LOG_H */