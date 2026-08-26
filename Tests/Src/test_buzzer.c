#include "test_buzzer.h"

// Lấy biến điều khiển Timer 4 từ main.c
extern TIM_HandleTypeDef htim4; 

void Test_Buzzer_Beep(void) {
    // Kích hoạt băm xung PWM để còi kêu
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    
    // Đặt độ rộng xung (Duty Cycle) khoảng 50% để nghe rõ nhất
    // Theo cấu hình, AutoReload Register (ARR) là 369, nên một nửa là khoảng 184
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 184);
    
    HAL_Delay(1000); // Kêu trong 1 giây
    
    // Tắt PWM để còi ngừng kêu
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
}