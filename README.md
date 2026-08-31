## Project Overview
This project implements STM32 into an obstacle warning system for vehicles. It triggers the ultrasonic waves, detects obstacles and calculates how far the objects are using the reflected ECHO signal. Based on real-time distance, it warns the user through multiple channels, including LEDs, buzzer, and LCD display. The system also transmits data logs periodically to the server using the U-ART interface, reporting realistic operation data for testing and future developments.
## Learning Objectived
* Practice embedded C programming on STM32 with HAL library.
* Understanding and applying Timers, Input Capture, Interrupts, I2C, U-ART, PWM, GPIO into a practical project.
* Design a finite state machine for distance-based warning states.
* Implement a non-blocking main loop using time-driven events.
* Apply logical framework into utilizing the overall system and deliver cutting-edge test cases. 
* Enhance documentation and project presentation.
## Hardware Implementation
## 3. Hardware

### 3.1. Overview
Dưới đây là danh sách chi tiết các linh kiện phần cứng (BOM) được sử dụng để xây dựng hệ thống:

| Component Category | Part / Module Name | Description / Specifications |
| :--- | :--- | :--- |
| **Microcontroller Board** | STM32F103C8T6 (Blue Pill) | ARM Cortex-M3 core @ 72 MHz |
| **Programmer / Debugger** | ST-Link V2 | SWD programming & debugging interface |
| **Primary Sensor** | HC-SR04 Ultrasonic Module | 2 cm – 400 cm range, 40 kHz acoustic frequency |
| **Display Interface** | 16x2 Character LCD + PCF8574 | I2C communication interface module |
| **Acoustic Actuator** | Passive Piezoelectric Buzzer | Driven via Timer PWM |
| **Visual Indicators** | 3x LEDs (Green, Yellow, Red) | Driven via GPIO with 220Ω current-limiting resistors |
| **User Input** | 1x Tactile Push-Button | Unit conversion (cm / inch), 10kΩ pull-up resistor |
| **Serial Telemetry** | CP2102 USB-to-UART Module | Transmitting logs to Serial Terminal on PC |
| **Software Toolchain** | VS Code, STM32CubeMX, Arm GNU Toolchain, CMake, Ninja, STM32 Programmer CLI| Các công cụ phần mềm phục vụ lập trình và biên dịch |

*Lưu ý:* Hệ thống sử dụng mạch nguồn hạ áp độc lập để cấp điện áp 5V (cho cảm biến, màn hình LCD) và điện áp 3.3V ổn định cho vi điều khiển STM32 trong suốt quá trình vận hành, chống sụt áp khi ngoại vi kích hoạt.

### 3.2. Pinout Schematic
Các ngoại vi được kết nối với vi điều khiển STM32 thông qua cấu hình chân trên STM32CubeMX như sau:

| Ngoại vi (Peripheral) | Chân vi điều khiển | Chức năng (Function) |
| :--- | :--- | :--- |
| **HC-SR04 (Trig)** | `PA1` | GPIO Output |
| **HC-SR04 (Echo)** | `PA0` | Timer Input Capture |
| **Nút nhấn (Button)** | `PA2` | Ngắt ngoài EXTI (Đổi đơn vị đo)     |
| **LED Xanh (Safe)** | `PB3` | GPIO Output |
| **LED Vàng (Warning)** | `PB4` | GPIO Output |
| **LED Đỏ (Danger)** | `PB5` | GPIO Output |
| **Buzzer** | `PB8` | Timer PWM (Điều khiển tần suất bíp) |
| **LCD 16x2 (SCL)** | `PB6` | I2C SCL |
| **LCD 16x2 (SDA)** | `PB7` | I2C SDA |
| **UART (TX)** | `PA9` | UART TX - Truyền dữ liệu log lên Terminal|
| **UART (RX)** | `PA10` | UART RX - Nhận dữ liệu|

### 3.3. PCB Layout
* **Công cụ thiết kế:** Mạch PCB của dự án được thiết kế chuyên nghiệp bằng phần mềm **Altium Designer**.
* **Bố trí linh kiện (Layout):** Mạch được phân bổ hợp lý thành các khu vực gồm: Header cho vi điều khiển Blue Pill, chân cắm module UART, cảm biến HC-SR04, màn hình LCD, khu vực cho nút nhấn, còi buzzer và cụm 3 domino để xuất tín hiệu cho đèn LED cảnh báo.

![Front PCB Layout](Picture/[MLIoT%20Project]%20Front%20PCB.png)
![Back PCB Layout](Picture/[MLIoT%20Project]%20Back%20PCB.png)

## Software Implementation

### 1. Overview: Development Tools and Libraries
The software architecture is built on a bare-metal approach, utilizing hardware interrupts instead of software delays to optimize CPU waiting time. The development environment and toolchain consist of:
*   **IDE & Code Editor:** Visual Studio Code.
*   **Configuration & Initialization:** STM32CubeMX.
*   **Compiler & Build System:** Arm GNU Toolchain, CMake, and Ninja.
*   **Flashing & Debugging Tool:** STM32CubeProgrammer CLI (Automated via custom `build_and_flash` shell/batch scripts).
*   **Core Libraries:** Hardware Abstraction Layer (HAL) and Standard C Libraries.

### 2. System Block Diagram

![System Block Diagram](<Picture/[MLIoT Project] Main Code Flow.png>) 

### 3. Warning State Machine (FSM)
The system utilizes a Finite State Machine (FSM) to classify spatial data into discrete zones and trigger multi-channel alerts (LED and Buzzer).

*   **State 1 - Safe State ($S \ge 50$ cm):** 
    *   The Green LED is turned ON to indicate a safe zone. 
    *   The buzzer remains completely silent.
*   **State 2 - Warning State ($20$ cm $\le S < 50$ cm):** 
    *   The Yellow LED is turned ON and blinks slowly to indicate a mid-range obstacle. 
    *   The buzzer emits a slow beep.
*   **State 3 - Danger State ($S < 20$ cm):** 
    *   The Red LED is turned ON and blinks rapidly to warn of extreme proximity. 
    *   The buzzer emits a rapid, continuous beep to signal an emergency.
*   **State 4 - Out of Range ($S \ge 400$ cm or Echo Timeout):** 
    *   All 3 LEDs are turned OFF. 
    *   The buzzer remains completely silent.

#### State Diagram:
![State Diagram](<Picture/[MLIoT Project] State Diagram.png>)

### 4. Signal Acquisition & Processing Flow
The obstacle detection logic relies on hardware time-stamping and digital filtering to ensure high precision with an average error of less than 5%.

*   **Triggering:** The STM32 sends a signal to the HC-SR04 to emit an ultrasonic wave by pulling the TRIG pin HIGH for $10\mu s$, then pulling it LOW.
*   **Echo Capture:** The HC-SR04 emits the ultrasonic pulse and pulls the ECHO pin HIGH until the reflected wave is received, at which point it is pulled LOW.
*   **Time Measurement:** The Timer Input Capture interrupt records time $T_1$ at the rising edge of the ECHO pin and time $T_2$ at the falling edge. The pulse width is calculated as $\Delta t = T_2 - T_1$.
*   **Distance Calculation:** The distance is calculated using the formula $S = \frac{v \times \Delta t}{2}$ (where $v = 0.0343$ cm/$\mu s$, accounting for the round-trip travel of the sound wave).
*   **Digital Filtering:** A Moving Average Filter is applied using the 5 most recent samples ($N=5$) to smooth the data and eliminate sudden noise spikes.

### 5. Timing & Interrupt Design
To maintain real-time performance without an RTOS, the system relies heavily on peripheral timers and a strict interrupt priority hierarchy.

*   **Timer Input Capture:** Utilized specifically to measure the Echo pulse width duration for distance calculation.
*   **PWM Control:** A Timer PWM is used to control the passive buzzer, modulating its beep frequency inversely proportional to the measured distance.
*   **SysTick Time Base:** SysTick is implemented to create a time base for the non-blocking FSM and button debouncing.
*   **Display & Communication Cycles:** Telemetry data (distance and state) is packaged into a standard string format and transmitted via UART to a PC terminal with a cycle of 200 ms. The LCD updates simultaneously at this 200 ms interval.
*   **Software Debounce & EXTI:** 
    *   The unit conversion button uses an EXTI interrupt combined with a software debounce technique.
    *   When the EXTI is triggered, the system compares the current time with the last valid press timestamp.
    *   If the difference is $\ge 200$ ms, it is confirmed as a valid press and the timestamp is updated.
    *   If the difference is $< 200$ ms, it is treated as mechanical noise and the event is ignored.
<!-- *   **NVIC Priority Configuration:** The Nested Vectored Interrupt Controller (NVIC) is structured with Timer IC having the highest priority, followed by SysTick, then UART, and finally the EXTI Button with the lowest priority. -->
*   **Timeout & Exception Handling:** Any distance metric exceeding $400\text{ cm}$ automatically transitions the system into the `OUT_OF_RANGE` state. Furthermore, if a new $50\text{ ms}$ trigger cycle begins while the previous Echo measurement is still incomplete (`is_first_captured == 1`), the system detects a missing falling edge and automatically overrides `echo_time_us` to $24,000\ \mu\text{s}$ ($\approx 411.6\text{ cm}$). This forces an immediate `OUT_OF_RANGE` state, resetting the Input Capture state machine and preventing timer corruption.