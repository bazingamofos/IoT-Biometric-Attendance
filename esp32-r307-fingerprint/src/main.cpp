#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <LittleFS.h>
#include <SD.h>
#include <WiFi.h>
#include <Update.h>
#include <Firebase_ESP_Client.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <bitmaps.h>
#include <professors.h>
#include <HTTPClient.h> 
#include <FastLED.h>
#include "time.h"
#include <ArduinoJson.h>

// Global Variables ========================================

int n = 0;
int currentCourseIndex = 0;
bool courseSelected = false;
String selectedCourse = "";
bool professorVerified = false;
bool attendanceMode = false;
int professorID = -1;
int profIndex = -1;  // <-- Added global variable

// WiFi ====================================================

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Supabase ================================================

const char* SUPABASE_URL = "https://your-project.supabase.co"; 
const char* SUPABASE_API_KEY  = "YOUR_SUPABASE_API_KEY";       
String SUPABASE_TABLE_NAME = "LY1_Electronics";

// Google Sheets ===========================================

String GOOGLE_SCRIPT_ID = "YOUR_GOOGLE_SCRIPT_ID";    

// Buzzer ==================================================

int buzzer = 19;

// LED =====================================================

#define LED_PIN     18
#define NUM_LEDS    1

CRGB leds[NUM_LEDS];

// Sensor ==================================================

HardwareSerial serialPort(2); // use UART2
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&serialPort);

// OLED ====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button ==================================================

#define BUTTON_PIN 4
unsigned long buttonPressStart = 0;
bool buttonPressed = false;
bool longPressHandled = false;

// Initialize WiFi
void initWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi ..");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print(F("Connecting to \n"));
    display.setCursor(0, 50);   
    display.setTextSize(2);          
    display.print(WIFI_SSID);
    display.drawBitmap( 73, 10, Wifi_start_bits, Wifi_start_width, Wifi_start_height, WHITE);
    display.display();

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(8, 0);
    display.print(F("Connected \n"));
    display.drawBitmap( 33, 15, Wifi_connected_bits, Wifi_connected_width, Wifi_connected_height, WHITE);
    display.display();

    Serial.println(WiFi.localIP());
    Serial.println();
}

void markAttendance(int fing_id, const String& courseCode) {
    HTTPClient http;
    String endpoint = String(SUPABASE_URL) + "/rest/v1/" + SUPABASE_TABLE_NAME;

    http.begin(endpoint);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", SUPABASE_API_KEY);
    http.addHeader("Authorization", "Bearer " + String(SUPABASE_API_KEY));

    StaticJsonDocument<200> jsonDoc;
    jsonDoc["fing_id"] = fing_id;
    jsonDoc["course_code"] = courseCode;
    jsonDoc["attendance"] = true;

    String requestBody;
    serializeJson(jsonDoc, requestBody);

    int responseCode = http.POST(requestBody);
    Serial.println("Attendance Insert Response Code: " + String(responseCode));
    Serial.println(http.getString());
    http.end();
}

void displayCourses(String courses[], int courseCount, int selectedIndex) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    for (int i = 0; i < courseCount; i++) {
        if (i == selectedIndex) {
            display.fillRect(0, i * 16, 128, 16, WHITE);
            display.setTextColor(BLACK);
            display.setCursor(5, i * 16 + 4);
            display.println(courses[i]);
            display.setTextColor(WHITE);
        } else {
            display.setCursor(5, i * 16 + 4);
            display.println(courses[i]);
        }
    }
    display.display();
}

void handleButton(String courses[], int courseCount) {
    if (digitalRead(BUTTON_PIN) == LOW) {
        if (!buttonPressed) {
            buttonPressed = true;
            buttonPressStart = millis();
            longPressHandled = false;
        } else {
            if (millis() - buttonPressStart > 1000 && !longPressHandled) {
                longPressHandled = true;
                if (!courseSelected) {
                    courseSelected = true;
                    // Get the course code (id), not name
                    selectedCourse = professors[profIndex].courses[currentCourseIndex].id;
                    Serial.println("Selected course: " + selectedCourse);
                }
            }
        }
    } else {
        if (buttonPressed) {
            if (!longPressHandled) {
                currentCourseIndex = (currentCourseIndex + 1) % courseCount;
                displayCourses(courses, courseCount, currentCourseIndex);
            }
            buttonPressed = false;
        }
    }
}

int getFingerprintIDez() {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)
        return 0;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
        return -1;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
        return -1;

    return finger.fingerID;
}

void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    display.clearDisplay();
    initWiFi();

    pinMode(buzzer, OUTPUT);
    digitalWrite(buzzer, 0);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    while (!Serial);
    delay(100);
    Serial.println("\n\nAdafruit finger detect test");

    finger.begin(57600);
    delay(5);
    if (finger.verifyPassword()) {
        Serial.println("Found fingerprint sensor!");
    } else {
        Serial.println("Did not find fingerprint sensor :(");
        while (1) { delay(1); }
    }

    finger.getParameters();
    finger.getTemplateCount();

    if (finger.templateCount == 0) {
        Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
    } else {
        Serial.println("Waiting for valid finger...");
        Serial.print("Sensor contains ");
        Serial.print(finger.templateCount);
        Serial.println(" templates");
    }
}

void loop() {
    if (!courseSelected) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(2, 25); 
        display.println(F("Please scan to start"));
        display.display();

        int profID = getFingerprintIDez();

        if (profID >= 50 && profID <= 53) {
            profIndex = profID - 50;
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(0, 0);
            display.println("Welcome,");
            display.setCursor(0, 16);
            display.setTextSize(2);
            display.println(professors[profIndex].name);
            display.display();
            delay(1500);

            String courseNames[2];
            for (int i = 0; i < 2; i++) {
                courseNames[i] = professors[profIndex].courses[i].name;
            }

            displayCourses(courseNames, 2, currentCourseIndex);
            while (!courseSelected) {
                handleButton(courseNames, 2);
                delay(100);
            }
        } else if (profID > 0) {
            display.clearDisplay();
            display.setCursor(10, 25);
            display.println("Access denied");
            display.display();
            delay(1500);
        }
    } else {
        int scanID = getFingerprintIDez();

        if (scanID == 0){
            display.clearDisplay();
            display.drawBitmap( 32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, WHITE);
            display.display();
        }
        else if (scanID == -1){
            display.clearDisplay();
            display.drawBitmap( 34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, WHITE);
            display.display();
            digitalWrite(buzzer, 1);
            leds[0] = CRGB(255, 0, 0);
            FastLED.show();
            delay(800);
            digitalWrite(buzzer, 0);
            leds[0] = CRGB(0, 0, 0);
            FastLED.show();
        }
        else {
            if (scanID >= 50 && scanID <= 53) {
                courseSelected = false;
                currentCourseIndex = 0;
                selectedCourse = "";
                return;
            }

            Serial.print("Found ID #");
            Serial.print(finger.fingerID);
            Serial.print(" with confidence of ");
            Serial.println(finger.confidence);

            markAttendance(scanID, selectedCourse);

            display.clearDisplay();
            display.drawBitmap( 34, 0, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, WHITE);
            display.display();
            delay(200);

            display.clearDisplay();
            display.setTextSize(1); 
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(5, 5);
            display.println(F("Captured ID"));
            display.setCursor(15, 25);
            display.setTextSize(2);
            display.print(F("#"));
            display.print(finger.fingerID);
            display.display();

            digitalWrite(buzzer, 1);
            leds[0] = CRGB(0, 255, 0);
            FastLED.show();
            delay(100);
            digitalWrite(buzzer, 0);
            delay(300);
            leds[0] = CRGB(0, 0, 0);
            FastLED.show();
        }

        handleButton(nullptr, 0);
    }
}
