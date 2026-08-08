#include "button_exti.h"

void Button_Init(void) {
    // TODO: Initialize button debouncing variables if needed
}

void Button_EXTI_Callback(uint16_t GPIO_Pin, SensorData_t *sensor_data) {
    // TODO: Check if GPIO_Pin == BTN_UNIT_PIN, debounce using HAL_GetTick(), toggle unit (CM <-> INCH)
}