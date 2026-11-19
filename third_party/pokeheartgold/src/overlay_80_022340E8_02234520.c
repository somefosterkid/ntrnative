#include "global.h"
#include "overlay_80_battle_arcade.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "overlay_12_field_battle_dto.h"
#include "save_frontier.h"

struct BallCapsule;
struct BallSeal;
struct BattleArcade;
struct BattleArcadeAppArgs;
struct BattleArcadeSave;
struct BattleArcadeStreakFlags;
struct FrontierSave;
struct BoxPokemon;
struct MailMessage;
struct FieldBattleDTO;
struct FrontierOpponent;
struct FrontierPokemon;
struct FrontierTrainer;
struct Mail;
struct ManagedSprite;
struct Party;
struct PartyPokemon;
struct Pokemon;
struct PokemonDataBlockA;
struct PokemonDataBlockB;
struct PokemonDataBlockC;
struct PokemonDataBlockD;
struct SaveData;
void Heap_Free(void *ptr);
typedef struct BattleArcadeAppArgs {
    SaveData *saveData;
    u8 challengeType;
    u8 padding_05[2];
    u8 performance;
    int *weather;
    u8 *rouletteSpeed;
    u8 *selectedEffect;
    u16 *randomIndex;
    u16 partnersStreak;
    u8 padding_1A[2];
    u16 currentStreak;
    u16 round;
    u8 padding_20[16];
    Party *party;
    Party *opponentsParty;
    u16 cursorPos;
    u8 padding_3A[2];
    void *battleArcade;
    u8 padding_3E[2];
    u8 randomizeCursorMovement;
    u8 padding_41[3];
} BattleArcadeAppArgs;
static u16 ov80_02234574(BattleArcadeAppArgs *args, u8 i);
typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;
typedef u16 charcode_t;
typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;
int Party_GetCount(const Party *party);
Pokemon *Party_GetMonByIndex(const Party *party, int slot);
u32 GetMonData(Pokemon *mon, int param, void *dest);
void sub_02030EA0(BattleArcadeSave *save, BOOL didSave);
void sub_02030EB4(BattleArcadeSave *save, u8 field, u8 index, u8 unused, const void *value);
BattleArcadeStreakFlags *sub_02030FA0(SaveData *saveData);
void sub_02030FB0(BattleArcadeStreakFlags *flags, u8 field, u8 challengeType, u8 unused, const void *value);
u8 BattleArcade_GetOpponentMonCount(u8 challengeType, BOOL includeBothOpponents);

/* DS: the callers were built against the wider return type below; the PC takes the definition's (abilint RET) */
#if defined(SDK_PORT) || defined(NONMATCHING)
u8 sub_0205C2C0(u8 challengeType);
u8 sub_0205C2E8(u8 challengeType);
#else
enum BattleFrontierStatsIndex sub_0205C2C0(u8 challengeType);
enum BattleFrontierStatsIndex sub_0205C2E8(u8 challengeType);
#endif
int sub_0205C268(u32 statIndex);
FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
u16 sub_02031108(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);
u16 sub_0203126C(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);

void BattleArcadeData_Free(BattleArcade *arcade);
void ov80_02234550(BattleArcade *battleArcade, BattleArcadeAppArgs *args);
static u16 ov80_02234574(BattleArcadeAppArgs *args, u8 i);
void ov80_02234588(BattleArcade *arcade, u8 saveType);

void BattleArcadeData_Free(BattleArcade *arcade) {
    if (arcade == 0L) {
        return;
    }

    if (arcade->playersParty != 0L) {
        Heap_Free(arcade->playersParty);
    }

    if (arcade->opponentsParty != 0L) {
        Heap_Free(arcade->opponentsParty);
    }

    MI_CpuClear8(arcade, sizeof(BattleArcade));
    Heap_Free(arcade);

    arcade = 0L;
}

void ov80_02234550(BattleArcade *battleArcade, BattleArcadeAppArgs *args) {
    for (int i = 0; i < 6; i++) {
        battleArcade->appCursorPos[i] = ov80_02234574(args, i);
    }
}

static u16 ov80_02234574(BattleArcadeAppArgs *args, u8 i) {
    if (i >= 6) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    return args->cursorPos;
}

void ov80_02234588(BattleArcade *arcade, u8 saveType) {
    u8 u8Ptr[4];
    u16 u16Ptr[4];
    BattleArcadeSave *arcadeSave = arcade->arcadeSave;
    BattleArcadeStreakFlags *persistentSave = sub_02030FA0(arcade->saveData);

    (void)(BattleArcade_GetOpponentMonCount(arcade->challengeType, 1));

    u8Ptr[0] = arcade->challengeType;
    sub_02030EB4(arcade->arcadeSave, 0, 0, 0, u8Ptr);
    sub_02030EA0(arcade->arcadeSave, 1);

    u8Ptr[0] = arcade->currentBattle;
    sub_02030EB4(arcade->arcadeSave, 2, 0, 0, u8Ptr);

    u8Ptr[0] = arcade->rouletteSpeed;
    sub_02030EB4(arcade->arcadeSave, 3, 0, 0, u8Ptr);

    u8Ptr[0] = arcade->cursorRandomized;
    sub_02030EB4(arcade->arcadeSave, 1, 0, 0, u8Ptr);
    sub_02031108(Save_Frontier_GetStatic(arcade->saveData), sub_0205C2C0(arcade->challengeType), sub_0205C268(sub_0205C2C0(arcade->challengeType)), arcade->currentStreak);

    if (saveType != 2) {
        sub_0203126C(Save_Frontier_GetStatic(arcade->saveData), sub_0205C2E8(arcade->challengeType), sub_0205C268(sub_0205C2E8(arcade->challengeType)), arcade->currentStreak);

        u8Ptr[0] = arcade->saveStreak;
        sub_02030FB0(persistentSave, 8, arcade->challengeType, 0, u8Ptr);

        if (arcade->challengeType == 3) {
            sub_02031108(Save_Frontier_GetStatic(arcade->saveData), STAT_ARCADE_WFC_STREAK_ACTIVE, sub_0205C268(STAT_ARCADE_WFC_STREAK_ACTIVE), arcade->saveStreak);
        }
    }

    for (u16 i = 0; i < 7 * 2; i++) {
        u16Ptr[0] = arcade->trainerIDs[i];
        sub_02030EB4(arcade->arcadeSave, 5, i, 0, u16Ptr);
    }

    for (u16 i = 0; i < 3; i++) {
        u8Ptr[0] = arcade->partySlots[i];
        sub_02030EB4(arcade->arcadeSave, 6, i, 0, u8Ptr);
    }

    u32 partySize = Party_GetCount(arcade->playersParty);

    for (u16 i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(arcade->playersParty, i);
        u16Ptr[0] = GetMonData(mon, MON_DATA_HELD_ITEM, 0L);
        sub_02030EB4(arcade->arcadeSave, 4, i, 0, u16Ptr);
    }

    partySize = Party_GetCount(arcade->opponentsParty);

    for (u16 i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(arcade->opponentsParty, i);
        u16Ptr[0] = arcade->monSetIDs[i];
        sub_02030EB4(arcade->arcadeSave, 7, i, 0, u16Ptr);
    }
}
