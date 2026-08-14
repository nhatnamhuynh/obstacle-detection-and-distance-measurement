#include "buzzer.h"

static void Buzzer_WriteHardware (Buzzer_t *buzzer, uint8_t state){ // to turn on or turn off the pwm
    if (buzzer == NULL || buzzer->htim == NULL) return;

    buzzer->is_active = state;
    if (state) {
        HAL_TIM_PWM_Start (buzzer->htim, buzzer->channel);
    } else HAL_TIM_PWM_Stop (buzzer->htim, buzzer->channel);
}

void Buzzer_Init(Buzzer_t *buzzer, TIM_HandleTypeDef *htim, uint32_t channel) {
    if (buzzer == NULL || htim == NULL) return;

    buzzer->htim = htim;
    buzzer->channel = channel;
    buzzer->mode = BUZZER_MODE_OFF;
    buzzer->last_toggle_time = 0;

    Buzzer_WriteHardware (buzzer, 0);
}

void Buzzer_SetMode(Buzzer_t *buzzer, BuzzerMode_t mode) {
    if (buzzer == NULL || buzzer->mode == mode) return; // if the change the mode then return;

    buzzer->mode = mode;
    buzzer->last_toggle_time = HAL_GetTick();

    if (mode == BUZZER_MODE_OFF) Buzzer_WriteHardware(buzzer, 0); 
    else Buzzer_WriteHardware (buzzer, 1); // turn on pwm from the SetMode step
}

void Buzzer_Update(Buzzer_t *buzzer) {
    // TODO: Set PWM Duty Cycle to 0 or call HAL_TIM_PWM_Stop()
    if (buzzer == NULL) return;

    uint32_t current_time = HAL_GetTick();

    switch (buzzer->mode) {
        case BUZZER_MODE_OFF: break; // do nothing
        case BUZZER_MODE_BEEP_SINGLE: 
            if (current_time - buzzer->last_toggle_time >= 100) {
                Buzzer_WriteHardware (buzzer, 0);
                buzzer->mode = BUZZER_MODE_OFF;
            }
            break;
        case BUZZER_MODE_WARNING_SLOW:
            if (current_time - buzzer->last_toggle_time >= 500) {
                Buzzer_WriteHardware (buzzer, !buzzer->is_active);
                buzzer->last_toggle_time = current_time;
            }
            break;
        case BUZZER_MODE_WARNING_FAST:
            if (current_time - buzzer->last_toggle_time >= 100){
                Buzzer_WriteHardware (buzzer, !buzzer->is_active);
                buzzer->last_toggle_time = current_time;
            }
            break;
    }
}