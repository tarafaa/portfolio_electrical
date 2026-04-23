# portfolio_electrical
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

##  Author

**Society of Women Engineers: Project Manager: Taranaki Faamatuainu**
Engineering @ CSUF
Focus: Embedded Systems, Real-Time Systems, Hardware-Software Integration

---
