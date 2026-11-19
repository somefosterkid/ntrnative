#include "global.h"

#include "overlay_93_thumb_2.h"
#include "field/overlay_01_021FB878.h"
#include "camera.h"
#include "error_handling.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "sprite_system.h"
#include "unk_02035900.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct OverlayManager;
struct TextOBJ;
struct GF3DVramMan;
struct MsgData;
struct ParticleSystem;
struct MessageFormat;
struct PlayerProfile;
struct UnkStruct_02012744_t;
struct UnkStruct_ov114_02025CAD4_t;
struct UnkStruct_ov117_02261280_t;
struct UnkStruct_ov117_022626B0_t;
struct UnkStruct_ov66_0222DFF8_t;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
BOOL ov93_02262748(UnkStruct_ov117_02261280_t *param0);
BOOL ov93_022627A4(UnkStruct_ov117_02261280_t *param0);

void ov93_0226273C(s32 arg0, s32 arg1, s32 arg2, void *arg3) {
    UnkStruct_ov117_02261280_t *v0 = arg3;

    v0->unk_2FC0 = 1;
}

BOOL ov93_02262748(UnkStruct_ov117_02261280_t *param0) {
    if (sub_02037030(25, NULL, 0) == 1) {
        return 1;
    }

    return 0;
}

void ov93_02262760(int param0, int param1, void *param2, void *param3) {
    UnkStruct_ov117_02261280_t *v0 = param3;
    u32 *v1 = param2;
    u32 v2;
    int v3;

    v2 = v1[0];

    for (v3 = 0; v3 < v0->unk_00->unk_30; v3++) {
        if (v0->unk_00->unk_2C[v3] == param0) {
            v0->unk_0C[v3] = v2;
            break;
        }
    }

    GF_ASSERT(v3 < v0->unk_00->unk_30);
    v0->unk_08++;
}

BOOL ov93_022627A4(UnkStruct_ov117_02261280_t *param0) {
    u32 v0 = 100;

    if (sub_02037030(23, &v0, sizeof(u32)) == 1) {
        return 1;
    }

    return 0;
}
