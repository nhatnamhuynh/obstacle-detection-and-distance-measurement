#ifndef __FSM_ALARM_H
#define __FSM_ALARM_H

#include "main.h"
#include "led.h"
#include "buzzer.h"
#include "ultrasonic.h"

#define DIST_SAFE_MIN_CM  30.0f
#define DIST_WARN_MIN_CM  10.0f

void FSM_Init(Led_t *led, Buzzer_t *buzzer);
void FSM_Update(float filtered_distance);
SystemState_t FSM_GetState (void);

#endif /* __FSM_ALARM_H */