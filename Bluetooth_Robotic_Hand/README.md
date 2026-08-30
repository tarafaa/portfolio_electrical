# 🦾 Robotic Hand Glove Controller

An embedded systems project developed using an **Arduino Uno (ATmega328P)**, flex sensors, and servo motors to translate human finger movement into real-time robotic hand motion.

Five flex sensors mounted on a wearable glove provide analog measurements of finger position. The microcontroller processes and maps these sensor readings to servo positions controlling each finger of the robotic hand.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Hardware Demo](#hardware-demo)
- [System Architecture](#system-architecture)
- [Hardware](#hardware)
- [Firmware](#firmware)
- [Sensor Processing](#sensor-processing)
- [Embedded Systems Concepts](#embedded-systems-concepts)
- [Project Images](#project-images)
- [Future Improvements](#future-improvements)

---

## Project Overview

The goal of this project was to develop an embedded system capable of translating human finger movements into corresponding movements on a robotic hand.

Five flex sensors mounted on a glove measure the position of the user's fingers. The Arduino Uno acquires the analog sensor signals through its ADC and maps each measurement to a corresponding servo position.

To improve stability, multiple sensor samples are averaged before calculating the requested servo angle. A small angular deadband is also used to reduce unnecessary servo movement caused by sensor noise.

The project involved:

- Embedded firmware development
- Analog sensor acquisition
- ADC-based measurements
- Servo motor control
- Sensor calibration
- Signal filtering
- Hardware-software integration
- Circuit prototyping
- Hardware debugging

---

## Hardware Demo

The prototype demonstrates flex sensor input being translated into physical movement of the servo-controlled robotic hand.

<!-- Add GitHub video attachment here -->

---

## System Architecture

```text
       Wearable Sensor Glove
               │
        Five Flex Sensors
           A0 - A4
               │
               │ Analog Signals
               ▼
      ┌──────────────────┐
      │   Arduino Uno    │
      │    ATmega328P    │
      │                  │
      │  ADC Acquisition │
      │        ↓         │
      │ Moving Average   │
      │        ↓         │
      │ Sensor Mapping   │
      │        ↓         │
      │  Servo Control   │
      └────────┬─────────┘
               │
               │ Servo Commands
               ▼
        Five Servo Motors
               │
               ▼
          Robotic Hand
```

---

## Hardware

| Component | Purpose |
|---|---|
| Arduino Uno | Main embedded controller |
| ATmega328P | Microcontroller |
| 5 Flex Sensors | Measure individual finger movement |
| 5 Servo Motors | Control robotic finger positions |
| Sensor Glove | Wearable sensor interface |
| Robotic Hand | Mechanical output system |
| Prototype Circuitry | Sensor and actuator connections |

### Pin Assignments

| Finger | Flex Sensor | Servo |
|---|---|---|
| Thumb | A0 / ADC0 | D2 |
| Index | A1 / ADC1 | D3 |
| Middle | A2 / ADC2 | D5 |
| Ring | A3 / ADC3 | D6 |
| Pinky | A4 / ADC4 | D4 |

---

## Firmware

The original robotic hand was implemented using **Arduino C++** on the Arduino Uno.

The firmware performs four primary operations:

1. Acquire analog measurements from the five flex sensors.
2. Average multiple ADC samples to reduce sensor noise.
3. Convert the calibrated sensor measurements into finger angles.
4. Update the corresponding servo when the requested angle changes.

### Sensor Calibration

The flex sensors were calibrated using raw ADC measurements representing the extended and closed finger positions.

```cpp
RAW_EXTENDED = 256
RAW_CLOSED   = 59
```

The calibrated values are mapped to approximately:

```text
0°  → Finger Extended
90° → Finger Closed
```

### Jitter Reduction

A four-sample moving average is applied to each flex sensor:

```cpp
SMOOTH_N = 4
```

A small angular deadband prevents unnecessary servo updates:

```cpp
DEADBAND_DEG = 1
```

This helps reduce visible servo jitter caused by small variations in the analog sensor readings.

---

## Sensor Processing

The firmware processing sequence is:

```text
Flex Sensor
     │
     ▼
ADC Measurement
     │
     ▼
4-Sample Moving Average
     │
     ▼
Sensor Calibration
     │
     ▼
Raw ADC → Finger Angle
     │
     ▼
Angular Deadband
     │
     ▼
Servo Position
```

Each sensor is processed independently, allowing the five robotic fingers to respond to their corresponding flex sensors.

---

## Embedded Systems Concepts

This project demonstrates experience with:

- Embedded C/C++
- Arduino Uno / ATmega328P
- Analog-to-Digital Conversion (ADC)
- Analog sensor acquisition
- Sensor calibration
- Moving-average filtering
- PWM-based servo control
- Real-time sensor processing
- Hardware-software integration
- Circuit prototyping
- Embedded system debugging

---

## Project Images

### Sensor Glove

![Sensor Glove](Glove_BLURoboticHand.png)

Wearable prototype containing flex sensors used to measure individual finger movement.

### Robotic Hand

![Robotic Hand](Robotic_Hand_Model.png)

Servo-controlled robotic hand used to reproduce finger positions measured by the sensor glove.

### Hardware Close-Up

![Robotic Hand Close-Up](Robotic_HandCloseup.png)

Close-up of the robotic hand hardware and servo integration.

### Circuit Schematic

![Robotic Hand Schematic](Robotic_Hand_Model_Schematic.png)

Electrical schematic showing the connections between the Arduino Uno and robotic hand hardware.

---

## Future Improvements

Potential improvements include:

- Individual calibration profiles for each flex sensor
- Closed-loop finger position feedback
- Improved digital filtering
- Wireless communication between the glove and robotic hand
- Custom PCB integration
- Additional sensors for hand orientation
- Register-level ATmega328P firmware
- Non-blocking sensor acquisition and control
