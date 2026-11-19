#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_struct_ov96_0220b148.h"

void ov96_0220B148(void **param_1, s16 *param_2, s16 *param_3);

void ov96_0220B148(void **param_1, s16 *param_2, s16 *param_3) {
    if (param_1 == NULL) {
        GF_AssertFail();
    }
    ManagedSprite_GetPositionXY((ManagedSprite *)*param_1, param_2, param_3);
    return;
}

u32 ov96_0220B164(UnkStruct_ov96_0220B148 *arg0);

u32 ov96_0220B164(UnkStruct_ov96_0220B148 *arg0) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    return (u32)(arg0->unk_10 << 0xC) >> 0x18;
}
