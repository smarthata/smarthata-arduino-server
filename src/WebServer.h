#ifndef SMARTHATA_WEBSERVER_H
#define SMARTHATA_WEBSERVER_H

#include <Ethernet.h>
#include "Page.h"

#define ETH_CS      10
#define SD_CS       4

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
            client.flush();
            client.stop();
            Serial.println(F("Client disconnected"));
        }
    }

private:

    EthernetServer server = EthernetServer(80);

    String requestUrl;

    void disableSpiChannel(byte csPin) const {
        pinMode(csPin, OUTPUT);
        digitalWrite(csPin, HIGH);
    }

    void readRequest(EthernetClient &client) {
        bool emptyLine = false;
        String request;
        requestUrl = "";

        while (client.connected()) {
            if (client.available()) {
                char c = (char) client.read();
                request += c;
                if (c == '\n') {
                    parseRequest(request);
                    request = "";
                    if (emptyLine) break;
                }
                if (c != '\r') {
                    emptyLine = c == '\n';
                }
            }
        }
    }

    void parseRequest(String &request) {
        Serial.print(request);
        int indexGet = request.indexOf("GET ");
        if (indexGet >= 0) {
            int urlEnd = request.indexOf(' ', (unsigned int) (indexGet + 4));
            urlEnd = urlEnd < 0 ? request.length() : (unsigned int) urlEnd;
            requestUrl = request.substring((unsigned int) (indexGet + 4), (unsigned int) urlEnd);
        }
    }

    void writeResponse(Print &client) const {
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

        client.print(F("Content-Type: "));
        client.println(getContentType(page->getName()));

        client.println();

        readPage(client, page);
    }

    const String getContentType(const String pageName) const {

        const String extensions[] = {".html", ".json", ".js", ".css", ".ico"};
        const __FlashStringHelper *contentTypes[] = {
                F("text/html"),
                F("application/json"),
                F("application/javascript"),
                F("text/css"),
                F("image/ico")
        };

        byte length = sizeof(extensions) / sizeof(String);
        for (int i = 0; i < length; ++i) {
            if (pageName.indexOf(extensions[i]) >= 0) {
                return contentTypes[i];
            }
        }

        return F("text/plain");
    }

    const Page *getPage() const {
        for (int i = 0; i < pagesCount; ++i) {
            if (requestUrl.equals(pages[i]->getName())) {
                return pages[i];
            }
        }

        if (requestUrl.equals("/")) {
            return &page_index_html;
        }

        return &page_not_found_html;
    }

    void readPage(Print &p, const Page *page) const {
        for (unsigned int i = 0; i < page->length; ++i) {
            p.print((char) pgm_read_byte_near(page->content + i));
        }
    }
};

#endif
