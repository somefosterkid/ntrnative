#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

#include "constants/heap.h"

#include "overlay_manager.h"
#include "palette.h"
#include "screen_fade.h"
#include "unk_02005D10.h"
#include "unk_02019BA4.h"
#include "unk_02088288.h"

s32 ov80_02237254(u8);

u8 ov81_02242F40(BattleFactoryAppPokeballSprite *ballSprite);


u32 ov81_0223ECE4(BattleFactoryApp *app);

u32 ov81_0223ECE4(BattleFactoryApp *app) {
    u16 uVar1;
    u32 uVar2;
    OverlayManager *pOVar3;
    BOOL BVar4;
    s32 iVar5;

    switch (app->subState) {
    case 0:
        app->subState = 1;
        app->summaryScreenOpened = FALSE;
        break;
    case 1:
        uVar2 = GridInputHandler_HandleInput_NoHold(app->inputHandler);
        switch (uVar2) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            PlaySE(0x5dc);
            app->unk_468 = uVar2;
            ov81_02241C0C(app);
            ov81_02242218(app, app->personalParty, app->unk_468, 6);
            ov81_02242300(app, app->unk_468, 6);
            ov81_02241450(app);
            break;
        case 6:
            PlaySE(0x5dc);
            ov81_022425C4(app, 2, 0xc, 6);
            app->subState = 2;
            break;
        case 7:
            PlaySE(0x5dc);
            ov81_022425C4(app, 0x12, 0xc, 7);
            app->subState = 2;
            break;
        case 8:
            PlaySE(0x5dc);
            ov81_022425C4(app, 10, 0xf, 8);
            app->subState = 2;
            break;
        case 0xfffffffd:
            PlaySE(0x5dc);
            ov81_02241C0C(app);
            ov81_02242218(app, app->personalParty, GridInputHandler_GetNextInput(app->inputHandler), 6);
            ov81_02242300(app, GridInputHandler_GetNextInput(app->inputHandler), 6);
            break;
        case 0xfffffffe:
            if ((u32)GridInputHandler_GetNextInput(app->inputHandler) >= 6) {
                PlaySE(0x5dc);
                ov81_022414E0(app);
                return 1;
            }
            if (app->numMonsSelected != 0) {
                PlaySE(0x5dc);
                ov81_0223EF5C(app);
                if (ov80_02237254(app->challengeType) == 1) {
                    ov81_02240FA4(app, 8, 0);
                }
                return 1;
            }
            break;
        }
        break;
    case 2:
        ov81_022425EC(app);
        break;
    case 3:
        BVar4 = IsPaletteFadeFinished();
        if (BVar4 == 1) {
            uVar1 = PaletteData_GetSelectedBuffersBitmask(app->plttData);
            if (uVar1 != 0) {
                PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(app->plttData);
                app->unk_478 = 0xff;
                break;
            }
            ov81_02240E78(app);
            ov81_02240BB0(app);
            pOVar3 = OverlayManager_New(&gOverlayTemplate_PokemonSummary,
                app->monSummary,
                HEAP_ID_100);
            app->monSummaryAppMan = pOVar3;
            app->summaryScreenOpened = TRUE;
            return 1;
        }
        break;
    case 4:
        if (app->monSummaryAppMan == NULL) {
            return 1;
        }
        break;
    case 5:
        BVar4 = IsPaletteFadeFinished();
        if (BVar4 == 1) {
            app->subState = 1;
        }
        break;
    case 6:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_100);
        app->summaryScreenOpen = TRUE;
        app->subState = 3;
        break;
    case 7:
        ov81_022414E0(app);
        iVar5 = ov81_02242F40(app->ballSprites[app->unk_468]);
        if (iVar5 == 0) {
            ov81_0223F038(app);
        } else {
            ov81_0223F0BC(app);
        }
        iVar5 = ov80_02237254(app->challengeType);
        if (iVar5 == 1) {
            ov81_02240FA4(app, 8, 0);
        }
        return 1;
    case 8:
        ov81_022414E0(app);
        return 1;
    }
    return 0;
}
