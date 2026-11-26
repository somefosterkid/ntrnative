#include "global.h"
#include "overlay_80_frontier_anim.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "sys_task.h"
#include "frontier_graphics.h"
#include "constants/heap.h"

struct BattleFrontier;

struct BgConfig;


struct FrontierGraphics;

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

typedef struct PaletteData PaletteData;

typedef struct SysTaskManager SysTaskManager;

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

typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;

void ov80_0223A00C(int facility, BgConfig *bgConfig, SpriteSystem *spriteSys, SpriteManager *spriteMan, PaletteData *plttData, u16 *isFinishedPtr, s16 x, s16 y);

typedef struct FrontierParticleSystem FrontierParticleSystem;

typedef struct G3DPipelineBuffers G3DPipelineBuffers;

void ov80_022398E4(FrontierGraphics *param0, s16 *param1, s16 *param2);

void FrontierScriptContext_Pause(FrontierScriptContext *ctx, FrontierShouldResumeScriptFunc shouldResume);

u16 FrontierScriptContext_ReadHalfWord(FrontierScriptContext *ctx);

FrontierGraphics *FrontierSystem_GetFrontierMap(FrontierScriptManager *scriptMan);


enum SoundScene {
    SOUND_SCENE_NONE = 0,
    SOUND_SCENE_TITLE_SCREEN,
    SOUND_SCENE_2,
    SOUND_SCENE_3,
    SOUND_SCENE_FIELD,
    SOUND_SCENE_BATTLE,
    SOUND_SCENE_CONTEST,
    SOUND_SCENE_7,
    SOUND_SCENE_8,
    SOUND_SCENE_9,
    SOUND_SCENE_10,
    SOUND_SCENE_11,
    SOUND_SCENE_12,
    SOUND_SCENE_13,
    SOUND_SCENE_14,
    SOUND_SCENE_15,
    SOUND_SCENE_16,
    SOUND_SCENE_17,
    SOUND_SCENE_18,
    SOUND_SCENE_19,
    SOUND_SCENE_20,
    SOUND_SCENE_21,
    SOUND_SCENE_VILLA_MUSIC_BOX,
    SOUND_SCENE_23,
    SOUND_SCENE_24,
    SOUND_SCENE_25,
    SOUND_SCENE_26,
    SOUND_SCENE_27,
    SOUND_SCENE_28,
    SOUND_SCENE_29,
    SOUND_SCENE_30,
    SOUND_SCENE_31,
    SOUND_SCENE_32,
    SOUND_SCENE_33,
    SOUND_SCENE_34,
    SOUND_SCENE_35,
    SOUND_SCENE_36,
    SOUND_SCENE_37,
    SOUND_SCENE_38,
    SOUND_SCENE_39,
    SOUND_SCENE_40,
    SOUND_SCENE_41,
    SOUND_SCENE_42,
    SOUND_SCENE_43,
    SOUND_SCENE_44,
    SOUND_SCENE_45,
    SOUND_SCENE_46,
    SOUND_SCENE_47,
    SOUND_SCENE_48,
    SOUND_SCENE_49,
    SOUND_SCENE_50,
    SOUND_SCENE_MAX,

    SOUND_SCENE_SUB_BAG = SOUND_SCENE_MAX,
    SOUND_SCENE_SUB_52,
    SOUND_SCENE_SUB_53,
    SOUND_SCENE_SUB_54,
    SOUND_SCENE_SUB_55,
    SOUND_SCENE_SUB_56,
    SOUND_SCENE_SUB_57,
    SOUND_SCENE_SUB_58,
    SOUND_SCENE_SUB_59,
    SOUND_SCENE_SUB_60,
    SOUND_SCENE_SUB_61,
    SOUND_SCENE_SUB_62,
    SOUND_SCENE_SUB_63,
    SOUND_SCENE_SUB_64,
    SOUND_SCENE_SUB_65,
    SOUND_SCENE_SUB_66,
    SOUND_SCENE_SUB_67,
    SOUND_SCENE_SUB_68,
};

BOOL Sound_SetSceneAndPlayBGM(u8 scene, u16 bgmID, int unused);

BOOL ov80_0222E05C(FrontierScriptContext *ctx);

void ov80_0222C7B0(s16 localID, UnkStruct_ov104_0223C634 *object, const FrontierAnimCmd *animCmd, UnkStruct_ov63_0222D77C *param3, u8 *movementCount, enum HeapID heapID);

BOOL FrtCmd_071(FrontierScriptContext *ctx);

BOOL FrtCmd_071(FrontierScriptContext *ctx) {
    FrontierGraphics *graphics = FrontierSystem_GetFrontierMap(ctx->frontierSystem);
    u16 facility = FrontierScriptContext_ReadHalfWord(ctx);
    s16 v2, v3;

    ov80_022398E4(graphics, &v2, &v3);
    ov80_0223A00C(facility, graphics->bgConfig, graphics->spriteSystem, graphics->spriteMan, graphics->plttData, &ctx->data[0], v2, v3);
    Sound_SetSceneAndPlayBGM(SOUND_SCENE_BATTLE, 1147, 1);
    FrontierScriptContext_Pause(ctx, ov80_0222E05C);

    return 1;
}
