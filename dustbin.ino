/*
 * This part of the code operates the components such as 
 * ultrasonic sensors, servo motors that are connected to 
 * to a NODEMCU in the dustbins and also communicate
 * with ThinkSpeak to send percentage of garbage present in 
 * each bin and check whether bot is coming and also arrived 
 * 
 * author(s): sampatghosh, ShaikhSahilAhmed
 */
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <Servo.h> 

//ultrasonic sensor in dustbin 1 
#define TRIGGER1 5
#define ECHO1   4

//ultrasonic sensor in dustbin 2
#define TRIGGER2 14
#define ECHO2  12

Servo motor1; //Servo motor in dustbin 1
Servo motor2; //Servo motor in dustbin 2
 
int flag1;
int flag2;
int flagArrived1;
int flagArrived2;


//-------- Enter Your Wi-Fi Details -----//
char ssid[] = "sahil";  //SSID
char pass[] = "sahilnitk"; //Password
//--------------------------------------//

WiFiClient  client;

unsigned long myChannelField = 00000; // Channel ID
const int ChannelField3 = 3; // Which To Field Write
const int ChannelField4 = 4;
const int ChannelField5 = 5; // Which To Field Write
const int ChannelField6 = 6;
const char * myWriteAPIKey = "Write API Key";
const char * myReadAPIKey = "Read API Key";
String value = "";
  
void setup() {
  Serial.begin (115200);
  Serial.println("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi Connected. Got IP address: ");
  Serial.println(WiFi.localIP());

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  
  pinMode(TRIGGER1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIGGER2, OUTPUT);
  pinMode(ECHO2, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  
  motor1.attach(13);
  motor2.attach(15);
}
 
void loop() {
  
  long duration, distance,bin,p;
  digitalWrite(TRIGGER1, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER1, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER1, LOW);
  duration = pulseIn(ECHO1, HIGH);
  distance = (duration/2) / 29.1;
  delay(100);
  ThingSpeak.writeField(myChannelField, ChannelField3, distance, myWriteAPIKey);
  
  Serial.print("Centimeter FOR UL1:");
  Serial.print(distance);
  Serial.print("\n");
  
  digitalWrite(TRIGGER2, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER2, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER2, LOW);
  duration = pulseIn(ECHO2, HIGH);
  distance = (duration/2) / 29.1;
  delay(100);
  ThingSpeak.writeField(myChannelField, ChannelField4, distance, myWriteAPIKey);
  Serial.print("Centimeter FOR UL2:");
  Serial.print(distance);
  Serial.print("\n");
  
/* 
 * checking if bot is supposed to come and bot came to position
 * if yes then open dustbin shaft using servo to dump waste onto bot
 */
  flag1 = ThingSpeak.readIntField(935348, 1, myReadAPIKey);
  flagArrived1 = ThingSpeak.readIntField(935348, 5, myReadAPIKey);
  flag2 = ThingSpeak.readIntField(935348, 2, myReadAPIKey);
  flagArrived2 = ThingSpeak.readIntField(935348, 6, myReadAPIKey);

  if(flag1 == 1 && flagArrived1 == 1) {
        for(p = 0; p <= 55; p++) {
          motor1.write(p);
          delay(10);
        }
        delay(3000);
        for(p = 55; p >= 0; p--) {
          motor1.write(p);
          delay(10);
        }
        flag1 = 0;
        flagArrived1 = 0;
  }
  if(flag2 == 1 && flagArrived2 == 1) {
        for(p = 0; p <= 55; p++) {
          motor2.write(p);
          delay(10);
        }
        delay(3000);
        for(p = 55; p >= 0; p--) {
          motor2.write(p);
          delay(10);
        }
        flag2 = 0;
        flagArrived2 = 0;
  }
  delay(5000);
}