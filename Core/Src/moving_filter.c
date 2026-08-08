#include "moving_filter.h"

void MovingFilter_Init(MovingFilter_t *filter) {
    // TODO: Clear buffer array to 0, reset index, count, sum to 0
}

float MovingFilter_Update(MovingFilter_t *filter, float input_value) {
    // TODO: Push new value into ring buffer, update sum, return average (sum / count)
    return input_value;
}