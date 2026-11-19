#include "global.h"

#include "overlay_40.h"
#include "sprite_system.h"
#include "unk_02013534.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_02241FD0_v0 {
    u8 unk_0[0x10];
    u8 unk_10;
    u8 unk_11[0x3];
    ManagedSprite *unk_14;
    TextOBJ *unk_18;
    u8 unk_1C[0x10];
    u8 unk_2C;
    u8 unk_2D[0x3];
    ManagedSprite *unk_30;
    TextOBJ *unk_34;
} UnkStruct_ov40_02241FD0_v0;

typedef struct UnkStruct_ov40_02241F74_v0 {
    u8 unk_0[0x14];
    ManagedSprite *unk_14;
    TextOBJ *unk_18;
    u8 unk_1C[0x14];
    ManagedSprite *unk_30;
    TextOBJ *unk_34;
} UnkStruct_ov40_02241F74_v0;

s32 ov40_0222BF80(s32, s32);

s32 ov40_02230944(s32);

s32 ov40_02241BDC(s32);

void ov40_02241F3C(s32 arg0, s32 arg1, s32 arg2);

void ov40_02241F3C(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 == 0) {
        switch (arg0) {
        case 0:
            ov40_02230944(arg2);
            ov40_02241BDC(arg2);
            ov40_0222BF80(arg2, 0x1C);
            return;
        case 1:
            ov40_02230944(arg2);
            ov40_0222BF80(arg2, 0x1B);
            break;
        }
    }
}

static void ov40_02241F74(UnkStruct_0208C06C *arg0, s32 arg1) {
    ManagedSprite *managedSprite;
    UnkStruct_ov40_02241F74_v0 *v0;

    v0 = arg0->unk_860;
    managedSprite = v0->unk_14;
    if (arg1 == 0) {
        ManagedSprite_SetDrawFlag(managedSprite, 1);
        TextOBJ_SetSpritesDrawFlag(v0->unk_18, 1);
        ManagedSprite_SetPositionXY(v0->unk_30, 0x80, 0xA8);
    } else {
        ManagedSprite_SetDrawFlag(managedSprite, 0);
        TextOBJ_SetSpritesDrawFlag(v0->unk_18, 0);
        ManagedSprite_SetPositionXY(v0->unk_30, 0x50, 0xA8);
    }
    sub_020136B4(v0->unk_18, 0x24, -8);
    sub_020136B4(v0->unk_34, 0x24, -8);
}

s32 ov40_0222D5AC(void *, void *, s32);

s32 ov40_0222D66C(void *, void *, s32);

s32 ov40_0222D6EC(void *, s32);

ManagedSprite *ov40_0222D800(void *, s32);

void ov40_02241FD0(UnkStruct_0208C06C *arg0);

void ov40_02241FD0(UnkStruct_0208C06C *arg0) {
    UnkStruct_ov40_02241FD0_v0 *v0;

    v0 = arg0->unk_860;
    ov40_0222D6EC(arg0, 1);
    v0->unk_14 = ov40_0222D800(arg0, 1);
    v0->unk_30 = ov40_0222D800(arg0, 1);
    ov40_0222D5AC((void *)&v0->unk_10, &arg0->unk_14, 1);
    ov40_0222D5AC((void *)&v0->unk_2C, &arg0->unk_14, 1);
    ov40_0222D66C((void *)&v0->unk_10, &arg0->unk_14, 3);
    ov40_0222D66C((void *)&v0->unk_2C, &arg0->unk_14, 0x6F);
    ManagedSprite_SetPositionXY(v0->unk_14, 0x20, 0xA8);
    ManagedSprite_SetPositionXY(v0->unk_30, 0x80, 0xA8);
    sub_020136B4(v0->unk_18, 0x24, -8);
    sub_020136B4(v0->unk_34, 0x24, -8);
    TextOBJ_SetSpritesDrawFlag(v0->unk_18, 1);
    TextOBJ_SetSpritesDrawFlag(v0->unk_34, 1);
    ManagedSprite_SetAnim(v0->unk_14, 0);
    ManagedSprite_SetAnim(v0->unk_30, 1);
    ov40_02241F74(arg0, 0);
}
