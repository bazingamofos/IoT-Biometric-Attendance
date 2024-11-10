# IoT-Biometric-Attendance-System
## v1.0
Automated Biometric Attendance System using ATmega328p microcontroller, R307 fingerprint sensor, Bluetooth module, OLED display, and essential components for seamless user interaction.
<p align="center">
  
  ![Screenshot 2023-12-27 154034](https://github.com/bazingamofos/Biometric-Attendance-System/assets/69231405/763877a2-9be5-462b-9fd7-aca0902ee1bc)
  Hardware Schematic
  
  ![ef1a89c5-4222-4c10-b507-ca81ae5f85cd](https://github.com/bazingamofos/Biometric-Attendance-System/assets/69231405/99f6e2ca-43bd-4a17-8433-674ca3d24192)
  ![IMG_20231226_164131](https://github.com/bazingamofos/Biometric-Attendance-System/assets/69231405/09e9101d-0079-4598-a5cb-14f75e9325a4)
  Hardware Assembly Pictures
</p>

**Project Overview:**
- Built with standalone ATmega328P.
- Bluetooth module connects to the ArduTooth mobile app (Play Store).
- The ArduTooth app displays real-time student entry and exit times.
- TimeLib.h library in Arduino IDE enables time tracking. The sync message is sent by getting the Epoch Timestamp using the conversion tool - https://www.epochconverter.com/
- OLED display, buzzer, and LEDs for a seamless user interface.

## v2.0
<p align="center">
  
  ![WhatsApp Image 2024-11-10 at 11 26 56 PM (2)](https://github.com/user-attachments/assets/1a1b4af2-c057-49f2-ac32-e3eedb9a762e | width=100)
  ![WhatsApp Image 2024-11-10 at 11 26 56 PM (1)](https://github.com/user-attachments/assets/3610b627-3cd9-4aed-978f-bf5775ae44b7 | width=100)
  ![WhatsApp Image 2024-11-10 at 11 26 55 PM](https://github.com/user-attachments/assets/3e02b022-35ee-4b42-8319-aaab341ae15b)
  ![WhatsApp Image 2024-11-10 at 11 26 56 PM](https://github.com/user-attachments/assets/962d7dcc-0c6e-4707-9ea5-e418ab904550)
  Hardware Assembly Pictures
</p>

**Project Overview:**
- Built with ESP32 DevKit, leveraging its WiFi capabilities.
- Fingerprint ID and scan time are recorded and sent directly to Google Sheets.
- A WS2812 RGB LED provides a user-friendly interface with visual cues.
- Student data, including name, registration ID, and institute email, is stored in Firebase Realtime Database as JSON files in the `student-data` directory.
- A `Code.gs` script fetches the associated student data from Firebase based on the scanned fingerprint ID.
- Google Sheets automatically logs each student's entry and exit times for easy tracking and record-keeping.

**Project Status: Ongoing**
- **Remaining Tasks:**
  - Implement functionality to mark students as present/absent based on their in and out times, and calculate their attendance percentage.
  - Develop a mobile application to facilitate easy collection and management of student data.
