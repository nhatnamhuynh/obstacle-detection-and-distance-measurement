#include "fsm_alarm.h"

void FSM_Init(SensorData_t *sensor_data) {
    // TODO: Set sensor_data->state = STATE_SAFE
}

void FSM_Update(SensorData_t *sensor_data) {
    // TODO: Check filtered_distance to update sensor_data->state
    // >30cm: SAFE | 10-30cm: WARNING | <10cm: DANGER | Invalid: OUT_OF_RANGE
}