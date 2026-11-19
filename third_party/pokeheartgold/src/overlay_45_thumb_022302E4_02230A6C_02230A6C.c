#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"
#include "filesystem.h"
#include "overlay_45_unk_struct_ov66_02234798.h"
#include "gf_3d_loader.h"

struct GF_2DGfxRawResMan;

struct TextureResourceManager;

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov66_02234548_t;

struct UnkStruct_ov66_02234798_t;

typedef struct UnkStruct_ov66_02234548_t UnkStruct_ov66_02234548;

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

typedef struct TextureResourceManager TextureResourceManager;

typedef struct {
    Field3dModel unk_00;
    u32 unk_10;
} UnkStruct_ov66_02234958;

struct UnkStruct_ov66_02234548_t {
    GF_2DGfxRawResMan *unk_00;
    GF_2DGfxRawResMan *unk_04;
    TextureResourceManager *unk_08;
    BillboardList *unk_0C;
    UnkStruct_ov66_02234798 *unk_10;
    u32 unk_14;
    UnkStruct_ov66_02234958 unk_18;
    int unk_2C;
};

BOOL ov45_02230AA4(const UnkStruct_ov66_02234798 *param0);

UnkStruct_ov66_02234798 *ov45_02230A6C(UnkStruct_ov66_02234548 *param0);

UnkStruct_ov66_02234798 *ov45_02230A6C(UnkStruct_ov66_02234548 *param0) {
    int v0;

    for (v0 = 0; v0 < param0->unk_14; v0++) {
        if (ov45_02230AA4(&param0->unk_10[v0]) == 0) {
            return &param0->unk_10[v0];
        }
    }

    ((0) ? (void)0 : GF_AssertFail());
    return 0L;
}
