#include "global.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"

struct BattleFrontier;

struct BgConfig;

struct FrontierMenuManager;

struct FrontierParticleSystem;

struct FrontierScriptContext;

struct FrontierScriptManager;

struct G3DPipelineBuffers;

struct ManagedSprite;

struct ListMenu2D;

struct MsgData;

struct PaletteData;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct UnkStruct_ov63_0222AE60_t;

struct UnkStruct_ov63_0222BB38_t;

struct UnkStruct_ov63_0222BCE8_t;

struct UnkStruct_ov63_0222BE18_t;

struct UnkStruct_ov63_0222BEC0_t;

struct UnkStruct_ov63_0222D1C0_t;

struct UnkStruct_ov63_0222D77C_t;

struct UnkStruct_ov63_0222D848_t;

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

typedef struct NARC {
    FSFile file;
    u32 fatbStart;
    u32 fimgStart;
    u16 numFiles;
} NARC;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

typedef struct MsgData MsgData;

typedef struct ListMenu2D ListMenu2D;

typedef struct PaletteData PaletteData;

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

typedef struct ManagedSprite ManagedSprite;

typedef struct SpriteSystem SpriteSystem;

typedef struct SpriteManager SpriteManager;

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

u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);

typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;

typedef struct UnkStruct_ov63_0222BEC0_t UnkStruct_ov63_0222BEC0;

typedef struct UnkStruct_ov63_0222AE60_t UnkStruct_ov63_0222AE60;

typedef struct UnkStruct_ov63_0222BB38_t UnkStruct_ov63_0222BB38;

typedef struct {
    const UnkStruct_ov63_0222BEC0 *unk_00;
    UnkStruct_ov63_0222BB38 *unk_04;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
    u16 unk_10;
    u8 unk_12;
    u8 unk_13;
} UnkStruct_ov63_0222CE44;

typedef struct {
    UnkStruct_ov63_0222AE60 *unk_00;
    UnkStruct_ov63_0222CE44 *unk_04;
    u16 unk_08;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov63_0222CD2C;

typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;

typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov63_0222D160;

typedef struct UnkStruct_ov63_0222D1C0_t UnkStruct_ov63_0222D1C0;

typedef struct UnkStruct_ov63_0222D848_t UnkStruct_ov63_0222D848;

typedef struct FrontierParticleSystem FrontierParticleSystem;

typedef struct {
    SysTask *unk_00;
    s16 unk_04;
    s16 unk_06;
    s8 unk_08;
    s8 unk_09;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_0223319C;

typedef struct {
    SysTask *unk_00;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    s16 unk_08;
    s8 unk_0A;
    u8 unk_0B;
} UnkStruct_ov104_022331E8;

typedef struct {
    ManagedSprite *unk_00[8];
    u16 unk_20[8];
    u32 unk_30;
    u16 unk_34[8];
} UnkStruct_ov104_0223D3B0_1;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

typedef struct FrontierGraphics {
    BgConfig *bgConfig;
    PaletteData *plttData;
    BattleFrontier *frontier;
    G3DPipelineBuffers *g3dPipeline;
    FrontierParticleSystem *particleSys;
    UnkStruct_ov63_0222BE18 *unk_14;
    UnkStruct_ov63_0222BCE8 *unk_18;
    UnkStruct_ov63_0222D160 unk_1C;
    UnkStruct_ov63_0222CD2C *unk_20;
    UnkStruct_ov63_0222D1C0 *unk_24;
    UnkStruct_ov63_0222D1C0 *unk_28;
    UnkStruct_ov63_0222D848 *unk_2C;
    UnkStruct_ov63_0222D77C *unk_30;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteMan;
    UnkStruct_ov104_0223D3B0_1 unk_3C;
    ManagedSprite *unk_80[4];
    void *unk_90;
    SysTask *unk_94;
    SysTask *unk_98;
    SysTask *unk_9C;
    SysTask *dummyExecuteOnVBlank;
    UnkStruct_ov104_0223319C unk_A4;
    UnkStruct_ov104_022331E8 unk_B0;
    u16 unk_BC;
    u16 unk_BE;
    u8 unk_C0;
    u8 sceneID;
    u8 unk_C2;
    u8 pad_C3[1];
} FrontierGraphics;

void ov80_0223962C(FrontierGraphics *param0, u16 param1);

u32 FrontierScriptContext_ReadWord(FrontierScriptContext *ctx);

void ov80_02239AF8(SpriteSystem *param0, SpriteManager *param1, NARC *param2, PaletteData *param3, u16 param4);

FrontierGraphics *sub_0209680C(BattleFrontier *frontier);

BOOL FrtCmd_042(FrontierScriptContext *ctx);

BOOL FrtCmd_042(FrontierScriptContext *ctx) {
    FrontierScriptManager *v0 = ctx->frontierSystem;
    FrontierGraphics *v1 = sub_0209680C(v0->frontier);
    const u8 *currScriptPtr;
    s32 offset = FrontierScriptContext_ReadWord(ctx);
    u16 v4;
    NARC *narc;

    currScriptPtr = ctx->scriptPtr;
    ctx->scriptPtr = (u8 *)(ctx->scriptPtr + offset);
    narc = NARC_New((enum NarcId)184, v0->heapID);

    while (TRUE) {
        v4 = FrontierScript_ReadVar(ctx);

        if (v4 == 0xFD13) {
            break;
        }

        ov80_02239AF8(v1->spriteSystem, v1->spriteMan, narc, v1->plttData, v4);
        ov80_0223962C(v1, v4);
    }

    NARC_Delete(narc);
    ctx->scriptPtr = currScriptPtr;

    return FALSE;
}
