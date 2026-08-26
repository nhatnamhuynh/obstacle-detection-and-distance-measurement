#include "test_buzzer.h"
#include "main.h"
#include <string.h>

extern TIM_HandleTypeDef htim4; 
extern UART_HandleTypeDef huart1;

void Test_Buzzer_Beep(void) {
    char msg_start[] = "[TEST] Buzzer Test Started...\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg_start, strlen(msg_start), HAL_MAX_DELAY);

    // Kích hoạt băm xung PWM để còi kêu
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    
    // Đặt độ rộng xung (Duty Cycle) khoảng 50% để nghe rõ nhất (ARR = 369 -> Duty = 184)
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 184);
    
    HAL_Delay(2000); // Kêu trong 2 giây
    
    // Tắt PWM để còi ngừng kêu
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);

    char msg_end[] = "[TEST] Buzzer Test PASSED!\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg_end, strlen(msg_end), HAL_MAX_DELAY);
}