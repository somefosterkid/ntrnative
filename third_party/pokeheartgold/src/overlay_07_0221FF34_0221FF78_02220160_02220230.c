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

void *sub_02015504(void);

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

u8 ov07_02231924(BattleAnimSystem *system, int battler);

void ov07_02220230(SPLEmitter *emitter);

static s16 hgasset_ov07_02220230_posTableX[6][6] __attribute__((section(BWPORT_DATA_SECTION))) = {

        { 0,    0,    0, 0, 0, 0 },
        { 0,    0,    0, 0, 0, 0 },
        { 0, 0, 0, -0,  0, -0  },
        { 0, 0, +0,  0, +0,  0 },
        { 0, 0, 0, +0,  0, +0   },
        { 0, 0, -0,  0, 0,    0 }
    };

static s16 hgasset_ov07_02220230_posTableY[6][6] __attribute__((section(BWPORT_DATA_SECTION))) = {

        { 0,    0,    0, 0, 0, 0 },
        { 0,    0,    0, 0, 0, 0 },
        { 0, 0, 0, +0,  0, +0  },
        { 0, 0, 0,    0, 0,    0 },
        { 0, 0, 0, 0,    0, 0    },
        { 0, 0, +0,  0, +0,  0 }
    };

void ov07_02220230(SPLEmitter *emitter) {
    s16 unused[6] = { 0, 0, 0, 0, 0, 0 };

    __typeof__(hgasset_ov07_02220230_posTableX) posTableX; __builtin_memcpy((void *)&posTableX, &hgasset_ov07_02220230_posTableX, sizeof(posTableX));
    __typeof__(hgasset_ov07_02220230_posTableY) posTableY; __builtin_memcpy((void *)&posTableY, &hgasset_ov07_02220230_posTableY, sizeof(posTableY));

    BattleAnimSystem *system = sub_02015504();
    int attacker = ov07_0221C468(system);
    int defender = ov07_0221C470(system);
    int attackerType = ov07_02231924(system, attacker);
    int defenderType = ov07_02231924(system, defender);
    int posX = ((posTableX[attackerType][defenderType]) * 172);
    int posY = ((posTableY[attackerType][defenderType]) * 172);

    VecFx32 pos;
    VEC_Set(&pos, posX, posY, 0);

    SPL_SetEmitterPositionX(emitter, pos.x);
    SPL_SetEmitterPositionY(emitter, pos.y);
    SPL_SetEmitterPositionZ(emitter, pos.z);
}

// tools/hg_assets.py: the tables above are blanks, port/src/asset_load.c fills them from the ROM at start
void PORT_AssetAdd(const char *game, const char *name, void *p, unsigned long size, const char *layout);
__attribute__((constructor)) static void hgasset_register_src_overlay_07_0221FF34_0221FF78_02220160_02220230_c_0(void) {
    PORT_AssetAdd("IPKE", "src/overlay_07_0221FF34_0221FF78_02220160_02220230.c:hgasset_ov07_02220230_posTableX", (void *)&hgasset_ov07_02220230_posTableX, sizeof(hgasset_ov07_02220230_posTableX), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_07_0221FF34_0221FF78_02220160_02220230.c:hgasset_ov07_02220230_posTableX", (void *)&hgasset_ov07_02220230_posTableX, sizeof(hgasset_ov07_02220230_posTableX), "u8");
    PORT_AssetAdd("IPKE", "src/overlay_07_0221FF34_0221FF78_02220160_02220230.c:hgasset_ov07_02220230_posTableY", (void *)&hgasset_ov07_02220230_posTableY, sizeof(hgasset_ov07_02220230_posTableY), "u8");
    PORT_AssetAdd("IPGE", "src/overlay_07_0221FF34_0221FF78_02220160_02220230.c:hgasset_ov07_02220230_posTableY", (void *)&hgasset_ov07_02220230_posTableY, sizeof(hgasset_ov07_02220230_posTableY), "u8");
}
