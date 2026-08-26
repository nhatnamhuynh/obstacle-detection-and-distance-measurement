# Obstacle Detection and Distance Measurement Using Ultrasonic Sensing and Timer Input Capture on STM32F103

> **Course Project:** MLIoT - Embedded C (2026)  
> **Group:** Group 3 (Nhóm 3)  
> **Target Microcontroller:** STM32F103C8T6 (ARM Cortex-M3)  

---

## 📌 Executive Summary & Project Context

### Problem Statement
In mobile robotics and vehicular control—specifically during automotive reversing operations—rear blind spots represent a primary cause of collisions. Traditional mirror-based observation systems fail to cover the entire spatial geometry behind a vehicle, particularly for low-lying obstacles or suddenly appearing hazards.

### Practical Need
This project develops a smart, real-time, high-precision embedded reversing warning system. Beyond raw data acquisition, the system processes spatial metrics and dispatches multi-channel feedback (visual LED indicators, acoustic PWM signals, LCD display metrics, and serial UART telemetry) to enable immediate driver reaction.

### Academic & Technical Significance
- **Commercial Hardware Emulation:** Simulates the operational principles of commercial automotive park-assist sensors.
- **Hardware Time-Stamping:** Utilizes hardware-based Timer Input Capture instead of software delay loops, maximizing distance calculation precision while eliminating CPU waiting cycles.
- **Bare-metal Mastery:** Demonstrates low-level programming on the STM32F103 micro-architecture, orchestrating synchronized core peripherals including TIM, PWM, I2C, UART, and EXTI interrupt handlers.

---

## 🎯 Project Objectives

1. **High-Precision Distance Measurement:** Maximize the capability of the HC-SR04 ultrasonic sensor paired with STM32 Timer Input Capture over a range of $2\text{ cm}$ to $400\text{ cm}$, targeting an average relative error below $5\%$.
2. **Multi-Level Hazard Alerting:**
   - **Acoustic (Buzzer):** Modulate passive buzzer beep frequency inversely proportional to obstacle distance. Kicks into a continuous alarm when distance drops below $20\text{ cm}$.
   - **Visual (LEDs):** Categorize spatial risk into 3 distinct zones:
     - **Safe Zone:** Solid Green LED
     - **Warning Zone:** Solid / Slow Blinking Yellow LED
     - **Danger Zone:** Solid / Fast Blinking Red LED
3. **Visual User Interface:** Drive a 16x2 LCD via an I2C I/O expander (PCF8574). Line 1 displays real-time distance and active units (supporting $cm \leftrightarrow inch$ toggling via a push-button). Line 2 displays the system FSM state (`Safe` / `Warning` / `Danger` / `Out of Range`).
4. **Telemetry & Data Acquisition:** Package spatial and state data into standardized telemetry strings transmitted via UART to a host PC every $200\text{ ms}$ for real-time monitoring, plotting, and log analysis.

---

## 🛠️ System Architecture & Specifications

### System Scope

| System WILL DO | System WILL NOT DO |
| :--- | :--- |
| Generate $10\mu s$ TRIG pulses via GPIO and capture Echo pulse edges using Timer Input Capture. | Integrate mechanical actuators or interface with vehicle automatic braking/throttle systems. |
| Apply a Moving Average Filter ($N=5$) to smooth spatial metrics and reject impulsive noise spikes. | Perform image recognition or classify obstacle geometries/materials automatically. |
| Execute hardware EXTI interrupts with $200\text{ ms}$ software lock-out debounce for unit switching. | Utilize a Real-Time Operating System (FreeRTOS); system relies strictly on Bare-metal + Interrupt architecture. |
| Monitor ultrasonic Echo response timeout to issue "Out of Range" states without freezing the CPU. | |

### Test Environment Assumptions
- **Reference Target:** Perpendicular, hard flat surface used as the primary benchmark to optimize acoustic reflection.
- **Power Supply Integrity:** Regulated $5\text{V}$ (HC-SR04, LCD) and $3.3\text{V}$ (STM32) power rails operating without voltage sag during peripheral activation.

---

## 🔬 Hardware Component Bill of Materials (BOM)

| Component Category | Part / Module Name | Description / Specifications |
| :--- | :--- | :--- |
| **Microcontroller Board** | STM32F103C8T6 (Blue Pill) | ARM Cortex-M3 core @ $72\text{ MHz}$ |
| **Programmer / Debugger** | ST-Link V2 | SWD programming & debugging interface |
| **Primary Sensor** | HC-SR04 Ultrasonic Module | $2\text{ cm} - 400\text{ cm}$ range, $40\text{ kHz}$ acoustic frequency |
| **Display Interface** | 16x2 Character LCD + PCF8574 | I2C communication interface module |
| **Acoustic Actuator** | Passive Piezoelectric Buzzer | Driven via Timer PWM |
| **Visual Indicators** | 3x LEDs (Green, Yellow, Red) | Driven via GPIO with $220\Omega$ current-limiting resistors |
| **User Input** | 1x Tactile Push-Button | Unit conversion ($cm/inch$), $10\text{k}\Omega$ pull-up resistor |
| **Serial Telemetry** | CP2102 USB-to-UART Module | Transmitting logs to Serial Terminal on PC |
| **Software Toolchain** | VS Code, STM32CubeMX, Arm GNU Toolchain, CMake, Ninja, STM32 Programmer CLI |

---

## ⚙️ Peripherals & Signal Processing Logic

### 1. Distance Calculation & Time-Stamping
1. STM32 issues a $10\mu s$ HIGH trigger pulse on the HC-SR04 TRIG pin.
2. HC-SR04 emits an $8$-cycle $40\text{ kHz}$ ultrasound burst and drives the ECHO pin HIGH.
3. Timer 2 (TIM2 Input Capture) captures rising edge time $T_1$ and falling edge time $T_2$.
4. Pulse width duration: $\Delta t = T_2 - T_1$.
5. Distance equation ($v = 0.0343\text{ cm}/\mu s$):
$$S = \frac{v \times \Delta t}{2}$$

### 2. Digital Filtering & Debouncing
- **Moving Average Filter:** Replaces raw distance values with the arithmetic mean of the last $N=5$ consecutive samples to smooth sensor drift:
$$\bar{S}_k = \frac{1}{5} \sum_{i=0}^{4} S_{k-i}$$
- **Software Debounce (EXTI):** When the button EXTI interrupt fires, `HAL_GetTick()` checks elapsed time against `last_valid_time`. If $\Delta t \ge 200\text{ ms}$, the unit toggles; otherwise, the event is rejected as mechanical contact chatter.

---

## 🔄 Finite State Machine (FSM) Specification

The system operates across 4 distinct operational states based on filtered distance metric $S$:
[didn't update the FSM picture yet]
| State Name | Distance Condition ($S$) | Visual Response (LEDs) | Acoustic Response (Buzzer) | LCD / UART Telemetry |
| :--- | :--- | :--- | :--- | :--- |
| `STATE_SAFE` | $S \ge 50\text{ cm}$ | Green LED ON | Silent | `Dist: XX.X cm \| SAFE` |
| `STATE_WARNING` | $20\text{ cm} \le S < 50\text{ cm}$ | Yellow LED ON (Slow Blink $500\text{ ms}$) | Slow Beep ($500\text{ ms}$ period) | `Dist: XX.X cm \| WARNING` |
| `STATE_DANGER` | $S < 20\text{ cm}$ | Red LED ON (Fast Blink $100\text{ ms}$) | Continuous / Rapid Beep ($100\text{ ms}$) | `Dist: XX.X cm \| DANGER` |
| `STATE_OUT_OF_RANGE` | $S \ge 400\text{ cm}$ or Echo Timeout | ALL LEDs OFF | Silent | `Out of Range` |

---

## 🧪 Test Cases & Verification Suite

| Test ID | Test Module | Test Procedure & Description | Expected Acceptance Result |
| :---: | :--- | :--- | :--- |
| **TC1** | Peripheral Test | Execute independent test scripts for LED blinking, Buzzer PWM, LCD string rendering, and UART transmission. | All hardware components respond correctly; no soldering or open-circuit faults. |
| **TC2** | Input Capture Test | Trigger HC-SR04 with $10\mu s$ pulse, capture ECHO edge timestamps via TIM2 IC, output calculated $t$ via UART. | Timer captures edges accurately; calculated distance matches physical ruler within tolerance. |
| **TC3** | Integration & Alarm | Continuously move an obstacle from $100\text{ cm} \rightarrow 2\text{ cm}$. Observe LED, Buzzer, and LCD state transitions. | Buzzer beep frequency accelerates; continuous tone at $<20\text{ cm}$; smooth FSM transition. |
| **TC4** | Button Debounce | Press unit button rapidly and repeatedly to test $cm \leftrightarrow inch$ conversion stability. | Immediate unit conversion on LCD; zero multi-triggering or mechanical contact bounce errors. |
| **TC5** | Out of Range Test | Point sensor at open space ($>400\text{ cm}$) to trigger Echo reception timeout. | LCD shows `Out of Range`; system reverts to `Safe` state gracefully without CPU deadlock. |
| **TC6** | Surface Geometry | Test flat, curved, angular, dented, hollow box, and mesh surfaces at a fixed $50\text{ cm}$ mox. | Quantify reflection attenuation; flat surfaces yield optimal accuracy; mesh/hollow surfaces cause high error. |
| **TC7** | Field of View (FoV) | Move obstacle vertically and horizontally off-axis at $30\text{ cm}$ to map sensor blind zones. | Define empirical beam angle boundary (maximum deviation angle for reliable detection). |
| **TC8** | Reflection Angle | Rotate flat target at angles of $0^\circ, 15^\circ, 30^\circ, 45^\circ, 60^\circ$ relative to the acoustic beam axis at $20\text{ cm}$. | Map relationship between inclination angle and measurement error; identify signal loss threshold. |
| **TC9** | Material Impact | Measure identical size targets made of metal, hard plastic, cardboard, fabric, and acoustic foam at $20\text{ cm}$. | High-density materials give strong Echo response; sound-absorbing foam causes signal extinction. |

### System Acceptance Criteria (Demo Requirements)
1. **Measurement Accuracy:** Average relative error $\le 5\%$ across calibrated distances; non-blocking recovery from `OUT_OF_RANGE` timeouts.
2. **State & Display Responsiveness:** Accurate state transitions at $20\text{ cm}$, $50\text{ cm}$, and $400\text{ cm}$; LCD & UART refresh rate $\le 200\text{ ms}$.
3. **User Interaction:** Instantaneous unit conversion without switch bounce; no system crash under continuous serial stream.
4. **Long-Run Stability:** Continuous $15 - 30$ minute continuous run test with serial logging showing zero stack overflow or timer interrupt locks.

---

## ⚡ Risk Management & Technical Mitigation

| Risk / Failure Mode | Technical Cause | Mitigation Strategy |
| :--- | :--- | :--- |
| **HC-SR04 Echo Noise** | Spurious acoustic reflections in ambient space. | Increase moving average filter sample depth ($N>5$). |
| **Interrupt Priority Conflicts** | High UART/EXTI load blocking Timer Input Capture. | Configure strict NVIC priority hierarchy: `TIM2_IC` (Highest) $\rightarrow$ `SysTick` $\rightarrow$ `UART1` $\rightarrow$ `EXTI` (Lowest). |
| **Timer Counter Overflow** | Missing Echo pulse in open space causing infinite waiting. | Set maximum counter timeout limit corresponding to $400\text{ cm}$; force `STATE_OUT_OF_RANGE`. |
| **Power Rail Voltage Drop** | Inrush current when activating PWM Buzzer and LEDs simultaneously. | Isolate power rails using dedicated low-dropout regulators ($5\text{V}$ and $3.3\text{V}$). |
| **Acoustic Extinction** | Soft absorption materials or extreme angles scattering sound waves. | Document physical sensor boundary limits; implement software timeout safety fallbacks. |

---

## 👥 Team Members & Task Distribution

| Member Name | Student ID | Primary Responsibilities & Deliverables |
| :--- | :---: | :--- |
| **Huỳnh Nhật Nam** *(Leader)* | 25112341 | Firmware development for Display & Communication modules; Input & Actuator unit testing; documentation. |
| **Phạm Minh Huy** | 2500292 | STM32CubeMX initialization & code skeleton; Input & Actuator module implementation; Display test routines. |
| **Thân Đức Phát** | 2551627 | PCB schematic & board layout in Altium; prototype breadboard assembly; hardware peripheral unit test code. |

---

## 📚 References & Technical Datasheets

1. **STMicroelectronics:** *STM32F103x8/B Datasheet & RM0008 Reference Manual*.
2. **Sensors & Actuators:** *HC-SR04 Ultrasonic Sensor User Manual*.
3. **Display & Drivers:** *HD44780 LCD Controller Datasheet* & *PCF8574 Remote 8-Bit I/O Expander Datasheet*.
4. **Communication:** *Silicon Labs CP2102 USB-to-UART Bridge Chipset Datasheet*.
5. **Prior Art & Code Repositories:** ST Community Examples, Controllers Tech STM32 Hardware Tutorials, Embedded C Coursework Repository (Vũ Phúc Thịnh capstone project).