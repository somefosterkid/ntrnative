#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "field_system.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "list_menu.h"
#include "message_format.h"
#include "pm_string.h"
#include "render_window.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "msgdata.h"
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

FieldMenuManager *ov01_021EDC28(FieldSystem *, u8, u8, u8, u8, u16 *, MessageFormat *, Window *, MsgData *);

u32 ov01_021EE934(s32, s32, u8 *);

void ov01_021EE7B8();

void PrintCurFloorInNewWindow(FieldSystem *fieldSystem, u8 x, u8 y, u16 *p_ret, MessageFormat *msgFmt, u16 floor);

void PrintCurFloorInNewWindow(FieldSystem *fieldSystem, u8 x, u8 y, u16 *p_ret, MessageFormat *msgFmt, u16 floor) {
    u32 width;
    u32 entryID;
    FieldMenuManager *menuManager;
    SysTask *pSVar1;
    u8 auStack_14[4];

    menuManager = (FieldMenuManager *)ov01_021EDC28(fieldSystem, x, y, 0, 0, p_ret, msgFmt, 0, 0);
    width = 8 * GetFontAttribute(0, 0);
    if ((width % 8) == 0) {
        width = width / 8;
    } else {
        width = width / 8 + 1;
    }
    AddWindowParameterized(menuManager->fieldSystem->bgConfig, &menuManager->menuWindow, 3, menuManager->anchorX, menuManager->anchorY, width, 4, 0xd, 0xdd);
    LoadUserFrameGfx1(menuManager->fieldSystem->bgConfig, GF_BG_LYR_MAIN_3, 0x3d9, 0xb, 0, HEAP_ID_FIELD1);
    DrawFrameAndWindow1(&menuManager->menuWindow, 1, 0x3d9, 0xb);
    FillWindowPixelRect(&menuManager->menuWindow, 0xf, 0, 0, (u16)(width << 3), 0x20);
    ov01_021EE754(menuManager, 0x10, 0, 0);
    entryID = ov01_021EE934(fieldSystem->location->mapId, floor, auStack_14);
    ov01_021EE754(menuManager, entryID, auStack_14[0], 0x10);
    (menuManager->menuTemplate).window = &menuManager->menuWindow;
    CopyWindowToVram(&menuManager->menuWindow);
    pSVar1 = SysTask_CreateOnMainQueue(ov01_021EE7B8, menuManager, 0);
    menuManager->sysTask = pSVar1;
    return;
}
