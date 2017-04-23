#include <Arduino.h>
#include <Ethernet.h>
#include "WebServer.h"

byte mac[] = {0x00, 0x2A, 0x97, 0x76, 0xF0, 0x57};
IPAddress ip(192, 168, 1, 8);

WebServer webServer;

void setup() {
    Serial.begin(9600);
    webServer.setup(mac, ip);
}

void loop() {
    webServer.loop();
}
