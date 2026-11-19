#include "global.h"

#include "error_handling.h"
#include "sprite_system.h"
#include "overlay_96_app_0221040c.h"

void ov96_0221040C(Ov96App_0221040C *arg0, s32 arg1) {
    s32 v1;
    s32 v0;
    s32 seqno;
    s32 v3;
    s32 v5;
    s32 v6;
    u8 v2;
    u8 v4;

    v4 = (u8)(arg1 % 100);
    v1 = (s32)(u8)((s32)v4 / 10);
    v0 = (s32)(u8)((s32)v4 % 10);
    v2 = arg0->unk_E4;
    switch (v2) {
    case 2:
        break;
    case 0:
        if ((u32)v4 <= 0x14U) {
            v5 = 0;
            do {
                ManagedSprite_SetPaletteOverride(arg0->sprites[v5 + 1], 2);
                v5 += 1;
            } while (v5 < 2);
            arg0->unk_E4 = arg0->unk_E4 + 1;
        }
        break;
    case 1:
        if ((u32)v4 <= 3U) {
            v6 = 0;
            do {
                ManagedSprite_SetPaletteOverride(arg0->sprites[v6 + 1], 3);
                v6 += 1;
            } while (v6 < 2);
            arg0->unk_E4 = arg0->unk_E4 + 1;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    seqno = v1 + 9;
    v1 = seqno;
    ManagedSprite_SetAnim(arg0->sprites[1], seqno);
    v3 = v0 + 9;
    v0 = v3;
    ManagedSprite_SetAnim(arg0->sprites[2], v3);
}

u16 ov96_022104C4(Ov96App_0221040C *arg0) {
    return (u16)(arg0->unk_E6 + (arg0->unk_E5 << 7));
}
