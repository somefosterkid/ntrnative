#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

void ov81_02242DE4(BattleFactoryAppPanelSprite *panelSprite, s32 seq);

void ov81_02242DE4(BattleFactoryAppPanelSprite *panelSprite, s32 seq) {
    Sprite_SetAnimationFrame(panelSprite->sprite, 0);
    Sprite_SetAnimCtrlSeq(panelSprite->sprite, seq);
}
