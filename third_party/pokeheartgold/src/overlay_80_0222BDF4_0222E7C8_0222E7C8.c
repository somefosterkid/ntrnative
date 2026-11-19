#include "global.h"
#include "sys_task.h"
#include "frontier_graphics.h"
#include "constants/heap.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "screen_fade_internal.h"
#include "font.h"
#include "msgdata.h"
#include "list_menu_items.h"
#include "list_menu.h"
#include "frontier/frontier_script_context.h"

struct BattleFrontier;

struct ListMenuCursor;

struct FrontierGraphics;

struct FrontierMenuManager;

struct FrontierScriptContext;

struct FrontierScriptManager;

struct ListMenu;

struct ListMenuTemplate;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MsgData;

struct Pokemon;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct SysTask;

struct Window;

typedef struct BattleFrontier BattleFrontier;

typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

typedef struct ListMenu2D ListMenu2D;

u32 Handle2dMenuInput(ListMenu2D *menu);

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
    ListMenu2DTemplate menuTemplate;
    ListMenu2D *menu;
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
    ListMenu2D *menu;
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

BOOL IsPaletteFadeFinished(void);

void ov80_0222E88C(FrontierMenuManager *menuManager);

void ov80_0222E8FC(FrontierMenuManager *menuManager);

void PrintMessage(FrontierScriptManager *scriptMan, enum Font font, int renderDelay, BOOL canSpeedUp, int autoScroll);

void ov80_0222F030(FrontierGraphics *param0, Pokemon *param1, enum HeapID heapID, int param3, int param4, int param5, int param6, int param7, int param8, u16 param9);

void ov80_0222E7C8(SysTask *task, void *data);

BOOL System_GetTouchNew(void);

u8 Get2dMenuSelection(void *);

void ov80_0222E7C8(SysTask *task, void *data) {
    FrontierMenuManager *menuManager = data;

    if (menuManager->sysTaskDelay != 0) {
        menuManager->sysTaskDelay--;
        return;
    }

    if (!IsPaletteFadeFinished()) {
        return;
    }

    u32 selectedEntry = Handle2dMenuInput(menuManager->menu);
    if (selectedEntry == -1 && System_GetTouchNew() == 1) {
        selectedEntry = Get2dMenuSelection(menuManager->menu);
    }

    if ((gSystem.newAndRepeatedKeys & (0x0040)) || (gSystem.newAndRepeatedKeys & (0x0080)) || (gSystem.newAndRepeatedKeys & (0x0020)) || (gSystem.newAndRepeatedKeys & (0x0010))) {
        ov80_0222E8FC(menuManager);
    }

    if (*menuManager->selectedOptionPtr == 0xeedd) {
        ov80_0222E88C(data);
    } else {
        switch (selectedEntry) {
        case -1:
            break;
        case -2:
            if (menuManager->canExitWithB == 1) {
                *menuManager->selectedOptionPtr = -2;
                ov80_0222E88C(data);
            }
            break;
        default:
            *menuManager->selectedOptionPtr = selectedEntry;
            ov80_0222E88C(data);
            break;
        }
    }
}
