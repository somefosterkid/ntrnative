#ifndef POKEHEARTGOLD_OVERLAY_07_DRILL_PECK_CONTEXT_H
#define POKEHEARTGOLD_OVERLAY_07_DRILL_PECK_CONTEXT_H

#include "overlay_07_battle_anim_common.h"
#include "overlay_07_value_lerp_context.h"

struct BattleAnimSystem;
struct Pokepic;

enum DrillPeckState {
    DRILL_PECK_STATE_MOVE_BWD = 0,
    DRILL_PECK_STATE_ROTATE_FWD,
    DRILL_PECK_STATE_DELAY_1,
    DRILL_PECK_STATE_MOVE_FWD,
    DRILL_PECK_STATE_DELAY_2,
    DRILL_PECK_STATE_ROTATE_BWD,
    DRILL_PECK_STATE_CLEANUP,
};

// sizeof == 0x4C on the DS (the DS build allocates sizeof(QuickAttackContext) == 0xF0 for it)
typedef struct DrillPeckContext {
    struct BattleAnimSystem *battleAnimSys;
    int state;
    s16 delay;
    struct Pokepic *attackerSprite;
    XYTransformContext pos;
    ValueLerpContext angle;
    s16 attackerX;
    s16 attackerY;
} DrillPeckContext;

#endif // POKEHEARTGOLD_OVERLAY_07_DRILL_PECK_CONTEXT_H
