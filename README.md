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