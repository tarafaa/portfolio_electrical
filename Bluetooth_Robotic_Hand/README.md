# 🦾 Bluetooth Robotic Hand

A wireless embedded system that uses a sensor-equipped glove to control a servo-driven robotic hand in real time.

The wearable glove uses an **Arduino Nano** to acquire finger-position measurements from five flex sensors. The processed sensor data is transmitted wirelessly using an **HC-09 Bluetooth module** to an **Arduino Uno**, which controls five servo motors corresponding to the fingers of the robotic hand.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Hardware Demo](#hardware-demo)
- [System Architecture](#system-architecture)
- [Hardware](#hardware)
- [Communication](#communication)
- [Firmware](#firmware)
- [Sensor Processing](#sensor-processing)
- [Embedded Systems Concepts](#embedded-systems-concepts)
- [Project Images](#project-images)
- [Future Improvements](#future-improvements)

---

## Project Overview

The goal of this project was to develop a wireless embedded system capable of translating human finger movements into corresponding movements on a robotic hand.

The system is divided into two embedded subsystems:

**Sensor Glove**
- Arduino Nano
- Five flex sensors
- HC-09 Bluetooth module

**Robotic Hand**
- Arduino Uno
- HC-09 Bluetooth module
- Five servo motors

The Arduino Nano acquires analog measurements from the flex sensors mounted on the glove. These measurements represent the position of each finger and are processed before being transmitted over Bluetooth.

The Arduino Uno receives the transmitted finger-position data and converts the commands into servo positions controlling the robotic hand.

---

## Hardware Demo

The prototype demonstrates wireless control of the robotic hand using finger movements measured from the wearable sensor glove.

<!-- Drag the prototype video into the GitHub editor here -->

---

## System Architecture

```text
           SENSOR GLOVE
    ┌────────────────────────┐
    │      Flex Sensors      │
    │       A0 - A4          │
    └───────────┬────────────┘
                │
                │ Analog
                ▼
    ┌────────────────────────┐
    │      Arduino Nano      │
    │                        │
    │   Sensor Acquisition   │
    │          ↓             │
    │   Signal Processing    │
    │          ↓             │
    │    Finger Position     │
    └───────────┬────────────┘
                │
                │ Serial
                ▼
          ┌───────────┐
          │   HC-09   │
          │ Bluetooth │
          └─────┬─────┘
                │
                │ Wireless
                │ Bluetooth
                ▼
          ┌───────────┐
          │   HC-09   │
          │ Bluetooth │
          └─────┬─────┘
                │
                │ Serial
                ▼
    ┌────────────────────────┐
    │       Arduino Uno      │
    │                        │
    │    Receive Commands    │
    │          ↓             │
    │     Servo Control      │
    └───────────┬────────────┘
                │
                │ PWM
                ▼
       ┌─────────────────┐
       │  5 Servo Motors │
       └────────┬────────┘
                │
                ▼
          ROBOTIC HAND
```

---

## Hardware

| Component | Purpose |
|---|---|
| Arduino Nano | Processes flex sensor measurements on the glove |
| Arduino Uno | Controls the robotic hand |
| 5 Flex Sensors | Measure individual finger movement |
| HC-09 Bluetooth Modules | Wireless communication between glove and hand |
| 5 Servo Motors | Control individual robotic fingers |
| Sensor Glove | Wearable sensor interface |
| Robotic Hand | Mechanical output system |
| Prototype Circuitry | Electrical integration and connections |

### Glove Sensor Inputs

| Finger | Arduino Nano Input |
|---|---|
| Thumb | A0 |
| Index | A1 |
| Middle | A2 |
| Ring | A3 |
| Pinky | A4 |

### Robotic Hand Servo Outputs

| Finger | Arduino Uno Output |
|---|---|
| Thumb | D2 |
| Index | D3 |
| Pinky | D4 |
| Middle | D5 |
| Ring | D6 |

---

## Communication

Wireless communication between the glove and robotic hand is provided by the **HC-09 Bluetooth modules**.

```text
Flex Sensors
     ↓
Arduino Nano
     ↓
Serial Data
     ↓
HC-09
     ↓
 Bluetooth
     ↓
HC-09
     ↓
Serial Data
     ↓
Arduino Uno
     ↓
Servo Control
```

This architecture separates sensor acquisition from actuator control and allows the robotic hand to operate without a physical connection to the glove.

---

## Firmware

The system firmware handles three main tasks:

### 1. Sensor Acquisition

The Arduino Nano reads analog measurements from five flex sensors corresponding to the user's fingers.

Multiple sensor samples can be averaged to reduce noise and provide more stable finger-position measurements.

### 2. Wireless Transmission

Processed finger-position data is transmitted from the glove controller through the HC-09 Bluetooth connection.

The robotic hand controller receives the transmitted data and determines the requested position for each finger.

### 3. Servo Control

The Arduino Uno controls five servo motors corresponding to the five robotic fingers.

Sensor measurements are mapped to servo angles using calibrated extended and closed positions.

For the prototype, the flex sensors were calibrated using raw ADC measurements such as:

```text
Extended Position ≈ 256
Closed Position   ≈ 59
```

The resulting measurements are mapped to the required servo position.

---

## Sensor Processing

The glove-side processing flow is:

```text
Finger Movement
      ↓
Flex Sensor
      ↓
ADC Measurement
      ↓
Sensor Filtering
      ↓
Calibration / Mapping
      ↓
Finger Position
      ↓
Bluetooth Transmission
```

On the robotic-hand side:

```text
Bluetooth Data
      ↓
Arduino Uno
      ↓
Finger Command
      ↓
Servo Position
      ↓
Robotic Finger Movement
```

---

## Embedded Systems Concepts

This project demonstrates experience with:

- Embedded C/C++
- Arduino Nano and Arduino Uno
- Analog-to-Digital Conversion (ADC)
- Analog sensor acquisition
- UART / serial communication
- Bluetooth communication
- Sensor calibration
- Signal filtering
- PWM servo control
- Real-time sensor and actuator control
- Multi-microcontroller communication
- Hardware-software integration
- Circuit prototyping
- Embedded system debugging

---

## Project Images

### Sensor Glove

![Sensor Glove](Glove_BLURoboticHand.png)

Wearable glove containing five flex sensors and an Arduino Nano used for finger-position acquisition.

### Robotic Hand

![Robotic Hand](Robotic_Hand_Model.png)

Arduino Uno-controlled robotic hand using five servo motors to reproduce finger movements received from the sensor glove.

### Hardware Close-Up

![Robotic Hand Close-Up](Robotic_HandCloseup.png)

Prototype hardware showing the integration of the controller, servo motors, and robotic hand.

### Circuit Schematic

![Robotic Hand Schematic](Robotic_Hand_Model_Schematic.png)

Electrical schematic showing the hardware connections used in the robotic hand system.

---

## Future Improvements

Potential improvements include:

- Individual calibration profiles for each flex sensor
- Improved digital filtering
- Closed-loop finger position feedback
- More robust wireless packet handling
- Communication error detection
- Custom PCB integration
- Additional sensors for hand orientation
- Register-level firmware development
