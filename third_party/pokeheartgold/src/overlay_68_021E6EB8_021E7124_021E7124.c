#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "list_menu.h"
#include "move.h"
#include "options.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02077678.h"
#include "overlay_68_move_relearner.h"
#include "pokemon_summary_app.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "sav_chatot.h"
struct OverlayManager;
struct Menu;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct ListMenuItem;
struct MessageFormat;

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct ColoredArrow ColoredArrow;

typedef struct MenuTemplate MenuTemplate;

typedef struct Menu Menu;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef u16 charcode_t;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef u32 TextColor;

struct ColoredArrow {
    TextColor color;
    String *string;
};

struct MenuTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
};

struct Menu {
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
};

struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

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

void ov68_021E7124(MoveRelearner *controller, u16 move);

void ov68_021E7124(MoveRelearner *controller, u16 move) {
    enum NarcId temp_r6;
    u32 temp_r4;

    temp_r4 = GetMoveAttr(move, MOVEATTR_CLASS);
    temp_r6 = GetMoveCategoryIconGfxNarcId();
    SpriteSystem_ReplaceCharResObj((SpriteSystem *)controller->spriteSystem, (SpriteManager *)controller->spriteManager, temp_r6, GetMoveCategoryIconGfxCharFileId(temp_r4), 1, 0xB8AA);
    ManagedSprite_SetPaletteOverride(controller->sprites[3], GetMoveCategoryIconGfxPlttOverride(temp_r4) + 4);
}
