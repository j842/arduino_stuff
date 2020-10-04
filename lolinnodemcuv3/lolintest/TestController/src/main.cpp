#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

/*

UDP Server. 10.10.10.200

test from linux with:   nc -u 10.10.10.200 9999

*/

WiFiUDP Udp;
unsigned int localUdpPort = 9999;  // local port to listen on 
char ReceivedMessage[255];  // buffer for incoming packets


// based on guide here:
// https://medium.com/@loginov_rocks/quick-start-with-nodemcu-v3-esp8266-arduino-ecosystem-and-platformio-ide-b8415bf9a038

void ledOn()
{
  digitalWrite(LED_BUILTIN, LOW);
}
void ledOff()
{
  digitalWrite(LED_BUILTIN, HIGH);
}
void flashit()
{
  for (int i=0;i<10;i++)
  {
    ledOn();
    delay(100+10*(10-i));
    ledOff();
    delay(100+10*(10-i));
  }  
}


void setup()
{ // connect via wifi to set up credentials (temporarily creates an access point to connect to)
  pinMode(LED_BUILTIN, OUTPUT);
  ledOn();

  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("NodeMCU Setup");
  Serial.println("WiFi Connected!");

  int rval = Udp.begin(localUdpPort);
  if (rval==1)
  {
    Serial.printf("Up and running on IP %s\n", WiFi.localIP().toString().c_str());
  }
  else
  {
    Serial.printf("Failed to open UDP port %d\n",localUdpPort);
    exit(-1);
  }

  flashit();
}

static bool alreadydone=false;

void loop()
{
  if (!alreadydone)
  {
    Serial.printf("Telling 10.10.10.200 to be cool.\n");
    Udp.beginPacket("10.10.10.200",9999);
    std::string msg="cool";
    Udp.write(msg.c_str());
    Udp.endPacket();
    alreadydone=true;
  }

  // int packetSize = Udp.parsePacket();

  // if (packetSize>0)
  // {
  //   ledOff(LED_BLUE);
  //   ledOff(LED_RED);
  //   // receive incoming UDP message
  //   Serial.printf("Received %d message from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
  //   int len = Udp.read(ReceivedMessage, 255);
  //   if (len > 0)
  //   {
  //     ReceivedMessage[len] = 0;
  //     if (ReceivedMessage[len-1]=='\n')
  //       ReceivedMessage[len-1]=0;
  //   }
  //   Serial.printf("UDP message: %s\n", ReceivedMessage);

  //   if (tolower(ReceivedMessage[0])=='r')
  //     ledOn(LED_RED);
  //   else if (tolower(ReceivedMessage[0]=='b'))
  //     ledOn(LED_BLUE);
  //   else if (tolower(ReceivedMessage[0]=='c'))
  //     cool();

  //   // send back a reply, to the IP address and port we got the packet from
  //   Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  //   std::string response;
  //   response="Recieved: \""+std::string(ReceivedMessage)+"\"\n";
  //   Udp.write(response.c_str());
  //   Udp.endPacket();
  // }

}