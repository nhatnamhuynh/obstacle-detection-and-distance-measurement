#include "uart_log.h"
#include "lcd_i2c.h"
#include "moving_filter.h"
#include "ultrasonic.h"
#include "buzzer.h"
#include "led.h"
#include "fsm_alarm.h"
#include "button_exti.h"
#include "main.h"

extern SensorData_t g_data;
extern Led_t g_led;
extern Buzzer_t g_buzzer;

void Display_RunHardwareTestbench(void) {
    // Nếu bạn có một hàm UART riêng để in chuỗi log độc lập, hãy dùng nó ở đây
    LCD_SendString("--- START TESTBENCH ---");
    HAL_Delay(2000);

    // ========================================================
    // TESTCASE 1: Trạng thái SAFE, Đơn vị CM (Giả lập 60cm)
    // ========================================================
    g_data.unit = UNIT_CM;                  // Cố định đơn vị
    FSM_Update(60.0f, &g_data);             // FSM tự động tính toán state và gán vào g_data
    
    LED_Update(&g_led, g_data.state);       // Kích hoạt ngoại vi theo trạng thái
    Buzzer_Update(&g_buzzer, g_data.state);
    LCD_DisplaySensorData(&g_data);         // Xuất ra màn hình LCD
    UART_Log_Process(&g_data);              // Đẩy dữ liệu lên UART
    
    HAL_Delay(3000); // Dừng 3 giây để bạn kịp quan sát phần cứng

    // ========================================================
    // TESTCASE 2: Trạng thái WARNING, Đơn vị CM (Giả lập 35cm)
    // ========================================================
    g_data.unit = UNIT_CM;
    FSM_Update(35.0f, &g_data);
    
    LED_Update(&g_led, g_data.state);
    Buzzer_Update(&g_buzzer, g_data.state);
    LCD_DisplaySensorData(&g_data);
    UART_Log_Process(&g_data);
    
    HAL_Delay(3000);

    // ========================================================
    // TESTCASE 3: Giả lập Nút Nhấn -> Chuyển sang Đơn vị INCH
    // ========================================================
    // Bạn giả lập nút nhấn bằng cách thay đổi trực tiếp g_data.unit 
    // và cập nhật lại FSM với cùng một mức khoảng cách
    g_data.unit = UNIT_INCH;
    FSM_Update(35.0f, &g_data); 
    
    LED_Update(&g_led, g_data.state);
    Buzzer_Update(&g_buzzer, g_data.state);
    LCD_DisplaySensorData(&g_data);
    UART_Log_Process(&g_data);
    
    HAL_Delay(3000);

    // ========================================================
    // TESTCASE 4: Trạng thái DANGER, Đơn vị INCH (Giả lập 10cm)
    // ========================================================
    g_data.unit = UNIT_INCH;
    FSM_Update(10.0f, &g_data);
    
    LED_Update(&g_led, g_data.state);
    Buzzer_Update(&g_buzzer, g_data.state);
    LCD_DisplaySensorData(&g_data);
    UART_Log_Process(&g_data);
    
    HAL_Delay(3000);

    // ========================================================
    // KẾT THÚC TESTBENCH
    // ========================================================
    LCD_SendString("--- TEST DONE! ---");
    HAL_Delay(2000);
}