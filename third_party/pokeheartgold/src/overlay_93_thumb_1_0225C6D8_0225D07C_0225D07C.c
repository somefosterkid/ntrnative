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

struct BgConfig;

struct Camera;

struct CameraPositionHistory;

struct CharTransferAllocation;

struct Field3dObject;

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

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

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

void ov93_0225E898(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022626B0 *param1);

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

void SpriteSystem_DrawSprites(SpriteManager *spriteMan);

void SpriteSystem_UpdateTransfer(void);

void ov93_0225FE80(UnkStruct_ov117_02261280_t *param0, BgConfig *param1, UnkStruct_ov117_02263DAC *param2);

void ov93_0225FFF8(UnkStruct_ov117_02261280_t *param0);

BOOL ov93_02260660(UnkStruct_ov117_02261280_t *param0);

BOOL ov93_02260A30(UnkStruct_ov117_02261280_t *param0);

void ov93_02261354(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1);

void ov93_02261D1C(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265C3C *param1);

void ov93_02262034(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02265EC8 *param1);

void sub_020181EC(Field3dObject *obj);

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

void Camera_SetStaticPtr(Camera *camera);

void Camera_PushLookAtToNNSGlb(void);

void Camera_ApplyPerspectiveType(const u8 projection, Camera *camera);

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

int sub_0201543C(void);

int sub_02015460(void);

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

void sub_020399FC(int unused_0, BgConfig *bgConfig);

void Thunk_G3X_Reset(void);

void RequestSwap3DBuffers(GXSortMode param0, GXBufferMode param1);

void ov93_0225E03C(UnkStruct_ov117_02261280_t *param0);

void ov93_0225E0A4(UnkStruct_ov117_02261280_t *param0);

void ov93_0225D07C(SysTask *param0, void *param1);

void ov93_0225D07C(SysTask *param0, void *param1) {
    UnkStruct_ov117_02261280_t *v0 = param1;
    VecFx32 v1, v2;
    MtxFx33 v3;

    v1.x = ((fx32)0x0000000000001000L);
    v1.y = ((fx32)0x0000000000001000L);
    v1.z = ((fx32)0x0000000000001000L);

    v2.x = 0;
    v2.y = 0;
    v2.z = 0;

    MTX_Identity33(&v3);

    ov93_02262034(v0, &v0->unk_15A8);
    ov93_02261354(v0, &v0->unk_1468);
    ov93_02261D1C(v0, &v0->unk_1560);
    ov93_0225FFF8(v0);
    ov93_0225FE80(v0, v0->unk_2C, &v0->unk_1428);
    ov93_02260660(v0);
    ov93_0225E03C(v0);
    ov93_02260A30(v0);

    {
        Thunk_G3X_Reset();
        Camera_SetStaticPtr(v0->camera1);
        Camera_ApplyPerspectiveType(1, v0->camera1);
        Camera_PushLookAtToNNSGlb();

        NNS_G3dGlbLightVector((GXLightId)(0), 0, -((fx32)0x0000000000001000L), 0);
        NNS_G3dGlbLightColor((GXLightId)(0), ((GXRgb)(((28) << (0)) | ((28) << (5)) | ((28) << (10)))));
        NNS_G3dGlbMaterialColorDiffAmb(((GXRgb)(((31) << (0)) | ((31) << (5)) | ((31) << (10)))), ((GXRgb)(((31) << (0)) | ((31) << (5)) | ((31) << (10)))), 0);
        NNS_G3dGlbMaterialColorSpecEmi(((GXRgb)(((31) << (0)) | ((31) << (5)) | ((31) << (10)))), ((GXRgb)(((31) << (0)) | ((31) << (5)) | ((31) << (10)))), 0);
        NNS_G3dGlbSetBaseTrans(&v2);
        NNS_G3dGlbSetBaseRot(&v3);
        NNS_G3dGlbSetBaseScale(&v1);
        NNS_G3dGlbFlush();

        ov93_0225E898(v0, v0->unk_D4);
        NNS_G3dGePushMtx();

        {
            sub_020181EC(&v0->unk_13A0.unk_10);
            sub_020181EC(&v0->unk_D8.unk_10);
            ov93_0225E0A4(v0);
        }

        NNS_G3dGePopMtx(1);
    }

    {
        int v4;

        Thunk_G3X_Reset();
        v4 = sub_0201543C();

        if (v4 > 0) {
            Thunk_G3X_Reset();
        }

        sub_02015460();
    }

    SpriteSystem_DrawSprites(v0->unk_28);
    SpriteSystem_UpdateTransfer();
    RequestSwap3DBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    sub_020399FC(117, v0->unk_2C);
}
