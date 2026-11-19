#include "global.h"

#include "constants/heap.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "options.h"
#include "overlay_42.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "overlay_80_field_frontier_dto.h"

struct BattleFrontier;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct JournalEntry;
struct Menu;
struct MessageLoader;
struct StringTemplate;
struct WifiBattleTowerRecord;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
typedef struct MessageLoader MessageLoader;
typedef struct Menu Menu;
typedef struct StringTemplate StringTemplate;
void BufferTypeName(StringTemplate *template, u32 idx, u32 type);
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
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
void BufferRivalsName(StringTemplate *template, u32 idx, const SaveData *saveData);
typedef struct JournalEntry JournalEntry;
FieldFrontierDTO *Frontier_GetLaunchArgs(BattleFrontier *frontier);
u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
typedef struct WifiBattleTowerRecord WifiBattleTowerRecord;
u16 FrontierData_BattlePointAction(WifiBattleTowerRecord *record, u16 value, int func);
WifiBattleTowerRecord *Save_FrontierData_Get(SaveData *saveData);

BOOL FrtCmd_127(FrontierScriptContext *ctx);
BOOL FrtCmd_128(FrontierScriptContext *ctx);
BOOL FrtCmd_129(FrontierScriptContext *ctx);

BOOL FrtCmd_127(FrontierScriptContext *ctx) {
    u8 templateArg = (*(ctx->scriptPtr++));
    u16 type = FrontierScript_ReadVar(ctx);

    BufferTypeName(ctx->frontierSystem->strTemplate, templateArg, type);
    return 0;
}

BOOL FrtCmd_128(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    u8 templateArg = (*(ctx->scriptPtr++));

    BufferRivalsName(ctx->frontierSystem->strTemplate, templateArg, fieldData->saveData);
    return 0;
}

BOOL FrtCmd_129(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    u16 *destVar = FrontierScript_ReadVarPtr(ctx);

    *destVar = FrontierData_BattlePointAction(Save_FrontierData_Get(fieldData->saveData), 0, 0);
    return 0;
}
