#include "global.h"

#include "gf_gfx_planes.h"

#include "overlay_87_scratch_off_card_app.h"
#include "save.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pm_string.h"
#include "options.h"
#include "palette.h"
#include "sys_task.h"
#include "overlay_manager.h"
#include "message_printer.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

struct CoordinatesS16;

struct PaletteAnimator;

struct PaletteData;

struct ScratchOffCardApp;

struct ScratchOffCardsAppSprite;

struct SysTask;

void ScratchOffCardsAppSprite_SetDrawFlag(ScratchOffCardsAppSprite *appSprite, int draw);

void ov87_021E7FEC(ScratchOffCardsAppSprite *appSprite, int x, int y);

void ScratchOffCardsAppSprite_SetAnim(ScratchOffCardsAppSprite *appSprite, u32 animID);

void ScratchOffCardsAppSprite_UpdatePalette(ScratchOffCardsAppSprite *appSprite, u32 palette);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

BOOL PlaySE(u16 seqID);

typedef struct PaletteAnimator PaletteAnimator;

typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);

struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};

void PlaceValuesInScratchCells(ScratchOffCardApp *app, u8 unused);

void ov87_021E7334(ScratchOffCardApp *app);

void ov87_021E734C(ScratchOffCardApp *app, u8 unused);

void UpdateBackgroundPalette(ScratchOffCardApp *app, u32 bgLayer, u8 palette, u8 width, u8 height);

void ov87_021E74B8(ScratchOffCardApp *app);

void ov87_021E74D4(ScratchOffCardApp *app);

void ov87_021E7A2C(ScratchOffCardApp *app);

extern const CoordinatesS16 ov87_021E82E4[9];

BOOL ov87_021E5E00(ScratchOffCardApp *app);

void ov87_021E7324(ScratchOffCardApp *app, u8 unused);

void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, int draw);

void ov87_021E8078(ScratchOffCardsAppSprite *appSprite, u32 palette);

void ov87_021E803C(ScratchOffCardsAppSprite *appSprite, u32 animID);

void ov87_021E7460(ScratchOffCardApp *app, u32 bgLayer, u8 palette, u8 width, u8 height);

BOOL ov87_021E5E00(ScratchOffCardApp *app) {
    switch (app->subState) {
    case 0:
        ov87_021E7A2C(app);

        BG_LoadCharTilesData(app->bgConfig, GF_BG_LYR_MAIN_0, app->scratchCellsDataBackup, app->scratchCellsCharData->szByte, 0);
        BgCommitTilemapBufferToVram(app->bgConfig, GF_BG_LYR_MAIN_0);

        app->xOffset2 = 256;

        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, app->xOffset2);
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, app->xOffset2);
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, app->xOffset2);

        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, 1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG1, 1);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, 1);

        app->xOffset = -256;
        app->dummy3[app->currentCard] = 0;

        for (int i = 0; i < 240 * 160; i++) {
            app->scratchedPixels[i] = 0;
        }

        app->numRevealedCells = 0;

        ov87_021E7334(app);
        ov87_021E7324(app, app->dummy3[app->currentCard]);
        ov87_021E734C(app, app->dummy3[app->currentCard]);
        ov87_021E74B8(app);
        ov87_021E74D4(app);

        app->subState = 1;
        break;
    case 1:
        for (int i = 0; i < 4; i++) {
            ov87_021E7FEC(app->topScreenMonSprites[i], 60 + app->xOffset, 26 + (i * 42));
            ov87_021E7FD4(app->topScreenMonSprites[i], 1);
        }

        for (int i = 0; i < 3; i++) {
            ov87_021E8078(app->matchingCellBoxSprites[i], 1);
        }

        app->highlightPairPalette = 0;
        app->pairFound = 0;
        app->scratchedOffCellIdxs[0] = 177;
        app->scratchedOffCellIdxs[1] = 178;
        app->scratchedOffCellIdxs[2] = 179;

        for (int i = 0; i < 9; i++) {
            ov87_021E7FEC(app->scratchMonSprites[i], ov87_021E82E4[i].x + app->xOffset, ov87_021E82E4[i].y);
            ov87_021E7FD4(app->scratchMonSprites[i], 1);
            ov87_021E803C(app->scratchMonSprites[i], app->scratchCellValues[i]);
        }

        ov87_021E7460(app, GF_BG_LYR_MAIN_1, app->selectedCardIdxs[app->currentCard] + 4, 32, 24);
        ov87_021E7460(app, GF_BG_LYR_SUB_2, app->selectedCardIdxs[app->currentCard] + 1, 32, 24);
        PlaySE(1376);
        app->subState = 2;
        break;
    case 2:
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, app->xOffset2);
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, app->xOffset2);
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, app->xOffset2);

        app->xOffset2 += -16;

        if (app->xOffset >= 0) {
            app->subState = 3;
        } else {
            for (int i = 0; i < 4; i++) {
                ov87_021E7FEC(app->topScreenMonSprites[i], 60 + app->xOffset, 26 + (i * 42));
            }

            for (int i = 0; i < 9; i++) {
                ov87_021E7FEC(app->scratchMonSprites[i], ov87_021E82E4[i].x + app->xOffset, ov87_021E82E4[i].y);
            }
        }

        app->xOffset += 16;
        break;
    case 3:
        app->xOffset = 0;
        app->xOffset2 = 0;
        return 1;
    }

    return 0;
}
