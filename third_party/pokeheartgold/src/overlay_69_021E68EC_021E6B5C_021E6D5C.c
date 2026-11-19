#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "geonet_globe.h"
#include "msgdata.h"
#include "pm_string.h"
#include "text.h"

void ov69_021E6D5C(GeonetGlobe *globe);

void ov69_021E6D5C(GeonetGlobe *globe) {
    u32 idx;
    String *countryName;
    String *regionName;

    if (globe->inspectMode == 0) {
        regionName = String_New(0x400, globe->heapID);
        FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 0xF, 0, 0, 0xD8, 0x30);
        ReadMsgDataIntoString(globe->messageLoader, 0xF, regionName);
        AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], FONT_MESSAGE, regionName, 0, 0, 0, NULL);
        String_Delete(regionName);
        return;
    }
    if (ov69_021E6C30(globe, &idx)) {
        countryName = String_New(0x40, globe->heapID);
        regionName = String_New(0x40, globe->heapID);
        FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 0xF, 0, 0, 0xD8, 0x30);
        ov69_021E7520(globe->markers.markers[idx].country, globe->markers.markers[idx].region, countryName, regionName, globe->heapID);
        if (globe->markers.markers[idx].region != 0) {
            AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], FONT_MESSAGE, regionName, 0, 0x10, 0xFF, NULL);
        }
        AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], FONT_MESSAGE, countryName, 0, 0, 0, NULL);
        String_Delete(regionName);
        String_Delete(countryName);
        globe->rotationAngle.x = globe->markers.markers[idx].x;
        globe->rotationAngle.y = globe->markers.markers[idx].y;
    } else {
        globe->inspectMode = 0;
    }
}
