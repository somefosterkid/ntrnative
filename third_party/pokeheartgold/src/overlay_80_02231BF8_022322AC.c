#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "pm_string.h"
#include "save.h"
#include "overlay_80_battle_castle.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "battle_castle_save.h"

struct BattleCastle;
struct BattleCastleSave;
struct BattleFrontier;
struct FieldBattleDTO;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct Menu;
struct MessageLoader;
struct Party;
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
void *Frontier_GetData(BattleFrontier *frontier);
BOOL ov80_02233280(BattleCastle *castle, u16 command, u16 arg);
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);
u16 FrontierScriptContext_ReadHalfWord(FrontierScriptContext *ctx);
BOOL ov80_02232318(FrontierScriptContext *ctx);

BOOL FrtCmd_161(FrontierScriptContext *ctx);
BOOL FrtCmd_162(FrontierScriptContext *ctx);
BOOL FrtCmd_163(FrontierScriptContext *ctx);

BOOL FrtCmd_161(FrontierScriptContext *ctx) {
    u16 *destVar = FrontierScript_ReadVarPtr(ctx);

    BattleCastle *battleCastle = Frontier_GetData(ctx->frontierSystem->frontier);
    *destVar = battleCastle->wonBattle;

    return 0;
}

BOOL FrtCmd_162(FrontierScriptContext *ctx) {
    u16 command = FrontierScript_ReadVar(ctx);
    u16 arg = FrontierScript_ReadVar(ctx);
    u16 *success = FrontierScript_ReadVarPtr(ctx);

    BattleCastle *castle = Frontier_GetData(ctx->frontierSystem->frontier);
    *success = ov80_02233280(castle, command, arg);

    return 1;
}

BOOL FrtCmd_163(FrontierScriptContext *ctx) {
    ctx->data[0] = FrontierScriptContext_ReadHalfWord(ctx);
    FrontierScriptContext_Pause(ctx, ov80_02232318);

    return 1;
}
