#include "global.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "sys_task.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "constants/heap.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "bg_window.h"
#include "camera.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "struct_defs/resdat_narc_structs.h"

struct G3DPipelineBuffers;

struct ManagedSprite;

struct NARC;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct ManagedSpriteTemplate;

struct SysTask;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void ManagedSprite_TickTwoFrames(ManagedSprite *managedSprite);

void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);

void Heap_Free(void *ptr);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

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

void ov92_0225DDD8(ManagedSpriteTemplate *param0, s16 param1, s16 param2, int param3, int param4, int param5);

void ov92_0225DE70(UnkStruct_ov116_02262A8C *param0);

typedef struct {
    s16 a[4];
} ov92_02263B54_Array;

extern const ov92_02263B54_Array ov92_02263B54;

void ov92_0225DE70(UnkStruct_ov116_02262A8C *param0) {
    ManagedSpriteTemplate v0;
    SpriteSystem *v1 = param0->unk_14->unk_08;
    SpriteManager *v2 = param0->unk_14->unk_0C;
    PaletteData *v3 = param0->unk_14->unk_14;
    int v4;
    ov92_02263B54_Array v5 = ov92_02263B54;

    for (v4 = 0; v4 < 4; v4++) {
        ov92_0225DDD8(&v0, v5.a[v4], 192 + 32, 1, 1, 9004);

        v0.bgPriority = 0;
        v0.drawPriority = 1;
        param0->unk_B8[v4] = SpriteSystem_NewSprite(v1, v2, &v0);

        ManagedSprite_TickTwoFrames(param0->unk_B8[v4]);
        ManagedSprite_SetPaletteOverrideOffset(param0->unk_B8[v4], 1);
    }
}
