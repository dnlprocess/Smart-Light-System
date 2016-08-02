#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

const int sensorMin = 25;
const int sensorMax = 230;

int distanceChange = 0;

const long nightMovementTime = 11 * 1000;
const long nightStartDuration = 8 * 1000;
int nightTimeSet = 0;
int nightMovement = 0;
const long nightDuration = 10 * 1000;
long nightTime = -10000000;
long nightTime_2 = -10000000;
long nightTime_3 = -10000000;
long startTime = -10000000;
long currentMillis = 0;
const long lightDuration = 10 * 1000;

const int second = 1000;
int lastChangeTime = 0;

const int threshold = 10;

int distance = 0;
int prevDistance = -100;

int ledPin1 = 13;
int ledPin2 = 12;
int ledPin3 = 11;
int ledPin4 = 10;
int ledPin5 = 9;
int ledPin6 = 8;
int ledPin7 = 7;
int ledPin8 = 6;
int ledPin9 = 5;
int ledPin10 = 4;
int ledPin11 = 3;
int ledPin12 = 2;

float Inch = 0.00;
//float cm = 0.00;
int SonarPin = A1;
int  sensorValue;
void setup() {

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
  pinMode(ledPin7, OUTPUT);
  pinMode(ledPin8, OUTPUT);
  pinMode(ledPin9, OUTPUT);
  pinMode(ledPin10, OUTPUT);
  pinMode(ledPin11, OUTPUT);
  pinMode(ledPin12, OUTPUT);

  pinMode(SonarPin, INPUT);

  Serial.begin(57600);
  Wire.begin();
  RTC.begin();

  //if (! RTC.isrunning()) {
  //Serial.println("RTC is NOT running!");
  // following line sets the RTC to the date & time this sketch was compiled
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  //}

}
void loop() {

  DateTime now = RTC.now();
  DateTime future (now.unixtime() + 7 * 86400L + 30);

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  currentMillis = millis();

  sensorValue = analogRead(SonarPin);

  Inch = (sensorValue * 0.497);
  //cm = Inch * 2.54;

  distance = Inch;

  //Serial.println(sensorValue);
  Serial.println(Inch);
  Serial.println("inch");

  //Serial.print(cm);
  //Serial.println("cm");

  int sensorReading = analogRead(A0);
  //Serial.println("light:");
  //Serial.print(sensorReading);

  int range = map(sensorReading, sensorMin, sensorMax, 0, 6);

  distanceChange = abs(distance - prevDistance);

  if ((now.hour() == 8) && ((now.minute() >= 0) && (now.minute() <= 15))) {
    level_1();
  }
  else {

    if ((now.hour() >= 0) && (now.hour() <= 6)) {
      if ((distanceChange >= threshold) && (prevDistance != -100) && (nightTimeSet == false)) {
        nightTime = millis();
        nightTimeSet = true;
      }
      if (currentMillis - nightTime <= nightDuration) {
        Serial.println("Night level 4");
        level_4();
        if ((currentMillis - nightTime >= nightStartDuration) && (currentMillis - nightTime <= nightDuration)) {
          if (distanceChange >= threshold && prevDistance != -100) {
            nightMovement = true;

            // Problem: after 10 minutes the light will still be 1/2 and if I tell it to turn off it wont continue because,
            // the part below still happens in the first 10 minutes.
            // Possible solution: make 1/2 light turn off after 10 minutes and below turn on after 10 min, 10 sec.

          }
        }
      }
      else {
        if (currentMillis - nightTime <= nightMovementTime) {
          level_7();
        }
      }
      if (nightMovement == true && currentMillis - nightTime >= nightMovementTime) {
        nightMovement = false;
        nightTime_2 = millis();
      }
      if (currentMillis - nightTime >= nightMovementTime) {
        if (currentMillis - nightTime_2 <= nightDuration) {
          level_1();
          if (distanceChange >= threshold && prevDistance != -100) {
            nightTime_2 = millis();
          }
        }
        else {
          level_7();
          nightTimeSet = false;
        }
      }
    }
    else {

      nightTimeSet = false;

      if (distanceChange >= threshold && prevDistance != -100) {
        startTime = millis();
      }

      if (currentMillis - startTime <= lightDuration) {
        if (currentMillis - lastChangeTime >= second) {
          switch
          (range) {
            case 0:
              level_1();
              break;
            case 1:
              level_2();
              break;
            case 2:
              level_3();
              break;
            case 3:
            Serial.println("normal level 4");
              level_4();
              break;
            case 4:
              level_5();
              break;
            case 5:
              //Serial.println("case 5");
              level_6();
              break;
            case 6:
              level_7();
              break;
          }
          lastChangeTime = millis();
        }
      }
      else {

        level_7();
      }
    }
  }


  prevDistance = distance;
  distanceChange = 0;
  delay(100);
}

void level_1() {

  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  digitalWrite(ledPin4, HIGH);
  digitalWrite(ledPin5, HIGH);
  digitalWrite(ledPin6, HIGH);
  digitalWrite(ledPin7, HIGH);
  digitalWrite(ledPin8, HIGH);
  digitalWrite(ledPin9, HIGH);
  digitalWrite(ledPin10, HIGH);
  digitalWrite(ledPin11, HIGH);
  digitalWrite(ledPin12, HIGH);
}

void level_2() {

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, HIGH);
  digitalWrite(ledPin4, HIGH);
  digitalWrite(ledPin5, HIGH);
  digitalWrite(ledPin6, HIGH);
  digitalWrite(ledPin7, HIGH);
  digitalWrite(ledPin8, HIGH);
  digitalWrite(ledPin9, HIGH);
  digitalWrite(ledPin10, HIGH);
  digitalWrite(ledPin11, HIGH);
  digitalWrite(ledPin12, HIGH);
}

void level_3() {

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, HIGH);
  digitalWrite(ledPin6, HIGH);
  digitalWrite(ledPin7, HIGH);
  digitalWrite(ledPin8, HIGH);
  digitalWrite(ledPin9, HIGH);
  digitalWrite(ledPin10, HIGH);
  digitalWrite(ledPin11, HIGH);
  digitalWrite(ledPin12, HIGH);
}

void level_4() {

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, LOW);
  digitalWrite(ledPin6, LOW);
  digitalWrite(ledPin7, HIGH);
  digitalWrite(ledPin8, HIGH);
  digitalWrite(ledPin9, HIGH);
  digitalWrite(ledPin10, HIGH);
  digitalWrite(ledPin11, HIGH);
  digitalWrite(ledPin12, HIGH);
}

void level_5() {

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, LOW);
  digitalWrite(ledPin6, LOW);
  digitalWrite(ledPin7, LOW);
  digitalWrite(ledPin8, LOW);
  digitalWrite(ledPin9, HIGH);
  digitalWrite(ledPin10, HIGH);
  digitalWrite(ledPin11, HIGH);
  digitalWrite(ledPin12, HIGH);
}

void level_6() {

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, LOW);
  digitalWrite(ledPin6, LOW);
  digitalWrite(ledPin7, LOW);
  digitalWrite(ledPin8, LOW);
  digitalWrite(ledPin9, LOW);
  digitalWrite(ledPin10, LOW);
  digitalWrite(ledPin11, HIGH);
  digitalWrite(ledPin12, HIGH);
}

void level_7() {

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
  digitalWrite(ledPin5, LOW);
  digitalWrite(ledPin6, LOW);
  digitalWrite(ledPin7, LOW);
  digitalWrite(ledPin8, LOW);
  digitalWrite(ledPin9, LOW);
  digitalWrite(ledPin10, LOW);
  digitalWrite(ledPin11, LOW);
  digitalWrite(ledPin12, LOW);
}
