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
struct FieldFrontierDTO;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct JournalEntry;
struct Menu;
struct MessageLoader;
struct StringTemplate;
struct TrainerInfo;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
typedef struct MessageLoader MessageLoader;
typedef struct Menu Menu;
typedef struct TrainerInfo TrainerInfo;
typedef struct StringTemplate StringTemplate;
void BufferPlayersName(StringTemplate *template, u32 idx, const TrainerInfo *playerInfo);
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
typedef struct JournalEntry JournalEntry;
FieldFrontierDTO *Frontier_GetLaunchArgs(BattleFrontier *frontier);
TrainerInfo *Save_PlayerData_GetProfile(SaveData *saveData);
TrainerInfo *sub_02034818(int netId);
u16 sub_0203769C(void);
void BufferMoveName(StringTemplate *template, u32 idx, u32 type);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);

u16 ov80_0222CF6C(u32 number);
BOOL FrtCmd_123(FrontierScriptContext *ctx);
BOOL FrtCmd_124(FrontierScriptContext *ctx);
BOOL FrtCmd_125(FrontierScriptContext *ctx);

u16 ov80_0222CF6C(u32 number) {
    if (number / 10 == 0) {
        return 1;
    } else if (number / 100 == 0) {
        return 2;
    } else if (number / 1000 == 0) {
        return 3;
    } else if (number / 10000 == 0) {
        return 4;
    } else if (number / 100000 == 0) {
        return 5;
    } else if (number / 1000000 == 0) {
        return 6;
    } else if (number / 10000000 == 0) {
        return 7;
    } else if (number / 100000000 == 0) {
        return 8;
    }

    return 1;
}

BOOL FrtCmd_123(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    u8 templateArg = (*(ctx->scriptPtr++));

    BufferPlayersName(ctx->frontierSystem->strTemplate, templateArg, Save_PlayerData_GetProfile(fieldData->saveData));
    return 0;
}

BOOL FrtCmd_124(FrontierScriptContext *ctx) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    u8 templateArg = (*(ctx->scriptPtr++));

    BufferPlayersName(ctx->frontierSystem->strTemplate, templateArg, sub_02034818(sub_0203769C() ^ 1));
    return 0;
}

BOOL FrtCmd_125(FrontierScriptContext *ctx) {
    u8 templateArg = (*(ctx->scriptPtr++));
    u16 type = FrontierScript_ReadVar(ctx);

    BufferMoveName(ctx->frontierSystem->strTemplate, templateArg, type);
    return 0;
}
