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
struct FrontierGraphics;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct GameRecords;
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
typedef struct FrontierGraphics FrontierGraphics;
FrontierGraphics *sub_0209680C(BattleFrontier *frontier);
typedef struct GameRecords GameRecords;
GameRecords *Save_GameStats_Get(SaveData *saveData);
void GameStats_Inc(GameRecords *records, int scoreID);
typedef struct JournalEntry JournalEntry;
u16 FrontierScriptContext_ReadHalfWord(FrontierScriptContext *ctx);
FieldFrontierDTO *Frontier_GetLaunchArgs(BattleFrontier *frontier);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
u32 GameStats_Add(GameRecords *records, int id, u32 toAdd);
void GameStats_AddScore(GameRecords *records, int scoreID);

BOOL ov80_0222E05C(FrontierScriptContext *ctx);
BOOL FrtCmd_072(FrontierScriptContext *ctx);
BOOL FrtCmd_073(FrontierScriptContext *ctx);
BOOL FrtCmd_074(FrontierScriptContext *ctx);

BOOL ov80_0222E05C(FrontierScriptContext *ctx) {
    (void)(sub_0209680C(ctx->frontierSystem->frontier));

    return ctx->data[0] == 1;
}

BOOL FrtCmd_072(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData;
    u16 scoreID = FrontierScriptContext_ReadHalfWord(ctx);

    fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    GameStats_Inc(Save_GameStats_Get(fieldData->saveData), scoreID);

    return 0;
}

BOOL FrtCmd_073(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData;
    u16 recordID = FrontierScriptContext_ReadHalfWord(ctx);
    u16 value = FrontierScript_ReadVar(ctx);

    fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    GameStats_Add(Save_GameStats_Get(fieldData->saveData), recordID, value);

    return 0;
}

BOOL FrtCmd_074(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData;
    u16 scoreID = FrontierScriptContext_ReadHalfWord(ctx);

    fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    GameStats_AddScore(Save_GameStats_Get(fieldData->saveData), scoreID);

    return 0;
}
