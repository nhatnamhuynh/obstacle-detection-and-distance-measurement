#include "uart_log.h"
#include "lcd_i2c.h"
#include "moving_filter.h"
#include "ultrasonic.h"
#include "buzzer.h"
#include "led.h"
#include "fsm_alarm.h"
#include "button_exti.h"
#include "main.h"

extern MovingFilter_t g_filter;
extern SensorData_t g_data;

void Input_RunHardwareTestbench(void) {
    printf("\r\n========================================\r\n");
    printf("--- START SENSOR & FILTER & FSM TEST ---\r\n");
    printf("========================================\r\n");
    HAL_Delay(1000); 

    MovingFilter_Init(&g_filter);
    
    g_data.unit = UNIT_CM; 

    int sample_count = 0;
    const int MAX_SAMPLES = 20; 

    while (sample_count < MAX_SAMPLES) {
        if (Ultrasonic_Trigger()) {

            HAL_Delay(20); 

            float raw_cm = Ultrasonic_ReadDistance();
            
            float filtered_cm = MovingFilter_Update(&g_filter, raw_cm);

            FSM_Update(filtered_cm, &g_data);

            char* state_str = "UNKNOWN";
            switch(g_data.state) {
                case STATE_OUT_OF_RANGE: state_str = "OUT_OF_RANGE"; break;
                case STATE_SAFE:         state_str = "SAFE        "; break;
                case STATE_WARNING:      state_str = "WARNING     "; break;
                case STATE_DANGER:       state_str = "DANGER      "; break;
            }

            printf("Sample [%2d/%d] | Raw: %7.2f cm | Filtered: %7.2f cm | State: %s\r\n",
                   sample_count + 1, MAX_SAMPLES, raw_cm, filtered_cm, state_str);

            sample_count++;
        }
    }

    printf("========================================\r\n");
    printf("---         INPUT TEST DONE!         ---\r\n");
    printf("========================================\r\n");
    HAL_Delay(2000);
}