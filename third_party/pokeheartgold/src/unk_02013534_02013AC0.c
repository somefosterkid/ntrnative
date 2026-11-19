#include "global.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "sprite.h"
#include "unk_02013534_font_system.h"

void sub_02013AC0(s8 *arg0);

FontOAM *sub_02013AD0(const UnkStruct_02012744 *param0);

extern const u8 _020F5F2C[12][2];
int sub_02013AF8(int param0, int param1);

void sub_02013AC0(s8 *arg0) {
    u32 v1;
    s8 *v0;

    v0 = arg0;
    v1 = sizeof(FontOAM);
    do {
        *v0 = 0;
        v0 += 1;
        v1 -= 1;
    } while (v1 != 0);
}

FontOAM *sub_02013AD0(const UnkStruct_02012744 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_64; v0++) {
        if (param0->unk_60[v0].unk_00 == 0L) {
            return param0->unk_60 + v0;
        }
    }

    return 0L;
}

int sub_02013AF8(int param0, int param1) {
    int v0;

    for (v0 = 0; v0 < 12; v0++) {
        if ((_020F5F2C[v0][0] <= param0) && (_020F5F2C[v0][1] <= param1)) {
            return v0;
        }
    }

    return 12;
}
