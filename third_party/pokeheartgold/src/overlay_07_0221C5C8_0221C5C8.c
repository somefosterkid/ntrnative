#include "global.h"

#include "unk_02005D10.h"
#include "overlay_07_battle_anim_system.h"

struct BattleAnimSystem;

void sub_020061EC(int pan);

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

BOOL ov07_0221C5C8(BattleAnimSoundContext *ctx);

BOOL ov07_0221C5C8(BattleAnimSoundContext *ctx) {
    if (ctx->tickCount++ < ctx->applyInterval) {
        return TRUE;
    }

    BOOL isActive = TRUE;

    ctx->tickCount = 0;
    ctx->curPan += ctx->panStep;

    if (ctx->panStep == 0) {
        isActive = FALSE;
    } else if (ctx->startPan < ctx->endPan) {
        if (ctx->curPan >= ctx->endPan) {
            isActive = FALSE;
        }
    } else {
        if (ctx->curPan <= ctx->endPan) {
            isActive = FALSE;
        }
    }

    sub_020061EC(ctx->curPan);

    if (!IsSEPlaying(ctx->effectID)) {
        return FALSE;
    }

    return isActive;
}
