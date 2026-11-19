#include "global.h"

#include "math_util.h"
#include "save.h"
#include "pm_string.h"

struct ImageClips;
struct String;
enum RecordMixedRNGEntry {
    RECORD_MIXED_RNG_PLAYER_ORIGINAL,
    RECORD_MIXED_RNG_PLAYER_OVERRIDE,
    RECORD_MIXED_RNG_QUEUE_0,
    RECORD_MIXED_RNG_QUEUE_1,
    RECORD_MIXED_RNG_QUEUE_2,
    RECORD_MIXED_RNG_QUEUE_3,

    RECORD_MIXED_RNG_MAX,
};
typedef u16 charcode_t;
enum RecordMixedRNGName {
    RECORD_MIXED_RNG_GROUP_NAME,
    RECORD_MIXED_RNG_PLAYER_NAME,
};
typedef struct RecordMixedRNG {
    charcode_t groupName[7 + 1];
    charcode_t playerName[7 + 1];
    u8 gender;
    u8 language;
    u16 dummy;
    u32 seed;
    u32 rand;
} RecordMixedRNG;
void sub_0202C824(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, int gender);
void sub_0202C848(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, int language);
void CopyStringToU16Array(const String *src, charcode_t *dst, u32 dstSize);
typedef struct ImageClips ImageClips;
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

void sub_0202C7C0(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, u32 seed);
u32 sub_0202C7DC(RecordMixedRNG *rngCollection);
const charcode_t *sub_0202C7E0(const RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, enum RecordMixedRNGName nameChoice);
void sub_0202C7F8(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, enum RecordMixedRNGName nameChoice, String *outString);
void sub_0202C824(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, int gender);
int sub_0202C830(const RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry);
int sub_0202C83C(const RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry);
void sub_0202C848(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, int language);
ImageClips *Save_FriendGroup_Get(SaveData *saveData);

void sub_0202C7C0(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, u32 seed) {
    rngCollection[entry].seed = seed;
    rngCollection[entry].rand = PRandom(seed);
}

u32 sub_0202C7DC(RecordMixedRNG *rngCollection) {
    return rngCollection[RECORD_MIXED_RNG_PLAYER_OVERRIDE].rand;
}

const charcode_t *sub_0202C7E0(const RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, enum RecordMixedRNGName nameChoice) {
    if (nameChoice == RECORD_MIXED_RNG_GROUP_NAME) {
        return rngCollection[entry].groupName;
    }

    return rngCollection[entry].playerName;
}

void sub_0202C7F8(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, enum RecordMixedRNGName nameChoice, String *outString) {
    charcode_t *name;
    if (nameChoice == RECORD_MIXED_RNG_GROUP_NAME) {
        name = rngCollection[entry].groupName;
        CopyStringToU16Array(outString, name, 7 + 1);
    } else {
        name = rngCollection[entry].playerName;
        CopyStringToU16Array(outString, name, 7 + 1);
    }
}

void sub_0202C824(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, int gender) {
    rngCollection[entry].gender = gender;
}

int sub_0202C830(const RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry) {
    return rngCollection[entry].gender;
}

int sub_0202C83C(const RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry) {
    return rngCollection[entry].language;
}

void sub_0202C848(RecordMixedRNG *rngCollection, enum RecordMixedRNGEntry entry, int language) {
    rngCollection[entry].language = language;
}

ImageClips *Save_FriendGroup_Get(SaveData *saveData) {
    return SaveArray_Get(saveData, SAVE_TABLE_ENTRY_IMAGE_CLIPS);
}
