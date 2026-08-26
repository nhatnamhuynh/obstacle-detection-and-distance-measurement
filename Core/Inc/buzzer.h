#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"

typedef struct {
    TIM_HandleTypeDef *htim; //pointer to Timer which controls PWM
    uint32_t channel; // channel of buzzer (we are currently set it to TIM4_CH3)

    uint32_t last_toggle_time;
    uint8_t is_active; // current state of PWM (1 = working, 0 = stop)
} Buzzer_t;

void Buzzer_Init(Buzzer_t *buzzer, TIM_HandleTypeDef *htim, uint32_t channel);

// void Buzzer_SetMode(Buzzer_t *buzzer, BuzzerMode_t mode);

void Buzzer_Update(Buzzer_t *buzzer, SystemState_t state);

#endif /* __BUZZER_H */