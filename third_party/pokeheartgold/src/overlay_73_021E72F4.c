#include "global.h"

#include "math_util.h"
#include "player_data.h"

struct PlayerProfile;
PlayerProfile *sub_02034818(int netId);

void ov73_021E72F4(u16 *param0);
int ov73_021E7340(void);

void ov73_021E72F4(u16 *param0) {
    *param0 += 10;

    if (*param0 > 360) {
        *param0 = 0;
    }

    fx32 v0 = GF_SinDeg(*param0);
    int v3 = 15 + (v0 * 10) / ((fx32)0x0000000000001000L);
    GXRgb v1 = ((GXRgb)(((29) << (0)) | ((v3) << (5)) | ((0) << (10))));

    GX_LoadOBJPltt((u16 *)&v1, 5 * 2, 2);
    GX_LoadOBJPltt((u16 *)&v1, (5 + 16) * 2, 2);
}

int ov73_021E7340(void) {
    int v0, v1;

    for (v1 = 0, v0 = 0; v0 < 5; v0++) {
        if (sub_02034818(v0) != 0L) {
            v1++;
        }
    }

    return v1;
}
