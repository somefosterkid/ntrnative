#include "global.h"

#include "constants/heap.h"
#include "overlay_92_unk_struct_ov116_0226139_c.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262a8c.h"
#include "struct_defs/resdat_narc_structs.h"

struct G3DPipelineBuffers;

struct ManagedSprite;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct SysTask;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void ManagedSprite_TickTwoFrames(ManagedSprite *managedSprite);

void ov92_0225DF58(UnkStruct_ov116_02262A8C *param0, int param1, int param2);

int ov92_0226077C(UnkStruct_ov116_0226139C *param0, int param1);

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

extern const s16 ov92_02263B74[4][3];

void ov92_0225E6A4(UnkStruct_ov116_0226139C *param0, UnkStruct_ov116_02262A8C *param1);

void ov92_0225E6A4(UnkStruct_ov116_0226139C *param0, UnkStruct_ov116_02262A8C *param1) {
    ManagedSpriteTemplate v0;
    SpriteSystem *v1 = param1->unk_14->unk_08;
    SpriteManager *v2 = param1->unk_14->unk_0C;
    PaletteData *v3 = param1->unk_14->unk_14;
    int v4;
    int v5 = 0;

    for (v4 = 0; v4 < param1->unk_04; v4++) {
        param1->unk_DC[v4] = 0xFF;
        if (v4 == param1->unk_00) {
            continue;
        }

        param1->unk_DC[v4] = v5;
        ov92_0225DDD8(&v0, ov92_02263B74[param1->unk_04 - 1][v5] + 0, 176 + 64, 2, 0, 9000);
        v0.drawPriority = 1;

        param1->unk_1C[v5] = SpriteSystem_NewSprite(v1, v2, &v0);
        ov92_0225DDD8(&v0, ov92_02263B74[param1->unk_04 - 1][v5] + 0, (176 + 64) + 0, 2, 0, 9000);
        v0.drawPriority = 2;

        param1->unk_20[v5] = SpriteSystem_NewSprite(v1, v2, &v0);
        ManagedSprite_SetPaletteOverrideOffset(param1->unk_20[v5], 0 + ov92_0226077C(param0, v4));
        ov92_0225DDD8(&v0, ov92_02263B74[param1->unk_04 - 1][v5] + 0, (176 + 64) + 0, 2, 0, 9000);
        v0.drawPriority = 3;

        param1->unk_24[v5] = SpriteSystem_NewSprite(v1, v2, &v0);
        ManagedSprite_SetPaletteOverrideOffset(param1->unk_24[v5], 0 + ov92_0226077C(param0, v4));
        ManagedSprite_SetAnim(param1->unk_24[v5], 5);
        ManagedSprite_TickTwoFrames(param1->unk_1C[v5]);
        ManagedSprite_TickTwoFrames(param1->unk_20[v5]);
        ManagedSprite_TickTwoFrames(param1->unk_24[v5]);
        ov92_0225DF58(param1, v4, 0);

        v5++;
    }
}
