#include "global.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "gf_3d_loader.h"
#include "move.h"
#include "overlay_manager.h"
#include "pokemon.h"
#include "sav_chatot.h"
#include "unk_0200A090.h"
#include "overlay_68_move_relearner.h"
#include "pokemon_summary_app.h"
#include "list_menu.h"
#include "msgdata.h"
struct OverlayManager;
struct ListMenu;
struct Menu;
struct String;
struct ListMenuItem;
struct MessageFormat;

struct OverlayManager;

struct ListMenu;

typedef u16 charcode_t;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;


typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

typedef u32 TextColor;

typedef struct ColoredArrow {
    TextColor color;
    String *string;
} ColoredArrow;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct MenuTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} MenuTemplate;

typedef struct Menu {
    MenuTemplate template;
    ColoredArrow *cursor;
    u32 cancelKeys;
    u8 dummy14;
    u8 cursorPos;
    u8 width;
    u8 xOffset;
    u8 yOffset;
    u8 letterWidth;
    u8 lineHeight;
    u8 lastAction;
    u8 heapID;
} Menu;

enum {
    MOVE_REMINDER_WIN_LABEL_BATTLE_MOVES = 0,
    MOVE_REMINDER_WIN_LABEL_CONTEST_MOVES,
    MOVE_REMINDER_WIN_LABEL_CATEGORY,
    MOVE_REMINDER_WIN_LABEL_POWER,
    MOVE_REMINDER_WIN_LABEL_ACCURACY,
    MOVE_REMINDER_WIN_LABEL_PP,
    MOVE_REMINDER_WIN_LABEL_APPEAL_PTS,
    MOVE_REMINDER_WIN_MOVE_POWER,
    MOVE_REMINDER_WIN_MOVE_ACCURACY,
    MOVE_REMINDER_WIN_MOVE_PP,
    MOVE_REMINDER_WIN_MOVE_BATTLE_DESCRIPTION,
    MOVE_REMINDER_WIN_MOVE_CONTEST_DESCRIPTION,
    MOVE_REMINDER_WIN_MESSAGE_BOX,
    MOVE_REMINDER_WIN_MOVES_NAMES,
    MOVE_REMINDER_WIN_YES_NO_MENU,
    MAX_MOVE_REMINDER_WIN
};

enum {
    MOVE_REMINDER_SPRITE_SIDE_ARROW_LEFT = 0,
    MOVE_REMINDER_SPRITE_SIDE_ARROW_RIGHT,
    MOVE_REMINDER_SPRITE_SCROLL_ARROW_UP,
    MOVE_REMINDER_SPRITE_SCROLL_ARROW_DOWN,
    MOVE_REMINDER_SPRITE_MOVE_SELECTOR,
    MOVE_REMINDER_SPRITE_CATEGORY,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_0,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_1,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_2,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_3,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_4,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_5,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_6,
    MAX_MOVE_REMINDER_SPRITE
};

u16 ov68_021E6BEC(MoveRelearner *controller);

enum {
    MOVE_REMINDER_STATE_INIT = 0,
    MOVE_REMINDER_STATE_PROCESS_MAIN_INPUT,
    MOVE_REMINDER_STATE_PRINT_MESSAGE_BOX_TEXT,
    MOVE_REMINDER_STATE_DRAW_YES_NO_MENU,
    MOVE_REMINDER_STATE_PROCESS_YES_NO_INPUT,
    MOVE_REMINDER_STATE_REPLACE_OLD_MOVE,
    MOVE_REMINDER_STATE_PLAY_FANFARE,
    MOVE_REMINDER_STATE_CONFIRM_SELECTION,
    MOVE_REMINDER_STATE_FADE_TO_EXIT,
    MOVE_REMINDER_STATE_EXIT,
    MOVE_REMINDER_STATE_FADE_TO_SUMMARY_SCREEN,
    MOVE_REMINDER_STATE_RUN_SUMMARY_SCREEN,
    MOVE_REMINDER_STATE_PROCESS_SUMMARY_SCREEN_INPUT,
};

int ov68_021E614C(MoveRelearner *controller);

s32 ov68_021E6C14(MoveRelearner *, s32);
s32 ov68_021E61A0(MoveRelearner *arg0);

int ov68_021E614C(MoveRelearner *controller) {
    u32 value = ov68_021E6BEC(controller);
    SetMonData(controller->args->mon, MON_DATA_MOVE1 + controller->args->moveSlot, &value);

    value = 0;
    SetMonData(controller->args->mon, MON_DATA_MOVE1_PP_UPS + controller->args->moveSlot, &value);

    value = GetMoveMaxPP(ov68_021E6BEC(controller), 0);
    SetMonData(controller->args->mon, MON_DATA_MOVE1_PP + controller->args->moveSlot, &value);

    controller->args->cancelled = FALSE;

    return MOVE_REMINDER_STATE_FADE_TO_EXIT;
}

s32 ov68_021E61A0(MoveRelearner *arg0) {
    ov68_021E6C14(arg0, 6);
    arg0->nextState = 5;
    return 2;
}
