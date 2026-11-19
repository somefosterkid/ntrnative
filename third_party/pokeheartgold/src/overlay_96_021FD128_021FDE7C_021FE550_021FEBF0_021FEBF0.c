#include "global.h"

#include "overlay_96_ov96_mon_icon_work.h"
#include "overlay_96_ov96_rower_work.h"
#include "overlay_96_sprite_res_man.h"


void ov96_021EB52C(s16 *param_1, s32 param_2, s32 param_3);

void ov96_021EB564(void *param_1, s32 param_2);

void ov96_021EB588(void *param_1, VecFx32 *param_2);

VecFx32 *ov96_021EB594(void *param_1);

void ov96_021FFE38(s32 param_1, u32 param_2, s32 param_3);

void ov96_021FEBF0(void *param_1, Ov96Rower *param_2, VecFx32 *param_3, u32 param_4, u8 param_5, u8 *param_6, s32 param_7, s32 param_8);

void ov96_021FEBF0(void *param_1, Ov96Rower *param_2, VecFx32 *param_3, u32 param_4, u8 param_5, u8 *param_6, s32 param_7, s32 param_8) {
    VecFx32 VStack_20;
    VecFx32 *pVar;
    switch (param_2->unkA0) {
    case 0:
        ov96_021EB588(param_2->unk74, param_3);
        ov96_021EB588(param_2->unk70, param_3);
        ov96_021EB52C(param_2->unk70, 1, 1);
        ov96_021EB564(param_2->unk70, 8);
        ov96_021EB52C(param_2->unk74, 1, 1);
        ov96_021EAB38(param_2->unk00[param_5], 0);
        param_2->unkC8 = 1;
        param_2->unkA1 = 0;
        ov96_021FFE38(param_3->x / 4096, 0x8b4, param_8);
        (param_2->unkA0)++;
        break;
    case 1:
        pVar = ov96_021EB594(param_2->unk74);
        VStack_20 = *pVar;
        VStack_20.y -= 0x14000;
        VStack_20.x = param_3->x;
        ov96_021EB588(param_2->unk74, &VStack_20);
        ov96_021EB588(param_2->unk70, param_3);
        (param_2->unkA1)++;
        if ((param_2->unkA1) >= 20) {
            (param_2->unkA0)++;
        }
        break;
    case 2:
        if (param_8 != 0) {
            if (ov96_022006BC(param_1, param_5) != 0) {
                param_6[8] = 1;
                (param_2->unkA0)++;
            }
        } else if (param_7 != 0) {
            if (ov96_02200900(param_1, param_4) != 0) {
                param_6[8] = 1;
                (param_2->unkA0)++;
            }
        } else {
            (param_2->unkA0)++;
        }
        break;
    case 3:
        break;
    }
}
