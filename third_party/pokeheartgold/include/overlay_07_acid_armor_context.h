#ifndef POKEHEARTGOLD_OVERLAY_07_ACID_ARMOR_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_ACID_ARMOR_CONTEXT_H

#include "global.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_alpha_fade_context.h"
#include "overlay_07_bg_scroll_context.h"

struct BattleAnimSystem;
struct Pokepic;

typedef struct AcidArmorContext {
    struct BattleAnimSystem *battleAnimSys;
    int state;
    int delay;
    struct Pokepic *attackerSprite;
    s16 attackerX;
    s16 attackerY;
    s16 scrollStartY;
    s16 scrollEndY;
    BgScrollContext *bgScroll;
    XYTransformContext pos;
    AlphaFadeContext alphaFade;
} AcidArmorContext;

void ov07_0222C850(AcidArmorContext *ctx, void *buffer);

#endif // POKEHEARTGOLD_OVERLAY_07_ACID_ARMOR_CONTEXT_H
