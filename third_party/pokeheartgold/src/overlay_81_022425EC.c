#include "global.h"

#include "bg_window.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

void ov81_02242FBC(BattleFactoryAppPokeballSprite *ballSprite, GXOamMode mode);

u32 ov81_022425EC(BattleFactoryApp *app) {
    if (app->blink.timer == 0) {
        if (app->blink.palSwap == 0) {
            BgTilemapRectChangePalette(app->bgConfig, 5, app->blink.x, app->blink.y, app->blink.width, app->blink.height, 0xa);
        } else {
            BgTilemapRectChangePalette(app->bgConfig, 5, app->blink.x, app->blink.y, app->blink.width, app->blink.height, 9);
        }
        ScheduleBgTilemapBufferTransfer(app->bgConfig, 5);
        app->blink.palSwap = app->blink.palSwap ^ 1;
    }
    app->blink.timer = app->blink.timer + 1;
    if (app->blink.timer == 2) {
        app->blink.count = app->blink.count + 1;
        if (app->blink.count == 4) {
            app->subState = app->blink.nextSubState;
            return 0;
        }
        app->blink.timer = 0;
    }
    return 1;
}

void ov81_02242694(BattleFactoryApp *app, s32 arg1) {
    u32 i;
    u32 j;

    if (arg1 == 1) {
        G2S_SetBlendAlpha(0, 0xF, 6, 0xA);
        for (i = 0; i < app->numPokeballs; i++) {
            if (app->ballSprites[i] != NULL) {
                ov81_02242FBC(app->ballSprites[i], GX_OAM_MODE_XLU);
            }
        }
    } else {
        reg_G2_BLDCNT = 0;
        for (j = 0; j < app->numPokeballs; j++) {
            if (app->ballSprites[j] != NULL) {
                ov81_02242FBC(app->ballSprites[j], GX_OAM_MODE_NORMAL);
            }
        }
    }
}
