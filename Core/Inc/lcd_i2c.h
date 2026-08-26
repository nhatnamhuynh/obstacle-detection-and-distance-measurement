#ifndef __LCD_I2C_H
#define __LCD_I2C_H

#include "main.h"

#define LCD_I2C_ADDR (0x27 << 1)

#define PIN_RS (1 << 0)
#define PIN_RW (1 << 1)
#define PIN_EN (1 << 2)
#define BACKLIGHT_BIT (1 << 3)

#define LCD_INTERVAL_MS 200

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_SendString(const char *str);
void LCD_DisplaySensorData(const SensorData_t *sensor_data);

#endif /* __LCD_I2C_H */