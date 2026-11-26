#ifndef POKEHEARTGOLD_FRONTIER_MENU_MANAGER_H
#define POKEHEARTGOLD_FRONTIER_MENU_MANAGER_H

#include "bg_window.h"
#include "list_menu.h"
#include "list_menu_2d.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "sys_task.h"

struct FrontierScriptManager;

// DS size 0x2E8 (see the allocation in the menu creators); head of the object is shared by the
// 2D-menu and the list-menu flavours.
typedef struct FrontierMenuManager {
    struct FrontierScriptManager *scriptMan;
    SysTask *sysTask;
    Window window;
    Window *parent;
    String *choiceStringBuffers[28];
    MsgData *msgLoader;
    MessageFormat *strTemplate;
    u8 sysTaskDelay;
    u8 unused;
    u8 initialCursorPos;
    u8 canExitWithB : 1;
    u8 freeMsgLoaderOnDelete : 1;
    u8 unused2 : 4;
    u8 anchorRight : 1;
    u8 anchorBottom : 1;
    u8 anchorX;
    u8 anchorY;
    u8 unused3;
    u8 optionCount;
    u16 *unused4;
    u16 *selectedOptionPtr;
    struct ListMenu2DTemplate menuTemplate;
    struct ListMenu2D *menu;
    ListMenuItem menuChoiceStrings[28];
    ListMenuTemplate listMenuTemplate;
    ListMenu *listMenu;
    u16 unused5;
    u16 listMenuAltTextIndex;
    ListMenuItem listMenuChoiceStrings[28];
    u16 choicesAltTextEntryIDs[28];
    u16 cursorPos;
} FrontierMenuManager;

#endif // POKEHEARTGOLD_FRONTIER_MENU_MANAGER_H
