#include "global.h"

#include "sprite.h"
#include "unk_struct_ov70_02242e58.h"

s32 ov70_02242390(UnkStruct_ov70_02242E58 *, u8 *);

u32 ov70_022433A8(UnkStruct_ov70_02242E58 *param_1);

u32 ov70_022433A8(UnkStruct_ov70_02242E58 *param_1) {
    u32 iVar1;

    iVar1 = ov70_02242390(param_1, param_1->unk_64);
    switch (iVar1) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        Sprite_SetDrawFlag(param_1->unk_C, 0);
        param_1->unk_4C = 10;
        param_1->unk_3E = (s16)iVar1;
        break;
    case -2:
        Sprite_SetDrawFlag(param_1->unk_C, 0);
        param_1->unk_4C = 10;
        param_1->unk_3E = -1;
        break;
    }
    return 0xffffffff;
}
