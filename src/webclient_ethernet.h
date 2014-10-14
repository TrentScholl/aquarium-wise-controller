#ifndef _WEBCLIENT_ETHERNET_h
#define _WEBCLIENT_ETHERNET_h

#include <SPI.h>
#include <UIPEthernet.h>
#include <UIPServer.h>
#include <UIPClient.h>
#include "Arduino.h"

class WebClient
{
 protected:
   void readln(char*, int);
   IPAddress serverIp;
   EthernetClient client;
   char buffer[150];
 public:
   bool init();
   bool query(const char* path, char* content, int maxContentSize);
   bool send(byte uom, char* desc, float val);
   void maintain();
   
};

#endif

