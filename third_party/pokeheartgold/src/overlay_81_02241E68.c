#include "global.h"

#include "party.h"
#include "pokepic.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_02240F48(void *, s32, Pokemon *, s32, s32, s32);

BattleFactoryAppPanelSprite *ov81_02242CBC(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);
void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);
void ov81_02241F50(void *arg0);
void *ov81_02242D74(BattleFactoryAppPanelSprite *panelSprite);

void ov81_02241E68(BattleFactoryApp *app, u32 arg1, s32 slot, s32 arg3) {
    Pokepic_Delete(((Pokepic *)app->monSprites[arg1]));
    if (app->numMonsToFade == 3) {
        ov81_02240F48(app, (u8)arg1, Party_GetMonByIndex(app->personalParty, slot), (0x50 * arg1) + 0x30, 0x90, arg3);
        return;
    }
    ov81_02240F48(app, (u8)arg1, Party_GetMonByIndex(app->personalParty, slot), (0x70 * arg1) + 0x48, 0x90, arg3);
}

void ov81_02241EDC(BattleFactoryApp *app, u32 arg1, s32 slot, s32 arg3) {
    Pokepic_Delete(((Pokepic *)app->monSprites[arg1]));
    if (*((s32 *)&app->numMonsToFade) == 3) {
        ov81_02240F48(app, (u8)arg1, Party_GetMonByIndex(app->receivableParty, slot), (0x50 * arg1) + 0x30, 0x90, arg3);
        return;
    }
    ov81_02240F48(app, (u8)arg1, Party_GetMonByIndex(app->receivableParty, slot), (0x70 * arg1) + 0x48, 0x90, arg3);
}

void ov81_02241F50(void *arg0) {
    s32 v0;
    s32 v1;
    s32 v5;
    u32 v2;
    u32 v3;
    struct BattleFactoryAppPanelSprite **v4;
    unsigned int new_var;
    v2 = ((BattleFactoryApp *)arg0)->numMonsToFade;
    new_var = 0U;
    if (v2 == 3) {
        v0 = 0x30;
        v1 = 0x50;
    } else {
        v0 = 0x48;
        v1 = 0x70;
    }
    v3 = 0;
    if (v2 > new_var) {
        v5 = 0;
        v4 = (struct BattleFactoryAppPanelSprite **)arg0;
        do {
            *(void **)((u8 *)v4 + offsetof(BattleFactoryApp, panelSprites)) = ov81_02242CBC(&((BattleFactoryApp *)arg0)->spriteMan, 1, 3, 1, 0, v0 + v5, 0x90);
            ov81_02242D88(*(void **)((u8 *)v4 + offsetof(BattleFactoryApp, panelSprites)), 0);
            v3 += 1;
            v5 += v1;
            v4 += 1;
        } while (v3 < ((u32)(((BattleFactoryApp *)arg0)->numMonsToFade)));
    }
}

void ov81_02241FC0(BattleFactoryApp *app) {
    u32 v0;
    unsigned int new_var;
    struct BattleFactoryAppPanelSprite **v1;
    new_var = 0U;
    v0 = 0;
    if (((u32)(app->numMonsToFade)) > new_var) {
        v1 = (struct BattleFactoryAppPanelSprite **)app;
        do {
            ov81_02242D74(*(void **)((u8 *)v1 + offsetof(BattleFactoryApp, panelSprites)));
            v0 += 1;
            v1 += 1;
        } while (v0 < ((u32)(app->numMonsToFade)));
    }
}
