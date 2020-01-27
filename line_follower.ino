/*
 * This part of the code controls the line follower and 
 * also sends and receive data from ThinkSpeak
 * and dumps waste at dumping ground by opening the 
 * shaft of the collector using a servo motor 
 * which is connected to a Arduino Yun mounted on the bot
 */
#include "ThingSpeak.h"
#include <BridgeClient.h>
#include <Servo.h>
//
//for Line Following Robot using 2IR sensors
int lm1 = 8; //left motor output 1
int lm2 = 9; //left motor output 2
int rm1 = 10; //right motor output 1
int rm2 = 11; //right motor output 2
int sl = 13;  //sensor 1 input (left)
int sr = 12;  //sensor 2 input (right)
int SlV = 0;
int SrV = 0;
int led = A0;
int flag1 = 0;
int flag2 = 0;
int c = 0; 
/*
 * c == 0 -> bot at dumping ground
 * c == 1 -> bot at dustbin 1
 * c == 2 -> bot at dustbin 2
 */
int p = 0;
String myStatus = "";
Servo motor;
BridgeClient client;
unsigned long myChannelNumber = 000000; // Enter your channel number here 
const char * myWriteAPIKey = "Enter your Write API key here";

void setup() {
  Serial.begin(115200);  //Initialize serial
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(sl, INPUT);
  pinMode(sr, INPUT);
  motor.attach(3);
  Bridge.begin();
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  sTOP();
}
void loop() {
  SlV = digitalRead(sl);
  SrV = digitalRead(sr);
  if (SrV == HIGH && SlV == HIGH) {
    ForWard();
  }
  if (SrV == HIGH  && SlV == LOW) {
    Left();
  }
  if (SrV == LOW && SlV == HIGH) {
    Right();
  }
  if (SrV == LOW && SlV == LOW) { //bot stops at either a station or dumping yard
    if (c == 0) {
      c = c + 1;
      for (p = 0; p < 55; p++) {
        motor.write(p);
        delay(10);
      }
      delay(5000);
      for (p = 55; p >= 1; p--) {
        motor.write(p);
        delay(10);
      }
      float bin1 = ThingSpeak.readFloatField(myChannelNumber, 3);
      if (bin1 >= 0.2) {
        flag1 = 1;
      }
      ThingSpeak.setField(1, flag1);
      ThingSpeak.setField(2, flag2);
      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      sTOP();
    }
    else if (c == 1) {
      c = c + 1;
      float bin2 = ThingSpeak.readFloatField(myChannelNumber, 4);
      if (bin2 >= 0.2) {
        flag2 = 1;
      }
      ThingSpeak.setField(2, flag2);
      if (flag1 == 1) {
        flag1 = 0;
        ThingSpeak.setField(1, flag1);
        ThingSpeak.setField(5, 1);
        ThingSpeak.setField(6, 0);
        int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
        sTOP();
      }

    }
    else if (c == 2) {
      c = 0;
      if (flag2 == 1) {
        flag2 = 0;
        ThingSpeak.setField(1, flag1);
        ThingSpeak.setField(5, 0);
        ThingSpeak.setField(6, 1);
        int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      }
      sTOP();
    }
  }
}
void ForWard() {
  digitalWrite(lm1, HIGH);
  digitalWrite(lm2, LOW);
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
}
void BackWard() {
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, HIGH);
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, HIGH);
}
void Left() {
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, HIGH);
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
}
void Right() {
  digitalWrite(lm1, HIGH);
  digitalWrite(lm2, LOW);
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, HIGH);
}
void sTOP() {
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, LOW);
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, LOW);
  delay(5000);
  //  to help the bot move forward after seeing a black cross
  while (!(digitalRead(sl) == HIGH && digitalRead(sr) == HIGH)) {
    digitalWrite(lm1, HIGH);
    digitalWrite(lm2, LOW);
    digitalWrite(rm1, HIGH);
    digitalWrite(rm2, LOW);
  }
}
