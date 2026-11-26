#include "global.h"

#include "constants/heap.h"

#include "field/overlay_01_021FB878.h"

#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "palette.h"
#include "save.h"
#include "sys_task.h"
#include "overlay_92_unk_struct_ov116_0226139_c.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "overlay_01_unk_struct_ov83_0223_da94.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_91_unk_struct_ov115_02260440.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"

struct G3DPipelineBuffers;
struct ManagedSprite;
struct SpriteManager;
struct SpriteSystem;
struct PlayerProfile;
struct UnkStruct_ov114_02025CAD4_t;
struct UnkStruct_ov114_0225D678_t;
struct UnkStruct_ov114_0225E854_t;
struct UnkStruct_ov66_0222DFF8_t;
typedef struct UnkStruct_ov114_0225D678_t UnkStruct_ov114_0225D678;
typedef struct UnkStruct_ov114_0225E854_t UnkStruct_ov114_0225E854;
typedef struct {
    u8 unk_00;
    u8 padding_01[3];
    u32 unk_04;
} UnkStruct_ov116_0226048C;
typedef struct {
    u32 unk_00;
    u8 padding_04[4];
} UnkStruct_ov116_02260494;
typedef struct {
    UnkStruct_ov116_02260494 unk_00[4];
} UnkStruct_ov116_02260498;
typedef struct G3DPipelineBuffers G3DPipelineBuffers;
typedef struct {
    u8 padding_00[4];
    int unk_04;
    s16 unk_08;
    u8 padding_0A;
    UnkStruct_ov116_022649E4 *unk_0C;
} UnkStruct_ov116_02262A44;
enum RotationAxis {
    ROTATION_AXIS_X = 0,
    ROTATION_AXIS_Y,
    ROTATION_AXIS_Z,
    ROTATION_AXIS_COUNT
};
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
static inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
static inline void inline_ov61_0222C3B0(UnkStruct_ov61_0222C3B0 *param0, NARC *param1, int param2, enum HeapID heapID) {
    NNSG2dPaletteData *v0;
    void *v1;

    MI_CpuClear8(param0, sizeof(UnkStruct_ov61_0222C3B0));

    v1 = GfGfxLoader_GetPlttDataFromOpenNarc(param1, param2, &v0, heapID);

    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_08, 4 * 0x20);
    MI_CpuCopy16(&((u16 *)(v0->pRawData))[0 * 16], param0->unk_88, 4 * 0x20);
    Heap_Free(v1);
    inline_ov61_0222C3B0_sub_1(param0);

    param0->unk_04 = 1;
    param0->unk_00 = SysTask_ExecuteOnVBlank(inline_ov61_0222C3B0_sub, param0, 20);
}
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

int ov92_0226077C(UnkStruct_ov116_0226139C *param0, int param1);
void ov92_02260798(UnkStruct_ov83_0223DA94 *param0, fx32 param1, fx32 param2, fx32 param3, int param4);
BOOL ov92_022607F8(UnkStruct_ov83_0223DA94 *param0);
void ov92_02260860(UnkStruct_ov116_0226469C *param0, fx32 param1, fx32 param2, s32 param3);
BOOL ov92_02260870(UnkStruct_ov116_0226469C *param0);

int ov92_0226077C(UnkStruct_ov116_0226139C *param0, int param1) {
    int v0;

    for (v0 = 0; v0 < 4; v0++) {
        if (param0->unk_B4[v0] == param1) {
            return v0;
        }
    }

    return 0xFF;
}

void ov92_02260798(UnkStruct_ov83_0223DA94 *param0, fx32 param1, fx32 param2, fx32 param3, int param4) {
    fx32 v0;
    fx32 v1;
    fx32 v2;
    fx32 v3;

    v2 = param2 - param1;

    v0 = (param4 * param4) << 12;
    v1 = FX_MulInline(param3, param4 * ((fx32)0x0000000000001000L));
    v1 = v2 - v1;
    v1 = FX_MulInline(v1, 2 * ((fx32)0x0000000000001000L));
    v3 = FX_Div(v1, v0);

    param0->unk_00 = param1;
    param0->unk_04 = param1;
    param0->unk_08 = param3;
    param0->unk_0C = v3;
    param0->unk_10 = 0;
    param0->unk_14 = param4;
}

BOOL ov92_022607F8(UnkStruct_ov83_0223DA94 *param0) {
    fx32 v0;
    fx32 v1;
    fx32 v2;
    fx32 v3;

    v3 = FX_MulInline(param0->unk_08, param0->unk_10 << 12);
    v1 = (param0->unk_10 * param0->unk_10) << 12;
    v2 = FX_MulInline(param0->unk_0C, v1);
    v2 = FX_Div(v2, 2 * ((fx32)0x0000000000001000L));
    v0 = v3 + v2;

    param0->unk_00 = param0->unk_04 + v0;

    if ((param0->unk_10 + 1) <= param0->unk_14) {
        param0->unk_10++;
        return 0;
    }

    param0->unk_10 = param0->unk_14;
    return 1;
}

void ov92_02260860(UnkStruct_ov116_0226469C *param0, fx32 param1, fx32 param2, s32 param3) {
    param0->unk_00 = param1;
    param0->unk_04 = param1;
    param0->unk_0C = param2 - param1;
    param0->unk_10 = 0;
    param0->unk_14 = param3;
}

BOOL ov92_02260870(UnkStruct_ov116_0226469C *param0) {
    s32 v0;
    BOOL v1;
    fx32 v2;

    if (param0->unk_10 >= param0->unk_14) {
        v0 = param0->unk_14;
        v1 = 1;
    } else {
        v0 = param0->unk_10;
        param0->unk_10++;
        v1 = 0;
    }

    v2 = FX_MulInline(param0->unk_0C, v0 << 12);
    v2 = FX_Div(v2, param0->unk_14 << 12);
    v2 += param0->unk_04;

    param0->unk_00 = v2;

    return v1;
}
