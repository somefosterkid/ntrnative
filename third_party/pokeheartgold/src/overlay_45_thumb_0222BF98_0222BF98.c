#include "global.h"

#include "overlay_45_wifi_plaza_api.h"

#include "constants/heap.h"
#include "player_data.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "overlay_45_wifi_plaza_system.h"

struct PasswordWordBank;

struct SaveBlockInfo;

struct SaveData;

struct SaveDataBody;

struct SaveDataState;

struct SavePageInfo;

struct PlayerProfile;

struct UnkStruct_0207E060_t;

struct UnkStruct_ov66_0222DFF8_t;

struct UnkStruct_ov66_0223177C_t;

struct WiFiPlazaProfile;

typedef struct UnkStruct_ov66_0223177C_t UnkStruct_ov66_0223177C;

typedef u16 charcode_t;

enum PlazaMinigame {
    UnkEnum_ov66_022324D0_00,
    UnkEnum_ov66_022324D0_01,
    UnkEnum_ov66_022324D0_02,
    UnkEnum_ov66_022324D0_03,
    UnkEnum_ov66_022324D0_04,
    UnkEnum_ov66_022324D0_05,
    UnkEnum_ov66_022324D0_06,
    UnkEnum_ov66_022324D0_07,
    UnkEnum_ov66_022324D0_08,
    UnkEnum_ov66_022324D0_09,
    UnkEnum_ov66_022324D0_10,
    UnkEnum_ov66_022324D0_11,
    UnkEnum_ov66_022324D0_12 = UnkEnum_ov66_022324D0_02 + 1,
};

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


void ov45_0222A844(const WiFiPlazaProfile *profile, PlayerProfile *param1, enum HeapID heapID);

typedef struct UnkStruct_0207E060_t UnkStruct_0207E060;

typedef struct {
    enum PlazaMinigame unk_00;
    u32 unk_04;
    const PlayerProfile *unk_08;
    const PlayerProfile *unk_0C;
    const PlayerProfile *unk_10;
    const PlayerProfile *unk_14;
    u16 unk_18;
    u16 unk_1A;
    u16 unk_1C;
    u16 unk_1E;
    BOOL unk_20;
} UnkStruct_ov66_02231908;

void ov45_0222D9EC(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_02231908 *param1);

u32 ov45_0222EC68(s32 param0);

typedef struct {
    s32 unk_00[4];
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13_0 : 7;
    u8 unk_13_7 : 1;
} UnkStruct_ov66_0222E990;

typedef struct PasswordWordBank {
    u32 length;
    u32 *words;
} PasswordWordBank;

WiFiPlazaSystem *ov66_0222DDF0(SaveData *saveData, enum HeapID heapID);

void ov66_0222EA10(WiFiPlazaSystem *param0, enum PlazaMinigame param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, BOOL param7);

void ov66_0222EAB4(WiFiPlazaSystem *param0, enum PlazaMinigame param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6);

void ov66_0222F7C8(UnkStruct_ov66_0222F6C4 *param0, SaveData *saveData, enum HeapID heapID);

void ov66_0222FBF0(UnkStruct_ov66_0222FBF0 *param0, enum HeapID heapID);

void ov66_0222FC0C(UnkStruct_ov66_0222FBF0 *param0, const UnkStruct_ov66_0222F6C4 *param1, enum HeapID heapID);

void ov45_0222BF98(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

void ov45_0222BF98(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1) {
    const WiFiPlazaProfile *v0;
    int v1;
    PlayerProfile *v2[4];
    u32 v3;
    UnkStruct_ov66_02231908 v4;

    if ((param1->unk_10 > 4) || (param1->unk_10 <= 0)) {
        return;
    }

    if (param1->unk_13_7 == 1) {
        for (v1 = 0; v1 < 4; v1++) {
            if (v1 < param1->unk_10) {
                v3 = ov45_0222EC68(param1->unk_00[v1]);

                if (v3 != 0xffffffff) {
                    v0 = ov45_0222A578(param0, v3);
                    ov45_0222A844(v0, param0->unk_04.trainerInfos[v1], param0->heapID);
                    v2[v1] = param0->unk_04.trainerInfos[v1];
                } else {
                    return;
                }
            } else {
                v2[v1] = 0L;
            }
        }
    } else {
        for (v1 = 0; v1 < 4; v1++) {
            if (v1 == 0) {
                u32 v5 = ov45_0222EC68(param1->unk_00[v1]);

                if (v5 != 0xffffffff) {
                    v0 = ov45_0222A578(param0, v5);
                    ov45_0222A844(v0, param0->unk_04.trainerInfos[v1], param0->heapID);
                    v2[v1] = param0->unk_04.trainerInfos[v1];
                } else {
                    return;
                }
            } else {
                v2[v1] = 0L;
            }
        }
    }

    v4.unk_00 = (enum PlazaMinigame)(param1->unk_13_0);
    v4.unk_04 = param1->unk_10;
    v4.unk_08 = v2[0];
    v4.unk_0C = v2[1];
    v4.unk_10 = v2[2];
    v4.unk_14 = v2[3];
    v4.unk_18 = ov45_0222EC68(param1->unk_00[0]);
    v4.unk_1A = ov45_0222EC68(param1->unk_00[1]);
    v4.unk_1C = ov45_0222EC68(param1->unk_00[2]);
    v4.unk_1E = ov45_0222EC68(param1->unk_00[3]);
    v4.unk_20 = param1->unk_13_7;

    ov45_0222D9EC(param0->unk_04.unk_00, &v4);
}

typedef struct {
    enum PlazaMinigame unk_00;
    u32 unk_04;
    const PlayerProfile *unk_08;
    u16 unk_0C;
} UnkStruct_ov66_0223199C;

void ov45_0222DA80(UnkStruct_ov66_0223177C *param0, const UnkStruct_ov66_0223199C *param1);

void ov45_0222C0CC(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1);

void ov45_0222C0CC(WiFiPlazaSystem *param0, const UnkStruct_ov66_0222E990 *param1) {
    const WiFiPlazaProfile *v0;
    u32 v2;
    UnkStruct_ov66_0223199C v3;

    if ((param1->unk_10 > 8) || (param1->unk_10 <= 0)) {
        return;
    }

    v2 = ov45_0222EC68(param1->unk_00[0]);

    if (v2 != 0xffffffff) {
        v0 = ov45_0222A578(param0, v2);
        ov45_0222A844(v0, param0->unk_04.trainerInfos[0], param0->heapID);
    } else {
        return;
    }

    v3.unk_00 = (enum PlazaMinigame)(param1->unk_13_0);
    v3.unk_04 = param1->unk_10;
    v3.unk_08 = param0->unk_04.trainerInfos[0];
    v3.unk_0C = ov45_0222EC68(param1->unk_00[0]);

    ov45_0222DA80(param0->unk_04.unk_00, &v3);
}
