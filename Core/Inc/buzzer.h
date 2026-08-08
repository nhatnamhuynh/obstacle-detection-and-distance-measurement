#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"

void Buzzer_Init(void);
void Buzzer_Update(SystemState_t state, float distance);
void Buzzer_Stop(void);

#endif /* __BUZZER_H */