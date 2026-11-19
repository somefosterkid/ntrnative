#include "global.h"

#include "unk_02005D10.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "sys_task.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "screen_fade_internal.h"

struct BattleAnimContext;
struct BattleAnimSystem;
struct BattleBgAnim;
struct BgConfig;
struct BgTrackingTask;
struct SpriteTrackingTask;
struct SysTask;
struct SysTaskQueue;
SysTask *ov07_0221C5C8(SysTaskFunc callback, void *param, u32 priority);
void sub_020061B4(u16 seqID, u16 tracks, int pan);
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
BOOL ov07_0221C674(BattleAnimSoundContext *ctx);
BOOL ov07_0221C634(BattleAnimSoundContext *ctx);
typedef struct BgConfig BgConfig;
void BlendPalette(const u16 *src, u16 *dest, u16 size, u8 fraction, u16 target);
typedef struct BattleAnimContext BattleAnimContext;
typedef struct BattleBgAnim BattleBgAnim;
typedef struct SpriteTrackingTask SpriteTrackingTask;
typedef struct BgTrackingTask BgTrackingTask;
SysTask *SysTask_ExecuteOnVBlank(SysTaskFunc callback, void *param, u32 priority);
void Heap_Free(void *ptr);
void *GfGfxLoader_GetPlttDataFromOpenNarc(NARC *narc, u32 narcMemberIdx, NNSG2dPaletteData **outPaletteData, enum HeapID heapID);
typedef struct {
    SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
} UnkStruct_ov61_0222C3B0;
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
typedef void (*UnkCallback)(void);
typedef struct System {
    Callback vBlankIntr;
    void *vBlankIntrArg;
    Callback hBlankIntr;
    void *hBlankIntrArg;
    UnkCallback unk10;
    UnkCallback unk14;
    SysTaskQueue *mainTaskQueue;
    SysTaskQueue *vblankTaskQueue;
    SysTaskQueue *vwaitTaskQueue;
    SysTaskQueue *printTaskQueue;
    u32 *unk28;
    u32 vblankCounter;
    u32 frameCounter;
    int buttonMode;
    int heldKeysRaw;
    int newKeysRaw;
    int newAndRepeatedKeysRaw;
    int heldKeys;
    int newKeys;
    int newAndRepeatedKeys;
    int keyRepeatCounter;
    int keyRepeatContinueDelay;
    int keyRepeatStartDelay;
    int simulatedInputs;
    u16 touchX;
    u16 touchY;
    u16 touchNew;
    u16 touchHeld;
    u8 touchpadReadAuto;
    u8 screensFlipped;
    u8 unk6A;
    u8 lidClosedPauseDisabled;
    u8 softResetDisabled;
    BOOL unk70;
    u32 *unk74;
} System;
extern System gSystem;
static inline void BattleAnimScript_Next(BattleAnimSystem *system);
static inline void BattleAnimScript_Next(BattleAnimSystem *system) {
    system->scriptPtr += 1;
}
void ov07_0221BE20(BattleAnimSystem *system);
static inline int BattleAnimScript_ReadWord(u32 *scriptPtr);
static inline int BattleAnimScript_ReadWord(u32 *scriptPtr) {
    return *scriptPtr;
}

SysTask *ov07_0221C624(SysTaskFunc callback, void *param, int priority);
SysTask *ov07_0221C62C(SysTaskFunc callback, void *param, int priority);
BOOL ov07_0221C634(BattleAnimSoundContext *ctx);
BOOL ov07_0221C674(BattleAnimSoundContext *ctx);
void ov07_0221C69C(void);
void ov07_0221C6B4(BattleAnimSystem *system);
void ov07_0221C6EC(BattleAnimSystem *system);
void ov07_0221C70C(BattleAnimSystem *system);
void ov07_0221C72C(BattleAnimSystem *system);

SysTask *ov07_0221C624(SysTaskFunc callback, void *param, int priority) {
    return ov07_0221C5C8(callback, param, priority);
}

SysTask *ov07_0221C62C(SysTaskFunc callback, void *param, int priority) {
    return ov07_0221C5C8(callback, param, priority);
}

BOOL ov07_0221C634(BattleAnimSoundContext *ctx) {
    if (ctx->tickCount++ < ctx->applyInterval) {
        return 1;
    }

    BOOL isActive = 1;

    ctx->tickCount = 0;
    ctx->repeatCount--;

    PlaySE(ctx->effectID);
    sub_020061B4(ctx->effectID, 0xFFFF, ctx->curPan);

    if (ctx->repeatCount == 0) {
        isActive = 0;
    }

    return isActive;
}

BOOL ov07_0221C674(BattleAnimSoundContext *ctx) {
    BOOL isActive = 1;

    if ((ctx->applyInterval--) == 0) {
        PlaySE(ctx->effectID);
        sub_020061B4(ctx->effectID, 0xFFFF, ctx->curPan);

        isActive = 0;
    }

    return isActive;
}

void ov07_0221C69C(void) {
    G2_SetBlendAlpha(GX_BLEND_PLANEMASK_NONE, (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3) | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD, 8, 8);
}

void ov07_0221C6B4(BattleAnimSystem *system) {
    system->scriptDelay = 1;

    if (gSystem.heldKeys & 0x0200) {
        if (gSystem.heldKeys & 0x0100) {
            if (gSystem.newKeys & 0x0400) {
                BattleAnimScript_Next(system);
                system->scriptDelay = 0;
            }
        }
    }
}

void ov07_0221C6EC(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);

    system->scriptDelay = (u8)BattleAnimScript_ReadWord(system->scriptPtr);
    BattleAnimScript_Next(system);

    system->executeAnimScriptFunc = ov07_0221BE20;
}

void ov07_0221C70C(BattleAnimSystem *system) {
    if (system->activeAnimTasks == 0) {
        BattleAnimScript_Next(system);
        system->scriptDelay = 0;
    } else {
        system->scriptDelay = 1;
    }
}

void ov07_0221C72C(BattleAnimSystem *system) {
    BattleAnimScript_Next(system);
    u32 id = (u32)BattleAnimScript_ReadWord(system->scriptPtr);

    BattleAnimScript_Next(system);
    u32 value = (u32)BattleAnimScript_ReadWord(system->scriptPtr);

    BattleAnimScript_Next(system);

    if (id < 10) {
        system->scriptVars[id] = value;
    }
}
