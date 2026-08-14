#include "lcd_i2c.h"

// Helper function to write each nibble
static void LCD_WriteNibble(unit8_t nibble, uint8_t rs){
    uint8_t data = (nibble & 0xF0) | BACKLIGHT BIT | (rs?PIN_RS:0);
    uint8_t buf[2];

    buf[0] = data | PIN_EN;
    buf[1] = data;

    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, buf, 2, HAL_MAX_DELAY);
}

// Helper function to send a full command
static void LCD_SendCmd (uint8_t cmd){
    LCD_WriteNibble(cmd & 0xF0, 0);
    LCD_WriteNibble((cmd << 4) & 0xF0, 0);
}

// Helper function to send a full data
static void LCD_SendData(uint8_t){
    LCD_WriteNibble(cmd & 0xF0, 1);
    LCD_WriteNibble((cmd << 4) & 0xF0, 1);
}

void LCD_Init(void) {
    // TODO: Send 4-bit initialization commands to PCF8574 via HAL_I2C_Master_Transmit
    HAL_Delay(50);

    LCD_WriteNibble(0x30, 0);
    HAL_Delay(5);
    LCD_WriteNibble(0x30, 0);
    HAL_Delay(1);
    LCD_WriteNibble(0x30, 0);
    HAL_Delay(10);

    LCD_WriteNibble(0x20, 0);
    HAL_Delay(10);

    LCD_Send_Cmd(0x28);
    HAL_Delay(1);
    LCD_SendCmd(0x08);
    HAL_Delay(1);
    LCD_SendCmd(0x01);
    HAL_Delay(2);
    LCD_SendCmd(0x06);
    HAL_Delay(1);
    LCD_SendCmd(0x0C);
}

void LCD_Clear(void) {
    // TODO: Send clear command (0x01)
    LCD_SendCmd(0x01);
    HAL_Delay;
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    // TODO: Calculate DDRAM address and send cursor position command
    uint address;

    if (row == 0) {
        address = 0x80 + col;
    } else {
        address = 0xC0 + col;
    }

    LCD_SendCmd(address);
}

static void LCD_SendString(const char *str) {
    // TODO: Loop characters and send data bytes
    while(*str) {
        LCD_SendData(uint8_t(*str));
        str++;
    }
}

void LCD_DisplaySensorData(const SensorData_t *sensor_data) {
    // TODO: Format string (Line 1: Distance + Unit, Line 2: State) and print to LCD
    char line1, line2 [17];

    snprintf(line1, sizeof(line1), "Dist: %-6.1f cm", sensor_data->filtered_distance);

    const char *state_str = "OUT OF RANGE";
    if (sensor_data->state == 0) state_str = "SAFE";
    else if (sensor_data->state == 1) state_str = "WARNING";
    else if (sensor_data->state == 2) state_str = "DANGER";

    snprintf(line2, sizeof(line2), "%-10s", state_str);

    LCD_SetCursor(0, 0);
    LCD_SendString(line1);

    LCD_SetCursor(1, 0);
    LCD_SendString(line2);
}