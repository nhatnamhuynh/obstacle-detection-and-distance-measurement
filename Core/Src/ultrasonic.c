#include "ultrasonic.h"
#include "main.h"

static uint32_t echo_time_us = 0; // time for the echo pulse width
static uint32_t is_first_captured = 0; // 0 = waiting for rising edge, 1 = waiting for falling edge
static uint32_t posedge_time = 0; // time of the rising edge
static uint32_t negedge_time = 0; // time of the falling edge
static float curr_dist = 0.0f; // current distance (cm)


static void delay_us(uint32_t us) {                         // approximate delay
    uint32_t count = (SystemCoreClock / 1000000) * us / 5; // devided by 5 because in while loop takes 5 cycles per iteration
    while (count--) {
        __NOP();
    }
}

void Ultrasonic_Init(void) {
    // TODO: Call HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1)
    echo_time_us = 0;       // reset all variables at init
    is_first_captured = 0;
    posedge_time = 0;
    negedge_time = 0;
    curr_dist = 0.0f;

    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET); // Set TRIG_PIN LOW as initial state 
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1); // init
}

void Ultrasonic_Trigger(void) {
    // TODO: Set TRIG_PIN HIGH -> delay 10us -> Set TRIG_PIN LOW
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

float Ultrasonic_ReadDistance(void) {
    // TODO: Return distance (cm) = (echo_time_us * 0.0343f) / 2.0f
    return curr_dist;
    // from Ultrasonic_CaptureCallback
}

void Ultrasonic_CaptureCallback(TIM_HandleTypeDef *htim) {
    // TODO: Measure pulse width on TIM2 Channel 1 (Rising to Falling edge)
    if (htim->Instance == TIM2) {
        if (is_first_captured == 0) {
            posedge_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
            is_first_captured = 1; // next capture will be falling edge

            __HAL_TIM_SET_CAPTUREPOLARITY (htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING); // wait for falling edge

        } else {
            negedge_time = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

            if (negedge_time >= posedge_time) {
                echo_time_us = negedge_time - posedge_time;
            } else {
                echo_time_us = (0xFFFF - posedge_time) + negedge_time;
            }
            curr_dist = (echo_time_us * 0.0343f) / 2.0f;

            is_first_captured = 0; // next capture will be rising edge
            __HAL_TIM_SET_CAPTUREPOLARITY (htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); // wait for rising edge
        }
    }
}