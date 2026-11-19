#include "global.h"

#include "battle/battler_sprite_context.h"

#include "heap.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_system.h"
#include "filesystem_files_def.h"
#include "overlay_07_battle_anim_funcs.h"

struct CustomBgScrollContext;

struct Pokepic;

struct PokemonSpriteTransforms;

struct SPLEmitter;

struct SPLParticle;

typedef struct Background {
    void *tilemapBuffer;
    u32 bufferSize;
    u32 baseTile;
    int hOffset;
    int vOffset;
    u8 mode;
    u8 size;
    u8 colorMode;
    u8 tileSize;
    u16 rotation;
    u16 unk22;
    int xScale;
    int yScale;
    int centerX;
    int centerY;
} Background;

typedef struct BgConfig {
    enum HeapID heapID;
    u16 scrollScheduled;
    u16 bufferTransferScheduled;
    struct Background bgs[8];
} BgConfig;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef struct Pokepic Pokepic;

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);

typedef struct PokepicTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokepicTemplate;

struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

typedef struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokemonSpriteShadow;

struct Pokepic {
    u32 active : 1;
    u32 polygonID : 6;
    u32 needReloadChar : 1;
    u32 needReloadPltt : 1;
    u32 padding_00_9 : 23;
    PokepicTemplate template;
    PokepicTemplate templateBackup;
    PokemonSpriteTransforms transforms;
    u8 animActive;
    u8 currAnimFrame;
    u8 animFrameDelay;
    u8 currSpriteFrame;
    u8 animLoopTimers[10];
    u8 padding_66[2];
    PokemonSpriteCallback *callback;
    PokemonSpriteShadow shadow;
    PokemonSpriteShadow shadowBackup;
    SpriteAnimFrame animFrames[10];
};

typedef struct CameraAngle {
    u16 x;
    u16 y;
    u16 z;
    u16 padding_06;
} CameraAngle;

typedef struct CameraLookAt {
    VecFx32 position;
    VecFx32 target;
    VecFx32 up;
} CameraLookAt;

typedef struct CameraPerspectiveProjection {
    fx32 sinFovY;
    fx32 cosFovY;
    fx32 aspectRatio;
    fx32 nearClip;
    fx32 farClip;
} CameraPerspectiveProjection;

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

typedef struct Camera {
    CameraPerspectiveProjection perspective;
    CameraLookAt lookAt;
    fx32 distance;
    CameraAngle angle;
    u8 projection;
    u16 fovY;
    VecFx32 prevTargetPos;
    const VecFx32 *targetPos;
    BOOL trackTargetX;
    BOOL trackTargetY;
    BOOL trackTargetZ;
    CameraPositionHistory *history;
} Camera;

typedef struct SPLParticle {
    struct SPLParticle *p_next;
    struct SPLParticle *p_prev;
    VecFx32 ptcl_pos;
    VecFx32 vel;
    u16 rtt;
    s16 rtt_vel;
    u16 life;
    u16 age;
    u16 life_rate_diff_loop;
    u16 life_rate_diff_noloop;
    struct { u16 tex_no : 8; u16 life_rate_offset : 8; } etc;
    struct {
        u16 baseAlpha : 5;
        u16 animAlpha : 5;
        u16 currentPolygonID : 6;
    } alp;
    fx32 base_scl;
    fx16 anm_scl;
    GXRgb clr;
    VecFx32 emtr_pos;
} SPLParticle;

typedef struct SPLParticleList {
    SPLParticle *first;
    int count;
    SPLParticle *last;
} SPLParticleList;

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

typedef struct SPLResource {
    SPLResourceHeader *header;
    SPLScaleAnim *scaleAnim;
    SPLColorAnim *colorAnim;
    SPLAlphaAnim *alphaAnim;
    SPLTexAnim *texAnim;
    SPLChildResource *childResource;
    SPLBehavior *behaviors;
    u16 behaviorCount;
    u16 reserved;
} SPLResource;

enum SPLUpdateCallbackType {
    SPL_CALLBACK_PRE_UPDATE = 0,
    SPL_CALLBACK_POST_UPDATE = 1,
};

typedef void (*SPLEmitterUpdateCallback)(struct SPLEmitter *emitter, enum SPLUpdateCallbackType type);

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

typedef struct SPLEmitter {
    struct SPLEmitter *next;
    struct SPLEmitter *prev;
    SPLParticleList particles;
    SPLParticleList childParticles;
    SPLResource *resource;
    SPLEmitterState state;
    VecFx32 position;
    VecFx32 velocity;
    VecFx32 particleInitVelocity;
    u16 age;
    fx16 emissionCountFractional;
    VecFx16 axis;
    u16 initAngle;
    fx32 emissionCount;
    fx32 radius;
    fx32 length;
    fx32 initVelPositionAmplifier;
    fx32 initVelAxisAmplifier;
    fx32 baseScale;
    u16 particleLifeTime;
    GXRgb color;
    fx32 collisionPlaneHeight;
    fx16 textureS;
    fx16 textureT;
    fx16 childTextureS;
    fx16 childTextureT;
    struct {
        u32 emissionInterval : 8;
        u32 baseAlpha : 8;
        u32 updateCycle : 3;
        u32 reserved : 13;
    } misc;
    VecFx16 crossAxis1;
    VecFx16 crossAxis2;
    SPLEmitterUpdateCallback updateCallback;

    void *userDataPtr;
    union {
        u32 u32;
        u16 u16[2];
        u8 u8[4];
    } userData;
} SPLEmitter;

typedef struct SPLEmitterList {
    SPLEmitter *first;
    int count;
    SPLEmitter *last;
} SPLEmitterList;

static inline void SPLEmitter_SetPosX(SPLEmitter *emtr, fx32 x) {
    emtr->position.x = x + emtr->resource->header->emitterBasePos.x;
}

static inline void SPLEmitter_SetPosY(SPLEmitter *emtr, fx32 y) {
    emtr->position.y = y + emtr->resource->header->emitterBasePos.y;
}

typedef union SPLTextureParam {
    u32 all;
    struct {
        u32 format : 4;
        u32 s : 4;
        u32 t : 4;
        u32 repeat : 2;
        u32 flip : 2;
        u32 palColor0 : 1;
        u32 useSharedTexture : 1;
        u32 sharedTexID : 8;
        u32 : 6;
    };
} SPLTextureParam;

typedef struct SPLTexture {
    const void *resource;
    u32 texAddr;
    u32 palAddr;
    SPLTextureParam param;
    u16 width;
    u16 height;
} SPLTexture;

typedef void *(*SPLAllocFunc)(u32);

typedef struct SPLManager {
    SPLAllocFunc alloc;
    SPLEmitterList activeEmitters;
    SPLEmitterList inactiveEmitters;
    SPLParticleList inactiveParticles;
    SPLResource *resources;
    SPLTexture *textures;
    u16 resCount;
    u16 texCount;
    u16 maxEmitters;
    u16 maxParticles;
    struct {
        u32 min : 6;
        u32 max : 6;
        u32 current : 6;
        u32 fix : 6;
        u32 drawOrder : 1;
        u32 unused : 7;
    } polygonID;
    GXPolygonAttrMisc miscPolygonAttr;
    struct {
        SPLEmitter *emitter;
        const MtxFx43 *viewMatrix;
    } renderState;
    u16 currentCycle;
    u16 reserved;
} SPLManager;

typedef struct ParticleSystem {
    SPLManager *manager;
    void *resource;
    SPLEmitter *lastAddedEmitter;
    void *heapStart;
    void *heap;
    void *heapEnd;
    void *texAllocFunc;
    void *palAllocFunc;
    Camera *camera;
    VecFx32 sLotoId;
    u16 cameraFov;
    u8 pad_32[2];
    VecFx32 cameraPos;
    VecFx32 cameraUp;
    VecFx32 cameraTarget;
    u32 texKeys[16];
    u32 plttKeys[16];
    u8 vramAutoRelease;
    u8 unused2;
    u8 id;
    u8 cameraProjection;
} ParticleSystem;

void sub_020154C4(ParticleSystem *particleSystem, SPLEmitter *emitter);

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct CustomBgScrollContext CustomBgScrollContext;

void ov07_0221C448(BattleAnimSystem *system, SysTask *task);

typedef struct BufferManager {
    void *buffers[2];
    u8 writeBuffer;
    u8 mode;
    u8 padding[2];
} BufferManager;

typedef struct ValueLerpContext {
    s32 value;
    s32 data[4];
} ValueLerpContext;

typedef void (*VBlankDMAFunc)(void *);

typedef struct VBlankDMAController {
    SysTask *onVBlankTask;
    SysTask *postVBlankTask;
    BOOL doDMA;
    BOOL swapBuffers;
    VBlankDMAFunc dmaFunc;
    VBlankDMAFunc swapBufferFunc;
    void *param;
} VBlankDMAController;

struct CustomBgScrollContext {
    VBlankDMAController dmaController;
    BufferManager *bufferManager;
    u32 buffer1[192];
    u32 buffer2[192];
    void *offsetReg;
};

BOOL ov07_022222B4(XYTransformContext *ctx);

fx32 GF_SinDeg(u16 degrees);

void ov07_02232508(void *ptr);

typedef struct EmitterAnimationContext {
    int emitterID;
    s16 startDelay;
    s16 frames;
    int radius;
    Point2D offset;
    int mode;
    int startBattler;
    int endBattler;
    int frame;
    int skipFrames;
    int maxFrames;
    int timer;
    int angle;
    BOOL curve;
    ParticleSystem *particleSys;
    SPLEmitter *emitter;
    BattleAnimScriptFuncCommon common;
    BattleAnimSpriteInfo unk_58[4];
    XYTransformContext transforms[2];
    ValueLerpContext unused;
} EmitterAnimationContext;

BOOL ov07_022260DC(SPLEmitter *emitter);

static void ov07_022260F8(SysTask *task, void *param);

static void ov07_022260F8(SysTask *task, void *param) {
    EmitterAnimationContext *ctx = param;
    BOOL emitterActive = ov07_022260DC(ctx->emitter);

    if (ctx->timer < ctx->startDelay) {
        ctx->timer++;
        return;
    }

    if (ov07_022222B4(&ctx->transforms[0]) == 0 && emitterActive == 0) {
        sub_020154C4(ctx->particleSys, ctx->emitter);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        ov07_02232508(ctx);
        ctx = 0L;
        return;
    } else {
        if ((ctx->skipFrames > ctx->frame || ctx->maxFrames < ctx->frame) && ctx->maxFrames != 0xFF) {
            return;
        }

        SPLEmitter_SetPosX(ctx->emitter, ((ctx->transforms[0].x) * 172));
        SPLEmitter_SetPosY(ctx->emitter, ((ctx->transforms[0].y) * 172));

        if (ctx->curve) {
            ctx->angle += 360 / (ctx->frames);
            SPLEmitter_SetPosY(ctx->emitter, ((ctx->transforms[0].y) * 172) + GF_SinDeg(ctx->angle));
        }
    }
}

SysTask *ov07_0221C410(BattleAnimSystem *system, SysTaskFunc func, void *param);

u16 ov07_0221C468(BattleAnimSystem *system);

void ov07_02222268(XYTransformContext *ctx, s16 sx, s16 ex, s16 sy, s16 ey, u16 steps);

void ov07_022223F0(ValueLerpContext *ctx, s32 start, s32 end, u32 steps);

void ov07_02231B90(BattleAnimSystem *system, int battler, VecFx32 *pos);

void ov07_02231FE4(BattleAnimSystem *system, BattleAnimScriptFuncCommon *common);

void ov07_0222600C(BattleAnimSystem *system, EmitterAnimationContext *ctx);

void ov07_022261B4(BattleAnimSystem *system);

void ov07_022261B4(BattleAnimSystem *system) {
    EmitterAnimationContext *ctx = ov07_022324D8(system, sizeof(EmitterAnimationContext));

    ov07_02231FE4(system, &ctx->common);
    ov07_0222600C(system, ctx);

    int dir = ov07_02222004(system, ov07_0221C468(system));

    VecFx32 startPos, endPos;
    ov07_02231B90(system, ctx->startBattler, &startPos);
    ov07_02231B90(system, ctx->endBattler, &endPos);

    ov07_02222268(
        &ctx->transforms[0],
        ((startPos.x) / 172),
        ((endPos.x) / 172) + (ctx->offset.x * dir),
        ((startPos.y) / 172),
        ((endPos.y) / 172) + (ctx->offset.y * dir),
        ctx->frames);

    if (dir > 0) {
        ov07_022223F0(&ctx->unused, (((20) * 0xFFFF) / 360) * dir, (((130) * 0xFFFF) / 360) * dir, 10);
    } else {
        ov07_022223F0(&ctx->unused, (((90) * 0xFFFF) / 360) * dir, (((130) * 0xFFFF) / 360) * dir, 10);
    }

    for (int i = 0; i < ctx->skipFrames; i++) {
        ov07_022222B4(&ctx->transforms[0]);
    }

    if (ctx->maxFrames != 0xFF) {
        ctx->frame = ctx->maxFrames + 1;
    }

    SPLEmitter_SetPosX(ctx->emitter, ((ctx->transforms[0].x) * 172));
    SPLEmitter_SetPosY(ctx->emitter, ((ctx->transforms[0].y) * 172));

    ov07_0221C410(ctx->common.battleAnimSys, ov07_022260F8, ctx);
}

u16 ov07_0221C470(BattleAnimSystem *system);

ParticleSystem *ov07_0221C478(BattleAnimSystem *system);

s32 ov07_0221C4A8(BattleAnimSystem *system, int index);

void ov07_02231DD0(VecFx32 *pos);

void ov07_022262E8(BattleAnimSystem *system);

void ov07_022262E8(BattleAnimSystem *system) {
    EmitterAnimationContext *ctx = ov07_022324D8(system, sizeof(EmitterAnimationContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->emitterID = ov07_0221C4A8(system, 0);
    ctx->mode = ov07_0221C4A8(system, 1);

    int type = ov07_0221C4A8(system, 2);

    ctx->frames = ov07_0221C4A8(system, 3);
    ctx->startDelay = ov07_0221C4A8(system, 4);
    ctx->timer = 0;

    int params = ov07_0221C4A8(system, 5);

    ctx->frame = 0;
    ctx->skipFrames = ((params & 0xFFFF0000) >> 16);
    ctx->maxFrames = ((params) & 0x0000FFFF);

    if (ctx->skipFrames == 0) {
        ctx->skipFrames = 0;
    }

    if (ctx->maxFrames == 0) {
        ctx->maxFrames = 0xFF;
    }

    ctx->emitter = ov07_0221C494(system, ctx->emitterID);
    ctx->particleSys = ov07_0221C478(system);

    if (ctx->mode == 0) {
        ctx->startBattler = ov07_0221C468(system);
        ctx->endBattler = ov07_0221C468(system);
    } else {
        ctx->startBattler = ov07_0221C470(system);
        ctx->endBattler = ov07_0221C470(system);
    }

    if (ctx->emitter == 0L) {
        ((0) ? (void)0 : GF_AssertFail());
    }

    VecFx32 startPos, endPos;
    ov07_02231B90(system, ctx->startBattler, &startPos);
    ov07_02231B90(system, ctx->endBattler, &endPos);

    if (type == 0) {
        ov07_02231DD0(&startPos);
        startPos.x = endPos.x;
    } else {
        ov07_02231DD0(&endPos);
        endPos.x = startPos.x;
    }

    ov07_02222268(
        &ctx->transforms[0],
        ((startPos.x) / 172),
        ((endPos.x) / 172),
        ((startPos.y) / 172),
        ((endPos.y) / 172),
        ctx->frames);

    for (int i = 0; i < ctx->skipFrames; i++) {
        ov07_022222B4(&ctx->transforms[0]);
    }

    if (ctx->maxFrames != 0xFF) {
        ctx->frame = ctx->maxFrames + 1;
    }

    SPLEmitter_SetPosX(ctx->emitter, ((ctx->transforms[0].x) * 172));
    SPLEmitter_SetPosY(ctx->emitter, ((ctx->transforms[0].y) * 172));

    ov07_0221C410(ctx->common.battleAnimSys, ov07_022260F8, ctx);
}

BOOL ov07_02222384(XYTransformContext *ctx, XYTransformContext *revs);

static void ov07_0222647C(SysTask *task, void *param);

static void ov07_0222647C(SysTask *task, void *param) {
    EmitterAnimationContext *ctx = param;
    BOOL emitterActive = ov07_022260DC(ctx->emitter);

    if (ov07_02222384(&ctx->transforms[0], &ctx->transforms[1]) == 0 && emitterActive == 0) {
        sub_020154C4(ctx->particleSys, ctx->emitter);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        ov07_02232508(ctx);
        ctx = 0L;
        return;
    } else {
        SPLEmitter_SetPosX(ctx->emitter, ((ctx->transforms[0].x) * 172));
        SPLEmitter_SetPosY(ctx->emitter, ((ctx->transforms[0].y) * 172));
    }
}

void ov07_02222338(XYTransformContext *linear, XYTransformContext *revs, s16 sx, s16 ex, s16 sy, s16 ey, u16 frames, fx32 arcRadius);

void ov07_022264E0(BattleAnimSystem *system);

void ov07_022264E0(BattleAnimSystem *system) {
    EmitterAnimationContext *ctx = ov07_022324D8(system, sizeof(EmitterAnimationContext));

    ov07_02231FE4(system, &ctx->common);
    ov07_0222600C(system, ctx);

    int dir = ov07_02222004(system, ov07_0221C468(system));

    VecFx32 startPos, endPos;
    ov07_02231B90(system, ctx->startBattler, &startPos);
    ov07_02231B90(system, ctx->endBattler, &endPos);
    ov07_02222338(
        &ctx->transforms[0],
        &ctx->transforms[1],
        ((startPos.x) / 172),
        ((endPos.x) / 172) + (ctx->offset.x * dir),
        ((startPos.y) / 172),
        ((endPos.y) / 172) + (ctx->offset.y * dir),
        ctx->frames,
        ctx->radius * -((fx32)0x0000000000001000L));

    if (dir > 0) {
        ov07_022223F0(&ctx->unused, (((20) * 0xFFFF) / 360) * dir, (((130) * 0xFFFF) / 360) * dir, 10);
    } else {
        ov07_022223F0(&ctx->unused, (((90) * 0xFFFF) / 360) * dir, (((130) * 0xFFFF) / 360) * dir, 10);
    }

    for (int i = 0; i < ctx->skipFrames; i++) {
        ov07_02222384(&ctx->transforms[0], &ctx->transforms[1]);
    }

    if (ctx->maxFrames != 0xFF) {
        ctx->frame = ctx->maxFrames + 1;
    }

    SPLEmitter_SetPosX(ctx->emitter, ((ctx->transforms[0].x) * 172));
    SPLEmitter_SetPosY(ctx->emitter, ((ctx->transforms[0].y) * 172));

    ov07_0221C410(ctx->common.battleAnimSys, ov07_0222647C, ctx);
}

BOOL ov07_02222180(XYTransformContext *ctx);

typedef struct EmitterRevolutionContext {
    int emitterID;
    int sx;
    int sy;
    int ex;
    int ey;
    int rx;
    int ry;
    int frames;
    int mode;
    int battler;
    int unused0[3];
    ParticleSystem *particleSys;
    SPLEmitter *emitter;
    BattleAnimScriptFuncCommon common;
    XYTransformContext revs;
    VecFx32 sLotoId;
    VecFx32 battlerPos;
} EmitterRevolutionContext;

static void ov07_02226628(SysTask *task, void *param);

static void ov07_02226628(SysTask *task, void *param) {
    EmitterRevolutionContext *ctx = param;
    BOOL emitterActive = ov07_022260DC(ctx->emitter);

    if (ov07_02222180(&ctx->revs) == 0 && emitterActive == 0) {
        sub_020154C4(ctx->particleSys, ctx->emitter);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
    } else {
        VecFx32 emitterPos;
        emitterPos.x = ctx->battlerPos.x + ((ctx->revs.x) * 172);
        emitterPos.y = ctx->battlerPos.y + ((ctx->revs.y) * 172);

        SPLEmitter_SetPosX(ctx->emitter, emitterPos.x);
        SPLEmitter_SetPosY(ctx->emitter, emitterPos.y);
    }
}

void ov07_022220FC(XYTransformContext *ctx, u16 sx, u16 ex, u16 sy, u16 ey, fx32 rx, fx32 ry, int steps);

void ov07_02226698(BattleAnimSystem *system);

void ov07_02226698(BattleAnimSystem *system) {
    EmitterRevolutionContext *ctx = ov07_022324D8(system, sizeof(EmitterRevolutionContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->emitterID = ov07_0221C4A8(system, 0);
    ctx->sx = ov07_0221C4A8(system, 1);
    ctx->ex = ov07_0221C4A8(system, 2);
    ctx->sy = ov07_0221C4A8(system, 3);
    ctx->ey = ov07_0221C4A8(system, 4);
    ctx->rx = ov07_0221C4A8(system, 5);
    ctx->ry = ov07_0221C4A8(system, 6);
    ctx->frames = ov07_0221C4A8(system, 7);
    ctx->mode = ov07_0221C4A8(system, 8);

    if (ctx->mode == 0) {
        ctx->battler = ov07_0221C468(system);
    } else {
        ctx->battler = ov07_0221C470(system);
    }

    ov07_02231B90(system, ctx->battler, &ctx->battlerPos);

    ctx->emitter = ov07_0221C494(system, ctx->emitterID);
    ctx->particleSys = ov07_0221C488(system, ov07_0221C4A8(system, 9));

    ov07_022220FC(
        &ctx->revs,
        (((ctx->sx) * 0xFFFF) / 360),
        (((ctx->ex) * 0xFFFF) / 360),
        (((ctx->sy) * 0xFFFF) / 360),
        (((ctx->ey) * 0xFFFF) / 360),
        ctx->rx * ((fx32)0x0000000000001000L),
        ctx->ry * ((fx32)0x0000000000001000L),
        ctx->frames);

    ov07_02222180(&ctx->revs);

    VecFx32 startPos;
    startPos.x = ctx->battlerPos.x + ((ctx->revs.x) * 172);
    startPos.y = ctx->battlerPos.y + ((ctx->revs.y) * 172);

    SPLEmitter_SetPosX(ctx->emitter, startPos.x);
    SPLEmitter_SetPosY(ctx->emitter, startPos.y);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02226628, ctx);
}

typedef struct OffsetAndAnimateSpriteContext {
    BattleAnimScriptFuncCommon common;
    ManagedSprite *sprite;
} OffsetAndAnimateSpriteContext;

static void ov07_02226804(SysTask *task, void *param);

static void ov07_02226804(SysTask *task, void *param) {
    OffsetAndAnimateSpriteContext *ctx = param;

    if (!ManagedSprite_IsAnimated(ctx->sprite)) {
        Sprite_DeleteAndFreeResources(ctx->sprite);
        ov07_0221C448(ctx->common.battleAnimSys, task);
        Heap_Free(ctx);
        return;
    }

    ManagedSprite_TickFrame(ctx->sprite);
    SpriteSystem_DrawSprites(ctx->common.primarySpriteManager);
}

void ov07_02226838(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite);

void ov07_02226838(BattleAnimSystem *system, SpriteSystem *spriteSys, SpriteManager *spriteMan, ManagedSprite *sprite) {
    OffsetAndAnimateSpriteContext *ctx = ov07_022324D8(system, sizeof(OffsetAndAnimateSpriteContext));
    ov07_02231FE4(system, &ctx->common);

    ctx->sprite = sprite;

    s16 x = ov07_0221C4A8(system, 0);
    s16 y = ov07_0221C4A8(system, 1);
    ManagedSprite_OffsetPositionXY(sprite, x, y);

    ov07_0221C410(ctx->common.battleAnimSys, ov07_02226804, ctx);
}
