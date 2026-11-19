#include "global.h"

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

struct OverlayManager;
struct MessagePrinter;
struct MsgData;
struct PaletteAnimator;
struct ScratchOffCardsAppSprite;
struct SpriteList;
struct SpriteResourceCollection;
struct MessageFormat;
struct YesNoPrompt;
void ov87_021E810C(BgConfig *bgConfig, Window *window);
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
u8 ov87_021E6F38(ScratchOffCardApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 fontID);
void ov87_021E7244(ScratchOffCardApp *app, u32 idx, s32 num);
u8 ov87_021E6F98(ScratchOffCardApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 fontID);

u8 ov87_021E7008(ScratchOffCardApp *app);
u8 ov87_021E7048(ScratchOffCardApp *app);
u8 ov87_021E708C(ScratchOffCardApp *app);

u8 ov87_021E7008(ScratchOffCardApp *app) {
    ov87_021E810C(app->bgConfig, &app->windows[SCRATCH_WINDOW_STOP_SCRATCH]);
    u8 printerID = ov87_021E6F38(app, &app->windows[SCRATCH_WINDOW_STOP_SCRATCH], 6, 1, 1, 0, 1, 2, 15, FONT_SYSTEM);
    ScheduleWindowCopyToVram(&app->windows[SCRATCH_WINDOW_STOP_SCRATCH]);

    return printerID;
}

u8 ov87_021E7048(ScratchOffCardApp *app) {
    ov87_021E7244(app, 0, app->currentCard + 1);
    u8 printerID = ov87_021E6F38(app, &app->windows[SCRATCH_WINDOW_SELECT_CARD], 12, 1, 1, 0, 1, 2, 15, FONT_MESSAGE);
    ScheduleWindowCopyToVram(&app->windows[SCRATCH_WINDOW_SELECT_CARD]);

    return printerID;
}

u8 ov87_021E708C(ScratchOffCardApp *app) {
    ov87_021E7244(app, 0, app->currentCard + 1);
    u8 printerID = ov87_021E6F98(app, &app->windows[SCRATCH_WINDOW_START_CARD], 9, 8 * 6, 1 + 4, 0, 1, 2, 0, FONT_SYSTEM);
    ScheduleWindowCopyToVram(&app->windows[SCRATCH_WINDOW_START_CARD]);

    return printerID;
}
