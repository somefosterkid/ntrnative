#include "global.h"

#include "message_format.h"

#include "overlay_87_scratch_off_card_app.h"

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

extern const u8 ov87_021E8388[4];

u8 ov87_021E71EC(ScratchOffCardApp *app, u8 idx);

u8 ov87_021E71EC(ScratchOffCardApp *app, u8 idx) {
    BufferItemName(app->strTemplate, 0, app->winnableItems[idx]);
    u8 printerID = ov87_021E6F98(app, &app->windows[SCRATCH_WINDOW_ITEM_NAME_1 + idx], 1, 8 * 6 - 4, ov87_021E8388[idx], 0, 1, 2, 0, FONT_SYSTEM);
    ScheduleWindowCopyToVram(&app->windows[SCRATCH_WINDOW_ITEM_NAME_1 + idx]);

    return printerID;
}
