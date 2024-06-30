//libraries to be used
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>


//initializing Global Variables
int RedLED_1 = 53;
int RedLED_2 = 49;
int YellowLED_1 = 51;
int YellowLED_2 = 47;

int RELAY_1 = 41;
int RELAY_2 = 39;

int CurrentSensor_1 = A2;
int CurrentSensor_2 = A3;

const int Num_readings = 500;
long int sensor_value = 0;  

float voltage = 0;
float current = 0;

int days_1 = 0;
int hours_1 = 0;
int minutes_1 = 0;
int seconds_1 = 0;

int days_2 = 0;
int hours_2 = 0;
int minutes_2 = 0;
int seconds_2 = 0;

//Relay 1 Time Schedules
const unsigned long RELAY_1_TIME_1 = 60; //  1 minutes in seconds
const unsigned long RELAY_1_TIME_2 = 120; // 2 minutes in seconds
const unsigned long RELAY_1_TIME_3 = 180; // 3 minutes in seconds

int State_1 = 0;
int State_2 = 0;

unsigned long remainingSeconds_1 = 0;
unsigned long remainingSeconds = 0;

// Relay 2 Time Schedules
const unsigned long RELAY_2_TIME_1 = 60; //  1 minutes in seconds
const unsigned long RELAY_2_TIME_2 = 120; // 2 minutes in seconds
const unsigned long RELAY_2_TIME_3 = 180; // 3 minutes in seconds

unsigned long lastRelay1OnTime = 0;
unsigned long lastRelay2OnTime = 0;

bool relay1CountdownInProgress = false; //Checks if Relay 1 countdown time is running
bool relay2CountdownInProgress = false; //Checks if Relay 2 countdown time is running

//Create software serial object to communicate with LCD
LiquidCrystal_I2C lcd(0x27, 16, 4); // Set the LCD address to 0x27 for a 16 chars and 2 line display

RTC_DS3231 rtc; //declaring object for DS3231

char daysOfTheWeek[7][12] = {"sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Sartuday"};



void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  rtc.begin();
  
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // February 14, 2023 at  1713hrs would call:
    //rtc.adjust(DateTime(2023, 2, 14, 17, 13, 0));
  }

  pinMode(RedLED_1, OUTPUT);
  pinMode(RedLED_2, OUTPUT);
  pinMode(YellowLED_1, OUTPUT);
  pinMode(YellowLED_2, OUTPUT);
  
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
   
  digitalWrite(RedLED_1, HIGH);
  digitalWrite(RedLED_2, HIGH);
  digitalWrite(YellowLED_1, LOW);
  digitalWrite(YellowLED_2, LOW);
  
  lcd.init();
  lcd.backlight();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SYSTEM BOOTING..");
  lcd.setCursor(0,1);
  lcd.print("BT  TEST: ");
  lcd.setCursor(-4,2);
  lcd.print("RTC TEST: ");
  lcd.setCursor(-1,3);
  lcd.print(" Loading...");
  delay(2000);
  /*while(!mySerial.available()){
    //mySerial.println("AT");
    lcd.setCursor(10,1);
    lcd.print("Wait");
    delay(1000);
  }
  while(rtc){
    lcd.setCursor(6,2);
    lcd.print("Wait");
    delay(1000);
  }*/
  lcd.setCursor(10,1);
  lcd.print("OK");
  lcd.setCursor(6,2);
  lcd.print("OK");
  delay(2000);

  lcd.clear();
  lcd.setCursor(5,1);
  lcd.print("READY");
  delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(RELAY_1) == HIGH){
    digitalWrite(RedLED_1, LOW);
    digitalWrite(YellowLED_1, HIGH);
  }else if(digitalRead(RELAY_1) == LOW){
    digitalWrite(RedLED_1, HIGH);
    digitalWrite(YellowLED_1, LOW);
  }

  if(digitalRead(RELAY_2) == HIGH){
    digitalWrite(RedLED_2, LOW);
    digitalWrite(YellowLED_2, HIGH);
  }else if(digitalRead(RELAY_2) == LOW){
    digitalWrite(RedLED_2, HIGH);
    digitalWrite(YellowLED_2, LOW);
  }
  
  /*for (int i = 0; i < Num_readings; i++)
  {
    sensor_value += analogRead(CurrentSensor_1);
    delay(2);
  }

  sensor_value = sensor_value / Num_readings;
  voltage = sensor_value * 5.0 / 1024.0;
  current = (voltage - 2.5) / 0.185;*/
  
  if (Serial.available()) {
    // Read the incoming data from the Bluetooth module
    char incomingData = Serial.read();
    
    // If the incoming data is '1', turn on relay 1
    if (incomingData == '1' && !relay1CountdownInProgress && State_1 == 0) {
      digitalWrite(RELAY_1, HIGH);
      lastRelay1OnTime = rtc.now().unixtime(); // store the current time in seconds
      Serial.println("Relay 1 turned on");
      State_1 = 1;
    }
    // If the incoming data is '2', turn on relay 1
    else if (incomingData == '2' && !relay1CountdownInProgress && State_1 == 0) {
      digitalWrite(RELAY_1, HIGH);
      lastRelay1OnTime = rtc.now().unixtime(); // store the current time in seconds
      Serial.println("Relay 1 turned on");
      State_1 = 2;
    }
    // If the incoming data is '3', turn on relay 1
    else if (incomingData == '3' && !relay1CountdownInProgress && State_1 == 0) {
      digitalWrite(RELAY_1, HIGH);
      lastRelay1OnTime = rtc.now().unixtime(); // store the current time in seconds
      Serial.println("Relay 2 turned on");
      State_1 = 3;
    }
    // If the incoming data is '4', turn on relay 2
    else if (incomingData == '4' && !relay2CountdownInProgress && State_2 == 0) {
      digitalWrite(RELAY_2, HIGH);
      lastRelay2OnTime = rtc.now().unixtime(); // store the current time in seconds
      Serial.println("Relay 2 turned on");
      State_2 = 1;
    }
    // If the incoming data is '5', turn on relay 2
    else if (incomingData == '5' && !relay2CountdownInProgress && State_2 == 0) {
      digitalWrite(RELAY_2, HIGH);
      lastRelay2OnTime = rtc.now().unixtime(); // store the current time in seconds
      Serial.println("Relay 2 turned on");
      State_2 = 2;
    }
    // If the incoming data is '6', turn on relay 2
    else if (incomingData == '6' && !relay2CountdownInProgress && State_2 == 0) {
      digitalWrite(RELAY_2, HIGH);
      lastRelay2OnTime = rtc.now().unixtime(); // store the current time in seconds
      Serial.println("Relay 2 turned on");
      State_2 = 3;
    }
  }

  DateTime now = rtc.now();

  if(digitalRead(RELAY_1) == HIGH){
    unsigned long elapsedSeconds = now.unixtime() - lastRelay1OnTime; // calculate the elapsed time since the relay was turned on
    if (elapsedSeconds >= RELAY_1_TIME_1 && State_1 == 1) {
      digitalWrite(RELAY_1, LOW); // turn off relay 1
      Serial.println("Relay 1 turned OFF");
      State_1 = 0;
    }
    else if(elapsedSeconds >= RELAY_1_TIME_2 && State_1 == 2) {
      digitalWrite(RELAY_1, LOW); // turn off relay 1
      Serial.println("Relay 1 turned OFF");
      State_1 = 0;
    }
    else if(elapsedSeconds >= RELAY_1_TIME_3 && State_1 == 3) {
      digitalWrite(RELAY_1, LOW); // turn off relay 1
      Serial.println("Relay 1 turned OFF");
      State_1 = 0;
    }
    else {
      if(State_1 == 1 ){
        remainingSeconds_1 = RELAY_1_TIME_1- elapsedSeconds; // calculate the remaining time
        Serial.print("Time remaining for Relay 1: ");
        Serial.print(remainingSeconds_1);
        Serial.println(" seconds");
        
      }else if(State_1 == 2 ){
        remainingSeconds_1 = RELAY_1_TIME_2- elapsedSeconds; // calculate the remaining time
        Serial.print("Time remaining for Relay 1: ");
        Serial.print(remainingSeconds_1);
        Serial.println(" seconds");
        
      }else if(State_1 == 3 ){
        remainingSeconds_1 = RELAY_1_TIME_3- elapsedSeconds; // calculate the remaining time
        Serial.print("Time remaining for Relay 1: ");
        Serial.print(remainingSeconds_1);
        Serial.println(" seconds");
        
      }
      
    }
  }
  if(digitalRead(RELAY_2) == HIGH){
    unsigned long elapsedSeconds = now.unixtime() - lastRelay2OnTime; // calculate the elapsed time since the relay was turned on
    if (elapsedSeconds >= RELAY_2_TIME_1 && State_2 == 1) {
      digitalWrite(RELAY_2, LOW); // turn off relay 2
      Serial.println("Relay 2 turned OFF");
      State_2 = 0;
    }
    else if(elapsedSeconds >= RELAY_2_TIME_2 && State_2 == 2) {
      digitalWrite(RELAY_2, LOW); // turn off relay 2
      Serial.println("Relay 2 turned OFF");
      State_2 = 0;
    }
    else if(elapsedSeconds >= RELAY_2_TIME_3 && State_2 == 3) {
      digitalWrite(RELAY_2, LOW); // turn off relay 2
      Serial.println("Relay 2 turned OFF");
      State_2 = 0;
    }
    else {
      if(State_2 == 1){
        remainingSeconds = RELAY_2_TIME_1 - elapsedSeconds; // calculate the remaining time
        Serial.print("Time remaining for Relay 2: ");
        Serial.print(remainingSeconds);
        Serial.println(" seconds");
        
      }else if(State_2 == 2){
        remainingSeconds = RELAY_2_TIME_2 - elapsedSeconds; // calculate the remaining time
        Serial.print("Time remaining for Relay 2: ");
        Serial.print(remainingSeconds);
        Serial.println(" seconds");
        
      }else if(State_2 == 3){
        remainingSeconds = RELAY_2_TIME_3 - elapsedSeconds; // calculate the remaining time
        Serial.print("Time remaining for Relay 2: ");
        Serial.print(remainingSeconds);
        Serial.println(" seconds");
        
      }
      
    }
  }
    lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("CHARGING POINT");
  lcd.setCursor(0,1);
  lcd.print("Socket 1: ");
  if(digitalRead(RELAY_1) == HIGH){
    lcd.setCursor(10,1);
    lcd.print("ON");
  }else{
    lcd.setCursor(10,1);
    lcd.print("OFF");
  }
  lcd.setCursor(-4,2);
  lcd.print("Socket 2: ");
  if(digitalRead(RELAY_2) == HIGH){
    lcd.setCursor(6,2);
    lcd.print("ON");
  }else{
    lcd.setCursor(6,2);
    lcd.print("OFF");
  }
  lcd.setCursor(-4,3);
  lcd.print("T1");
  if(remainingSeconds_1 > 60){
    int Final_1 = remainingSeconds_1 / 60;
    int Final_2 = remainingSeconds_1 % 60;
    lcd.setCursor(-1,3);
    lcd.print(Final_1);
    lcd.setCursor(0,3);
    lcd.print(":");
    lcd.setCursor(1,3);
    lcd.print(Final_2);
  }else if(remainingSeconds_1 > 1 && remainingSeconds_1 < 60){
    lcd.setCursor(-1,3);
    lcd.print("0:");
    lcd.setCursor(1,3);
    lcd.print(remainingSeconds_1); 
  }else if(remainingSeconds_1 == 1){
    lcd.setCursor(-1,3);
    lcd.print("0:00");
  }
  
  lcd.setCursor(4,3);
  lcd.print("T2 ");
  if(remainingSeconds > 60){
    int Final_3 = remainingSeconds / 60;
    int Final_4 = remainingSeconds % 60;
    lcd.setCursor(7,3);
    lcd.print(Final_3);
    lcd.setCursor(8,3);
    lcd.print(":");
    lcd.setCursor(9,3);
    lcd.print(Final_4);
  }else if(remainingSeconds > 1 && remainingSeconds < 60){
    lcd.setCursor( 7,3);
    lcd.print("0:");
    lcd.setCursor(9,3);
    lcd.print(remainingSeconds); 
  }else if(remainingSeconds == 1){
    lcd.setCursor(7,3);
    lcd.print("0:00");
  }
  delay(1000);

}

