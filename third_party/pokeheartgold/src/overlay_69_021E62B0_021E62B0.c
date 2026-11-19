#include "global.h"

#include "geonet_globe.h"

void ov69_021E62B0(GeonetGlobe *globe);

void ov69_021E62B0(GeonetGlobe *globe) {
    for (int i = 0; i < globe->markers.count; i++) {
        if (globe->markers.markers[i].country == globe->playerCountry && globe->markers.markers[i].region == globe->playerRegion) {
            globe->markers.markers[i].recency = GEONET_GLOBE_RECENCY_SELF;
            globe->rotationAngle.x = globe->markers.markers[i].x;
            globe->rotationAngle.y = globe->markers.markers[i].y;
        }
    }
}
