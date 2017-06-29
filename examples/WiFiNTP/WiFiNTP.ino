//Code by Jinyu Liu
//Date and time fuctions adapted from Jeelab's RTClib library @ https://github.com/jcw/rtclib

#include <Arduino.h>
#include <NTPClient.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>、
#endif
#include <WiFiUDP.h>

#define NTP_POLLING_INTERVAL 20000 //poll the ntp server once every 20 sec
#define TIME_ZONE_OFFSET 8 * 3600

char ssid[] = "ssid";
char password[] = "password";

WiFiUDP udp;
NTPClient ntp(&udp);

unsigned long lastPoll = 0l;
int tries = 0;

unsigned long secondsFrom2000;
byte seconds;
byte minutes;
byte hours;
byte days;
byte months;
byte yearSince2000;

const byte daysInMonth [12] = {31,28,31,30,31,30,31,31,30,31,30,31};

void updateTime(){
  unsigned long t = secondsFrom2000;
  seconds = t % 60;
  t = t / 60;
  minutes = t % 60;
  t = t / 60;
  hours = t % 24;
  t = t / 24;

  //handle year
  yearSince2000 = 0;
  byte leap;
  while(true){
    leap = yearSince2000%4 == 0;
    if(t < (365 + leap)){
      break;
    }
    t = t - (365 + leap);
    yearSince2000++;
  }

  //handle months
  months = 1;
  while(true){
    byte daysThisMonth = daysInMonth[months-1];
    if(months == 2 && leap){
      daysThisMonth++;
    }
    if(t < daysThisMonth){
      break;
    }
    t = t - daysThisMonth;
    months++;
  }
  days = t + 1;
}

void setup(){
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  Serial.println(F("Connecting"));
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(F("."));
    delay(500);
  }
  Serial.println();

  Serial.print(F("Connected, IP address: "));
  Serial.println(WiFi.localIP());
}

void loop(){
  unsigned long unixTime;
  if((millis() - lastPoll) >= NTP_POLLING_INTERVAL){
    lastPoll = millis();
    Serial.println(F("Syncing Time with NTP Server..."));
    while(true){
      unixTime = ntp.syncTime();
      if(unixTime == 0){
        Serial.println(F("Request Timed Out."));
        tries ++;
      }else{
        break;
      }
      if(tries >= 3){
        Serial.println(F("Time Sync Failed"));
        break;
      }
      Serial.println(F("Retrying..."));
    }
    if(unixTime > 0){
      secondsFrom2000 = ntp.secondsSince2000() + TIME_ZONE_OFFSET;
      updateTime();
      Serial.println(F("Time Sync Successful."));
      Serial.print(F("Current time is: "));
      Serial.print(yearSince2000+2000);
      Serial.print(F("/"));
      Serial.print(months);
      Serial.print(F("/"));
      Serial.print(days);
      Serial.print(F(" "));
      Serial.print(hours);
      Serial.print(F(":"));
      Serial.print(minutes);
      Serial.print(F(":"));
      Serial.println(seconds);
    }

  }
  delay(1000);
}
