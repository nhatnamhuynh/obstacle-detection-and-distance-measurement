#ifndef MEDIAN_FILTER_H
#define MEDIAN_FILTER_H

#include <stdint.h>

#define MEDIAN_WINDOW_SIZE 5

void MedianFilter_Init(void);
float MedianFilter_Update(float input_val);

#endif /* MEDIAN_FILTER_H */