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

struct ManagedSprite;

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

struct Sprite;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct ManagedSpriteTemplate;

struct String;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct UnkStruct_ov117_02261280_t;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

enum AffineOverwriteMode {
    AFFINE_OVERWRITE_MODE_NONE = 0,
    AFFINE_OVERWRITE_MODE_NORMAL,
    AFFINE_OVERWRITE_MODE_DOUBLE,
};

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

ManagedSprite *SpriteSystem_NewSprite(SpriteSystem *spriteSys, SpriteManager *spriteMan, const ManagedSpriteTemplate *template);

void Sprite_TickFrame(Sprite *sprite);

void ManagedSprite_SetDrawFlag(ManagedSprite *managedSprite, BOOL draw);

void ManagedSprite_SetPositionXYWithSubscreenOffset(ManagedSprite *managedSprite, s16 x, s16 y, fx32 offset);

void ManagedSprite_SetAffineScale(ManagedSprite *managedSprite, f32 xScale, f32 yScale);

void ManagedSprite_SetOamMode(ManagedSprite *managedSprite, GXOamMode mode);

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

fx32 GF_SinDegFX32(fx32 degrees);

fx32 GF_CosDegFX32(fx32 degrees);

void ov93_02262444(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_02266344 *param1);

extern const ManagedSpriteTemplate ov93_02262E9C;

extern const ManagedSpriteTemplate ov93_02262ED0;

extern const ManagedSpriteTemplate ov93_02262F04;

extern const u16 ov93_02262C7A[4];

void ov93_02261164(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1);

void ov93_02261164(UnkStruct_ov117_02261280_t *param0, UnkStruct_ov117_022653F4 *param1) {
    ManagedSpriteTemplate v0;
    int v1;
    fx32 v3, v4;

    v0 = ov93_02262E9C;

    for (v1 = 0; v1 < 3; v1++) {
        v3 = 256 / 2 + FX_MulInline(GF_SinDegFX32(param1->unk_E4 + param1->unk_00[v1].unk_0C), 76);
        v4 = 196 / 2 + (-FX_MulInline(GF_CosDegFX32(param1->unk_E4 + param1->unk_00[v1].unk_0C), (64 + 4)));

        param1->unk_00[v1].unk_00 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &v0);
        ManagedSprite_SetPositionXYWithSubscreenOffset(param1->unk_00[v1].unk_00, v3, v4 + -24, ((192 + 160) << 12));
        ManagedSprite_SetAnim(param1->unk_00[v1].unk_00, ov93_02262C7A[v1 + 1]);
        Sprite_TickFrame(param1->unk_00[v1].unk_00->sprite);
        ManagedSprite_SetDrawFlag(param1->unk_00[v1].unk_00, 0);

        param1->unk_00[v1].unk_04 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &ov93_02262ED0);
        ManagedSprite_SetPositionXYWithSubscreenOffset(param1->unk_00[v1].unk_04, 0, 0, ((192 + 160) << 12));
        ManagedSprite_SetAnim(param1->unk_00[v1].unk_04, 33);
        Sprite_TickFrame(param1->unk_00[v1].unk_04->sprite);
        ManagedSprite_SetDrawFlag(param1->unk_00[v1].unk_04, 0);

        param1->unk_00[v1].unk_08 = SpriteSystem_NewSprite(param0->unk_24, param0->unk_28, &ov93_02262F04);
        ManagedSprite_SetPositionXYWithSubscreenOffset(param1->unk_00[v1].unk_08, v3, v4 + -24 + 24, ((192 + 160) << 12));
        ManagedSprite_SetOamMode(param1->unk_00[v1].unk_08, GX_OAM_MODE_XLU);
        ManagedSprite_SetAffineOverwriteMode(param1->unk_00[v1].unk_08, AFFINE_OVERWRITE_MODE_NORMAL);
        ManagedSprite_SetAffineScale(param1->unk_00[v1].unk_08, 1.0f, 1.0f);
        ManagedSprite_SetAnim(param1->unk_00[v1].unk_08, 34 + v1);
        Sprite_TickFrame(param1->unk_00[v1].unk_08->sprite);
        ManagedSprite_SetDrawFlag(param1->unk_00[v1].unk_08, 0);

        ov93_02262444(param0, &param1->unk_00[v1].unk_30);
    }

    ManagedSprite_SetDrawFlag(param1->unk_00[0].unk_00, 1);
    ManagedSprite_SetDrawFlag(param1->unk_00[0].unk_08, 1);
}
