#ifndef SMARTHATA_PAGESHOLDER_H
#define SMARTHATA_PAGESHOLDER_H

#include "Pageable.h"

FunctionPage msPage = FunctionPage("/ms", []() { return String(millis()); });
FunctionPage mcsPage = FunctionPage("/mcs", []() { return String(micros()); });

Page page_app_js = {app_js_name, app_js, app_js_len - 1};
Page page_date_time_json = {date_time_json_name, date_time_json, date_time_json_len - 1};
Page page_favicon_ico = {favicon_ico_name, favicon_ico, favicon_ico_len - 1};
Page page_index_html = {index_html_name, index_html, index_html_len - 1};
Page page_not_found_html = {not_found_html_name, not_found_html, not_found_html_len - 1};
Page page_style_css = {style_css_name, style_css, style_css_len - 1};
Page page_tiles_html = {tiles_html_name, tiles_html, tiles_html_len - 1};
Page page_watering_alarms_json = {watering_alarms_json_name, watering_alarms_json, watering_alarms_json_len - 1};
Page page_watering_html = {watering_html_name, watering_html, watering_html_len - 1};

Pageable *const pages[] = {
        &msPage,
        &mcsPage,
        &page_app_js,
        &page_date_time_json,
        &page_favicon_ico,
        &page_index_html,
        &page_not_found_html,
        &page_style_css,
        &page_tiles_html,
        &page_watering_alarms_json,
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