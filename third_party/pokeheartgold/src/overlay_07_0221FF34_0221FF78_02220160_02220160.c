#include "global.h"

#include "battle/battler_sprite_context.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sprite_resources_header.h"
#include "overlay_07_battle_anim_system.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "bg_window.h"
#include "palette.h"
#include "pokepic.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "camera.h"
#include "particle_system.h"
#include "overlay_07_bg_scroll_context.h"
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

struct ManagedSprite;

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

enum CameraProjection {
    CAMERA_PROJECTION_PERSPECTIVE = 0,
    CAMERA_PROJECTION_ORTHOGRAPHIC,
    CAMERA_PROJECTION_COUNT
};

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

extern void *sub_02015504(void);

extern u8 sub_02015530(ParticleSystem *particleSystem);

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

extern BOOL ov07_0221BFC0(BattleAnimSystem *system);

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

extern void ov07_02231AD0(int battler, VecFx32 *pos, BOOL isContest, enum CameraProjection projection);

void ov07_02220160(SPLEmitter *emitter);

void ov07_02220160(SPLEmitter *emitter) {
    BattleAnimSystem *system = sub_02015504();
    ParticleSystem *ps = ov07_0221C478(system);
    enum CameraProjection projection = (enum CameraProjection)(sub_02015530(ps));
    int attacker = ov07_0221C468(system);
    int attackerSide = ov07_0223192C(system, attacker);

    int battler = (attackerSide == BTLSCR_PLAYER) ? 0 : 1;

    VecFx32 pos;
    ov07_02231AD0(battler, &pos, ov07_0221BFC0(system), projection);

    SPL_SetEmitterPositionX(emitter, pos.x);
    SPL_SetEmitterPositionY(emitter, pos.y);
    SPL_SetEmitterPositionZ(emitter, pos.z);
}

void ov07_022201C8(SPLEmitter *emitter);

void ov07_022201C8(SPLEmitter *emitter) {
    BattleAnimSystem *system = sub_02015504();
    ParticleSystem *ps = ov07_0221C478(system);
    enum CameraProjection projection = (enum CameraProjection)(sub_02015530(ps));
    int attacker = ov07_0221C470(system);
    int attackerSide = ov07_0223192C(system, attacker);

    int battler = (attackerSide == BTLSCR_PLAYER) ? 0 : 1;

    VecFx32 pos;
    ov07_02231AD0(battler, &pos, ov07_0221BFC0(system), projection);

    SPL_SetEmitterPositionX(emitter, pos.x);
    SPL_SetEmitterPositionY(emitter, pos.y);
    SPL_SetEmitterPositionZ(emitter, pos.z);
}
