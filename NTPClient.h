#ifndef NTPCLIENT_H
#define NEPCLIENT_H

#include <Arduino.h>
#include <Udp.h>

#define NTP_PACKET_SIZE 48
#define NTP_PORT 123
#define SECONDS_FROM_1900_TO_1970 2208988800L
#define SECONDS_FROM_1970_TO_2000 946684800L

class NTPClient{
  public:
    //constructor
    NTPClient(UDP *udp,String NTPServerName="pool.ntp.org",int16_t localPort=2390);

    uint32_t syncTime();
    void sendNTPPacket();
    uint32_t processPacket();

    uint32_t currentUnixTime();
    uint32_t secondsSince2000();
    
    void setTimeout(uint16_t newTimeOut);
    uint16_t getTimeout();

    uint32_t getLastSynced();
    uint32_t getUnixTimeAtLastSynced();

  private:
     UDP *UDPClient;
     String serverName;
     int16_t port;
     uint8_t packetBuffer[NTP_PACKET_SIZE];
     uint32_t lastSynced = 0;
     uint32_t unixTimeAtLastSynced=0;
     uint16_t timeOut = 10000;


     uint32_t timeDifferance(uint32_t start,uint32_t end);
};
#endif
