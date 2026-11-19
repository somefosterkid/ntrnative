#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

void ov81_02242DFC(BattleFactoryAppPanelSprite *panelSprite, int palIndex);
void ov81_02242E08(BattleFactoryAppPanelSprite *panelSprite, int palIndex);

void ov81_02242DFC(BattleFactoryAppPanelSprite *panelSprite, int palIndex) {
    Sprite_SetPaletteOverride(panelSprite->sprite, palIndex);
}

void ov81_02242E08(BattleFactoryAppPanelSprite *panelSprite, int palIndex) {
    Sprite_SetPalIndexRespectVramOffset(panelSprite->sprite, palIndex);
}
