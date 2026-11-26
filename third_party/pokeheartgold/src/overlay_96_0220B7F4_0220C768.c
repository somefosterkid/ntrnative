#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_96_unk_struct_ov96_0220_b9_a8.h"

s32 ov96_0220C768(ManagedSprite *arg0, s32 arg1, s32 arg2);

s32 ov96_0220C768(ManagedSprite *managedSprite, s32 numer, s32 denom) {
    f32 v2;
    s32 v0;
    s32 v1;

    v0 = (s32)(FX_Div(numer, denom) * 0x64) >> 0xC;
    if (v0 <= 0x28) {
        v1 = (0x64 - v0) * 2;
        if (v1 > 0) {
            v2 = 0.5f + (f32)(v1 << 0xC);
        } else {
            v2 = (f32)(v1 << 0xC) - 0.5f;
        }
        ManagedSprite_SetAnimSpeed(managedSprite, (s32)v2 / 100);
        return 1;
    }
    return 0;
}

void ov96_0220C7C4(u8 *param_1);

void ov96_0220C7C4(u8 *param_1) {
    *(u32 *)(param_1 + 0x44) = *(u32 *)(param_1 + 0x44) | 0x100;
    *(u32 *)(param_1 + 0x40) = *(u32 *)(param_1 + 0x40) & 0x1fffff;
    ManagedSprite_SetAnim(*(ManagedSprite **)(param_1 + 0x14), 0x10);
    ManagedSprite_SetAnimSpeed(*(ManagedSprite **)(param_1 + 0x14), 0x1000);
    PlaySE(0x8bf);
    return;
}

void ov96_0220C7FC(void *arg0);

void ov96_0220C7FC(void *arg0) {
    s32 v0;
    void *v1;

    *((s32 *)((u8 *)&((UnkStruct_ov96_0220B9A8 *)arg0)->pad_38[7] + 0x5)) = (s32)(*((s32 *)((u8 *)&((UnkStruct_ov96_0220B9A8 *)arg0)->pad_38[7] + 0x5)) & 0xFFFFFEFF);
    *((s32 *)((u8 *)&((UnkStruct_ov96_0220B9A8 *)arg0)->pad_38[7] + 0x1)) = (s32)((0xFFFFFEFFU >> 0xB) & *((s32 *)((u8 *)&((UnkStruct_ov96_0220B9A8 *)arg0)->pad_38[7] + 0x1)));
    *((s32 *)((u8 *)&((UnkStruct_ov96_0220B9A8 *)arg0)->pad_38[7] + 0x5)) = (s32)(*((s32 *)((u8 *)&((UnkStruct_ov96_0220B9A8 *)arg0)->pad_38[7] + 0x5)) & ~0xFF);
    v1 = &((UnkStruct_ov96_0220B9A8 *)arg0)->unk_34;
    v0 = ((u32)(*((s32 *)((u8 *)&((UnkStruct_ov96_0220B9A8 *)arg0)->pad_38[7] + 0x1)) << 0x1E) >> 0x1E) * 4;
    *(s32 *)(v1 + v0) = 0x280000 | (*(s32 *)(v1 + v0) & 0xFC03FFFF);
    ManagedSprite_SetDrawFlag(((UnkStruct_ov96_0220B9A8 *)arg0)->spr14, 0);
}
