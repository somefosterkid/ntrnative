#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_manager.h"
#include "palette.h"
#include "screen_fade.h"
#include "unk_02005D10.h"
#include "unk_0201956C.h"
#include "unk_02019BA4.h"
#include "unk_02088288.h"

void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);

void ov81_02242E08(BattleFactoryAppPanelSprite *panelSprite, int palIndex);


u32 ov81_0223F38C(BattleFactoryApp *app);

u32 ov81_0223F38C(BattleFactoryApp *app) {
    u16 uVar1;
    u32 uVar2;
    s32 iVar3;
    OverlayManager *pOVar4;
    BOOL BVar5;

    switch (app->subState) {
    case 0:
        ov81_02240F38(app, 0);
        app->subState = 1;
        app->summaryScreenOpened = FALSE;
        break;
    case 1:
        uVar2 = GridInputHandler_HandleInput_NoHold(app->inputHandler);
        switch (uVar2) {
        case 0:
        case 1:
        case 2:
            PlaySE(0x5dc);
            app->unk_468 = uVar2;
            ov81_02242218(app, app->personalParty, app->unk_468, 4);
            ov81_02241D38(app);
            sub_020196E8(app->unk_474, 0, 7, 0);
            ov81_0224174C(app);
            ov81_02242E08(app->unk_388, 1);
            break;
        case 4:
            PlaySE(0x5dc);
            ov81_022425C4(app, 2, 12, 7);
            app->subState = 2;
            break;
        case 5:
            PlaySE(0x5dc);
            ov81_022425C4(app, 18, 12, 8);
            app->subState = 2;
            break;
        case 6:
            PlaySE(0x5dc);
            ov81_022425C4(app, 10, 0xf, 9);
            app->subState = 2;
            break;
        case 3:
            PlaySE(0x5dc);
            ov81_02242D88(app->monIconSprites[0], 0);
            ov81_02242D88(app->monIconSprites[1], 0);
            ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
            sub_0201980C(app->unk_474, 0);
            ov81_02241D38(app);
            ov81_022425D8(app, 0x17, 0xf, 6);
            app->subState = 2;
            break;
        case 0xfffffffd:
            PlaySE(0x5dc);
            iVar3 = GridInputHandler_GetNextInput(app->inputHandler);
            if (iVar3 == 3) {
                ov81_02242D88(app->monIconSprites[0], 0);
                ov81_02242D88(app->monIconSprites[1], 0);
                ClearWindowTilemapAndScheduleTransfer(&app->windows[8]);
                sub_0201980C(app->unk_474, 0);
            } else {
                ov81_02242218(app, app->personalParty, iVar3, 3);
                ScheduleWindowCopyToVram(&app->windows[8]);
                sub_020196E8(app->unk_474, 0, 7, 0);
            }
            ov81_02241D38(app);
            break;
        case 0xfffffffe:
            uVar2 = GridInputHandler_GetNextInput(app->inputHandler);
            if (uVar2 >= 4) {
                PlaySE(0x5dc);
                ov81_022417B4(app);
                ov81_02242E08(app->unk_388, 0);
            } else {
                PlaySE(0x5dc);
                ov81_02241840(app);
                ov81_02242E08(app->unk_388, 1);
                ov81_02240F38(app, 1);
                return 1;
            }
        }
        break;
    case 2:
        ov81_022425EC(app);
        break;
    case 3:
        BVar5 = IsPaletteFadeFinished();
        if (BVar5 == 1) {
            uVar1 = PaletteData_GetSelectedBuffersBitmask(app->plttData);
            if (uVar1 != 0) {
                PaletteData_ScheduleFadeTaskEndIfNoSelectedBuffers(app->plttData);
                app->unk_478 = 0xff;
            } else {
                ov81_02240E78(app);
                ov81_02240BB0(app);
                pOVar4 = OverlayManager_New(&gOverlayTemplate_PokemonSummary,
                    app->monSummary,
                    HEAP_ID_100);
                app->monSummaryAppMan = pOVar4;
                app->summaryScreenOpened = TRUE;
                return 1;
            }
        }
        break;
    case 4:
        if (app->monSummaryAppMan == NULL) {
            return 1;
        }
        break;
    case 5:
        BVar5 = IsPaletteFadeFinished();
        if (BVar5 == 1) {
            app->subState = 1;
        }
        break;
    case 6:
        ov81_02241840(app);
        ov81_02240F38(app, 1);
        return 1;
    case 7:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_100);
        app->summaryScreenOpen = TRUE;
        app->subState = 3;
        break;
    case 8:
        ov81_0223F684(app);
        ov81_022417B4(app);
        ov81_02241840(app);
        return 1;
    case 9:
        ov81_022417B4(app);
        ov81_02242E08(app->unk_388, 0);
        app->subState = 1;
    }
    return 0;
}
