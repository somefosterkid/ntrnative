#include "global.h"

#include "math_util.h"
#include "sprite_system.h"
#include "overlay_112_pokewalker_app.h"

typedef struct UnkStruct_ov112_021EA9BC {
    u8 unk_0[0x1E564];
    ManagedSprite *unk_1E564;
    ManagedSprite *unk_1E568;
} UnkStruct_ov112_021EA9BC;

s32 ov112_021EA6D8(void *, s32, s32, s32, s32);

void ov112_021EA864(void *arg0);

void ov112_021EA984(Ov112PokewalkerApp *arg0);

s32 ov112_021EA9A0(u8 *arg0);
void ov112_021EA9BC(UnkStruct_ov112_021EA9BC *arg0, void *arg1);
void ov112_021EAA10(void *arg0);

void ov112_021EAA34(Ov112PokewalkerApp *arg0, u16 arg1, s32 arg2);

void ov112_021EA864(void *arg0) {
    void *v0;
    s16 v2;
    s32 v1;
    s32 v3;
    s32 v4;
    s32 v7;
    void *v5;
    struct ManagedSprite **v6;
    v0 = arg0;
    v7 = 0;
    v5 = (u8 *)arg0 + offsetof(Ov112PokewalkerApp, unk_1EC80);
    v6 = (struct ManagedSprite **)arg0;
loop_1:
    v4 = ((s32)((*(s8 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_2) - offsetof(Ov112PokewalkerApp, unk_1EC80))) * ((*(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_6) - offsetof(Ov112PokewalkerApp, unk_1EC80))) + 0xB4))) / 320;

    if ((*((u8 *)v5)) != 0) {
        *(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_A) - offsetof(Ov112PokewalkerApp, unk_1EC80)) = (s16)(GF_SinDeg(*(u16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_C) - offsetof(Ov112PokewalkerApp, unk_1EC80))) / 360);
        v1 = v4 * GF_CosDeg(*(u16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_C) - offsetof(Ov112PokewalkerApp, unk_1EC80)));
        *(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_8) - offsetof(Ov112PokewalkerApp, unk_1EC80)) = (s16)(((s32)(v1 + (((u32)(v1 >> 0xB)) >> 0x14))) >> 0xC);
        *(u16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_C) - offsetof(Ov112PokewalkerApp, unk_1EC80)) = (u16)((*(u16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_C) - offsetof(Ov112PokewalkerApp, unk_1EC80))) + 0xA);
        ov112_021EA6D8(v0, v7, ((s32)(((s32)(*(u16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_C) - offsetof(Ov112PokewalkerApp, unk_1EC80)))) % 360)) / 73, (*(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_4) - offsetof(Ov112PokewalkerApp, unk_1EC80))) + (*(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_8) - offsetof(Ov112PokewalkerApp, unk_1EC80))), (*(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_6) - offsetof(Ov112PokewalkerApp, unk_1EC80))) + (*(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_A) - offsetof(Ov112PokewalkerApp, unk_1EC80))));
        *(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_6) - offsetof(Ov112PokewalkerApp, unk_1EC80)) = (s16)((*(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_6) - offsetof(Ov112PokewalkerApp, unk_1EC80))) - (*(s8 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_3) - offsetof(Ov112PokewalkerApp, unk_1EC80))));
        *(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_E) - offsetof(Ov112PokewalkerApp, unk_1EC80)) = (s16)((*(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_E) - offsetof(Ov112PokewalkerApp, unk_1EC80))) - 1);
        v2 = *(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_6) - offsetof(Ov112PokewalkerApp, unk_1EC80));
        if (((((s32)v2) < (-0x6E)) || (((s32)(*(s16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_E) - offsetof(Ov112PokewalkerApp, unk_1EC80)))) < 0)) || (((s32)v2) > 0xA0)) {
            *((u8 *)v5) = 0U;
            ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)v6 + offsetof(Ov112PokewalkerApp, sprites[0x75])), 0);
            ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)v6 + offsetof(Ov112PokewalkerApp, sprites[0x11])), 0);
        }
        v3 = ((s32)(*(u16 *)((u8 *)v5 + offsetof(Ov112PokewalkerApp, unk_1EC80[0].unk_C) - offsetof(Ov112PokewalkerApp, unk_1EC80)))) % 360;
        if ((v3 >= 0) && (v3 <= 0xB4)) {
            ManagedSprite_SetDrawPriority(*(ManagedSprite **)((u8 *)v6 + offsetof(Ov112PokewalkerApp, sprites[0x75])), 1U);
            ManagedSprite_SetDrawPriority(*(ManagedSprite **)((u8 *)v6 + offsetof(Ov112PokewalkerApp, sprites[0x11])), 1U);
        } else {
            ManagedSprite_SetDrawPriority(*(ManagedSprite **)((u8 *)v6 + offsetof(Ov112PokewalkerApp, sprites[0x75])), 3U);
            ManagedSprite_SetDrawPriority(*(ManagedSprite **)((u8 *)v6 + offsetof(Ov112PokewalkerApp, sprites[0x11])), 3U);
        }
    }
    v7 += 1;
    v5 += sizeof(Ov112PokewalkerRec);
    v6 += 1;
    if (v7 < 0x64) {
        goto loop_1;
    }
}

void ov112_021EA984(Ov112PokewalkerApp *arg0) {
    s32 v1;
    void *v0;

    v1 = 0;
    v0 = arg0->unk_1EC80;
    do {
        *(s8 *)v0 = 0;
        *(s16 *)((u8 *)v0 + 0xE) = 0;
        v1 += 1;
        v0 += 0x10;
    } while (v1 < 0x64);
}

s32 ov112_021EA9A0(u8 *arg0) {
    s32 v1;
    u8 *v0;

    v0 = arg0;
    v1 = 0;
loop_1:
    if (*v0 == 0) {
        return v1;
    }
    v1 += 1;
    v0 += 0x10;
    if (v1 >= 0x64) {
        return -1;
    }
    goto loop_1;
}

void ov112_021EA9BC(UnkStruct_ov112_021EA9BC *arg0, void *arg1) {
    if ((s32) * (s16 *)((u8 *)arg1 + 6) < 0) {
        ManagedSprite_SetDrawFlag(arg0->unk_1E568, 1);
    } else {
        ManagedSprite_SetDrawFlag(arg0->unk_1E564, 1);
    }
    ManagedSprite_SetAnimateFlag(arg0->unk_1E568, 1);
    ManagedSprite_SetAnimateFlag(arg0->unk_1E564, 1);
    ManagedSprite_SetAnim(arg0->unk_1E568, 0xA);
    ManagedSprite_SetAnim(arg0->unk_1E564, 0x18);
}

void ov112_021EAA10(void *arg0) {
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)arg0 + 0x1E568), 0);
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)arg0 + 0x1E564), 0);
}

void ov112_021EAA34(Ov112PokewalkerApp *arg0, u16 x, s32 y) {
    if (y < 0) {
        ManagedSprite_SetPositionXYWithSubscreenOffset(arg0->sprites[14], x, (s16)(y + 0xD0), FX32_CONST(256));
        ManagedSprite_SetDrawFlag(arg0->sprites[14], 1);
        ManagedSprite_SetDrawFlag(arg0->sprites[13], 0);
        return;
    }
    ManagedSprite_SetPositionXY(arg0->sprites[13], x, (s16)y);
    ManagedSprite_SetDrawFlag(arg0->sprites[13], 1);
    ManagedSprite_SetDrawFlag(arg0->sprites[14], 0);
}
