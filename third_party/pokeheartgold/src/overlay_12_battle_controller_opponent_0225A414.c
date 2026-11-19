#include "global.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/command_set_data.h"

#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "msgdata.h"
#include "party.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "overlay_12_menu_task_data.h"

struct BallRotation;
struct BattleAnimSystem;
struct BattleBagContext;
struct BattleMonOBJData;
struct BattlePartyContext;
struct PokemonSprite;
struct PokemonSpriteTransforms;
typedef struct HealthBox {
    u8 state;
    ManagedSprite *mainSprite;
    ManagedSprite *arrowSprite;
    BattleSystem *battleSys;
    SysTask *task_10;
    Window unused_14;
    u8 battler;
    u8 type;
    u8 selectedPartySlot;
    u8 numSafariBalls;
    s32 curHP;
    s32 maxHP;
    s32 damage;
    s32 hpTemp;
    s32 curExp;
    s32 maxExp;
    s32 expReward;
    s32 expTemp;
    u8 level;
    u8 gender;
    u8 status;
    u8 caughtSpecies;
    u8 command;
    u8 delay;
    u8 expSoundTimer;
    u8 scrollDirection : 1;
    u8 doneScrolling : 1;
    u8 unused_4F_2 : 1;
    u8 numberMode : 1;
    u8 padding : 4;
    SysTask *bobbingAnimTask;
    u16 degrees;
} HealthBox;
void ov12_0226498C(HealthBox *healthbox, u32 hp, u32 flags);
void BattleHpBar_SetEnabled(HealthBox *healthbox, BOOL enable);
u8 BattleHpBar_Util_GetBarTypeFromBattlerSide(u8 battlerType, u32 battleType);
typedef struct BallRotation BallRotation;
typedef struct PokemonSprite PokemonSprite;
typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;
typedef void(PokemonSpriteCallback)(PokemonSprite *, PokemonSpriteTransforms *);
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
struct PokemonSprite {
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
    PokepicAnimScript animFrames[10];
};
typedef struct BattleMonOBJData BattleMonOBJData;
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
typedef struct BattlerData {
    BattlerDataSysTaskFuncs taskFuncs;
    ManagedSprite *managedSprite;
    PokemonSprite *monSpriteBackup;
    PokemonSprite *monSprite;
    Window *unused_24;
    HealthBox healthbox;
    SavedCursorPosition savedCursorPos;
    BallRotation *ballRotation;
    BattleMonOBJData *btlMonObjData;
    void *unused_8C;
    u8 data[256];
    u8 battler;
    u8 battlerType;
    u8 bootState;
    u8 commandUIReady;
    SysTask *sysTask;
    u16 degrees;
    int isAlternateSprite;
    NARC *narc;
    u8 msgPending;
    u8 unused_1A5[3];
} BattlerData;
typedef struct HealthBoxData {
    u8 command;
    u8 level;
    s16 curHP;
    u16 maxHP;
    u8 selectedPartySlot;
    u8 status : 5;
    u8 gender : 2;
    u8 speciesCaught : 1;
    u32 expFromLastLevel;
    u32 expToNextLevel;
    int numSafariBalls;
    u8 delay;
} HealthBoxData;
u32 BattleSystem_GetBattleType(BattleSystem *battleSys);
void ov12_0225DA18(SysTask *task, void *data);
enum HealthBoxScrollDirection {
    HEALTHBOX_SCROLL_IN = 0,
    HEALTHBOX_SCROLL_OUT,
};
void ov12_02264FB0(HealthBox *healthbox, enum HealthBoxScrollDirection direction);
void ov12_0225DA8C(SysTask *task, void *data);
enum PartyStockStatus {
    STOCK_STATUS_NO_MON = 0,
    STOCK_STATUS_MON_ALIVE,
    STOCK_STATUS_MON_FAINTED,
    STOCK_STATUS_HAS_STATUS_CONDITION,
};
enum PartyGaugeSide {
    PARTY_GAUGE_OURS,
    PARTY_GAUGE_THEIRS,
    PARTY_GAUGE_COUNT
};
typedef struct CommandSetMessage {
    u8 command;
    u8 partySlot;
    u8 expPercents[6];
    u8 ballStatus[PARTY_GAUGE_COUNT][6];
    u16 moves[4];
    u8 curPP[4];
    u8 maxPP[4];
    s16 curHP;
    u16 maxHP;
    u8 ballStatusBattler;
    u8 switchingOrCanPickCommandMask;
    u16 padding_2A;
} CommandSetMessage;
typedef struct MoveSelectMenuMessage {
    u8 command;
    u8 partySlot;
    u16 invalidMoves;
    u16 moves[4];
    u8 ppCur[4];
    u8 ppMax[4];
} MoveSelectMenuMessage;
typedef struct TargetSelectMenuMessage {
    u8 command;
    u8 shouldHidePanel;
    u16 range;
    TargetMonData targetMon[4];
} TargetSelectMenuMessage;
int BattleSystem_GetMaxBattlers(BattleSystem *battleSys);
void ov12_0223C1C4(BattleSystem *battleSys, u8 *battlerTypes);
typedef struct BagMenuMessage {
    u8 command;
    u8 hasTwoOpponents;
    u8 semiInvulnerable;
    u8 substitute;
    u8 partySlots[4];
    u8 partyOrder[4][6];
    u8 embargoTurns[4];
} BagMenuMessage;
typedef struct BattleBagContext BattleBagContext;
typedef struct BattlePartyContext BattlePartyContext;
struct BattlePartyContext {
    Party *party;
    void *sLotoId;
    BattleSystem *battleSys;
    enum HeapID heapID;
    u8 unused2;
    u8 selectedPartyIndex;
    u8 doubleBattleFirstSelectedPartySlot;
    u8 unused3;
    u8 playerPokemonPartySlot;
    u8 partnerPokemonPartySlot;
    u32 embargoRemainingTurns[2];
    u16 currentDamage;
    u16 selectedBattleBagItem;
    u16 moveToLearn;
    s32 battler;
    u8 pokemonPartySlots[6];
    u8 isCursorEnabled;
    u8 selectedBattleBagPocket;
    u8 selectedMoveSlot;
    u8 battlePartyMode;
    u8 battlePartyExited;
};
typedef struct PartyMenuData {
    BattleSystem *battleSys;
    BattlePartyContext *battlePartyCtx;
    u8 command;
    u8 battler;
    u8 state;
    u8 listMode;
    u8 partySlots[4];
    int canSwitch;
    u16 selectedBattleBagItem;
    u8 doublesSelection;
    u8 isCursorEnabled;
    u8 battlersSwitchingMask;
    u8 unused[3];
    u8 partyOrder[4][6];
} PartyMenuData;
typedef struct BagMenuData {
    BattleSystem *battleSys;
    BattleBagContext *battleBagCtx;
    PartyMenuData *partyMenuData;
    u8 command;
    u8 battler;
    u8 state;
    u8 battlerType;
    u8 isCursorEnabled;
    u8 msgIdx;
    u16 stateAfterDelay;
    u8 hasTwoOpponents;
    u8 semiInvulnerable;
    u8 substitute;
    u8 delay;
    u8 partyOrder[4][6];
    u8 embargoTurns[4];
} BagMenuData;
typedef struct PartyMenuMessage {
    u8 command;
    u8 battler;
    u8 listMode;
    u8 doublesSelection;
    u8 selectedPartySlot[4];
    u8 partyOrder[4][6];
    int canSwitch;
    u8 battlersSwitchingMask;
    u8 padding_25[3];
} PartyMenuMessage;
typedef struct YesNoMenuMessage {
    u8 command;
    u8 yesNoType;
    u16 promptMsg;
    int move;
    int nickname;
} YesNoMenuMessage;
enum BattleMessageTag {
    TAG_NONE = 0,
    TAG_NONE_SIDE_CONSCIOUS = 1,
    TAG_NICKNAME = 2,
    TAG_MOVE = 3,
    TAG_STAT = 4,
    TAG_ITEM = 5,
    TAG_NUMBER = 6,
    TAG_NUMBERS = 7,
    TAG_TRNAME = 8,
    TAG_NICKNAME_NICKNAME = 9,
    TAG_NICKNAME_MOVE = 10,
    TAG_NICKNAME_ABILITY = 11,
    TAG_NICKNAME_STAT = 12,
    TAG_NICKNAME_TYPE = 13,
    TAG_NICKNAME_POKE = 14,
    TAG_NICKNAME_ITEM = 15,
    TAG_NICKNAME_POFFIN = 16,
    TAG_NICKNAME_NUM = 17,
    TAG_NICKNAME_TRNAME = 18,
    TAG_NICKNAME_BOX = 19,
    TAG_MOVE_SIDE = 20,
    TAG_MOVE_NICKNAME = 21,
    TAG_MOVE_MOVE = 22,
    TAG_ABILITY_NICKNAME = 23,
    TAG_ITEM_MOVE = 24,
    TAG_NUMBER_NUMBER = 25,
    TAG_TRNAME_TRNAME = 26,
    TAG_TRNAME_NICKNAME = 27,
    TAG_TRNAME_ITEM = 28,
    TAG_TRNAME_NUM = 29,
    TAG_TRCLASS_TRNAME = 30,
    TAG_NICKNAME_NICKNAME_MOVE = 31,
    TAG_NICKNAME_NICKNAME_ABILITY = 32,
    TAG_NICKNAME_NICKNAME_ITEM = 33,
    TAG_NICKNAME_MOVE_MOVE = 34,
    TAG_NICKNAME_MOVE_NUMBER = 35,
    TAG_NICKNAME_ABILITY_NICKNAME = 36,
    TAG_NICKNAME_ABILITY_MOVE = 37,
    TAG_NICKNAME_ABILITY_ITEM = 38,
    TAG_NICKNAME_ABILITY_STAT = 39,
    TAG_NICKNAME_ABILITY_TYPE = 40,
    TAG_NICKNAME_ABILITY_STATUS = 41,
    TAG_NICKNAME_ABILITY_NUMBER = 42,
    TAG_NICKNAME_ITEM_NICKNAME = 43,
    TAG_NICKNAME_ITEM_MOVE = 44,
    TAG_NICKNAME_ITEM_STAT = 45,
    TAG_NICKNAME_ITEM_STATUS = 46,
    TAG_NICKNAME_BOX_BOX = 47,
    TAG_ITEM_NICKNAME_FLAVOR = 48,
    TAG_TRNAME_NICKNAME_NICKNAME = 49,
    TAG_TRCLASS_TRNAME_NICKNAME = 50,
    TAG_TRCLASS_TRNAME_ITEM = 51,
    TAG_NICKNAME_ABILITY_NICKNAME_MOVE = 52,
    TAG_NICKNAME_ABILITY_NICKNAME_ABILITY = 53,
    TAG_NICKNAME_ABILITY_NICKNAME_STAT = 54,
    TAG_NICKNAME_ITEM_NICKNAME_ITEM = 55,
    TAG_TRNAME_NICKNAME_TRNAME_NICKNAME = 56,
    TAG_TRCLASS_TRNAME_NICKNAME_NICKNAME = 57,
    TAG_TRCLASS_TRNAME_NICKNAME_TRNAME = 58,
    TAG_TRCLASS_TRNAME_TRCLASS_TRNAME = 59,
    TAG_TRCLASS_TRNAME_NICKNAME_TRCLASS_TRNAME_NICKNAME = 60,
};
typedef struct AttackMsgMessage {
    u8 command;
    u8 partySlot;
    u16 move;
} AttackMsgMessage;
MsgData *ov12_0223A934(BattleSystem *battleSys);
u8 BattleSystem_GetTextFrameDelay(BattleSystem *battleSys);
u32 BattleSystem_PrintBattleMessage(BattleSystem *battleSys, MsgData *msgLoader, BattleMessage *battleMsg, u8 renderDelay);
void ov12_022605D0(SysTask *task, void *data);
typedef struct BattleMessageWaitTask {
    BattleSystem *battleSys;
    u8 command;
    u8 battler;
    u8 msgIdx;
    u8 pad_7[1];
} BattleMessageWaitTask;
MsgData *BattleSystem_GetMessageLoader(BattleSystem *battleSys);
typedef struct MoveAnimation {
    u8 command;
    u8 unk_01;
    u16 move;
    s32 damage;
    u16 power;
    u16 effectChance;
    u16 friendship;
    u16 isSubstitute : 1;
    u16 isTransformed : 1;
    u16 unk_0E_2 : 1;
    u16 unk_0E_3 : 1;
    u16 unk_0E_4 : 12;
    u32 fieldConditions;
    u16 attacker;
    u16 defender;
    u16 species[4];
    u8 genders[4];
    u8 isShiny[4];
    u8 formNums[4];
    u32 personalities[4];
    u32 moveEffectMasks[4];
    int animMode;
    int secondaryAnimID;
    int terrain;
} MoveAnimation;
typedef struct BattleAnimSystem BattleAnimSystem;
BattleAnimSystem *ov12_0223A8DC(BattleSystem *battleSys);
void ov12_0225FD14(SysTask *task, void *data);
void ov12_02261D30(u8 *hideHealthboxes, u8 *hideShadows, int animMode, int secondaryAnimID, u16 move);
typedef struct SetMoveAnimationData {
    BattleSystem *battleSys;
    BattlerData *battlerData;
    PokemonSprite *monSprite;
    BattleAnimSystem *battleAnimSys;
    MoveAnimation moveAnim;
    u8 command;
    u8 battler;
    u8 state;
    u8 hideHealthboxes;
    u8 hideShadows;
    u8 unused[3];
} SetMoveAnimationData;
void ov12_0225FF80(SysTask *task, void *data);
typedef struct FlickerBattlerData {
    BattleSystem *battleSys;
    PokemonSprite *monSprite;
    u8 battler;
    u8 counter;
    u8 delay;
    u8 unused;
} FlickerBattlerData;
typedef struct HPGaugeUpdateMessage {
    u8 command;
    u8 level;
    s16 curHP;
    u16 maxHP;
    u8 padding_06;
    u8 gender;
    int hpCalcTemp;
    u32 exp;
    u32 expToNextLevel;
} HPGaugeUpdateMessage;
void ov12_0225FFDC(SysTask *task, void *data);
typedef struct ExpGaugeUpdateMessage {
    u8 command;
    u8 padding_01[3];
    u32 curExp;
    u32 gainedExp;
    u32 expToNextLevel;
} ExpGaugeUpdateMessage;
void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);
void ov12_02260030(SysTask *task, void *data);
typedef struct FaintingSequenceMessage {
    u8 command;
    u8 gender;
    u16 species;
    u32 personality;
    u8 form;
    u8 isSubstitute;
    u8 isTransformed;
    u8 unk_0B;
    u16 monSpecies[4];
    u8 monGenders[4];
    u8 monShiny[4];
    u8 monFormNums[4];
    u32 monPersonalities[4];
} FaintingSequenceMessage;
void ov12_022600F0(SysTask *task, void *data);
typedef struct FaintingSequenceData {
    BattleSystem *battleSys;
    BattlerData *battlerData;
    PokemonSprite *monSprite;
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
typedef struct PlaySoundMessage {
    u8 command;
    u8 unk_01;
    u16 sdatID;
} PlaySoundMessage;
void ov12_0226037C(SysTask *task, void *data);
typedef struct FadeOutData {
    BattleSystem *battleSys;
    u8 command;
    u8 battler;
    u8 state;
    u8 unused;
} FadeOutData;
typedef struct ToggleVanishMessage {
    u8 command;
    u8 toggle;
    u8 isSubstitute;
    u8 padding_03;
    u16 species[4];
    u8 gender[4];
    u8 isShiny[4];
    u8 formNum[4];
    u32 personality[4];
} ToggleVanishMessage;
void ov12_02260418(SysTask *task, void *data);
typedef struct ToggleVanishData {
    BattleSystem *battleSys;
    BattlerData *battlerData;
    MoveAnimation moveAnim;
    u8 command;
    u8 battler;
    u8 state;
    u8 toggleHide;
    int isSubstitute;
} ToggleVanishData;
typedef struct SetStatusIconMessage {
    u8 command;
    u8 status;
    u16 padding_02;
} SetStatusIconMessage;

void ov12_0225A414(BattleSystem *battleSys, BattlerData *battlerData, HealthBoxData *healthboxData);
void ov12_0225A4DC(BattleSystem *battleSys, BattlerData *battlerData);
void ov12_0225A524(BattleSystem *battleSys, BattlerData *battlerData, CommandSetMessage *message);
void ov12_0225A604(BattleSystem *battleSys, BattlerData *battlerData, MoveSelectMenuMessage *message);
void ov12_0225A674(BattleSystem *battleSys, BattlerData *battlerData, TargetSelectMenuMessage *message);
void ov12_0225A700(BattleSystem *battleSys, BattlerData *battlerData, BagMenuMessage *message);
void ov12_0225A7AC(BattleSystem *battleSys, BattlerData *battlerData, PartyMenuMessage *message);
void ov12_0225A818(BattleSystem *battleSys, BattlerData *battlerData, YesNoMenuMessage *message);
void ov12_0225A85C(BattleSystem *battleSys, BattlerData *battlerData, AttackMsgMessage *message);
void ov12_0225A8C4(BattleSystem *battleSys, BattlerData *battlerData, BattleMessage *battleMsg);
void ov12_0225A914(BattleSystem *battleSys, BattlerData *battlerData, MoveAnimation *animation);
void ov12_0225A9B0(BattleSystem *battleSys, BattlerData *battlerData);
void ov12_0225A9E0(BattleSystem *battleSys, BattlerData *battlerData, HPGaugeUpdateMessage *message);
void ov12_0225AA6C(BattleSystem *battleSys, BattlerData *battlerData, ExpGaugeUpdateMessage *message);
void ov12_0225AAE0(BattleSystem *battleSys, BattlerData *battlerData, FaintingSequenceMessage *message);
void ov12_0225ABB8(BattleSystem *battleSys, BattlerData *battlerData, PlaySoundMessage *message);
void ov12_0225ABE8(BattleSystem *battleSys, BattlerData *battlerData);
void ov12_0225AC1C(BattleSystem *battleSys, BattlerData *battlerData, ToggleVanishMessage *message);
void ov12_0225ACB0(BattleSystem *battleSys, BattlerData *battlerData, SetStatusIconMessage *message);

void ov12_0225A414(BattleSystem *battleSys, BattlerData *battlerData, HealthBoxData *healthboxData) {
    HealthBox *healthbox = &battlerData->healthbox;
    MI_CpuClearFast(&healthbox->state, sizeof(u8));

    healthbox->battleSys = battleSys;
    healthbox->battler = battlerData->battler;
    healthbox->type = BattleHpBar_Util_GetBarTypeFromBattlerSide(battlerData->battlerType, BattleSystem_GetBattleType(battleSys));
    healthbox->command = healthboxData->command;
    healthbox->curHP = healthboxData->curHP;
    healthbox->maxHP = healthboxData->maxHP;
    healthbox->level = healthboxData->level;
    healthbox->gender = healthboxData->gender;
    healthbox->damage = 0;
    healthbox->curExp = healthboxData->expFromLastLevel;
    healthbox->maxExp = healthboxData->expToNextLevel;
    healthbox->selectedPartySlot = healthboxData->selectedPartySlot;
    healthbox->status = healthboxData->status;
    healthbox->caughtSpecies = healthboxData->speciesCaught;
    healthbox->delay = healthboxData->delay;
    healthbox->numSafariBalls = healthboxData->numSafariBalls;

    BattleHpBar_SetEnabled(healthbox, 0);
    ov12_0226498C(healthbox, healthbox->curHP, ~0);

    healthbox->task_10 = SysTask_CreateOnMainQueue(ov12_0225DA18, healthbox, 1000);
}

void ov12_0225A4DC(BattleSystem *battleSys, BattlerData *battlerData) {
    HealthBox *healthbox = &battlerData->healthbox;
    MI_CpuClearFast(&healthbox->state, sizeof(u8));

    healthbox->battleSys = battleSys;
    healthbox->battler = battlerData->battler;
    healthbox->command = battlerData->data[0];

    ov12_02264FB0(healthbox, HEALTHBOX_SCROLL_OUT);

    healthbox->task_10 = SysTask_CreateOnMainQueue(ov12_0225DA8C, healthbox, 1000);
}

void ov12_0225A524(BattleSystem *battleSys, BattlerData *battlerData, CommandSetMessage *message) {
    CommandSetData *commandSetData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(CommandSetData));
    int i;
    MI_CpuClearFast(commandSetData, sizeof(CommandSetData));

    commandSetData->state = 0;
    commandSetData->unused_0B = 0;
    commandSetData->battleSys = battleSys;
    commandSetData->command = message->command;
    commandSetData->battler = battlerData->battler;
    commandSetData->battlerType = battlerData->battlerType;
    commandSetData->healthbox = &battlerData->healthbox;
    commandSetData->partySlot = message->partySlot;
    commandSetData->curHP = message->curHP;
    commandSetData->maxHP = message->maxHP;
    commandSetData->ballStatusBattler = message->ballStatusBattler;
    commandSetData->switchingOrCanPickCommandMask = message->switchingOrCanPickCommandMask;

    for (i = 0; i < PARTY_GAUGE_COUNT; i++) {
        for (int j = 0; j < 6; j++) {
            commandSetData->ballStatus[i][j] = message->ballStatus[i][j];
        }
    }

    for (i = 0; i < 6; i++) {
        if (message->ballStatus[0][i] == STOCK_STATUS_MON_FAINTED) {
            commandSetData->expPercents[i] = 0;
        } else {
            commandSetData->expPercents[i] = message->expPercents[i];
        }
    }

    for (int battler = 0; battler < 4; battler++) {
        commandSetData->moves[battler] = message->moves[battler];
        commandSetData->curPP[battler] = message->curPP[battler];
        commandSetData->maxPP[battler] = message->maxPP[battler];
    }

    SysTask_CreateOnMainQueue(battlerData->taskFuncs.setCommandSelection, commandSetData, 0);
}

void ov12_0225A604(BattleSystem *battleSys, BattlerData *battlerData, MoveSelectMenuMessage *message) {
    MoveSelectMenuData *moveSelectMenuData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(MoveSelectMenuData));

    moveSelectMenuData->state = 0;
    moveSelectMenuData->battleSys = battleSys;
    moveSelectMenuData->command = battlerData->data[0];
    moveSelectMenuData->battler = battlerData->battler;
    moveSelectMenuData->battlerType = battlerData->battlerType;
    moveSelectMenuData->healthbox = &battlerData->healthbox;
    moveSelectMenuData->partySlot = message->partySlot;

    for (int i = 0; i < 4; i++) {
        moveSelectMenuData->moves[i] = message->moves[i];
        moveSelectMenuData->ppCur[i] = message->ppCur[i];
        moveSelectMenuData->ppMax[i] = message->ppMax[i];
    }

    moveSelectMenuData->invalidMoves = message->invalidMoves;

    SysTask_CreateOnMainQueue(battlerData->taskFuncs.showMoveSelectMenu, moveSelectMenuData, 0);
}

void ov12_0225A674(BattleSystem *battleSys, BattlerData *battlerData, TargetSelectMenuMessage *message) {
    TargetSelectMenuData *targetSelectMenuData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(TargetSelectMenuData));
    int maxBattlers;
    u32 battleType;
    u8 battlerTypes[6];

    targetSelectMenuData->state = 0;
    targetSelectMenuData->battleSys = battleSys;
    targetSelectMenuData->command = battlerData->data[0];
    targetSelectMenuData->battler = battlerData->battler;
    targetSelectMenuData->battlerType = battlerData->battlerType;
    targetSelectMenuData->range = message->range;
    targetSelectMenuData->healthbox = &battlerData->healthbox;
    targetSelectMenuData->shouldHidePanel = message->shouldHidePanel;

    ov12_0223C1C4(battleSys, &battlerTypes[0]);

    maxBattlers = BattleSystem_GetMaxBattlers(battleSys);
    battleType = BattleSystem_GetBattleType(battleSys);

    for (int i = 0; i < maxBattlers; i++) {
        targetSelectMenuData->targetMon[i] = message->targetMon[i];
    }

    SysTask_CreateOnMainQueue(battlerData->taskFuncs.showTargetSelectMenu, targetSelectMenuData, 0);
}

void ov12_0225A700(BattleSystem *battleSys, BattlerData *battlerData, BagMenuMessage *message) {
    BagMenuData *bagMenuData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BagMenuData));

    bagMenuData->partyMenuData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(PartyMenuData));
    bagMenuData->partyMenuData->battlePartyCtx = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattlePartyContext));
    bagMenuData->partyMenuData->battlePartyCtx->party = SaveArray_Party_Alloc(HEAP_ID_BATTLE);
    bagMenuData->state = 0;
    bagMenuData->battleSys = battleSys;
    bagMenuData->command = message->command;
    bagMenuData->battler = battlerData->battler;
    bagMenuData->battlerType = battlerData->battlerType;
    bagMenuData->hasTwoOpponents = message->hasTwoOpponents;
    bagMenuData->semiInvulnerable = message->semiInvulnerable;
    bagMenuData->substitute = message->substitute;

    for (int i = 0; i < 4; i++) {
        bagMenuData->partyMenuData->partySlots[i] = message->partySlots[i];

        for (int j = 0; j < 6; j++) {
            bagMenuData->partyOrder[i][j] = message->partyOrder[i][j];
        }

        bagMenuData->embargoTurns[i] = message->embargoTurns[i];
    }

    SysTask_CreateOnMainQueue(battlerData->taskFuncs.showBagMenu, bagMenuData, 0);
}

void ov12_0225A7AC(BattleSystem *battleSys, BattlerData *battlerData, PartyMenuMessage *message) {
    PartyMenuData *partyMenuData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(PartyMenuData));

    partyMenuData->state = 0;
    partyMenuData->battleSys = battleSys;
    partyMenuData->command = message->command;
    partyMenuData->battler = message->battler;
    partyMenuData->listMode = message->listMode;
    partyMenuData->canSwitch = message->canSwitch;
    partyMenuData->doublesSelection = message->doublesSelection;
    partyMenuData->selectedBattleBagItem = 0;
    partyMenuData->battlersSwitchingMask = message->battlersSwitchingMask;

    for (int i = 0; i < 4; i++) {
        partyMenuData->partySlots[i] = message->selectedPartySlot[i];

        for (int j = 0; j < 6; j++) {
            partyMenuData->partyOrder[i][j] = message->partyOrder[i][j];
        }
    }

    SysTask_CreateOnMainQueue(battlerData->taskFuncs.showPartyMenu, partyMenuData, 0);
}

void ov12_0225A818(BattleSystem *battleSys, BattlerData *battlerData, YesNoMenuMessage *message) {
    YesNoMenuData *yesNoMenuData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(YesNoMenuData));

    yesNoMenuData->state = 0;
    yesNoMenuData->battleSys = battleSys;
    yesNoMenuData->command = message->command;
    yesNoMenuData->battler = battlerData->battler;
    yesNoMenuData->healthbox = &battlerData->healthbox;
    yesNoMenuData->promptMsg = message->promptMsg;
    yesNoMenuData->yesNoType = message->yesNoType;
    yesNoMenuData->move = message->move;
    yesNoMenuData->nickname = message->nickname;

    SysTask_CreateOnMainQueue(battlerData->taskFuncs.showYesNoMenu, yesNoMenuData, 0);
}

void ov12_0225A85C(BattleSystem *battleSys, BattlerData *battlerData, AttackMsgMessage *message) {
    BattleMessageWaitTask *waitTask;
    MsgData *msgLoader = ov12_0223A934(battleSys);
    BattleMessage battleMsg;

    waitTask = (BattleMessageWaitTask *)Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleMessageWaitTask));

    waitTask->battleSys = battleSys;
    waitTask->command = message->command;
    waitTask->battler = battlerData->battler;

    battleMsg.id = message->move * 3;
    battleMsg.tag = TAG_NICKNAME;
    battleMsg.param[0] = battlerData->battler | (message->partySlot << 8);

    waitTask->msgIdx = BattleSystem_PrintBattleMessage(battleSys, msgLoader, &battleMsg, BattleSystem_GetTextFrameDelay(battleSys));

    SysTask_CreateOnMainQueue(ov12_022605D0, waitTask, 0);
}

void ov12_0225A8C4(BattleSystem *battleSys, BattlerData *battlerData, BattleMessage *battleMsg) {
    MsgData *msgLoader = BattleSystem_GetMessageLoader(battleSys);
    BattleMessageWaitTask *waitTask = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleMessageWaitTask));

    waitTask->battleSys = battleSys;
    waitTask->command = battlerData->data[0];
    waitTask->battler = battlerData->battler;
    waitTask->msgIdx = BattleSystem_PrintBattleMessage(battleSys, msgLoader, battleMsg, BattleSystem_GetTextFrameDelay(battleSys));

    SysTask_CreateOnMainQueue(ov12_022605D0, waitTask, 0);
}

void ov12_0225A914(BattleSystem *battleSys, BattlerData *battlerData, MoveAnimation *animation) {
    SetMoveAnimationData *setMoveAnimationData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(SetMoveAnimationData));

    setMoveAnimationData->state = 0;
    setMoveAnimationData->battleSys = battleSys;
    setMoveAnimationData->battlerData = battlerData;
    setMoveAnimationData->command = battlerData->data[0];
    setMoveAnimationData->battler = battlerData->battler;
    setMoveAnimationData->battleAnimSys = ov12_0223A8DC(battleSys);
    setMoveAnimationData->moveAnim = *animation;
    setMoveAnimationData->monSprite = battlerData->monSprite;

    if (animation->animMode == 1 && animation->secondaryAnimID == 25) {
        battlerData->isAlternateSprite = 1;
    }

    if (animation->animMode == 1 && animation->secondaryAnimID == 26) {
        battlerData->isAlternateSprite = 0;
    }

    ov12_02261D30(&setMoveAnimationData->hideHealthboxes, &setMoveAnimationData->hideShadows, animation->animMode, animation->secondaryAnimID, animation->move);
    SysTask_CreateOnMainQueue(ov12_0225FD14, setMoveAnimationData, 0);
}

void ov12_0225A9B0(BattleSystem *battleSys, BattlerData *battlerData) {
    FlickerBattlerData *flickerBattlerData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(FlickerBattlerData));

    flickerBattlerData->counter = 0;
    flickerBattlerData->battleSys = battleSys;
    flickerBattlerData->monSprite = battlerData->monSprite;
    flickerBattlerData->battler = battlerData->battler;
    flickerBattlerData->delay = 0;

    SysTask_CreateOnMainQueue(ov12_0225FF80, flickerBattlerData, 0);
}

void ov12_0225A9E0(BattleSystem *battleSys, BattlerData *battlerData, HPGaugeUpdateMessage *message) {
    HealthBox *healthbox;

    ((battlerData->healthbox.mainSprite != 0L) ? (void)0 : GF_AssertFail());

    healthbox = &battlerData->healthbox;
    MI_CpuClear8(&healthbox->state, sizeof(u8));

    healthbox->battleSys = battleSys;
    healthbox->command = message->command;
    healthbox->battler = battlerData->battler;
    healthbox->type = BattleHpBar_Util_GetBarTypeFromBattlerSide(battlerData->battlerType, BattleSystem_GetBattleType(battleSys));
    healthbox->curHP = message->curHP;
    healthbox->maxHP = message->maxHP;
    healthbox->damage = message->hpCalcTemp;
    healthbox->level = message->level;

    if (message->hpCalcTemp == 0x7FFF) {
        healthbox->curHP = 0;
        healthbox->damage = 0;
    }

    healthbox->task_10 = SysTask_CreateOnMainQueue(ov12_0225FFDC, healthbox, 1000);
}

void ov12_0225AA6C(BattleSystem *battleSys, BattlerData *battlerData, ExpGaugeUpdateMessage *message) {
    HealthBox *healthbox;

    ((battlerData->healthbox.mainSprite != 0L) ? (void)0 : GF_AssertFail());

    healthbox = &battlerData->healthbox;

    MI_CpuClear8(&healthbox->state, sizeof(u8));

    healthbox->battleSys = battleSys;
    healthbox->command = message->command;
    healthbox->battler = battlerData->battler;
    healthbox->curExp = message->curExp;
    healthbox->maxExp = message->expToNextLevel;
    healthbox->expReward = message->gainedExp - healthbox->curExp;

    if (battlerData->battlerType == 0) {
        healthbox->task_10 = SysTask_CreateOnMainQueue(ov12_02260030, healthbox, 1000);
        return;
    } else {
        BattleController_EmitClearCommand(healthbox->battleSys, healthbox->battler, healthbox->command);
    }
}

void ov12_0225AAE0(BattleSystem *battleSys, BattlerData *battlerData, FaintingSequenceMessage *message) {
    FaintingSequenceData *faintingSequenceData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(FaintingSequenceData));

    if (battlerData->battlerType & 1) {
        faintingSequenceData->face = 2;
    } else {
        faintingSequenceData->face = 0;
    }

    faintingSequenceData->state = 0;
    faintingSequenceData->battleSys = battleSys;
    faintingSequenceData->battlerData = battlerData;
    faintingSequenceData->command = message->command;
    faintingSequenceData->battler = battlerData->battler;
    faintingSequenceData->monSprite = battlerData->monSprite;
    faintingSequenceData->species = message->species;
    faintingSequenceData->gender = message->gender;
    faintingSequenceData->form = message->form;
    faintingSequenceData->personality = message->personality;
    faintingSequenceData->isSubstitute = message->isSubstitute;
    faintingSequenceData->isTransformed = message->isTransformed;

    for (int i = 0; i < 4; i++) {
        faintingSequenceData->moveAnim.species[i] = message->monSpecies[i];
        faintingSequenceData->moveAnim.genders[i] = message->monGenders[i];
        faintingSequenceData->moveAnim.isShiny[i] = message->monShiny[i];
        faintingSequenceData->moveAnim.formNums[i] = message->monFormNums[i];
        faintingSequenceData->moveAnim.personalities[i] = message->monPersonalities[i];
    }

    SysTask_CreateOnMainQueue(ov12_022600F0, faintingSequenceData, 0);
}

void ov12_0225ABB8(BattleSystem *battleSys, BattlerData *battlerData, PlaySoundMessage *message) {
    int pan;

    if (battlerData->battlerType & 1) {
        pan = 117;
    } else {
        pan = -117;
    }

    BattleController_EmitClearCommand(battleSys, battlerData->battler, message->command);
    sub_0200602C(message->sdatID, pan);
}

void ov12_0225ABE8(BattleSystem *battleSys, BattlerData *battlerData) {
    FadeOutData *fadeOutData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(FadeOutData));

    fadeOutData->state = 0;
    fadeOutData->battleSys = battleSys;
    fadeOutData->command = battlerData->data[0];
    fadeOutData->battler = battlerData->battler;

    SysTask_CreateOnMainQueue(ov12_0226037C, fadeOutData, 0);
}

void ov12_0225AC1C(BattleSystem *battleSys, BattlerData *battlerData, ToggleVanishMessage *message) {
    ToggleVanishData *toggleVanishData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(ToggleVanishData));

    toggleVanishData->battleSys = battleSys;
    toggleVanishData->battlerData = battlerData;
    toggleVanishData->command = message->command;
    toggleVanishData->battler = battlerData->battler;
    toggleVanishData->state = 0;
    toggleVanishData->toggleHide = message->toggle;
    toggleVanishData->isSubstitute = message->isSubstitute;

    for (int i = 0; i < 4; i++) {
        toggleVanishData->moveAnim.species[i] = message->species[i];
        toggleVanishData->moveAnim.genders[i] = message->gender[i];
        toggleVanishData->moveAnim.isShiny[i] = message->isShiny[i];
        toggleVanishData->moveAnim.formNums[i] = message->formNum[i];
        toggleVanishData->moveAnim.personalities[i] = message->personality[i];
    }

    SysTask_CreateOnMainQueue(ov12_02260418, toggleVanishData, 0);
}

void ov12_0225ACB0(BattleSystem *battleSys, BattlerData *battlerData, SetStatusIconMessage *message) {
    ((battlerData->healthbox.mainSprite != 0L) ? (void)0 : GF_AssertFail());

    battlerData->healthbox.status = message->status;

    ov12_0226498C(&battlerData->healthbox, battlerData->healthbox.curHP, (1 << 8));
    BattleController_EmitClearCommand(battleSys, battlerData->battler, message->command);
}
