#include "global.h"

#include "sprite_system.h"
#include "overlay_96_unk_struct_ov96_0220_be28.h"

void ov96_0220C0DC(void *arg0, s32 *arg1);

void ov96_0220C0DC(void *arg0, s32 *arg1) {
    int new_var;
    ManagedSprite *temp_r0_2;
    unsigned int temp_r1;
    s32 temp_r4;
    u8 temp_r0;
    temp_r0 = (u8)(*arg1);
    if ((((u32)((*((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4))) << 9)) >> 0x18) != temp_r0) {
        temp_r1 = temp_r0 % 10U;
        temp_r4 = temp_r1 >> 1;
        new_var = 0xC - temp_r4;
        temp_r0_2 = ((UnkStruct_ov96_0220BE28 *)arg0)->spr1C;
        if (temp_r1 != 0) {
            ManagedSprite_SetAnimNoRestart(((UnkStruct_ov96_0220BE28 *)arg0)->spr1C, new_var);
        } else {
            ManagedSprite_SetAnimNoRestart(((UnkStruct_ov96_0220BE28 *)arg0)->spr1C, 0xD);
        }
        ManagedSprite_SetPositionXY(((UnkStruct_ov96_0220BE28 *)arg0)->spr20, (s16)(((((u32)((*((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4))) * 8)) >> 0x1E) << 6) + 0x48), (s16)((temp_r4 * 3) + 0x30));
        ManagedSprite_SetAnim(((UnkStruct_ov96_0220BE28 *)arg0)->spr20, 5);
        ManagedSprite_SetDrawFlag(((UnkStruct_ov96_0220BE28 *)arg0)->spr20, 1);
        *((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4)) = (s32)(((*((s32 *)((u8 *)&((UnkStruct_ov96_0220BE28 *)arg0)->w34 + 0x4))) & 0xFF807FFF) | (((u32)(((u8)(*arg1)) << 0x18)) >> 9));
    }
}
