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

void ov87_021E7FEC(ScratchOffCardsAppSprite *appSprite, int x, int y);

void ScratchOffCardsAppSprite_GetPosition(ScratchOffCardsAppSprite *appSprite, int *x, int *y, s32);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

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

extern const s8 ov87_021E827C[4][8];

extern const s8 ov87_021E838C[8];

void ov87_021E8014(ScratchOffCardsAppSprite *appSprite, int *x, int *y, s32);

BOOL ov87_021E73E0(ScratchOffCardApp *app, u8 selectedIdx, s32 arg2, s32 arg3);

BOOL ov87_021E73E0(ScratchOffCardApp *app, u8 selectedIdx, s32 arg2, s32 arg3) {
    if (app->centeringStep >= 8) {
        return TRUE;
    }

    int x, y;
    ov87_021E8014(app->cardSprites[selectedIdx], &x, &y, arg3);

    int newX = x + ov87_021E827C[selectedIdx][app->centeringStep];
    int newY = y + ov87_021E838C[app->centeringStep];
    app->centeringStep++;

    ov87_021E7FEC(app->cardSprites[selectedIdx], newX, newY);

    return FALSE;
}
