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

struct CameraPositionHistory;

struct CharTransferAllocation;

struct Field3DModelAnimation;

struct Field3dModel;

struct Field3dObject;

struct MessageBank;

struct MessageBankEntry;

struct NARC;

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

int ov93_0225E45C(UnkStruct_ov117_02261280_t *param0, int param1);

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

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

void sub_02018030(Field3dModel *model, NARC *narc, u32 memberIndex, u32 heapID);

void sub_020180BC(Field3DModelAnimation *anim, const Field3dModel *model, NARC *narc, u32 memberIndex, enum HeapID heapID, NNSFndAllocator *allocator);

void sub_020180E8(Field3DModelAnimation *anim, const Field3dModel *model, void *data, NNSFndAllocator *allocator);

void sub_02018198(Field3DModelAnimation *anim, fx32 frame);

void sub_020181B0(Field3dObject *obj, Field3dModel *model);

void sub_020182A0(Field3dObject *obj, BOOL visible);

void sub_020182A8(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

void sub_020182C4(Field3dObject *obj, fx32 x, fx32 y, fx32 z);

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

u16 sub_0203769C(void);

extern const struct {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
} ov93_02262AF0[4];

void ov93_0225DD2C(UnkStruct_ov117_02261280_t *param0, NARC *param1);

void ov93_0225DD2C(UnkStruct_ov117_02261280_t *param0, NARC *param1) {
    int v0, v1;
    UnkStruct_ov117_02261F08 *v2 = &param0->unk_D8;
    UnkStruct_ov117_02261C2C *v3 = &param0->unk_13A0;
    int v4;
    void *v5 = 0L;

    v4 = ov93_0225E45C(param0, sub_0203769C());

    sub_02018030(&v2->unk_00, param1, ov93_02262AF0[v4].unk_00, HEAP_ID_117);
    sub_020181B0(&v2->unk_10, &v2->unk_00);
    sub_020182A8(&v2->unk_10, ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), ((fx32)(((-25) > 0) ? ((-25) * (1 << 12) + 0.5f) : ((-25) * (1 << 12) - 0.5f))), ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))));
    sub_020182C4(&v2->unk_10, ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))), ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))), ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))));
    sub_020182A0(&v2->unk_10, 1);
    sub_02018030(&v2->unk_88[0], param1, ov93_02262AF0[v4].unk_04, HEAP_ID_117);
    sub_02018030(&v2->unk_88[1], param1, ov93_02262AF0[v4].unk_08, HEAP_ID_117);
    sub_02018030(&v2->unk_88[2], param1, ov93_02262AF0[v4].unk_0C, HEAP_ID_117);
    sub_02018030(&v2->unk_88[3], param1, ov93_02262AF0[v4].unk_10, HEAP_ID_117);

    for (v0 = 0; v0 < 8; v0++) {
        for (v1 = 0; v1 < 4; v1++) {
            if (v5 == 0L) {
                sub_020180BC(&v2->unk_C8[v0][v1].unk_7C, &v2->unk_88[v1], param1, 35 + v1, (enum HeapID)(117), &param0->unk_A8);
                v5 = v2->unk_C8[v0][v1].unk_7C.fileDataRaw;
            } else {
                sub_020180E8(&v2->unk_C8[v0][v1].unk_7C, &v2->unk_88[v1], v5, &param0->unk_A8);
            }

            sub_02018198(&v2->unk_C8[v0][v1].unk_7C, 0);
        }
    }

    sub_02018030(&v3->unk_00, param1, 30, HEAP_ID_117);
    sub_020181B0(&v3->unk_10, &v3->unk_00);
    sub_020182A8(&v3->unk_10, ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))), ((fx32)(((-25) > 0) ? ((-25) * (1 << 12) + 0.5f) : ((-25) * (1 << 12) - 0.5f))), ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f))));
    sub_020182C4(&v3->unk_10, ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))), ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))), ((fx32)(((1.00f) > 0) ? ((1.00f) * (1 << 12) + 0.5f) : ((1.00f) * (1 << 12) - 0.5f))));
    sub_020182A0(&v3->unk_10, 1);
}
