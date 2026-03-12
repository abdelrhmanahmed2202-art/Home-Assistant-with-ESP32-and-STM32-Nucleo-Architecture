# Mobile FarmBot Control System

This repository contains the firmware and control logic for an autonomous, mobile FarmBot graduation project. The system utilizes an STM32 microcontroller for precise real-time hardware control and an ESP microcontroller to interface with Home Assistant for remote monitoring and automation.

## ⚙️ Hardware Components
* **Main Controller:** STM32 (Handles precise motor timing and kinematics)
* **IoT/Web Interface:** ESP32/ESP8266 (Handles Wi-Fi and Home Assistant integration)
* **Stepper Motors:** 4x NEMA 23
* **Motor Drivers:** 4x TB6600 Stepper Motor Drivers

## 📂 Repository Structure

* `/STM32_Code`
  * Contains the C/C++ source files, `.ioc` configuration, and drivers generated via STM32CubeIDE.
  * Responsible for generating the PWM signals required by the TB6600 drivers to control the 4 NEMA 23 motors.
* `/ESP_HomeAssistant_Code`
  * Contains the ESP code used to bridge the STM32 hardware with our web application.
  * Includes the configuration for integrating the robot into a Home Assistant dashboard.
* `/Media`
  * Contains a short video demonstration of the motors and control system in action. 

## 🚀 How It Works
1. **User Input:** Commands are sent via the Home Assistant dashboard to the ESP.
2. **Communication:** The ESP relays these commands to the STM32.
3. **Execution:** The STM32 calculates the necessary steps and outputs high-speed signals to the TB6600 drivers, smoothly moving the NEMA 23 motors to the target positions.

## 🎥 Demonstration
*(https://drive.google.com/file/d/18_L9GZR3KirrR-WXnmTAEp8Pghg2siYp/view?usp=sharing)*
