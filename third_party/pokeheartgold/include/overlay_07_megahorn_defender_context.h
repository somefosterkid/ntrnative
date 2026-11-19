#ifndef POKEHEARTGOLD_OVERLAY_07_MEGAHORN_DEFENDER_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_MEGAHORN_DEFENDER_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_system.h"
#include "pokepic.h"

// the Megahorn defender task's work (0x60 on the DS; the init ov07_02230D1C fills it, the task ov07_02230BF8 runs it)
typedef struct MegahornDefenderContext {
    BattleAnimSystem *battleAnimSys;
    int state;
    int dirX;
    int dirY;
    Pokepic *sprite;
    s16 x;
    s16 y;
    XYTransformContext pos;
    XYTransformContext shake;
} MegahornDefenderContext;

#endif // POKEHEARTGOLD_OVERLAY_07_MEGAHORN_DEFENDER_CONTEXT_H
