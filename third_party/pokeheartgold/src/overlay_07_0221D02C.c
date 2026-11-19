#include "global.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

struct BattleAnimContext;
struct BattleAnimSystem;
struct BattleBgAnim;
struct BgTrackingTask;
struct SpriteTrackingTask;
struct SysTask;
struct SysTaskQueue;
void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);
typedef struct BattleAnimContext BattleAnimContext;
typedef struct BattleBgAnim BattleBgAnim;
typedef struct SpriteTrackingTask SpriteTrackingTask;
typedef struct BgTrackingTask BgTrackingTask;
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
void Heap_Free(void *ptr);
void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);
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
static inline void BattleAnimScript_Next(BattleAnimSystem *system);
static inline void BattleAnimScript_JumpBy(BattleAnimSystem *system, u32 offset);
static inline int BattleAnimScript_ReadWord(u32 *scriptPtr);
static inline void BattleAnimScript_Next(BattleAnimSystem *system) {
    system->scriptPtr += 1;
}
static inline void BattleAnimScript_JumpBy(BattleAnimSystem *system, u32 offset) {
    system->scriptPtr += offset;
}
static inline int BattleAnimScript_ReadWord(u32 *scriptPtr) {
    return *scriptPtr;
}

void ov07_0221D02C(BattleAnimSystem *system);

void ov07_0221D02C(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);

    for (int i = 0; i < 3; i++) {
        if (system->callStack[i] != 0L) {
            continue;
        }

        system->callStack[i] = system->scriptPtr + 1;
        BattleAnimScript_JumpBy(system, (u32)BattleAnimScript_ReadWord(system->scriptPtr));

        return;
    }
}
