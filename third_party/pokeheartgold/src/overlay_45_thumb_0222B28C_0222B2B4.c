#include "global.h"

#include "constants/heap.h"
#include "overlay_45_wifi_plaza_system.h"


struct PasswordWordBank;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct PlayerProfile;

struct UnkStruct_0207E060_t;

struct UnkStruct_ov66_0223177C_t;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef u16 charcode_t;

typedef enum {
    UnkEnum_ov66_02231E54_00 = 0,
    UnkEnum_ov66_02231E54_01 = 1 << 0,
    UnkEnum_ov66_02231E54_02 = 1 << 1,
} UnkEnum_ov66_02231E54;

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


extern u32 ov45_0222A9CC(const WiFiPlazaProfile *profile);

BOOL ov45_0222AA28(const WiFiPlazaProfile *profile);

extern u32 ov45_0222AA84(const WiFiPlazaProfile *profile);

extern u8 ov45_0222AAA8(const WiFiPlazaProfile *profile);

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

extern void ov45_0222D500(UnkStruct_ov66_02231428 *param0, const s64 *param1);

typedef struct {
    u32 unk_00;
} UnkStruct_ov66_02231B24;

extern void ov45_0222D8C8(UnkStruct_ov66_0223177C *param0, u32 param1, u32 param2, UnkEnum_ov66_02231E54 param3);

extern void ov45_0222D8F0(UnkStruct_ov66_0223177C *param0, u32 param1);

extern void ov45_0222DC08(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_02231B24 *param1, const UnkStruct_ov66_02232068 *param2);

extern void ov45_0222EA4C(s32 param0, s64 *param1);

extern s32 ov45_0222EA78(void);

u32 ov45_0222EC68(s32 param0);

extern BOOL ov45_0222F484(s32 param0);

extern s32 ov45_0222F4AC(s32 param0);

extern UnkStruct_ov66_02231428 ov45_02230F94(u16 param0, u8 param1, UnkStruct_ov66_02231428 param2, enum HeapID heapID);

typedef struct PasswordWordBank {
    u32 length;
    u32 *words;
} PasswordWordBank;

extern void ov45_0222BA3C(WiFiPlazaSystem *param0);

extern void ov45_0222BAC4(UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData);

BOOL ov45_0222BADC(const UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData);

extern void ov45_0222BCE4(UnkStruct_ov66_0222FBF0 *param0, const UnkStruct_ov66_0222F6C4 *param1, enum HeapID heapID);

void ov45_0222C944(UnkStruct_ov66_02232068 *param0, u32 param1, BOOL param2);

extern void ov45_0222C9A0(UnkStruct_ov66_022308A0 *param0, u32 param1, BOOL param2, u32 param3);

extern BOOL ov45_0222CBD0(UnkStruct_ov66_02230A6C *param0, s32 param1, s32 param2);

WiFiPlazaSystem *ov66_0222DDF0(SaveData *saveData, enum HeapID heapID);

void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *trainerInfo, enum HeapID heapID);

void ov66_0222F7C8(UnkStruct_ov66_0222F6C4 *param0, SaveData *saveData, enum HeapID heapID);

void ov66_0222FBF0(UnkStruct_ov66_0222FBF0 *param0, enum HeapID heapID);

void ov45_0222B2B4(s32 param0, const void *param1, void *param2, BOOL param3);

void ov45_0222B2B4(s32 param0, const void *param1, void *param2, BOOL param3) {
    WiFiPlazaSystem *v0;
    u32 v1;
    const WiFiPlazaProfile *v2;
    s32 v3;

    v0 = param2;

    if (param3) {
        ov45_0222BCE4(&v0->unk_508, &v0->unk_108, v0->heapID);

        if (ov45_0222BADC(&v0->unk_108, v0->saveData) == 0) {
            v0->unk_52C = 1;
        }

        v1 = ov45_0222EC68(param0);
        ov45_0222D8C8((UnkStruct_ov66_0223177C *)(v0->unk_04.unk_00), v1, v0->unk_108.profile.trainerGender, (UnkEnum_ov66_02231E54)(UnkEnum_ov66_02231E54_01 | UnkEnum_ov66_02231E54_02));
        v0->unk_108.profile.unk_00 = param0;

        {
            s64 v5;

            ov45_0222EA4C(param0, &v5);
            ov45_0222D500(&v0->unk_108.profile.unk_18, &v5);
        }

        {
            u32 v6, v7;
            BOOL v8;

            v6 = ov45_0222AA84(&v0->unk_108.profile);
            v7 = ov45_0222AAA8(&v0->unk_108.profile);
            v8 = ov45_0222AA28(&v0->unk_108.profile);

            if ((v6 != 0) && (v8 == 1)) {
                v0->unk_108.profile.unk_1C = ov45_02230F94(v6, v7, v0->unk_108.profile.unk_18, v0->heapID);
            } else {
                v0->unk_108.profile.unk_1C.unk_00_val2_unk_00 = 12;
                v0->unk_108.profile.unk_1C.unk_00_val2_unk_01 = 0;
                v0->unk_108.profile.unk_1C.unk_00_val2_unk_02 = 0;
            }
        }

        ov45_0222BAC4(&v0->unk_108, v0->saveData);
        ov45_0222BA3C(v0);
    } else {
        v2 = (const WiFiPlazaProfile *)param1;
        v3 = ov45_0222EA78();
        v1 = ov45_0222EC68(param0);

        ov45_0222D8C8(v0->unk_04.unk_00, v1, ov45_0222A9CC(v2), UnkEnum_ov66_02231E54_00);

        if (v3 == param0) {
            ov45_0222D8F0(v0->unk_04.unk_00, v1);
        }

        if (ov45_0222CBD0(&v0->unk_4BC, param0, -1)) {
            ov45_0222BA3C(v0);
        }
    }

    v1 = ov45_0222EC68(param0);
    v0->unk_F8.unk_00 |= 1 << v1;

    if (ov45_0222F484(param0) == 1) {
        u32 v9;

        ov45_0222C944(&v0->unk_3CB, v1, 1);

        {
            UnkStruct_ov66_02231B24 v10;

            v10.unk_00 = v1;
            ov45_0222DC08(v0->unk_04.unk_00, &v10, &v0->unk_3CB);
        }

        v9 = ov45_0222F4AC(param0);

        if (v9 != 0) {
            ov45_0222C9A0(&v0->unk_3E4, v1, 1, v9);
        } else {
            ov45_0222C9A0(&v0->unk_3E4, v1, 0, v9);
        }
    } else {
        ov45_0222C944(&v0->unk_3CB, v1, 0);
        ov45_0222C9A0(&v0->unk_3E4, v1, 0, 0);
    }
}
