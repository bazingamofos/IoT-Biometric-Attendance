# IoT-Biometric-Attendance-System
## v1.0
Automated Biometric Attendance System using ATmega328p microcontroller, R307 fingerprint sensor, Bluetooth module, OLED display, and essential components for seamless user interaction.
<p align="center">

  ![Screenshot 2023-12-27 154034](https://github.com/bazingamofos/Biometric-Attendance-System/assets/69231405/763877a2-9be5-462b-9fd7-aca0902ee1bc)
  
  Hardware Schematic

  <img src="https://github.com/bazingamofos/Biometric-Attendance-System/assets/69231405/99f6e2ca-43bd-4a17-8433-674ca3d24192" alt="" width="600">
  <img src="https://github.com/bazingamofos/Biometric-Attendance-System/assets/69231405/09e9101d-0079-4598-a5cb-14f75e9325a4" alt="" width="600">
  
  Hardware Assembly Pictures
</p>

**Project Overview:**
- Built with standalone ATmega328P.
- Bluetooth module connects to the ArduTooth mobile app (Play Store).
- The ArduTooth app displays real-time student entry and exit times.
- TimeLib.h library in Arduino IDE enables time tracking. The sync message is sent by getting the Epoch Timestamp using the conversion tool - https://www.epochconverter.com/
- OLED display, buzzer, and LEDs for a seamless user interface.
##
## v2.0

![image](https://github.com/user-attachments/assets/022fed23-dc00-4d05-8ee6-d194c4fe99ee)

Hardware Schematic

<img src="https://github.com/user-attachments/assets/1a1b4af2-c057-49f2-ac32-e3eedb9a762e" alt="" width="600">
<img src="https://github.com/user-attachments/assets/3610b627-3cd9-4aed-978f-bf5775ae44b7" alt="" width="600">
<img src="https://github.com/user-attachments/assets/3e02b022-35ee-4b42-8319-aaab341ae15b" alt="" width="600">
<img src="https://github.com/user-attachments/assets/962d7dcc-0c6e-4707-9ea5-e418ab904550" alt="" width="600">

Hardware Assembly Pictures

**Project Overview:**
- Built with ESP32 DevKit, leveraging its WiFi capabilities.
- Fingerprint ID and scan time are recorded and sent directly to Google Sheets.
- A WS2812 RGB LED provides a user-friendly interface with visual cues.
- Student data, including name, registration ID, and institute email, is stored in Firebase Realtime Database as JSON files in the `student-data` directory.
- A `Code.gs` script fetches the associated student data from Firebase based on the scanned fingerprint ID.
- Google Sheets automatically logs each student's entry and exit times for easy tracking and record-keeping.

<img src="https://github.com/user-attachments/assets/c35b042a-65af-4c45-a962-91799a8bce89" alt="">

##

---

# IoT Attendance System - Google Sheets Integration

This project automates the process of updating attendance in a Google Sheet based on data received from an IoT sensor. The data, such as timestamps and sensor readings, is parsed, and the corresponding sheet entries are updated accordingly. The project also synchronizes student data from a Firebase database to Google Sheets for better tracking of student attendance.

## Features

- **Attendance Update**: The system updates attendance based on sensor data, marking students as present or absent based on in-time and out-time.
- **Student Information Sync**: It syncs student information (name, student ID, email) from Firebase to Google Sheets.
- **Timely Trigger**: The `doPost` function is triggered automatically every minute to update the attendance status.

## Project Structure

- **Google Sheet**: The attendance data is maintained in a Google Sheet with the following columns:
  - Column A: Fingerprint ID
  - Column B: Student Name
  - Column C: Student ID
  - Column D: Student Email
  - Column E: In-time
  - Column F: Out-time
  - Additional columns represent attendance for specific dates.
- **Google Apps Script**: The script handles the parsing of the sensor data and updates the attendance status in the sheet.
- **Firebase Database**: Syncs student data (name, student ID, and email) from Firebase to Google Sheets.

## How It Works

1. **Attendance Recording**:
   - The system receives sensor data (such as sensor ID, time, and date) through a GET or POST request.
   - The `doPost` function is triggered by an external event (such as a button press or sensor data collection) and updates the attendance status.
   - The system checks if the student has already been recorded for that day and updates their attendance accordingly (in-time, out-time).

2. **Student Information Sync**:
   - The system fetches student data from Firebase and updates the corresponding student details in the Google Sheet.

3. **Timely Trigger**:
   - A trigger is set to automatically call the `doPost` function every minute, ensuring attendance data is updated periodically.
  
---

## Demo

Here is a demonstration video of the IoT Attendance System in action:

[![Demo Video](https://img.youtube.com/vi/yBd02_0_lxA/0.jpg)](https://www.youtube.com/watch?v=yBd02_0_lxA)

Click on the image to watch the demo video.

---


## Google Apps Script

The Google Apps Script file handles the following operations:

- **`updateEntry(e)`**: Parses the incoming data (date, time, sensor ID) and updates the attendance status in the Google Sheet.
- **`updateGoogleSheetsWithStudentData()`**: Syncs student data from Firebase to Google Sheets.
- **`doPost(e)`**: This function is triggered every minute by a time-driven trigger and sends a POST request to update attendance.

## Setup Instructions

### 1. **Google Sheets Setup**:
   - Create a Google Sheet with the necessary columns: Fingerprint ID, Student Name, Student ID, Student Email, In-time, Out-time, and attendance columns for dates.
   - Note down the `Sheet ID` (found in the URL of your Google Sheet) and replace `"YOUR_SHEET_ID"` in the script with the actual Sheet ID.

### 2. **Firebase Setup**:
   - Create a Firebase project and store student data in the database. The student data should include `fing_id`, `name`, `stud_id`, and `stud_email`.
   - Replace `"YOUR_FIREBASE_URL"` in the script with your Firebase database URL.

### 3. **Ngrok Setup**:
   - Ngrok is used for creating a secure tunnel to your local development environment, so the `doPost` function can send data to an external URL.
   - Replace `"YOUR_NGROK_URL"` in the script with your Ngrok URL.

### 4. **Setting up Google Apps Script Trigger**:
   - In the Google Apps Script editor, go to **Triggers** (clock icon) in the sidebar.
   - Create a new trigger for the `doPost` function, set it to run **Time-driven** every **minute**.
   - ![image](https://github.com/user-attachments/assets/b08b1d31-1425-492d-95a5-5470dcdb719a)


### 5. **Deploying the Script**:
   - After setting up the script, deploy it as a web app with appropriate permissions to allow external requests to interact with it.
   - Ensure that the script is authorized to access both Google Sheets and Firebase.

## Requirements

1. **Google Sheets API**
2. **Firebase Realtime Database**
3. **Ngrok for local tunneling**
4. **Google Apps Script**

### Google Apps Script Permissions
Make sure that the Google Apps Script has the necessary permissions to:
- Read and update Google Sheets.
- Make requests to external URLs (Firebase, Ngrok).

## Example Workflow

1. A student scans their fingerprint using the IoT sensor, triggering a request with their sensor ID and time.
2. The `doPost` function is called, and attendance is updated in Google Sheets for the student.
3. Every minute, the `doPost` function is triggered to ensure attendance data is kept up to date.

## License

This project is licensed under the **GNU General Public License** (GPL) Version 3, 29 June 2007.

See the [LICENSE](LICENSE) file for more details.
