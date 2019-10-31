#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "vanderwalt"
#define STAPSK  "0824517915"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;
const uint16_t port = 6969;
const char* ip = "192.168.1.19";
WiFiClient client;

void setup()
{
  Wire.begin(1, 2, 8);
  Wire.onRequest(requestEvent);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  if (!client.connect(ip, port)) {
      return;
  }
  
}

void loop()
{
  client.print("LT1234\n\0");
  delay(500);
  while(!client.available() > 0){}
  String line = client.readStringUntil('\r');
  while(client.available()){char a = client.read();}
  delay(1000);
}

void requestEvent() {
  Wire.write("hello ");
}
