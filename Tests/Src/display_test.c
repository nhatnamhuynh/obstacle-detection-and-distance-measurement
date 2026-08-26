#include "uart_log.h"
#include "lcd_i2c.h"
#include "moving_filter.h"
#include "ultrasonic.h"
#include "buzzer.h"
#include "led.h"
#include "fsm_alarm.h"
#include "button_exti.h"
#include "main.h"

#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1; // Thay huart1 nếu bạn dùng bộ UART khác
extern SensorData_t g_data;
extern Led_t g_led;
extern Buzzer_t g_buzzer;

void Display_RunHardwareTestbench(void) {
    char msg1[] = "\r\n========================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg1, strlen(msg1), HAL_MAX_DELAY);

    char msg2[] = "--- START DISPLAY & SYSTEM FSM TEST ----\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg2, strlen(msg2), HAL_MAX_DELAY);

    char msg3[] = "========================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg3, strlen(msg3), HAL_MAX_DELAY);

    LCD_SendString("START TESTBENCH");
    HAL_Delay(2000);
    LCD_Clear();

    // ========================================================
    // TESTCASE 1: SAFE - 60cm
    // ========================================================
    char log1[] = "[DISPLAY TEST] Case 1: SAFE (Distance: 60.0 cm)\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)log1, strlen(log1), HAL_MAX_DELAY);

    g_data.unit = UNIT_CM; 
    FSM_Update(60.0f, &g_data); 
    
    LED_Update(&g_led, g_data.state); 
    Buzzer_Update(&g_buzzer, g_data.state);
    LCD_DisplaySensorData(&g_data); 
    UART_Log_Process(&g_data); 
    
    HAL_Delay(2000);

    // ========================================================
    // TESTCASE 2: WARNING - 35cm
    // ========================================================
    char log2[] = "[DISPLAY TEST] Case 2: WARNING (Distance: 35.0 cm)\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)log2, strlen(log2), HAL_MAX_DELAY);

    FSM_Update(35.0f, &g_data);
    
    LED_Update(&g_led, g_data.state);
    Buzzer_Update(&g_buzzer, g_data.state);
    LCD_DisplaySensorData(&g_data);
    UART_Log_Process(&g_data);
    
    HAL_Delay(2000);

    // ========================================================
    // TESTCASE 3: DANGER - 10cm
    // ========================================================
    char log3[] = "[DISPLAY TEST] Case 3: DANGER (Distance: 10.0 cm)\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)log3, strlen(log3), HAL_MAX_DELAY);

    FSM_Update(10.0f, &g_data);
    
    LED_Update(&g_led, g_data.state);
    Buzzer_Update(&g_buzzer, g_data.state);
    LCD_DisplaySensorData(&g_data);
    UART_Log_Process(&g_data);
    
    HAL_Delay(2000);

    // ========================================================
    // KẾT THÚC TESTBENCH
    // ========================================================
    LCD_SendString("--- TEST DONE! ---");

    char msg4[] = "========================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg4, strlen(msg4), HAL_MAX_DELAY);

    char msg5[] = "---        DISPLAY TEST DONE!        ---\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg5, strlen(msg5), HAL_MAX_DELAY);

    char msg6[] = "========================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg6, strlen(msg6), HAL_MAX_DELAY);

    HAL_Delay(2000);
}