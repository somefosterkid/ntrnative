#ifndef POKEHEARTGOLD_OVERLAY_01_FIELD_MENU_MANAGER_H
#define POKEHEARTGOLD_OVERLAY_01_FIELD_MENU_MANAGER_H

// FieldMenuManager (sizeof 0x2e0): the field's list/touch menu object (ov01_021EDF78 allocates it). Layout merged by
// tools/hg/promote.py from the m2c local copies; hk526 typed every field with the HGSS library types (offsets unchanged).

#include "bg_window.h"
#include "list_menu.h"
#include "list_menu_2d.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"

struct FieldSystem;
struct SysTask;

typedef struct FieldMenuManager {
    struct FieldSystem *fieldSystem;
    struct SysTask *sysTask;
    Window menuWindow;
    Window *parentWindow;
    String *choicesStringsBuffers[28];
    MsgData *messageLoader;
    MessageFormat *stringTemplate;
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
    u8 optionsCount;
    u16 *unused4;
    u16 *selectedOptionPtr;
    u16 *listMenuListOffsetPtr;
    u16 *listMenuCursorPosPtr;
    struct ListMenu2DTemplate menuTemplate;
    struct ListMenu2D *menu;
    ListMenuItem menuChoicesStrings[28];
    ListMenuTemplate listMenuTemplate;
    ListMenu *listMenu;
    u16 unused5;
    u16 listMenuAltTextIndex;
    ListMenuItem listMenuChoicesStrings[28];
    u16 choicesAltTextStringIDs[28];
    u16 cursorPos;
    u8 pad_2DE[0x2];
} FieldMenuManager;

// sizeof(FieldMenuManager) == 0x2e0

#endif // POKEHEARTGOLD_OVERLAY_01_FIELD_MENU_MANAGER_H
