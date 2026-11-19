#include "global.h"

#include "overlay_96_app_02210C98.h"

s32 ov96_022143FC(s32 arg0, s32 arg1);

s32 ov96_022143FC(s32 arg0, s32 arg1) {
    if (arg0 < 0x80) {
        if (arg1 < 0x70) {
            return 0;
        }
        return 2;
    }
    if (arg1 < 0x70) {
        return 1;
    }
    return 3;
}

void ov96_02214418(Ov96App_02210C98 *arg0);

void ov96_02214418(Ov96App_02210C98 *arg0) {
    u8 v0;
    u8 v2;
    u8 v3;
    u8 v4;

    v3 = 0;
    if (arg0->unk_738 <= 0x12C) {
        do {
            arg0->unk_818[v3] = 0;
            v3 += 1;
        } while ((u32)v3 < 4U);
        return;
    }
    v2 = 0;
    do {
        v0 = arg0->unk_814[v2];
        if ((u32)v3 < (u32)v0) {
            v3 = v0;
        }
        v2 += 1;
    } while ((u32)v2 < 4U);
    v4 = 0;
    do {
        if ((v3 != 0) && (v3 == arg0->unk_814[v4])) {
            arg0->unk_818[v4] = 1;
        } else {
            arg0->unk_818[v4] = 0;
        }
        v4 += 1;
    } while ((u32)v4 < 4U);
}

s32 ov96_02214044(Ov96App_02210C98 *, u8, s32);

void ov96_02214490(Ov96App_02210C98 *arg0);

void ov96_02214490(Ov96App_02210C98 *arg0) {
    u8 v0;

    v0 = 0;
    do {
        if (arg0->unk_7D4[v0].unk_0 == 0) {
            ov96_02214044(arg0, v0, 0);
        }
        v0 += 1;
    } while ((u32)v0 < 4U);
}
