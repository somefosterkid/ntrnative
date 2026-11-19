#include "global.h"

#include "options.h"
#include "pokepic.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_02240658(void *, s32);

s32 ov81_022408C4(void *, void *, s32, s32, u8);

s32 ov81_0224185C(void *);

s32 ov81_02241C84(s32, u32, s32 *, s32 *);

s32 ov81_02241F50(void *);

s32 ov81_02242058(void *, s32);

BattleFactoryAppPanelSprite *ov81_02242CBC(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);

BattleFactoryAppPanelSprite *ov81_02242D18(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);

void ov81_02242D94(BattleFactoryAppPanelSprite *panelSprite, int x, int y);

void ov81_02242E08(BattleFactoryAppPanelSprite *panelSprite, int palIndex);

s32 ov81_02243228(void *, s32, s32 *, s32 *);

GridInputHandler *ov81_02243370(void *, GridInputHandler *);

void ov81_0223E8BC(BattleFactoryApp *app) {
    s32 v0;
    s32 v1;
    BattleFactoryApp *new_var2;
    BattleFactoryApp *new_var;
    u32 v3;
    unsigned int new_var3;
    ov81_02240F38(app, 0);
    app->inputHandler = ov81_02243370(app, app->inputHandler);
    ov81_0224185C(app);
    ov81_02243228(app->inputHandler, app->unk_468, &v0, &v1);
    app->unk_388 = ov81_02242D18(&app->spriteMan, 0, 2, 0, 0, v0, v1);
    app->unk_390 = ov81_02242CBC(&app->spriteMan, 1, 0, 0, 0, 0, 0);
    ov81_02241D38(app);
    ov81_02243228(app->inputHandler, 4, &v0, &v1);
    app->unk_38C = ov81_02242D18(&app->spriteMan, 0, 3, 0, 0, v0, v1);
    ov81_02241F50(app);
    ov81_02242058(app, 0);
    ov81_022420B4(app);
    app->monIconSprites[0] = ov81_02242D18(&app->spriteMan, 4, 0, 0, 0, 0x50, 0xC);
    app->monIconSprites[1] = ov81_02242D18(&app->spriteMan, 5, 0, 0, 0, 0x50, 0xC);
    ov81_02242218(app, app->personalParty, app->unk_468, 4);
    ov81_022423D0(app, app->personalParty);
    sub_020196E8(app->unk_474, 0, 7, 0);
    ov81_022408C4(app, &app->windows[0], 0, 0, 0);
    new_var = app;
    new_var2 = new_var;
    ov81_02243028(&app->windows[7], Options_GetFrame(new_var2->options));
    new_var2->printerID = ov81_0224086C(new_var2, 9);
    new_var3 = 0U;
    if (new_var2->summaryScreenOpened == 1) {
        v3 = 0;
        if (((u32)(app->numMonsToFade)) > new_var3) {
            do {
                Pokepic_SetAttr(new_var2->monSprites[v3], 6, 0);
                v3 += 1;
            } while (v3 < ((u32)(new_var2->numMonsToFade)));
        }
        ov81_02241C84(new_var2->unk_468, new_var2->numMonsToFade, &v0, &v1);
        ov81_02242D94(new_var2->unk_390, v0, v1);
        ov81_02240658(app, 0xFF);
        ov81_0224174C(new_var2);
        ov81_02242E08(app->unk_388, 1);
        return;
    }
    ov81_022417B4(app);
    ov81_02242E08(new_var2->unk_388, 0);
}
