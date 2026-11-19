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

typedef u32 TextColor;

UnkStruct_ov117_02264E84 *ov93_02260F84(UnkStruct_ov117_02261280_t *param0);

BOOL ov93_02260FB8(UnkStruct_ov117_02261280_t *param0, int param1);

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void ov93_022627E8(UnkStruct_ov117_02261280_t *param0, const UnkStruct_ov117_02264E84 *param1);

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

void ov117_02265428(UnkStruct_ov117_02265428 *param0, int param1);

int ov93_022614F4(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265428 **param1);

extern const struct {
    s16 unk_00;
    s16 unk_02;
} Unk_ov117_02266BB4[4];

void ov93_02261EB8(BgConfig *param0, SpriteManager *param1, UnkStruct_02012744 *param2, UnkStruct_ov117_02265EB0 *param3, const String *param4, enum Font param5, TextColor param6, int param7, int param8, int param9, int param10, int param11, int param12, int param13, int param14);

BOOL ov93_02260080(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02263EF8 *param1);

void ov93_02261528(UnkStruct_ov117_02265428 *param0, s8 param1);

extern const fx32 ov93_02262CA4[4];

BOOL ov93_02260080(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02263EF8 *param1) {
    s16 v0, v1, v2, v3;
    int v4 = 0;
    s32 v5, v6;
    f32 v7;
    fx32 v8;

    ManagedSprite_GetPositionXYWithSubscreenOffset(param1->unk_10, &v2, &v3, ((192 + 160) << FX32_SHIFT));

    switch (param1->unk_14->unk_07) {
    case 0:
        v6 = 28 - v3;

        ManagedSprite_AddSpritePrecisePositionXY(param1->unk_10, 0, (FX32_CONST(5.0)));
        ManagedSprite_GetPositionXYWithSubscreenOffset(param1->unk_10, &v0, &v1, ((192 + 160) << FX32_SHIFT));

        v5 = 28 - v1;

        if (v1 > (98 - 24)) {
            v4 = 1;
        }
        break;
    case 1:
        v6 = v2 - 204;

        ManagedSprite_AddSpritePrecisePositionXY(param1->unk_10, -(FX32_CONST(5.0)), 0);
        ManagedSprite_GetPositionXYWithSubscreenOffset(param1->unk_10, &v0, &v1, ((192 + 160) << FX32_SHIFT));

        v5 = v0 - 204;

        if (v0 < (128 + 24)) {
            v4 = 1;
        }
        break;
    case 2:
        v6 = v3 - 164;

        ManagedSprite_AddSpritePrecisePositionXY(param1->unk_10, 0, -(FX32_CONST(5.0)));
        ManagedSprite_GetPositionXYWithSubscreenOffset(param1->unk_10, &v0, &v1, ((192 + 160) << FX32_SHIFT));

        v5 = v1 - 164;

        if (v1 < (98 + 16)) {
            v4 = 1;
        }
        break;
    case 3:
        v6 = 52 - v2;

        ManagedSprite_AddSpritePrecisePositionXY(param1->unk_10, (FX32_CONST(5.0)), 0);
        ManagedSprite_GetPositionXYWithSubscreenOffset(param1->unk_10, &v0, &v1, ((192 + 160) << FX32_SHIFT));

        v5 = 52 - v0;

        if (v0 > (128 - 24)) {
            v4 = 1;
        }
        break;
    }
    if (v5 >= 0) {
        if (v5 < (24 / 2 + 2)) {
            v8 = FX32_ONE * v5 / (24 / 2 + 2);

            if (v8 < 0x400) {
                v8 = 0x400;
            }

            v7 = FX_FX32_TO_F32(v8);
            ManagedSprite_SetAffineScale(param1->unk_10, v7, v7);
        }
    } else {
        if (v5 > -(24 / 2 + 2)) {
            v8 = ov93_02262CA4[param1->unk_0E] * (-v5) / (24 / 2 + 2);

            if (v8 < 0x400) {
                v8 = 0x400;
            }

            v7 = FX_FX32_TO_F32(v8);
            ManagedSprite_SetAffineScale(param1->unk_10, v7, v7);
        }
    }
    if ((param1->unk_04 == sub_0203769C()) && (v6 >= 0) && (v5 <= 0)) {
        UnkStruct_ov117_02265428 *v9;

        param1->unk_0E = ov93_022614F4(param0, &v9);

        switch (param1->unk_0E) {
        case 1:
            param1->unk_08 *= 2;
            break;
        case 2:
            param1->unk_08 /= 2;
            break;
        case 3:
            param1->unk_08 *= 3;
            break;
        }

        ov93_02260FB8(param0, param1->unk_0E);

        if (param1->unk_0E != 0) {
            ov93_02261528(v9, 3);
        }

        {
            UnkStruct_ov117_02264E84 *v10;

            v10 = ov93_02260F84(param0);

            if (v10 != NULL) {
                ov93_022627E8(param0, v10);
            } else {
                (void)0;
            }
        }
    }

    if (v4 == 1) {
        return 1;
    }

    return 0;
}
