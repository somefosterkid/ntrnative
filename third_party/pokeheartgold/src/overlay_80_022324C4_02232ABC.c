#include "global.h"
#include "overlay_80_battle_castle.h"
#include "save.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"
#include "battle_castle_app_args.h"
#include "battle_castle_save.h"
#include "unk_0202D230.h"
#include "save_frontier.h"

struct BallCapsule;
struct BallSeal;
struct BattleArcadeSave;
struct BattleArcadeStreakFlags;
struct BattleCastle;
struct BattleCastleAppArgs;
struct BattleCastlePersistentSave;
struct BattleCastleSave;
struct BattleFactorySave;
struct BattleFactoryStreakFlags;
struct FrontierSave;
struct BattleFrontierStats;
struct BattleHallSave;
struct BattleHallStreakFlags;
struct BattleTowerPartnerData;
struct BattleTowerSave;
struct BoxPokemon;
struct MailMessage;
struct FieldBattleDTO;
struct FrontierEasyChatMessages;
struct FrontierOpponent;
struct FrontierPokemon;
struct FrontierTrainer;
struct Mail;
struct Party;
struct PartyPokemon;
struct Pokemon;
struct PokemonDataBlockA;
struct PokemonDataBlockB;
struct PokemonDataBlockC;
struct PokemonDataBlockD;
struct SaveData;
struct WifiBattleTowerDownloadData;
struct WifiBattleTowerMatchCandidate;
struct WifiBattleTowerRecord;
struct WifiTrainerRecord;
void Heap_Free(void *ptr);
static u16 ov80_02232B44(BattleCastleAppArgs *args, u8 i);
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
void sub_02030CE0(BattleCastleSave *save, BOOL didSave);
void sub_02030CF4(BattleCastleSave *save, u8 field, u8 index, u8 index2, void *value);
BattleCastlePersistentSave *sub_02030E08(SaveData *saveData);
void sub_02030E18(BattleCastlePersistentSave *save, u8 field, u8 challengeType, u8 unused, u8 *value);
u8 ov80_02237B58(u8 challengeType, BOOL includeBothOpponents);
enum BattleFrontierStatsIndex sub_0205C1A0(u8 challengeType);
enum BattleFrontierStatsIndex sub_0205C1C8(u8 challengeType);
enum BattleFrontierStatsIndex sub_0205C1F0(u8 challengeType);
enum BattleFrontierStatsIndex sub_0205C240(u8 challengeType);
int sub_0205C268(u32 statIndex);
FrontierSave *Save_Frontier_GetStatic(SaveData *saveData);
u16 FrontierSave_GetStat(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID);
u16 sub_02031108(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);
u16 sub_0203126C(FrontierSave *frontier, enum BattleFrontierStatsIndex statIndex, int hostFriendID, u16 newValue);

void ov80_02232ABC(BattleCastle *castle);
void ov80_02232AEC(BattleCastle *battleCastle, BattleCastleAppArgs *args);
static u16 ov80_02232B44(BattleCastleAppArgs *args, u8 i);
void ov80_02232B58(BattleCastle *castle, u8 saveType);

void ov80_02232ABC(BattleCastle *castle) {
    if (castle == 0L) {
        return;
    }

    if (castle->playersParty != 0L) {
        Heap_Free(castle->playersParty);
    }

    if (castle->opponentsParty != 0L) {
        Heap_Free(castle->opponentsParty);
    }

    MI_CpuClear8(castle, sizeof(BattleCastle));
    Heap_Free(castle);

    castle = 0L;
}

void ov80_02232AEC(BattleCastle *battleCastle, BattleCastleAppArgs *args) {
    for (int i = 0; i < 6; i++) {
        battleCastle->selectedAppSlots[i] = ov80_02232B44(args, i);
    }

    for (int i = 0; i < 2 * 2; i++) {
        battleCastle->appIdentityUnlocked[i] = args->identityUnlocked[i];
        battleCastle->levelAdjustments[i] = args->levelAdjustments[i];
        battleCastle->appStatsUnlocked[i] = args->statsUnlocked[i];
        battleCastle->appMovesUnlocked[i] = args->movesUnlocked[i];
    }

    battleCastle->partnersCP = args->partnersCP;
}

static u16 ov80_02232B44(BattleCastleAppArgs *args, u8 i) {
    if (i >= 6) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    return args->selectedMonSlot;
}

void ov80_02232B58(BattleCastle *castle, u8 saveType) {
    u8 u8Ptr[4];
    u16 u16Ptr[4];
    u32 u32Ptr[4];
    u32 partySize;
    BattleCastleSave *castleSave = castle->castleSave;
    BattleCastlePersistentSave *persistentSave = sub_02030E08(castle->saveData);

    FrontierSave *frontier = Save_Frontier_GetStatic(castle->saveData);
    (void)(ov80_02237B58(castle->challengeType, 1));

    *u8Ptr = castle->challengeType;
    sub_02030CF4(castle->castleSave, 0, 0, 0, u8Ptr);
    sub_02030CE0(castle->castleSave, 1);

    *u8Ptr = castle->currentBattle;

    sub_02030CF4(castle->castleSave, 1, 0, 0, u8Ptr);
    sub_02031108(frontier, sub_0205C1A0(castle->challengeType), sub_0205C268(sub_0205C1A0(castle->challengeType)), castle->currentStreak);

    if (saveType != 2) {
        u16 recordStreak = FrontierSave_GetStat(frontier, sub_0205C1C8(castle->challengeType), sub_0205C268(sub_0205C1C8(castle->challengeType)));
        sub_0203126C(frontier, sub_0205C1C8(castle->challengeType), sub_0205C268(sub_0205C1C8(castle->challengeType)), castle->currentStreak);
        u16 newRecord = FrontierSave_GetStat(frontier, sub_0205C1C8(castle->challengeType), sub_0205C268(sub_0205C1C8(castle->challengeType)));
        u32 latestStreak = FrontierSave_GetStat(frontier, sub_0205C1F0(castle->challengeType), sub_0205C268(sub_0205C1F0(castle->challengeType)));

        if (castle->currentStreak == recordStreak) {
            sub_0203126C(frontier, sub_0205C240(castle->challengeType), sub_0205C268(sub_0205C240(castle->challengeType)), latestStreak);
        } else if (recordStreak < newRecord) {
            sub_02031108(frontier, sub_0205C240(castle->challengeType), sub_0205C268(sub_0205C240(castle->challengeType)), latestStreak);
        }

        *u8Ptr = castle->saveStreak;
        sub_02030E18(persistentSave, 9, castle->challengeType, 0, u8Ptr);

        if (castle->challengeType == 3) {
            sub_02031108(frontier, STAT_CASTLE_WFC_STREAK_ACTIVE, sub_0205C268(STAT_CASTLE_WFC_STREAK_ACTIVE), castle->saveStreak);
        }
    }

    for (u16 i = 0; i < 7 * 2; i++) {
        *u16Ptr = castle->trainerIDs[i];
        sub_02030CF4(castle->castleSave, 6, i, 0, u16Ptr);
    }

    for (u16 i = 0; i < 3; i++) {
        *u8Ptr = castle->partySlots[i];
        sub_02030CF4(castle->castleSave, 7, i, 0, u8Ptr);
    }

    partySize = Party_GetCount(castle->playersParty);

    for (u16 i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(castle->playersParty, i);

        *u16Ptr = GetMonData(mon, MON_DATA_HP, 0L);
        sub_02030CF4(castle->castleSave, 2, i, 0, u16Ptr);

        *u8Ptr = GetMonData(mon, MON_DATA_MOVE1_PP, 0L);
        sub_02030CF4(castle->castleSave, 3, i, 0, u8Ptr);

        *u8Ptr = GetMonData(mon, MON_DATA_MOVE2_PP, 0L);
        sub_02030CF4(castle->castleSave, 3, i, 1, u8Ptr);

        *u8Ptr = GetMonData(mon, MON_DATA_MOVE3_PP, 0L);
        sub_02030CF4(castle->castleSave, 3, i, 2, u8Ptr);

        *u8Ptr = GetMonData(mon, MON_DATA_MOVE4_PP, 0L);
        sub_02030CF4(castle->castleSave, 3, i, 3, u8Ptr);

        *u32Ptr = GetMonData(mon, MON_DATA_STATUS, 0L);
        sub_02030CF4(castle->castleSave, 4, i, 0, u32Ptr);

        *u16Ptr = GetMonData(mon, MON_DATA_HELD_ITEM, 0L);
        sub_02030CF4(castle->castleSave, 5, i, 0, u16Ptr);
    }

    partySize = Party_GetCount(castle->opponentsParty);

    for (u16 i = 0; i < partySize; i++) {
        (void)(Party_GetMonByIndex(castle->opponentsParty, i));
        *u16Ptr = castle->monSetIDs[i];

        sub_02030CF4(castle->castleSave, 8, i, 0, u16Ptr);
    }
}
