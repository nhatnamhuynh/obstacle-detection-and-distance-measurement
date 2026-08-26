#include "fsm_alarm.h"
#include "button_exti.h"
#include "main.h"

static Led_t *pled = NULL;
static Buzzer_t *pbuzzer = NULL;
static SystemState_t current_state = STATE_OUT_OF_RANGE;

void FSM_Init(Led_t *led, Buzzer_t *buzzer) {
    pled = led;
    pbuzzer = buzzer;
    current_state = STATE_OUT_OF_RANGE;
}

void FSM_Update(float filtered_distance, SensorData_t *data) {
    SystemState_t new_state;

    if (filtered_distance < 0.0f || filtered_distance > 400.0f) {
        new_state = STATE_OUT_OF_RANGE;
    } else if (filtered_distance >= 50.0f) new_state = STATE_SAFE;
    else if (filtered_distance >= 20.0f) new_state = STATE_WARNING;
    else if (filtered_distance < 20.0f) new_state = STATE_DANGER;

    if (current_state != new_state) {
        current_state = new_state;
        data->state = current_state;
    }

    if (data->unit == UNIT_INCH) {
        data->filtered_distance = filtered_distance / 2.54f;
    } else data->filtered_distance = filtered_distance;
}

