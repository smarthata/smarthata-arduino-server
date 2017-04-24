#ifndef SMARTHATA_WEBSERVER_H
#define SMARTHATA_WEBSERVER_H

#include <Ethernet.h>
#include "Page.h"

#define ETH_CS      10
#define SD_CS       4
#define BUFFER_LEN  1024
#define MAX_URL_LEN 128

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

    void readPage(Print &p, const Page *page) {
        for (unsigned int i = 0; i < page->length; ++i) {
            p.print((char) pgm_read_byte_near(page->content + i));
        }
    }

private:
    EthernetServer server = EthernetServer(80);

    char buffer[BUFFER_LEN];
    char url[MAX_URL_LEN];

    void disableSpiChannel(byte csPin) const {
        pinMode(csPin, OUTPUT);
        digitalWrite(csPin, HIGH);
    }

    void readRequest(EthernetClient &client) {
        int len = 0;
        bool emptyLine = false;

        while (client.connected()) {
            if (client.available()) {
                char c = (char) client.read();
                buffer[len++] = c;
                if (c == '\n' && emptyLine) {
                    buffer[len] = '\0';
                    Serial.println(buffer);
                    break;
                }
                if (c != '\r') {
                    emptyLine = c == '\n';
                }
            }
        }
        parseUrl();

    }

    void parseUrl() {
        url[0] = '\0';
        char *search;
        if ((search = strstr(buffer, "GET "))) {
            char *startUrl = search + 4;
            strlcpy(url, startUrl, getUrlLength(startUrl));
        }
    }

    byte getUrlLength(const char *startUrl) const {
        char *endUrl;
        if ((endUrl = strchr(startUrl, ' '))) {
            unsigned long len = (endUrl - startUrl + 1);
            return (byte) (len < MAX_URL_LEN ? len : MAX_URL_LEN);
        }
        return MAX_URL_LEN;
    }

    void writeResponse(EthernetClient &client) {
        const Page *page = getPage();

        if (page == &page_not_found_html) {
            client.println(F("HTTP/1.1 404 Not Found"));
        } else {
            client.println(F("HTTP/1.1 200 OK"));
        }

        client.println(F("Server: Smarthata/1.0.0"));
        client.println(F("Connection: close"));
        client.print(F("Content-Length: "));
        client.println(page->length);

        if (strstr(url, ".html") || !strcmp(url, "/")) {
            client.println(F("Content-Type: text/html"));
        } else if (strstr(url, ".json")) {
            client.println(F("Content-Type: application/json"));
        } else if (strstr(url, ".js")) {
            client.println(F("Content-Type: application/javascript"));
        } else if (strstr(url, ".css")) {
            client.println(F("Content-Type: text/css"));
        } else if (strstr(url, ".ico")) {
            client.println(F("Content-Type: image/ico"));
        } else if (strstr(url, ".txt")) {
            client.println(F("Content-Type: text/plain"));
        }
        client.println();

        readPage(client, page);

        client.flush();
        client.stop();
    }

    const Page *getPage() const {
        int i = 0;
        while (i < pagesCount) {
            if (!strcmp_P(url, pages[i]->name)) {
                return pages[i];
            }
            i++;
        }

        if (!strcmp(url, "/")) {
            return &page_index_html;
        }

        return &page_not_found_html;
    }
};

#endif
