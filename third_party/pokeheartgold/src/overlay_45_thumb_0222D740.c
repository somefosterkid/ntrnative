#include "global.h"
#include "geonet_locations.h"

void ov45_0222D740(void *param0);

s32 ov45_0222D79C(u32 arg0, u32 arg1);

void ov45_0222D740(void *param0) {
    NNSG3dResMdlSet *v0 = NNS_G3dGetMdlSet(param0);
    NNSG3dResMdl *v1 = NNS_G3dGetMdlByIdx(v0, 0);

    NNS_G3dMdlUseGlbDiff(v1);
    NNS_G3dMdlUseGlbAmb(v1);
    NNS_G3dMdlUseGlbSpec(v1);
    NNS_G3dMdlUseGlbEmi(v1);
}

s32 ov45_0222D79C(u32 country, u32 arg1) {
    u32 v0;

    v0 = GeonetLocations_GetLocationCountFromCountry(country);
    if ((v0 == 0) && (arg1 == 0)) {
        return 1;
    }
    if ((arg1 >= 1U) && (arg1 <= v0)) {
        return 1;
    }
    return 0;
}
