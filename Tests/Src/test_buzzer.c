#include "test_buzzer.h"
#include "main.h"
#include <string.h>

extern TIM_HandleTypeDef htim4; 
extern UART_HandleTypeDef huart1;

void Test_Buzzer_Beep(void) {
    char msg_start[] = "[TEST] Buzzer Test Started...\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg_start, strlen(msg_start), HAL_MAX_DELAY);

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);

    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 184);
    
    HAL_Delay(2000); 

    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);

    char msg_end[] = "[TEST] Buzzer Test PASSED!\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg_end, strlen(msg_end), HAL_MAX_DELAY);
}