#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_42.h"
#include "pm_string.h"
#include "sys_task.h"

struct BattleFrontier;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct Menu;
struct MessageLoader;
struct StringTemplate;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
typedef struct MessageLoader MessageLoader;
typedef struct Menu Menu;
typedef struct StringTemplate StringTemplate;
typedef struct FrontierScriptManager FrontierScriptManager;
typedef struct FrontierScriptContext FrontierScriptContext;
typedef BOOL (*FrontierScrCmdFunc)(FrontierScriptContext *);
typedef BOOL (*FrontierShouldResumeScriptFunc)(FrontierScriptContext *);
struct FrontierScriptContext {
    FrontierScriptManager *frontierSystem;
    u8 padding[4];
    u16 unk_08[8];
    u8 stackPointer;
    u8 state;
    const u8 *scriptPtr;
    const u8 *stack[20];
    const FrontierScrCmdFunc *cmdTable;
    u32 cmdTableSize;
    u16 data[4];
    MessageLoader *msgLoader;
    u8 *scripts;
    FrontierShouldResumeScriptFunc shouldResume;
    u8 unk_8C;
};
typedef struct FrontierMenuManager FrontierMenuManager;
struct FrontierScriptManager {
    BattleFrontier *frontier;
    FrontierScriptContext *ctx[8];
    UnkStruct_ov104_0222E8C8 unk_24;
    enum HeapID heapID;
    u8 unk_38;
    MessageLoader *msgLoader;
    u8 *script;
    StringTemplate *strTemplate;
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
u16 *ov80_0222BE24(FrontierScriptContext *ctx, u16 varID);
u16 FrontierScriptContext_ReadHalfWord(FrontierScriptContext *ctx);
FrontierMenuManager *ov80_0222E920(FrontierScriptManager *scriptMan, u8 anchorX, u8 anchorY, u8 initialCursorPos, u8 canExitWithB, u16 *selectedOptionPtr, StringTemplate *strTemplate, MessageLoader *msgLoader);
void ov80_0222E940(FrontierMenuManager *menuManager, u32 entry, u32 altTextEntryID, u32 index);

BOOL FrtCmd_026(FrontierScriptContext *ctx);
BOOL FrtCmd_027(FrontierScriptContext *ctx);
BOOL FrtCmd_028(FrontierScriptContext *ctx);

BOOL FrtCmd_026(FrontierScriptContext *ctx) {
    FrontierScriptManager *scriptMan = ctx->frontierSystem;
    u8 anchorX = (*(ctx->scriptPtr++));
    u8 anchorY = (*(ctx->scriptPtr++));
    u8 initialCursorPos = (*(ctx->scriptPtr++));
    u8 canExitWithB = (*(ctx->scriptPtr++));
    u16 selectedOptionVar = FrontierScriptContext_ReadHalfWord(ctx);

    scriptMan->menuMan = ov80_0222E920(scriptMan, anchorX, anchorY, initialCursorPos, canExitWithB, ov80_0222BE24(ctx, selectedOptionVar), scriptMan->strTemplate, 0L);
    ctx->data[0] = selectedOptionVar;

    return 1;
}

BOOL FrtCmd_027(FrontierScriptContext *ctx) {
    FrontierScriptManager *scriptMan = ctx->frontierSystem;
    u8 anchorX = (*(ctx->scriptPtr++));
    u8 anchorY = (*(ctx->scriptPtr++));
    u8 initialCursorPos = (*(ctx->scriptPtr++));
    u8 canExitWithB = (*(ctx->scriptPtr++));
    u16 selectedOptionVar = FrontierScriptContext_ReadHalfWord(ctx);

    scriptMan->menuMan = ov80_0222E920(scriptMan, anchorX, anchorY, initialCursorPos, canExitWithB, ov80_0222BE24(ctx, selectedOptionVar), scriptMan->strTemplate, ctx->msgLoader);
    ctx->data[0] = selectedOptionVar;

    return 1;
}

BOOL FrtCmd_028(FrontierScriptContext *ctx) {
    FrontierScriptManager *scriptMan = ctx->frontierSystem;

    u16 entryID = FrontierScriptContext_ReadHalfWord(ctx);
    u16 descriptionID = FrontierScriptContext_ReadHalfWord(ctx);
    u16 index = FrontierScriptContext_ReadHalfWord(ctx);

    ov80_0222E940(scriptMan->menuMan, entryID, descriptionID, index);
    return 0;
}
