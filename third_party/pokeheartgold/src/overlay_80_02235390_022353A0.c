#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "unk_02005D10.h"
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
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);
static BOOL ov80_022353D0(FrontierScriptContext *ctx);

BOOL FrtCmd_086(FrontierScriptContext *ctx);
BOOL FrtCmd_087(FrontierScriptContext *ctx);
static BOOL ov80_022353D0(FrontierScriptContext *ctx);

BOOL FrtCmd_086(FrontierScriptContext *ctx) {
    StopSE(FrontierScript_ReadVar(ctx), 0);
    return 0;
}

BOOL FrtCmd_087(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScript_ReadVar(ctx);
    FrontierScriptContext_Pause(ctx, ov80_022353D0);

    return 1;
}

static BOOL ov80_022353D0(FrontierScriptContext *ctx) {
    return IsSEPlaying(ctx->data[0]) == 0;
}
