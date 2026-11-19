#include "global.h"
#include "overlay_70_gtsapplication_state.h"

#include "error_handling.h"
#include "sprite.h"
#include "unk_0201956C.h"

typedef struct UnkStruct_ov70_02243848 {
    u8 unk_0[0xC];
    Sprite *unk_C;
    u8 unk_10[0x8];
    Sprite *unk_18;
    UnkStruct_0201956C *unk_1C;
    u8 unk_20[0x1C];
    s16 unk_3C;
    u16 unk_3E;
    u8 unk_40[0xC];
    u32 unk_4C;
    u32 unk_50;
} UnkStruct_ov70_02243848;

void ov70_02241DB4(void *, u32);

extern u32 ov70_02245F5C;

u32 ov70_02243848(UnkStruct_ov70_02243848 *param_1);

u32 ov70_02243848(UnkStruct_ov70_02243848 *param_1) {
    s16 sVar1;

    ov70_02241DB4(param_1, 6);
    Sprite_SetDrawFlag(param_1->unk_C, 0);
    sVar1 = param_1->unk_3C;
    if (sVar1 < 0) {
        if (sVar1 == -1) {
            return 0xfffffffe;
        }
        if (sVar1 == -2) {
            return 0;
        }
    } else {
        if (8 < sVar1) {
            GF_AssertFail();
        }
        if (*(s32 *)(&ov70_02245F5C + param_1->unk_3C * 2) == 1) {
            param_1->unk_3E = 0;
            sub_020198FC(param_1->unk_1C, 0, -4, 0, 3);
            if (param_1->unk_18 != NULL) {
                Sprite_SetDrawFlag(param_1->unk_18, 0);
            }
            Sprite_SetAnimCtrlSeq(param_1->unk_C, 0x32);
            param_1->unk_4C = 2;
            param_1->unk_50 = 0x18;
        } else {
            param_1->unk_4C = 0x14;
        }
    }
    return 0xffffffff;
}
