#ifndef __LED_H
#define __LED_H

#include "main.h"

void LED_Init(void);
void LED_Update(SystemState_t state);
void LED_AllOff(void);

#endif /* __LED_H */