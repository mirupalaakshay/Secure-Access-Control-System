# Secure Access Control System
Secure access control system with multi-level authentication
using LPC2148 ARM7 microcontroller.

## Hardware Used
- LPC2148 ARM7
- 16x2 LCD
- 4x4 Keypad
- R305 Fingerprint Sensor
- AT25C256 EEPROM
- L293D Motor Driver
- DC Motor

## Authentication Process
1. User ID verification
2. Password verification
3. Fingerprint verification

## Software Used
- Embedded C
- Keil µVision
- Flash Magic

## Working
The user enters the User ID and password through the keypad.
After successful verification, the fingerprint is checked.
If all authentication levels are successful, the LPC2148
controls the DC motor through the L293D motor driver.
