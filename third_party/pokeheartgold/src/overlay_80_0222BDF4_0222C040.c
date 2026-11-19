#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_42.h"
#include "pm_string.h"
#include "sys_task.h"
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

void ov80_0222C03C(FrontierScriptContext *ctx, const u8 *ptr);

u32 FrontierScriptContext_ReadWord(FrontierScriptContext *ctx);

BOOL FrtCmd_010(FrontierScriptContext *ctx);

extern const u8 ov80_0223B9D8[6][3];
void FrontierScriptContext_Call(FrontierScriptContext *ctx, const u8 *ptr);
BOOL FrtCmd_011(FrontierScriptContext *ctx);
BOOL FrtCmd_012(FrontierScriptContext *ctx);

BOOL FrtCmd_010(FrontierScriptContext *ctx) {
    s32 offset = FrontierScriptContext_ReadWord(ctx);
    ov80_0222C03C(ctx, ctx->scriptPtr + offset);
    return 0;
}

BOOL FrtCmd_011(FrontierScriptContext *ctx) {
    u8 condition = (*(ctx->scriptPtr++));
    s32 offset = FrontierScriptContext_ReadWord(ctx);
    if (ov80_0223B9D8[condition][ctx->unk_8C] == 1) {
        ov80_0222C03C(ctx, ctx->scriptPtr + offset);
    }
    return 0;
}

BOOL FrtCmd_012(FrontierScriptContext *ctx) {
    s32 offset = FrontierScriptContext_ReadWord(ctx);
    FrontierScriptContext_Call(ctx, ctx->scriptPtr + offset);
    return 0;
}
