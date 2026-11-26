#include "global.h"

#include "constants/heap.h"

#include "bag.h"
#include "bag_types_def.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021E64D0(BoxPokemon *arg0);
s32 ov14_021E6094(void *, u32, s32, void *);
s32 ov14_021F2ED0(void *, u32, u32, s32);
s32 ov14_021F3844(UnkStruct_ov14_boxsys *, u16);

void ov14_021F3B3C(void *arg0);
s32 ov14_021F396C(struct UnkStruct_ov14_boxsys *, u32, s32);

s32 ov14_021E6070(void *, s32, s32, void *);
BoxPokemon *ov14_021E60C0(void *, u32, u32);
s32 ov14_021E7588(void *, s32);
s32 ov14_021F01D8(void *, s32);
s32 ov14_021F2A18(struct UnkStruct_ov14_boxsys *, s32, s32);
s32 ov14_021F3488(void *, s32, s32);
s32 ov14_021F39D0(struct UnkStruct_ov14_boxsys *);
s32 ov14_021F3F6C(void *);
s32 ov14_021F43F4(struct UnkStruct_ov14_boxsys *, s32);
s32 ov14_021F5FBC(void *, s32);
s32 ov14_021F6AC0(void *, s32, s32);

extern s32 ov14_021F7D3C;

void ov14_021EBB3C(UnkStruct_ov14_boxapp *arg0) {
    u8 sp0[2];
    BoxPokemon *temp_r6;
    s32 var_r4;
    s32 var_r4_2;
    u16 temp_r0;
    u8 temp_r2;
    u8 temp_r2_2;
    void *temp_r0_2;
    UnkStruct_ov14_boxsys *temp_r0_3;
    if (((u32)(arg0->unk21)) >= 0x1EU) {
        ov14_021F0B70(arg0);
        ov14_021F43F4(((void *)arg0->sys), 0);
        ov14_021F3488(arg0, 1, 1);
    }
    if ((*((s32 *)(((u8 *)(*((void **)arg0))) + 8))) != 3) {
        ov14_021F3F6C(arg0);
    }
    if (((u32)((*((s32 *)(((u8 *)(*((void **)arg0))) + 8))) - 2)) <= 1U) {
        ov14_021E87F4(arg0);
        ov14_021E82FC(arg0->sys->unk_2F0);
    }
    temp_r0 = arg0->unk_1C;
    if (temp_r0 == 0) {
        if ((*((s32 *)(((u8 *)(*((void **)arg0))) + 8))) == 3) {
            ov14_021F5FBC(arg0, 0);
            if (((u32)(arg0->unk21)) < 0x1EU) {
                ov14_021F3488(arg0, 0x81, 1);
                var_r4 = 0x75;
            } else {
                ov14_021F3488(arg0, 0x82, 1);
                ov14_021F6AC0(arg0, 7, 8);
                var_r4 = 0x8B;
            }
        } else {
            ov14_021F5EE4(arg0, &ov14_021F7D3C, 5);
            if (((u32)(arg0->unk21)) < 0x1EU) {
                var_r4 = 0xC;
            } else {
                ov14_021F6AC0(arg0, 5, 0xA);
                var_r4 = 0x24;
            }
        }
        ov14_021E83C4(arg0->sys->unk_2F0);
    } else {
        var_r4_2 = 0;
        if ((temp_r0 == 0x70) && (ov14_021E6070(arg0, arg0->unk21, 5, 0) != 0x1E7)) {
            var_r4_2 = 1;
        } else {
            temp_r6 = ov14_021E60C0(arg0, arg0->unk1F, arg0->unk21);
            ov14_021E6094(arg0, arg0->unk21, 6, &arg0->unk_1C);
            if (ov14_021E64D0(temp_r6) == 1) {
                temp_r2 = arg0->unk21;
                ov14_021F2ED0(arg0, arg0->unk1F, temp_r2, arg0->sys->unk_4094[temp_r2]);
            }
            Bag_TakeItem(arg0->bag, arg0->unk_1C, 1U, HEAP_ID_10);
        }
        temp_r2_2 = arg0->unk21;
        temp_r0_2 = *((void **)arg0);
        if (((u32)temp_r2_2) >= 0x1EU) {
            if ((*((s32 *)(((u8 *)(*((void **)arg0))) + 8))) == 3) {
                ov14_021F6AC0(arg0, 7, temp_r2_2 - 0x1E);
            } else {
                ov14_021F6AC0(arg0, 5, 0xA);
            }
        } else if ((*((s32 *)(((u8 *)(*((void **)arg0))) + 8))) == 3) {
            GridInputHandler_SetNextInput(arg0->sys->unk_2C, (s32)temp_r2_2);
            DpadMenuBox_GetPosition(GridInputHandler_GetDpadBox(arg0->sys->unk_2C, (s32)(arg0->unk21)), &sp0[1], &sp0[0]);
            ManagedSprite_SetPositionXY(arg0->sys->sprites[9], (s16)(sp0[1]), (s16)(sp0[0]));
        }
        ov14_021F2A18(((void *)arg0->sys), 9, 0);
        if ((*((s32 *)(((u8 *)(*((void **)arg0))) + 8))) == 3) {
            if (var_r4_2 == 0) {
                arg0->sys->unk_88C8 = (u16)(arg0->unk_1C);
                temp_r0_3 = arg0->sys;
                ov14_021F3844(temp_r0_3, temp_r0_3->unk_88C8);
                ov14_021F2A18(((void *)arg0->sys), 0xB, 1);
                if (((u32)(arg0->unk21)) >= 0x1EU) {
                    ov14_021F3488(arg0, 0x82, 1);
                    ov14_021F396C(((void *)arg0->sys), arg0->unk21, 1);
                } else {
                    ov14_021F3488(arg0, 0x81, 1);
                    ov14_021F396C(((void *)arg0->sys), arg0->unk21, 0);
                }
                ov14_021F39D0(((void *)arg0->sys));
                ov14_021F3B3C(((void *)arg0->sys));
            } else {
                arg0->unk_1C = 0U;
                if (((u32)(arg0->unk21)) >= 0x1EU) {
                    ov14_021F3488(arg0, 0x82, 1);
                } else {
                    ov14_021F3488(arg0, 0x81, 1);
                }
            }
            var_r4 = 0x7D;
        } else {
            if (var_r4_2 == 1) {
                arg0->unk_1C = 0U;
            }
            var_r4 = 0x12;
        }
    }
    ov14_021E7588(arg0, arg0->unk21);
    if ((*((s32 *)(((u8 *)(*((void **)arg0))) + 8))) != 3) {
        ov14_021E8874(((void *)arg0->sys));
    } else if ((arg0->unk_1C) != 0) {
        ov14_021E892C(arg0->sys->unk_2F0);
    }
    ov14_021F01D8(arg0, var_r4);
}
