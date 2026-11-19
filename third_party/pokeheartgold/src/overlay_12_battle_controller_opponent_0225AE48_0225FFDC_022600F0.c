#include "global.h"

#include "battle/battler_sprite_context.h"
#include "sprite_resources_header.h"

#include "unk_02005D10.h"
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
#include "overlay_12_mon_encounter_data.h"

struct BallRotation;

struct BattleAnimSystem;

struct BattleContext;

struct BattleSystem;

struct TextOBJ;

struct Pokepic;

struct PokemonSpriteTransforms;

struct SysTask;

struct SysTaskManager;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef struct SysTaskManager SysTaskManager;

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

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

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);

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

void Pokepic_Delete(Pokepic *monSprite);

void Pokepic_SetVisible(Pokepic *monSprite, int xOffset, int yOffset, int width, int height);

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

typedef struct SavedCursorPosition {
    s8 actionMenuX;
    s8 actionMenuY;
    s8 moveMenuX;
    s8 moveMenuY;
    s8 targetSelectX;
    s8 targetSelectY;
    s8 targetingLayout;
    u8 padding;
} SavedCursorPosition;

typedef struct BattlerDataSysTaskFuncs {
    SysTaskFunc setCommandSelection;
    SysTaskFunc showMoveSelectMenu;
    SysTaskFunc showTargetSelectMenu;
    SysTaskFunc showBagMenu;
    SysTaskFunc showPartyMenu;
    SysTaskFunc showYesNoMenu;
} BattlerDataSysTaskFuncs;

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

u8 GetMonPicHeightBySpeciesGenderForm(u16 species, u8 gender, u8 face, u8 form, u32 personality);

void sub_0207204C(SOUND_CHATOT *chatotCry, enum PokemonCryMod cryMod, u16 species, int form, int pan, int volume, int forceDefaultChatot, enum HeapID heapID, u8 delay);

void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);

void BattleController_SetMoveAnimation(BattleSystem *battleSys, BattleContext *battleCtx, MoveAnimation *animation, int param3, int param4, int attacker, int defender, u16 move);

void Heap_Free(void *ptr);

BOOL ov07_0221C394(BattleAnimSystem *system);

BOOL ov07_0221C3B0(BattleAnimSystem *system);

BOOL ov07_0221C3C0(BattleAnimSystem *system);

BattleAnimSystem *ov12_0223A8DC(BattleSystem *battleSys);

SOUND_CHATOT *BattleSystem_GetChatotVoice(BattleSystem *battleSys, int battler);

void ov07_0223494C(BattlerSpriteContext *battlerSpriteCtx, enum HeapID heapID);

void ov12_02261B80(BattleSystem *battleSys, OpponentData *battlerData, BattleAnimSystem *battleAnimSystem, MoveAnimation *animation);

void ov12_02261CA8(BattleSystem *battleSys, MoveAnimation *animation, BattlerSpriteContext *battlerSpriteCtx, int battler);

typedef struct FaintingSequenceData {
    BattleSystem *battleSys;
    OpponentData *battlerData;
    Pokepic *monSprite;
    MoveAnimation moveAnim;
    u8 command;
    u8 battler;
    u8 state;
    u8 face;
    u16 species;
    u8 gender;
    u8 form;
    u32 personality;
    u16 isSubstitute;
    u16 isTransformed;
} FaintingSequenceData;

void ov12_022600F0(SysTask *task, void *data);

void ov12_022600F0(SysTask *task, void *data) {
    FaintingSequenceData *faintingSequenceData = data;
    BattleAnimSystem *battleAnimSys = ov12_0223A8DC(faintingSequenceData->battleSys);
    int y;
    int height;

    switch (faintingSequenceData->state) {
    case 0:
        if (faintingSequenceData->isSubstitute) {
            MoveAnimation moveAnim;

            BattleController_SetMoveAnimation(faintingSequenceData->battleSys, 0L, &moveAnim, 1, 15, faintingSequenceData->battler, faintingSequenceData->battler, 0L);
            ov12_02261B80(faintingSequenceData->battleSys, faintingSequenceData->battlerData, battleAnimSys, &moveAnim);
            faintingSequenceData->state++;
        } else {
            faintingSequenceData->state = 4;
        }
        break;
    case 2:
        BattlerSpriteContext battlerSpriteCtx;
        MoveAnimation moveAnim;

        ov12_02261CA8(faintingSequenceData->battleSys, &faintingSequenceData->moveAnim, &battlerSpriteCtx, faintingSequenceData->battler);
        ov07_0223494C(&battlerSpriteCtx, HEAP_ID_BATTLE);

        BattleController_SetMoveAnimation(faintingSequenceData->battleSys, 0L, &moveAnim, 1, 16, faintingSequenceData->battler, faintingSequenceData->battler, 0L);
        ov12_02261B80(faintingSequenceData->battleSys, faintingSequenceData->battlerData, battleAnimSys, &moveAnim);
        faintingSequenceData->battlerData->isAlternateSprite = 0;
        faintingSequenceData->state++;
        break;
    case 1:
    case 3:
        ov07_0221C394(battleAnimSys);

        if (ov07_0221C3B0(battleAnimSys) == 0) {
            ov07_0221C3C0(battleAnimSys);
            faintingSequenceData->state++;
        }
        break;
    case 4: {
        int pan;

        if (faintingSequenceData->face == 2) {
            pan = 117;
        } else {
            pan = -117;
        }

        sub_0207204C(BattleSystem_GetChatotVoice(faintingSequenceData->battleSys, faintingSequenceData->battler),
            POKECRY_FAINT,
            faintingSequenceData->species,
            faintingSequenceData->form,
            pan,
            127,
            faintingSequenceData->isTransformed,
            HEAP_ID_BATTLE,
            0);
        faintingSequenceData->state++;
    }
    case 5:
        if (IsCryFinished() == 0) {
            faintingSequenceData->state++;
        }
        break;
    case 6:
        if (faintingSequenceData->face == 2) {
            sub_0200602C(1795, 117);
        } else {
            sub_0200602C(1795, -117);
        }

        if (Pokepic_GetAttr(faintingSequenceData->monSprite, MON_SPRITE_SHADOW_HEIGHT) > 0) {
            faintingSequenceData->state = 7;
        } else {
            faintingSequenceData->state = 8;
        }
        break;
    case 7:
        height = Pokepic_GetAttr(faintingSequenceData->monSprite, MON_SPRITE_SHADOW_HEIGHT);
        height -= 8;

        if (height < 0) {
            height = 0;
        }

        Pokepic_SetAttr(faintingSequenceData->monSprite, MON_SPRITE_SHADOW_HEIGHT, height);

        if (height) {
            break;
        }

        faintingSequenceData->state++;
    case 8:
        height = GetMonPicHeightBySpeciesGenderForm(faintingSequenceData->species, faintingSequenceData->gender, faintingSequenceData->face, faintingSequenceData->form, faintingSequenceData->personality);
        height = (10 * 8) - height;
        Pokepic_SetVisible(faintingSequenceData->monSprite, 0, 0, (10 * 8), height);
        faintingSequenceData->state++;
        break;
    case 9:
        y = Pokepic_GetAttr(faintingSequenceData->monSprite, MON_SPRITE_Y_CENTER);
        y += 8;
        Pokepic_SetAttr(faintingSequenceData->monSprite, MON_SPRITE_Y_CENTER, y);
        height = Pokepic_GetAttr(faintingSequenceData->monSprite, MON_SPRITE_DRAW_HEIGHT);
        height -= 8;

        if (height < 0) {
            height = 0;
        }

        Pokepic_SetAttr(faintingSequenceData->monSprite, MON_SPRITE_DRAW_HEIGHT, height);

        if (height == 0) {
            Pokepic_Delete(faintingSequenceData->monSprite);
            faintingSequenceData->state++;
        }
        break;
    case 10:
        BattleController_EmitClearCommand(faintingSequenceData->battleSys, faintingSequenceData->battler, faintingSequenceData->command);
        Heap_Free(data);
        SysTask_Destroy(task);
        break;
    }
}
