#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "overlay_42.h"
#include "pm_string.h"
#include "sys_task.h"

struct BattleFrontier;
struct FrontierMenuManager;
struct FrontierMessageOptions;
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
void FrontierScriptContext_Call(FrontierScriptContext *ctx, const u8 *ptr);
u32 FrontierScriptContext_ReadWord(FrontierScriptContext *ctx);
extern const u8 ov80_0223B9D8[6][3];
u16 FrontierScriptContext_ReadHalfWord(FrontierScriptContext *ctx);
typedef struct FrontierMessageOptions FrontierMessageOptions;
struct FrontierMessageOptions {
    u8 renderDelay;
    u8 scrollFlags;
    u8 font;
    u8 unused;
};
void ov80_0222E268(FrontierScriptManager *scriptMan, const MessageLoader *msgLoader, u32 messageID, u8 canSpeedUp, FrontierMessageOptions *msgOptions);
void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);
BOOL ov80_0222C17C(FrontierScriptContext *ctx);

BOOL FrtCmd_014(FrontierScriptContext *ctx);
BOOL FrtCmd_015(FrontierScriptContext *ctx);
BOOL FrtCmd_016(FrontierScriptContext *ctx);
BOOL FrtCmd_017(FrontierScriptContext *ctx);

BOOL FrtCmd_014(FrontierScriptContext *ctx) {
    u8 condition = (*(ctx->scriptPtr++));
    s32 offset = FrontierScriptContext_ReadWord(ctx);
    if (ov80_0223B9D8[condition][ctx->unk_8C] == 1) {
        FrontierScriptContext_Call(ctx, ctx->scriptPtr + offset);
    }
    return 0;
}

BOOL FrtCmd_015(FrontierScriptContext *ctx) {
    FrontierMessageOptions printParams;
    u16 messageID = FrontierScriptContext_ReadHalfWord(ctx);

    printParams.renderDelay = 0;
    printParams.scrollFlags = 0;
    printParams.font = FONT_MESSAGE;
    printParams.unused = 0;

    ov80_0222E268(ctx->frontierSystem, ctx->msgLoader, messageID, 0, &printParams);

    return 0;
}

BOOL FrtCmd_016(FrontierScriptContext *ctx) {
    u16 messageID = FrontierScriptContext_ReadHalfWord(ctx);

    ov80_0222E268(ctx->frontierSystem, ctx->msgLoader, messageID, 0, 0L);
    FrontierScriptContext_Pause(ctx, ov80_0222C17C);
    return 1;
}

BOOL FrtCmd_017(FrontierScriptContext *ctx) {
    u16 messageID = FrontierScriptContext_ReadHalfWord(ctx);

    ov80_0222E268(ctx->frontierSystem, ctx->msgLoader, messageID, 1, 0L);
    FrontierScriptContext_Pause(ctx, ov80_0222C17C);
    return 1;
}
