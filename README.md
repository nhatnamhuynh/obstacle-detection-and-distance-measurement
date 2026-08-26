```markdown
# Obstacle Detection and Distance Measurement Using Ultrasonic Sensing and Timer Input Capture on STM32F103[cite: 1]

> **Vietnamese Title:** Hệ thống Phát hiện vật cản và Đo khoảng cách Sử dụng Cảm biến Siêu âm và Timer Input Capture trên STM32F103[cite: 1]  
> **Course:** MLIoT - Embedded C 2026[cite: 1]  
> **Group:** Group 3 (SHEETS)[cite: 1]  

---

## 1. Executive Summary & Problem Statement

Rear blind spots in motor vehicles (especially during reversing maneuvers) and mobile robots pose significant risks of collisions[cite: 1]. Conventional rearview mirrors cannot fully cover all spatial zones, particularly for low-profile or unexpectedly moving obstacles[cite: 1].

This project implements a real-time, high-accuracy smart reversing warning system on the STM32F103 micro-controller[cite: 1]. By leveraging hardware-based time measurement via **Timer Input Capture** instead of software delay loops, the system achieves precise distance calculations while dramatically reducing CPU waiting overhead[cite: 1]. The architecture delivers multi-channel feedback, including multi-color LED indicators, a PWM passive buzzer, an I2C LCD display, and real-time UART data telemetry[cite: 1].

---

## 2. Project Objectives

1. **High-Precision Distance Measurement:** Utilize the HC-SR04 ultrasonic sensor with STM32 Timer Input Capture across a $2\text{ cm} - 400\text{ cm}$ range, maintaining an average relative error below $5\%$[cite: 1].
2. **Multi-Tiered Alert System:**
   * **Acoustic (Buzzer):** Modulate beep frequency inversely proportional to obstacle distance, transitioning to a continuous alarm tone when distance $< 20\text{ cm}$[cite: 1].
   * **Visual (LEDs):** Categorize ranges into 3 distinct zones: Safe Zone (Green LED ON), Warning Zone (Yellow LED ON / slow blink), Danger Zone (Red LED ON / fast blink)[cite: 1].
3. **Intuitive User Interface:** Drive a $16\times2$ LCD via I2C (PCF8574)[cite: 1]. Line 1 displays real-time distance and unit (supporting $cm \leftrightarrow inch$ toggle via button); Line 2 displays FSM operational states (`Safe`/`Warning`/`Danger`)[cite: 1].
4. **Data Logging & Telemetry:** Package distance and state telemetry into formatted strings, transmitting via UART to a PC host every $200\text{ ms}$ for real-time monitoring and analysis[cite: 1].

---

## 3. System Scope & Technical Assumptions

### In-Scope
* **Signal Acquisition:** Generate $10\mu s$ TRIG pulse via GPIO; capture ECHO rising/falling edges using Timer Input Capture[cite: 1].
* **Digital Filtering:** Implement a 5-sample Moving Average Filter ($N=5$) to suppress transient spikes[cite: 1].
* **Interrupt & Input Debouncing:** Process unit conversion button presses via EXTI interrupts integrated with a $200\text{ ms}$ software debounce window[cite: 1].
* **PWM Audio Control:** Drive a passive buzzer via Timer PWM to dynamically adjust beep alert frequency[cite: 1].
* **Exception Handling:** Detect missing Echo signals ($> 400\text{ cm}$ or timeout) and safely transition to an `Out of Range` state without halting the MCU[cite: 1].

### Out-of-Scope
* Mechanical actuation, automatic braking systems, or motor drives[cite: 1].
* Computer vision, camera-based object recognition, or surface material classification[cite: 1].
* Real-Time Operating Systems (FreeRTOS); the project strictly uses a bare-metal architecture with hardware interrupts[cite: 1].

### System Assumptions
* Primary testing obstacles are rigid and flat, positioned perpendicular to the acoustic beam axis[cite: 1].
* Power supply rails ($5\text{V}$ for HC-SR04/LCD and $3.3\text{V}$ for STM32) remain stable without voltage sags during peripheral activation[cite: 1].

---

## 4. Hardware & Software Specifications

### Bill of Materials (BOM)

| Component Category | Hardware / Tool Name | Quantity / Details |
| :--- | :--- | :--- |
| **Microcontroller Board** | STM32F103C8T6 (Blue Pill)[cite: 1] | 1x Board + ST-Link V2 Debugger[cite: 1] |
| **Primary Sensor** | HC-SR04 Ultrasonic Sensor[cite: 1] | 1x Module ($2\text{ cm} - 400\text{ cm}$ range)[cite: 1] |
| **Display Module** | LCD 16x2 + PCF8574 I2C Adapter[cite: 1] | 1x Display Assembly[cite: 1] |
| **Audio & Visual** | Passive Buzzer, LEDs (Green, Yellow, Red)[cite: 1] | 1x Buzzer, 3x LEDs, 220$\Omega$ Resistors[cite: 1] |
| **User Input & Serial** | Tactile Push Button, CP2102 USB-UART Converter[cite: 1]| 1x Button (10k$\Omega$ pull-up), 1x Serial Module[cite: 1] |
| **Toolchain & Software** | VS Code, STM32CubeMX, Arm GNU Toolchain[cite: 1] | CMake, Ninja, STM32 Programmer CLI, Draw.io[cite: 1] |
| **Software Libraries** | STM32 HAL Library, Standard C Libraries[cite: 1] | Embedded C Bare-Metal Execution[cite: 1] |

---

## 5. Architectural Design & Signal Processing

### 1. Distance Calculation Formula
The STM32 pulls the TRIG pin HIGH for $10\mu s$[cite: 1]. The HC-SR04 emits an ultrasonic burst and pulls the ECHO pin HIGH until the reflected wave is received[cite: 1]. Timer Input Capture records timestamps $T_1$ (rising edge) and $T_2$ (falling edge)[cite: 1]:
$$\Delta T = T_2 - T_1 \quad (\mu s)[cite: 1]$$
$$\text{Distance } S = \frac{v \cdot \Delta T}{2} = \frac{0.0343 \cdot \Delta T}{2} \quad (\text{cm})[cite: 1]$$

### 2. Button Debounce Algorithm
When an EXTI interrupt fires on the push button, the system compares the current SysTick timestamp against the last valid press timestamp ($T_{last}$)[cite: 1]:
$$\Delta t = T_{current} - T_{last}[cite: 1]$$
* If $\Delta t \ge 200\text{ ms}$: Accept keypress, update unit ($cm \leftrightarrow inch$), and set $T_{last} = T_{current}$[cite: 1].
* If $\Delta t < 200\text{ ms}$: Reject keypress as mechanical contact bounce[cite: 1].

### 3. Finite State Machine (FSM) Logic


```

[Distance S / Timeout Exception]
│
├───► (S >= 400cm OR Echo Timeout) ──► [STATE_OUT_OF_RANGE]
│                                     ├─ LEDs: OFF
│                                     └─ Buzzer: Silent
│
├───► (S >= 50cm) ──────────────────► [STATE_SAFE]
│                                     ├─ LED: Green ON
│                                     └─ Buzzer: Silent
│
├───► (20cm <= S < 50cm) ───────────► [STATE_WARNING]
│                                     ├─ LED: Yellow Slow Blink
│                                     └─ Buzzer: Slow Beep
│
└───► (S < 20cm) ───────────────────► [STATE_DANGER]
├─ LED: Red Fast Blink
└─ Buzzer: Continuous Tone

```

* **State 1 — Safe State ($S \ge 50\text{ cm}$):** Green LED ON, Buzzer completely silent[cite: 1].
* **State 2 — Warning State ($20\text{ cm} \le S < 50\text{ cm}$):** Yellow LED ON (slow blinking), Buzzer emits slow beeps[cite: 1].
* **State 3 — Danger State ($S < 20\text{ cm}$):** Red LED ON (fast blinking), Buzzer emits rapid continuous alarm[cite: 1].
* **State 4 — Out of Range ($S \ge 400\text{ cm}$ or Echo Timeout):** All 3 LEDs OFF, Buzzer completely silent[cite: 1].

---

## 6. Test Suite & Verification Matrix

| Test ID | Test Category | Execution Procedure | Expected Result |
| :---: | :--- | :--- | :--- |
| **TC1**[cite: 1] | Independent Peripheral Test[cite: 1] | Verify 3-LED blink, Buzzer activation, I2C LCD printing, and UART text output[cite: 1]. | All hardware peripherals respond without solder defects or assembly errors[cite: 1]. |
| **TC2**[cite: 1] | HC-SR04 Input Capture[cite: 1] | Send $10\mu s$ TRIG pulse; capture ECHO edges via Timer IC; print pulse duration over UART[cite: 1]. | Timer captures edge timestamps accurately; calculated distance matches physical tape measure[cite: 1]. |
| **TC3**[cite: 1] | Integration & FSM Alarm[cite: 1] | Sweep target continuously from $100\text{ cm} \rightarrow 2\text{ cm}$; observe LEDs, Buzzer rate, and LCD[cite: 1]. | Buzzer beep frequency increases seamlessly; LEDs transition correctly per FSM rules[cite: 1]. |
| **TC4**[cite: 1] | Button & Debounce[cite: 1] | Press unit conversion button rapidly; verify $cm \leftrightarrow inch$ toggle response[cite: 1]. | Instant unit switching on LCD/UART without double-triggering or contact bouncing[cite: 1]. |
| **TC5**[cite: 1] | Out-of-Range Exception[cite: 1] | Point sensor toward open space or target $> 400\text{ cm}$[cite: 1]. | LCD shows `Out of Range`; system returns to Safe state smoothly without MCU crash[cite: 1]. |
| **TC6**[cite: 1] | Target Geometry Effect[cite: 1] | Measure flat, curved, irregular, hollow box, and mesh targets at a fixed $50\text{ cm}$ mark[cite: 1]. | Quantify measurement errors caused by acoustic scattering and cavity reflection[cite: 1]. |
| **TC7**[cite: 1] | Field of View (FOV)[cite: 1] | Offset target vertically/horizontally from sensor central axis at $30\text{ cm}$ until signal lost[cite: 1]. | Determine sensor effective angular coverage and blind-zone boundaries[cite: 1]. |
| **TC8**[cite: 1] | Reflection Angle Limit[cite: 1] | Tilt flat target at $0^\circ, 15^\circ, 30^\circ, 45^\circ, 60^\circ$ relative to beam normal at $20\text{ cm}$[cite: 1]. | Determine critical reflection angle limits where acoustic echo misses the receiver[cite: 1]. |
| **TC9**[cite: 1] | Target Material Effect[cite: 1] | Test metal, hard plastic, cardboard, cloth, and acoustic foam targets at $20\text{ cm}$[cite: 1]. | Identify optimal reflective materials vs. sound-absorbing materials that attenuate Echo pulses[cite: 1]. |

---

## 7. Acceptance & Demonstration Criteria

* **Measurement Accuracy:** Average relative error $\le 5\%$ across valid measurement points[cite: 1]. Reliable capture of `OUT OF RANGE` states without system lockup[cite: 1].
* **FSM & Display Stability:** Accurate state rendering (`SAFE`, `WARNING`, `DANGER`) with smooth transitions at boundaries ($20\text{ cm}, 50\text{ cm}, 400\text{ cm}$)[cite: 1]. Refresh rates for LCD and UART updated every $200\text{ ms}$[cite: 1].
* **User Interaction:** Instantaneous unit conversion without switch bounce glitches[cite: 1]. Data stream over UART remains clean and uncorrupted[cite: 1].
* **Repository Quality:** Complete GitHub repository containing clean source code, complete `README.md`, and verifiable empirical test data[cite: 1].

---

## 8. Team Structure & Task Allocation

| Member Name | Student ID | Role | Key Deliverables & Responsibilities |
| :--- | :---: | :--- | :--- |
| **Huỳnh Nhật Nam**[cite: 1] | **25112341**[cite: 1] | Team Leader[cite: 1] | Display & UART Communication Drivers, Input/Actuator Test Code, System Architecture[cite: 1]. |
| **Phạm Minh Huy**[cite: 1] | **2500292**[cite: 1] | Core Firmware[cite: 1] | CubeMX Init & Project Boilerplate, Input/Actuator Drivers, Display/Comm Test Code[cite: 1]. |
| **Thân Đức Phát**[cite: 1] | **2551627**[cite: 1] | Hardware & Testing[cite: 1] | PCB Layout Design, Hardware Prototype Assembly, Component Test Code, Soldering[cite: 1]. |

---

## 9. References & Datasheets

* **STM32F103x Reference Manual & Datasheet** — STMicroelectronics[cite: 1]
* **HC-SR04 Ultrasonic Sensor Datasheet** — SparkFun / Elecfreaks[cite: 1]
* **HD44780 LCD Controller & PCF8574 I2C Backpack Datasheets** — Texas Instruments / Hitachi[cite: 1]
* **CP2102 USB-to-UART Bridge Controller Datasheet** — Silicon Labs[cite: 1]
* **ST Community & Controllers Tech Tutorials** — STM32 HAL & Input Capture Guides[cite: 1]

```