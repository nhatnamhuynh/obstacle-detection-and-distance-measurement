#include "uart_log.h"
#include "lcd_i2c.h"
#include "moving_filter.h"
#include "ultrasonic.h"
#include "buzzer.h"
#include "led.h"
#include "fsm_alarm.h"
#include "button_exti.h"
#include "main.h"

extern SensorData_t g_data;
extern Led_t g_led;
extern Buzzer_t g_buzzer;

void Display_RunHardwareTestbench(void) {
    LCD_SendString("--- START TESTBENCH ---");
    HAL_Delay(2000);

    // ========================================================
    // TESTCASE 1: SAFE - 60cm
    // ========================================================
    g_data.unit = UNIT_CM;                  // by default
    FSM_Update(60.0f, &g_data);             
    
    LED_Update(&g_led, g_data.state);       
    Buzzer_Update(&g_buzzer, g_data.state);
    LCD_DisplaySensorData(&g_data);         
    UART_Log_Process(&g_data);              
    
    HAL_Delay(10000);

    // ========================================================
    // TESTCASE 2: WARNING - 35cm
    // ========================================================
    FSM_Update(35.0f, &g_data);
    
    LED_Update(&g_led, g_data.state);
    Buzzer_Update(&g_buzzer, g_data.state);
    LCD_DisplaySensorData(&g_data);
    UART_Log_Process(&g_data);
    
    HAL_Delay(10000);

    // ========================================================
    // TESTCASE 3: ANGER - 10cm
    // ========================================================
    FSM_Update(10.0f, &g_data);
    
    LED_Update(&g_led, g_data.state);
    Buzzer_Update(&g_buzzer, g_data.state);
    LCD_DisplaySensorData(&g_data);
    UART_Log_Process(&g_data);
    
    HAL_Delay(10000);

    // ========================================================
    // KẾT THÚC TESTBENCH
    // ========================================================
    LCD_SendString("--- TEST DONE! ---");
    HAL_Delay(2000);
}