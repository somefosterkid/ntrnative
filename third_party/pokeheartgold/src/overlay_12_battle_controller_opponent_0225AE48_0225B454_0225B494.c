#include "global.h"
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
#include "overlay_12_mon_encounter_data.h"
#include "pokepic.h"
#include "unk_02013534.h"
#include "item.h"
#include "transform_data.h"
#include "pokemon_anim.h"
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
#include "overlay_12_battle_controller_opponent.h"

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

struct PokemonAnim;

struct PokemonAnimManager;

struct Pokepic;

struct PokemonSpriteData;

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

struct OpponentDataCursorPos;

struct SelfTurnFlags;

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

struct UnkBattleSystemSub17C;

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

void ManagedSprite_SetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

void ManagedSprite_GetPositionXY(ManagedSprite *managedSprite, s16 *outX, s16 *outY);

void ManagedSprite_OffsetPositionXY(ManagedSprite *managedSprite, s16 x, s16 y);

typedef struct SpriteSystemContext {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;
} SpriteSystemContext;

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

void Pokepic_StartAnim(Pokepic *monSprite, int dummy);

BOOL Pokepic_IsAnimActive(Pokepic *monSprite);

void Pokepic_StartPaletteFade(Pokepic *monSprite, int initAlpha, int targetAlpha, int delay, int color);

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
    BATTLE_ACTION_PICK_COMMAND = 0,
    BATTLE_ACTION_CHOOSE_TARGET = 1,
    BATTLE_ACTION_TEMP_VALUE = 2,
    BATTLE_ACTION_SELECTED_COMMAND = 3,
    MAX_BATTLE_ACTIONS = 4,
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
    u8 futureSightTurns[4];
    u8 wishTurns[4];
    u16 futureSightMove[4];
    int futureSightAttacker[4];
    s32 futureSightDamage[4];
    u8 wishTarget[4];
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

    int physicalDamageTakenFrom[4];
    int physicalDamageLastAttacker;
    int physicalDamageAttackerMask;

    int specialDamageTakenFrom[4];
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

typedef struct PokemonAnimManager {
    PokemonAnim *anims;
    enum HeapID heapID;
    u8 flipSprite;
    u8 animCount;
} PokemonAnimManager;

BOOL sub_02017068(PokemonAnimManager *monAnimMan, u8 index);

void sub_0207204C(SOUND_CHATOT *chatotCry, enum PokemonCryMod cryMod, u16 species, int form, int pan, int volume, int forceDefaultChatot, enum HeapID heapID, u8 delay);

void sub_0207294C(NARC *narc, PokemonAnimManager *monAnimMan, Pokepic *sprite, u16 species, int face, int flipSprite, int frame);

void sub_020729A4(NARC *narc, u8 *ret, u16 species, u16 isFrontpic);


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

BOOL ov07_0221C394(BattleAnimSystem *system);

BOOL ov07_0221C3B0(BattleAnimSystem *system);

BOOL ov07_0221C3C0(BattleAnimSystem *system);

BattleAnimSystem *ov12_0223A8DC(BattleSystem *battleSys);

PokemonAnimManager *ov12_0223B750(BattleSystem *battleSys);

SOUND_CHATOT *BattleSystem_GetChatotVoice(BattleSystem *battleSys, int battler);

void sub_02005B58(u8 allowed);

void ov12_02261B80(BattleSystem *battleSys, OpponentData *battlerData, BattleAnimSystem *battleAnimSystem, MoveAnimation *animation);


void ov12_0225B494(SysTask *task, void *data);

void ov12_0225B494(SysTask *task, void *data) {
    MonEncounterData *monEncounterData = data;
    BattleAnimSystem *battleAnimSys = ov12_0223A8DC(monEncounterData->battleSys);
    s16 x, y;

    switch (monEncounterData->state) {
    case 0:
        monEncounterData->delay = 28;
        monEncounterData->state++;
    case 1:
        if (--monEncounterData->delay) {
            break;
        }

        monEncounterData->state++;
    case 2:
        if (monEncounterData->face == 2) {
            ManagedSprite_GetPositionXY(monEncounterData->terrain->unk0, &x, &y);

            if (monEncounterData->battlerType == 1 || monEncounterData->battlerType == 3) {
                if (x < (24 * 8)) {
                    ManagedSprite_OffsetPositionXY(monEncounterData->terrain->unk0, 8, 0);
                } else {
                    ManagedSprite_SetPositionXY(monEncounterData->terrain->unk0, 24 * 8, 8 * 11);
                }
            }

            ManagedSprite_GetPositionXY(monEncounterData->terrain->unk0, &x, &y);

            if (monEncounterData->battlerType == 1) {
                Pokepic_SetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER, x);
            } else if (monEncounterData->battlerType == 3) {
                x = Pokepic_GetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER) - x;
                x -= 24;

                Pokepic_AddAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER, -x);
            } else if (monEncounterData->battlerType == 5) {
                x = x - Pokepic_GetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER);
                x -= 16;

                Pokepic_AddAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER, x);
            }

            if (Pokepic_GetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER) >= monEncounterData->targetPos) {
                Pokepic_SetAttr(monEncounterData->sprite, MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y, 0);
                Pokepic_SetAttr(monEncounterData->sprite, MON_SPRITE_SHADOW_IS_AFFINE, 0);
                Pokepic_SetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER, monEncounterData->targetPos);

                ov12_02261F38(monEncounterData->battleSys, monEncounterData->battler, monEncounterData->battlerType, monEncounterData->sprite, monEncounterData->battlerData->narc, monEncounterData->species, monEncounterData->formNum, monEncounterData->face, monEncounterData->cryMod);

                if (monEncounterData->battlerType == 1 || monEncounterData->battlerType == 3) {
                    ManagedSprite_SetPositionXY(monEncounterData->terrain->unk0, 24 * 8, 8 * 11);
                }

                Pokepic_StartPaletteFade(monEncounterData->sprite, 8, 0, 0, 0);
                monEncounterData->state++;
            }
        } else {
            ManagedSprite_GetPositionXY(monEncounterData->terrain->unk0, &x, &y);

            if (monEncounterData->battlerType == 0 || monEncounterData->battlerType == 2) {
                if (x > 64) {
                    ManagedSprite_OffsetPositionXY(monEncounterData->terrain->unk0, -8, 0);
                } else {
                    ManagedSprite_SetPositionXY(monEncounterData->terrain->unk0, 64, 128 + 8);
                }
            }

            ManagedSprite_GetPositionXY(monEncounterData->terrain->unk0, &x, &y);

            if (monEncounterData->battlerType == 0) {
                Pokepic_SetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER, x);
            } else if (monEncounterData->battlerType == 2) {
                x = x - Pokepic_GetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER);
                x -= 24;
                Pokepic_AddAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER, x);
            } else if (monEncounterData->battlerType == 4) {
                x = Pokepic_GetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER) - x;
                x -= 16;
                Pokepic_AddAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER, -x);
            }

            if (Pokepic_GetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER) <= monEncounterData->targetPos) {
                Pokepic_SetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER, monEncounterData->targetPos);

                ov12_02261F38(monEncounterData->battleSys, monEncounterData->battler, monEncounterData->battlerType, monEncounterData->sprite, monEncounterData->battlerData->narc, monEncounterData->species, monEncounterData->formNum, monEncounterData->face, monEncounterData->cryMod);

                if (monEncounterData->battlerType == 0 || monEncounterData->battlerType == 2) {
                    ManagedSprite_SetPositionXY(monEncounterData->terrain->unk0, 64, 128 + 8);
                }

                monEncounterData->state++;
            }
        }
        break;
    case 3:
        if (sub_02017068(ov12_0223B750(monEncounterData->battleSys), monEncounterData->battler) == 1
            && Pokepic_IsAnimActive(monEncounterData->sprite) == 0) {
            if (monEncounterData->isShiny) {
                MoveAnimation moveAnim;

                BattleController_SetMoveAnimation(monEncounterData->battleSys, 0L, &moveAnim, 1, 11, monEncounterData->battler, monEncounterData->battler, 0L);
                ov12_02261B80(monEncounterData->battleSys, monEncounterData->battlerData, battleAnimSys, &moveAnim);
                monEncounterData->state = 4;
            } else {
                monEncounterData->state = 0xFF;
            }
        }
        break;
    case 4:
        ov07_0221C394(battleAnimSys);

        if (ov07_0221C3B0(battleAnimSys) == 0) {
            ov07_0221C3C0(battleAnimSys);
            monEncounterData->state = 0xFF;
        }
        break;
    default:
        sub_02005B58(0);
        BattleController_EmitClearCommand(monEncounterData->battleSys, monEncounterData->battler, monEncounterData->command);
        Heap_Free(data);
        SysTask_Destroy(task);
        break;
    }
}
