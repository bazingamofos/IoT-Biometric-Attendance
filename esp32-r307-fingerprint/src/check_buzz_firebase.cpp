// #include <Arduino.h>
// #include <Adafruit_Fingerprint.h>
// #include <LittleFS.h>
// #include <SD.h>
// #include <WiFi.h>
// #include <Update.h>
// #include <Firebase_ESP_Client.h>
// #include "time.h"

// //Provide the token generation process info.
// #include "addons/TokenHelper.h"
// //Provide the RTDB payload printing info and other helper functions.
// #include "addons/RTDBHelper.h"

// // Replace with your Firebase project URL and API key
// #define DATABASE_URL "DATABASE_URL"
// #define API_KEY "API_KEY"

// // Replace with your Wi-Fi SSID and password
// #define WIFI_SSID "WIFI_SSID"
// #define WIFI_PASSWORD "WIFI_PASSWORD"

// // Insert Authorized Email and Corresponding Password
// #define USER_EMAIL "USER_EMAIL"
// #define USER_PASSWORD "USER_PASSWORD"

// // Define FirebaseESP32 data object
// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;
// FirebaseJson json;

// String uid;
// String databasePath;
// String tempPath = "/rollno";
// String timePath = "/timestamp";

// // Parent Node (to be updated in every loop)
// String parentPath;
// int timestamp;

// const char* ntpServer = "pool.ntp.org";

// int buzzer = 19;

// HardwareSerial serialPort(2); // use UART2
// Adafruit_Fingerprint finger = Adafruit_Fingerprint(&serialPort);

// uint8_t getFingerprintID();

// // Initialize WiFi
// void initWiFi() {
//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     Serial.print("Connecting to WiFi ..");
//     while (WiFi.status() != WL_CONNECTED) {
//         Serial.print('.');
//         delay(1000);
//     }
//     Serial.println(WiFi.localIP());
//     Serial.println();
// }

// // Function that gets current epoch time
// unsigned long getTime() {
//     time_t now;
//     struct tm timeinfo;
//     if (!getLocalTime(&timeinfo)) {
//         //Serial.println("Failed to obtain time");
//         return(0);
//     }
//     time(&now);
//     return now;
// }

// void setup()
// {
//     Serial.begin(115200);

//     initWiFi();
//     configTime(0, 0, ntpServer);

//     // Assign the api key (required)
//     config.api_key = API_KEY;

//     // Assign the user sign in credentials
//     auth.user.email = USER_EMAIL;
//     auth.user.password = USER_PASSWORD;

//     // Assign the RTDB URL (required)
//     config.database_url = DATABASE_URL;

//     Firebase.reconnectWiFi(true);
//     fbdo.setResponseSize(4096);

//     // Assign the callback function for the long running token generation task */
//     config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

//     // Assign the maximum retry of token generation
//     config.max_token_generation_retry = 5;

//     // Initialize the library with the Firebase authen and config
//     Firebase.begin(&config, &auth);

//     // Getting the user UID might take a few seconds
//     Serial.println("Getting User UID");
//     while ((auth.token.uid) == "") {
//         Serial.print('.');
//         delay(1000);
//     }
//     // Print user UID
//     uid = auth.token.uid.c_str();
//     Serial.print("User UID: ");
//     Serial.println(uid);

//     // Update database path
//     databasePath = "/UsersData/" + uid + "/readings";

//     pinMode(buzzer, OUTPUT);
//     digitalWrite(buzzer, 1);

//     while (!Serial)
//         ; // For Yun/Leo/Micro/Zero/...
//     delay(100);
//     Serial.println("\n\nAdafruit finger detect test");

//     // set the data rate for the sensor serial port
//     finger.begin(57600);
//     delay(5);
//     if (finger.verifyPassword())
//     {
//         Serial.println("Found fingerprint sensor!");
//     }
//     else
//     {
//         Serial.println("Did not find fingerprint sensor :(");
//         while (1)
//         {
//             delay(1);
//         }
//     }

//     Serial.println(F("Reading sensor parameters"));
//     finger.getParameters();
//     Serial.print(F("Status: 0x"));
//     Serial.println(finger.status_reg, HEX);
//     Serial.print(F("Sys ID: 0x"));
//     Serial.println(finger.system_id, HEX);
//     Serial.print(F("Capacity: "));
//     Serial.println(finger.capacity);
//     Serial.print(F("Security level: "));
//     Serial.println(finger.security_level);
//     Serial.print(F("Device address: "));
//     Serial.println(finger.device_addr, HEX);
//     Serial.print(F("Packet len: "));
//     Serial.println(finger.packet_len);
//     Serial.print(F("Baud rate: "));
//     Serial.println(finger.baud_rate);

//     finger.getTemplateCount();

//     if (finger.templateCount == 0)
//     {
//         Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
//     }
//     else
//     {
//         Serial.println("Waiting for valid finger...");
//         Serial.print("Sensor contains ");
//         Serial.print(finger.templateCount);
//         Serial.println(" templates");
//     }
// }

// void loop()
// {
//     getFingerprintID();
//     delay(50); // don't ned to run this at full speed.
// }

// uint8_t getFingerprintID()
// {
//     uint8_t p = finger.getImage();
//     switch (p)
//     {
//     case FINGERPRINT_OK:
//         Serial.println("Image taken");
//         break;
//     case FINGERPRINT_NOFINGER:
//         Serial.println("No finger detected");
//         return p;
//     case FINGERPRINT_PACKETRECIEVEERR:
//         Serial.println("Communication error");
//         return p;
//     case FINGERPRINT_IMAGEFAIL:
//         Serial.println("Imaging error");
//         return p;
//     default:
//         Serial.println("Unknown error");
//         return p;
//     }

//     // OK success!

//     p = finger.image2Tz();
//     switch (p)
//     {
//     case FINGERPRINT_OK:
//         Serial.println("Image converted");
//         break;
//     case FINGERPRINT_IMAGEMESS:
//         Serial.println("Image too messy");
//         return p;
//     case FINGERPRINT_PACKETRECIEVEERR:
//         Serial.println("Communication error");
//         return p;
//     case FINGERPRINT_FEATUREFAIL:
//         Serial.println("Could not find fingerprint features");
//         return p;
//     case FINGERPRINT_INVALIDIMAGE:
//         Serial.println("Could not find fingerprint features");
//         return p;
//     default:
//         Serial.println("Unknown error");
//         return p;
//     }

//     // OK converted!
//     p = finger.fingerSearch();
//     if (p == FINGERPRINT_OK)
//     {
//         Serial.println("Found a print match!");
//     }
//     else if (p == FINGERPRINT_PACKETRECIEVEERR)
//     {
//         Serial.println("Communication error");
//         return p;
//     }
//     else if (p == FINGERPRINT_NOTFOUND)
//     {
//         Serial.println("Did not find a match");
//         digitalWrite(buzzer, 0);
//         delay(800);
//         digitalWrite(buzzer, 1);
//         return p;
//     }
//     else
//     {
//         Serial.println("Unknown error");
//         return p;
//     }

//     // found a match!
//     Serial.print("Found ID #");
//     Serial.print(finger.fingerID);
//     Serial.print(" with confidence of ");
//     Serial.println(finger.confidence);
//     digitalWrite(buzzer, 0);
//     delay(100);
//     digitalWrite(buzzer, 1);
//     // Send new readings to database
//     if (Firebase.ready()){

//         //Get current timestamp
//         timestamp = getTime();
//         Serial.print ("time: ");
//         Serial.println (timestamp);

//         parentPath= databasePath + "/" + String(timestamp);

//         json.set(tempPath.c_str(), String(finger.fingerID));
//         json.set(timePath, String(timestamp));
//         Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
//     }


//     return finger.fingerID;
// }

// // returns -1 if failed, otherwise returns ID #
// int getFingerprintIDez()
// {
//     uint8_t p = finger.getImage();
//     if (p != FINGERPRINT_OK)
//         return -1;

//     p = finger.image2Tz();
//     if (p != FINGERPRINT_OK)
//         return -1;

//     p = finger.fingerFastSearch();
//     if (p != FINGERPRINT_OK)
//         return -1;

//     // found a match!
//     Serial.print("Found ID #");
//     Serial.print(finger.fingerID);
//     Serial.print(" with confidence of ");
//     Serial.println(finger.confidence);
//     return finger.fingerID;
// }
