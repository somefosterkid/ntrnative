#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "frontier/frontier_script_context.h"

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

FrontierMenuManager *ov80_0222E558(FrontierScriptManager *scriptMan, u8 anchorX, u8 anchorY, u8 initalCursorPos, u8 canExitWithB, u16 *selectedOptionPtr, MessageFormat *strTemplate, MsgData *msgLoader);

FrontierMenuManager *ov80_0222E920(FrontierScriptManager *scriptMan, u8 anchorX, u8 anchorY, u8 initialCursorPos, u8 canExitWithB, u16 *selectedOptionPtr, MessageFormat *strTemplate, MsgData *msgLoader);

FrontierMenuManager *ov80_0222E920(FrontierScriptManager *scriptMan, u8 anchorX, u8 anchorY, u8 initialCursorPos, u8 canExitWithB, u16 *selectedOptionPtr, MessageFormat *strTemplate, MsgData *msgLoader) {
    return ov80_0222E558(scriptMan, anchorX, anchorY, initialCursorPos, canExitWithB, selectedOptionPtr, strTemplate, msgLoader);
}
