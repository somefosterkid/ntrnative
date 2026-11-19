#include "global.h"

#include "camera.h"
#include "geonet_globe.h"

BOOL ov69_021E737C(GeonetGlobe *globe) {
    s32 distance;
    BOOL v1;
    u16 v0;

    distance = Camera_GetDistance(globe->camera);
    v1 = FALSE;
    v0 = globe->zoomMode;
    if (v0 != GEONET_GLOBE_ZOOM_OUT) {
        if (v0 == GEONET_GLOBE_ZOOM_IN) {
            if (distance > 0x58000) {
                distance -= 0x8000;
                globe->markerScale.x = (s32)(globe->markerScale.x - 0x80);
                globe->markerScale.y = (s32)globe->markerScale.x;
            } else {
                distance = 0x50000;
                v1 = TRUE;
            }
        }
    } else if (distance < 0x120000) {
        distance += 0x8000;
        globe->markerScale.x = (s32)(globe->markerScale.x + 0x80);
        globe->markerScale.y = (s32)globe->markerScale.x;
    } else {
        distance = 0x128000;
        v1 = TRUE;
    }
    Camera_SetDistance(distance, globe->camera);
    return v1;
}
