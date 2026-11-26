#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "filesystem.h"
#include "overlay_45_unk_struct_ov66_02234798.h"

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov66_02234798_t;

s32 ov42_02228188(const UnkStruct_ov63_0222BEC0 *param0, int param1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

void sub_02023EE0(Billboard *billboard, int animNum);

void sub_02023F40(Billboard *billboard, fx32 animFrameNum);

u32 ov45_022309C4(BOOL param0, u32 param1);

void ov45_02230D5C(UnkStruct_ov66_02234798 *param0);

s32 ov45_02230DC4(u32 arg0);

BOOL ov45_02230AA4(const UnkStruct_ov66_02234798 *param0);
void ov45_02230E28(UnkStruct_ov66_02234798 *param0);
BOOL ov45_02230E78(Billboard *param0);
void ov45_02230DF4(UnkStruct_ov66_02234798 *param0);
void sub_020182A0(Field3dObject *obj, BOOL visible);
void sub_02023EA4(Billboard *billboard, u8 draw);
NNSG3dResMdl *sub_02023FDC(Billboard *billboard);
void ov45_02230E64(Billboard *param0, void *param1);
const VecFx32 *sub_02023E94(const Billboard *billboard);
NNSG3dResMdl *sub_02023F90(const Billboard *billboard);
typedef struct GFXTestBox {
    fx16 width;
    fx16 height;
    fx16 depth;
    u8 padding[2];
    int xScale;
    int yScale;
    int zScale;
} GFXTestBox;
u32 sub_0201FA34(const VecFx32 *position, const GFXTestBox *box);

void ov45_02230D5C(UnkStruct_ov66_02234798 *param0) {
    u32 v0;
    u32 v1;

    if (param0->unk_02 == 0) {
        v0 = ov42_02228188(param0->unk_04, 6);
        v1 = ov45_022309C4(1, v0);

        sub_02023EE0(param0->unk_08, v1);
        sub_02023F40(param0->unk_08, 4 * ((fx32)0x0000000000001000L));
    } else if (param0->unk_02 == 4) {
        v0 = ov42_02228188(param0->unk_04, 6);
        v1 = ov45_022309C4(1, v0);

        sub_02023EE0(param0->unk_08, v1);
        sub_02023F40(param0->unk_08, (4 * 3) * ((fx32)0x0000000000001000L));
    }

    param0->unk_02 = (param0->unk_02 + 1) % (4 * 2);
}

s32 ov45_02230DC4(u32 arg0) {
    switch (arg0) {
    case 2:
    case 3:
    case 5:
    case 6:
    case 10:
    case 11:
        return 1;
    default:
        return 0;
    }
}

void ov45_02230DF4(UnkStruct_ov66_02234798 *param0) {
    BOOL v0;

    if (ov45_02230AA4(param0)) {
        v0 = ov45_02230E78(param0->unk_08);

        if (v0 == 0) {
            param0->unk_00_4 = 1;
        } else {
            param0->unk_00_4 = 0;
        }

        ov45_02230E28(param0);
    }
}

void ov45_02230E28(UnkStruct_ov66_02234798 *param0) {
    if ((param0->unk_00_4 == 0) && (param0->unk_00_6 == 1)) {
        sub_02023EA4(param0->unk_08, 1);
        sub_020182A0(&param0->unk_0C, 1);
    } else {
        sub_02023EA4(param0->unk_08, 0);
        sub_020182A0(&param0->unk_0C, 0);
    }
}

void ov45_02230E64(Billboard *param0, void *param1) {
    UnkStruct_ov66_02234798 *v0 = param1;
    NNSG3dResMdl *v1 = sub_02023FDC(param0);
    NNS_G3dMdlSetMdlLightEnableFlagAll(v1, v0->unk_8C);
}

BOOL ov45_02230E78(Billboard *param0) {
    BOOL v0;
    VecFx32 v1;
    NNSG3dResMdlInfo *v2;
    NNSG3dResMdl *v3;
    GFXTestBox v4;
    MtxFx33 v5;

    v3 = sub_02023F90(param0);
    v2 = NNS_G3dGetMdlInfo(v3);
    v1 = *sub_02023E68(param0);

    v4.width = v2->boxW;
    v4.height = v2->boxH;
    v4.depth = v2->boxH;
    v4.xScale = v2->boxPosScale >> FX32_SHIFT;
    v4.yScale = v2->boxPosScale >> FX32_SHIFT;
    v4.zScale = v2->boxPosScale >> FX32_SHIFT;

    v1.x += FX_Mul(v2->boxX, v2->boxPosScale);
    v1.y += FX_Mul(v2->boxY, v2->boxPosScale);
    v1.z += FX_Mul(v2->boxZ, v2->boxPosScale);
    v1.z -= FX_Mul(v2->boxH, v2->boxPosScale);

    MTX_Identity33(&v5);
    NNS_G3dGlbSetBaseRot(&v5);
    NNS_G3dGlbSetBaseScale(sub_02023E94(param0));

    v0 = sub_0201FA34(&v1, &v4);

    if (v0 == 0) {
        return 0;
    }

    return 1;
}
