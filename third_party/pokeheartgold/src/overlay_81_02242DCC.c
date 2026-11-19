#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

void ov81_02242DCC(BattleFactoryAppPanelSprite *panelSprite, VecFx32 *position);
BOOL ov81_02242DD8(BattleFactoryAppPanelSprite *panelSprite);

void ov81_02242DCC(BattleFactoryAppPanelSprite *panelSprite, VecFx32 *position) {
    Sprite_SetMatrix(panelSprite->sprite, position);
}

BOOL ov81_02242DD8(BattleFactoryAppPanelSprite *panelSprite) {
    return Sprite_IsAnimated(panelSprite->sprite);
}
