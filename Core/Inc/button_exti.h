#ifndef __BUTTON_EXTI_H
#define __BUTTON_EXTI_H

#include "main.h"

#define DEBOUNCE_DELAY_MS 200

void Button_Init(void);
void Button_EXTI_Callback(uint16_t GPIO_Pin, SensorData_t *sensor_data);

#endif /* __BUTTON_EXTI_H */