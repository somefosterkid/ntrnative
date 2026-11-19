#include "global.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "overlay_81.h"
#include "unk_02035900.h"
#include "overlay_81_battle_factory_app.h"
#include "overlay_80_02236B78.h"


void ov81_0224113C(void) {
    GF_AssertFail();
}

void ov81_0224093C(BattleFactoryApp *app, Window *window, u32 xOffset, u32 yOffset, u8 font);
void ov81_02240AD8(BattleFactoryApp *app, Window *window, u32 xOffset, u32 yOffset, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, u16 species, u8 gender);

void ov81_02241144(BattleFactoryApp *app) {
    s32 i;
    s32 numPartners;

    numPartners = ov80_02236DD4(app->challengeType);
    if (app->clearPartnerInfo == 1) {
        ClearWindowTilemapAndScheduleTransfer(&app->windows[1]);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[5]);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[6]);
        GfGfx_EngineATogglePlanes(4U, 0U);
        return;
    }
    if (app->unk_463 == 1) {
        for (i = 0; i < numPartners; i++) {
            FillWindowPixelBuffer(&app->windows[i + 5], 0U);
            if (i < app->partnersNumSelectedMons) {
                ov81_02240AD8(app, &app->windows[i + 5], 0, 0, 0xF, 2, 0, 0, app->partnerMonSpecies[i], (u8)app->partnenMonGenders[i]);
            }
            ScheduleWindowCopyToVram(&app->windows[i + 5]);
        }
        ov81_0224093C(app, &app->windows[1], 0, 0, 0);
        GfGfx_EngineATogglePlanes(4U, 1U);
    }
    app->unk_463 = 0U;
}

void ov81_02241218(BattleFactoryApp *app, u16 cmd, u16 tradedMon) {
    app->commPayload[0] = cmd;
    app->commPayload[1] = tradedMon;
    app->commPayload[2] = app->selectedIndices[0];
    app->commPayload[3] = app->selectedIndices[1];
}

void ov81_02241238(s32 netID, s32 unused, void *data, void *context) {
    BattleFactoryApp *app = context;
    u16 *payload = data;

    app->numTradeResultMsgReceived++;
    if (netID != sub_0203769C()) {
        app->partnerTradedMon = (u8)payload[1];
        if (sub_0203769C() == 0) {
            if (app->tradedMon == 1) {
                app->partnerTradedMon = 0U;
                return;
            }
            if (app->partnerTradedMon == 1) {
                app->selectedIndices[0] = (u8)payload[2];
                app->selectedIndices[1] = (u8)payload[3];
            }
        } else if (app->partnerTradedMon == 1) {
            app->tradedMon = FALSE;
            app->selectedIndices[0] = (u8)payload[2];
            app->selectedIndices[1] = (u8)payload[3];
        }
    }
}
