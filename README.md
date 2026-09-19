<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/84948d9c-bbe8-4eb3-b8cc-da1cd1ead80d" /># Secure Access Control System
Secure access control system with multi-level authentication
using LPC2148 ARM7 microcontroller.

## 📌 Project Overview

The system provides secure user authentication using **User ID, Password, and Fingerprint verification**. The R305 fingerprint sensor is used for biometric authentication, while a 4×4 keypad is used for user input and a 16×2 LCD displays system information and authentication status.

An **I2C EEPROM** is used for storing user information, and an **L293D motor driver** controls the access mechanism.

The user enters the User ID and password through the keypad. After successful verification, the fingerprint is checked. If all authentication levels are successful, the LPC2148 controls the DC motor through the L293D motor driver then the door will open successfully.

---

## ⚙️ How It Works

```text
                 POWER ON
                    ↓
            System Initialization
                    ↓
              Enter User ID
                    ↓
             Enter Password
                    ↓
          Fingerprint Verification
                    ↓
             Authentication
                    ↓
          ┌─────────┴─────────┐
          ↓                   ↓
       SUCCESS              FAILURE
          ↓                   ↓
   Access Granted       Access Denied
          ↓                   ↓
    Motor / Lock ON       LCD Message
```
## Block Diagram
<img width="1536" height="1024" alt="image" src="https://github.com/user-attachments/assets/a1445954-df73-40f4-b071-7d0abca6d77d" />

---

## 🧩 Hardware Components

- LPC2148 ARM7
- 16x2 LCD
- 4x4 Keypad
- R305 Fingerprint Sensor
- AT24C256 EEPROM
- L293D Motor Driver
- DC Motor

---

## 💻 Software & Technologies

- Embedded C
- Keil µVision
- Flash Magic

---

## 🔑 Main Features

* User ID authentication
* Password authentication
* Fingerprint authentication
* Fingerprint enrollment
* Fingerprint verification
* Multiple user records
* EEPROM-based data storage
* Admin/setup functions
* Password management
* Keypad-based menu
* LCD status display
* Motor/lock control

---

## 🔄 Authentication Flow

```text
User
 │
 ▼
Enter User ID
 │
 ▼
Enter Password
 │
 ▼
Fingerprint Scan
 │
 ▼
Verify User Details
 │
 ├───────────────┐
 │               │
 ▼               ▼
Valid           Invalid
 │               │
 ▼               ▼
Access          Access
Granted         Denied
 │
 ▼
Motor / Lock
Activated
```
##⚡ **Interrupt / Setup Operation**

The system uses an external interrupt switch connected to the LPC2148.

When the interrupt switch is activated, the controller enters the setup/menu operation, where administrative functions can be performed.

Interrupt Switch
       ↓
External Interrupt
       ↓
LPC2148 Interrupt Handler
       ↓
Setup / Menu Mode
       ↓
Administrative Operations

## 📂 Project Modules

| Module         | Description                       |
| -------------- | --------------------------------- |
| `main.c`       | Main application program          |
| `exint_menu.c` | Menu and interrupt functions      |
| `r305.c`       | R305 fingerprint sensor interface |
| `lcd.c`        | 16×2 LCD driver                   |
| `kpm.c`        | 4×4 keypad driver                 |
| `uart.c`       | UART communication                |
| `i2c.c`        | I2C communication                 |
| `i2c_eeprom.c` | EEPROM read/write operations      |
| `l293d.c`      | L293D motor control               |
| `delay.c`      | Delay functions                   |

---

## 🛠️ Development

1. Develop the firmware using **Embedded C**.
2. Compile the source code using **Keil µVision**.
3. Generate the `.hex` file.
4. Program the HEX file into the **LPC2148** board.
5. Connect the required hardware modules.
6. Power on the system.
7. Perform user authentication using the keypad, password and fingerprint.

---

## 🎯 Applications

* Secure door access systems
* Electronic locking systems
* Restricted-area access
* Employee authentication
* Biometric security systems
* Embedded access-control applications

---

## 📸 Project

The hardware implementation consists of the LPC2148 development board, R305 fingerprint sensor, 4×4 keypad, 16×2 LCD, I2C EEPROM and L293D motor-control interface.

---

## 👨‍💻 Author

**Akshay**

**Embedded Systems & Firmware Engineering Project**

---

## ⭐ Project Highlights

**Microcontroller:** LPC2148 ARM7
**Programming:** Embedded C
**Biometric Sensor:** R305 Fingerprint Sensor
**Display:** 16×2 LCD
**Input:** 4×4 Keypad
**Memory:** I2C EEPROM
**Motor Driver:** L293D
**Communication:** UART & I2C
# SECURE ACCESS CONTROL SYSTEM WITH MULTI-LEVEL AUTHENTICATION
### ARM7 LPC2148 | R305 Fingerprint | AT24C256 EEPROM | 4x4 Keypad | DC Motor Door Lock

> A 3-level secure access system where user must pass ID + Password + Fingerprint to open door. Built on VECTOR ARM7 Development Board.

---

## 📸 Project Demo

### 1. Block Diagram
![Block Diagram](images/block_diagram.png)
*LPC2148 as core controller, R305 via UART0, EEPROM via I2C, LCD & L293D via GPIO*

### 2. Hardware Photos
| Main Screen | Menu Screen |
| :---: | :---: |
| ![Main](images/hardware_main.jpg) | ![Menu](images/hardware_menu.jpg) |
| `SECURE ACCESS CONTROL SYSTEM` on LCD | `1.ENROLL 2.EDIT 3.DELETE 4.EXIT` |

### 3. Working Video Demo
> Door opens (DC Fan rotates) after successful 3-level authentication

https://github.com/YOUR_USERNAME/Secure-Access-Control-System/assets/demo.mp4
*Video shows: LCD displays menu -> Finger placed -> Access Granted -> Fan ON (Door Open)*

![Working Demo](images/demo.gif)
*If video link doesn't play, upload demo.mp4 in GitHub -> it will auto-convert to this.*

---

## 🛠️ Components Used

**Hardware:**
- LPC2148 ARM7 Microcontroller
- R305 Fingerprint Module (UART0)
- 4x4 Matrix Keypad
- 16x2 LCD Display
- AT24C256 EEPROM (I2C External Storage)
- L293D Motor Driver + DC Motor/Fan (Door Lock)
- VECTOR Advanced Development Board for ARM7

**Software:**
- Keil uVision4
- Embedded C
- Flash Magic

## ⚙️ Working Principle

**Enrollment:**
`ID (1 Byte) + Password (4 Bytes + '\0') -> Stored at EEPROM address (ID-1)*6`

**Authentication:**
1. Enter ID -> System reads from `EEPROM_BASE + 1`
2. Enter Password -> Compares with stored password using `strcmp()`
3. Place Finger -> R305 verifies via UART0
4. If all match -> `ACCESS GRANTED` -> L293D drives DC Motor (Door Opens)

## 🐛 Bug Fixed During Development
**Issue:** Password always incorrect after enroll.
**Root Cause:** Address miscalculation `found+1` instead of `(found-1)*6 + 1`, missing NULL termination.
**Fix:** Now reads 5 bytes (4 char + '\0') and properly terminates string before strcmp.

## 📂 Folder Structure

