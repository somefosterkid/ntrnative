#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "gf_3d_loader.h"
#include "gf_3d_vramman.h"
#include "pm_string.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_49_unk_struct_ov70_0225_e9_c8.h"
#include "overlay_49_unk_struct_ov70_0225_e4_ec.h"

typedef struct {
    u32 unk_00;
    Window unk_04[5];
    Sprite *unk_54;
    SpriteResource *unk_58[4];
    Sprite *unk_68;
    SpriteResource *unk_6C[4];
    Sprite *unk_7C[12];
    SpriteResource *unk_AC[4];
    SpriteResource *unk_BC[12];
    SpriteResource *unk_EC[4];
    SpriteResource *unk_FC[4];
} UnkStruct_ov70_0225FA84;

s32 ov49_0225C148(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225E4EC *);

s32 ov49_0225C328(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225E4EC *);

s32 ov49_0225C78C(UnkStruct_ov70_0225FA84 *, UnkStruct_ov70_0225E4EC *);

void ov49_0225BF80(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225E4EC *param2);

void ov49_0225BFF0(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E4EC *param1, NARC *param2, u32 heapID, u32 param4, BOOL param5);
void ov49_0225BFC4(UnkStruct_ov70_0225FA84 *arg0, UnkStruct_ov70_0225E4EC *arg1, NARC *arg2, u32 arg3, u32 arg4);
s32 ov49_0225BFEC(const UnkStruct_ov70_0225FA84 *arg0);

void ov49_0225BF80(UnkStruct_ov70_0225FA84 *param0, UnkStruct_ov70_0225E9C8 *param1, UnkStruct_ov70_0225E4EC *param2) {
    if (param0->unk_00 == 1) {
        param0->unk_00 = 0;
    } else {
        ov49_0225C78C(param0, param2);
    }
    ov49_0225C148(param0, param2);
    ov49_0225C328(param0, param2);
    BgClearTilemapBufferAndCommit(param2->unk_00, 4U);
    BgClearTilemapBufferAndCommit(param2->unk_00, 5U);
    BgClearTilemapBufferAndCommit(param2->unk_00, 6U);
}

void ov49_0225BFC4(UnkStruct_ov70_0225FA84 *arg0, UnkStruct_ov70_0225E4EC *arg1, NARC *arg2, u32 arg3, u32 arg4) {
    ov49_0225C148(arg0, arg1);
    ov49_0225BFF0(arg0, arg1, arg2, arg3, arg4, 1);
}

s32 ov49_0225BFEC(const UnkStruct_ov70_0225FA84 *arg0) {
    return arg0->unk_00;
}
