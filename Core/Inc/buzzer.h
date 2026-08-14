#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"

typedef enum {
    BUZZER_MODE_OFF = 0, // turn off the buzzer
    BUZZER_MODE_BEEP_SINGLE, // 1 beep then off
    BUZZER_MODE_WARNING_FAST, // repeatedly single beep (100ms)
    BUZZER_MODE_WARNING_SLOW // repeatedly single beep but slower (500ms)
} BuzzerMode_t;

typedef struct {
    TIM_HandleTypeDef *htim; //pointer to Timer which controls PWM
    uint32_t channel; // channel of buzzer (we are currently set it to TIM4_CH3)

    BuzzerMode_t mode; // current mode
    uint32_t last_toggle_time;
    uint8_t is_active; // current state of PWM (1 = working, 0 = stop)
} Buzzer_t;

void Buzzer_Init(Buzzer_t *buzzer, TIM_HandleTypeDef *htim, uint32_t channel);
void Buzzer_SetMode(Buzzer_t *buzzer, BuzzerMode_t mode);
void Buzzer_Update(Buzzer_t *buzzer);

#endif /* __BUZZER_H */