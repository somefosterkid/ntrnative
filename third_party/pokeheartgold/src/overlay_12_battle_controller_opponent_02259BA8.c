#include "global.h"
#include "constants/heap.h"
#include "battle/battle.h"
#include "battle/battle_message_structs.h"
#include "overlay_12_mon_encounter_data.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_ball_capsule_seal_effect.h"
#include "overlay_12_mon_show_data.h"
#include "battle/battle_system.h"

struct BallCapsuleSealEffect_t;

struct UnkBallData;

struct BattleAnimSystem;

struct BattleMonOBJData;

struct BattleSystem;

struct OpponentData;

struct OpponentDataSysTaskFuncs;

struct BgConfig;

struct BattleHpBar;

struct ManagedSprite;

struct MonShowData;

struct MonShowMessage;

struct MoveAnimation;

struct NARC;

struct Pokepic;

struct PokepicShadow;

struct PokepicTemplate;

struct PokepicDrawParam;

struct OpponentDataCursorPos;

struct SpriteAnimFrame;

struct SysTask;

struct SysTaskQueue;

struct Window;

enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct BattleMonOBJData BattleMonOBJData;

SysTask * SysTask_CreateOnMainQueue(SysTaskFunc func, void *data, u32 priority);

u8 GetNatureFromPersonality(u32 monPersonality);

void GetMonSpriteCharAndPlttNarcIdsEx(PokepicTemplate *spriteTemplate, u16 species, u8 gender, u8 face, u8 shiny, u8 form, u32 personality);

u8 GetMonPicHeightBySpeciesGenderForm(u16 species, u8 gender, u8 face, u8 form, u32 personality);

void sub_020729D8(NARC *narc, s8 *yOffset, u16 species);

void sub_020729FC(NARC *narc, s8 *xOffsetShadow, u16 species);

void sub_02072A20(NARC *narc, u8 *shadowSize, u16 species);

void ov12_02261284(OpponentData *battlerData);


u32 BattleSystem_GetBattleType(BattleSystem *battleSys);

void Sound_Set2PokemonCriesAllowed(BOOL allowed);

void ov12_0225B960(SysTask *task, void *data);

void ov12_0225BE38(SysTask *task, void *data);


void ov12_02259BA8(BattleSystem *battleSys, OpponentData *battlerData, MonShowMessage *message);

void sub_02005B58(u8 allowed);

void ov12_02259BA8(BattleSystem *battleSys, OpponentData *battlerData, MonShowMessage *message) {
    BOOL isShiny;
    MonShowData *monShowData;
    u32 battleType;
    BattleSystem_GetBattleType(battleSys);
    monShowData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(MonShowData));

    if (battlerData->battlerType & 1) {
        monShowData->face = 2;
    } else {
        monShowData->face = 0;
    }

    isShiny = !!message->isShiny;

    GetMonSpriteCharAndPlttNarcIdsEx(&monShowData->spriteTemplate, message->species, message->gender, monShowData->face, isShiny, message->formNum, message->personality);

    monShowData->yOffset = GetMonPicHeightBySpeciesGenderForm(message->species, message->gender, monShowData->face, message->formNum, message->personality);

    sub_020729D8(battlerData->narc, &monShowData->height, message->species);
    sub_020729FC(battlerData->narc, &monShowData->shadowXOffset, message->species);
    sub_02072A20(battlerData->narc, &monShowData->shadowSize, message->species);
    ov12_02261284(battlerData);

    monShowData->battleSys = battleSys;
    monShowData->battlerData = battlerData;
    monShowData->state = 0;
    monShowData->delay = 0;
    monShowData->command = message->command;
    monShowData->battler = battlerData->battler;
    monShowData->species = message->species;
    monShowData->formNum = message->formNum;
    monShowData->battlerType = battlerData->battlerType;
    monShowData->cryMod = message->cryModulation;
    monShowData->selectedPartySlot = message->selectedPartySlot;
    monShowData->nature = GetNatureFromPersonality(message->personality);
    monShowData->capturedBall = message->capturedBall;
    monShowData->isShiny = message->isShiny;
    monShowData->isQuickSendOut = 0;

    sub_02005B58(1);
    battleType = BattleSystem_GetBattleType(battleSys);
    if (ov12_0223C140(battleSys, battlerData->battler) != 0xFF) {
        if ((battleType & 2) && !(battleType & 8) && battlerData->battlerType > 3) {
            SysTask_CreateOnMainQueue(ov12_0225B960, monShowData, 0);
        } else {
            SysTask_CreateOnMainQueue(ov12_0225BE38, monShowData, 0);
        }
    } else {
        SysTask_CreateOnMainQueue(ov12_0225B960, monShowData, 0);
    }
}
