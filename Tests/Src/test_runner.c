#include "main.h"
#include "test_runner.h"
#include "test_led.h"
#include "test_buzzer.h"
#include "test_lcd.h"
#include "test_uart.h"
#if ENABLE_HARDWARE_TESTBENCH

extern UART_HandleTypeDef huart1; 

extern void Input_RunHardwareTestbench(void);
extern void Display_RunHardwareTestbench(void);
extern void Actuators_RunHardwareTestbench(void);

void TestRunner_RunAll(void) {
    char msg1[] = "\r\n==============================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg1, strlen(msg1), HAL_MAX_DELAY);

    char msg2[] = "     SYSTEM HARDWARE TESTBENCH STARTED        \r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg2, strlen(msg2), HAL_MAX_DELAY);

    char msg3[] = "==============================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg3, strlen(msg3), HAL_MAX_DELAY);

    Test_Buzzer_Beep();
    HAL_Delay(1000);

    Test_LCD_I2C();
    HAL_Delay(1000);
    Test_3_LED_Blink();
    HAL_Delay(1000);

    Test_UART_Terminal(&huart1);
    HAL_Delay(1000);

    Input_RunHardwareTestbench();
    HAL_Delay(1000);

    Display_RunHardwareTestbench();
    HAL_Delay(1000);

    Actuators_RunHardwareTestbench();
    HAL_Delay(1000);

    char msg4[] = "\r\n==============================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg4, strlen(msg4), HAL_MAX_DELAY);

    char msg5[] = "     ALL TESTCASES PASSED! STARTING APP...    \r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg5, strlen(msg5), HAL_MAX_DELAY);

    char msg6[] = "==============================================\r\n\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg6, strlen(msg6), HAL_MAX_DELAY);
}

#endif