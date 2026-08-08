#ifndef __LCD_I2C_H
#define __LCD_I2C_H

#include "main.h"

#define LCD_I2C_ADDR (0x27 << 1)

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_SendString(const char *str);
void LCD_DisplaySensorData(const SensorData_t *sensor_data);

#endif /* __LCD_I2C_H */