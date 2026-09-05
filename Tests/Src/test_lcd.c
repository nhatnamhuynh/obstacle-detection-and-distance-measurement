#include "test_lcd.h"
#include "lcd_i2c.h"
#include "main.h"
#include <string.h>

extern UART_HandleTypeDef huart1;

void Test_LCD_I2C(void) {
    char msg_start[] = "[TEST] LCD I2C Test Started...\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg_start, strlen(msg_start), HAL_MAX_DELAY);

    LCD_Init(); 
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_SendString("Hello");
    
    HAL_Delay(2000);
    LCD_Clear();

    char msg_end[] = "[TEST] LCD I2C Test PASSED!\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg_end, strlen(msg_end), HAL_MAX_DELAY);
}