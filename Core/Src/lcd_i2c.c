#include "lcd_i2c.h"

// Helper function prototypes
static void LCD_WriteNibble(uint8_t nibble, uint8_t rs);
static void LCD_SendCmd(uint8_t cmd);
static void LCD_SendData(uint8_t data);

// Helper function to write each nibble
static void LCD_WriteNibble(uint8_t nibble, uint8_t rs){
    uint8_t data = (nibble & 0xF0) | BACKLIGHT_BIT | (rs?PIN_RS:0);
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
static void LCD_SendData(uint8_t data){
    LCD_WriteNibble(data & 0xF0, 1);
    LCD_WriteNibble((data << 4) & 0xF0, 1);
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

    LCD_SendCmd(0x28);
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
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    // TODO: Calculate DDRAM address and send cursor position command
    uint8_t address;

    if (row == 0) {
        address = 0x80 + col;
    } else {
        address = 0xC0 + col;
    }

    LCD_SendCmd(address);
}

void LCD_SendString(const char *str) {
    // TODO: Loop characters and send data bytes
    while(*str) {
        LCD_SendData((uint8_t)*str);
        str++;
    }
}

void LCD_DisplaySensorData(const SensorData_t *sensor_data) {
    // TODO: Format string (Line 1: Distance + Unit, Line 2: State) and print to LCD
    static uint32_t last_lcd = 0;
    uint32_t current_time = HAL_GetTick();
    if (current_time - last_lcd >= LCD_INTERVAL_MS){
        char line1[17];
        char line2[17];
        const char* unit_str = "cm";
        const char* state_str = "UNKNOWN";

        if (sensor_data->unit == 0) unit_str = "cm";
        else unit_str = "in";

        switch (sensor_data->state){
            case STATE_SAFE: 
                state_str = "SAFE";
                break;
            case STATE_WARNING: 
                state_str = "WARNING";
                break;
            case STATE_DANGER: 
                state_str = "DANGER";
                break;
            case STATE_OUT_OF_RANGE: 
                state_str = "OUT OF RANGE";
                break;
            default: 
                state_str = "UNKNOWN";
                break;
        }

        if(sensor_data->state == STATE_OUT_OF_RANGE) {
            snprintf(line1, sizeof(line1), "Dist:  --- %-4s", unit_str);
        } else {
            snprintf(line1, sizeof(line1), "Dist: %-6.1f %-3s", sensor_data->filtered_distance, unit_str);
        }
        snprintf(line2, sizeof(line2), "%-12s", state_str);

        LCD_SetCursor(0, 0);
        LCD_SendString(line1);

        LCD_SetCursor(1, 0);
        LCD_SendString(line2);

        last_lcd = current_time;
    }
}