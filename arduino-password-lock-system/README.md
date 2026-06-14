# Arduino Password-Protected Lock System

This repository presents an Arduino-based password-protected lock system with motion detection and alarm functionality.

The project was developed as a microcontroller-based embedded system. It combines keypad-based password entry, LCD user feedback, servo motor control, MPU6050 motion detection, LED indicators, and buzzer alarm logic.

## Project Overview

The system is designed to model a simple embedded security application. A user enters a 4-digit password using a keypad. If the password is correct, the servo motor unlocks the mechanism. If the password is incorrect, the system gives visual and audible warnings. After multiple incorrect attempts, the system enters a locked alarm state.

The MPU6050 accelerometer/gyroscope sensor is used to detect physical movement. If motion is detected, the alarm is triggered using a red LED and buzzer.

## Key Features

* 4-digit password entry using a keypad
* LCD-based user interface
* Servo motor-based locking and unlocking mechanism
* MPU6050-based motion detection
* LED indicators for access and alarm states
* Buzzer warning for incorrect password and alarm events
* Incorrect password attempt counter
* Embedded control logic using Arduino C/C++

## Hardware Components

* Arduino Uno / Nano
* 4x3 Keypad
* 16x2 I2C LCD Display
* SG90 Servo Motor
* MPU6050 Accelerometer and Gyroscope Sensor
* Buzzer
* Red LED
* Green LED
* Resistors and jumper wires

## System Operation

1. The system starts in locked mode.
2. The LCD displays `ENTER THE CODE`.
3. The user enters a 4-digit password using the keypad.
4. If the password is correct, access is granted and the servo motor unlocks the mechanism.
5. If the password is incorrect, the red LED and buzzer are activated briefly.
6. After three incorrect attempts, the system enters a locked alarm state and requires reset/restart.
7. The MPU6050 sensor monitors motion.
8. If motion is detected, the alarm is triggered.

## Software Logic

The Arduino code is structured around the following functions:

* `handlePasswordInput()`: Reads keypad input and manages password entry.
* `checkPassword()`: Compares the entered password with the saved password.
* `incorrectPassword()`: Activates warning feedback after incorrect password entry.
* `detectMotion()`: Reads MPU6050 acceleration values and detects movement.
* `triggerAlarm()`: Activates the alarm when motion is detected.
* `unlock()`: Controls the servo motor and unlocks the mechanism after correct password entry.

## Communication and Control

The system uses I2C communication for the LCD display and MPU6050 sensor. The servo motor is controlled using PWM, while the keypad, LEDs, and buzzer are controlled using Arduino digital I/O pins.

## Repository Structure

```text
arduino-password-lock-system/
├── README.md
├── src/
│   └── password_lock_system.ino
└── circuit/
    └── circuit_diagram.png
```

## Tools and Technologies

* Arduino IDE
* Arduino C/C++
* I2C communication
* PWM servo control
* Embedded system design
* Sensor-based motion detection

## Possible Improvements

* Add EEPROM support to store and update the password.
* Add reset button or master password functionality.
* Improve motion detection using filtered MPU6050 data.
* Add debounce logic for keypad input.
* Add low-power mode for battery-operated use.

## Note

The original project report is not included in this repository to avoid sharing student numbers and personal information. This repository contains the Arduino source code, circuit documentation, and a summarized project description.
