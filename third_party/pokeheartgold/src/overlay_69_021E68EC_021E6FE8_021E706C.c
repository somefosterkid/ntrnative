#include "global.h"

#include "geonet_globe.h"

void ov69_021E706C(GeonetGlobe *globe);

void ov69_021E706C(GeonetGlobe *globe) {
    globe->lightVector.x = 0;
    globe->lightVector.y = 0;
    globe->lightVector.z = -0xFFF;
    NNS_G3dGlbLightVector(GX_LIGHTID_0, (s16)globe->lightVector.x, (s16)globe->lightVector.y, (s16)globe->lightVector.z);
}
