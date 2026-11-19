#include "global.h"

#include "constants/heap.h"

#include "frontier/overlay_80.h"

#include "heap.h"
#include "options.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_layout.h"
#include "trainer_data.h"
#include "player_data.h"

struct BattleRecording;
struct UnkStruct_0202F298_t;
struct UnkStruct_0202F41C_t;
struct UnkStruct_02030A80_t;
typedef struct UnkStruct_0202F298_t UnkStruct_0202F298;
typedef struct UnkStruct_0202F41C_t UnkStruct_0202F41C;
typedef struct UnkStruct_02030A80_t UnkStruct_02030A80;
typedef struct {
    u16 unk_00;
    u16 unk_02;
} UnkStruct_0202F298_sub1;
typedef u16 charcode_t;
typedef struct {
    u32 unk_00;
    int unk_04;
    int unk_08[4];
    Trainer unk_18[4];
    int unk_E8;
    int unk_EC;
    int unk_F0;
    int unk_F4;
    int unk_F8;
    int unk_FC;
    int unk_100;
    int unk_104;
    int unk_108;
    int unk_10C;
    int unk_110;
    u32 unk_114[4];
    u32 unk_124;
    int unk_128;
    u32 unk_12C;
    u32 unk_130;
    int unk_134[4];
    u16 unk_144;
    u16 unk_146;
    int unk_148;
    u8 unk_14C[4];
} UnkStruct_0202FAA8;
typedef struct UnkStruct_0202FAA8_sub1_t {
    u8 unk_00[4][1024];
} UnkStruct_0202FAA8_sub1;
typedef struct UnkStruct_02078B40_t {
    u32 personality;
    u16 partyDecrypted : 1;
    u16 boxDecrypted : 1;
    u16 checksumFailed : 1;
    u16 : 13;
    u16 species;
    u16 heldItem;
    u32 otID;
    u32 exp;
    u8 friendship;
    u8 ability;
    u8 hpEV;
    u8 atkEV;
    u8 defEV;
    u8 speedEV;
    u8 spAtkEV;
    u8 spDefEV;
    u16 unk_1C[4];
    u8 unk_24[4];
    u8 unk_28[4];
    u32 hpIV : 5;
    u32 atkIV : 5;
    u32 defIV : 5;
    u32 speedIV : 5;
    u32 spAtkIV : 5;
    u32 spDefIV : 5;
    u32 isEgg : 1;
    u32 unk_2C_31 : 1;
    u8 fatefulEncounter : 1;
    u8 gender : 2;
    u8 form : 5;
    u16 unk_32[11];
    u16 unk_48[8];
    u8 pokeball;
    u8 originLanguage;
    u32 unk_5C;
    u8 level;
    u8 unk_61;
    u16 unk_62;
    u16 unk_64;
    u16 unk_66;
    u16 unk_68;
    u16 unk_6A;
    u16 unk_6C;
    u16 unk_6E;
} UnkStruct_02078B40;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    UnkStruct_02078B40 unk_04[6];
} UnkStruct_0202FD30;
struct UnkStruct_0202F298_t {
    UnkStruct_0202FAA8 unk_00;
    UnkStruct_0202FAA8_sub1 unk_150;
    UnkStruct_0202FD30 unk_1150[4];
    PlayerProfile unk_1B68[4];
    Options unk_1BE8;
    u16 unk_1BEA;
    UnkStruct_0202F298_sub1 unk_1BEC;
};
typedef struct BattleRegulation {
    u16 name[12];
    u16 maxTotalLevel;
    u8 teamSize;
    u8 maxLevel;
    s8 heightRestriction;
    s8 weightRestriction;
    u8 evolutionRestriction : 1;
    u8 specialSpeciesRestriction : 1;
    u8 speciesUniqueness : 1;
    u8 itemUniqueness : 1;
    u8 moveRestrictions : 1;
} BattleRegulation;
struct UnkStruct_0202F41C_t {
    u16 unk_00[12];
    u8 unk_18[12];
    u16 unk_24;
    u8 unk_26;
    u8 unk_27;
    BattleRegulation battleRegulation;
    u16 unk_48;
    u8 unk_4A[14];
    u64_a4 unk_58;
    UnkStruct_0202F298_sub1 unk_60;
};
struct UnkStruct_02030A80_t {
    u16 unk_00[8];
    u32 id;
    u8 gender;
    u8 month;
    u8 appearance;
    u8 country;
    u8 region;
    u8 version;
    u8 language;
    u8 isEgg : 1;
    u8 form : 7;
    u16 species;
    u8 unk_1E;
    u8 unk_1F;
    union {
        MailMessage unk_20_val1;
        u16 unk_20_val2[40];
    };
    u8 unk_70[12];
    UnkStruct_0202F298_sub1 unk_7C;
};
typedef struct BattleRecording BattleRecording;
struct BattleRecording {
    u32 unk_00;
    UnkStruct_02030A80 unk_04;
    UnkStruct_0202F41C unk_84;
    UnkStruct_0202F298 unk_E8;
};
LAYOUT_CHECK(BattleRecording_size, sizeof(BattleRecording) == 0x1D50);
LAYOUT_CHECK_OFFSET(BattleRecording_unk_84, BattleRecording, unk_84, 0x84);
LAYOUT_CHECK_OFFSET(BattleRecording_unk_E8, BattleRecording, unk_E8, 0xE8);
void sub_0202FBD4(void *param0);
BattleRecording *sub_0202711C(SaveData *saveData, enum HeapID heapID, int *resultCode, int recNum);
extern BattleRecording *_021D2AF8;
void sub_0202FBD4(void *battleRecording);

u32 sub_0202FBCC(void);
void sub_0202FBD4(void *battleRecording);
void sub_0202FBF0(SaveData *saveData, enum HeapID heapID, int *resultCode);

u32 sub_0202FBCC(void) {
    ((sizeof(BattleRecording) < 0x1000 * 2) ? (void)0 : GF_AssertFail());

    return sizeof(BattleRecording);
}

void sub_0202FBD4(void *ptr) {
    BattleRecording *battleRecording = ptr;
    MI_CpuClear32(battleRecording, sizeof(BattleRecording));
    battleRecording->unk_00 = 0xFFFFFFFF;
}

void sub_0202FBF0(SaveData *saveData, enum HeapID heapID, int *resultCode) {
    if (_021D2AF8 != 0L) {
        Heap_Free(_021D2AF8);
        _021D2AF8 = 0L;
    }

    _021D2AF8 = sub_0202711C(saveData, heapID, resultCode, 0);
    sub_0202FBD4(_021D2AF8);
}

void sub_0202FC24(void) {
    ((_021D2AF8) ? (void)0 : GF_AssertFail());

    Heap_Free(_021D2AF8);
    _021D2AF8 = 0L;
}
