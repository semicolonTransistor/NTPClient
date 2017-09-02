#  Introduction  

This library is designed to be a lightweight implementation of an NTP Client for Arduino.
Only the most basic functionality of acquiring time is implemented. There is no compensation
for trip delay.  

##  Fuction Overview
###  constructor  
```C++
NTPClient(UDP *udp,String NTPServerName="pool.ntp.org",int16_t localPort=2390)
```
####  parameters:
* *udp: pointer to the udp interface to use(EthernetUDP,WiFiUDP, etc.)  
* NTPServerName: optional, string of the host name of the time server to use. Defaults to pool.ntp.org  
* localPort: optional,the local port number. Defaults to 2390  

### syncTime
```C++
uint32_t syncTime()
```
sends an ntp package and wait a maxium of time specified with timeout.
####  returns:
0 if timed out. current time in unix time if successful.  

###  currentUnixTime  
```C++
uint32_t currentUnixTime()
```
####  returns:  
the current unix time.
