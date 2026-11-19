#include "global.h"

#include "math_util.h"
#include "sprite_system.h"

struct ManagedSprite;

struct SysTask;

struct SysTaskQueue;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct {
    ManagedSprite *unk_00[2];
    SysTask *unk_08;
    fx32 unk_0C;
    f32 unk_10;
    s16 unk_14;
    s16 unk_16;
    s8 unk_18;
    u8 unk_19;
    u8 padding_1A[2];
    int unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
    int unk_2C;
} UnkStruct_ov62_02230C28;

void ov40_0222D048(SysTask *param0, void *param1);

void ov40_0222D048(SysTask *param0, void *param1) {
    UnkStruct_ov62_02230C28 *v0 = param1;

    v0->unk_1C++;
    v0->unk_1C %= 2;

    if (v0->unk_1C) {
        return;
    }

    switch (v0->unk_2C) {
    case 0: {
        if (v0->unk_28 == 3) {
            v0->unk_2C++;
            v0->unk_28 = 0;
        } else {
            v0->unk_28++;
        }
    } break;
    case 1: {
        fx32 v1, v2;
        fx32 v3, v4;

        if (v0->unk_28 == 4) {
            v0->unk_2C++;
            v0->unk_28 = 0;
        } else {
            ManagedSprite_GetSpritePositionFxXY(v0->unk_00[0], &v1, &v2);
            ManagedSprite_GetSpritePositionFxXY(v0->unk_00[1], &v3, &v4);

            v1 = v1 + ((GF_SinDeg((v0->unk_20 * 0xffff) / 360) * 1) * (v0->unk_18 * -1));
            v3 = v1;

            v0->unk_20 += 32;
            v0->unk_20 %= 360;

            ManagedSprite_SetPositonFxXY(v0->unk_00[0], v1, v2);
            ManagedSprite_SetPositonFxXY(v0->unk_00[1], v3, v4);

            v0->unk_28++;
        }
    } break;

    case 2: {
        s16 v5, v6, v7;
        int v8[] = {
            90,
            135,
            270,
            45,
            225,
            0,
        };

        ManagedSprite_GetPositionXY(v0->unk_00[0], &v5, &v6);
        ManagedSprite_GetPositionXY(v0->unk_00[1], &v5, &v7);

        if (v0->unk_28 == 0) {
            v0->unk_14 = v0->unk_16 - v5;
            v0->unk_14 /= v0->unk_24;
            v0->unk_28++;
        } else if (v0->unk_28 == v0->unk_24 + 1) {
            v5 = v0->unk_16;
            v0->unk_20 = v8[v0->unk_19];
            v0->unk_2C++;
            v0->unk_28 = 0;
        } else {
            v5 += v0->unk_14;
            v0->unk_28++;
        }

        ManagedSprite_SetPositionXY(v0->unk_00[0], v5, v6);
        ManagedSprite_SetPositionXY(v0->unk_00[1], v5, v7);
    } break;
    case 3: {
        fx32 v9, v10;
        fx32 v11, v12;

        if (v0->unk_28 == 0) {
            ManagedSprite_GetSpritePositionFxXY(v0->unk_00[0], &v0->unk_0C, &v10);
            v0->unk_28++;
        }

        ManagedSprite_GetSpritePositionFxXY(v0->unk_00[0], &v9, &v10);
        ManagedSprite_GetSpritePositionFxXY(v0->unk_00[1], &v11, &v12);

        v9 = v0->unk_0C + ((GF_SinDeg((v0->unk_20 * 0xffff) / 360) * 3) * (v0->unk_18 * -1));
        v11 = v9;

        if (v0->unk_19 % 2) {
            v0->unk_20 -= 4;
        } else {
            v0->unk_20 += 4;
        }

        v0->unk_20 %= 360;

        ManagedSprite_SetPositonFxXY(v0->unk_00[0], v9, v10);
        ManagedSprite_SetPositonFxXY(v0->unk_00[1], v11, v12);
    } break;
    }

    if ((v0->unk_2C >= 2) && (v0->unk_10 > 0.1f)) {
        v0->unk_10 -= 0.10f;

        ManagedSprite_SetAffineScale(v0->unk_00[0], v0->unk_10, 1.0f);
        ManagedSprite_SetAffineScale(v0->unk_00[1], v0->unk_10, 1.0f);
    }
}
