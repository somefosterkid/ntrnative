#include "global.h"

#include "render_text.h"
#include "text.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "frontier/frontier_script_context.h"

void ov80_0222E344(void *arg0, enum Font arg1, u32 arg2, s32 arg3, s32 arg4);

struct BattleFrontier;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct Menu;
struct MsgData;
struct MessageFormat;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
typedef struct Menu Menu;
typedef struct FrontierScriptManager FrontierScriptManager;
typedef struct FrontierMenuManager FrontierMenuManager;
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
void ClearFrameAndWindow2(Window *window, u8 skipTransfer);
void ov80_0222E38C(FrontierScriptManager *scriptMan);

void ov80_0222E344(void *arg0, enum Font fontId, u32 textSpeed, s32 enable, s32 param) {
    TextFlags_SetCanABSpeedUpPrint(enable);
    TextFlags_SetAutoScrollParam(param);
    if ((textSpeed == 0) || (textSpeed == 0xFF)) {
        TextFlags_SetCanTouchSpeedUpPrint(0);
    } else {
        TextFlags_SetCanTouchSpeedUpPrint(1);
    }
    ((FrontierScriptManager *)arg0)->printerID = AddTextPrinterParameterized(&((FrontierScriptManager *)arg0)->msgWindow, fontId, ((FrontierScriptManager *)arg0)->string, 0U, 0U, textSpeed, NULL);
}

void ov80_0222E38C(FrontierScriptManager *scriptMan) {
    ((scriptMan->isMsgBoxOpen == 1) ? (void)0 : GF_AssertFail());

    ClearFrameAndWindow2(&scriptMan->msgWindow, 0);
    RemoveWindow(&scriptMan->msgWindow);

    scriptMan->isMsgBoxOpen = 0;
}
