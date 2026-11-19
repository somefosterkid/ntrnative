#include "global.h"
struct BattleFactoryApp;
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);

void ov81_02242DFC(BattleFactoryAppPanelSprite *panelSprite, int palIndex);

void ov81_02241FEC(struct BattleFactoryApp *arg0);

void ov81_02242058(struct BattleFactoryApp *arg0, s32 arg1);
BattleFactoryAppPanelSprite *ov81_02242CBC(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);
BattleFactoryAppPanelSprite *ov81_02242D18(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);
void *ov81_02242D74(BattleFactoryAppPanelSprite *panelSprite);

void ov81_02241FEC(BattleFactoryApp *arg0) {
    u32 v1;
    short new_var;
    u8 v0;
    struct BattleFactoryAppPanelSprite **v2;
    new_var = 0U;
    v1 = 0;
    if (((u32)(arg0->numMonsToFade)) > new_var) {
        v2 = (struct BattleFactoryAppPanelSprite **)arg0;
        do {
            v0 = arg0->numMonsSelected;
            if (v1 == v0) {
                ov81_02242DFC(*(BattleFactoryAppPanelSprite **)((u8 *)v2 + offsetof(BattleFactoryApp, panelSprites)), 3);
                ov81_02242D88(*(BattleFactoryAppPanelSprite **)((u8 *)v2 + offsetof(BattleFactoryApp, panelSprites)), 1);
            } else if (v1 < ((u32)v0)) {
                ov81_02242DFC(*(BattleFactoryAppPanelSprite **)((u8 *)v2 + offsetof(BattleFactoryApp, panelSprites)), 2);
                ov81_02242D88(*(BattleFactoryAppPanelSprite **)((u8 *)v2 + offsetof(BattleFactoryApp, panelSprites)), 1);
            } else {
                ov81_02242D88(*(BattleFactoryAppPanelSprite **)((u8 *)v2 + offsetof(BattleFactoryApp, panelSprites)), 0);
            }
            v1 += 1;
            v2 += 1;
        } while (v1 < ((u32)(arg0->numMonsToFade)));
    }
}

void ov81_02242058(BattleFactoryApp *arg0, s32 arg1) {
    u32 v0;
    unsigned int new_var;
    struct BattleFactoryAppPanelSprite **v1;
    v0 = 0;
    new_var = 0U;
    if (((u32)(arg0->numMonsToFade)) > new_var) {
        v1 = (struct BattleFactoryAppPanelSprite **)arg0;
        do {
            if ((v0 == (arg0->unk_468)) && (arg1 == 1)) {
                ov81_02242DFC(*(BattleFactoryAppPanelSprite **)((u8 *)v1 + offsetof(BattleFactoryApp, panelSprites)), 3);
            } else {
                ov81_02242DFC(*(BattleFactoryAppPanelSprite **)((u8 *)v1 + offsetof(BattleFactoryApp, panelSprites)), 2);
            }
            ov81_02242D88(*(BattleFactoryAppPanelSprite **)((u8 *)v1 + offsetof(BattleFactoryApp, panelSprites)), 1);
            v0 += 1;
            v1 += 1;
        } while (v0 < ((u32)(arg0->numMonsToFade)));
    }
}

void ov81_022420B4(BattleFactoryApp *app) {
    s32 v0;
    s32 v5;
    u16 v2;
    u16 v3;
#if !(defined(SDK_PORT) || defined(NONMATCHING))
    u8 *v1;
#endif
    u16 v4;

    if (app->numMonsToFade == 3) {
        v5 = 0x10;
        v0 = 0x50;
    } else {
        v5 = 0x28;
        v0 = 0x70;
    }
    v3 = 0;
    v2 = 0;
    do {
        app->unk_3A8[v2] = ov81_02242CBC(&app->spriteMan, 2, v3, 0, 0, v5 + (v0 * v2), 0xB0);
        v3 += 1;
        v2 += 1;
    } while ((u32)v2 < 3U);
    if (app->numMonsToFade == 2) {
        ov81_02242D88(app->unk_3A8[2], 0);
    }
    v4 = 0;
    if ((u32)v2 < 6U) {
        do {
#if defined(SDK_PORT) || defined(NONMATCHING)
            app->unk_3A8[v2] = ov81_02242D18(&app->spriteMan, 3, v4, 0, 0, 0, 0);
            ov81_02242D88(app->unk_3A8[v2], 0);
#else
            v1 = (u8 *)app + (v2 * 4);
            *(BattleFactoryAppPanelSprite **)(v1 + offsetof(BattleFactoryApp, unk_3A8)) = ov81_02242D18(&app->spriteMan, 3, v4, 0, 0, 0, 0);
            ov81_02242D88(*(BattleFactoryAppPanelSprite **)(v1 + offsetof(BattleFactoryApp, unk_3A8)), 0);
#endif
            v4 += 1;
            v2 += 1;
        } while ((u32)v2 < 6U);
    }
}

void ov81_02242170(BattleFactoryApp *app) {
    u32 v0;

    v0 = 0;
    do {
        ov81_02242D74(app->unk_3A8[v0]);
        v0 += 1;
    } while (v0 < 6U);
}
