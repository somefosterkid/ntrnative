#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "options.h"
#include "palette.h"
#include "pm_string.h"
#include "render_window.h"
#include "save.h"
#include "sys_task.h"
#include "touchscreen.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_87_scratch_off_card_app.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct BgConfig;

struct FontSpecialCharsContext;

struct G2dRenderer;

struct MessageLoader;

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

struct StringTemplate;

struct SysTask;

struct SysTaskManager;

struct TouchPadDataBuffer;

struct Window;

struct YesNoTouchMenu;

typedef struct ApplicationManager ApplicationManager;

typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, int draw);

typedef struct FontSpecialCharsContext FontSpecialCharsContext;

typedef struct MessageLoader MessageLoader;

typedef struct SysTaskManager SysTaskManager;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct StringTemplate StringTemplate;

typedef union {
    struct {
        u8 top;
        u8 bottom;
        u8 left;
        u8 right;
    } rect;

    struct {
        u8 code;
        u8 x;
        u8 y;
        u8 r;
    } circle;
} TouchScreenRect, TouchScreenHitTable;

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

typedef struct YesNoTouchMenu YesNoTouchMenu;

extern const TouchScreenRect ov87_021E81C0[4 + 1];

BOOL ov87_021E5C38(ScratchOffCardApp *app);

BOOL ov87_021E5C38(ScratchOffCardApp *app) {
    switch (app->subState) {
    case 0:
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 1);
        app->subStateTimer = 30;
        app->subState = 1;
        break;
    case 1:
        if (app->subStateTimer == 0) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
            ov87_021E7FD4(app->centerMessageBoxSprite, 0);
        } else {
            app->subStateTimer--;
        }

        int selectedCard = (int)((const union TouchscreenHitbox *)(TouchscreenHitbox_FindRectAtTouchHeld((const TouchscreenHitbox *)(ov87_021E81C0))));

        if (selectedCard != -1) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, 0);
            ov87_021E7FD4(app->centerMessageBoxSprite, 0);
            PlaySE(1508);

            app->selectedCardIdxs[app->currentCard] = selectedCard;
            app->currentCard++;

            u32 x, y;
            System_GetTouchHeldCoords(&x, &y);
            ClearFrameAndWindow2(&app->windows[SCRATCH_WINDOW_SELECT_CARD], 0);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG0, 0);
            app->subState = 2;
        }
        break;
    case 2:
        return 1;
    }

    return 0;
}
