#ifndef __MOVING_FILTER_H
#define __MOVING_FILTER_H

#include "main.h"

#define FILTER_WINDOW_SIZE 5

typedef struct {
    float buffer[FILTER_WINDOW_SIZE];
    uint8_t index;
    uint8_t count;
    float sum;
} MovingFilter_t;

void MovingFilter_Init(MovingFilter_t *filter);
float MovingFilter_Update(MovingFilter_t *filter, float input_value);

#endif /* __MOVING_FILTER_H */