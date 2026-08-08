#include "lcd_i2c.h"

void LCD_Init(void) {
    // TODO: Send 4-bit initialization commands to PCF8574 via HAL_I2C_Master_Transmit
}

void LCD_Clear(void) {
    // TODO: Send clear command (0x01)
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    // TODO: Calculate DDRAM address and send cursor position command
}

void LCD_SendString(const char *str) {
    // TODO: Loop characters and send data bytes
}

void LCD_DisplaySensorData(const SensorData_t *sensor_data) {
    // TODO: Format string (Line 1: Distance + Unit, Line 2: State) and print to LCD
}