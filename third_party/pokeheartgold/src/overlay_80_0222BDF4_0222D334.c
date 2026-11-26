#include "global.h"
#include "overlay_80_frontier_anim.h"

#include "frontier_launch.h"
#include "sprite_resources_header.h"
#include "overlay_80_field_frontier_dto.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "pm_string.h"
#include "msgdata.h"
#include "overlay_45_unk_struct_ov66_02234798.h"
#include "bag_cursor.h"
#include "save_vars_flags.h"
#include "frontier_graphics.h"
#include "screen_scroll_manager.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "camera.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "particle_system.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"

struct BattleFrontier;
struct BgConfig;
struct CameraPositionHistory;
struct FieldFrontierDTO;
struct FrontierGraphics;
struct FrontierMenuManager;
struct FrontierParticleSystem;
struct FrontierScriptContext;
struct FrontierScriptManager;
struct G2dRenderer;
struct G3DPipelineBuffers;
struct JournalEntry;
struct ListMenu2D;
struct MsgData;
struct PaletteData;
struct PaletteFadeControl;
struct ParticleSystem;
struct SPLAlphaAnim;
struct SPLBehavior;
struct SPLChildResource;
struct SPLColorAnim;
struct SPLEmitter;
struct SPLEmitterState;
struct SPLParticle;
struct SPLResourceHeader;
struct SPLScaleAnim;
struct SPLTexAnim;
struct SaveData;
struct ScreenScrollManager;
struct SpriteList;
struct SpriteManager;
struct SpriteResourceCollection;
struct GF_2DGfxResObjList;
struct GF_2DGfxResHeader;
struct SpriteResourcesHeader;
struct SpriteResourcesHeaderList;
struct SpriteSystem;
struct String;
struct MessageFormat;
struct SysTask;
struct UnkStruct_ov104_0223EBD0_t;
struct UnkStruct_ov104_0223F174_t;
struct UnkStruct_ov63_0222D77C_t;
struct SaveVarsFlags;
struct Window;
typedef struct BattleFrontier BattleFrontier;
typedef struct {
    u16 unk_00[8];
} UnkStruct_ov104_0222E8C8;
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
typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;
typedef struct JournalEntry JournalEntry;
u16 FrontierScriptContext_ReadHalfWord(FrontierScriptContext *ctx);
SaveVarsFlags *Save_VarsFlags_Get(SaveData *saveData);
u16 *Save_VarsFlags_GetVarAddr(SaveVarsFlags *varsFlags, u16 varID);
void ov80_0222C7B0(s16 localID, UnkStruct_ov104_0223C634 *object, const FrontierAnimCmd *animCmd, UnkStruct_ov63_0222D77C *param3, u8 *movementCount, enum HeapID heapID);
u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
typedef struct UnkStruct_ov104_0223EBD0_t UnkStruct_ov104_0223EBD0;
typedef struct {
    ScreenScrollManager *screenScrollMgr;
    SysTask *unk_04;
    u32 unk_08;
} UnkStruct_ov104_0223F094;
typedef struct UnkStruct_ov104_0223F174_t UnkStruct_ov104_0223F174;
void ov80_0223AC24(int param0, u32 param1, u32 param2, BOOL *param3, u32 param4);
enum FadeType {
    FADE_TYPE_BRIGHTNESS_OUT = 0,
    FADE_TYPE_BRIGHTNESS_IN = 1,
    FADE_TYPE_DOWNWARD_OUT = 2,
    FADE_TYPE_DOWNWARD_IN = 3,
    FADE_TYPE_UPWARD_OUT = 4,
    FADE_TYPE_UNK_5 = 5,
    FADE_TYPE_UNK_6 = 6,
    FADE_TYPE_UNK_7 = 7,
    FADE_TYPE_UNK_8 = 8,
    FADE_TYPE_UNK_9 = 9,
    FADE_TYPE_UNK_10 = 10,
    FADE_TYPE_UNK_11 = 11,
    FADE_TYPE_UNK_12 = 12,
    FADE_TYPE_UNK_13 = 13,
    FADE_TYPE_UNK_14 = 14,
    FADE_TYPE_UNK_15 = 15,
    FADE_TYPE_CIRCLE_OUT = 16,
    FADE_TYPE_CIRCLE_IN = 17,
    FADE_TYPE_TOP_HALF_CIRCLE_OUT = 18,
    FADE_TYPE_TOP_HALF_CIRCLE_IN = 19,
    FADE_TYPE_UNK_20 = 20,
    FADE_TYPE_UNK_21 = 21,
    FADE_TYPE_UNK_22 = 22,
    FADE_TYPE_UNK_23 = 23,
    FADE_TYPE_UNK_24 = 24,
    FADE_TYPE_UNK_25 = 25,
    FADE_TYPE_UNK_26 = 26,
    FADE_TYPE_UNK_27 = 27,
    FADE_TYPE_UNK_28 = 28,
    FADE_TYPE_UNK_29 = 29,
    FADE_TYPE_UNK_30 = 30,
    FADE_TYPE_UNK_31 = 31,
    FADE_TYPE_UNK_32 = 32,
    FADE_TYPE_UNK_33 = 33,
    FADE_TYPE_UNK_34 = 34,
    FADE_TYPE_UNK_35 = 35,
    FADE_TYPE_UNK_36 = 36,
    FADE_TYPE_UNK_37 = 37,
    FADE_TYPE_UNK_38 = 38,
    FADE_TYPE_UNK_39 = 39,
    FADE_TYPE_CLAMP_OUT = 40,
    FADE_TYPE_CLAMP_IN = 41,
    FADE_TYPE_MAX = 42,
};
enum FadeMode {
    FADE_BOTH_SCREENS = 0,
    FADE_MAIN_THEN_SUB,
    FADE_SUB_THEN_MAIN,
    FADE_MAIN_ONLY,
    FADE_SUB_ONLY,
};
void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);
BOOL IsPaletteFadeFinished(void);
void ResetVisibleHardwareWindows(enum GFScreen screen);
typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    int unk_0C;
} UnkStruct_ov104_022313FC_sub1;
typedef struct {
    UnkStruct_ov104_022313FC_sub1 unk_00[96];
    UnkStruct_ov104_0223F174 *unk_600;
    int unk_604;
} UnkStruct_ov104_022313FC;
typedef struct {
    FrontierGraphics *unk_00;
    int unk_04;
    int unk_08;
    BOOL unk_0C;
    BOOL unk_10;
    void *unk_14;
    UnkStruct_ov104_0223F094 unk_18;
    UnkStruct_ov104_022313FC *unk_24;
    Window *unk_28;
    UnkStruct_ov104_0223EBD0 *unk_2C;
} UnkStruct_ov104_02231148;
BOOL ov104_022311BC(UnkStruct_ov104_02231148 *param0);
BOOL ov80_0222D404(UnkStruct_ov104_02231148 *param0);
BOOL ov104_02231720(UnkStruct_ov104_02231148 *param0);
BOOL ov104_02231864(UnkStruct_ov104_02231148 *param0);
BOOL ov104_022319CC(UnkStruct_ov104_02231148 *param0);
extern BOOL (*const ov80_0223B9EC[6])(UnkStruct_ov104_02231148 *);
void BG_ClearCharDataRange(u8 bgLayer, u32 size, u32 offset, enum HeapID heapID);
void BgClearTilemapBufferAndCommit(BgConfig *bgConfig, u8 bgLayer);
void AddWindowParameterized(BgConfig *bgConfig, Window *window, u8 bgLayer, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palette, u16 baseTile);
void RemoveWindow(Window *window);
void ScheduleWindowCopyToVram(Window *window);
void ClearWindowTilemapAndCopyToVram(Window *window);
void FillWindowPixelBuffer(Window *window, u8 val);
typedef struct PaletteFadeControl {
    u16 unfadedMask;
    u16 wait : 6;
    u16 cur : 5;
    u16 end : 5;
    u16 target : 15;
    u16 sign : 1;
    u16 step : 4;
    u16 waitStep : 6;
    u16 unused : 6;
} PaletteFadeControl;
void PaletteData_FillPaletteInBuffer(PaletteData *paletteData, enum PaletteBufferId bufferID, enum PaletteSelector selector, u16 fillVal, u16 start, u16 end);
typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;
typedef struct SpriteSystem {
    enum HeapID heapID;
    int spriteManagerCount;
    BOOL inUse;
    NNSG2dCellTransferState *cellTransferStates;
    G2dRenderer renderer;
} SpriteSystem;
typedef struct SpriteManager {
    SpriteList *sprites;
    SpriteResourcesHeaderList *resourceHeaders;
    GF_2DGfxResHeader *resourcePaths;
    SpriteResourceCollection *ownedResources[GF_GFX_RES_TYPE_MAX];
    GF_2DGfxResObjList *unownedResources[GF_GFX_RES_TYPE_MAX];
    int loadedResourceCount[GF_GFX_RES_TYPE_MAX];
    int numResourceTypes;
} SpriteManager;
typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;
typedef struct SPLBehavior {
    void (*applyFunc)(const void *, struct SPLParticle *, VecFx32 *, struct SPLEmitter *);
    const void *object;
} SPLBehavior;
typedef union SPLResourceFlags {
    u32 all;
    struct {
        u32 emissionType : 4;
        u32 drawType : 2;
        u32 circleAxis : 2;
        u32 hasScaleAnim : 1;
        u32 hasColorAnim : 1;
        u32 hasAlphaAnim : 1;
        u32 hasTexAnim : 1;
        u32 hasRotation : 1;
        u32 randomInitAngle : 1;

        u32 selfMaintaining : 1;
        u32 followEmitter : 1;
        u32 hasChildResource : 1;
        u32 polygonRotAxis : 2;
        u32 polygonReferencePlane : 1;
        u32 randomizeLoopedAnim : 1;
        u32 drawChildrenFirst : 1;
        u32 hideParent : 1;
        u32 useViewSpace : 1;
        u32 hasGravityBehavior : 1;
        u32 hasRandomBehavior : 1;
        u32 hasMagnetBehavior : 1;
        u32 hasSpinBehavior : 1;
        u32 hasCollisionPlaneBehavior : 1;
        u32 hasConvergenceBehavior : 1;
        u32 hasFixedPolygonID : 1;
        u32 childHasFixedPolygonID : 1;
    };
} SPLResourceFlags;
typedef union SPLChildResourceFlags {
    u16 all;
    struct {
        u16 usesBehaviors : 1;
        u16 hasScaleAnim : 1;
        u16 hasAlphaAnim : 1;
        u16 rotationType : 2;
        u16 followEmitter : 1;
        u16 useChildColor : 1;
        u16 drawType : 2;
        u16 polygonRotAxis : 2;
        u16 polygonReferencePlane : 1;
        u16 : 4;
    };
} SPLChildResourceFlags;
typedef union SPLCurveInOut {
    u16 all;
    struct {
        u16 in : 8;
        u16 out : 8;
    };
} SPLCurveInOut;
typedef union SPLCurveInPeakOut {
    u32 all;
    struct {
        u32 in : 8;
        u32 peak : 8;
        u32 out : 8;
        u32 : 8;
    };
} SPLCurveInPeakOut;
typedef struct SPLResourceHeader {
    SPLResourceFlags flags;
    VecFx32 emitterBasePos;
    fx32 emissionCount;
    fx32 radius;
    fx32 length;
    VecFx16 axis;
    GXRgb color;
    fx32 initVelPosAmplifier;
    fx32 initVelAxisAmplifier;
    fx32 baseScale;
    fx16 aspectRatio;
    u16 startDelay;
    s16 minRotation;
    s16 maxRotation;
    u16 initAngle;
    u16 reserved;
    u16 emitterLifeTime;
    u16 particleLifeTime;

    struct {
        u32 baseScale : 8;
        u32 lifeTime : 8;
        u32 initVel : 8;
        u32 : 8;
    } randomAttenuation;

    struct {
        u32 emissionInterval : 8;
        u32 baseAlpha : 8;
        u32 airResistance : 8;
        u32 textureIndex : 8;
        u32 loopFrames : 8;
        u32 dbbScale : 16;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 scaleAnimDir : 3;
        u32 dpolFaceEmitter : 1;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 : 30;
    } misc;
    fx16 polygonX;
    fx16 polygonY;
    struct {
        u32 flags : 8;
        u32 : 24;
    } userData;
} SPLResourceHeader;
typedef struct SPLScaleAnim {
    fx16 start;
    fx16 mid;
    fx16 end;
    SPLCurveInOut curve;
    struct {
        u16 loop : 1;
        u16 : 15;
    } flags;
    u16 padding;
} SPLScaleAnim;
typedef struct SPLColorAnim {
    GXRgb start;
    GXRgb end;
    SPLCurveInPeakOut curve;
    struct {
        u16 randomStartColor : 1;
        u16 loop : 1;
        u16 interpolate : 1;
        u16 : 13;
    } flags;
    u16 padding;
} SPLColorAnim;
typedef struct SPLAlphaAnim {
    union {
        u16 all;
        struct {
            u16 start : 5;
            u16 mid : 5;
            u16 end : 5;
            u16 : 1;
        };
    } alpha;
    struct {
        u16 randomRange : 8;
        u16 loop : 1;
        u16 : 7;
    } flags;
    SPLCurveInOut curve;
    u16 padding;
} SPLAlphaAnim;
typedef struct SPLTexAnim {
    u8 textures[8];
    struct {
        u32 frameCount : 8;
        u32 step : 8;
        u32 randomizeInit : 1;
        u32 loop : 1;
        u32 : 14;
    } param;
} SPLTexAnim;
typedef struct SPLChildResource {
    SPLChildResourceFlags flags;
    fx16 randomInitVelMag;
    fx16 endScale;
    u16 lifeTime;
    u8 velocityRatio;
    u8 scaleRatio;
    GXRgb color;
    struct {
        u32 emissionCount : 8;
        u32 emissionDelay : 8;
        u32 emissionInterval : 8;
        u32 texture : 8;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 dpolFaceEmitter : 1;
        u32 : 25;
    } misc;
} SPLChildResource;
typedef struct SPLEmitterState {
    union {
        u32 all;
        struct
        {
            u32 terminate : 1;
            u32 emissionPaused : 1;
            u32 paused : 1;
            u32 renderingDisabled : 1;
            u32 started : 1;
            u32 : 27;
        };
    };
} SPLEmitterState;
typedef struct FrontierParticleSystem {
    ParticleSystem *particleSys[8];
    u16 heapID;
} FrontierParticleSystem;
typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;
UnkStruct_ov104_0223EBD0 *ov80_0223AF30(enum HeapID heapID);
void ov80_0223AF60(UnkStruct_ov104_0223EBD0 *param0);
void ov80_0223AF80(UnkStruct_ov104_0223EBD0 *param0, u8 param1, u8 param2, Window *param3, u8 param4);
BOOL ov80_0223AFC4(UnkStruct_ov104_0223EBD0 *param0);
void sub_0200FBF4(enum GFScreen screen, u16 color);
BOOL ov80_0222D390(UnkStruct_ov104_02231148 *param0);
BOOL ov80_0223B1D4(UnkStruct_ov104_0223EBD0 *param0);

BOOL FrtCmd_061(FrontierScriptContext *ctx);
BOOL FrtCmd_062(FrontierScriptContext *ctx);
BOOL ov80_0222D390(UnkStruct_ov104_02231148 *param0);
BOOL ov80_0222D404(UnkStruct_ov104_02231148 *param0);
BOOL ov80_0222D520(UnkStruct_ov104_02231148 *param0);

BOOL FrtCmd_061(FrontierScriptContext *ctx) {
    u16 destVarID = FrontierScriptContext_ReadHalfWord(ctx);
    u16 value = FrontierScript_ReadVar(ctx);
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);

    u16 *destVar = Save_VarsFlags_GetVarAddr(Save_VarsFlags_Get(fieldData->saveData), destVarID);
    *destVar = value;

    return 0;
}

BOOL FrtCmd_062(FrontierScriptContext *ctx) {
    u16 srcVarID = FrontierScriptContext_ReadHalfWord(ctx);
    u16 *dest = FrontierScript_ReadVarPtr(ctx);
    FieldFrontierDTO *fieldData = Frontier_GetLaunchArgs(ctx->frontierSystem->frontier);

    u16 *srcVar = Save_VarsFlags_GetVarAddr(Save_VarsFlags_Get(fieldData->saveData), srcVarID);
    *dest = *srcVar;

    return 0;
}

BOOL ov80_0222D390(UnkStruct_ov104_02231148 *param0) {
    switch (param0->unk_04) {
    case 0:
        ov80_0223AC24(1, 16, -16, &param0->unk_0C, 2);
        param0->unk_04++;
        break;
    case 1:
        if (param0->unk_0C == 0) {
            break;
        }

        ResetVisibleHardwareWindows(SCREEN_MAIN);
        ResetVisibleHardwareWindows(SCREEN_SUB);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_UNK_32, FADE_TYPE_UNK_32, (((0) << 10) | ((0) << 5) | (0)), 12, 1, HEAP_ID_FIELD2);
        param0->unk_04++;
        break;
    default:
        if (IsPaletteFadeFinished() == 1) {
            return 0;
        }
        break;
    }

    return 1;
}

BOOL ov80_0222D404(UnkStruct_ov104_02231148 *param0) {
    switch (param0->unk_04) {
    case 0:
        ov80_0223AC24(1, 16, -16, &param0->unk_0C, 2);
        param0->unk_04++;
        break;
    case 1:
        if (param0->unk_0C == 0) {
            break;
        }

        param0->unk_28 = AllocWindows(HEAP_ID_FIELD2, 1);

        AddWindowParameterized(param0->unk_00->bgConfig, param0->unk_28, 1, 0, 0, 32, 32, 0, 0);
        PaletteData_FillPaletteInBuffer(param0->unk_00->plttData, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, 0x0, 0, 16);
        FillWindowPixelBuffer(param0->unk_28, 0);
        ScheduleWindowCopyToVram(param0->unk_28);

        param0->unk_2C = ov80_0223AF30(HEAP_ID_FIELD2);
        param0->unk_04++;
    case 2:
        ov80_0223AF80(param0->unk_2C, 1, 1, param0->unk_28, 15);
        param0->unk_04++;
        break;
    case 3:
        BOOL v0 = ov80_0223AFC4(param0->unk_2C);

        ScheduleWindowCopyToVram(param0->unk_28);

        if (v0) {
            param0->unk_04++;
        }
        break;

    default:
        if (IsPaletteFadeFinished() == 1) {
            ov80_0223AF60(param0->unk_2C);

            ClearWindowTilemapAndCopyToVram(param0->unk_28);
            RemoveWindow(param0->unk_28);
            WindowArray_Delete(param0->unk_28, 1);

            sub_0200FBF4(SCREEN_MAIN, (((0) << 10) | ((0) << 5) | (0)));
            sub_0200FBF4(SCREEN_SUB, (((0) << 10) | ((0) << 5) | (0)));

            BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_FIELD2);
            BgClearTilemapBufferAndCommit(param0->unk_00->bgConfig, 1);

            return 0;
        }
        break;
    }

    return 1;
}

BOOL ov80_0222D520(UnkStruct_ov104_02231148 *param0) {
    switch (param0->unk_04) {
    case 0:
        ov80_0223AC24(1, 16, -16, &param0->unk_0C, 2);
        param0->unk_04++;
        break;
    case 1:
        if (param0->unk_0C == 0) {
            break;
        }

        param0->unk_28 = AllocWindows(HEAP_ID_FIELD2, 1);

        AddWindowParameterized(param0->unk_00->bgConfig, param0->unk_28, 1, 0, 0, 32, 32, 0, 0);
        PaletteData_FillPaletteInBuffer(param0->unk_00->plttData, PLTTBUF_MAIN_BG, PLTTSEL_BOTH, 0x0, 0, 16);
        FillWindowPixelBuffer(param0->unk_28, 0);
        ScheduleWindowCopyToVram(param0->unk_28);

        param0->unk_2C = ov80_0223AF30(HEAP_ID_FIELD2);
        param0->unk_04++;
    case 2:
        ov80_0223AF80(param0->unk_2C, 1, 1, param0->unk_28, 15);
        param0->unk_04++;
        break;
    case 3:
        BOOL v0 = ov80_0223B1D4(param0->unk_2C);

        ScheduleWindowCopyToVram(param0->unk_28);

        if (v0) {
            param0->unk_04++;
        }
        break;

    default:
        if (IsPaletteFadeFinished() == 1) {
            ov80_0223AF60(param0->unk_2C);

            ClearWindowTilemapAndCopyToVram(param0->unk_28);
            RemoveWindow(param0->unk_28);
            WindowArray_Delete(param0->unk_28, 1);

            sub_0200FBF4(SCREEN_MAIN, (((0) << 10) | ((0) << 5) | (0)));
            sub_0200FBF4(SCREEN_SUB, (((0) << 10) | ((0) << 5) | (0)));

            BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 32, 0, HEAP_ID_FIELD2);
            BgClearTilemapBufferAndCommit(param0->unk_00->bgConfig, 1);

            return 0;
        }
        break;
    }

    return 1;
}
