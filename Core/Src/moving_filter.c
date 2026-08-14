#include "moving_filter.h"

void MovingFilter_Init(MovingFilter_t *filter) {
    // TODO: Clear buffer array to 0, reset index, count, sum to 0
    if (filter == NULL) return;

    filter->count = 0;
    filter->index = 0;
    filter->sum = 0.0f;
    for (uint8_t i = 0; i < FILTER_WINDOW_SIZE; i++) {
        filter->buffer[i] = 0.0f;
    }
}

float MovingFilter_Update(MovingFilter_t *filter, float input_value) {
    // TODO: Push new value into ring buffer, update sum, return average (sum / count)
    if (filter == NULL) return 0.0f;

    if (input_value < 0.0f) {
        if (filter->count > 0) {
            return filter->sum / (float) filter->count; // return the current avg;
        }
        else return 0.0f;
    }

    if (filter->count == FILTER_WINDOW_SIZE) {
        filter->sum -= filter->buffer [filter->index];
    }else {
        filter->count++;
    }

    filter->buffer [filter->index] = input_value;
    filter->sum += filter->buffer [filter->index];

    filter->index = (filter->index + 1) % FILTER_WINDOW_SIZE;  // ring shifting the index
    return filter->sum / (float) filter->count;
}