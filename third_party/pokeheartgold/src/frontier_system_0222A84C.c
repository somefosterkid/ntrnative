#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "pm_string.h"

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
StringTemplate *MessageFormat_New_Custom(u32 maxArgs, u32 maxLen, u32 heapID);
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
void FrontierSystem_deadstripped_0222AA3C(FrontierScriptManager *scriptMan, u16 scene, u32 *null1, void *null2);
void ov80_0222AA40(u8 **script, MessageLoader **msgLoader, int scene, enum HeapID heapID);
BOOL FrontierScriptContext_Run(FrontierScriptContext *ctx);
void ov80_02239C54(BattleFrontier *param0);
void ov80_0222AA0C(FrontierScriptManager *scriptMan, FrontierScriptContext *ctx);
void DestroyMsgData(MessageLoader *loader);
void MessageFormat_Delete(StringTemplate *template);
void FrontierScriptContext_Init(FrontierScriptContext *ctx, const FrontierScrCmdFunc *cmdTable, u32 cmdTableSize);
BOOL FrontierScriptContext_Start(FrontierScriptContext *ctx, const u8 *ptr);
extern const FrontierScrCmdFunc gFrontierCommandTable[];
extern const u32 ov80_0223B9CC;
BOOL ov80_0222A9DC(FrontierScriptManager *scriptMan, FrontierScriptContext *ctx);
void ov80_0222AB14(FrontierScriptContext *ctx, int offsetID);

FrontierScriptManager *FrontierSystem_Create(BattleFrontier *frontier, enum HeapID heapID, int scene);
BOOL FrontierSystem_Main(FrontierScriptManager *scriptMan);
void ov80_0222A920(FrontierScriptManager *scriptMan);
void FrontierSystem_AddTask(FrontierScriptManager *scriptMan, int scene, int offsetID);

FrontierScriptManager *FrontierSystem_Create(BattleFrontier *frontier, enum HeapID heapID, int scene) {
    FrontierScriptManager *scriptMan = Heap_Alloc(heapID, sizeof(FrontierScriptManager));
    MI_CpuClear8(scriptMan, sizeof(FrontierScriptManager));

    scriptMan->frontier = frontier;
    scriptMan->heapID = heapID;
    scriptMan->scene = scene;

    ov80_0222AA40(&scriptMan->script, &scriptMan->msgLoader, scene, heapID);

    scriptMan->strTemplate = MessageFormat_New_Custom(8, 64, heapID);
    scriptMan->string = String_New(1024, heapID);
    scriptMan->fmtString = String_New(1024, heapID);

    FrontierSystem_deadstripped_0222AA3C(scriptMan, scene, 0L, 0L);

    return scriptMan;
}

BOOL FrontierSystem_Main(FrontierScriptManager *scriptMan) {
    if (scriptMan->numSetContexts == 0) {
        return 1;
    }

    if (scriptMan->unk_38 == 0) {
        for (int i = 0; i < ((int)(sizeof(scriptMan->ctx) / sizeof(*(scriptMan->ctx)))); i++) {
            FrontierScriptContext *ctx = scriptMan->ctx[i];

            if (ctx != 0L) {
                if (!FrontierScriptContext_Run(ctx)) {
                    ov80_0222AA0C(scriptMan, ctx);
                    scriptMan->ctx[i] = 0L;
                    scriptMan->numSetContexts--;
                }
            }
        }

        ov80_02239C54(scriptMan->frontier);
    }

    return scriptMan->numSetContexts == 0;
}

void ov80_0222A920(FrontierScriptManager *scriptMan) {
    ((scriptMan->numSetContexts == 0) ? (void)0 : GF_AssertFail());

    MessageFormat_Delete(scriptMan->strTemplate);
    String_Delete(scriptMan->string);
    String_Delete(scriptMan->fmtString);
    Heap_Free(scriptMan->script);
    DestroyMsgData(scriptMan->msgLoader);

    Heap_Free(scriptMan);
}

void FrontierSystem_AddTask(FrontierScriptManager *scriptMan, int scene, int offsetID) {
    FrontierScriptContext *ctx = Heap_Alloc(scriptMan->heapID, sizeof(FrontierScriptContext));
    MI_CpuClear8(ctx, sizeof(FrontierScriptContext));

    FrontierScriptContext_Init(ctx, gFrontierCommandTable, ov80_0223B9CC);

    ctx->frontierSystem = scriptMan;

    if (scene == 0xffff || scriptMan->scene == scene) {
        ctx->scripts = scriptMan->script;
        ctx->msgLoader = scriptMan->msgLoader;
    } else {
        ov80_0222AA40(&ctx->scripts, &ctx->msgLoader, scene, scriptMan->heapID);
    }

    FrontierScriptContext_Start(ctx, ctx->scripts);
    ov80_0222AB14(ctx, offsetID);
    ov80_0222A9DC(scriptMan, ctx);
}
