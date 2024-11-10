#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <TimeLib.h>
#include <Adafruit_SSD1306.h>
#include <avr/pgmspace.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define TIME_HEADER "T"
#define TIME_REQUEST 7

SoftwareSerial B(9, 10);

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

const char string0[] PROGMEM = "Flavia1";
const char string1[] PROGMEM = "Flavia2";
const char string2[] PROGMEM = "Flavia3";
const char string3[] PROGMEM = "Flavia4";
const char string4[] PROGMEM = "Flavia5";

const char* const name[] PROGMEM = {string0, string1, string2, string3, string4};

byte u[5] = {0, 0, 0, 0, 0};
byte id;
char result1[11]; // Buffer for date (MM/DD/YYYY)
char result2[9];  // Buffer for time (HH:MM:SS)

byte buzz = 8;
byte green = 7;
byte red = 6;

void setup() {
  Serial.begin(9600);
  pinMode(buzz, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  B.begin(9600);
  finger.begin(57600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) {
    } // Don't proceed, loop forever
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  while (!Serial)
    ;
  setSyncProvider(requestSync);
  Serial.println(F("Waiting for sync message"));
  if (Serial.available()) {
    processSyncMessage();
  }
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    if (pctime >= DEFAULT_TIME) {
      setTime(pctime);
    }
  }
}

time_t requestSync() {
  Serial.write(TIME_REQUEST);
  return 0;
}

void loop() {
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);

  mySerial.listen();
  int p = -1;
  int p1 = -1;

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(5, 10);
    display.println(F("Waiting..."));
    display.setTextSize(1);
    display.setCursor(10, 40);
    display.print(F("Total enrolled: "));
    finger.getTemplateCount();
    display.print(finger.templateCount);
    display.display();

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    p1 = finger.fingerSearch();
    if (p == FINGERPRINT_OK && finger.image2Tz() == FINGERPRINT_OK && p1 == FINGERPRINT_OK) {
      digitalWrite(green, HIGH);
      id = finger.fingerID;
      
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(10, 10);
      display.println(F("Valid ID!"));
      display.setCursor(30, 35);
      display.print(F("ID #"));
      display.print(finger.fingerID);
      display.display();

      if (Serial.available()) {
        processSyncMessage();
      }
      if (timeStatus() != timeNotSet) {
        B.listen();
        int d = day();
        int m = month();
        int y = year();
        snprintf_P(result1, sizeof(result1), PSTR("%02d/%02d/%04d"), d, m, y);

        int h = hour();
        int min = minute();
        int sec = second();
        snprintf_P(result2, sizeof(result2), PSTR("%02d:%02d:%02d"), h, min, sec);
      }
      B.print(result1);
      B.print(F(","));
      B.print(result2);
      B.print(F(","));
      B.print(reinterpret_cast<const __FlashStringHelper *>(pgm_read_word(&(name[id - 1]))));
      B.print(F(","));
      B.print(id);
      B.print(F(","));
      if (u[id - 1] == 0) {
        B.print(F("In"));
        u[id - 1] = 1;
      } else if (u[id - 1] == 1) {
        B.print(F("Out"));
        u[id - 1] = 0;
      }

      B.print(F(";"));
      digitalWrite(buzz, HIGH);
      delay(50);
      digitalWrite(buzz, LOW);
      delay(950);
    } else if (p == FINGERPRINT_OK && finger.image2Tz() == FINGERPRINT_OK && p1 == FINGERPRINT_NOTFOUND) {
      digitalWrite(red, HIGH);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(20, 25);
      display.println(F("Invalid"));
      display.display();
      digitalWrite(buzz, HIGH);
      delay(1000);
      digitalWrite(buzz, LOW);
    }
  }
  delay(50);
}
