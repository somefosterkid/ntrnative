#include "global.h"

#include "overlay_81_battle_factory_app.h"
#include "sprite.h"

void ov81_02242E14(BattleFactoryAppPanelSprite *panelSprite, void *pSrc, u32 szByte, enum NNS_G2D_VRAM_TYPE type);

void ov81_02242E14(BattleFactoryAppPanelSprite *panelSprite, void *pSrc, u32 szByte, enum NNS_G2D_VRAM_TYPE type) {
    u32 offset;

    offset = NNS_G2dGetImageLocation(Sprite_GetImageProxy(panelSprite->sprite), type);
    DC_FlushRange(pSrc, szByte);
    if (type == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GX_LoadOBJ(pSrc, offset, szByte);
        return;
    }
    GXS_LoadOBJ(pSrc, offset, szByte);
}
