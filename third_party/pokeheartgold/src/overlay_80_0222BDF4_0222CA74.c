#include "global.h"

#include "frontier_launch.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_42.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"

struct BattleFrontier;
struct FrontierGraphics;
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
void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
static BOOL ov80_0222CA94(FrontierScriptContext *ctx);
typedef struct FrontierGraphics FrontierGraphics;
ManagedSprite *ov80_02239700(FrontierGraphics *param0, u16 param1);
u32 ov80_02239734(FrontierGraphics *param0, u16 param1);
extern void ov80_02239BF0(BattleFrontier *param0, u16 param1, u8 param2, s16 param3[], int param4);

BOOL FrtCmd_051(FrontierScriptContext *ctx);
static BOOL ov80_0222CA94(FrontierScriptContext *ctx);
void ov80_0222CAD0(FrontierScriptContext *ctx, int param1, int param2);
BOOL FrtCmd_083(FrontierScriptContext *ctx);

BOOL FrtCmd_051(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScript_ReadVar(ctx);
    FrontierScriptContext_Pause(ctx, ov80_0222CA94);

    return 1;
}

static BOOL ov80_0222CA94(FrontierScriptContext *ctx) {
    ManagedSprite *managedSprite;
    FrontierGraphics *v1 = sub_0209680C(ctx->frontierSystem->frontier);

    managedSprite = ov80_02239700(v1, ctx->data[0]);

    if (ov80_02239734(v1, ctx->data[0]) == 0 || ManagedSprite_IsAnimated(managedSprite) == 0) {
        return 1;
    }

    return 0;
}

void ov80_0222CAD0(FrontierScriptContext *ctx, int param1, int param2) {
    FrontierScriptManager *v0 = ctx->frontierSystem;
    s16 *v1;
    u16 v2;
    int v3;

    ((param2 <= 7) ? (void)0 : GF_AssertFail());

    v1 = Heap_Alloc(v0->heapID, sizeof(s16) * param2);
    v2 = FrontierScript_ReadVar(ctx);

    for (v3 = 0; v3 < 3; v3++) {
        v1[v3] = FrontierScript_ReadVar(ctx);
    }

    ov80_02239BF0(v0->frontier, v2, 1, v1, 3);
    Heap_Free(v1);
}

BOOL FrtCmd_083(FrontierScriptContext *ctx) {
    u16 v0 = FrontierScript_ReadVar(ctx);
    ov80_02239BF0(ctx->frontierSystem->frontier, v0, 0, 0L, 0);

    return 0;
}
