#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "unk_struct_ov96_0220b148.h"

s32 ov96_0220B068(UnkStruct_ov96_0220B148 *);

void ov96_0220AF64(UnkStruct_ov96_0220B148 *arg0);

void ov96_0220AF64(UnkStruct_ov96_0220B148 *arg0) {
    s16 sp0[2];
    s32 temp_r1;
    s32 temp_r2;
    u32 temp_r4;

    temp_r1 = arg0->unk_10;
    if (((u32)(temp_r1 << 0xC) >> 0x18) == 1) {
        temp_r4 = 0xA8 - (((u32)(temp_r1 << 5) >> 0x19) * 4);
        temp_r2 = (0xFFFFF00F & temp_r1) | ((u32)((((u32)(temp_r1 << 0x14) >> 0x18) + 0x14) << 0x18) >> 0x14);
        arg0->unk_10 = temp_r2;
        ManagedSprite_OffsetPositionXY(arg0->unk_0, 0, (s16)((u32)(temp_r2 << 0x14) >> 0x18));
        ManagedSprite_OffsetPositionXY(arg0->unk_4, 0, (s16)((u32)(arg0->unk_10 << 0x14) >> 0x18));
        ManagedSprite_GetPositionXY(arg0->unk_4, &sp0[1], &sp0[0]);
        ManagedSprite_GetPositionXY(arg0->unk_0, &sp0[1], &sp0[0]);
        if ((u32) sp0[0] >= temp_r4) {
            sp0[0] = (s16)temp_r4;
            ManagedSprite_SetPositionXY(arg0->unk_0, sp0[1], sp0[0]);
            ov96_0220B068(arg0);
            arg0->unk_10 = (s32)(0x2000 | (arg0->unk_10 & 0xFFF00FFF));
        }
    }
}
