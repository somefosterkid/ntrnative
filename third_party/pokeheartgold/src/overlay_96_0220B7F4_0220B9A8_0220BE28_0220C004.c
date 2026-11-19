#include "global.h"

#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_0220_be28.h"

s32 ov96_021EAC5C(u32 *, s32);

s32 ov96_0220BFB4(void *, s32);

s32 ov96_0220C768(ManagedSprite *, s32, s32);

s32 ov96_0220C8B8(ManagedSprite *, s32, s32, s32);

typedef struct {
    u32 unk0_0 : 8;
    u32 unk0_8 : 8;
    u32 unk0_16 : 8;
    u32 unk0_24 : 2;
    u32 unk0_26 : 1;
    u32 unk0_27 : 5;
} UnkStruct_ov96_0220C004;

void ov96_0220C004(void *arg0, UnkStruct_ov96_0220C004 *arg1, s32 arg2, s32 arg3);

void ov96_0220C004(void *arg0, UnkStruct_ov96_0220C004 *arg1, s32 arg2, s32 arg3) {
    u32 temp_r1;
    u32 temp_r1_2;
    if (arg1->unk0_26) {
        ManagedSprite_SetAnimNoRestart(((UnkStruct_ov96_0220BE28 *)arg0)->spr28, 0x10);
        ManagedSprite_SetDrawFlag(((UnkStruct_ov96_0220BE28 *)arg0)->spr28, 1);
    } else {
        temp_r1 = arg1->unk0_16;
        if (temp_r1 >= 0x32U) {
            ov96_0220C8B8(((UnkStruct_ov96_0220BE28 *)arg0)->spr28, (u16)temp_r1, 0, arg3);
        } else {
            ManagedSprite_SetDrawFlag(((UnkStruct_ov96_0220BE28 *)arg0)->spr28, 0);
        }
    }
    temp_r1_2 = arg1->unk0_8;
    if (temp_r1_2 == 0) {
        ov96_0220BFB4(arg0, 0x1A);
        ManagedSprite_SetDrawFlag(((UnkStruct_ov96_0220BE28 *)arg0)->spr28, 0);
        ov96_021EAC5C(*((void **)&((UnkStruct_ov96_0220BE28 *)arg0)->w4[(((u32)((*((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4))) << 7)) >> 0x1E)]), 0xC);
        return;
    }
    if (ov96_0220C768(((UnkStruct_ov96_0220BE28 *)arg0)->spr24, temp_r1_2 << 0xC, (*((s32 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w10[(((u32)((*((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4))) << 7)) >> 0x1E)])) << 0xC) != 0) {
        ov96_0220BFB4(arg0, 0x18);
        ov96_021EAC5C(*((void **)&((UnkStruct_ov96_0220BE28 *)arg0)->w4[(((u32)((*((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4))) << 7)) >> 0x1E)]), 0);
        return;
    }
    if (arg1->unk0_26) {
        ov96_021EAC5C(*((void **)&((UnkStruct_ov96_0220BE28 *)arg0)->w4[(((u32)((*((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4))) << 7)) >> 0x1E)]), 8);
        return;
    }
    ManagedSprite_SetDrawFlag(((UnkStruct_ov96_0220BE28 *)arg0)->spr24, 0);
    ov96_021EAC5C(*((void **)&((UnkStruct_ov96_0220BE28 *)arg0)->w4[(((u32)((*((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4))) << 7)) >> 0x1E)]), 4);
}
