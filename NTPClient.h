#ifndef NTPCLIENT_H
#define NEPCLIENT_H

#include <Arduino.h>
#include <Udp.h>

#define NTP_PACKET_SIZE 48
#define NTP_PORT 123
#define SECONDS_FROM_1900_TO_1970 2208988800

class NTPClient{
  public:
    //constructors
    NTPClient(UDP *udp,String NTPServerName="pool.ntp.org",int16_t localPort=2390);

    uint32_t syncTime();
    uint32_t currentUnixTime();

  private:
     UDP *UDPClient;
     String serverName;
     int16_t port;
     uint8_t packetBuffer[NTP_PACKET_SIZE];
     uint8_t synced = false;
     uint32_t lastSynced = 0;
     uint32_t unixTimeAtLastSynced=0;
     uint32_t timeOut = 10000;

     void sendNTPPacket();

     uint32_t timeDifferance(uint32_t start,uint32_t end);
};
#endif
