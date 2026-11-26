#include "global.h"
struct BattleFactoryApp;

#include "bg_window.h"
#include "options.h"
#include "pokepic.h"
#include "unk_0201956C.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"
#include "map_object.h"
#include "overlay_80_02236B78.h"


s32 ov81_02240658(void *, s32);

s32 ov81_022408C4(void *, void *, s32, s32, u8);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov81_02240AD8(void *, void *, s32, s32, u8, u8, u8, u8, u16, u8);
#else
s32 ov81_02240AD8(void *, void *, s32, s32, s32, s32, s32, s32, s32, s32);
#endif

s32 ov81_02241524(void *);

s32 ov81_02241C84(s32, s32, s32 *, s32 *);

s32 ov81_02241CA0(BattleFactoryApp *, s32 *, s32 *);

s32 ov81_02241D0C(void *);

s32 ov81_02241F50(void *);

s32 ov81_02241FEC(void *);

s32 ov81_0224218C(void *);

BattleFactoryAppPanelSprite *ov81_02242CBC(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);

BattleFactoryAppPanelSprite *ov81_02242D18(BattleFactoryAppSpriteManager *spriteMan, u32 resourceID, u32 animID, int resourcePriority, u32 priority, int x, int y);

void ov81_02242D94(BattleFactoryAppPanelSprite *panelSprite, int x, int y);

void ov81_02242F48(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_02242F94(BattleFactoryAppPokeballSprite *ballSprite, u32 animID);

void ov81_02242FB0(BattleFactoryAppPokeballSprite *ballSprite, int palIndex);

s32 ov81_02243228(void *, s32, s32 *, s32 *);

BattleFactoryAppPokeballSprite *ov81_022412C4(BattleFactoryApp *, s32);
void ov81_02242EB8(BattleFactoryAppPokeballSprite *ballSprite, BOOL draw);

void ov81_02241DDC(BattleFactoryApp *app, int param1);
void ov81_0223E8B0(BattleFactoryApp *app);

void ov81_0223E5B4(BattleFactoryApp *app) {
    s32 v4;
    s32 v3;
    s32 v1;
    s32 v0;
    s32 v9;
    s32 v10;
    s32 v11;

    v0 = ov80_02236DD4(app->challengeType);
    ov81_02240F38(app, 0);
    ov81_02241524(app);
    ov81_02243228(app->inputHandler, app->unk_468, &v4, &v3);
    app->unk_388 = ov81_02242D18(&app->spriteMan, 0, 2, 0, 0, v4, v3);
    ov81_02243228(app->inputHandler, 6, &v4, &v3);
    app->unk_38C = ov81_02242D18(&app->spriteMan, 0, 3, 0, 0, v4, v3);
    ov81_02241CA0(app, &v4, &v3);
    app->unk_390 = ov81_02242CBC(&app->spriteMan, 1, 0, 0, 0, v4, v3);
    ov81_02241F50(app);
    ov81_02241FEC(app);
    ov81_022420B4(app);
    ov81_0224218C(app);
    app->monIconSprites[0] = ov81_02242D18(&app->spriteMan, 4, 0, 0, 0, 0x50, 0xC);
    app->monIconSprites[1] = ov81_02242D18(&app->spriteMan, 5, 0, 0, 0, 0x50, 0xC);
    ov81_02242218(app, app->personalParty, app->unk_468, 6);
    ov81_02242300(app, app->unk_468, 6);
    sub_020196E8(app->unk_474, 0, 7, 0);
    ov81_022408C4(app, &app->windows[0], 0, 0, 0);
    v10 = 0;
    if ((s32) app->numMonsSelected > 0) {
        do {
            ov81_02242F48(app->ballSprites[app->selectedIndices[v10]]);
            ov81_02242FB0(app->ballSprites[app->selectedIndices[v10]], 0);
            ov81_02242F94(app->ballSprites[app->selectedIndices[v10]], 1);
            v10 += 1;
        } while (v10 < (s32) app->numMonsSelected);
    }
    ov81_02243028(&app->windows[7], Options_GetFrame(app->options));
    ov81_022408A0(app, 0, app->numMonsSelected + 1);
    app->printerID = ov81_0224086C(app, 0);
    if (app->summaryScreenOpened == 1) {
        v1 = ov81_02241D0C(app);
        v11 = 0;
        if ((s32) app->numMonsSelected > 0) {
            do {
                Pokepic_SetAttr(app->monSprites[v11], 6, 0);
                v11 += 1;
            } while (v11 < (s32) app->numMonsSelected);
        }
        Pokepic_SetAttr(app->monSprites[v1], 6, 0);
        ov81_02241E68(app, v1, app->unk_468, 0);
        ov81_02241FEC(app);
        ov81_02241C84(v1, app->numMonsToFade, &v4, &v3);
        ov81_02242D94(app->unk_390, v4, v3);
        ov81_02240658(app, 0xFF);
        ov81_02241450(app);
        if (app->partnersNumSelectedMons != 0) {
            v9 = 0;
            if (v0 > 0) {
                do {
                    FillWindowPixelBuffer(&app->windows[v9 + 5], 0U);
                    if (v9 < (s32) app->partnersNumSelectedMons) {
                        ov81_02240AD8(app, &app->windows[v9 + 5], 0, 0, 0xF, 2, 0, 0, (s32)app->partnerMonSpecies[v9], (s32)(u8)app->partnenMonGenders[v9]);
                    }
                    ScheduleWindowCopyToVram(&app->windows[v9 + 5]);
                    v9 += 1;
                } while (v9 < v0);
            }
        }
    } else {
        ov81_022414E0(app);
    }
}

void ov81_0223E87C(BattleFactoryApp *app) {
    s32 v0;

    v0 = 0;
    if ((s32) app->numPokeballs > 0) {
        do {
            app->ballSprites[v0] = ov81_022412C4(app, v0);
            ov81_02242EB8(app->ballSprites[v0], 1);
            v0 += 1;
        } while (v0 < (s32) app->numPokeballs);
    }
}

void ov81_0223E8B0(BattleFactoryApp *app) {
    ov81_02241DDC(app, 0);
}
