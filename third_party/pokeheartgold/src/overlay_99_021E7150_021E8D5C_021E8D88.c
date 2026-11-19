#include "global.h"

#include "sprite_system.h"
#include "overlay_99_pokeathlon_event_record.h"

ManagedSprite *ov98_0221E6E0(void *, s32 *);

s32 ov99_021E8518(void *, u32, s32);

s32 ov99_021E86D4(void *, u8, s16, s16, s32, s32);

extern s32 ov99_021EA394;

void ov99_021E8D88(void *arg0);

void ov99_021E8D88(void *arg0) {
    s32 sp2C[13] = { 0 };
    u32 i;
    u32 j;
    u8 idx;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            idx = j + i * 5;
            MI_CpuCopy8(&ov99_021EA394, &sp2C[5], 0x18);
            sp2C[4] = 1;
            *(u16 *)&sp2C[0] = j * 0x30 + 0x20;
            *(s16 *)((u8 *)sp2C + 2) = i * 0x40 + 0x48;
            sp2C[2] = 0;
            *(u16 *)((u8 *)sp2C + 6) = 0x10;
            *(ManagedSprite **)((u8 *)arg0 + (idx + 0xC) * 4 + 0x18) = ov98_0221E6E0(*((void **)&((PokeathlonEventRecord *)arg0)->spriteContext), sp2C);
            ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)arg0 + (idx + 0xC) * 4 + 0x18), *(s32 *)((u8 *)arg0 + idx * 4 + 0xBC) == 0);
            sp2C[2] = 1;
            *(u16 *)((u8 *)sp2C + 6) = 4;
            *(ManagedSprite **)((u8 *)arg0 + (idx + 2) * 4 + 0x18) = ov98_0221E6E0(*((void **)&((PokeathlonEventRecord *)arg0)->spriteContext), sp2C);
            ManagedSprite_SetDrawFlag(*(ManagedSprite **)((u8 *)arg0 + (idx + 2) * 4 + 0x18), ov99_021E8518(arg0, (s8)idx, 1));
            ManagedSprite_SetAnimateFlag(*(ManagedSprite **)((u8 *)arg0 + (idx + 2) * 4 + 0x18), 1);
            ov99_021E86D4(arg0, idx, (u16)(j * 6 + 1), (u16)(i * 8 + 6), *(s32 *)((u8 *)arg0 + idx * 4 + 0xBC) == 0, 0);
        }
    }
}

void ov99_021E8EBC(void *arg0);

void ov99_021E8EBC(void *arg0) {
    s32 sp14[13] = { 0 };
    u32 i;
    u32 j;
    s32 idx;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 3; j++) {
            idx = j + i * 3;
            sp14[5] = idx + 2;
            sp14[6] = 2;
            sp14[7] = 2;
            sp14[8] = 2;
            sp14[4] = 2;
            *(u16 *)&sp14[0] = j * 0x18 + 0x3C + i * 8;
            *(s16 *)((u8 *)sp14 + 2) = i * 0x20 + 0x2C;
            sp14[2] = 3 - j;
            *(ManagedSprite **)((u8 *)arg0 + (idx + 0x16) * 4 + 0x18) = ov98_0221E6E0(*((void **)&((PokeathlonEventRecord *)arg0)->spriteContext), sp14);
            ManagedSprite_SetAnimateFlag(*(ManagedSprite **)((u8 *)arg0 + (idx + 0x16) * 4 + 0x18), 1);
        }
    }
}
