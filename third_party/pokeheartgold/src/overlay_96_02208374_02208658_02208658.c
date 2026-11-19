#include "global.h"

#include "overlay_96_ov96hurdle_hud.h"

s32 ov96_021EB52C(void *, s32, s32);

s32 ov96_021EB564(void *, s32);

s32 ov96_021EB588(void *, VecFx32 *);

void ov96_0220831C(Ov96HurdleHud *, s32, s32);

void ov96_02208A80(Ov96HurdleHudBadge *, s32);

void ov96_02208658(Ov96HurdleHud *hud, s32 arg1, u8 arg2, s32 arg3, s32 arg4);

void ov96_02208658(Ov96HurdleHud *hud, s32 arg1, u8 arg2, s32 arg3, s32 arg4) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    if (arg2 != hud->unk_1A4[arg1]) {
        hud->unk_1A4[arg1] = arg2;
        ov96_0220831C(hud, arg1, arg2);
    }
    if (arg3 != 0) {
        VecFx32 a;
        a.z = 0;
        a.x = (arg1 * 0x28 + 0x70) << 12;
        a.y = 0x350004;
        ov96_021EB588(hud->players[arg1].unk_0C, &a);
        ov96_021EB588(hud->players[arg1].unk_14, &a);
        ov96_02208A80(&hud->badges[arg1], 0);
    } else {
        VecFx32 b;
        b.z = 0;
        b.x = (arg1 * 0x28 + 0x70) << 12;
        b.y = 0x350000;
        ov96_021EB588(hud->players[arg1].unk_0C, &b);
        ov96_021EB588(hud->players[arg1].unk_14, &b);
        ov96_02208A80(&hud->badges[arg1], 1);
        if (arg4 == 1) {
            if (arg4 != hud->players[arg1].unk_18[0]) {
                ov96_021EB52C(hud->players[arg1].unk_10, 1, 1);
                ov96_021EB564(hud->players[arg1].unk_10, 1);
            }
        } else {
            ov96_021EB52C(hud->players[arg1].unk_10, 1, 0);
        }
    }
    hud->players[arg1].unk_18[0] = arg4;
#else
    u8 *arg0 = (u8 *)hud;
    s32 off;
    u8 *p;
    s32 t;

    if (arg2 != arg0[0x1A4 + arg1]) {
        arg0[0x1A4 + arg1] = arg2;
        ov96_0220831C(hud, arg1, arg2);
    }
    if (arg3 != 0) {
        VecFx32 a;
        a.z = 0;
        a.x = (arg1 * 0x28 + 0x70) << 12;
        a.y = 0x350004;
        off = arg1 * 0x1C;
        ov96_021EB588(*(void **)(arg0 + (off + 0x38)), &a);
        ov96_021EB588(*(void **)(arg0 + (off + 0x40)), &a);
        ov96_02208A80(&hud->badges[arg1], 0);
    } else {
        VecFx32 b;
        b.z = 0;
        b.x = (arg1 * 0x28 + 0x70) << 12;
        b.y = 0x350000;
        off = arg1 * 0x1C;
        ov96_021EB588(*(void **)(arg0 + (off + 0x38)), &b);
        ov96_021EB588(*(void **)(arg0 + (off + 0x40)), &b);
        ov96_02208A80(&hud->badges[arg1], 1);
        if (arg4 == 1) {
            if (arg4 != *(arg0 + (off + 0x44))) {
                t = 0x1C * (u32)arg1;
                p = arg0 + 0x3C;
                ov96_021EB52C(*(void **)(p + t), 1, 1);
                ov96_021EB564(*(void **)(p + t), 1);
            }
        } else {
            ov96_021EB52C(*(void **)(arg0 + (off + 0x3C)), 1, 0);
        }
    }
    *(arg0 + (off + 0x44)) = arg4;
#endif
}
