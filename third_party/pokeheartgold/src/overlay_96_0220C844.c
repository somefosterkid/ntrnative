#include "global.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "sprite.h"
#include "overlay_96_sprite_res_man.h"

void ov96_021EAF94(Ov96SpriteSetElem *elem, u32 param1, u32 param2);

void ov96_0220C844(Ov96SpriteSet *arg0, s32 arg1);

void ov96_0220C8B8(ManagedSprite *arg0, s32 arg1, s32 arg2);

void ov96_0220C90C(void *param_1, u32 param_2, s16 param_3, s16 param_4);

void ov96_0220C844(Ov96SpriteSet *arg0, s32 arg1) {
    s32 v2;
    s32 v1;
    Ov96SpriteSet *v0;
    s32 v3;
    Ov96SpriteSetElem *v4;
    s32 v6;
    s32 v7;
    s32 v8;
    u8 v5;

    v0 = arg0;
    v1 = arg1;
    v2 = 0;
    v6 = 0;
    v8 = 0x48;
    do {
        if (v2 != v1) {
            v7 = 0;
            do {
                v4 = ov96_021EAA04(v0, (u8)(v7 + v6));
                ov96_021EABA8(v4, 0x20);
                ov96_021EAF94(v4, v8, 0x30);
                ov96_021EAC0C(v4, 2);
                if (v7 == 0) {
                    v5 = 1;
                } else {
                    v5 = 0;
                }
                ov96_021EAB38(v4, v5);
                ov96_021EAB74(v4, 0);
                v7 += 1;
            } while (v7 < 3);
            v6 += 3;
            v8 += 0x40;
        }
        v3 = v2 + 1;
        v2 = v3;
    } while (v3 < 4);
}

void ov96_0220C8B8(ManagedSprite *managedSprite, s32 arg1, s32 arg2) {
    s32 v0;
    s32 speed;

    speed = 0;
    v0 = arg1 - 0x32;
    if (v0 != 0) {
        speed = (s32)(v0 << 0xC) / 10;
    }
    if (speed < 0x99A) {
        speed = 0x99A;
    }
    ManagedSprite_SetAnimSpeed(managedSprite, speed);
    ManagedSprite_SetAnimNoRestart(managedSprite, 0x11);
    if ((arg2 != 0) && (ManagedSprite_GetDrawFlag(managedSprite) == 0)) {
        PlaySE(0x8C0U);
    }
    ManagedSprite_SetDrawFlag(managedSprite, 1);
}

void ov96_0220C90C(void *param_1, u32 param_2, s16 param_3, s16 param_4) {
    ManagedSprite_SetPositionXYWithSubscreenOffset(*(ManagedSprite **)(param_1 + param_2 * 4 + 0x10), param_3, param_4, FX32_CONST(256));
    ManagedSprite_SetPositionXYWithSubscreenOffset(*(ManagedSprite **)(param_1 + (param_2 * 4 + 0x18)), param_3, param_4 + -0x18, FX32_CONST(256));
    return;
}
