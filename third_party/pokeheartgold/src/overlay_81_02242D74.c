#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

void *ov81_02242D74(BattleFactoryAppPanelSprite *panelSprite);
void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);
void ov81_02242D94(BattleFactoryAppPanelSprite *panelSprite, int x, int y);

void *ov81_02242D74(BattleFactoryAppPanelSprite *panelSprite) {
    Sprite_Delete(panelSprite->sprite);
    Heap_Free(panelSprite);

    return NULL;
}

void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw) {
    Sprite_SetDrawFlag(panelSprite->sprite, draw);
}

void ov81_02242D94(BattleFactoryAppPanelSprite *panelSprite, int x, int y) {
    VecFx32 position;

    position.x = x * FX32_ONE;
    position.y = y * FX32_ONE;

    Sprite_SetMatrix(panelSprite->sprite, &position);
}
