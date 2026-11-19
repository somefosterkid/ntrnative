#include "global.h"

#include "overlay_05.h"
#include "sprite_system.h"
#include "unk_02005D10.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"

struct UnkStruct_ov05_0221C8A0 {
    u8 unk_0[0x1C8];
    ManagedSprite *unk_1C8;
    ManagedSprite *unk_1CC;
    u8 unk_1D0[0x9B1];
    s8 unk_B81;
};

s32 ov05_0221C8A0(UnkStruct_ov05_0221C8A0 *arg0) {
    PlaySE(0x71AU);
    ManagedSprite_SetDrawFlag(arg0->unk_1C8, 1);
    ManagedSprite_SetDrawFlag(arg0->unk_1CC, 1);
    ManagedSprite_SetAnim(arg0->unk_1C8, 0);
    arg0->unk_B81 = 0x20;
    return 1;
}

s32 ov05_0221C8E0(UnkStruct_ov10_0221FB28 *arg0) {
    PlaySE(0x71AU);
    ManagedSprite_SetDrawFlag(arg0->unk_198[12], 1);
    arg0->unk_B81 = 0x40;
    return 1;
}
