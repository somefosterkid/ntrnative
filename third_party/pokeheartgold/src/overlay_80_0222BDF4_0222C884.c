#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "overlay_42.h"
#include "palette.h"
#include "pm_string.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02009D48.h"

struct BattleFrontier;
struct FrontierGraphics;
struct FrontierMenuManager;
struct FrontierParticleSystem;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct G3DPipelineBuffers;
struct Menu;
struct MessageLoader;
struct StringTemplate;
struct UnkStruct_ov63_0222BCE8_t;
struct UnkStruct_ov63_0222BE18_t;
struct UnkStruct_ov63_0222D1C0_t;
struct UnkStruct_ov63_0222D77C_t;
struct UnkStruct_ov63_0222D848_t;
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
u16 FrontierScript_ReadVar(FrontierScriptContext *ctx);
typedef struct UnkStruct_ov63_0222BE18_t UnkStruct_ov63_0222BE18;
typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;
typedef struct UnkStruct_ov63_0222BCE8_t UnkStruct_ov63_0222BCE8;
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
typedef struct FrontierGraphics FrontierGraphics;
struct FrontierGraphics {
    BgConfig *bgConfig;
    PaletteData *plttData;
    BattleFrontier *frontier;
    G3DPipelineBuffers *g3dPipeline;
    FrontierParticleSystem *particleSys;
    UnkStruct_ov63_0222BE18 *unk_14;
    UnkStruct_ov63_0222BCE8 *unk_18;
    UnkStruct_ov44_02232914 unk_1C;
    UnkStruct_ov42_02228EDC *unk_20;
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
};
void ov80_0223965C(FrontierGraphics *param0, u16 param1);
void ov80_02239B7C(SpriteManager *param0, u16 param1);
FrontierGraphics *sub_0209680C(BattleFrontier *frontier);
ManagedSprite *ov80_0223968C(FrontierGraphics *param0, u16 param1, u16 param2);
void ov80_02239708(FrontierGraphics *param0, u16 param1, int param2);
u32 FrontierScriptContext_ReadWord(FrontierScriptContext *ctx);
void ov80_022396D8(FrontierGraphics *param0, u16 param1);
ManagedSprite *ov80_02239700(FrontierGraphics *param0, u16 param1);

BOOL FrtCmd_043(FrontierScriptContext *ctx);
BOOL FrtCmd_044(FrontierScriptContext *ctx);
BOOL FrtCmd_045(FrontierScriptContext *ctx);
BOOL FrtCmd_046(FrontierScriptContext *ctx);
BOOL FrtCmd_047(FrontierScriptContext *ctx);
BOOL FrtCmd_048(FrontierScriptContext *ctx);

BOOL FrtCmd_043(FrontierScriptContext *ctx) {
    FrontierScriptManager *v0 = ctx->frontierSystem;
    FrontierGraphics *v1 = sub_0209680C(v0->frontier);
    u16 v2 = FrontierScript_ReadVar(ctx);

    ov80_02239B7C(v1->spriteMan, v2);
    ov80_0223965C(v1, v2);

    return 0;
}

BOOL FrtCmd_044(FrontierScriptContext *ctx) {
    FrontierScriptManager *v0 = ctx->frontierSystem;
    FrontierGraphics *v1 = sub_0209680C(v0->frontier);
    const u8 *currScriptPtr;
    s32 offset = FrontierScriptContext_ReadWord(ctx);
    u16 v4, x, y, draw, v8, v9;
    ManagedSprite *managedSprite;
    currScriptPtr = ctx->scriptPtr;

    ctx->scriptPtr = (u8 *)(ctx->scriptPtr + offset);

    while (1) {
        v4 = FrontierScript_ReadVar(ctx);

        if (v4 == 0xFD13) {
            break;
        }

        x = FrontierScript_ReadVar(ctx);
        y = FrontierScript_ReadVar(ctx);
        draw = FrontierScript_ReadVar(ctx);
        v8 = (*(ctx->scriptPtr++));
        v9 = (*(ctx->scriptPtr++));
        managedSprite = ov80_0223968C(v1, v9, v4);

        ManagedSprite_SetPositionXY(managedSprite, x, y);
        ManagedSprite_SetDrawFlag(managedSprite, draw);

        ov80_02239708(v1, v9, v8);
    }

    ctx->scriptPtr = currScriptPtr;
    return 0;
}

BOOL FrtCmd_045(FrontierScriptContext *ctx) {
    FrontierScriptManager *v0 = ctx->frontierSystem;
    u16 v1;
    FrontierGraphics *v2 = sub_0209680C(v0->frontier);

    v1 = FrontierScript_ReadVar(ctx);
    ov80_022396D8(v2, v1);

    return 0;
}

BOOL FrtCmd_046(FrontierScriptContext *ctx) {
    FrontierScriptManager *v0 = ctx->frontierSystem;
    FrontierGraphics *v1 = sub_0209680C(v0->frontier);
    u16 v2 = FrontierScript_ReadVar(ctx);
    u8 draw = (*(ctx->scriptPtr++));
    ManagedSprite *managedSprite = ov80_02239700(v1, v2);

    ((managedSprite != 0L) ? (void)0 : GF_AssertFail());
    ManagedSprite_SetDrawFlag(managedSprite, draw);

    return 0;
}

BOOL FrtCmd_047(FrontierScriptContext *ctx) {
    FrontierScriptManager *v0 = ctx->frontierSystem;
    FrontierGraphics *v1 = sub_0209680C(v0->frontier);
    u16 v2 = FrontierScript_ReadVar(ctx);
    ManagedSprite *managedSprite = ov80_02239700(v1, v2);

    ((managedSprite != 0L) ? (void)0 : GF_AssertFail());
    thunk_Sprite_SetFlipMode(managedSprite->sprite, 1);

    return 0;
}

BOOL FrtCmd_048(FrontierScriptContext *ctx) {
    ManagedSprite *managedSprite;
    FrontierScriptManager *v1 = ctx->frontierSystem;
    FrontierGraphics *v2 = sub_0209680C(v1->frontier);
    u16 v3 = FrontierScript_ReadVar(ctx);
    u16 v4 = FrontierScript_ReadVar(ctx);

    managedSprite = ov80_02239700(v2, v3);
    ((managedSprite != 0L) ? (void)0 : GF_AssertFail());

    if (v4 == 0) {
        thunk_Sprite_SetOamMode(managedSprite->sprite, GX_OAM_MODE_NORMAL);
    } else {
        thunk_Sprite_SetOamMode(managedSprite->sprite, GX_OAM_MODE_XLU);
    }

    return 0;
}
