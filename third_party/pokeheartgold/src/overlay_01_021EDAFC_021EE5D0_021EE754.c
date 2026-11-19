#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "font.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sys_task.h"
#include "text.h"
#include "overlay_01_field_menu_manager.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct ColoredArrow ColoredArrow;

typedef struct MenuTemplate MenuTemplate;

typedef struct Menu Menu;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


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

void ov01_021EE754(FieldMenuManager *menuManager, u16 entryID, u8 xOffset, u8 yOffset);

void ov01_021EE754(FieldMenuManager *menuManager, u16 entryID, u8 xOffset, u8 yOffset) {
    String *dest;
    String *dest_00;

    dest = String_New(0x5a, HEAP_ID_FIELD1);
    dest_00 = String_New(0x5a, HEAP_ID_FIELD1);
    ReadMsgDataIntoString(menuManager->messageLoader, (u32)entryID, dest);
    StringExpandPlaceholders(menuManager->stringTemplate, dest_00, dest);
    AddTextPrinterParameterized(&menuManager->menuWindow, FONT_SYSTEM, dest_00, (u32)xOffset, (u32)yOffset, 0xff, (PrinterCallback_t)0x0);
    String_Delete(dest);
    String_Delete(dest_00);
    return;
}
