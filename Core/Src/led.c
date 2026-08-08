#include "led.h"

void LED_Init(void) {
    // TODO: Call LED_AllOff()
}

void LED_Update(SystemState_t state) {
    // TODO: Switch state: SAFE -> Green ON; WARNING -> Yellow ON; DANGER -> Red ON; OUT_OF_RANGE -> All OFF
}

void LED_AllOff(void) {
    // TODO: Write GPIO_PIN_RESET to Green, Yellow, Red pins
}