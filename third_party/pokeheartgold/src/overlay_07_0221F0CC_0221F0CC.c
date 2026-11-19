#include "global.h"

#include "unk_02005D10.h"
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

s8 ov07_0221F8C8(BattleAnimSystem *system, s8 pan);

static inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *param0);

static inline void inline_ov61_0222C3B0_sub(SysTask *param0, void *param1);

void ov07_0221F398(void);

SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
void Heap_Free(void *ptr);
void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);
inline void inline_ov61_0222C3B0_sub_1(UnkStruct_ov61_0222C3B0 *);
inline void inline_ov61_0222C3B0_sub(SysTask *, void *);
BOOL GF_IsAnySEPlaying(void);
static inline void BattleAnimScript_Next(BattleAnimSystem *system);
static inline int BattleAnimScript_ReadWord(u32 *scriptPtr);
void ov07_0221C69C(void);
void ov07_0221F39C(BattleAnimSystem *system);
void ov07_0221F3F4(BattleAnimSystem *system);
void ov07_0221F424(void);

void ov07_0221F42C(void);

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

void sub_020061B4(u16 seqID, u16 tracks, int pan);

static inline void BattleAnimScript_Next(BattleAnimSystem *system);

static inline int BattleAnimScript_ReadWord(u32 *scriptPtr);

static inline void BattleAnimScript_Next(BattleAnimSystem *system) {
    system->scriptPtr += 1;
}

static inline int BattleAnimScript_ReadWord(u32 *scriptPtr) {
    return *scriptPtr;
}

void ov07_0221F0CC(BattleAnimSystem *system);

void ov07_0221F0CC(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);

    u16 effectID = (u16)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    int pan = (int)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    pan = ov07_0221F8C8(system, pan);

    PlaySE(effectID);
    sub_020061B4(effectID, 0xFFFF, pan);
}

void sub_020061EC(int pan);

void ov07_0221F104(BattleAnimSystem *system);

void ov07_0221F104(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);

    int pan = (int)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    sub_020061EC(ov07_0221F8C8(system, pan));
}

s8 ov07_0221F980(s8 start, s8 end, s8 step);

typedef struct BattleAnimSoundContext {
    u8 type;

    u8 unused01;
    u8 unused02;

    u8 applyInterval;
    u8 tickCount;

    int startPan;
    int endPan;
    int panStep;
    int curPan;

    u8 repeatCount;
    u16 effectID;

    u32 unused1C[7];

    BattleAnimSystem *system;
} BattleAnimSoundContext;

enum BattleAnimSoundTaskType {
    BATTLE_ANIM_SOUND_TASK_NONE = 0,
    BATTLE_ANIM_SOUND_TASK_PAN,
    BATTLE_ANIM_SOUND_TASK_PAN_2,
    BATTLE_ANIM_SOUND_TASK_PAN_3,
    BATTLE_ANIM_SOUND_TASK_REPEAT,
    BATTLE_ANIM_SOUND_TASK_DELAY,

    BATTLE_ANIM_SOUND_TASK_COUNT,
};

BattleAnimSoundContext *ov07_0221C53C(BattleAnimSystem *system);

void ov07_0221C56C(BattleAnimSystem *system, BattleAnimSoundContext *ctx);

void ov07_0221F120(BattleAnimSystem *system);

void ov07_0221F120(BattleAnimSystem *system) {
    BattleAnimSoundContext *ctx = ov07_0221C53C(system);
    memset(ctx, 0, sizeof(BattleAnimSoundContext));

    ctx->type = BATTLE_ANIM_SOUND_TASK_PAN;

    BattleAnimScript_Next(system);

    ctx->effectID = (u16)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->startPan = (int)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->endPan = (int)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->panStep = (int)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->applyInterval = (u8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->startPan = ov07_0221F8C8(system, ctx->startPan);
    ctx->endPan = ov07_0221F8C8(system, ctx->endPan);
    ctx->panStep = ov07_0221F980(ctx->startPan, ctx->endPan, ctx->panStep);

    PlaySE(ctx->effectID);
    sub_020061B4(ctx->effectID, 0xFFFF, ctx->startPan);

    ov07_0221C56C(system, ctx);
}

void ov07_0221F1C0(BattleAnimSystem *system);

void ov07_0221F1C0(BattleAnimSystem *system) {
    BattleAnimSoundContext *ctx = ov07_0221C53C(system);
    memset(ctx, 0, sizeof(BattleAnimSoundContext));

    ctx->type = BATTLE_ANIM_SOUND_TASK_PAN_2;
    BattleAnimScript_Next(system);

    ctx->effectID = (u16)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->startPan = (s8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->endPan = (s8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->panStep = (s8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->applyInterval = (u8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    PlaySE(ctx->effectID);
    sub_020061B4(ctx->effectID, 0xFFFF, ctx->startPan);

    ov07_0221C56C(system, ctx);
}

void ov07_0221F238(BattleAnimSystem *system);

void ov07_0221F238(BattleAnimSystem *system) {
    BattleAnimSoundContext *ctx = ov07_0221C53C(system);
    memset(ctx, 0, sizeof(BattleAnimSoundContext));

    ctx->type = BATTLE_ANIM_SOUND_TASK_PAN;
    BattleAnimScript_Next(system);

    ctx->effectID = (u16)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->startPan = (s8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->endPan = (s8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->panStep = (s8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->applyInterval = (u8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->startPan = ov07_0221F8C8(system, ctx->startPan);
    ctx->endPan = ov07_0221F8C8(system, ctx->endPan);
    ctx->panStep = ov07_0221F8C8(system, ctx->panStep);

    PlaySE(ctx->effectID);
    sub_020061B4(ctx->effectID, 0xFFFF, ctx->startPan);

    ov07_0221C56C(system, ctx);
}

void ov07_0221F2DC(BattleAnimSystem *system);

void ov07_0221F2DC(BattleAnimSystem *system) {
    BattleAnimSoundContext *ctx = ov07_0221C53C(system);
    memset(ctx, 0, sizeof(BattleAnimSoundContext));

    ctx->type = BATTLE_ANIM_SOUND_TASK_REPEAT;
    BattleAnimScript_Next(system);

    ctx->effectID = (u16)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->curPan = (s8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->applyInterval = (u8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->repeatCount = (u8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->tickCount = ctx->applyInterval;
    ctx->curPan = ov07_0221F8C8(system, ctx->curPan);

    ov07_0221C56C(system, ctx);
}

void ov07_0221F340(BattleAnimSystem *system);

void ov07_0221F340(BattleAnimSystem *system) {
    BattleAnimSoundContext *ctx = ov07_0221C53C(system);
    memset(ctx, 0, sizeof(BattleAnimSoundContext));

    ctx->type = BATTLE_ANIM_SOUND_TASK_DELAY;

    BattleAnimScript_Next(system);

    ctx->effectID = (u16)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->curPan = (s8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->applyInterval = (u8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    ctx->curPan = ov07_0221F8C8(system, ctx->curPan);

    ov07_0221C56C(system, ctx);
}

void ov07_0221F398(void) {
}

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

void ov07_0221F39C(BattleAnimSystem *system) {
    if (system->activeSoundTasks) {
        system->scriptDelay = 1;
        system->soundEffectWaitTimer = 0;
        return;
    }

    if (GF_IsAnySEPlaying()) {
        system->soundEffectWaitTimer++;

        if (system->soundEffectWaitTimer > 90) {
            system->soundEffectWaitTimer = 0;
        } else {
            system->scriptDelay = 1;
            return;
        }
    } else {
        system->scriptDelay = 0;
        system->soundEffectWaitTimer = 0;
        BattleAnimScript_Next(system);
    }
}

void ov07_0221F3F4(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);

    u16 ev1 = (u16)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    u16 ev2 = (u16)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    G2S_SetBlendAlpha(GX_BLEND_PLANEMASK_BG0, GX_BLEND_PLANEMASK_BG1, ev1, ev2);
}

void ov07_0221F424(void) {
    ov07_0221C69C();
}

void ov07_0221F42C(void) {
}
