#include "global.h"

#include "constants/heap.h"

#include "frontier/frontier.h"

#include "bg_window.h"
#include "heap.h"
#include "overlay_80_0222F608.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"

struct BattleFactory;
struct BattleFrontier;
struct FrontierMenuManager;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct Menu;
struct MessageLoader;
struct StringTemplate;
typedef struct BattleFactoryAppArgs {
    SaveData *saveData;
    u8 challengeType;
    u8 isOpenLevel;
    u8 isExchangeMode;
    u8 padding_07;
    Party *personalParty;
    Party *receivableParty;
    u16 selectedIndices[6];
    void *battleFactory;
    u8 padding_20[4];
} BattleFactoryAppArgs;
typedef struct BattleFactory BattleFactory;
void ov80_02230460(BattleFactory *battleFactory, BattleFactoryAppArgs *args);
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
void ov80_022307F0(BattleFactory *factory);
void ov80_022308C4(BattleFactory *factory);
void ov80_022309F8(BattleFactory *factory);
void ov80_02230A60(BattleFactory *factory);

BOOL FrtCmd_099(FrontierScriptContext *ctx);
BOOL FrtCmd_100(FrontierScriptContext *ctx);
BOOL FrtCmd_101(FrontierScriptContext *ctx);
BOOL FrtCmd_102(FrontierScriptContext *ctx);

void ov80_0222F7CC(void *data) {
    BattleFactoryAppArgs *args = data;
    ov80_02230460(args->battleFactory, data);
    Heap_Free(data);
}

BOOL FrtCmd_099(FrontierScriptContext *ctx) {
    BattleFactory *factory = Frontier_GetData(ctx->frontierSystem->frontier);
    ov80_022307F0(factory);

    return 0;
}

BOOL FrtCmd_100(FrontierScriptContext *ctx) {
    BattleFactory *factory = Frontier_GetData(ctx->frontierSystem->frontier);
    ov80_022308C4(factory);

    return 0;
}

BOOL FrtCmd_101(FrontierScriptContext *ctx) {
    BattleFactory *factory = Frontier_GetData(ctx->frontierSystem->frontier);
    ov80_022309F8(factory);

    return 0;
}

BOOL FrtCmd_102(FrontierScriptContext *ctx) {
    BattleFactory *factory = Frontier_GetData(ctx->frontierSystem->frontier);
    ov80_02230A60(factory);

    return 0;
}
