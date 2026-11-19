#include "global.h"

#include "unk_02005D10.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
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

void ov87_021E7FD4(ScratchOffCardsAppSprite *appSprite, BOOL draw);

void ov87_021E803C(ScratchOffCardsAppSprite *appSprite, u32 animID);

s32 ov87_021E8084(void *, s32);

s32 ov87_021E70D0(ScratchOffCardApp *app);

struct OverlayManager;
struct MessagePrinter;
struct MsgData;
struct PaletteAnimator;
struct ScratchOffCardsAppSprite;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct YesNoPrompt;
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
u8 ov87_021E6F98(ScratchOffCardApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 fontID);
u8 ov87_021E7134(ScratchOffCardApp *app);
u8 ov87_021E717C(ScratchOffCardApp *app);
u8 ov87_021E71B4(ScratchOffCardApp *app);

s32 ov87_021E70D0(ScratchOffCardApp *app) {
    u8 i;

    i = 0;
    do {
        ov87_021E803C(app->winMonSprites[i], app->scratchCellValues[app->scratchedOffCellIdxs[i]]);
        ov87_021E7FD4(app->winMonSprites[i], 1);
        i += 1;
    } while ((u32)i < 3U);
    ov87_021E7FD4(app->winPopupSprite, 1);
    ov87_021E8084(app->winPopupSprite, 1);
    PlaySE(0x561U);
    return 0;
}

u8 ov87_021E7134(ScratchOffCardApp *app) {
    PlaySE(1523);
    u8 printerID = ov87_021E6F98(app, &app->windows[SCRATCH_WINDOW_TOO_BAD], 11, 8 * 6, 1 + 4, 0, 1, 2, 0, FONT_SYSTEM);
    ScheduleWindowCopyToVram(&app->windows[SCRATCH_WINDOW_TOO_BAD]);

    return printerID;
}

u8 ov87_021E717C(ScratchOffCardApp *app) {
    u8 printerID = ov87_021E6F98(app, &app->windows[SCRATCH_WINDOW_NEXT_CARD], 4, 8 * 6, 1, 0, 1, 1, 0, FONT_SUBSCREEN);
    ScheduleWindowCopyToVram(&app->windows[SCRATCH_WINDOW_NEXT_CARD]);

    return printerID;
}

u8 ov87_021E71B4(ScratchOffCardApp *app) {
    u8 printerID = ov87_021E6F98(app, &app->windows[SCRATCH_WINDOW_CANCEL], 5, 8 * 3, 1, 0, 1, 1, 0, FONT_SUBSCREEN);
    ScheduleWindowCopyToVram(&app->windows[SCRATCH_WINDOW_CANCEL]);

    return printerID;
}
