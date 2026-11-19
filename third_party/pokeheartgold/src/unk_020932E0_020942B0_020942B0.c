#include "global.h"

#include "sprite.h"
#include "touchscreen.h"
#include "unk_020932E0.h"

void sub_020942B0(UnkStruct_020932E0 *param_1) {
    s32 j;
    s32 i;
    s32 k;
    SpriteTemplate tmpl;
    sub_02094150(&tmpl, param_1, NNS_G2D_VRAM_TYPE_2DMAIN);
    k = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            sub_0209428C(&param_1->unk_7E4[k], 20 + j * 24, 40 + i * 24, 24, 24);
            tmpl.position.x = (20 + j * 24) << 12;
            tmpl.position.y = (40 + i * 24) << 12;
            if (param_1->unk_198[k].unk_0 == NULL) {
                param_1->unk_198[k].unk_0 = Sprite_CreateAffine(&tmpl);
            }
            Sprite_SetAnimActiveFlag(param_1->unk_198[k].unk_0, 1);
            Sprite_SetAnimCtrlSeq(param_1->unk_198[k].unk_0, k + 7);
            Sprite_SetPriority(param_1->unk_198[k].unk_0, 1);
            Sprite_SetDrawFlag(param_1->unk_198[k].unk_0, 0);
            Sprite_SetDrawPriority(param_1->unk_198[k].unk_0, 6);
            k++;
        }
    }
    tmpl.position.x = 0x14000;
    tmpl.position.y = 0x28000;
    if (param_1->unk_7B0.unk_0 == NULL) {
        param_1->unk_7B0.unk_0 = Sprite_CreateAffine(&tmpl);
    }
    Sprite_SetAnimActiveFlag(param_1->unk_7B0.unk_0, 1);
    Sprite_SetAnimCtrlSeq(param_1->unk_7B0.unk_0, 0x30);
    Sprite_SetPriority(param_1->unk_7B0.unk_0, 1);
    Sprite_SetDrawFlag(param_1->unk_7B0.unk_0, 0);
    Sprite_SetDrawPriority(param_1->unk_7B0.unk_0, 8);
    Sprite_SetOamMode(param_1->unk_7B0.unk_0, GX_OAM_MODE_XLU);
}
