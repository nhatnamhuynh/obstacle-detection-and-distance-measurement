#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "main.h"

void Ultrasonic_Init(void);
void Ultrasonic_Trigger(void);
float Ultrasonic_ReadDistance(void);
void Ultrasonic_CaptureCallback(TIM_HandleTypeDef *htim);

#endif /* __ULTRASONIC_H */