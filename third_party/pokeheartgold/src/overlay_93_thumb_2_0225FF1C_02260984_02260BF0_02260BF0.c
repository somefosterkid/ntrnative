#include "global.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_41_sprite_resource_collection.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "msgdata.h"
#include "font.h"
#include "struct_defs/resdat_narc_structs.h"

struct BgConfig;

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

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

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

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef u32 TextColor;

int ov93_0225E3C4(UnkStruct_ov117_02261280_t *param0, int param1);

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

u16 sub_0203769C(void);

extern const u16 ov93_02262DA4[5][4];

extern const u16 ov93_02262C72[4];

void ov117_02265DB8(BgConfig *param0, SpriteManager *param1, UnkStruct_02012744 *param2, UnkStruct_ov117_02265EB0 *param3, const String *param4, enum Font param5, TextColor param6, int param7, int param8, int param9, int param10, int param11, int param12, int param13, int param14);

void ov93_02260BF0(UnkStruct_ov117_02261280_t *param0);

void ov93_02260BF0(UnkStruct_ov117_02261280_t *param0) {
    u16 v0[4][3];
    int v1, v2;
    int v3 = 0;
    int v4, v5, v6, v7;
    u16 *v8, *v9;

    for (v2 = 0; v2 < param0->unk_00->unk_30; v2++) {
        v4 = ov93_02262C72[v2];

        for (v1 = 0; v1 < 3; v1++) {
            v0[v2][v1] = PaletteData_GetBufferColorAtIndex(param0->unk_8C, (PaletteBufferId)(1), (enum PaletteSelector)(1), v4 + v1);
        }
    }

    v7 = sub_0203769C();

    for (v3 = 0; v3 < param0->unk_00->unk_30; v3++) {
        if (param0->unk_00->unk_2C[v3] == v7) {
            break;
        }
    }

    v8 = PaletteData_GetUnfadedBuf(param0->unk_8C, (PaletteBufferId)(1));
    v9 = PaletteData_GetFadedBuf(param0->unk_8C, (PaletteBufferId)(1));

    for (v1 = 0; v1 < param0->unk_00->unk_30; v1++) {
        v4 = v1;
        v5 = ov93_02262DA4[param0->unk_00->unk_30][ov93_0225E3C4(param0, param0->unk_00->unk_2C[v1])];

        for (v6 = 0; v6 < 3; v6++) {
            v8[v5 + v6] = v0[v4][v6];
            v9[v5 + v6] = v0[v4][v6];
        }
    }
}

extern const u16 ov93_02262D04[5][4];

extern const u16 ov93_02262C8A[4];

void ov93_02260CF8(UnkStruct_ov117_02261280_t *param0);

void ov93_02260CF8(UnkStruct_ov117_02261280_t *param0) {
    u16 v0[4][16];
    int v1, v2;
    int v3 = 0;
    int v4, v5, v6, v7;
    u16 *v8, *v9;
    int v10 = SpriteManager_FindPlttResourceOffset(param0->unk_28, 10006, NNS_G2D_VRAM_TYPE_2DSUB);

    for (v2 = 0; v2 < param0->unk_00->unk_30; v2++) {
        v4 = ov93_02262C8A[v2];

        for (v1 = 0; v1 < 16; v1++) {
            v0[v2][v1] = PaletteData_GetBufferColorAtIndex(param0->unk_8C, (PaletteBufferId)(3), (enum PaletteSelector)(1), v10 * 16 + v4 + v1);
        }
    }

    v7 = sub_0203769C();

    for (v3 = 0; v3 < param0->unk_00->unk_30; v3++) {
        if (param0->unk_00->unk_2C[v3] == v7) {
            break;
        }
    }

    v8 = PaletteData_GetUnfadedBuf(param0->unk_8C, (PaletteBufferId)(3));
    v9 = PaletteData_GetFadedBuf(param0->unk_8C, (PaletteBufferId)(3));

    for (v1 = 0; v1 < param0->unk_00->unk_30; v1++) {
        v4 = v1;
        v5 = ov93_02262D04[param0->unk_00->unk_30][ov93_0225E3C4(param0, param0->unk_00->unk_2C[v1])] + v10 * 16;

        for (v6 = 0; v6 < 16; v6++) {
            v8[v5 + v6] = v0[v4][v6];
            v9[v5 + v6] = v0[v4][v6];
        }
    }
}

extern const u16 ov93_02262D54[5][4];

extern const u16 ov93_02262C82[4];

void ov93_02260E1C(UnkStruct_ov117_02261280_t *param0);

void ov93_02260E1C(UnkStruct_ov117_02261280_t *param0) {
    u16 v0[4][1];
    int v1, v2;
    int v3 = 0;
    int v4, v5, v6, v7;
    u16 *v8, *v9;
    int v10 = SpriteManager_FindPlttResourceOffset(param0->unk_28, 10006, NNS_G2D_VRAM_TYPE_2DSUB);

    for (v2 = 0; v2 < param0->unk_00->unk_30; v2++) {
        v4 = ov93_02262C82[v2];

        for (v1 = 0; v1 < 1; v1++) {
            v0[v2][v1] = PaletteData_GetBufferColorAtIndex(param0->unk_8C, (PaletteBufferId)(3), (enum PaletteSelector)(1), v10 * 16 + v4 + v1);
        }
    }

    v7 = sub_0203769C();

    for (v3 = 0; v3 < param0->unk_00->unk_30; v3++) {
        if (param0->unk_00->unk_2C[v3] == v7) {
            break;
        }
    }

    v8 = PaletteData_GetUnfadedBuf(param0->unk_8C, (PaletteBufferId)(3));
    v9 = PaletteData_GetFadedBuf(param0->unk_8C, (PaletteBufferId)(3));

    for (v1 = 0; v1 < param0->unk_00->unk_30; v1++) {
        v4 = v1;
        v5 = ov93_02262D54[param0->unk_00->unk_30][ov93_0225E3C4(param0, param0->unk_00->unk_2C[v1])] + v10 * 16;

        for (v6 = 0; v6 < 1; v6++) {
            v8[v5 + v6] = v0[v4][v6];
            v9[v5 + v6] = v0[v4][v6];
        }
    }
}
