#include "global.h"
#include "overlay_80_field_frontier_dto.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "constants/heap.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BagCursor;

struct BattleFrontier;

struct BgConfig;

struct FieldFrontierDTO;

struct FrontierAnimCmd;

struct FrontierMenuManager;

struct FrontierScriptContext;

struct FrontierScriptManager;

struct JournalEntry;

struct ListMenu2D;

struct MsgData;

struct NamingScreenArgs;

struct Options;

struct SaveData;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct UnkStruct_ov63_0222D77C_t;

struct Window;

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

typedef struct String String;

typedef struct MsgData MsgData;

typedef struct ListMenu2D ListMenu2D;

typedef struct SaveData SaveData;

typedef struct SysTask SysTask;

typedef struct SysTaskManager SysTaskManager;

typedef void (*SysTaskFunc)(SysTask *, void *);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

struct SysTask {
    SysTaskManager *manager;
    SysTask *prevTask;
    SysTask *nextTask;
    u32 priority;
    void *param;
    SysTaskFunc callback;
    enum TaskState state;
};

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

typedef struct MessageFormat MessageFormat;

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
    MsgData *msgLoader;
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
    MsgData *msgLoader;
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

typedef struct Options Options;

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

enum NamingScreenType {
    NAMING_SCREEN_TYPE_PLAYER,
    NAMING_SCREEN_TYPE_POKEMON,
    NAMING_SCREEN_TYPE_BOX,
    NAMING_SCREEN_TYPE_RIVAL,
    NAMING_SCREEN_TYPE_UNK4,
    NAMING_SCREEN_TYPE_GROUP,
    NAMING_SCREEN_TYPE_SHAYMIN_TABLET,
    NAMING_SCREEN_TYPE_PAL_PAD,
};

typedef struct NamingScreenArgs NamingScreenArgs;

extern const OverlayManagerTemplate gOverlayTemplate_NamingScreen;

NamingScreenArgs *NamingScreen_CreateArgs(enum HeapID heapID, int, int param, int maxLen, Options *options, void *);

typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;

typedef struct BagCursor BagCursor;

typedef struct JournalEntry JournalEntry;

typedef struct FrontierAnimCmd FrontierAnimCmd;

typedef void (*BattleFrontierSubAppCallback)(void *);

FieldFrontierDTO *Frontier_GetLaunchArgs(void *a0);

void Frontier_LaunchApplication(BattleFrontier *frontier, const OverlayManagerTemplate *appTemplate, void *appArgs, BOOL freeArgsAfter, BattleFrontierSubAppCallback finishCallback);

void ov80_0222CB94(void *namingScreenArgs);

void ov80_0222C7B0(s16 localID, UnkStruct_ov104_0223C634 *object, const FrontierAnimCmd *animCmd, UnkStruct_ov63_0222D77C *param3, u8 *movementCount, enum HeapID heapID);

BOOL FrtCmd_052(FrontierScriptContext *ctx, s32 arg1);

BOOL FrtCmd_052(FrontierScriptContext *ctx, s32 arg1) {
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);
    void *namingScreenArgs = NamingScreen_CreateArgs(HEAP_ID_FIELD2, NAMING_SCREEN_TYPE_PLAYER, 0, 8, (void *)fieldData->options, NAMING_SCREEN_TYPE_PLAYER);

    Frontier_LaunchApplication(ctx->frontierSystem->frontier, &gOverlayTemplate_NamingScreen, namingScreenArgs, 0, ov80_0222CB94);

    return 1;
}
