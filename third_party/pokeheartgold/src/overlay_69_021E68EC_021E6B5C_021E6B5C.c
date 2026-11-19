#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "geonet_globe.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"

extern const WindowTemplate ov69_021E764C;

void ov69_021E6B5C(GeonetGlobe *globe, int country, int region);

void ov69_021E6B5C(GeonetGlobe *globe, int country, int region) {
    String *text;
    String *text_00;

    text = String_New(0x40, globe->heapID);
    text_00 = String_New(0x40, globe->heapID);
    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO], &ov69_021E764C);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO], 0xf, 0, 0, 0xd8, 0x30);
    DrawFrameAndWindow1(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO], 0, 0x1d9, 7);
    ov69_021E7520(country, region, text, text_00, globe->heapID);
    if (region != 0) {
        AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO], FONT_SYSTEM, text_00, 0, 0x10, 0xff, NULL);
    }
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO], FONT_SYSTEM, text, 0, 0, 0, NULL);
    String_Delete(text_00);
    String_Delete(text);
    CopyWindowToVram(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO]);
    return;
}

void ov69_021E6C14(GeonetGlobe *globe);

void ov69_021E6C14(GeonetGlobe *globe) {
    ClearWindowPixelBuffer(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO], 0);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_REGION_INFO]);
    return;
}
