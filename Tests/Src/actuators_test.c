#include "uart_log.h"
#include "lcd_i2c.h"
#include "moving_filter.h"
#include "ultrasonic.h"
#include "buzzer.h"
#include "led.h"
#include "fsm_alarm.h"
#include "button_exti.h"
#include "main.h"

#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1; 
extern Led_t g_led;
extern Buzzer_t g_buzzer;

static void TestActuators_RunStateForDuration(SystemState_t state, const char *state_name, uint32_t duration_ms) {
    char tx_buf[128];
    int len = snprintf(tx_buf, sizeof(tx_buf),
                       "[ACTUATOR TEST] Testing State: %s for %lu ms...\r\n", 
                       state_name, (unsigned long)duration_ms);
    HAL_UART_Transmit(&huart1, (uint8_t*)tx_buf, len, HAL_MAX_DELAY);
    
    uint32_t start_time = HAL_GetTick();
    while (HAL_GetTick() - start_time < duration_ms) {
        LED_Update(&g_led, state);
        Buzzer_Update(&g_buzzer, state);
        HAL_Delay(10); 
    }
}

void Actuators_RunHardwareTestbench(void) {
    char msg1[] = "\r\n========================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg1, strlen(msg1), HAL_MAX_DELAY);

    char msg2[] = "--- START ACTUATOR (LED & BUZZER) TEST ---\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg2, strlen(msg2), HAL_MAX_DELAY);

    char msg3[] = "========================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg3, strlen(msg3), HAL_MAX_DELAY);

    HAL_Delay(1000);

    // TESTCASE 1: OUT OF RANGE (All OFF)
    TestActuators_RunStateForDuration(STATE_OUT_OF_RANGE, "STATE_OUT_OF_RANGE", 3000);

    // TESTCASE 2: SAFE (Green LED ON, Buzzer OFF)
    TestActuators_RunStateForDuration(STATE_SAFE, "STATE_SAFE", 3000);

    // TESTCASE 3: WARNING (Yellow LED & Buzzer blink/beep at 500ms interval)
    TestActuators_RunStateForDuration(STATE_WARNING, "STATE_WARNING", 5000);

    // TESTCASE 4: DANGER (Red LED & Buzzer blink/beep fast at 100ms interval)
    TestActuators_RunStateForDuration(STATE_DANGER, "STATE_DANGER", 5000);

    // Cleanup - return to OUT_OF_RANGE
    LED_Update(&g_led, STATE_OUT_OF_RANGE);
    Buzzer_Update(&g_buzzer, STATE_OUT_OF_RANGE);

    char msg4[] = "========================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg4, strlen(msg4), HAL_MAX_DELAY);

    char msg5[] = "--- ACTUATOR TEST DONE! ENTERING MAIN ---\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg5, strlen(msg5), HAL_MAX_DELAY);

    char msg6[] = "========================================\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg6, strlen(msg6), HAL_MAX_DELAY);

    HAL_Delay(2000);
}