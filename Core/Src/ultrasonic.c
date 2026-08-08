#include "ultrasonic.h"

void Ultrasonic_Init(void) {
    // TODO: Call HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1)
}

void Ultrasonic_Trigger(void) {
    // TODO: Set TRIG_PIN HIGH -> delay 10us -> Set TRIG_PIN LOW
}

float Ultrasonic_ReadDistance(void) {
    // TODO: Return distance (cm) = (echo_time_us * 0.0343f) / 2.0f
    return 0.0f;
}

void Ultrasonic_CaptureCallback(TIM_HandleTypeDef *htim) {
    // TODO: Measure pulse width on TIM2 Channel 1 (Rising to Falling edge)
}