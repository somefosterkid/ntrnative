#include "global.h"

#include "bg_window.h"
#include "overlay_81.h"
#include "pokepic.h"
#include "unk_02019BA4.h"
#include "overlay_81_battle_factory_app.h"

s32 ov81_02241FEC(void *);

s32 ov81_0224218C(void *);

void ov81_02242F54(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_02242F94(BattleFactoryAppPokeballSprite *ballSprite, u32 animID);

void ov81_02242FB0(BattleFactoryAppPokeballSprite *ballSprite, int palIndex);

void ov81_0223EF5C(BattleFactoryApp *app) {
    Pokepic_SetAttr(((Pokepic *)app->monSprites[(app->numMonsSelected)]), 6, 1);
    FillWindowPixelBuffer(&app->windows[app->numMonsSelected + 2], 0U);
    ScheduleWindowCopyToVram(&app->windows[app->numMonsSelected + 2]);
    app->numMonsSelected = (u8)(app->numMonsSelected - 1);
    ov81_022408A0(app, 0, app->numMonsSelected + 1);
    *((s8 *)&app->printerID) = ov81_0224086C(app, 0);
    ov81_02242F54(*((void **)&app->ballSprites[(*((u16 *)&app->selectedIndices[(app->numMonsSelected)]))]));
    ov81_02242FB0(*((void **)&app->ballSprites[(*((u16 *)&app->selectedIndices[(app->numMonsSelected)]))]), 1);
    ov81_02242F94(*((void **)&app->ballSprites[(*((u16 *)&app->selectedIndices[(app->numMonsSelected)]))]), 0);
    *((s16 *)&app->selectedIndices[(app->numMonsSelected)]) = 0;
    ov81_02241C0C(app);
    ov81_02241FEC(app);
    ov81_0224218C(app);
    ov81_02242300(app, GridInputHandler_GetNextInput(app->inputHandler), 6);
}

u8 ov81_02240F18(u8);

void ov81_02242F48(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_0223F038(BattleFactoryApp *app) {
    u8 v0;

    ov81_02242F48(*((void **)&app->ballSprites[(app->unk_468)]));
    ov81_02242FB0(*((void **)&app->ballSprites[(app->unk_468)]), 0);
    ov81_02242F94(*((void **)&app->ballSprites[(app->unk_468)]), 1);
    *((s16 *)&app->selectedIndices[(app->numMonsSelected)]) = (s16) app->unk_468;
    app->numMonsSelected = (u8)(app->numMonsSelected + 1);
    v0 = app->numMonsSelected;
    if (v0 != ov81_02240F18(app->challengeType)) {
        ov81_022408A0(app, 0, v0 + 1);
        *((s8 *)&app->printerID) = ov81_0224086C(app, 0);
    }
    ov81_02241FEC(app);
    ov81_0224218C(app);
}

void ov81_0223F0BC(BattleFactoryApp *app) {
    s32 v0;
    s32 v1;

    ov81_02242F54(*((void **)&app->ballSprites[(app->unk_468)]));
    ov81_02242FB0(*((void **)&app->ballSprites[(app->unk_468)]), 1);
    ov81_02242F94(*((void **)&app->ballSprites[(app->unk_468)]), 0);
    if (((u32) app->numMonsSelected >= 2U) && (*((u16 *)&app->selectedIndices[0]) == app->unk_468)) {
        *((u16 *)&app->selectedIndices[0]) = (u16) *((u16 *)&app->selectedIndices[1]);
        ov81_02241E68(app, 0, *((u16 *)&app->selectedIndices[0]), 0);
        Pokepic_SetAttr(((Pokepic *)app->monSprites[0]), 6, 0);
    }
    app->numMonsSelected = (u8)(app->numMonsSelected - 1);
    *((s16 *)&app->selectedIndices[(app->numMonsSelected)]) = 0;
    v0 = ov81_02240F18(app->challengeType);
    v1 = 0;
    if (v0 > 0) {
        do {
            FillWindowPixelBuffer(&app->windows[v1 + 2], 0U);
            v1 += 1;
        } while (v1 < v0);
    }
    ov81_022408A0(app, 0, app->numMonsSelected + 1);
    *((s8 *)&app->printerID) = ov81_0224086C(app, 0);
    ov81_02241C0C(app);
    ov81_02241FEC(app);
    ov81_0224218C(app);
    ov81_02242300(app, app->unk_468, 6);
}
