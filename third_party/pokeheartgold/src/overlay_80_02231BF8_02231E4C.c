#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_80_02231BF8.h"
#include "pm_string.h"
#include "save.h"
#include "overlay_80_battle_castle.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "battle_castle_app_args.h"
#include "battle_castle_save.h"

struct BattleCastle;
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
void ov80_02232AEC(BattleCastle *battleCastle, BattleCastleAppArgs *args);
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
void ov80_02232F00(BattleCastle *castle);
void *Frontier_GetData(BattleFrontier *frontier);
void ov80_02232F08(BattleCastle *castle);

void ov80_02231E4C(BattleCastleAppArgs *args, BattleCastle *battleCastle);
BOOL FrtCmd_158(FrontierScriptContext *ctx);
BOOL FrtCmd_159(FrontierScriptContext *ctx);

void ov80_02231E4C(BattleCastleAppArgs *args, BattleCastle *battleCastle) {
    args->challengeType = battleCastle->challengeType;
    args->party = battleCastle->playersParty;
    args->opponentsParty = battleCastle->opponentsParty;
    args->battleCastle = battleCastle;
    args->partnersCP = battleCastle->partnersCP;

    for (int i = 0; i < 4; i++) {
        args->identityUnlocked[i] = battleCastle->appIdentityUnlocked[i];
        args->levelAdjustments[i] = battleCastle->levelAdjustments[i];
        args->statsUnlocked[i] = battleCastle->appStatsUnlocked[i];
        args->movesUnlocked[i] = battleCastle->appMovesUnlocked[i];
    }
}

void ov80_02231E94(void *data) {
    BattleCastleAppArgs *args = data;
    ov80_02232AEC(args->battleCastle, data);
    Heap_Free(data);
}

BOOL FrtCmd_158(FrontierScriptContext *ctx) {
    BattleCastle *castle = Frontier_GetData(ctx->frontierSystem->frontier);
    ov80_02232F00(castle);

    return 0;
}

BOOL FrtCmd_159(FrontierScriptContext *ctx) {
    BattleCastle *castle = Frontier_GetData(ctx->frontierSystem->frontier);
    ov80_02232F08(castle);

    return 0;
}
