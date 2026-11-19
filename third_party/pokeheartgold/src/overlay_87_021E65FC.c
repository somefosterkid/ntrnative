#include "global.h"

#include "render_window.h"
#include "screen_fade.h"
#include "bg_window.h"
#include "filesystem.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "unk_0200A090.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_87_scratch_off_card_app.h"
#include "message_printer.h"
#include "msgdata.h"
#include "yes_no_prompt.h"

BOOL ov87_021E65FC(ScratchOffCardApp *app);

struct OverlayManager;
struct MessagePrinter;
struct MsgData;
struct PaletteAnimator;
struct ScratchOffCardsAppSprite;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct YesNoPrompt;
ScratchOffCardsAppSprite *ov87_021E7F6C(ScratchOffCardsAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u16 x, u16 y, u8 onSubScreen, u32 resourcePriority, u32 priority);
void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, int draw);
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
void ov87_021E6668(ScratchOffCardApp *app);

BOOL ov87_021E65FC(ScratchOffCardApp *app) {
    u8 v0;

    v0 = app->subState;
    switch (v0) {
    case 0:
        app->fadeOutDelay = 10;
        app->subState++;
    default:
        break;
    case 1:
        app->fadeOutDelay--;
        if (app->fadeOutDelay == 0) {
            ClearFrameAndWindow2(&app->windows[SCRATCH_WINDOW_STOP_SCRATCH], 0);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0U, 6, 1, HEAP_ID_122);
            app->subState++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }
    return 0;
}

void ov87_021E6668(ScratchOffCardApp *app) {
    for (int i = 0; i < 3; i++) {
        app->matchingCellBoxSprites[i] = ov87_021E7F6C(&app->spriteMan, 3, 2, ov87_021E82E4[i].x, ov87_021E82E4[i].y, 0, 2, 11);

        ov87_021E7FD4(app->matchingCellBoxSprites[i], 0);
    }
}
