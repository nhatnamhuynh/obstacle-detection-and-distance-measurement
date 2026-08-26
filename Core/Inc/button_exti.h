#ifndef __BUTTON_EXTI_H
#define __BUTTON_EXTI_H

#include "main.h"

#define DEBOUNCE_DELAY_MS 200

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    volatile uint8_t flag;
    volatile uint32_t last_press;
} Button_t;

void Button_Init(Button_t *btn, GPIO_TypeDef *port, uint16_t pin);
uint8_t Button_WasPressed (Button_t *btn);
void Button_EXTI_Callback(Button_t *btn, uint16_t GPIO_pin);

void HandleUnitButton (Button_t *btn, SensorData_t *data);


#endif /* __BUTTON_EXTI_H */