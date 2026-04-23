# Electrical Portfolio 

# 🌱 Environmental Monitoring Node (RTOS-Based Data Acquisition System)

##  Overview

This project is a **real-time environmental monitoring system** that collects and displays sensor data including:

* Temperature
* Pressure
* Humidity
* Gas Resistance

Data is acquired from a **BME sensor module** and displayed on:

* Serial monitor (for logging/debugging)
* OLED display (for real-time visualization)

The system is designed with a focus on **reliable data acquisition, real-time performance, and embedded system integration**.

---

##  System Architecture

### Components:

* **Microcontroller** (Arduino Nano / compatible)
* **BME Sensor** (Temperature, Pressure, Humidity, Gas)
* **OLED Display**
* Serial interface for monitoring

### Data Flow:

Sensor → SPI Communication → RTOS Tasks → Display + Serial Output

---

##  Key Features

* Real-time environmental data acquisition
* Multitasking using RTOS (concurrent sensing + display updates)
* SPI-based communication for low-latency data transfer
* Dual output:

  * OLED display (live data)
  * Serial monitor (debug + logging)
* Designed for **continuous monitoring applications**

---

##  RTOS Design

The system uses an RTOS to manage concurrent operations:

* **Sensor Task**

  * Periodically reads data from BME sensor
* **Display Task**

  * Updates OLED with latest values
* **Communication Task**

  * Sends data to serial monitor

This ensures:

* Deterministic behavior
* Non-blocking execution
* Improved system responsiveness

---

## Hardware Details

* Operating Voltage: **5V**
* Communication Protocol: **SPI**
* Sensor: BME Environmental Sensor Module
* Display: OLED (I2C/SPI depending on configuration)

---

##  Data Acquisition

The system continuously samples environmental data and processes it for output.
Key considerations:

* Stable sampling intervals
* Synchronized data transfer
* Low-latency communication
* Reliable sensor interfacing

---

##  Tools & Technologies

* Embedded C / Arduino Framework
* RTOS (task scheduling & concurrency)
* SPI Communication
* Arduino IDE
* Serial debugging tools

---

##  Challenges & Solutions

### Challenge:

Ensuring consistent and synchronized data acquisition across multiple tasks

### Solution:

* Used RTOS task scheduling to separate sensing and display
* Implemented structured communication between tasks
* Optimized timing to prevent blocking and delays

---

##  Future Improvements

* Add data logging (SD card or cloud integration)
* Improve power efficiency for long-term deployment
* Introduce interrupt-driven sensor acquisition
* Environmental threshold alerts

---

##  Applications

* Environmental monitoring systems
* Industrial sensing applications
* Smart home systems
* Data acquisition platforms

---

## Robotic Hand Control System
## Overview

This subsystem implements a sensor-driven robotic hand model designed to replicate human finger motion using flex sensors. Each finger is equipped with a flex sensor that detects bending and converts mechanical motion into an electrical signal.

The system processes sensor data in real time using a microcontroller and translates it into control signals for actuation or communication. The design emphasizes embedded sensing, signal processing, and hardware-software integration.

## System Architecture
## Components
Microcontroller (Arduino Uno) - Glove and Mech Hand
Flex sensors (one per finger)
Fixed resistors for signal conditioning
Optional actuators (servos or control outputs)
Optional communication module (Bluetooth HC-09)
## Data Flow
Finger Motion → Flex Sensor → Analog Voltage → ADC (Microcontroller) → Processed Output
## Key Features
Real-time detection of finger movement
Analog signal acquisition and processing
Scalable multi-sensor design
Compatible with wireless communication systems
Suitable for human-interactive embedded applications
Sensor Design: Flex Sensors
Sensor Description

A flex sensor is a variable resistor whose resistance changes as it bends. This property allows it to convert physical deformation into an electrical signal that can be measured by a microcontroller.

Unbent state: lower resistance
Bent state: higher resistance
Electrical Configuration

The flex sensor is implemented using a voltage divider circuit to produce a measurable analog voltage.

5V ─── Flex Sensor ────┬──→ Analog Input (Microcontroller)
                       │
                  Fixed Resistor
                       │
                      GND

The output voltage at the midpoint varies proportionally with the bending of the sensor.

## Signal Processing

The microcontroller performs the following operations:

Analog-to-digital conversion (ADC) of the sensor signal
Mapping of voltage levels to meaningful motion values
Thresholding for discrete control (e.g., open/close states)

This enables consistent interpretation of finger movement for control applications.

## Design Considerations
Selection of an appropriate fixed resistor (commonly ~10 kΩ) for stable readings
Minimization of noise in analog signals
Proper placement and alignment of sensors on fingers
Calibration to account for sensor variability and user differences
System Integration

The robotic hand subsystem can be integrated with other modules in the overall system:

Sensor data can be displayed on an OLED interface
Data can be transmitted via Bluetooth using an HC-09 module
Sensor readings can be used directly for actuator control

This creates a complete sensing and control pipeline within an embedded system.

## Applications
Prosthetic and assistive device development
Human-computer interaction systems
Gesture-based control interfaces
Robotics prototyping and education

##  Author

**Society of Women Engineers: Project Manager: Taranaki Faamatuainu**
Engineering @ CSUF
Focus: Embedded Systems, Real-Time Systems, Hardware-Software Integration

---
