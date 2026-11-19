#include "global.h"
#include "overlay_81.h"
#include "overlay_81_battle_factory_app.h"

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "pokepic.h"
#include "render_window.h"
#include "unk_02005D10.h"
#include "unk_0201956C.h"

void ov80_02236DD4(u8, u32);

s32 ov80_02237254(u8);

void *ov81_02242D74(BattleFactoryAppPanelSprite *panelSprite);

void ov81_02242D88(BattleFactoryAppPanelSprite *panelSprite, BOOL draw);

BOOL ov81_02242DD8(BattleFactoryAppPanelSprite *panelSprite);

void ov81_02242DE4(BattleFactoryAppPanelSprite *panelSprite, s32 seq);

void *ov81_02242EA4(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_02242F60(BattleFactoryAppPokeballSprite *ballSprite);

void ov81_02242F94(BattleFactoryAppPokeballSprite *ballSprite, u32 animID);


u32 ov81_0223FC74(BattleFactoryApp *app, u32 param_2, u32 param_3, u32 param_4);

u32 ov81_0223FC74(BattleFactoryApp *app, u32 param_2, u32 param_3, u32 param_4) {
    s32 iVar2;
    u32 uVar3;
    s32 iVar4;
    ov80_02236DD4(app->challengeType, param_2);
    switch (app->subState) {
    case 0:
        app->clearPartnerInfo = TRUE;
        ClearWindowTilemapAndCopyToVram(&app->windows[1]);
        ClearWindowTilemapAndCopyToVram(&app->windows[5]);
        ClearWindowTilemapAndCopyToVram(&app->windows[6]);
        FillWindowPixelBuffer(&app->windows[0], 0);
        ScheduleWindowCopyToVram(&app->windows[0]);
        ClearWindowTilemapAndCopyToVram(&app->windows[2]);
        ClearWindowTilemapAndCopyToVram(&app->windows[3]);
        ClearWindowTilemapAndCopyToVram(&app->windows[4]);
        ClearWindowTilemapAndCopyToVram(&app->windows[8]);
        sub_0201980C(app->unk_474, 0);
        ClearFrameAndWindow2(&app->windows[7], 1);
        ClearWindowTilemapAndScheduleTransfer(&app->windows[7]);
        ov81_0224300C(app->windows);
        Pokepic_StartPaletteFadeAll(app->monSpriteMan, 0, 0x10, 0, 0xffff);
        GfGfx_EngineATogglePlanes(4, 0);
        app->animationTimer = 0;
        app->subState = app->subState + 1;
        break;

    case 1:
        app->animationTimer = app->animationTimer + 1;
        if (app->animationTimer >= 2) {
            for (uVar3 = 0; uVar3 < app->numMonsToFade; uVar3++) {
                Pokepic_Delete(app->monSprites[uVar3]);
            }
            ov81_02242D74(app->unk_388);
            ov81_02242D74(app->unk_38C);
            ov81_02242D74(app->unk_390);
            ov81_02242D74(app->monIconSprites[0]);
            ov81_02242D74(app->monIconSprites[1]);
            ov81_02241FC0(app);
            ov81_02242170(app);
            ov81_02242DE4(app->unk_380, 2);
            app->animationTimer = 0;
            app->subState = app->subState + 1;
        }
        break;

    case 2:
        Pokepic_ResumePaletteFade(app->monSprites[0]);
        iVar2 = ov81_02242DD8(app->unk_380);
        if (iVar2 != 1) {
            ov81_02242D88(app->unk_380, 0);
            ov81_02240728(app, GF_BG_LYR_SUB_2);
            ov81_022406E0(app, GF_BG_LYR_MAIN_2);
            BgSetPosTextAndCommit(app->bgConfig, 6, BG_POS_OP_SET_ROT, app->conveyorXOffset);
            PlaySE(0x611);
            app->animationTimer = 0;
            app->subState = app->subState + 1;
        }
        break;

    case 3:
        iVar2 = ov81_02240564(app);
        if (iVar2 == 1) {
            StopSE(0x611, 0);
            PlaySE(0x678);
            app->wheelPaletteCounter = 8;
            for (iVar4 = 0; iVar4 < app->numPokeballs; iVar4++) {
                app->ballSprites[iVar4] = ov81_02242EA4(app->ballSprites[iVar4]);
                app->ballSprites[iVar4] = NULL;
            }
            iVar2 = ov80_02237254(app->challengeType);
            if (iVar2 == 1) {
                if (app->numPokeballs == 2) {
                    app->numPokeballs = 4;
                } else {
                    app->numPokeballs = 2;
                }
            }
            ov81_0223EBE4(app);
            for (iVar4 = 0; iVar4 < app->numPokeballs; iVar4++) {
                ov81_02242F60(app->ballSprites[iVar4]);
            }
            PlaySE(0x611);
            app->animationTimer = 0;
            app->subState = app->subState + 1;
        }
        break;

    case 4:
        iVar2 = ov81_022404B4(app);
        if (iVar2 == 1) {
            StopSE(0x611, 0);
            PlaySE(0x678);
            for (iVar4 = 0; iVar4 < app->numPokeballs; iVar4++) {
                ov81_02242F94(app->ballSprites[iVar4], 4);
            }
            app->wheelPaletteCounter = 8;
            ov81_02242D88(app->unk_380, 1);
            ov81_02242DE4(app->unk_380, 1);
            app->animationTimer = 0;
            app->subState = app->subState + 1;
        }
        break;

    case 5:
        iVar2 = ov81_02242DD8(app->unk_380);
        if (iVar2 != 1) {
            app->animationTimer = 0;
            app->subState = app->subState + 1;
        }
        break;

    case 6:
        if (app->animationTimer == 0) {
            GfGfx_EngineATogglePlanes(2, 0);
            if (app->exchangeMonSelected == 0) {
                uVar3 = 0;
                if (uVar3 < app->numMonsToFade) {
                    do {
                        ov81_02241E68(app, uVar3, uVar3, 1);
                        uVar3 = uVar3 + 1;
                    } while (uVar3 < app->numMonsToFade);
                }
            } else {
                uVar3 = 0;
                if (uVar3 < app->numMonsToFade) {
                    do {
                        if (uVar3 == app->selectedIndices[0]) {
                            ov81_02241EDC(app, uVar3, 0, 0);
                        } else {
                            ov81_02241E68(app, uVar3, uVar3, 1);
                        }
                        uVar3 = uVar3 + 1;
                    } while (uVar3 < app->numMonsToFade);
                }
            }
            Pokepic_StartPaletteFadeAll(app->monSpriteMan, 0x10, 0, 1, 0xffff);
        }
        Pokepic_ResumePaletteFade(app->monSprites[0]);
        app->animationTimer = app->animationTimer + 1;
        if (app->animationTimer >= 0x15) {
            ov81_02241398(app);
            app->clearPartnerInfo = FALSE;
            iVar2 = ov80_02237254(app->challengeType);
            if (iVar2 == 1) {
                app->unk_463 = 1;
                ov81_02241144(app);
            }
            app->animationTimer = 0;
            return 1;
        }
    }

    return 0;
}
