#include "button_exti.h"

void Button_Init(Button_t *btn, GPIO_TypeDef *port, uint16_t pin) {
    if (btn == NULL) return;

    btn->pin = pin;
    btn->port = port;
    btn->flag = 0;
    btn->last_press = 0;
}
uint8_t Button_WasPressed (Button_t *btn) {
    if (btn == NULL) return;

    if (btn->flag) {
        btn->flag = 0;
        return 1;
    }
    return 0;
}

void Button_EXTI_Callback(Button_t *btn, uint16_t GPIO_pin) {
    if (btn == NULL) return;

    if  (GPIO_pin == btn->pin) {
        uint32_t current_time = HAL_GetTick();
        if (current_time - btn->last_press >= DEBOUNCE_DELAY_MS) {
            btn->last_press = current_time;
            btn->flag = 1;
        }
    }
}

void HandleUnitButton (Button_t *btn, SensorData_t *data) {
  if (Button_WasPressed (&btn)) {
      if (data->unit == UNIT_CM) {
        data->unit = UNIT_INCH;
      }
      else if (data->unit == UNIT_INCH) {
        data->unit = UNIT_CM;
      }
    } 
}