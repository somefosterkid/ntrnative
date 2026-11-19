#ifndef POKEHEARTGOLD_OVERLAY_07_SHADOW_PUNCH_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_SHADOW_PUNCH_CONTEXT_H

#include "overlay_07_battle_anim_common.h"

struct ManagedSprite;

// sizeof == 0xA8 on the DS
typedef struct ShadowPunchContext {
    BattleAnimScriptFuncCommon common;

    BattleAnimSpriteInfo spriteInfo;
    struct ManagedSprite *sprite;
    XYTransformContext mainPos;
    XYTransformContext afterimage1Pos;
    XYTransformContext afterimage2Pos;
    u8 iteration;
    u8 unused;
    u8 spriteAlpha;
    u8 bgAlpha;
    s16 attackerHeight;
} ShadowPunchContext;

#endif // POKEHEARTGOLD_OVERLAY_07_SHADOW_PUNCH_CONTEXT_H
