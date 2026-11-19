#include "global.h"

#include "bg_window.h"
#include "geonet_globe.h"

void ov69_021E67B8(GeonetGlobe *globe);

void ov69_021E67B8(GeonetGlobe *globe) {
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON]);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION]);
    RemoveWindow(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE]);
    FreeBgTilemapBuffer(globe->bgConfig, 2U);
    FreeBgTilemapBuffer(globe->bgConfig, 6U);
    FreeBgTilemapBuffer(globe->bgConfig, 3U);
    FreeBgTilemapBuffer(globe->bgConfig, 7U);
}
