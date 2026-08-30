# Bluetooth Robotic Hand

An embedded systems project that uses a sensor-equipped glove to wirelessly control a robotic hand in real time.

The system reads finger movement using flex sensors, processes the sensor data with a microcontroller, and translates the measurements into servo positions controlling the robotic hand. Bluetooth communication enables wireless control between the glove and robotic system.

## Table of Contents

* [Project Overview](#project-overview)
* [System Architecture](#system-architecture)
* [Hardware](#hardware)
* [Embedded Systems Concepts](#embedded-systems-concepts)
* [Project Images](#project-images)
* [Future Improvements](#future-improvements)

---

## Project Overview

The goal of this project was to develop a real-time embedded system capable of translating human hand movements into corresponding movements on a robotic hand.

Flex sensors mounted on a glove measure finger position. The microcontroller acquires and processes these analog signals before mapping them to servo positions.

The project involved embedded firmware development, sensor integration, actuator control, wireless communication, circuit prototyping, and hardware debugging.

---

## System Architecture

```text
Flex Sensors
     │
     ▼
Glove Microcontroller
     │
     │ Bluetooth
     ▼
Robotic Hand Microcontroller
     │
     ▼
Servo Motors
     │
     ▼
Robotic Fingers
```

---

## Hardware

* Arduino-compatible microcontrollers
* Flex sensors
* Bluetooth communication module
* Servo motors
* Custom sensor glove
* Robotic hand assembly
* Prototype circuitry and wiring

---

## Embedded Systems Concepts

This project demonstrates experience with:

* Embedded C/C++ firmware development
* Analog sensor acquisition
* ADC-based sensor measurements
* Real-time sensor processing
* PWM servo control
* Bluetooth communication
* Hardware-software integration
* Sensor calibration
* Circuit prototyping
* Embedded system debugging

---

## Project Images

### Sensor Glove

![Bluetooth Robotic Hand Glove](Glove_BLURoboticHand.png)

The wearable glove contains flex sensors used to measure the position of the user's fingers.

### Robotic Hand

![Robotic Hand](Robotic_Hand_Model.png)

The robotic hand uses servo motors to reproduce finger movements detected by the sensor glove.

### Hardware Close-Up

![Robotic Hand Close-Up](Robotic_HandCloseup.png)

Close-up of the embedded hardware and electrical integration used to control the robotic hand.

### Circuit Schematic

![Robotic Hand Schematic](Robotic_Hand_Model_Schematic.png)

Circuit schematic showing the electrical connections between the microcontroller and robotic hand hardware.

---

## Future Improvements

Potential improvements to the system include:

* Improved sensor calibration
* Closed-loop finger position feedback
* More robust wireless communication
* Custom PCB integration
* Additional sensors for hand orientation
* Improved real-time response and filtering
