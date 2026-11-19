#include "global.h"

#include "battle/battler_sprite_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_resources_header.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "pokepic.h"
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
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_battle_anim_common.h"
#include "sav_chatot.h"
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_anim_tracking.h"

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

struct DoubleTeamContext;

struct ManagedSprite;

struct PaletteData;

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

struct Sprite;

struct SpriteAnimFrame;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SpriteTrackingTask;

struct SysTask;

struct SysTaskQueue;

struct WaveAnimContext;

struct XYTransformContext;

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};

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

NNSG2dImagePaletteProxy *Sprite_GetPaletteProxy(Sprite *paletteProxy);

extern void ManagedSprite_SetPriority(ManagedSprite *managedSprite, int priority);

void ManagedSprite_SetOamMode(ManagedSprite *managedSprite, GXOamMode mode);

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

extern enum HeapID ov07_0221BFD0(BattleAnimSystem *system);

extern int ov07_0221FAE8(BattleAnimSystem *system);

enum Battler {
    BTLSCR_ALL_BATTLERS = 0,
    BTLSCR_ATTACKER = 1,
    BTLSCR_DEFENDER = 2,
    BTLSCR_PLAYER = 3,
    BTLSCR_ENEMY = 4,
    BTLSCR_FAINTED_MON = 5,
    BTLSCR_SWITCHED_MON = 6,
    BTLSCR_SIDE_EFFECT_MON = 7,
    BTLSCR_ABILITY_MON = 8,
    BTLSCR_PLAYER_SLOT_1 = 9,
    BTLSCR_ENEMY_SLOT_1 = 10,
    BTLSCR_PLAYER_SLOT_2 = 11,
    BTLSCR_ENEMY_SLOT_2 = 12,
    BTLSCR_UNUSED_13 = 13,
    BTLSCR_MSG_ATTACKER = 14,
    BTLSCR_MSG_DEFENDER = 15,
    BTLSCR_ATTACKER_PARTNER = 16,
    BTLSCR_DEFENDER_PARTNER = 17,
    BTLSCR_FORCED_OUT = 18,
    BTLSCR_ATTACKER_ENEMY = 19,
    BTLSCR_DEFENDER_ENEMY = 20,
    BTLSCR_MSG_BATTLER_TEMP = 21,
    BTLSCR_SWITCHED_MON_AFTER = 22,
};

extern enum Battler ov07_0223192C(BattleAnimSystem *system, int battler);

extern void ov07_02231E08(BattleAnimSystem *system, int spriteAlpha, int bgAlpha);

u32 ObjPlttTransfer_GetPaletteVramOffset(const NNSG2dImagePaletteProxy *paletteProxy, NNS_G2D_VRAM_TYPE vramType);

typedef struct DoubleTeamContext {
    u8 iteration;
    u8 state;
    u16 sLotoId;
    u8 blendA;
    u8 blendB;
    u16 unused2;
    s16 attackerX;
    s16 attackerY;
    BattleAnimSystem *battleAnimSys;
    Pokepic *attackerSprite;
    SpriteManager *pokemonSpriteManager;
    ManagedSprite *monSprites[4];
    XYTransformContext pos[4];
} DoubleTeamContext;

extern void ov07_02227B58(SysTask *task, void *param);

void ov07_02227D1C(BattleAnimSystem *system);

void ov07_02227D1C(BattleAnimSystem *system) {
    DoubleTeamContext *ctx = Heap_Alloc(ov07_0221BFD0(system), sizeof(DoubleTeamContext));

    ctx->state = 0;
    ctx->iteration = 0;
    ctx->battleAnimSys = system;
    ctx->attackerSprite = ov07_0221FA48(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    ctx->attackerX = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_X_CENTER);
    ctx->attackerY = Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_Y_CENTER);
    ctx->attackerY -= Pokepic_GetAttr(ctx->attackerSprite, MON_SPRITE_SHADOW_HEIGHT);
    ctx->pokemonSpriteManager = ov07_0221C514(ctx->battleAnimSys);
    ctx->blendA = 8;
    ctx->blendB = 6;

    ov07_02231E08(ctx->battleAnimSys, -1, -1);
    G2_ChangeBlendAlpha(ctx->blendA, ctx->blendB);

    int monPlttIndex = ov07_0221FA80(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys));
    int monSpritePriority = ov07_0221FAE8(ctx->battleAnimSys);
    enum NarcId monSpriteNarcIndex = (enum NarcId)(ov07_0221FA90(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys)));

    for (int i = 0; i < 4; i++) {
        ctx->monSprites[i] = ov07_0221C4E8(ctx->battleAnimSys, i);
        ManagedSprite_SetOamMode(ctx->monSprites[i], GX_OAM_MODE_XLU);
    }

    if (ov07_0223192C(ctx->battleAnimSys, ov07_0221C468(ctx->battleAnimSys)) == BTLSCR_PLAYER) {

        ManagedSprite_SetDrawPriority(ctx->monSprites[0], 10);
        ManagedSprite_SetDrawPriority(ctx->monSprites[1], 10);
        ManagedSprite_SetDrawPriority(ctx->monSprites[2], 20);
        ManagedSprite_SetDrawPriority(ctx->monSprites[3], 20);
        ManagedSprite_SetPriority(ctx->monSprites[0], monSpritePriority);
        ManagedSprite_SetPriority(ctx->monSprites[1], monSpritePriority);
        ManagedSprite_SetPriority(ctx->monSprites[2], monSpritePriority);
        ManagedSprite_SetPriority(ctx->monSprites[3], monSpritePriority);

        u16 offset = ObjPlttTransfer_GetPaletteVramOffset(
            Sprite_GetPaletteProxy(ctx->monSprites[0]->sprite),
            NNS_G2D_VRAM_TYPE_2DMAIN);

        PaletteData_LoadNarc_CustomTint(
            ov07_0221FA78(ctx->battleAnimSys),
            monSpriteNarcIndex,
            monPlttIndex,
            ov07_0221BFD0(system),
            PLTTBUF_MAIN_OBJ,
            ((16) * sizeof(u16)),
            ((offset) * (16)),
            128,
            128,
            128);

        offset = ObjPlttTransfer_GetPaletteVramOffset(
            Sprite_GetPaletteProxy(ctx->monSprites[2]->sprite),
            NNS_G2D_VRAM_TYPE_2DMAIN);

        PaletteData_LoadNarc_CustomTint(
            ov07_0221FA78(ctx->battleAnimSys),
            monSpriteNarcIndex,
            monPlttIndex,
            ov07_0221BFD0(system),
            PLTTBUF_MAIN_OBJ,
            ((16) * sizeof(u16)),
            ((offset) * (16)),
            196,
            196,
            196);
    } else {

        ManagedSprite_SetDrawPriority(ctx->monSprites[0], 20);
        ManagedSprite_SetDrawPriority(ctx->monSprites[1], 20);
        ManagedSprite_SetDrawPriority(ctx->monSprites[2], 10);
        ManagedSprite_SetDrawPriority(ctx->monSprites[3], 10);
        ManagedSprite_SetPriority(ctx->monSprites[0], monSpritePriority + 1);
        ManagedSprite_SetPriority(ctx->monSprites[1], monSpritePriority + 1);
        ManagedSprite_SetPriority(ctx->monSprites[2], monSpritePriority + 1);
        ManagedSprite_SetPriority(ctx->monSprites[3], monSpritePriority + 1);
        ManagedSprite_SetPriority(ctx->monSprites[0], monSpritePriority);
        ManagedSprite_SetPriority(ctx->monSprites[1], monSpritePriority);
        ManagedSprite_SetPriority(ctx->monSprites[2], monSpritePriority);
        ManagedSprite_SetPriority(ctx->monSprites[3], monSpritePriority);

        u16 offset = ObjPlttTransfer_GetPaletteVramOffset(
            Sprite_GetPaletteProxy(ctx->monSprites[0]->sprite),
            NNS_G2D_VRAM_TYPE_2DMAIN);

        PaletteData_LoadNarc_CustomTint(
            ov07_0221FA78(ctx->battleAnimSys),
            monSpriteNarcIndex,
            monPlttIndex,
            ov07_0221BFD0(system),
            PLTTBUF_MAIN_OBJ,
            ((16) * sizeof(u16)),
            ((offset) * (16)),
            196,
            196,
            196);

        offset = ObjPlttTransfer_GetPaletteVramOffset(
            Sprite_GetPaletteProxy(ctx->monSprites[2]->sprite),
            NNS_G2D_VRAM_TYPE_2DMAIN);

        PaletteData_LoadNarc_CustomTint(
            ov07_0221FA78(ctx->battleAnimSys),
            monSpriteNarcIndex,
            monPlttIndex,
            ov07_0221BFD0(system),
            PLTTBUF_MAIN_OBJ,
            ((16) * sizeof(u16)),
            ((offset) * (16)),
            128,
            128,
            128);
    }

    ov07_0221C410(ctx->battleAnimSys, ov07_02227B58, ctx);
}
