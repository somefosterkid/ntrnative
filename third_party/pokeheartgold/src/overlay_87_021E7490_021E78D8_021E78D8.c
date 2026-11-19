#include "global.h"
#include "save.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "bg_window.h"
#include "pm_string.h"
#include "options.h"
#include "palette.h"
#include "overlay_87_scratch_off_card_app.h"
#include "overlay_manager.h"
#include "message_printer.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct MessagePrinter;

struct G2dRenderer;

struct MsgData;

struct NARC;

struct Options;

struct PaletteAnimator;

struct PaletteData;

struct SaveData;

struct ScratchOffCardApp;

struct ScratchOffCardsAppSprite;

struct ScratchOffCardsAppSpriteManager;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskQueue;

struct TouchPadDataBuffer;

struct Window;

struct YesNoPrompt;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void ov87_021E803C(ScratchOffCardsAppSprite *appSprite, u32 animID);

void ov87_021E80A8(ScratchOffCardsAppSprite *appSprite, BOOL mosaic);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

BOOL PlaySE(u16 seqID, u8);

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

BOOL ov87_021E78D8(ScratchOffCardApp *app, u8 decreaseMosaic);

BOOL ov87_021E78D8(ScratchOffCardApp *app, u8 decreaseMosaic) {
    int i;

    for (i = 0; i < 3; i++) {
        if (app->scratchCellValues[app->scratchedOffCellIdxs[i]] == 4) {
            break;
        }
    }

    if (i == 3) {
        return 0;
    }

    if (!decreaseMosaic) {
        if (app->mosaicSize == 0) {
            PlaySE(1379, decreaseMosaic);
        }

        if (app->mosaicSize < 4) {
            app->mosaicSize++;
        } else {
            for (i = 0; i < 3; i++) {
                if (app->scratchCellValues[app->scratchedOffCellIdxs[i]] == 4) {
                    ov87_021E803C(app->winMonSprites[i], app->winningCellValue);
                }
            }

            return 0;
        }
    } else if (app->mosaicSize > 0) {
        app->mosaicSize--;
    } else {
        for (i = 0; i < 3; i++) {
            ov87_021E80A8(app->winMonSprites[i], 0);
        }

        return 0;
    }

    G2_SetOBJMosaicSize(app->mosaicSize, app->mosaicSize);

    return 1;
}
