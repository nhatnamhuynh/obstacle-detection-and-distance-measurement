#include "median_filter.h"

static float buffer[MEDIAN_WINDOW_SIZE];
static uint8_t index;
static uint8_t count;

void MedianFilter_Init(void) {
    for (uint8_t i = 0; i < MEDIAN_WINDOW_SIZE; i++) {
        buffer[i] = 0.0f;
    }
    index = 0;
    count = 0;
}

float MedianFilter_Update(float input_val) {
    // Circular buffer like moving average filter
    buffer[index] = input_val;
    index = (index + 1) % MEDIAN_WINDOW_SIZE;
    if (count < MEDIAN_WINDOW_SIZE) {
        count++;
    }

    // make a copy of buffer to sort
    float temp[MEDIAN_WINDOW_SIZE];
    for (uint8_t i = 0; i < count; i++) {
        temp[i] = buffer[i];
    }

    // Interstion sort
    for (uint8_t i = 1; i < count; i++) {
        float key = temp[i];
        int8_t j = i - 1;
        while (j >= 0 && temp[j] > key) {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = key;
    }

    // return the median value
    return temp[count / 2];
}