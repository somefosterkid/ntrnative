#include "global.h"

#include "battle/battler_sprite_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_system.h"
#include "overlay_07_battle_anim_system.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_07_battle_anim_common.h"
#include "bg_window.h"
#include "pokepic.h"
#include "overlay_41_sprite_resource_collection.h"
#include "camera.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "particle_system.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_value_lerp_context.h"
#include "sav_chatot.h"
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_anim_tracking.h"

struct KinesisSpoon;

struct ManagedSprite;

struct ValueLerpContext;

struct XYTransformContext;

enum XYTransformParam {
    XY_PARAM_STEPS = 0,
    XY_PARAM_STEP_SIZE,
    XY_PARAM_STEP_SIZE_X = 1,
    XY_PARAM_STEP_SIZE_Y,
    XY_PARAM_CUR_X,
    XY_PARAM_CUR_Y,

    XY_PARAM_SHAKE_COUNTER = 0,
    XY_PARAM_SHAKE_INTERVAL,
    XY_PARAM_SHAKE_REMAINING,
    XY_PARAM_SHAKE_CYCLE,
    XY_PARAM_SHAKE_EXTENT_X,
    XY_PARAM_SHAKE_EXTENT_Y,
    XY_PARAM_SHAKE_PREV_X,
    XY_PARAM_SHAKE_PREV_Y,

    XY_PARAM_REV_STEPS = 0,
    XY_PARAM_REV_CUR_X,
    XY_PARAM_REV_RADIUS_X,
    XY_PARAM_REV_CUR_Y,
    XY_PARAM_REV_RADIUS_Y,
    XY_PARAM_REV_STEP_SIZE_X,
    XY_PARAM_REV_STEP_SIZE_Y,
};

void ov07_0222212C(XYTransformContext *ctx, u16 sx, u16 ex, u16 sy, u16 ey, fx32 rx, fx32 ry, u16 stepSizeX);

BOOL ov07_02222180(XYTransformContext *ctx);

void ov07_02222494(ValueLerpContext *ctx, u16 start, u16 end, fx32 amplitude, u32 steps);

BOOL ov07_022224AC(ValueLerpContext *ctx);

typedef struct KinesisSpoon {
    ManagedSprite *sprite;
    XYTransformContext revs;
    ValueLerpContext cosWave;
    s16 revStepSizeX;
    s16 posX;
    s16 posY;
    int state;
    BOOL active;
} KinesisSpoon;

enum KinesisSpoonState {
    KINESIS_SPOON_STATE_INIT_REVS = 0,
    KINESIS_SPOON_STATE_REV,
    KINESIS_SPOON_STATE_DONE,
};

BOOL ov07_0222C214(KinesisSpoon *spoon);

BOOL ov07_0222C214(KinesisSpoon *spoon)
{
    if (spoon->active == 0 ) {
        return 1 ;
    }

    switch (spoon->state) {
    case KINESIS_SPOON_STATE_INIT_REVS:
    case KINESIS_SPOON_STATE_REV:
        if (ov07_02222180(&spoon->revs)) {
            ov07_022224AC(&spoon->cosWave);
            spoon->revs.data[XY_PARAM_REV_STEP_SIZE_X] = spoon->revStepSizeX + spoon->cosWave.value;
            ManagedSprite_SetPositionXY(spoon->sprite, spoon->posX + spoon->revs.x, spoon->posY + spoon->revs.y);
        } else {
            if (spoon->state < KINESIS_SPOON_STATE_REV) {
                if (spoon->state == KINESIS_SPOON_STATE_INIT_REVS) {
                    ov07_0222212C(
                        &spoon->revs,
                        spoon->revs.data[XY_PARAM_REV_CUR_X],
                        (((180) * 0xFFFF) / 360) ,
                        spoon->revs.data[XY_PARAM_REV_CUR_X],
                        (((180) * 0xFFFF) / 360) ,
                        spoon->revs.data[XY_PARAM_REV_RADIUS_X],
                        spoon->revs.data[XY_PARAM_REV_RADIUS_Y],
                        (((10) * 0xFFFF) / 360) );
                }

                ov07_02222494(
                    &spoon->cosWave,
                    (((0) * 0xFFFF) / 360) ,
                    (((360) * 0xFFFF) / 360) ,
                    -((fx32) 0x0000000000001000L) ,
                    spoon->revs.data[XY_PARAM_REV_STEPS]);
            }

            spoon->state++;
        }
        break;
    case KINESIS_SPOON_STATE_DONE:
        return 0 ;
    }

    return 1 ;
}

struct BattleAnimContext;

struct BattleAnimScriptLoop;

struct BattleAnimSystem;

struct BattleBackgroundReference;

struct BattleBgAnim;

struct BgConfig;

struct BgTrackingTask;

struct BgWaveStrip;

struct CameraPerspective;

struct CameraPositionHistory;

struct SOUND_CHATOT;

struct CustomBgScrollContext;

struct KinesisSpriteContext;

struct SelectedPaletteData;

struct ParticleSystem;

struct Pokepic;

struct PokemonSpriteDrawContext;

struct PokepicShadow;

struct PokemonSpriteTrackingTask;

struct PokepicDrawParam;

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

struct SpriteAnimFrame;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SpriteTrackingTask;

struct SysTask;

struct SysTaskQueue;

struct WaveAnimContext;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;

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
    u8 textures[8 ];
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

typedef struct BattleAnimScriptLoop {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
} BattleAnimScriptLoop;

typedef struct PokemonSpriteDrawContext {
    struct BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
} PokemonSpriteDrawContext;

typedef struct KinesisSpriteContext {
    BattleAnimSystem *battleAnimSys;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    KinesisSpoon spoon;
    KinesisSpoon extraSpoons[2 ];
    int state;
    int delay;
    int activeSpoon;
    int spoonAlpha;
} KinesisSpriteContext;

enum KinesisSpriteState {
    KINESIS_SPRITE_STATE_INIT = 0,
    KINESIS_SPRITE_STATE_FADE_IN_SPOONS,
    KINESIS_SPRITE_STATE_FIX_ALPHA,
    KINESIS_SPRITE_STATE_MOVE_SPOONS,
    KINESIS_SPRITE_STATE_ANIMATE_SPOON,
    KINESIS_SPRITE_STATE_FADE_OUT_SPOONS,
    KINESIS_SPRITE_STATE_CLEANUP,
};

void ov07_0222C1FC(KinesisSpoon *spoon);

void ov07_0222C208(KinesisSpoon *spoon, BOOL draw);

void ov07_0222C2D4(SysTask *task, void *param);

void ov07_0222C2D4(SysTask *task, void *param)
{
    KinesisSpriteContext *ctx = param;
    int i;

    switch (ctx->state) {
    case KINESIS_SPRITE_STATE_INIT:
        ov07_0222C214(&ctx->spoon);
        ctx->state++;
        break;
    case KINESIS_SPRITE_STATE_FADE_IN_SPOONS:
        ctx->spoonAlpha += 1 * 10 ;
        G2_ChangeBlendAlpha(
            ctx->spoonAlpha / 10 ,
            31 - (ctx->spoonAlpha / 10 ));

        if (ctx->spoonAlpha >= 31 * 10 ) {
            ManagedSprite_SetOamMode(ctx->spoon.sprite, GX_OAM_MODE_NORMAL);

            ctx->state++;
        }
        break;
    case KINESIS_SPRITE_STATE_FIX_ALPHA:
        G2_ChangeBlendAlpha(-1 , -1 );
        ctx->state++;
        break;
    case KINESIS_SPRITE_STATE_MOVE_SPOONS: {
        if (ctx->delay <= 0) {
            ctx->extraSpoons[ctx->activeSpoon].active = 1 ;
            ManagedSprite_SetDrawFlag(ctx->extraSpoons[ctx->activeSpoon].sprite, 1 );
            ctx->activeSpoon++;
            ctx->delay = 8 ;
        }

        if (ctx->activeSpoon < 2 ) {
            ctx->delay--;
        }

        BOOL anyActive = ov07_0222C214(&ctx->spoon);

        for (i = 0; i < 2 ; i++) {
            BOOL active = ov07_0222C214(&ctx->extraSpoons[i]);
            if (active == 0 ) {
                ov07_0222C208(&ctx->extraSpoons[i], 0 );
            }

            anyActive |= active;
        }

        if (anyActive == 0 ) {
            ctx->state++;
        }
        break;
    }
    case KINESIS_SPRITE_STATE_ANIMATE_SPOON:
        ManagedSprite_TickFrame(ctx->spoon.sprite);

        if (Sprite_IsAnimated(ctx->spoon.sprite->sprite) == 0 ) {
            ManagedSprite_SetOamMode(ctx->spoon.sprite, GX_OAM_MODE_XLU);
            ctx->state++;
        }
        break;
    case KINESIS_SPRITE_STATE_FADE_OUT_SPOONS:
        ctx->spoonAlpha -= 1 * 10 ;
        G2_ChangeBlendAlpha(
            ctx->spoonAlpha / 10 ,
            31 - (ctx->spoonAlpha / 10 ));

        if (ctx->spoonAlpha <= 0) {
            ctx->state++;
        }
        break;
    case KINESIS_SPRITE_STATE_CLEANUP:
        for (i = 0; i < 2 ; i++) {
            ov07_0222C1FC(&ctx->extraSpoons[i]);
        }

        Sprite_DeleteAndFreeResources(ctx->spoon.sprite);
        ov07_0221C448(ctx->battleAnimSys, task);
        Heap_Free(ctx);

        return;
    }

    SpriteSystem_DrawSprites(ctx->spriteMan);
}
