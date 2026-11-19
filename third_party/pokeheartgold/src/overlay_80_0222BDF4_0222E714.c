#include "global.h"
struct FrontierMenuManager;

#include "font.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "pm_string.h"
#include "sys_task.h"
#include "list_menu.h"
#include "frontier/frontier_script_context.h"

s32 ov80_0222E714(struct FrontierMenuManager *arg0);

struct BattleFrontier;
struct ColoredArrow;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct ListMenu;
struct Menu;
struct MsgData;
struct MessageFormat;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
typedef struct ColoredArrow ColoredArrow;
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
typedef struct Menu Menu;
typedef struct FrontierScriptManager FrontierScriptManager;
typedef struct FrontierMenuManager {
    FrontierScriptManager *scriptMan;
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
    MenuTemplate menuTemplate;
    Menu *menu;
    ListMenuItem menuChoiceStrings[28];
    ListMenuTemplate listMenuTemplate;
    ListMenu *listMenu;
    u16 unused5;
    u16 listMenuAltTextIndex;
    ListMenuItem listMenuChoiceStrings[28];
    u16 choicesAltTextEntryIDs[28];
    u16 cursorPos;
} FrontierMenuManager;
struct FrontierScriptManager {
    BattleFrontier *frontier;
    FrontierScriptContext *ctx[8];
    UnkStruct_ov104_0222E8C8 unk_24;
    enum HeapID heapID;
    u8 unk_38;
    MsgData *msgLoader;
    u8 *script;
    MessageFormat *strTemplate;
    String *string;
    String *fmtString;
    u8 printerID;
    s8 numSetContexts;
    u16 scene;
    u8 unused[5];
    u8 movementCount;
    u8 isMsgBoxOpen;
    int unused2;
    FrontierMenuManager *menuMan;
    Window msgWindow;
    Menu *menu;
    u16 *unused3;
    void *savingIcon;
    int unused4[10];
    Window *castleHostInfoWin;
    Window *castlePlayer2InfoWin;
    u16 unk_B0;
    u16 unk_B2;
    u16 unk_B4;
    u16 unk_B6;
    u16 *unk_B8;
};
void ov80_0222E754(FrontierMenuManager *menuManager);

s32 ov80_0222E714(FrontierMenuManager *arg0) {
    String *string;
    s32 v1;
    u32 v0;
    u32 v3;

    v3 = 0;
    v1 = 0;
    if ((s32) arg0->optionCount > 0) {
loop_2:
        string = ((String *)arg0->menuChoiceStrings[v1].text);
        if (string != NULL) {
            v0 = FontID_String_GetWidth(FONT_SYSTEM, string, 0U);
            if (v3 < v0) {
                v3 = v0;
            }
            v1 += 1;
            if (v1 < (s32) arg0->optionCount) {
                goto loop_2;
            }
        }
    }
    return v3 + 0xC;
}

void ov80_0222E754(FrontierMenuManager *menuManager) {
    menuManager->menuTemplate.choices = menuManager->menuChoiceStrings;
    menuManager->menuTemplate.window = &menuManager->window;
    menuManager->menuTemplate.fontID = FONT_SYSTEM;
    menuManager->menuTemplate.xSize = 1;
    menuManager->menuTemplate.ySize = menuManager->optionCount;
    menuManager->menuTemplate.lineSpacing = 0;
    menuManager->menuTemplate.suppressCursor = 0;

    if (menuManager->optionCount >= 4) {
        menuManager->menuTemplate.loopAround = 1;
    } else {
        menuManager->menuTemplate.loopAround = 0;
    }
}
