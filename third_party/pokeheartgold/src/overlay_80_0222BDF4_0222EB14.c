#include "global.h"
struct FrontierMenuManager;

#include "font.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "pm_string.h"
#include "sys_task.h"
#include "frontier/frontier_script_context.h"
#include "list_menu_items.h"

s32 ov80_0222EB14(struct FrontierMenuManager *arg0);

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
enum ListMenuPagerMode {
    PAGER_MODE_NONE = 0,
    PAGER_MODE_LEFT_RIGHT_PAD,
    PAGER_MODE_SHOULDER_BUTTONS,
};
typedef struct ListMenu ListMenu;
typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);
typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);
typedef struct ListMenuTemplate {
    const ListMenuItem *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;
struct ListMenu {
    ListMenuTemplate template;
    u8 cursorPal : 4;
    u8 fillValue : 4;
    u8 cursorShadowPal : 4;
    u8 unk_21_4 : 4;
    u8 lettersSpacing : 6;
    u8 unk_23;
    u8 fontId : 7;
    u8 overrideEnabled : 1;
    struct ListMenuCursor *cursor;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_30;
    u8 unk_31;
    u8 taskId;
    u8 unk_33;
    u8 heapID;
};
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
void ov80_0222EC60(ListMenu *listMenu, u32 index, u8 yOffset);
void ov80_0222EC84(ListMenu *listMenu, u32 index, u8 onInit);
void ov80_0222EB54(FrontierMenuManager *menuManager);

s32 ov80_0222EB14(FrontierMenuManager *arg0) {
    String *string;
    s32 v1;
    u32 v0;
    u32 v3;

    v3 = 0;
    v1 = 0;
    if ((s32) arg0->optionCount > 0) {
loop_2:
        string = ((String *)arg0->listMenuChoiceStrings[v1].text);
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

void ov80_0222EB54(FrontierMenuManager *menuManager) {
    menuManager->listMenuTemplate.choices = menuManager->listMenuChoiceStrings;
    menuManager->listMenuTemplate.cursorCallback = ov80_0222EC84;
    menuManager->listMenuTemplate.printCallback = ov80_0222EC60;
    menuManager->listMenuTemplate.window = &menuManager->window;
    menuManager->listMenuTemplate.count = menuManager->optionCount;
    menuManager->listMenuTemplate.maxDisplay = 8;
    menuManager->listMenuTemplate.headerXOffset = 1;
    menuManager->listMenuTemplate.textXOffset = 12;
    menuManager->listMenuTemplate.cursorXOffset = 2;
    menuManager->listMenuTemplate.yOffset = 1;
    menuManager->listMenuTemplate.textColorFg = 1;
    menuManager->listMenuTemplate.textColorBg = 15;
    menuManager->listMenuTemplate.textColorShadow = 2;
    menuManager->listMenuTemplate.letterSpacing = 0;
    menuManager->listMenuTemplate.lineSpacing = 0;
    menuManager->listMenuTemplate.pagerMode = PAGER_MODE_NONE;
    menuManager->listMenuTemplate.fontID = FONT_SYSTEM;
    menuManager->listMenuTemplate.cursorType = 0;
    menuManager->listMenuTemplate.parent = menuManager;
}
