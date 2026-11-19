#include "global.h"

#include "sprite.h"
#include "unk_02005D10.h"
#include "unk_020932E0.h"

void sub_02093F84(UnkStruct_020932E0 *param_1, u8 param_2, u8 param_3, u32 param_4) {
    PlaySE(0x5ea);
    param_1->unk_884[param_2].unk_C = 0;
    Sprite_SetDrawFlag(param_1->unk_7E8[param_2].unk_0, 0);
    param_1->unk_46A4[param_2].unk_4 = 0;
    param_1->unk_46A4[param_3].unk_4 = 1;
    sub_02094668(param_1, param_3, param_2, 1);
    param_1->unk_884[param_3].unk_4 = param_1->unk_884[param_2].unk_4;
    param_1->unk_884[param_3].unk_0 = param_1->unk_884[param_2].unk_0;
    param_1->unk_884[param_3].unk_8 = param_1->unk_884[param_2].unk_8;
    param_1->unk_884[param_3].unk_C = 1;
    param_1->unk_46B0 = param_3;
}
