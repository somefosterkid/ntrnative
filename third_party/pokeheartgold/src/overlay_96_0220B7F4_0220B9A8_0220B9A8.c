#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_96_unk_struct_ov96_0220_b9_a8.h"

#if defined(SDK_PORT) || defined(NONMATCHING) // as defined in src/overlay_96_0220B7F4_0220C40C_0220C54C.c (tools/pokehg_abi.py)
void ov96_0220C54C(void *, s32, s32, s32, s32);
#else
s32 ov96_0220C54C(void *, s32, u8, u8, s32);
#endif

s32 ov96_0220C578(void *, s32);

void ov96_0220C90C(void *param_1, u32 param_2, s16 param_3, s16 param_4);

void ov96_0220B9A8(UnkStruct_ov96_0220B9A8 *p);

void ov96_0220B9A8(UnkStruct_ov96_0220B9A8 *p) {
    s16 v[3][2];
    s16 t;
    s32 d;
    s32 step;
    s32 e;
    s32 k;
    s8 sel;

    switch (p->state) {
    case 0:
        ManagedSprite_ResetSpriteAnimCtrlState(p->spr1C);
        ManagedSprite_SetDrawFlag(p->spr1C, 1);
        PlaySE(0x8B4);
        p->state++;
        return;
    case 1:
        p->cnt++;
        if (p->cnt < 2) {
            return;
        }
        ManagedSprite_ResetSpriteAnimCtrlState(p->spr20);
        ManagedSprite_SetAnimateFlag(p->spr20, 0);
        ManagedSprite_SetDrawFlag(p->spr20, 1);
        ManagedSprite_SetDrawFlag(p->spr10, 0);
        ManagedSprite_SetDrawFlag(p->spr14, 0);
        ManagedSprite_SetDrawFlag(p->spr18, 0);
        p->cnt = 0;
        p->state++;
        return;
    case 2:
        ManagedSprite_GetPositionXY(p->spr20, &v[2][1], &v[2][0]);
        t = v[2][0];
        d = -0x40 - t;
        d = (s32)((d + ((u32)(d >> 1) >> 0x1E)) << 0xE) >> 0x10;
        if (t <= -0x34 || d == 0) {
            sel = p->sel - 1;
            if (sel < 0) {
                sel = 2;
            }
            ov96_0220C54C(p, 6, ov96_021E5F24(p->course), sel, 0);
            k = 0x28;
            v[2][1] = k;
            v[2][0] = k - 0x50;
            ov96_0220C90C(p, 6, k, k - 0x50);
            p->state++;
            return;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(p->spr20, v[2][1], t + d, 0x100000);
        ManagedSprite_GetPositionXYWithSubscreenOffset(p->spr28, &v[2][1], &v[2][0], 0x100000);
        d = 0x110 - v[2][0];
        if (d < 0) {
            d = -d;
        }
        v[2][0] += d >> 2;
        t = v[2][0];
        if (t > 0xD8) {
            t = 0xD8;
        }
        v[2][0] = t;
        ov96_0220C90C(p, 6, v[2][1], v[2][0]);
        return;
    case 3:
        ManagedSprite_GetPositionXYWithSubscreenOffset(p->spr24, &v[1][1], &v[1][0], 0x100000);
        t = v[1][0];
        d = 0x88 - t;
        if (d < 0) {
            d = -d;
        }
        step = (s32)(d << 0xF) >> 0x10;
        if (t < 0x88) {
            if (step == 0) {
                step = 1;
            }
            v[1][0] += step;
            ov96_0220C90C(p, 5, v[1][1], v[1][0]);
            ManagedSprite_GetPositionXYWithSubscreenOffset(p->spr28, &v[1][1], &v[1][0], 0x100000);
            v[1][0] += step;
            ov96_0220C90C(p, 6, v[1][1], v[1][0]);
        }
        p->cnt++;
        if (p->cnt < 4) {
            return;
        }
        {
            ManagedSprite *tmp = p->spr24;
            s32 tw;
            p->spr24 = p->spr28;
            p->spr28 = tmp;
            tw = p->w2C;
            p->w2C = p->w30;
            p->w30 = tw;
        }
        ov96_0220C90C(p, 5, 0x28, 0x30);
        ov96_0220C90C(p, 6, 0x28, 0x88);
        ManagedSprite_SetPositionXYWithSubscreenOffset(p->spr20, 0x88, -8, 0x100000);
        PlaySE(0x8B5);
        p->cnt = 0;
        p->state++;
        return;
    case 4:
        ManagedSprite_GetPositionXY(p->spr20, &v[0][1], &v[0][0]);
        d = -0x18 - v[0][0];
        if (d < 0) {
            d = -d;
        }
        e = (s16)d;
        if (e != 0) {
            e = (s32)((e + ((u32)e >> 0x1F)) << 0xF) >> 0x10;
        }
        v[0][0] += e + 2;
        t = v[0][0];
        if (t >= 0x70) {
            PlaySE(0x8B6);
            ManagedSprite_SetPositionXY(p->spr20, 0x88, 0x70);
            ManagedSprite_SetAnimateFlag(p->spr20, 1);
            ManagedSprite_ResetSpriteAnimCtrlState(p->spr1C);
            p->state++;
            return;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(p->spr20, v[0][1], t, 0x100000);
        return;
    case 5:
        p->cnt++;
        if (p->cnt >= 2) {
            ManagedSprite_SetDrawFlag(p->spr20, 0);
            ov96_0220C54C(p, 0, ov96_021E5F24(p->course), p->sel, 1);
            p->cnt = 0;
            p->state++;
            return;
        }
        return;
    case 6:
        ov96_0220C578(p, 0);
        return;
    default:
        GF_AssertFail();
        break;
    }
}
