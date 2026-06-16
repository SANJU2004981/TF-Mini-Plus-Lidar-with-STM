🚀 Overview

The TFmini Plus is a single-point ranging LiDAR based on Time-of-Flight (ToF)
principles. This implementation:

  - Parses the 9-byte raw data frame at high frequency.
  - Extracts Distance (cm), Signal Strength, and Sensor Temperature.
  - Uses Checksum verification to ensure data integrity.
  - Utilizes Hardware UART (USART1) for reliable communication.

🛠 Hardware Requirements

  - Microcontroller: ST Nucleo-H723ZG
  - LiDAR Sensor: Benewake TFmini Plus
  - Cables: Jumper wires (Female-to-Male)
  - Environment: VS Code with PlatformIO extension.

📌 Wiring Diagram

The TFmini Plus operates at 5V for power but uses 3.3V Logic. Ensure you connect
the LiDAR TX to the STM32 RX.

| TFmini Plus Wire | Signal   | STM32 Nucleo Pin   | MCU Pin  |
| :--------------- | :------- | :----------------- | :------- |
| **Red**          | \+5V     | **5V**             | \-       |
| **Black**        | GND      | **GND**            | \-       |
| **White**        | LiDAR TX | **D2** (USART1 RX) | **PA10** |
| **Green**        | LiDAR RX | **D8** (USART1 TX) | **PA9**  |

Note: Ensure the power supply can provide up to 140mA (peak) for the LiDAR.

💻 Software Setup

1. Project Configuration

Ensure your platformio.ini file is configured as follows:

[env:nucleo_h723zg]
platform = ststm32
board = nucleo_h723zg
framework = arduino
monitor_speed = 115200

2. Implementation Logic

The code listens for the standard TFmini Plus data packet format:

  - Header: 0x59 0x59
  - Data: 2 bytes Distance, 2 bytes Strength, 2 bytes Temperature.
  - Verification: 1 byte Checksum (Low 8 bits of the sum of the first 8 bytes).

📊 Sample Output

When running, the Serial Monitor (115200 baud) will display:

Distance: 247 cm     Strength: 5012      Temp: 49.25 °C
Distance: 247 cm     Strength: 5015      Temp: 50.00 °C
Distance: 175 cm     Strength: 10083     Temp: 50.00 °C

🔧 How to Use

1.  create a new project in PlatformIO
2.  Open the src -> main.cpp.
3.  then paste the program
4.  also paste the PlatformIO.ini.
5.  Connect your Nucleo-H723ZG via USB.
6.  Click the PlatformIO: Build icon.
7.  Click the PlatformIO: Upload icon.
8.  Open the Serial Monitor to view live LiDAR data.

