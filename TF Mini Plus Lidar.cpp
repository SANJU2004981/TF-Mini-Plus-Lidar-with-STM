#include <Arduino.h>

// Define the Hardware Serial port for the TFmini.
// Assuming we are using USART1 (PA10 for RX, PA9 for TX).
// Adjust these pins if you wired the sensor to a different UART port.
HardwareSerial tfSerial(PA10, PA9); 

void setup() {
  // Initialize USB Serial for the Serial Monitor
  Serial.begin(115200);
  while (!Serial) { delay(10); } // Wait for USB connection
  
  // Initialize TFmini Plus Serial (Default is 115200)
  tfSerial.begin(115200);
  
  Serial.println("STM32H723ZG + TFmini Plus Initialized!");
  Serial.println("Waiting for data...");
}

void loop() {
  // Buffer to hold the 9-byte data frame from the LiDAR
  static uint8_t frame[9];
  static int index = 0;

  // Read data while it is available in the serial buffer
  while (tfSerial.available()) {
    uint8_t inByte = tfSerial.read();

    // The TFmini Plus data frame ALWAYS starts with 0x59 0x59
    if (index == 0 && inByte != 0x59) continue; // Look for first header byte
    if (index == 1 && inByte != 0x59) {         // Look for second header byte
      index = 0; 
      continue; 
    }

    // Store the valid byte into the array
    frame[index++] = inByte;

    // Once we have a full 9-byte frame, process it
    if (index == 9) {
      index = 0; // Reset index for the next incoming frame

      // Calculate the checksum (sum of first 8 bytes)
      uint8_t checksum = 0;
      for (int i = 0; i < 8; i++) {
        checksum += frame[i];
      }

      // Verify the checksum matches the 9th byte
      if (checksum == frame[8]) {
        // Parse Distance (Bytes 2 and 3)
        uint16_t distance = frame[2] | (frame[3] << 8);

        // Parse Signal Strength (Bytes 4 and 5)
        uint16_t strength = frame[4] | (frame[5] << 8);

        // Parse Temperature (Bytes 6 and 7)
        float temp = (frame[6] | (frame[7] << 8)) / 8.0 - 256.0;

        // Output the results to the PlatformIO Serial Monitor
        Serial.print("Distance: "); 
        Serial.print(distance); 
        Serial.print(" cm \t");
        
        Serial.print("Strength: "); 
        Serial.print(strength); 
        Serial.print(" \t");
        
        Serial.print("Temp: "); 
        Serial.print(temp); 
        Serial.println(" °C");
      } else {
        Serial.println("Checksum error! Frame dropped.");
      }
    }
  }
}
