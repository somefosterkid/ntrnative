#include "global.h"

#include "gf_gfx_planes.h"
#include "overlay_87_scratch_off_card_app.h"
#include "overlay_manager.h"
#include "save.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pm_string.h"
#include "message_printer.h"
#include "options.h"
#include "msgdata.h"
#include "palette.h"
#include "yes_no_prompt.h"

struct CoordinatesS16;

struct PaletteAnimator;

struct PaletteData;

struct ScratchOffCardApp;

struct ScratchOffCardsAppSprite;

struct SysTask;

void ScratchOffCardsAppSprite_SetDrawFlag(ScratchOffCardsAppSprite *appSprite, int draw);

void ov87_021E7FEC(ScratchOffCardsAppSprite *appSprite, int x, int y);

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

extern const CoordinatesS16 ov87_021E82E4[9];

BOOL ov87_021E64F8(ScratchOffCardApp *app);

void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, int draw);

BOOL ov87_021E64F8(ScratchOffCardApp *app) {
    switch (app->subState) {
    case 0:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 0);
        ov87_021E7FD4(app->skipCardButtonSprite, 0);

        app->xOffset = 0;
        app->xOffset2 = 0;

        PlaySE(0x560);

        app->subState = 1;
        break;
    case 1:
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_SUB_2, BG_POS_OP_SET_X, app->xOffset2);
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_X, app->xOffset2);
        BgSetPosTextAndCommit(app->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_X, app->xOffset2);
        app->xOffset2 += -16;

        if (app->xOffset >= 256) {
            app->subState = 2;
        } else {
            for (int i = 0; i < 4; i++) {
                ov87_021E7FEC(app->topScreenMonSprites[i], 60 + app->xOffset, 26 + (i * 42));
            }

            for (int i = 0; i < 3; i++) {
                ov87_021E7FD4(app->matchingCellBoxSprites[i], 0);
            }

            for (int i = 0; i < 9; i++) {
                ov87_021E7FEC(app->scratchMonSprites[i], ov87_021E82E4[i].x + app->xOffset, ov87_021E82E4[i].y);
            }
        }

        app->xOffset += 16;
        break;
    case 2:
        app->xOffset = 0;
        app->xOffset2 = 0;
        return 1;
    }

    return 0;
}
