#include "global.h"

#include "overlay_96_ov96_rower_work.h"
#include "overlay_96_sprite_res_man.h"


void ov96_021EB52C(void *, u32, u32);

void ov96_021EB588(void *, s32 *);

void *ov96_021EB594(void *, s32 *, u32);

void ov96_021FFE38(s32, u32, u32);

void ov96_021FED3C(Ov96Rower *param_1, s32 *param_2, u32 param_3, s32 param_4, u8 *param_5, s32 param_6, u32 param_7);

void ov96_021FED3C(Ov96Rower *param_1, s32 *param_2, u32 param_3, s32 param_4, u8 *param_5, s32 param_6, u32 param_7) {
    void *iVar1;
    VecFx32 v1;
    VecFx32 v0;

    switch (param_1->unkA0) {
    case 0:
        v1 = *(VecFx32 *)param_2;
        v1.y -= 0x190000;
        ov96_021EB588(param_1->unk74, (s32 *)&v1);
        ov96_021EB52C(param_1->unk74, 1, 1);
        ov96_021EAB38(param_1->unk00[param_4], 0);
        param_1->unkA1 = 0;
        ov96_021FFE38((s32)(*param_2 + ((u32)(*param_2 >> 0xb) >> 0x14)) >> 0xc, 0x8b5, param_7);
        param_1->unkA0 = param_1->unkA0 + 1;
        return;
    case 1:
        iVar1 = ov96_021EB594(param_1->unk74, param_2, param_3);
        v0 = *(VecFx32 *)iVar1;
        v0.y += 0x14000;
        v0.x = *param_2;
        ov96_021EB588(param_1->unk74, (s32 *)&v0);
        param_1->unkA1 = param_1->unkA1 + 1;
        if (param_1->unkA1 >= 0x14) {
            ov96_021EB588(param_1->unk70, param_2);
            param_1->unkA0 = param_1->unkA0 + 1;
            return;
        }
        break;
    case 2:
        if (param_6 != 0) {
            *(u8 *)(param_5 + 8) = 2;
        }
        ov96_021EB588(param_1->unk74, param_2);
        ov96_021EB588(param_1->unk70, param_2);
        param_1->unkA0 = param_1->unkA0 + 1;
    case 3:
        ov96_021EB588(param_1->unk74, param_2);
        ov96_021EB588(param_1->unk70, param_2);
    }
    return;
}
