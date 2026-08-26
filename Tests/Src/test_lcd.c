#include "test_lcd.h"
#include "lcd_i2c.h" // Nhớ đảm bảo có file thư viện LCD của Nam trong project

void Test_LCD_I2C(void) {
    LCD_Init(); 
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_PrintString("Hello");
    
    HAL_Delay(2000); // Giữ chữ 2 giây để quan sát
    LCD_Clear();
}