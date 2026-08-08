#ifndef __FSM_ALARM_H
#define __FSM_ALARM_H

#include "main.h"

#define DIST_SAFE_MIN_CM  30.0f
#define DIST_WARN_MIN_CM  10.0f

void FSM_Init(SensorData_t *sensor_data);
void FSM_Update(SensorData_t *sensor_data);

#endif /* __FSM_ALARM_H */