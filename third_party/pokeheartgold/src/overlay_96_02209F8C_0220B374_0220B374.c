#include "global.h"

#include "constants/heap.h"

#include "pokeathlon/pokeathlon.h"

#include "heap.h"
#include "sprite.h"
#include "sprite_system.h"

s32 ov96_0220C490(void *, SpriteSystem *, SpriteManager *);

void ov96_0220C54C(void *, s32, s32, s32, BOOL);

#if defined(SDK_PORT) || defined(NONMATCHING)
ManagedSprite *ov96_0220D13C(SpriteSystem *, SpriteManager *, s16, u8, u16, u16);
#else
ManagedSprite *ov96_0220D13C(SpriteSystem *, SpriteManager *, s16, s16, s32, s32);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
ManagedSprite *ov96_0220D1A0(SpriteSystem *, SpriteManager *, s16, u8, u16, u16);
#else
ManagedSprite *ov96_0220D1A0(SpriteSystem *, SpriteManager *, s16, s16, s32, s32);
#endif

s32 ov96_0220D200(void *, s32, PokeathlonCourseData *);

void *ov96_0220B374(enum HeapID arg0, SpriteSystem *arg1, SpriteManager *arg2, PokeathlonCourseData *arg3);

void *ov96_0220B374(enum HeapID arg0, SpriteSystem *arg1, SpriteManager *arg2, PokeathlonCourseData *arg3) {
    s16 sp10[4];
    u8 spC;
    PokeathlonCourseData *sp8;
    ManagedSprite *temp_r0_2;
    ManagedSprite *temp_r0_3;
    ManagedSprite *temp_r0_4;
    ManagedSprite *temp_r0_5;
    ManagedSprite *temp_r0_6;
    ManagedSprite *temp_r0_7;
    enum HeapID var_r7;
    SpriteSystem *var_r5;
    SpriteManager *var_r6;
    void *temp_r0;
    void *var_r4;

    var_r7 = arg0;
    var_r5 = arg1;
    var_r6 = arg2;
    sp8 = arg3;
    spC = ov96_021E5F24(arg3);
    temp_r0 = Heap_Alloc(var_r7, 0x48U);
    var_r4 = temp_r0;
    MI_CpuFill8(temp_r0, 0U, 0x48U);
    *(enum HeapID *)var_r4 = var_r7;
    *(SpriteSystem **)((u8 *)var_r4 + 4) = var_r5;
    *(SpriteManager **)((u8 *)var_r4 + 8) = var_r6;
    *(PokeathlonCourseData **)((u8 *)var_r4 + 0xC) = sp8;
    *(s32 *)((u8 *)var_r4 + 0x44) = (s32)(*(s32 *)((u8 *)var_r4 + 0x44) & ~0xFF);
    ov96_0220D200(var_r4 + 0x34, spC, sp8);
    ov96_0220C490(var_r4 + 0x10, *(void **)((u8 *)var_r4 + 4), *(void **)((u8 *)var_r4 + 8));
    ManagedSprite_GetPositionXYWithSubscreenOffset(*(ManagedSprite **)((u8 *)var_r4 + 0x24), &sp10[3], &sp10[2], FX32_CONST(256));
    sp10[2] = (s16)(sp10[2] - 0x18);
    temp_r0_2 = ov96_0220D1A0(var_r5, var_r6, sp10[3], sp10[2], 0x11, 0);
    *(ManagedSprite **)((u8 *)var_r4 + 0x2C) = temp_r0_2;
    ManagedSprite_SetDrawFlag(temp_r0_2, 0);
    ManagedSprite_GetPositionXYWithSubscreenOffset(*(ManagedSprite **)((u8 *)var_r4 + 0x28), &sp10[3], &sp10[2], FX32_CONST(256));
    sp10[2] = (s16)(sp10[2] - 0x18);
    temp_r0_3 = ov96_0220D1A0(var_r5, var_r6, sp10[3], sp10[2], 0x11, 0);
    *(ManagedSprite **)((u8 *)var_r4 + 0x30) = temp_r0_3;
    ManagedSprite_SetDrawFlag(temp_r0_3, 0);
    ManagedSprite_GetPositionXY(*(ManagedSprite **)((u8 *)var_r4 + 0x10), &sp10[1], &sp10[0]);
    temp_r0_4 = ov96_0220D13C(var_r5, var_r6, sp10[1], sp10[0], 0x11, 0x1A);
    *(ManagedSprite **)((u8 *)var_r4 + 0x14) = temp_r0_4;
    ManagedSprite_SetDrawFlag(temp_r0_4, 0);
    temp_r0_5 = ov96_0220D13C(var_r5, var_r6, sp10[1], sp10[0], 0xF, 0x17);
    *(ManagedSprite **)((u8 *)var_r4 + 0x1C) = temp_r0_5;
    ManagedSprite_SetDrawFlag(temp_r0_5, 0);
    temp_r0_6 = ov96_0220D13C(var_r5, var_r6, sp10[1], sp10[0], 0x1B, 0x18);
    *(ManagedSprite **)((u8 *)var_r4 + 0x20) = temp_r0_6;
    ManagedSprite_SetDrawFlag(temp_r0_6, 0);
    sp10[0] = (s16)(sp10[0] - 0x18);
    temp_r0_7 = ov96_0220D13C(var_r5, var_r6, sp10[1], sp10[0], 0x18, 0x19);
    *(ManagedSprite **)((u8 *)var_r4 + 0x18) = temp_r0_7;
    ManagedSprite_SetDrawFlag(temp_r0_7, 0);
    ov96_0220C54C(var_r4, 0, spC, 0, 1);
    ov96_0220C54C(var_r4, 5, spC, 2, 0);
    ov96_0220C54C(var_r4, 6, spC, 1, 0);
    return var_r4;
}
