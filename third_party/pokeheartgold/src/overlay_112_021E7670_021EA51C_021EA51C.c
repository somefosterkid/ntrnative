#include "global.h"

#include "sprite_system.h"

extern s32 ov112_021FECE4;

extern s32 ov112_021FECE5;

extern s32 ov112_021FECE6;

extern s32 ov112_021FECE7;

void ov112_021EA51C(void *arg0, s32 arg1);

void ov112_021EA51C(void *arg0, s32 arg1) {
    s32 temp_r4;

    temp_r4 = arg1;
    ManagedSprite_SetPositionXYWithSubscreenOffset(*(ManagedSprite **)((u8 *)arg0 + 0x1E530), (s16) * (u8 *)(&ov112_021FECE4 + temp_r4), (s16) * (u8 *)(&ov112_021FECE5 + temp_r4), FX32_CONST(256));
    ManagedSprite_SetAnim(*(ManagedSprite **)((u8 *)arg0 + 0x1E530), (s32) * (u8 *)(&ov112_021FECE6 + temp_r4));
    ManagedSprite_SetPaletteOverride(*(ManagedSprite **)((u8 *)arg0 + 0x1E530), (s32) * (u8 *)(&ov112_021FECE7 + temp_r4));
    ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)arg0 + 0x1E530), 1);
}
