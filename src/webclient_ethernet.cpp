/*
* Some code Borrowed from:
* Benoit Blanchon 2014 - MIT License
* https://github.com/bblanchon/DrinksRfidTerminal
*/

#include <JsonGenerator.h>

#include <SPI.h>
#include <UIPEthernet.h>
#include <UIPClient.h>
#include <Dns.h>
#include "Arduino.h"
#include "webclient_ethernet.h"
#include "configuration.h"
#include "uoms.h"

#define xstr(s) str(s)
#define str(s) #s

#define NOT_EMPTY(s) (s[0]!=0 && s[0]!='\r' && s[0]!='\n')

bool WebClient::init()
{
  byte mac[6] = MAC_ADDRESS;
  
  Ethernet.begin(mac);
  
  DNSClient dns;
  
  dns.begin(Ethernet.dnsServerIP());
  
  dns.getHostByName(SERVER_NAME, serverIp);
}

void WebClient::readln(char* buffer, int size)
{
  int i = 0;
  bool connected = true;

  while (i < size - 1)
  {
    if (client.available() > 0)
    {
      char c = client.read();
      
      if (c == '\n') break;
      
      buffer[i++] = c;
    }
    else if (!connected)
    {
      break;
    }
    
    connected = client.connected();
  }

  buffer[i] = 0;
}

bool WebClient::query(const char* request, char* content, int maxContentSize)
{
    if (!client.connect(serverIp, SERVER_PORT))
    {
        return false;
    }
    
    client.print(request);
    client.println(" HTTP/1.1");
    client.println("Host: " SERVER_NAME ":" xstr(SERVER_PORT));
    client.println("Accept: application/json");
    client.println("Connection: close");

    if (content[0])
    {
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(strlen(content));
        client.println();

        client.println(content);
    }
    else
    {
        client.println();
    }

    while (readln(content, maxContentSize), NOT_EMPTY(content));

    readln(content, maxContentSize);
        
    client.stop();

    return content[0] != 0;
}

bool WebClient::send(byte uom, char* desc, float val)
{
  using namespace ArduinoJson::Generator;

  JsonArray<2> columns;
  columns.add("sensor");
  columns.add("temperature");

  JsonArray<2> point1;
  point1.add(desc);
  point1.add(val);
  
  JsonArray<1> points;
  points.add(point1);
  
  JsonObject<3> json;
  
  switch (uom)
  {
    case TEMPERATURE:
      json.add("name", "temperature");
      break;
    case PH:
      json.add("name", "ph");
      break;
    default:
      return false;
  }
  
  json.add("columns", columns);
  json.add("points", points);
  
  JsonArray<1> dataArray;
  dataArray.add(json);
  
  dataArray.printTo(buffer, sizeof(buffer));

  return this->query("POST /db/aquarium-wise/series?&u=" USERNAME "&p=" PASSWORD, buffer, sizeof(buffer));
}

void WebClient::maintain()
{
    Ethernet.maintain();
}