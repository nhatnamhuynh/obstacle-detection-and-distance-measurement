#include "buzzer.h"

void Buzzer_Init(void) {
    // TODO: Call HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3)
}

void Buzzer_Update(SystemState_t state, float distance) {
    // TODO: Adjust PWM Duty Cycle / Frequency based on state (SAFE=Off, WARN=Beep, DANGER=Continuous)
}

void Buzzer_Stop(void) {
    // TODO: Set PWM Duty Cycle to 0 or call HAL_TIM_PWM_Stop()
}