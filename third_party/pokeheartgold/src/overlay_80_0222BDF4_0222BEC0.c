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
void FrontierScriptContext_Stop(FrontierScriptContext *ctx);
void sub_0209684C(BattleFrontier *frontier);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);
#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02096854(BattleFrontier *frontier, s8 sceneID, s16 entryPointOffset);
#else
void sub_02096854(BattleFrontier *frontier, u16 sceneID, u16 entryPointOffset);
#endif
BOOL ov80_0222BEFC(FrontierScriptContext *ctx);

BOOL FrtCmd_002(FrontierScriptContext *ctx);
BOOL FrtCmd_003(FrontierScriptContext *ctx);

BOOL FrtCmd_002(FrontierScriptContext *ctx) {
    FrontierScriptContext_Stop(ctx);
    sub_0209684C(ctx->frontierSystem->frontier);

    return 0;
}

BOOL FrtCmd_003(FrontierScriptContext *ctx) {
    FrontierScriptManager *scriptMan = ctx->frontierSystem;
    u16 sceneID = FrontierScript_ReadVar(ctx);

    sub_02096854(scriptMan->frontier, sceneID, 0xffff);
    FrontierScriptContext_Pause(ctx, ov80_0222BEFC);

    return 1;
}
