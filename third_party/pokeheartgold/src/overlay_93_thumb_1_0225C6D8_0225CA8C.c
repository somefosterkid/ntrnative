#include "global.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "msgdata.h"
#include "struct_defs/resdat_narc_structs.h"

struct OverlayManager;

struct Camera;

struct CameraPositionHistory;

struct CharTransferAllocation;

struct MessageBank;

struct MessageBankEntry;

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

struct SaveBlockInfo;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct UnkStruct_ov117_02261280_t;

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

BOOL ov90_02258B98(const UnkStruct_ov114_0225C76C *param0);

void ov90_02258C8C(UnkStruct_ov114_0225CAD4 *param0);

void ov90_02258CB0(UnkStruct_ov114_0225CAD4 *param0);

BOOL ov90_02258CE0(const UnkStruct_ov114_0225CAD4 *param0);

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

void ov93_02262250(UnkStruct_ov117_02261280_t *param0);

void ov93_02262310(UnkStruct_ov117_02261280_t *param0);

BOOL ov93_02262374(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02266274 *param1);

BOOL ov93_022627A4(UnkStruct_ov117_02261280_t *param0);

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

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

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

typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;

typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;

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

void sub_0200FB70(void);

void sub_0200FC20(u16 color);

void sub_02037AC0(u8 syncNo);

BOOL sub_02037B38(u8 syncState);

int ov93_0225E300(UnkStruct_ov117_02261280_t *param0);

void ov93_0225E370(UnkStruct_ov117_02261280_t *param0);

int ov93_0225E10C(UnkStruct_ov117_02261280_t *param0);

BOOL ov93_0225E4B0(UnkStruct_ov117_02261280_t *param0);

BOOL ov93_0225E764(UnkStruct_ov117_02261280_t *param0);

void ov93_0225D4B8(UnkStruct_ov117_02261280_t *param0);

void ov93_0225D5AC(UnkStruct_ov117_02261280_t *param0, int param1);

void ov93_0225E7AC(Camera *camera);

int ov93_0225CA8C(OverlayManager *appMan, int *param1);

int ov93_0225CA8C(OverlayManager *appMan, int *param1) {
    UnkStruct_ov117_02261280_t *v0 = OverlayManager_GetData(appMan);

    if (v0->unk_00->unk_3D == 1) {
        switch (v0->unk_00->unk_3E) {
        case 0:
            if (IsPaletteFadeFinished() == 1) {
                sub_0200FB70();
            }

            sub_0200FC20(((0) << 10) | ((0) << 5) | (0));
            GX_SetVisibleWnd(GX_WNDMASK_NONE);
            v0->unk_00->unk_3E++;
            break;

        default:
        case 1:
            if (ov90_02258B98(&v0->unk_00->unk_00) == 1) {
                return 1;
            }
            break;
        }

        return 0;
    }

    switch (*param1) {
    case 0:
        if (IsPaletteFadeFinished() == 1) {
            (*param1)++;
        }
        break;
    case 1:
        sub_02037AC0(211);
        (*param1)++;
        break;
    case 2:
        if (sub_02037B38(211) == 1) {
            (*param1)++;
        }
        break;
    case 3:
        if (ov93_022627A4(v0) == 1) {
            (*param1)++;
        }
        break;
    case 4:
        if (v0->unk_08 >= v0->unk_00->unk_30) {
            ov93_0225E10C(v0);
            (*param1)++;
        }
        break;
    case 5:
        if (v0->unk_2FC0 == 1) {
            ov93_02262250(v0);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_UNK_26, FADE_TYPE_UNK_26, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_117);
            (*param1)++;
        }

        ov93_0225E764(v0);

        switch (v0->unk_20) {
        case 1:
            GX_SetVisibleWnd(GX_WNDMASK_W1);
            G2_SetWnd1Position(4, 192 - 32 + 8, 8 * 9 + 4, 192 - 8);
            G2_SetWnd1InsidePlane(GX_WND_PLANEMASK_OBJ, 0);
            G2_SetWndOutsidePlane(GX_WND_PLANEMASK_BG0 | GX_WND_PLANEMASK_BG1 | GX_WND_PLANEMASK_BG2 | GX_WND_PLANEMASK_BG3 | GX_WND_PLANEMASK_OBJ, 1);
            ov93_02262310(v0);
            v0->unk_20 = 2;
        case 2:
            if (ov93_02262374(v0, &v0->unk_174C) == 1) {
                v0->unk_174C.unk_1C = 1;
                v0->unk_20 = 0;
            }
            break;
        case 3:
            ov90_02258C8C(v0->unk_1C);
            v0->unk_20 = 4;
            break;
        case 4:
            if (ov90_02258CE0(v0->unk_1C) == 1) {
                v0->unk_2FB8 = 1;
                v0->unk_1468.unk_F1 = 0;
                v0->unk_20 = 5;
                ov93_0225D4B8(v0);
                ov93_0225D5AC(v0, 1);
            }
            break;
        case 6:
            ov90_02258CB0(v0->unk_1C);
            v0->unk_20 = 7;
            break;
        case 7:
            if (ov90_02258CE0(v0->unk_1C) == 1) {
                v0->unk_20 = 8;
            }
            break;
        }

        ov93_0225E4B0(v0);
        ov93_0225E370(v0);

        if (v0->unk_2FBC == 1) {
            break;
        }

        if (ov93_0225E300(v0) == 1) {
            v0->unk_2FC4 = 13;
        }

        if (v0->unk_2FB4 == (3 * 30)) {
            (void)0;
        }

        if ((v0->unk_2FB8 == 1) && (v0->unk_2FB4 <= (43 * 30 + 12))) {
            v0->unk_2FB4++;
        }
        break;
    case 6:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }

    ov93_0225E7AC(v0->camera1);
    v0->unk_384C++;

    return 0;
}
