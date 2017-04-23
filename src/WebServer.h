#ifndef SMARTHATA_WEBSERVER_H
#define SMARTHATA_WEBSERVER_H

#include <Ethernet.h>

#define ETH_CS    10
#define SD_CS     4

class WebServer {
public:

    WebServer() {
        disableSpiChannel(ETH_CS);
        disableSpiChannel(SD_CS);
    }

    void setup(byte mac[6], IPAddress ip) {
        Ethernet.begin(mac, ip);
        server.begin();
        Serial.print(F("Server is at "));
        Serial.println(Ethernet.localIP());
    }

    void loop() {
        EthernetClient client = server.available();
        if (client) {
            Serial.println(F("New client"));
            readRequest(client);
            writeResponse(client);
            Serial.println(F("Client disconnected"));
        }
    }

private:
    EthernetServer server = EthernetServer(80);

    void disableSpiChannel(byte csPin) const {
        pinMode(csPin, OUTPUT);
        digitalWrite(csPin, HIGH);
    }

    void readRequest(EthernetClient &client) {
        int len = 0;
        char request[1024];
        bool emptyLine = false;

        while (client.connected()) {
            if (client.available()) {
                char c = (char) client.read();
                request[len++] = c;
                if (c == '\n' && emptyLine) {
                    request[len] = '\0';
                    Serial.println(request);
                    break;
                }
                if (c != '\r') {
                    emptyLine = c == '\n';
                }
            }
        }
    }


    void writeResponse(EthernetClient &client) {
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: text/html"));
        client.println();
        client.println(F("<html><body>Hi</body></html>"));

        client.flush();
        delay(1);
        client.stop();
    }
};

#endif
