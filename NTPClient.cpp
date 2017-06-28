#include "NTPClient.h"

NTPClient::NTPClient(UDP *udp,String NTPServerName,int16_t localPort){
  UDPClient = udp;
  serverName = NTPServerName;
  port = localPort;
}

uint32_t NTPClient::syncTime(){
  UDPClient->begin(port);
  sendNTPPacket();
  uint32_t timeSent = millis();
  while(UDPClient->parsePacket() == 0){
    if(timeDifferance(timeSent,millis()) > timeOut){
      return 0;
    }
    delay(100);
  }
  lastSynced = millis();
  UDPClient->read(packetBuffer, NTP_PACKET_SIZE);
  uint32_t epoch = packetBuffer[43];
  epoch = epoch | ((uint32_t)packetBuffer[42]) << 8;
  epoch = epoch | ((uint32_t)packetBuffer[41]) << 16;
  epoch = epoch | ((uint32_t)packetBuffer[40]) << 24;
  unixTimeAtLastSynced = epoch - SECONDS_FROM_1900_TO_1970;
  return currentUnixTime();
}

void NTPClient::sendNTPPacket(){
  memset(packetBuffer,0,NTP_PACKET_SIZE); //clear the buffer
  packetBuffer[0] = B11100011; //version 4,client request
  packetBuffer[1] = 0; //specturm unknown
  packetBuffer[2] = 6; //request every minute
  packetBuffer[3] = 0; //one second accuracy
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  char serverNameArray[serverName.length()];
  serverName.toCharArray(serverNameArray,serverName.length()+1);
  UDPClient->beginPacket(serverNameArray, NTP_PORT);
  UDPClient->write(packetBuffer, NTP_PACKET_SIZE);
  UDPClient->endPacket();
}

uint32_t NTPClient::currentUnixTime(){
  return unixTimeAtLastSynced + (timeDifferance(lastSynced, millis()) / 1000);
}

uint32_t NTPClient::timeDifferance(uint32_t start,uint32_t end){
  if(start <= end){
    return end - start;
  }else{
    return (4294967295 - start) + end + 1;
  }
}
