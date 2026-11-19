#include "global.h"

#include "gf_gfx_planes.h"
#include "options.h"
#include "unk_02005D10.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_87_scratch_off_card_app.h"
#include "pm_string.h"
#include "palette.h"
#include "overlay_manager.h"
#include "message_printer.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct CoordinatesS16;

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

void ScratchOffCardsAppSprite_SetPriority(ScratchOffCardsAppSprite *appSprite, u32 priority);

void ov87_021E7FEC(ScratchOffCardsAppSprite *appSprite, int x, int y);

void ScratchOffCardsApp_DrawMessageBox(Window *window, int frame);

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

u8 ov87_021E7048(ScratchOffCardApp *app);

extern const CoordinatesS16 ov87_021E81A0[4];

BOOL ov87_021E5B48(ScratchOffCardApp *app);

void ov87_021E7FE0(ScratchOffCardsAppSprite *appSprite, u32 priority);

void ov87_021E8134(Window *window, int frame);

BOOL ov87_021E5B48(ScratchOffCardApp *app) {
    switch (app->subState) {
    case 0:
        app->xOffset = -256;

        for (int i = 0; i < 4; i++) {
            ov87_021E7FEC(app->cardSprites[i], ov87_021E81A0[i].x + app->xOffset, ov87_021E81A0[i].y);
            ov87_021E7FE0(app->cardSprites[i], 20 + i);
        }

        ov87_021E8134(&app->windows[SCRATCH_WINDOW_SELECT_CARD], Options_GetFrame(app->options));
        ov87_021E7048(app);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 1);
        app->xOffset2 = 4 - 1;
        PlaySE(1376);
        app->subState = 1;
        break;
    case 1:
        app->xOffset += 32;
        ov87_021E7FEC(app->cardSprites[app->xOffset2], ov87_021E81A0[app->xOffset2].x + app->xOffset, ov87_021E81A0[app->xOffset2].y);

        if (app->xOffset >= 0) {
            if (app->xOffset2 == 0) {
                app->subState = 2;
            } else {
                PlaySE(1376);
                app->xOffset2--;
                app->xOffset = -256;
            }
        }
        break;
    case 2:
        app->xOffset = 0;
        return 1;
    }

    return 0;
}
