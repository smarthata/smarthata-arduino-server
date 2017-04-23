#ifndef SMARTHATA_PAGE_H
#define SMARTHATA_PAGE_H

#include "web.h"

struct Page {
    PGM_P name;
    PGM_P content;
};

const Page page_app_js = {app_js_name, app_js};
const Page page_date_time_json = {date_time_json_name, date_time_json};
const Page page_favicon_ico = {favicon_ico_name, favicon_ico};
const Page page_index_html = {index_html_name, index_html};
const Page page_not_found_html = {not_found_html_name, not_found_html};
const Page page_style_css = {style_css_name, style_css};
const Page page_tiles_html = {tiles_html_name, tiles_html};
const Page page_watering_alarms_json = {watering_alarms_json_name, watering_alarms_json};
const Page page_watering_html = {watering_html_name, watering_html};

const Page *const pages[] = {
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

const byte pagesCount = (byte) (sizeof(pages) / sizeof(Page *));

#endif
