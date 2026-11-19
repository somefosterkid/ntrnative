#include "global.h"

#include "screen_fade.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "bg_window.h"
#include "camera.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"

struct BgConfig;

struct Camera;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

struct SysTaskQueue;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

void ov92_02260798(UnkStruct_ov116_0226469C *param0, fx32 param1, fx32 param2, fx32 param3, int param4);

BOOL ov92_022607F8(UnkStruct_ov116_0226469C *param0);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0) {
    int v0, v1, v2, v3, v4, v5;
    v1 = 0;

    for (v0 = 0; v0 < 4 - 1; v0++) {
        v2 = v0 + 1;
        v3 = 0;
        v5 = 0;
        do {
            ((v1 < (((16 << 8) / 0x300 + 2) * (4 - 1))) ? (void)0 : GF_AssertFail());
            for (v4 = 1; v4 < 1 + 15; v4++) {
                BlendPalette(&param0->unk_08[v0][v4], &param0->unk_88[v1][v4], 1, v3 >> 8, param0->unk_08[v2][v4]);
            }
            v1++;
            if (v5 == 1) {
                break;
            }
            v3 += 0x300;
            if (v3 >= (16 << 8)) {
                v3 = (16 << 8);
                v5 = 1;
            }
        } while (1);
    }
    DC_FlushRange(param0->unk_88, (((16 << 8) / 0x300 + 2) * (4 - 1)) * 16 * sizeof(u16));
}

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1) {
    UnkStruct_ov61_0222C3B0 *v0 = param1;

    if (v0->unk_04 == 0) {
        return;
    }

    v0->unk_32B ^= 1;

    if (v0->unk_32B & 1) {
        return;
    }

    GX_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);
    GXS_LoadBGPltt((const void *)v0->unk_88[v0->unk_328], 0 * 0x20, 1 * 0x20);

    if (v0->unk_32A == 0) {
        v0->unk_328++;
        if (v0->unk_328 >= (((16 << 8) / 0x300 + 2) * (4 - 1))) {
            v0->unk_328 = (((16 << 8) / 0x300 + 2) * (4 - 1)) - 2;
            v0->unk_32A ^= 1;
        }
    } else {
        v0->unk_328--;
        if (v0->unk_328 < 0) {
            v0->unk_328 = 1;
            v0->unk_32A ^= 1;
        }
    }
}

void ov92_022608B8(SysTask *param0, void *param1);

void ov92_022608B8(SysTask *param0, void *param1) {
    UnkStruct_ov116_022647BC *v0 = param1;
    int v1;
    fx32 v2, v3;
    BOOL v4[4][2];
    BOOL v5 = 1;
    const fx32 v6[] = {
        FX32_CONST(-5),
        FX32_CONST(+3),
        FX32_CONST(+4),
        FX32_CONST(-3),
        FX32_CONST(+3),
        FX32_CONST(-2)
    };
    const fx32 v7[] = {
        FX32_CONST(+4),
        FX32_CONST(+5),
        FX32_CONST(-4),
        FX32_CONST(-5),
        FX32_CONST(-3),
        FX32_CONST(+2)
    };

    if ((IsPaletteFadeFinished() == FALSE) || (v0->unk_A4->unk_2C.unk_00 == 1)) {
        for (v1 = 0; v1 < 3; v1++) {
            Sprite_DeleteAndFreeResources(v0->unk_08[v1]);
        }

        v0->unk_00 = 0;
        SysTask_Destroy(param0);

        return;
    }

    switch (v0->unk_04) {
    case 0:
        for (v1 = 0; v1 < 3; v1++) {
            ManagedSprite_GetSpritePositionFxXY(v0->unk_08[v1], &v2, &v3);
            ov92_02260798(&v0->unk_14[v1][0], v2, v2 + v6[v1], FX_F32_TO_FX32(0.3f), 8);
            ov92_02260798(&v0->unk_14[v1][1], v3, v3 + v7[v1], FX_F32_TO_FX32(0.2f), 8);
        }

        v0->unk_04++;
        break;
    case 1:
        for (v1 = 0; v1 < 3; v1++) {
            v4[v1][0] = ov92_022607F8(&v0->unk_14[v1][0]);
            v4[v1][1] = ov92_022607F8(&v0->unk_14[v1][1]);

            ManagedSprite_SetPositonFxXY(v0->unk_08[v1], v0->unk_14[v1][0].unk_00, v0->unk_14[v1][1].unk_00);

            if ((v4[v1][0] == 0) || (v4[v1][1] == 0)) {
                v5 = 0;
            }

            ManagedSprite_TickFrame(v0->unk_08[v1]);
        }
        if (v5) {
            v0->unk_04++;
        }

        break;
    default:
        for (v1 = 0; v1 < 3; v1++) {
            Sprite_DeleteAndFreeResources(v0->unk_08[v1]);
        }

        v0->unk_00 = 0;

        SysTask_Destroy(param0);
        break;
    }
}
