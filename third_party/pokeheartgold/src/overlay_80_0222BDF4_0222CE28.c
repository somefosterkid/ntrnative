#include "global.h"

#include "frontier_launch.h"

#include "constants/heap.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "options.h"
#include "overlay_42.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "system.h"
#include "overlay_80_field_frontier_dto.h"

struct BattleFrontier;


struct FrontierMenuManager;

struct FrontierScriptContext;

struct FrontierScriptManager;

struct JournalEntry;

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

u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);


typedef struct JournalEntry JournalEntry;


BOOL FrtCmd_114(FrontierScriptContext *ctx);

void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);
BOOL ov80_0222CE80(FrontierScriptContext *ctx);
enum SaveResult {
    SAVE_RESULT_PROCEED = 0,
    SAVE_RESULT_PROCEED_FINAL,
    SAVE_RESULT_OK,
    SAVE_RESULT_CORRUPT,
};
void *WaitingIcon_New(Window *window, u32 baseTile);
BOOL FrtCmd_115(FrontierScriptContext *ctx);
BOOL ov80_0222CE80(FrontierScriptContext *ctx);
BOOL FrtCmd_116(FrontierScriptContext *ctx);
BOOL FrtCmd_117(FrontierScriptContext *ctx);
BOOL FrtCmd_119(FrontierScriptContext *ctx);

BOOL FrtCmd_114(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    u16 *unused = FrontierScript_ReadVarPtr(ctx);
    SaveGameNormal(fieldData->saveData);
    return FALSE;
}

BOOL FrtCmd_115(FrontierScriptContext *ctx) {
    FrontierScriptManager *v0 = ctx->frontierSystem;
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    u16 *v2 = FrontierScript_ReadVarPtr(ctx);

    sub_0201A728(1 << 2);
    sub_0201A748(v0->heapID);
    Save_PrepareForAsyncWrite(fieldData->saveData, 2);

    FrontierScriptContext_Pause(ctx, ov80_0222CE80);
    return 1;
}

BOOL ov80_0222CE80(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    int saveResult = Save_WriteFileAsync(fieldData->saveData);

    if (saveResult == SAVE_RESULT_OK) {
        sub_0201A774();
        sub_0201A738(1 << 2);
        return 1;
    }

    if (saveResult == SAVE_RESULT_CORRUPT) {
        sub_0201A774();
        sub_0201A738(1 << 2);
        return 1;
    }

    return 0;
}

BOOL FrtCmd_116(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);

    Save_WipeExtraChunks(fieldData->saveData);
    return 0;
}

BOOL FrtCmd_117(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    u16 *destVar = FrontierScript_ReadVarPtr(ctx);

    *destVar = Save_CheckExtraChunksExist(fieldData->saveData);
    return 0;
}

BOOL FrtCmd_119(FrontierScriptContext *ctx) {
    FrontierScriptManager *scriptMan = ctx->frontierSystem;
    scriptMan->savingIcon = WaitingIcon_New(&scriptMan->msgWindow, (1024 - ((6 * 3) + ((2 * 2) * 3))));
    return 0;
}
