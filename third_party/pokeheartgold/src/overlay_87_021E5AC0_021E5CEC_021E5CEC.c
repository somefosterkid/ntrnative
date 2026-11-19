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

void ov87_021E7FE0(ScratchOffCardsAppSprite *appSprite, u32 priority);

void ov87_021E7FEC(ScratchOffCardsAppSprite *appSprite, int x, int y);

void ov87_021E8014(ScratchOffCardsAppSprite *appSprite, int *x, int *y);

void ov87_021E8058(ScratchOffCardsAppSprite *appSprite, u32 unused);

void ov87_021E806C(ScratchOffCardsAppSprite *appSprite, VecFx32 *scale);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

BOOL ov87_021E73E0(ScratchOffCardApp *app, u8 selectedIdx, s32);

BOOL ov87_021E5CEC(ScratchOffCardApp *app, s32 arg1, s32 arg2);

BOOL ov87_021E5CEC(ScratchOffCardApp *app, s32 arg1, s32 arg2) {
    int selectedIdx = app->selectedCardIdxs[app->currentCard - 1];

    switch (app->subState) {
    case 0:
        app->cardScaleFactor.x = 0x1000;
        app->cardScaleFactor.y = 0x1000;
        app->cardScaleFactor.z = 0x1000;
        app->xOffset = 0;
        app->centeringStep = 0;
        ov87_021E7FE0(app->cardSprites[selectedIdx], 0);
        app->subState = 1;
        break;
    case 1:
        ov87_021E73E0(app, selectedIdx, arg2);

        app->cardScaleFactor.x += 0x100;
        app->cardScaleFactor.y += 0x100;
        app->cardScaleFactor.z += 0x100;
        app->xOffset++;

        if (app->xOffset % 1 == 0) {
            ov87_021E806C(app->cardSprites[selectedIdx], &app->cardScaleFactor);
        }

        if (app->cardScaleFactor.x == 0x1800) {
            app->xOffset = 0;
            app->cardScaleFactor.x = 0x1000;
            app->cardScaleFactor.y = 0x1000;
            app->cardScaleFactor.z = 0x1000;
            PlaySE(1376);
            app->subState = 2;
        }
        break;
    case 2:
        app->xOffset += 16;

        int x, y;
        for (int i = 0; i < 4; i++) {
            ov87_021E8014(app->cardSprites[i], &x, &y);
            ov87_021E7FEC(app->cardSprites[i], x + 16, y);
        }

        if (app->xOffset >= 256) {
            ov87_021E8058(app->cardSprites[selectedIdx], 0);
            app->subState = 3;
        }
        break;
    case 3:
        app->xOffset = 0;
        return 1;
    }

    return 0;
}
