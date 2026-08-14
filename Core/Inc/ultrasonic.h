#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "main.h"

void Ultrasonic_Init(void);
void Ultrasonic_Trigger(void);
float Ultrasonic_ReadDistance(void);
// the interrupt function is included in the HAL library so we dont need to declare it anymore

#endif /* __ULTRASONIC_H */