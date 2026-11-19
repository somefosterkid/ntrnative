#include "global.h"
struct UnkStruct_0208C06C;

#include "sprite_system.h"
#include "unk_02013534.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_022368EC_v0 {
    u8 unk_0[0x34C];
    ManagedSprite *unk_34C;
    TextOBJ *unk_350;
    u8 unk_354[0x14];
    ManagedSprite *unk_368;
    TextOBJ *unk_36C;
} UnkStruct_ov40_022368EC_v0;

void ov40_022368EC(struct UnkStruct_0208C06C *arg0, s32 arg1);

void ov40_022368EC(UnkStruct_0208C06C *arg0, s32 arg1) {
    UnkStruct_ov40_022368EC_v0 *v0;

    v0 = arg0->unk_860;
    if (arg1 == 0) {
        ManagedSprite_SetDrawFlag(v0->unk_368, 1);
        TextOBJ_SetSpritesDrawFlag(v0->unk_36C, 1);
        ManagedSprite_SetPositionXY(v0->unk_34C, 0x20, 0xE8);
    } else {
        ManagedSprite_SetDrawFlag(v0->unk_368, 0);
        TextOBJ_SetSpritesDrawFlag(v0->unk_36C, 0);
        ManagedSprite_SetPositionXY(v0->unk_34C, 0x50, 0xE8);
    }
    sub_020136B4(v0->unk_350, 0x24, -8);
    sub_020136B4(v0->unk_36C, 0x24, -8);
}
