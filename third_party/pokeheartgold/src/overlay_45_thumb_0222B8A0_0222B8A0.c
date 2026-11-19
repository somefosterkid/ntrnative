#include "global.h"

#include "constants/heap.h"
#include "player_data.h"
#include "overlay_45_wifi_plaza_system.h"
#include "seal_case.h"
#include "pokemon_types_def.h"
#include "pokedex.h"

struct BallCapsule;

struct BallSeal;

struct BoxPokemon;

struct MailMessage;

struct GameTime;

struct Mail;

struct Party;

struct PartyPokemon;

struct Pokedex;

struct Pokemon;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct String;

struct PlayerProfile;

struct WiFiHistory;

struct WiFiPlazaProfile;

enum Version {
    VERSION_NONE = 0,
    VERSION_SAPPHIRE_pl,
    VERSION_RUBY_pl,
    VERSION_EMERALD_pl,
    VERSION_FIRERED_pl,
    VERSION_LEAFGREEN_pl,
    VERSION_UNUSED_6,
    VERSION_HEARTGOLD_pl,
    VERSION_SOULSILVER_pl,
    VERSION_UNUSED_9,
    VERSION_DIAMOND_pl,
    VERSION_PEARL_pl,
    VERSION_PLATINUM_pl,
    VERSION_UNUSED_13,
    VERSION_UNUSED_14,
    VERSION_GAMECUBE_pl,
};





typedef u16 charcode_t;

enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};

enum SaveTableEntryID {
    SAVE_TABLE_ENTRY_SYSTEM = 0,
    SAVE_TABLE_ENTRY_PLAYER,
    SAVE_TABLE_ENTRY_PARTY,
    SAVE_TABLE_ENTRY_BAG,
    SAVE_TABLE_ENTRY_VARS_FLAGS,
    SAVE_TABLE_ENTRY_POKETCH,
    SAVE_TABLE_ENTRY_FIELD_PLAYER_STATE,
    SAVE_TABLE_ENTRY_POKEDEX,
    SAVE_TABLE_ENTRY_DAYCARE,
    SAVE_TABLE_ENTRY_PAL_PAD,
    SAVE_TABLE_ENTRY_MISC,
    SAVE_TABLE_ENTRY_FIELD_OVERWORLD_STATE,
    SAVE_TABLE_ENTRY_UNDERGROUND,
    SAVE_TABLE_ENTRY_REGULATION_BATTLES,
    SAVE_TABLE_ENTRY_IMAGE_CLIPS,
    SAVE_TABLE_ENTRY_MAILBOX,
    SAVE_TABLE_ENTRY_POFFINS,
    SAVE_TABLE_ENTRY_RECORD_MIXED_RNG,
    SAVE_TABLE_ENTRY_JOURNAL,
    SAVE_TABLE_ENTRY_TRAINER_CASE,
    SAVE_TABLE_ENTRY_GAME_RECORDS,
    SAVE_TABLE_ENTRY_SEAL_CASE,
    SAVE_TABLE_ENTRY_CHATOT,
    SAVE_TABLE_ENTRY_FRONTIER,
    SAVE_TABLE_ENTRY_RIBBONS,
    SAVE_TABLE_ENTRY_ENCOUNTERS,
    SAVE_TABLE_ENTRY_GLOBAL_TRADE,
    SAVE_TABLE_ENTRY_TV_BROADCAST,
    SAVE_TABLE_ENTRY_RANKINGS,
    SAVE_TABLE_ENTRY_WIFI_LIST,
    SAVE_TABLE_ENTRY_WIFI_HISTORY,
    SAVE_TABLE_ENTRY_MYSTERY_GIFT,
    SAVE_TABLE_ENTRY_PAL_PARK_TRANSFER,
    SAVE_TABLE_ENTRY_LINK_CONTEST_RECORDS,
    SAVE_TABLE_ENTRY_UNLOCKED_EASY_CHAT_WORDS,
    SAVE_TABLE_ENTRY_EMAIL,
    SAVE_TABLE_ENTRY_WIFI_PLAZA_QUESTIONS,
    SAVE_TABLE_ENTRY_PC_BOXES,

    SAVE_TABLE_ENTRY_MAX
};

typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;

typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;

typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;

typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;

void String_Delete(String *string);

void CopyStringToU16Array(const String *src, charcode_t *dst, u32 dstSize);






BOOL PlayerProfile_GetGameClearFlag(PlayerProfile *info);


typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;

typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

int Party_GetCount(const Party *party);

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

Party *SaveArray_Party_Get(SaveData *saveData);

u32 GetMonData(Pokemon *mon, int param, void *dest);

BOOL Pokedex_GetNatDexFlag(const Pokedex *pokedex);

Pokedex *Save_Pokedex_Get(SaveData *saveData);

PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);

typedef struct GameTime {
    BOOL canary;
    RTCDate date;
    RTCTime time;
    u32 day;
    s64 startTimestamp;
    s64 firstCompletionTimestamp;
    u32 penaltyInMinutes;
} GameTime;

GameTime *Save_SysInfo_RTC_Get(SaveData *saveData);

WiFiHistory *Save_WiFiHistory_Get(SaveData *saveData);

u8 WifiHistory_GetPlayerCountry(const WiFiHistory *wiFiHistory);

u8 WiFiHistory_GetPlayerRegion(const WiFiHistory *wiFiHistory);

void ov45_0222BAC4(UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData);

u32 ov45_0222CCE4(u32 param0);

void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *trainerInfo, enum HeapID heapID);

void ov66_0222FBF0(UnkStruct_ov66_0222FBF0 *param0, enum HeapID heapID);

void ov66_0222FC0C(UnkStruct_ov66_0222FBF0 *param0, const UnkStruct_ov66_0222F6C4 *param1, enum HeapID heapID);

void ov45_0222B8A0(UnkStruct_ov66_0222F6C4 *param0, SaveData *saveData, enum HeapID heapID);

void ov45_0222B8A0(UnkStruct_ov66_0222F6C4 *param0, SaveData *saveData, enum HeapID heapID) {

    PlayerProfile *trainerInfo;
    Party *party;
    WiFiHistory *wiFiHistory;
    Pokedex *pokedex;
    GameTime *gameTime;

    trainerInfo = Save_PlayerData_GetProfile(saveData);
    party = SaveArray_Party_Get(saveData);
    pokedex = Save_Pokedex_Get(saveData);
    wiFiHistory = Save_WiFiHistory_Get(saveData);
    gameTime = Save_SysInfo_RTC_Get(saveData);

    String *trainerName = PlayerProfile_GetPlayerName_NewString(trainerInfo, heapID);
    CopyStringToU16Array(trainerName, param0->profile.trainerName, (sizeof(param0->profile.trainerName) / sizeof(*(param0->profile.trainerName))));
    CopyStringToU16Array(trainerName, param0->unk_00, (sizeof(param0->unk_00) / sizeof(*(param0->unk_00))));
    String_Delete(trainerName);

    param0->profile.unk_00 = -1;
    param0->profile.trainerID = PlayerProfile_GetTrainerID(trainerInfo);

    int partyCount = Party_GetCount(party);
    for (int partyIdx = 0; partyIdx < 6; partyIdx++) {
        if (partyIdx < partyCount) {
            Pokemon *pokemon = Party_GetMonByIndex(party, partyIdx);
            param0->profile.partySpecies[partyIdx] = GetMonData(pokemon, MON_DATA_SPECIES, 0L);
            param0->profile.partyForms[partyIdx] = GetMonData(pokemon, MON_DATA_FORM, 0L);
            param0->profile.partyIsEgg[partyIdx] = GetMonData(pokemon, MON_DATA_IS_EGG, 0L);
        } else {
            param0->profile.partySpecies[partyIdx] = SPECIES_BAD_EGG;
        }
    }

    param0->profile.trainerGender = PlayerProfile_GetTrainerGender(trainerInfo);
    param0->profile.language = PlayerProfile_GetLanguage(trainerInfo);
    param0->profile.trainerAppearanceIdx = PlayerProfile_GetAvatar(trainerInfo);
    param0->profile.trainerAppearanceIdx = ov45_0222CCE4(param0->profile.trainerAppearanceIdx);
    param0->profile.country = WifiHistory_GetPlayerCountry(wiFiHistory);
    param0->profile.region = WiFiHistory_GetPlayerRegion(wiFiHistory);
    param0->profile.isNationalDexObtained = Pokedex_GetNatDexFlag(pokedex);
    param0->profile.isMainStoryCleared = PlayerProfile_GetGameClearFlag(trainerInfo);
    param0->profile.unk_41 = 0xff;
    param0->profile.unk_43 = 0;
    param0->profile.gameVersion = GAME_VERSION;
    param0->profile.startTimestamp = gameTime->startTimestamp;

    for (int i = 0; i < 12; i++) {
        param0->profile.unk_4C[i] = 24;
        param0->profile.unk_58[i] = -1;
    }

    for (int i = 0; i < 2; i++) {
        param0->profile.unk_88[i] = TYPE_NORMAL;
    }

    param0->profile.unk_8C.questionNumber = 0xffffffff;
    param0->profile.unk_8C.selectedAnswer = 3;

    ov45_0222BAC4(param0, saveData);
}
