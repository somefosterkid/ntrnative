#include "global.h"

#include "pokeathlon/pokeathlon.h"

#include "error_handling.h"
#include "sprite.h"
#include "overlay_96_unk_struct_ov96_021_fab24.h"
#include "overlay_96_sprite_res_man.h"

void ov96_021E8228(PokeathlonCourseData *param_1, u32 param_2, u32 param_3, s32 param_4, u32 param_5);


struct Sprite *ov96_021EB5B8(const SaveData *param_1, void *, s32);

void sub_0200606C(u32, s32);

void sub_020061D0(s32, s32);

extern const u8 ov96_0221DC28[];

extern const u8 ov96_0221DC2C[];

extern const u8 ov96_0221C444[];

void ov96_021FBCB8(PokeathlonCourseData *param_1, UnkStruct_ov96_021FAB24 *param_2, s32 arg2);

void ov96_021FBCB8(PokeathlonCourseData *param_1, UnkStruct_ov96_021FAB24 *param_2, s32 arg2) {
    u8 cVar1;
    Sprite *sprite;
    u32 uVar3;

    if (param_2->unk_9 < 4) {
        sprite = ov96_021EB5B8(param_2->unk_4C, param_2, arg2);
        param_2->unk_9 = param_2->unk_9 + 1;
        cVar1 = param_2->unk_9;
        if (cVar1 == 1) {
            sub_0200606C(0x8a6, ov96_0221DC2C[param_2->unk_18]);
            sub_020061D0(ov96_0221DC2C[param_2->unk_18],
                (s32)(s8)ov96_0221DC28[param_2->unk_18]);
            ov96_021EAC0C(param_2->unk_24, 5);
            Sprite_SetAnimCtrlSeq(sprite, 1);
        } else if (cVar1 == 4) {
            sub_0200606C(0x8a9, ov96_0221DC2C[param_2->unk_18]);
            sub_020061D0(ov96_0221DC2C[param_2->unk_18],
                (s32)(s8)ov96_0221DC28[param_2->unk_18]);
            ov96_021EAC0C(param_2->unk_24, 6);
            Sprite_SetAnimCtrlSeq(sprite, 2);
        } else {
            if ((u8)(cVar1 + 0xfe) <= 1) {
                sub_0200606C(0x8a7, ov96_0221DC2C[param_2->unk_18]);
                sub_020061D0(ov96_0221DC2C[param_2->unk_18],
                    (s32)(s8)ov96_0221DC28[param_2->unk_18]);
            }
        }
        if (param_2->unk_C < 10.0) {
            param_2->unk_C = param_2->unk_50 + *(float *)((param_2->unk_9 - 1) * 4 + ((u8 *)ov96_0221C444));
        } else {
            GF_AssertFail();
        }
    }
    uVar3 = ov96_021E5F24(param_1);
    ov96_021E8228(param_1, uVar3 & 0xff, (u32) param_2->unk_18, 6, 1);
    return;
}
