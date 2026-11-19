#include "global.h"

#include "system.h"
#include "touchscreen.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "pm_string.h"
#include "frontier/frontier_script_context.h"

s32 ov80_0222D2AC(void);

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
void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
BOOL ov80_0222D2EC(FrontierScriptContext *ctx);
BOOL FrtCmd_059(FrontierScriptContext *ctx);

s32 ov80_0222D2AC(void) {
    if ((3 & gSystem.newKeys) || (System_GetTouchNew() != 0)) {
        return 1;
    }
    return 0;
}

BOOL FrtCmd_059(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScript_ReadVar(ctx);
    FrontierScriptContext_Pause(ctx, ov80_0222D2EC);

    return 1;
}
