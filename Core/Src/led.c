#include "led.h"

static void Led_WritePin (GPIO_TypeDef *port, uint16_t pin, uint8_t state){
    if (port == NULL) return;
    HAL_GPIO_WritePin (port, pin, (state)? (GPIO_PIN_SET) : (GPIO_PIN_RESET));
}

void LED_AllOff (Led_t *led) {
    if (led == NULL) return;
    Led_WritePin (led->green_port, led->green_pin, 0);
    Led_WritePin (led->yellow_port, led->yellow_pin, 0);
    Led_WritePin (led->red_port, led->red_pin, 0);
}

void LED_Init(Led_t *led,
              GPIO_TypeDef *g_port, uint16_t g_pin,
              GPIO_TypeDef *y_port, uint16_t y_pin,
              GPIO_TypeDef *r_port, uint16_t r_pin) {
    if (led == NULL) return;

    led->green_port = g_port;
    led->green_pin = g_pin;

    led->yellow_port = y_port;
    led->yellow_pin = y_pin;

    led->red_port = r_port;
    led->red_pin = r_pin;

    led->state = LED_STATE_OFF;
    led->last_toggle_time = 0;
    led->toggle_flag = 0;

    LED_AllOff(led);
}

void LED_SetState(Led_t *led, LedState_t state) {
    if (led == NULL || led->state == state) return;

    led->state = state;
    led->last_toggle_time = HAL_GetTick();
    led->toggle_flag = 1;

    LED_AllOff(led); // turn off all led before change to next state

    switch (state) {
        case LED_STATE_OFF: break;
        case LED_STATE_SAFE: 
            Led_WritePin (led->green_port, led->green_pin, 1);
            break;
        case LED_STATE_WARNING:
            Led_WritePin (led->yellow_port, led->yellow_pin, 1);
            break;
        case LED_STATE_DANGER:
            Led_WritePin (led->red_port, led->red_pin, 1);
            break;
        default: break;

    }
}

void LED_Update(Led_t *led) {
    if (led == NULL) return;

    uint32_t current_time = HAL_GetTick();

    switch (led->state) {
        case LED_STATE_OFF: break;
        case LED_STATE_SAFE: break;
        case LED_STATE_WARNING: 
            if (current_time - led->last_toggle_time >= 500) {
                led->toggle_flag = !led->toggle_flag;
                Led_WritePin (led->yellow_port, led->yellow_pin, led->toggle_flag);
                led->last_toggle_time = current_time;
            }
            break;
        case LED_STATE_DANGER:
            if (current_time - led->last_toggle_time >= 100) {
                led->toggle_flag = !led->toggle_flag;
                Led_WritePin (led->red_port, led->red_pin, led->toggle_flag);
                led->last_toggle_time = current_time;
            }
            break;
    }
}