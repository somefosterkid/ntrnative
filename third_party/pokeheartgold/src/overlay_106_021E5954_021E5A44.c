#include "global.h"

#include "field/model_attributes.h"

#include "overlay_106.h"

void ov106_021E5A44(const ModelAttributes *attrs) {
    s32 var_r4;
    var_r4 = GX_LIGHTID_0;
    do {
        NNS_G3dGlbLightVector((GXLightId)(var_r4), attrs->lightVectors[var_r4].x, attrs->lightVectors[var_r4].y, attrs->lightVectors[var_r4].z);
        NNS_G3dGlbLightColor((GXLightId)(var_r4), attrs->lightColors[var_r4]);
        var_r4 += 1;
    } while ((u32)var_r4 < 4U);
}
