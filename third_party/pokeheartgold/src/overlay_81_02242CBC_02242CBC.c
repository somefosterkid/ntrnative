#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

Sprite *ov81_02242A8C(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u32 priority, int resourcePriority, u8 onSubScreen);
void ov81_02242D94(BattleFactoryAppPanelSprite *panelSprite, int x, int y);
void ov81_02242DAC(BattleFactoryAppPanelSprite *panelSprite, int x, int y);
BattleFactoryAppPanelSprite *ov81_02242CBC(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);
BattleFactoryAppPanelSprite *ov81_02242D18(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);

BattleFactoryAppPanelSprite *ov81_02242CBC(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y) {
    BattleFactoryAppPanelSprite *panelSprite = Heap_Alloc(HEAP_ID_100, sizeof(BattleFactoryAppPanelSprite));
    memset(panelSprite, 0, sizeof(BattleFactoryAppPanelSprite));
    panelSprite->x = x;
    panelSprite->y = y;
    panelSprite->sprite = ov81_02242A8C(spriteMan, resourceID, animID, priority, resourcePriority, FALSE);
    ov81_02242D94(panelSprite, x, y);
    return panelSprite;
}

BattleFactoryAppPanelSprite *ov81_02242D18(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y) {
    BattleFactoryAppPanelSprite *panelSprite = Heap_Alloc(HEAP_ID_100, sizeof(BattleFactoryAppPanelSprite));
    memset(panelSprite, 0, sizeof(BattleFactoryAppPanelSprite));
    panelSprite->x = x;
    panelSprite->y = y;
    panelSprite->sprite = ov81_02242A8C(spriteMan, resourceID, animID, priority, resourcePriority, TRUE);
    ov81_02242DAC(panelSprite, x, y);
    return panelSprite;
}
