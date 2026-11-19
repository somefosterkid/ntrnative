#include "global.h"

#include "error_handling.h"
#include "obj_pltt_transfer.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_common.h"
#include "overlay_07_battle_anim_funcs.h"

struct BattleAnimSystem;

BOOL ov07_0221BFC0(BattleAnimSystem *system);

enum BattleAnimPositionType {
    BATTLE_ANIM_POSITION_MON_X = 0,
    BATTLE_ANIM_POSITION_MON_Y,
    BATTLE_ANIM_POSITION_PARTICLE_X,
    BATTLE_ANIM_POSITION_PARTICLE_Y,
};

u8 ov07_02231924(BattleAnimSystem *system, int battler);

extern const Point2D ov07_02236468[2][6];

s16 ov07_02221F80(BattleAnimSystem *system, int battler, enum BattleAnimPositionType posType);

u8 ov07_02221FF0(ManagedSprite *sprite);

s16 ov07_02221F80(BattleAnimSystem *system, int battler, enum BattleAnimPositionType posType) {
    int isDoubles;
    int battlerType = ov07_02231924(system, battler);

    if (ov07_0221FAB0(system) == TRUE) {
        isDoubles = TRUE;
    } else {
        isDoubles = FALSE;
    }

    if (ov07_0221BFC0(system) == TRUE) {
        battlerType += 2;
    }

    switch (posType) {
    case BATTLE_ANIM_POSITION_MON_X:
    case BATTLE_ANIM_POSITION_PARTICLE_X:
        return ov07_02236468[isDoubles][battlerType].x;
    case BATTLE_ANIM_POSITION_MON_Y:
    case BATTLE_ANIM_POSITION_PARTICLE_Y:
        return ov07_02236468[isDoubles][battlerType].y;
    }

    GF_ASSERT(FALSE);
    return 0;
}

u8 ov07_02221FF0(ManagedSprite *sprite) {
    return ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(sprite->sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
}
