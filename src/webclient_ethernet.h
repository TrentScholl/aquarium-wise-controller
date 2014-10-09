#ifndef _WEBCLIENT_ETHERNET_h
#define _WEBCLIENT_ETHERNET_h

#include "Arduino.h"

#include <SPI.h>
#include <UIPEthernet.h>

class WebClient
{
 protected:
  char *username_;
  char *password_;
  
 public:
   WebClient(char *username, char *password);
   EthernetClient client;
   bool init();

};

#endif

