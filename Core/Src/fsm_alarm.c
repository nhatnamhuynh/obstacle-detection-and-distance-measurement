#include "fsm_alarm.h"

static Led_t *pled = NULL;
static Buzzer_t *pbuzzer = NULL;
static SystemState_t current_state = STATE_OUT_OF_RANGE;

void FSM_Init(Led_t *led, Buzzer_t *buzzer) {
    pled = led;
    pbuzzer = buzzer;
    current_state = STATE_OUT_OF_RANGE;

    if (pled != NULL) LED_SetState (pled, LED_STATE_OFF);
    if (pbuzzer != NULL) Buzzer_SetMode (pbuzzer, BUZZER_MODE_OFF);
}

void FSM_Update(float filtered_distance) {
    SystemState_t new_state;

    if (filtered_distance < 0.0f || filtered_distance > 400.0f) {
        new_state = STATE_OUT_OF_RANGE;
    } else if (filtered_distance >= 50.0f) new_state = STATE_SAFE;
    else if (filtered_distance >= 20.0f) new_state = STATE_WARNING;
    else if (filtered_distance < 20.0f) new_state = STATE_DANGER;

    if (current_state != new_state) {
        current_state = new_state;

        switch (current_state) {
            case STATE_OUT_OF_RANGE:
                if (pled != NULL) LED_SetState (pled,LED_STATE_OFF);
                if (pbuzzer != NULL) Buzzer_SetMode (pbuzzer, BUZZER_MODE_OFF);
                break;
            case STATE_SAFE:
                if (pled != NULL) LED_SetState (pled, LED_STATE_SAFE);
                if (pbuzzer != NULL) Buzzer_SetMode (pbuzzer, BUZZER_MODE_OFF);
                break;
            case STATE_WARNING:
                if (pled != NULL) LED_SetState (pled, LED_STATE_WARNING);
                if (pbuzzer != NULL) Buzzer_SetMode (pbuzzer, BUZZER_MODE_WARNING_SLOW);
                break;
            case STATE_DANGER:
                if (pled != NULL) LED_SetState (pled, LED_STATE_DANGER);
                if (pbuzzer != NULL) Buzzer_SetMode (pbuzzer, BUZZER_MODE_WARNING_FAST);
                break;
            default: break;
        }
    }
}

SystemState_t FSM_GetState (void) {
    return current_state;
}