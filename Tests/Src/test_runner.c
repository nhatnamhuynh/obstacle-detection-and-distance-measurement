#include "test_runner.h"
#include "main.h"
#include "test_led.h"
#include "test_buzzer.h"
#include "test_lcd.h"
#include "test_uart.h"
#if ENABLE_HARDWARE_TESTBENCH

extern void Input_RunHardwareTestbench(void);
extern void Display_RunHardwareTestbench(void);
extern void Actuators_RunHardwareTestbench(void);

void TestRunner_RunAll(void) {
    printf("\r\n==============================================\r\n");
    printf("     SYSTEM HARDWARE TESTBENCH STARTED        \r\n");
    printf("==============================================\r\n");

    Input_RunHardwareTestbench();
    Display_RunHardwareTestbench();
    Actuators_RunHardwareTestbench();
    Test_UART_Terminal(&huart1);
    Test_3_LED_Blink();
    Test_Buzzer_Beep();
    Test_LCD_I2C();
    printf("\r\n==============================================\r\n");
    printf("     ALL TESTCASES PASSED! STARTING APP...    \r\n");
    printf("==============================================\r\n\n");
}

#endif