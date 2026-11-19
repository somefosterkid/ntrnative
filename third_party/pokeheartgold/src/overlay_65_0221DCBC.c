#include "global.h"

#include "math_util.h"

void ov65_0221DCBC(u16 *angle);

typedef struct TradeRoomMonDisplayData {
    u16 species;
    u16 heldItemType;
    u8 pokeBall;
    u8 isEgg;
    u8 form;
    u16 gender;
    u16 iconShouldFlip;
    int ballCapsuleId;
} TradeRoomMonDisplayData;
extern const u8 ov65_0221FFB4[13][4][6];
int ov65_0221DCFC(int currentSlot, int direction, TradeRoomMonDisplayData *displayData);

void ov65_0221DCBC(u16 *angle) {
    fx32 sinVal;
    GXRgb color;
    int green;

    *angle += 20;

    if (*angle > 360) {
        *angle = 0;
    }

    sinVal = GF_SinDeg(*angle);
    green = 15 + (sinVal * 10) / ((fx32)0x0000000000001000L);
    color = ((GXRgb)(((29) << (0)) | ((green) << (5)) | ((0) << (10))));

    GX_LoadOBJPltt((u16 *)&color, (16 + 13) * 2, 2);
}

int ov65_0221DCFC(int currentSlot, int direction, TradeRoomMonDisplayData *displayData) {
    int i, targetSlot = 0;

    for (i = 0; i < 6; i++) {
        if (displayData[ov65_0221FFB4[currentSlot][direction][i]].species != 0) {
            targetSlot = ov65_0221FFB4[currentSlot][direction][i];
            break;
        }
    }

    return targetSlot;
}
