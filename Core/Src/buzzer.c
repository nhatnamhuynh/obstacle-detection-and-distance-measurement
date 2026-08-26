#include "buzzer.h"

static SystemState_t current_state = STATE_OUT_OF_RANGE;

static void Buzzer_WriteHardware (Buzzer_t *buzzer, uint8_t state){ // to turn on or turn off the pwm
    if (buzzer == NULL || buzzer->htim == NULL) return;

    buzzer->is_active = state;
    if (state) {
        __HAL_TIM_SET_COMPARE(buzzer->htim, buzzer->channel, 500);
        HAL_TIM_PWM_Start (buzzer->htim, buzzer->channel);
    } else {
        __HAL_TIM_SET_COMPARE(buzzer->htim, buzzer->channel, 0);
        HAL_TIM_PWM_Stop (buzzer->htim, buzzer->channel);
    }
}

void Buzzer_Init(Buzzer_t *buzzer, TIM_HandleTypeDef *htim, uint32_t channel) {
    if (buzzer == NULL || htim == NULL) return;

    buzzer->htim = htim;
    buzzer->channel = channel;
    buzzer->last_toggle_time = 0;

    Buzzer_WriteHardware (buzzer, 0);
}

void Buzzer_Update(Buzzer_t *buzzer, SystemState_t state) {
    if (buzzer == NULL) return;
        uint32_t current_time = HAL_GetTick();
    if (current_state != state) {
        current_state = state;
        buzzer->last_toggle_time = current_time;
    }

    switch (state) {
        case STATE_OUT_OF_RANGE:
            Buzzer_WriteHardware (buzzer, 0);
            break; // do nothing
        case STATE_SAFE: 
            Buzzer_WriteHardware (buzzer, 0);
            break;
        case STATE_WARNING:
            if (current_time - buzzer->last_toggle_time >= 500) {
                Buzzer_WriteHardware (buzzer, !buzzer->is_active);
                buzzer->last_toggle_time = current_time;
            }
            break;
        case STATE_DANGER:
            if (current_time - buzzer->last_toggle_time >= 100){
                Buzzer_WriteHardware (buzzer, !buzzer->is_active);
                buzzer->last_toggle_time = current_time;
            }
            break;
    }
}