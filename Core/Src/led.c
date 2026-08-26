#include "led.h"

static SystemState_t current_state = STATE_OUT_OF_RANGE;

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

    led->last_toggle_time = 0;
    led->toggle_flag = 0;

    LED_AllOff(led);
}

void LED_Update(Led_t *led, SystemState_t state) {
    if (led == NULL) return;
    uint32_t current_time = HAL_GetTick();

    if (current_state != state) {
        current_state = state;
        led->last_toggle_time = current_time;
        led->toggle_flag = 0;
        LED_AllOff(led);
    }

    switch (current_state) {
        case STATE_OUT_OF_RANGE: break;
        case STATE_SAFE: 
            Led_WritePin (led->green_port, led->green_pin, 1);
            break;
        case STATE_WARNING: 
            if (current_time - led->last_toggle_time >= 500) {
                led->last_toggle_time = current_time;
                led->toggle_flag = !led->toggle_flag; // Đảo trạng thái cờ
                Led_WritePin(led->yellow_port, led->yellow_pin, led->toggle_flag);
            }
            break;
        case STATE_DANGER:
            if (current_time - led->last_toggle_time >= 100) {
                led->last_toggle_time = current_time;
                led->toggle_flag = !led->toggle_flag; // Đảo trạng thái cờ
                Led_WritePin(led->red_port, led->red_pin, led->toggle_flag);
            }
            break;
    }
}