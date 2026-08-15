#include "ultrasonic.h"
#include "main.h"

static volatile uint32_t echo_time_us = 0; // time for the echo pulse width
static volatile uint32_t is_first_captured = 0; // 0 = waiting for rising edge, 1 = waiting for falling edge
static volatile uint32_t posedge_time = 0; // time of the rising edge
static volatile uint32_t negedge_time = 0; // time of the falling edge


static void delay_us(uint32_t us) {                         // approximate delay
    uint32_t count = ((SystemCoreClock / 1000000) * us) / 5; // devided by 5 because in while loop takes 5 cycles per iteration
    while (count--) {
        __NOP();
    }
}

void Ultrasonic_Init(void) {
    echo_time_us = 0;       // reset all variables at init
    is_first_captured = 0;
    posedge_time = 0;
    negedge_time = 0;

    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET); // Set TRIG_PIN LOW as initial state 
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); // init
}

void Ultrasonic_Trigger(void) {
    is_first_captured = 0;
    __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); // wait for rising edge

    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

float Ultrasonic_ReadDistance(void) {
    if (echo_time_us == 0) {
        return -1.0f; //out of range
    }

    float distance = ((float)echo_time_us * 0.0343f) / 2.0f; // s = v.t

    if (distance > 400.0f) return -1.0f; // out of range
    return distance;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
        if (is_first_captured == 0) {
            posedge_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            is_first_captured = 1; // next capture will be falling edge

            __HAL_TIM_SET_CAPTUREPOLARITY (htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING); // wait for falling edge

        } else {
            negedge_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

            echo_time_us = (uint16_t) (negedge_time - posedge_time);

            is_first_captured = 0; // next capture will be rising edge
            __HAL_TIM_SET_CAPTUREPOLARITY (htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); // wait for rising edge
        }
    }
}