#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "overlay_88_wifi_plaza_news.h"

void ov88_0225985C(WiFiPlazaNewsFlash *newsFlash);

void ov88_022597DC(WiFiPlazaNewsCaptions *newsCaptions);

void ov88_02259884(WiFiPlazaNewsFlash *newsFlash);
void ov88_02259800(WiFiPlazaNewsCaptions *newsCaptions);
void ov88_02259878(WiFiPlazaNewsFlash *newsFlash);
void ov88_02259818(WiFiPlazaNewsCaptions *newsCaptions, u32 captionIndex);

void ov88_022597DC(WiFiPlazaNewsCaptions *newsCaptions) {
    for (int i = 0; i < WIFI_PLAZA_NEWS_CAPTION_COUNT; i++) {
        RemoveWindow(&newsCaptions->windows[i]);
        ov88_0225985C(&newsCaptions->newsFlashes[i]);
    }
}

void ov88_02259800(WiFiPlazaNewsCaptions *newsCaptions) {
    for (int i = 0; i < WIFI_PLAZA_NEWS_CAPTION_COUNT; i++) {
        ov88_02259884(&newsCaptions->newsFlashes[i]);
    }
}

void ov88_02259818(WiFiPlazaNewsCaptions *newsCaptions, u32 captionIndex) {
    if (captionIndex >= WIFI_PLAZA_NEWS_CAPTION_COUNT) {
        GF_AssertFail();
    }
    if (captionIndex == 0) {
        GF_AssertFail();
    }
    ov88_02259878(&newsCaptions->newsFlashes[captionIndex]);
}
