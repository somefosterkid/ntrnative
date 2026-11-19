#include "global.h"

#include "error_handling.h"
#include "sprite_system.h"
#include "unk_02035900.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct Camera;

struct CharTransferAllocation;

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct TextOBJ;

struct GF3DVramMan;

struct ManagedSprite;

struct MsgData;

struct PaletteData;

struct ParticleSystem;

struct SaveData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct PlayerProfile;

struct UnkStruct_02012744_t;

struct UnkStruct_ov114_02025CAD4_t;

struct UnkStruct_ov117_02261280_t;

struct UnkStruct_ov117_022626B0_t;

struct UnkStruct_ov66_0222DFF8_t;

struct Window;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

void Sprite_GetPositionXYWithSubscreenOffset2(ManagedSprite *managedSprite, s16 *outX, s16 *outY, fx32 offset);

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

BOOL ov93_0225DF38(UnkStruct_ov117_02261280_t *param0, int param1);

int ov93_0225E3C4(UnkStruct_ov117_02261280_t *param0, int param1);

enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

ManagedSprite *ov93_0226027C(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02266F10 *param1);

extern const UnkStruct_ov117_02266F10 ov93_02263010[5][4];

extern const struct {
    s16 unk_00;
    s16 unk_02;
} Unk_ov117_02266BB4[4];

int ov93_0225FF1C(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02264E84 *param1, UnkStruct_ov117_02263EF8 *param2);

int ov93_0225FF1C(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02264E84 *param1, UnkStruct_ov117_02263EF8 *param2) {
    int v0;
    const UnkStruct_ov117_02266F10 *v1;
    int v2;

    GF_ASSERT(param2->unk_00 == 0);

    MI_CpuClear8(param2, sizeof(UnkStruct_ov117_02263EF8));

    v0 = ov93_0225E3C4(param0, param1->unk_00);
    v1 = &ov93_02263010[param0->unk_00->unk_30][v0];

    param2->unk_04 = param1->unk_00;
    param2->unk_06 = param1->unk_02;
    param2->unk_08 = param1->unk_08;
    param2->unk_14 = v1;
    param2->unk_0E = param1->unk_01;
    param2->unk_10 = ov93_0226027C(param0, v1);

    if (param1->unk_00 == sub_0203769C()) {
        ov93_0225DF38(param0, param1->unk_04);
        param2->unk_0C = 25;
        ManagedSprite_SetDrawFlag(param2->unk_10, 0);
    }

    param2->unk_00 = 1;

    {
        s16 v3, v4;
        s32 v5 = 0;

        ManagedSprite_GetPositionXYWithSubscreenOffset(param2->unk_10, &v3, &v4, ((192 + 160) << FX32_SHIFT));

        switch (v1->unk_07) {
        case 0:
            v5 = 28 - v4;
            break;
        case 1:
            v5 = v3 - 204;
            break;
        case 2:
            v5 = v4 - 164;
            break;
        case 3:
            v5 = 52 - v3;
            break;
        }

        v2 = (v5 * FX32_ONE) / (FX32_CONST(5.0));
    }
    return v2;
}
