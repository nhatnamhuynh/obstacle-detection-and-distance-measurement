#ifndef __LED_H
#define __LED_H

#include "main.h"

typedef struct {
    GPIO_TypeDef *green_port;
    uint16_t green_pin;

    GPIO_TypeDef *yellow_port;
    uint16_t yellow_pin;

    GPIO_TypeDef *red_port;
    uint16_t red_pin;

    uint32_t last_toggle_time;
    uint8_t toggle_flag;

} Led_t;

void LED_Init(Led_t *led,
              GPIO_TypeDef *g_port, uint16_t g_pin,
              GPIO_TypeDef *y_port, uint16_t y_pin,
              GPIO_TypeDef *r_port, uint16_t r_pin);

// void LED_SetState(Led_t *led, LedState_t state);

void LED_Update(Led_t *led, SystemState_t state);
void LED_AllOff (Led_t *led);

#endif /* __LED_H */