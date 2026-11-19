#include "global.h"

#include "battle/battler_sprite_context.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "bg_window.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "overlay_07_ball_rotation.h"
#include "pokepic.h"
#include "unk_02013534.h"
#include "item.h"
#include "camera.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "particle_system.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_battle_anim_system.h"
#include "battle/battle.h"
#include "battle/battle_message_structs.h"
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_anim_tracking.h"

struct AIContext;

struct UnkBallData;

struct BattleAnimContext;

struct BattleAnimScriptLoop;

struct BattleAnimSystem;

struct BattleBackgroundReference;

struct BattleBgAnim;

struct BattleContext;

struct BattleMessage;

struct BattleMon;

struct BattleMonOBJData;

struct BattleScriptTaskData;

struct BattleSystem;

struct OpponentData;

struct OpponentDataSysTaskFuncs;

struct BgConfig;

struct BgTrackingTask;

struct BgWaveStrip;

struct CameraPerspective;

struct CameraPositionHistory;

struct CharTransferAllocation;

struct SOUND_CHATOT;

struct CustomBgScrollContext;

struct FieldConditions;

struct TextOBJ;

struct BattleHpBar;

struct ItemData;

struct ManagedSprite;

struct MoveAnimation;

struct MoveContestData;

struct MoveEffectsData;

struct MoveFailFlags;

struct MoveTable;

struct NARC;

struct PaletteData;

struct SelectedPaletteData;

struct ParticleSystem;

struct Pokepic;

struct PokemonSpriteDrawContext;

struct PokepicManager;

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

struct OpponentDataCursorPos;

struct SelfTurnFlags;

struct SetMoveAnimationData;

struct SideConditions;

struct SpriteAnimFrame;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SpriteSystemContext;

struct SpriteTrackingTask;

struct SysTask;

struct SysTaskQueue;

struct TurnFlags;

struct WaveAnimContext;

struct Window;

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

typedef struct SpriteSystemContext {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;
} SpriteSystemContext;

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER               =  0,
    MON_SPRITE_Y_CENTER               =  1,
    MON_SPRITE_Z_CENTER               =  2,
    MON_SPRITE_X_OFFSET               =  3,
    MON_SPRITE_Y_OFFSET               =  4,
    MON_SPRITE_Z_OFFSET               =  5,
    MON_SPRITE_HIDE                   =  6,
    MON_SPRITE_ROTATION_X             =  7,
    MON_SPRITE_ROTATION_Y             =  8,
    MON_SPRITE_ROTATION_Z             =  9,
    MON_SPRITE_X_PIVOT                = 10,
    MON_SPRITE_Y_PIVOT                = 11,
    MON_SPRITE_SCALE_X                = 12,
    MON_SPRITE_SCALE_Y                = 13,
    MON_SPRITE_PARTIAL_DRAW           = 14,
    MON_SPRITE_DRAW_X_OFFSET          = 15,
    MON_SPRITE_DRAW_Y_OFFSET          = 16,
    MON_SPRITE_DRAW_WIDTH             = 17,
    MON_SPRITE_DRAW_HEIGHT            = 18,
    MON_SPRITE_SHADOW_X               = 19,
    MON_SPRITE_SHADOW_Y               = 20,
    MON_SPRITE_SHADOW_X_OFFSET        = 21,
    MON_SPRITE_SHADOW_Y_OFFSET        = 22,
    MON_SPRITE_ALPHA                  = 23,
    MON_SPRITE_DIFFUSE_R              = 24,
    MON_SPRITE_DIFFUSE_G              = 25,
    MON_SPRITE_DIFFUSE_B              = 26,
    MON_SPRITE_AMBIENT_R              = 27,
    MON_SPRITE_AMBIENT_G              = 28,
    MON_SPRITE_AMBIENT_B              = 29,
    MON_SPRITE_FADE_ACTIVE            = 30,
    MON_SPRITE_FADE_TARGET_COLOR      = 31,
    MON_SPRITE_FADE_INIT_ALPHA        = 32,
    MON_SPRITE_FADE_TARGET_ALPHA      = 33,
    MON_SPRITE_FADE_DELAY_COUNTER     = 34,
    MON_SPRITE_FLIP_H                 = 35,
    MON_SPRITE_FLIP_V                 = 36,
    MON_SPRITE_HIDE_2                 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME      = 38,
    MON_SPRITE_DUMMY                  = 39,
    MON_SPRITE_MOSAIC_INTENSITY       = 40,
    MON_SPRITE_SHADOW_HEIGHT          = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT       = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE       = 45,
    MON_SPRITE_SHADOW_SIZE            = 46,
};

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

void PokepicManager_SetG3UpdateFlagsMask(PokepicManager *monSpriteMan, u32 value);

void PokepicManager_ResetG3UpdateFlagsMask(PokepicManager *monSpriteMan, u32 value);

static inline void PokemonSpriteManager_HideShadows(PokepicManager *monSpriteMan)
{
    PokepicManager_SetG3UpdateFlagsMask(monSpriteMan, 1);
}

static inline void PokemonSpriteManager_ShowShadows(PokepicManager *monSpriteMan)
{
    PokepicManager_ResetG3UpdateFlagsMask(monSpriteMan, 1);
}

typedef struct BattleMonOBJData {
    enum HeapID heapID;
    int battlerIdx;
    SpriteSystemContext spriteContext;
    int resourceIDs[4];
    ManagedSprite *managedSprites[4];
    PokemonSpriteData *pokemonSpriteDataArray[4];
    u8 battlerTypes[4];
    Pokepic *pokemonSprites[4];
} BattleMonOBJData;

enum BattleAction {
    BATTLE_ACTION_PICK_COMMAND     = 0,
    BATTLE_ACTION_CHOOSE_TARGET    = 1,
    BATTLE_ACTION_TEMP_VALUE       = 2,
    BATTLE_ACTION_SELECTED_COMMAND = 3,
    MAX_BATTLE_ACTIONS             = 4,
};


typedef struct MoveContestData {
    u8 effect;
    u8 type;
    u8 padding02[2];
} MoveContestData;

typedef struct MoveTable {
    u16 effect;
    u8 class;
    u8 power;
    u8 type;
    u8 accuracy;
    u8 pp;
    u8 effectChance;
    u16 range;
    s8 priority;
    u8 flags;
    MoveContestData contest;
} MoveTable;


typedef struct MoveEffectsData {
    u32 disabledTurns : 3;
    u32 encoredTurns : 3;
    u32 chargedTurns : 2;
    u32 tauntedTurns : 3;
    u32 protectSuccessTurns : 2;
    u32 perishSongTurns : 2;
    u32 rolloutCount : 3;
    u32 furyCutterCount : 3;
    u32 stockpileCount : 3;
    u32 stockpileDefBoosts : 3;
    u32 stockpileSpDefBoosts : 3;
    u32 truant : 1;
    u32 flashFire : 1;

    u32 lockOnTarget : 2;
    u32 mimickedMoveSlot : 4;
    u32 bindTarget : 2;
    u32 meanLookTarget : 2;
    u32 lastResortCount : 3;
    u32 magnetRiseTurns : 3;
    u32 healBlockTurns : 3;
    u32 embargoTurns : 3;
    u32 canUnburden : 1;
    u32 metronomeTurns : 4;
    u32 micleBerry : 1;
    u32 custapBerry : 1;
    u32 quickClaw : 1;
    u32 meFirst : 1;
    u32 padding0004_1F : 1;

    int rechargeTurnNumber;
    int fakeOutTurnNumber;
    int slowStartTurnNumber;
    int meFirstTurnNumber;
    int substituteHP;
    u32 transformedPID;

    u16 disabledMove;
    u16 bindingMove;
    u16 encoredMove;
    u16 encoredMoveSlot;
    u16 lastResortMoves[4];
    u16 choiceLockedMove;
    u16 transformedGender;

    int itemHPRecovery;
} MoveEffectsData;

typedef struct FieldConditions {
    u32 weatherTurns;
    u8 futureSightTurns[4 ];
    u8 wishTurns[4 ];
    u16 futureSightMove[4 ];
    int futureSightAttacker[4 ];
    s32 futureSightDamage[4 ];
    u8 wishTarget[4 ];
} FieldConditions;

typedef struct SideConditions {
    u32 reflectUser : 2;
    u32 reflectTurns : 3;
    u32 lightScreenUser : 2;
    u32 lightScreenTurns : 3;
    u32 mistUser : 2;
    u32 mistTurns : 3;
    u32 safeguardUser : 2;
    u32 safeguardTurns : 3;
    u32 followMe : 1;
    u32 followMeUser : 2;
    u32 knockedOffItemsMask : 6;
    u32 padding00_1D : 3;

    u32 spikesLayers : 2;
    u32 toxicSpikesLayers : 2;
    u32 padding04_04 : 28;
} SideConditions;

typedef struct TurnFlags {
    u32 struggling : 1;
    u32 ppDecremented : 1;
    u32 protecting : 1;
    u32 helpingHand : 1;
    u32 magicCoat : 1;
    u32 snatching : 1;
    u32 roosting : 1;
    u32 fleeing : 2;
    u32 enduring : 1;
    u32 padding00_0A : 22;

    int physicalDamageTakenFrom[4 ];
    int physicalDamageLastAttacker;
    int physicalDamageAttackerMask;

    int specialDamageTakenFrom[4 ];
    int specialDamageLastAttacker;
    int specialDamageAttackerMask;

    int lastDamageTaken;
    int lastAttacker;
    int assuranceDamageMask;
} TurnFlags;

typedef struct SelfTurnFlags {
    u32 skipPressureCheck : 1;
    u32 lightningRodActivated : 1;
    u32 stormDrainActivated : 1;
    u32 moldBreakerActivated : 1;
    u32 trickRoomActivated : 1;
    u32 focusItemActivated : 1;
    u32 repeatedMoveCount : 3;
    u32 padding00_09 : 23;

    int physicalDamageTaken;
    int physicalDamageLastAttacker;
    int specialDamageTaken;
    int specialDamageLastAttacker;
    int statusFlags;
    int shellBellDamageDealt;
} SelfTurnFlags;

typedef struct BattleScriptTaskData {
    BattleSystem *battleSys;
    BattleContext *battleCtx;
    BallRotation *ballRotation;
    ManagedSprite *sprites[2];
    TextOBJ *fontOAM;
    CharTransferAllocation charTransferAllocation;
    int flag;
    int seqNum;
    int ball;
    int tmpData[8];
    void *tmpPtr[2];
} BattleScriptTaskData;

void SysTask_Destroy(SysTask *task);

typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;

void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);

void BattleController_SetMoveAnimation(BattleSystem *battleSys, BattleContext *battleCtx, MoveAnimation *animation, int param3, int param4, int attacker, int defender, u16 move);

void Heap_Free(void *ptr);

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

BOOL ov07_0221C394(BattleAnimSystem *system);

BOOL ov07_0221C3B0(BattleAnimSystem *system);

BOOL ov07_0221C3C0(BattleAnimSystem *system);

PokepicManager *BattleSystem_GetPokepicManager(BattleSystem *battleSys);

void BattleSystem_SetHpBarEnabled(BattleSystem *battleSys);

void BattleSystem_SetHpBarDisabled(BattleSystem *battleSys);

void BattleSystem_SetCriticalHpMusicFlag(BattleSystem *battleSys, u8 value);

void ov07_0223494C(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);

void ov07_02234A20(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);

void ov12_02261B80(BattleSystem *battleSys, OpponentData *battlerData, BattleAnimSystem *battleAnimSystem, MoveAnimation *animation);

void ov12_02261CA8(BattleSystem *battleSys, MoveAnimation *animation, BattlerSpriteContext *battlerSpriteCtx, int battler);

typedef struct SetMoveAnimationData {
    BattleSystem *battleSys;
    OpponentData *battlerData;
    Pokepic *monSprite;
    BattleAnimSystem *battleAnimSys;
    MoveAnimation moveAnim;
    u8 command;
    u8 battler;
    u8 state;
    u8 hideHealthboxes;
    u8 hideShadows;
    u8 unused[3];
} SetMoveAnimationData;

void ov12_0225FD14(SysTask *task, void *data);

void ov12_0225FD14(SysTask *task, void *data)
{
    SetMoveAnimationData *setMoveAnimationData = data;

    switch (setMoveAnimationData->state) {
    case 0:
        if (setMoveAnimationData->moveAnim.isSubstitute && setMoveAnimationData->moveAnim.animMode == 0 && Pokepic_GetAttr(setMoveAnimationData->monSprite, MON_SPRITE_HIDE) == 0 ) {
            MoveAnimation moveAnim;

            BattleController_SetMoveAnimation(setMoveAnimationData->battleSys, 0L , &moveAnim, 1, 15, setMoveAnimationData->battler, setMoveAnimationData->battler, 0L );
            ov12_02261B80(setMoveAnimationData->battleSys, setMoveAnimationData->battlerData, setMoveAnimationData->battleAnimSys, &moveAnim);
            setMoveAnimationData->state = 1;
        } else {
            setMoveAnimationData->state = 4;
        }
        break;
    case 2:
        BattlerSpriteContext battlerSpriteCtx;
        MoveAnimation moveAnim;

        ov12_02261CA8(setMoveAnimationData->battleSys, &setMoveAnimationData->moveAnim, &battlerSpriteCtx, setMoveAnimationData->battler);
        ov07_0223494C(&battlerSpriteCtx, HEAP_ID_BATTLE);

        BattleController_SetMoveAnimation(setMoveAnimationData->battleSys, 0L , &moveAnim, 1, 16, setMoveAnimationData->battler, setMoveAnimationData->battler, 0L );
        ov12_02261B80(setMoveAnimationData->battleSys, setMoveAnimationData->battlerData, setMoveAnimationData->battleAnimSys, &moveAnim);
        setMoveAnimationData->battlerData->isAlternateSprite = 0;
        setMoveAnimationData->state++;
        break;
    case 4:
        BattleSystem_SetCriticalHpMusicFlag(setMoveAnimationData->battleSys, 2);

        if (setMoveAnimationData->hideHealthboxes) {
            BattleSystem_SetHpBarDisabled(setMoveAnimationData->battleSys);
        }

        if (setMoveAnimationData->hideShadows) {
            PokemonSpriteManager_HideShadows(BattleSystem_GetPokepicManager(setMoveAnimationData->battleSys));
        }

        ov12_02261B80(setMoveAnimationData->battleSys, setMoveAnimationData->battlerData, setMoveAnimationData->battleAnimSys, &setMoveAnimationData->moveAnim);
        setMoveAnimationData->state++;
        break;
    case 6:
        BattleSystem_SetCriticalHpMusicFlag(setMoveAnimationData->battleSys, 0);

        if (setMoveAnimationData->hideHealthboxes) {
            BattleSystem_SetHpBarEnabled(setMoveAnimationData->battleSys);
        }

        if (setMoveAnimationData->hideShadows) {
            PokemonSpriteManager_ShowShadows(BattleSystem_GetPokepicManager(setMoveAnimationData->battleSys));
        }

        if (setMoveAnimationData->moveAnim.isSubstitute && setMoveAnimationData->moveAnim.animMode == 0 && Pokepic_GetAttr(setMoveAnimationData->monSprite, MON_SPRITE_HIDE) == 0 ) {
            MoveAnimation moveAnim;

            BattleController_SetMoveAnimation(setMoveAnimationData->battleSys, 0L , &moveAnim, 1, 15, setMoveAnimationData->battler, setMoveAnimationData->battler, 0L );
            ov12_02261B80(setMoveAnimationData->battleSys, setMoveAnimationData->battlerData, setMoveAnimationData->battleAnimSys, &moveAnim);
            setMoveAnimationData->state = 7;
        } else {
            setMoveAnimationData->state = 10;
        }
        break;
    case 8: {
        BattlerSpriteContext battlerSpriteCtx;
        MoveAnimation moveAnim;

        ov12_02261CA8(setMoveAnimationData->battleSys, &setMoveAnimationData->moveAnim, &battlerSpriteCtx, setMoveAnimationData->battler);
        ov07_02234A20(&battlerSpriteCtx, HEAP_ID_BATTLE);

        BattleController_SetMoveAnimation(setMoveAnimationData->battleSys, 0L , &moveAnim, 1, 16, setMoveAnimationData->battler, setMoveAnimationData->battler, 0L );
        ov12_02261B80(setMoveAnimationData->battleSys, setMoveAnimationData->battlerData, setMoveAnimationData->battleAnimSys, &moveAnim);
        setMoveAnimationData->battlerData->isAlternateSprite = 1;
    }
        setMoveAnimationData->state++;
        break;
    case 10:
        BattleController_EmitClearCommand(setMoveAnimationData->battleSys, setMoveAnimationData->battler, setMoveAnimationData->command);
        Heap_Free(data);
        SysTask_Destroy(task);
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
        ov07_0221C394(setMoveAnimationData->battleAnimSys);

        if (ov07_0221C3B0(setMoveAnimationData->battleAnimSys) == 0 ) {
            ov07_0221C3C0(setMoveAnimationData->battleAnimSys);
            setMoveAnimationData->state++;
        }
        break;
    }
}
