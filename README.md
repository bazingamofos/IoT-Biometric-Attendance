# IoT-Biometric-Attendance-System
Automated Biometric Attendance System using ATmega328p microcontroller, R307 fingerprint sensor, Bluetooth module, OLED display, and essential components for seamless user interaction.
<p align="center">
  
  ![Screenshot 2023-12-27 154034](https://github.com/bazingamofos/Biometric-Attendance-System/assets/69231405/763877a2-9be5-462b-9fd7-aca0902ee1bc)
  Hardware Schematic
  
  ![ef1a89c5-4222-4c10-b507-ca81ae5f85cd](https://github.com/bazingamofos/Biometric-Attendance-System/assets/69231405/99f6e2ca-43bd-4a17-8433-674ca3d24192)
  ![IMG_20231226_164131](https://github.com/bazingamofos/Biometric-Attendance-System/assets/69231405/09e9101d-0079-4598-a5cb-14f75e9325a4)
  Hardware Assembly Pictures
</p>

Project Overview:
- Built with standalone ATmega328P.
- Bluetooth module connects to the ArduTooth mobile app (Play Store).
- The ArduTooth app displays real-time student entry and exit times.
- TimeLib.h library in Arduino IDE enables time tracking. The sync message is sent by getting the Epoch Timestamp using the conversion tool - https://www.epochconverter.com/
- OLED display, buzzer, and LEDs for a seamless user interface.
