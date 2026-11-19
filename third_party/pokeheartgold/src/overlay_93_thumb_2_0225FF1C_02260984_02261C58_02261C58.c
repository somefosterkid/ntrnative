#include "global.h"

#include "sprite_system.h"
#include "unk_02035900.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "font.h"

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

struct Sprite;

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

typedef u32 TextColor;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

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

extern const struct {
    s16 unk_00;
    s16 unk_02;
} ov93_02262CB4[4];

extern const u16 ov93_02262C7A[4];

extern const u16 ov93_02262D2C[5][4];

extern const u8 ov93_02262CD8[5][4];

void ov93_02261EB8(BgConfig *param0, SpriteManager *param1, UnkStruct_02012744 *param2, UnkStruct_ov117_02265EB0 *param3, const String *param4, enum Font param5, TextColor param6, int param7, int param8, int param9, int param10, int param11, int param12, int param13, int param14);

void ov93_02261C58(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265C3C *param1, int param2, int param3, int param4);

void ov93_02261C58(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265C3C *param1, int param2, int param3, int param4) {
    int v0, v1, v2, v3;
    UnkStruct_ov117_02265C1C *v4 = NULL;

    if ((param2 == 0) || (param3 == sub_0203769C())) {
        return;
    }

    for (v0 = 0; v0 < 3; v0++) {
        if (param1->unk_00[v0].unk_15 == 0) {
            v4 = &param1->unk_00[v0];
            break;
        }
    }

    if (v4 == NULL) {
        return;
    }

    v2 = ov93_0225E3C4(param0, param3);
    v1 = ov93_02262D2C[param0->unk_00->unk_30][v2];

    ManagedSprite_SetPaletteOverride(v4->unk_00, v1);
    ManagedSprite_SetAnim(v4->unk_00, ov93_02262C7A[param2] + 0);

    v3 = ov93_02262CD8[param0->unk_00->unk_30][v2];
    v4->unk_10 = ov93_02262CB4[v3].unk_02 + -24;

    ManagedSprite_SetPositionXYWithSubscreenOffset(v4->unk_00, ov93_02262CB4[v3].unk_00, v4->unk_10 + -96, ((192 + 160) << FX32_SHIFT));
    Sprite_TickFrame(v4->unk_00->sprite);

    v4->unk_12 = param2;
    v4->unk_16 = param4 - (MATH_ABS(-96) * FX32_ONE) / 0x8000;
    v4->unk_13 = 0;
    v4->unk_15 = 1;
}
