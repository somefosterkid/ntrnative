#include "global.h"

#include "render_text.h"
#include "text.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "list_menu.h"
#include "frontier/frontier_script_context.h"

s32 ov80_0222F4D0(void **arg0);

struct BattleFrontier;
struct ColoredArrow;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct ListMenu;
struct Menu;
struct MsgData;
struct MessageFormat;
typedef struct {
    SysTask *unk_00;
    s16 unk_04;
    s16 unk_06;
    s8 unk_08;
    s8 unk_09;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_0223319C;
typedef struct {
    SysTask *unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    s16 unk_08;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_022331E8;
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
void ov80_0222F4F0(SysTask *param0, void *param1);
void ov80_0222F53C(SysTask *param0, void *param1);
void ov80_0222F5D0(FrontierMenuManager *menuManager, BOOL anchorRight);
void ov80_0222F5EC(FrontierMenuManager *menuManager, BOOL anchorBottom);

s32 ov80_0222F4D0(void **arg0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    if (TextPrinterCheckActive(((FrontierScriptManager *)(*arg0))->printerID) == 0) {
#else
    if (TextPrinterCheckActive(*(u8 *)((u8 *)(*arg0) + 0x50)) == 0) {
#endif
        TextFlags_SetCanTouchSpeedUpPrint(0);
        return 1;
    }
    return 0;
}

void ov80_0222F4F0(SysTask *param0, void *param1) {
    UnkStruct_ov104_0223319C *v0 = param1;

    v0->unk_08++;

    if (v0->unk_08 > v0->unk_09) {
        v0->unk_08 = 0;
        v0->unk_0A--;

        if (v0->unk_0A < 0) {
            SysTask_Destroy(param0);
            MI_CpuClear8(v0, sizeof(UnkStruct_ov104_0223319C));
            return;
        }

        v0->unk_04 = -v0->unk_04;
        v0->unk_06 = -v0->unk_06;
    }
}

void ov80_0222F53C(SysTask *param0, void *param1) {
    UnkStruct_ov104_022331E8 *v0 = param1;

    if (v0->unk_08 <= 0) {
        if (v0->unk_0A == 1) {
            G2_SetWnd0InsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3, 1);
            G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 1);
            G2_SetWnd0Position(v0->unk_04, v0->unk_05, v0->unk_06, v0->unk_07);
            GX_SetVisibleWnd(GX_WNDMASK_W0);
        } else {
            GX_SetVisibleWnd(GX_WNDMASK_NONE);
        }

        SysTask_Destroy(param0);
        MI_CpuClear8(v0, sizeof(UnkStruct_ov104_022331E8));
        return;
    } else {
        v0->unk_08--;
    }
}

void ov80_0222F5D0(FrontierMenuManager *menuManager, BOOL anchorRight) {
    menuManager->anchorRight = anchorRight;
}

void ov80_0222F5EC(FrontierMenuManager *menuManager, BOOL anchorBottom) {
    menuManager->anchorBottom = anchorBottom;
}
