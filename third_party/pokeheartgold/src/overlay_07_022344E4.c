#include "global.h"

#include "overlay_07.h"
#include "seal_case.h"


void ov07_02234510(SEAL *arg0);

int ov07_022344E4(s16 x1, s16 y1, s16 x2, s16 y2) {
    int distance;
    s16 dx = (x1 - x2);
    s16 dy = (y1 - y2) * -1;

    distance = FX_Sqrt(((dy * dy) + (dx * dx)) * FX32_ONE) / FX32_ONE;

    return distance;
}

void ov07_02234510(SEAL *seal) {
    u8 x1;

    x1 = SealOnCapsuleGetX(seal);
    ov07_022344E4((s16)x1, (s16)SealOnCapsuleGetY(seal), 0xBE, 0x46);
}
