#include "global.h"

#include "math_util.h"
#include "overlay_85.h"
#include "unk_02034354.h"

void ov85_021E9E58(u16 *param0) {
    *param0 += 10;

    if (*param0 > 360) {
        *param0 = 0;
    }

    fx32 v0 = GF_SinDeg(*param0);
    int v3 = 15 + (v0 * 10) / FX32_ONE;
    GXRgb v1 = GX_RGB(29, v3, 0);

    GX_LoadOBJPltt((u16 *)&v1, 5 * 2, 2);
    GX_LoadOBJPltt((u16 *)&v1, (5 + 16) * 2, 2);
}

int ov85_021E9EA4(void);

int ov85_021E9EA4(void) {
    int v0, v1;

    for (v1 = 0, v0 = 0; v0 < 5; v0++) {
        if (sub_02034818(v0) != NULL) {
            v1++;
        }
    }

    return v1;
}
