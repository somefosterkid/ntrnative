#include "global.h"
#include "overlay_80_frontier_anim.h"
#include "sprite_resources_header.h"
#include "overlay_80_unk_struct_ov104_0223_c634.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "bg_window.h"
#include "struct_defs/resdat_narc_structs.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "camera.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "particle_system.h"
#include "frontier_graphics.h"
#include "screen_scroll_manager.h"

struct BattleFrontier;

struct CameraPerspective;

struct CameraPositionHistory;


struct FrontierGraphics;

struct FrontierParticleSystem;

struct G3DPipelineBuffers;

struct SelectedPaletteData;

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

struct ScreenScrollManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SysTask;

struct SysTaskQueue;

struct UnkStruct_ov104_0223EBD0_t;

struct UnkStruct_ov104_0223F174_t;

struct UnkStruct_ov63_0222D77C_t;

struct Window;

typedef struct BattleFrontier BattleFrontier;

void ToggleBgLayer(u8 bgLayer, u8 enable);

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct UnkStruct_ov63_0222D77C_t UnkStruct_ov63_0222D77C;

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


typedef struct UnkStruct_ov104_0223EBD0_t UnkStruct_ov104_0223EBD0;

typedef struct {
    ScreenScrollManager *screenScrollMgr;
    SysTask *unk_04;
    u32 unk_08;
} UnkStruct_ov104_0223F094;

typedef struct UnkStruct_ov104_0223F174_t UnkStruct_ov104_0223F174;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u32 unk_08;
    u32 unk_0C;
    u16 unk_10;
    u16 unk_12;
    u16 unk_14;
    u16 unk_16;
    u32 unk_18;
    u32 unk_1C;
} UnkStruct_ov104_0223F1B4;

void ov80_0223AC24(int param0, u32 param1, u32 param2, BOOL *param3, u32 param4);

UnkStruct_ov104_0223F174 *ov80_0223B544(void *param0, UnkStruct_ov104_0223F1B4 *param1, enum HeapID heapID);

void StartBrightnessTransition(u16 stepCount, s16 targetBrightness, s16 startBrightness, int planeMask, u32 screenSelect);

BOOL IsBrightnessTransitionActive(u32 screenSelect);

SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);

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

u32 ov80_0222D63C(s32 param0, s32 param1);

void ov80_0222C7B0(s16 localID, UnkStruct_ov104_0223C634 *object, const FrontierAnimCmd *animCmd, UnkStruct_ov63_0222D77C *param3, u8 *movementCount, enum HeapID heapID);

void ov80_0222D644(SysTask *param0, void *param1);

BOOL ov80_0222D968(UnkStruct_ov104_02231148 *param0);

s32 ov80_0223B424(void *, s32);
s32 ov80_0223B440(void *, u8, u8, u16, s32, s16, void *, s32, s32);
s32 ov80_0222DC14(void *arg0);

BOOL ov80_0222D968(UnkStruct_ov104_02231148 *param0) {
    UnkStruct_ov104_0223F1B4 v0;

    switch (param0->unk_04) {
    case 0:
        ov80_0223AC24(1, 16, -16, &param0->unk_0C, 2);
        param0->unk_04++;
        break;
    case 1:
        if (param0->unk_0C == 0) {
            break;
        }

        MI_CpuClear8(&v0, sizeof(UnkStruct_ov104_0223F1B4));

        param0->unk_10 = 0;
        param0->unk_24 = Heap_Alloc(HEAP_ID_101, sizeof(UnkStruct_ov104_022313FC));
        param0->unk_24->unk_604 = 1;
        param0->unk_24->unk_600 = ov80_0223B544(&reg_G2_BG2PA, &v0, HEAP_ID_101);

        int v1;

        for (v1 = 0; v1 < (192 / 2); v1++) {
            param0->unk_24->unk_00[v1].unk_00 = (v1 * 2);
            param0->unk_24->unk_00[v1].unk_02 = param0->unk_24->unk_00[v1].unk_00 + 2;
            param0->unk_24->unk_00[v1].unk_04 = ((((192 / 2) / 2) - v1) + 1) % 8;
            param0->unk_24->unk_00[v1].unk_06 = ((((192 / 2) / 2) - v1) + 1) / 4;
            param0->unk_24->unk_00[v1].unk_08 = 0;
            param0->unk_24->unk_00[v1].unk_0A = 0;
            param0->unk_24->unk_00[v1].unk_0C = ov80_0222D63C(0, 0);
        }

        StartBrightnessTransition(40, -16, 0, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, (1 << SCREEN_MAIN));
        SysTask_CreateOnMainQueue(ov80_0222D644, param0, 0x1000);

        param0->unk_04++;
        break;
    case 2:
        if (IsBrightnessTransitionActive(1 << SCREEN_MAIN) == 0) {
            break;
        }

        param0->unk_10 = 1;

        ToggleBgLayer(GF_BG_LYR_MAIN_3, 0);
        BgSetPosTextAndCommit(param0->unk_00->bgConfig, 3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0->unk_00->bgConfig, 3, BG_POS_OP_SET_Y, 0);

        param0->unk_04++;
        break;
    default:
        return 0;
    }

    return 1;
}

BOOL ov80_0222DAAC(UnkStruct_ov104_02231148 *param0);

BOOL ov80_0222DAAC(UnkStruct_ov104_02231148 *param0) {
    UnkStruct_ov104_0223F1B4 v0;

    switch (param0->unk_04) {
    case 0:
        ov80_0223AC24(1, 16, -16, &param0->unk_0C, 2);
        param0->unk_04++;
        break;
    case 1:
        if (param0->unk_0C == 0) {
            break;
        }

        MI_CpuClear8(&v0, sizeof(UnkStruct_ov104_0223F1B4));

        param0->unk_10 = 0;
        param0->unk_24 = Heap_Alloc(HEAP_ID_101, sizeof(UnkStruct_ov104_022313FC));
        param0->unk_24->unk_604 = 2;
        param0->unk_24->unk_600 = ov80_0223B544(&reg_G2_BG2PA, &v0, HEAP_ID_101);

        int v1;

        for (v1 = 0; v1 < (192 / 2); v1++) {
            param0->unk_24->unk_00[v1].unk_00 = (v1 * 2);
            param0->unk_24->unk_00[v1].unk_02 = param0->unk_24->unk_00[v1].unk_00 + 2;
            param0->unk_24->unk_00[v1].unk_04 = ((((192 / 2) / 2) - v1) / 8) + 1;

            if (v1 % 2) {
                param0->unk_24->unk_00[v1].unk_04 *= -1;
            }

            if (v1 < ((192 / 2) / 2)) {
                param0->unk_24->unk_00[v1].unk_06 = v1;
            } else {
                param0->unk_24->unk_00[v1].unk_06 = (192 / 2) - v1;
            }

            param0->unk_24->unk_00[v1].unk_08 = 0;
            param0->unk_24->unk_00[v1].unk_0A = 0;
            param0->unk_24->unk_00[v1].unk_0C = ov80_0222D63C(0, 0);
        }

        StartBrightnessTransition(40, -16, 0, GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ, (1 << SCREEN_MAIN));
        SysTask_CreateOnMainQueue(ov80_0222D644, param0, 0x1000);

        param0->unk_04++;
        break;
    case 2:
        if (IsBrightnessTransitionActive(1 << SCREEN_MAIN) == 0) {
            break;
        }

        param0->unk_10 = 1;

        ToggleBgLayer(GF_BG_LYR_MAIN_3, 0);
        BgSetPosTextAndCommit(param0->unk_00->bgConfig, 3, BG_POS_OP_SET_X, 0);
        BgSetPosTextAndCommit(param0->unk_00->bgConfig, 3, BG_POS_OP_SET_Y, 0);

        param0->unk_04++;
        break;
    default:
        return 0;
    }

    return 1;
}

s32 ov80_0222DC14(void *arg0) {
    s32 v0;

    v0 = *(s32 *)((u8 *)arg0 + 4);
    switch (v0) {
    case 0:
        ov80_0223B424(arg0 + 0x18, 0x65);
        *(s32 *)((u8 *)arg0 + 4) = (s32)(*(s32 *)((u8 *)arg0 + 4) + 1);
        break;
    case 1:
        ov80_0223B440(arg0 + 0x18, 0, 0xBF, 0x2AA, 0x12000, 0x190, (void *)REG_BG3OFS_ADDR, 0, 4);
        *(s32 *)((u8 *)arg0 + 4) = (s32)(*(s32 *)((u8 *)arg0 + 4) + 1);
        break;
    }
    return 1;
}
