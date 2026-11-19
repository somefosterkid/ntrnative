#include "global.h"

#include "camera.h"
#include "geonet_globe.h"

void ov69_021E6FE8(GeonetGlobe *globe);

void ov69_021E6FE8(GeonetGlobe *globe) {
    VecFx32 target = { 0, 0, 0 };
    VecFx32 position = { 0, 0, 0x128000 };

    Camera_Init_FromTargetAndPos(&target, &position, 0x5c1, 0, 0, globe->camera);
    Camera_SetPerspectiveClippingPlane(0, FX32_ONE * 100, globe->camera);
    Camera_ApplyPerspectiveType(0, globe->camera);
    Camera_SetStaticPtr(globe->camera);

    if (globe->canRotateFreely == FALSE) {
        globe->zoomMode = GEONET_GLOBE_ZOOM_IN;
    } else {
        globe->zoomMode = GEONET_GLOBE_ZOOM_OUT;
    }

    while (TRUE) {
        if (ov69_021E737C(globe) == 1) {
            break;
        }
    }
}
