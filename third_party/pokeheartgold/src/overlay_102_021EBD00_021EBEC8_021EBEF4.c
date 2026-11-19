#include "global.h"

#include "sprite.h"

typedef struct UnkStruct_ov102_021EBEF4 {
    u8 unk_0[0x4];
    void *unk_4;
    u8 unk_8[0x28];
    Sprite *unk_30;
    Sprite *unk_34;
    Sprite *unk_38;
} UnkStruct_ov102_021EBEF4;

s32 ov102_021E9050(u8 *arg0);

s32 ov102_021E9064(u8 *arg0);

void ov102_021EBEF4(UnkStruct_ov102_021EBEF4 *param_1, s32 param_2);

void ov102_021EBEF4(UnkStruct_ov102_021EBEF4 *param_1, s32 param_2) {
    s32 sVar1;

    if (param_2 != 0) {
        Sprite_SetDrawFlag(param_1->unk_30, 1);
        sVar1 = ov102_021E9050(param_1->unk_4);
        Sprite_SetDrawFlag(param_1->unk_34, sVar1);
        sVar1 = ov102_021E9064(param_1->unk_4);
        Sprite_SetDrawFlag(param_1->unk_38, sVar1);
        return;
    }
    Sprite_SetDrawFlag(param_1->unk_30, param_2);
    Sprite_SetDrawFlag(param_1->unk_34, 0);
    Sprite_SetDrawFlag(param_1->unk_38, 0);
    return;
}
