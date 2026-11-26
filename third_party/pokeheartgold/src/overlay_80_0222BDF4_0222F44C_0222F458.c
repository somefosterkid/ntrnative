#include "global.h"

#include "frontier_launch.h"
#include "overlay_80_field_frontier_dto.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "msgdata.h"

struct BagCursor;

struct BattleFrontier;

struct BgConfig;

struct FrontierMenuManager;

struct FrontierMessageOptions;

struct FrontierScriptContext;

struct FrontierScriptManager;

struct JournalEntry;

struct ListMenu2D;

struct MessageBank;

struct Options;

struct SaveData;

struct String;

struct MessageFormat;

typedef struct BattleFrontier BattleFrontier;

typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

typedef struct MessageBank MessageBank;

struct MsgData;

void DestroyMsgData(struct MsgData *loader);

typedef struct ListMenu2D ListMenu2D;

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
    struct MsgData *msgLoader;
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
    struct MsgData *msgLoader;
    u8 *script;
    MessageFormat *strTemplate;
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
    ListMenu2D *menu;
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

typedef struct FrontierMessageOptions FrontierMessageOptions;

void ov80_0222E268(FrontierScriptManager *scriptMan, const struct MsgData *msgLoader, u32 messageID, u8 canSpeedUp, FrontierMessageOptions *msgOptions);

typedef struct BagCursor BagCursor;

void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);

typedef struct JournalEntry JournalEntry;


Options *Save_PlayerData_GetOptionsAddr(SaveData *saveData);

void ov80_0222E3B8(FrontierScriptManager *scriptMan, u8 renderDelay, u16 sentenceType, u16 sentenceID, u16 word1, s16 word2, u8 canSpeedUp);

BOOL ov80_0222F4D0(FrontierScriptContext *ctx);

void ov80_0222F458(FrontierScriptContext *ctx, u16 *args, u32 bankID);

void ov80_0222F458(FrontierScriptContext *ctx, u16 *args, u32 bankID) {
    struct MsgData *msgLoader;
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);

    if (args[0] == 0xFFFF) {
        msgLoader = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, (enum NarcId)27, bankID, HEAP_ID_FIELD3);

        ov80_0222E268(ctx->frontierSystem, msgLoader, args[1], 1, 0L);
        DestroyMsgData(msgLoader);
    } else {
        u8 frameDelay = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(fieldData->saveData));
        ov80_0222E3B8(ctx->frontierSystem, frameDelay, args[0], args[1], args[2], args[3], 1);
    }

    FrontierScriptContext_Pause(ctx, ov80_0222F4D0);
}
