#ifndef SMARTHATA_PAGESHOLDER_H
#define SMARTHATA_PAGESHOLDER_H

#include <ArduinoJson.h>
#include <MemoryFree.h>
#include "Pageable.h"

FunctionPage dateTime = FunctionPage("/date-time.json", []() {
    StaticJsonBuffer<100> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["date"] = "20 Апреля";
    unsigned long time = millis() / 1000;
    root["time"] = String(time / 60) + ":" + String(time % 60);
    root["temperature"] = 0;
    root["memory"] = freeMemory();

    String result;
    root.printTo(result);
    return result;
});

FunctionPage wateringAlarms = FunctionPage("/watering-alarms.json", []() {
    StaticJsonBuffer<1000> jsonBuffer;
    JsonArray &root = jsonBuffer.createArray();

    for (int i = 0; i < 4; ++i) {
        JsonObject &object = root.createNestedObject();
        object["time"] = String(i * 4 + 10) + ":" + String(i * 10 + 20);
        object["enabled"] = i % 3 != 0;
    }

    String result;
    root.printTo(result);
    return result;
});

Page page_app_js = {app_js_name, app_js, app_js_len - 1};
Page page_favicon_ico = {favicon_ico_name, favicon_ico, favicon_ico_len - 1};
Page page_index_html = {index_html_name, index_html, index_html_len - 1};
Page page_not_found_html = {not_found_html_name, not_found_html, not_found_html_len - 1};
Page page_style_css = {style_css_name, style_css, style_css_len - 1};
Page page_tiles_html = {tiles_html_name, tiles_html, tiles_html_len - 1};
Page page_watering_html = {watering_html_name, watering_html, watering_html_len - 1};

Pageable *const pages[] = {
        &dateTime,
        &wateringAlarms,
        &page_app_js,
        &page_favicon_ico,
        &page_index_html,
        &page_not_found_html,
        &page_style_css,
        &page_tiles_html,
        &page_watering_html,
};

class PagesHolder {
public:
    Pageable *getPage(String requestUrl) const {
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

private:
    const byte pagesCount = (byte) (sizeof(pages) / sizeof(Pageable *));
};

#endif