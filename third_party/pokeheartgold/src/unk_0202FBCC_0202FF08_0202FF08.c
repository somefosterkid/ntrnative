#include "global.h"

#include "link_ruleset_data.h"
#include "trainer_data.h"
#include "options.h"
#include "player_data.h"

struct BattleRegulation;

struct MailMessage;

struct Options;

struct SaveData;

struct Trainer;

struct TrainerData;

struct PlayerProfile;

struct UnkStruct_0202F298_t;

struct UnkStruct_0202F41C_t;

struct UnkStruct_0202FAA8_sub1_t;

struct UnkStruct_02030A80_t;

struct UnkStruct_02078B40_t;

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
    u64 unk_58;
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

void sub_0202FEB8(int param0, int *param1, int *param2);

typedef enum {
    UnkEnum_0202F510_00,
    UnkEnum_0202F510_01,
    UnkEnum_0202F510_02,
    UnkEnum_0202F510_03,
    UnkEnum_0202F510_04,
    UnkEnum_0202F510_05,
    UnkEnum_0202F510_06,
    UnkEnum_0202F510_07,
    UnkEnum_0202F510_08,
    UnkEnum_0202F510_09,
    UnkEnum_0202F510_10,
    UnkEnum_0202F510_11,
    UnkEnum_0202F510_12,
    UnkEnum_0202F510_13,
    UnkEnum_0202F510_14,
    UnkEnum_0202F510_15,
    UnkEnum_0202F510_16,
    UnkEnum_0202F510_17,
    UnkEnum_0202F510_18,
    UnkEnum_0202F510_19,
    UnkEnum_0202F510_20,
    UnkEnum_0202F510_21,
    UnkEnum_0202F510_22,
    UnkEnum_0202F510_23,
    UnkEnum_0202F510_24,
    UnkEnum_0202F510_25,
    UnkEnum_0202F510_26,
    UnkEnum_0202F510_27,
    UnkEnum_0202F510_28,
    UnkEnum_0202F510_29,
    UnkEnum_0202F510_30,
    UnkEnum_0202F510_31,
    UnkEnum_0202F510_32,
    UnkEnum_0202F510_33 = 0xfa,
    UnkEnum_0202F510_34 = 0xfb,
    UnkEnum_0202F510_35 = 0xfc,
    UnkEnum_0202F510_36 = 0xfd,
    UnkEnum_0202F510_37 = 0xfe,
    UnkEnum_0202F510_38 = 0xff,
} UnkEnum_0202F510;

UnkStruct_0202F41C *sub_0203077C(enum HeapID heapID);

UnkStruct_02030A80 *sub_020307AC(enum HeapID heapID);

UnkStruct_0202F41C *sub_02030920(enum HeapID heapID);

void sub_0202FF08(SaveData *saveData, UnkStruct_0202F41C *param1, const UnkStruct_0202F298 *param2, int param3, int param4);

void sub_0202FF08(SaveData *saveData, UnkStruct_0202F41C *param1, const UnkStruct_0202F298 *param2, int param3, int param4) {
    int v0, v1, v2, v3, v4, v5, v6;
    const UnkStruct_02078B40 *v7;
    const u8 v8[2][4] = {
        { 0, 2, 3, 1 },
        { 3, 1, 0, 2 },
    };
    const u8 v9[4] = { 0, 2, 1, 3 };

    MI_CpuClear8(param1, sizeof(UnkStruct_0202F41C));
    sub_0202FEB8(param3, &v2, &v3);

    v4 = 0;

    if (param2->unk_00.unk_00 & 0x4) {
        if (param2->unk_00.unk_00 & 0x80) {
            v6 = param2->unk_00.unk_144 * 2;
        } else {
            v6 = param2->unk_00.unk_144;
        }
    } else {
        v6 = 0;
    }

    for (v0 = 0; v0 < v2; v0++) {
        if ((param2->unk_00.unk_00 & 0x8) && ((param2->unk_00.unk_00 & 0x80) == 0)) {
            for (v5 = 0; v5 < v2; v5++) {
                if (param2->unk_00.unk_134[v5] == v8[param2->unk_00.unk_134[v6] & 1][v0]) {
                    break;
                }
            }
        } else if ((param2->unk_00.unk_00 & 0x8) && (param2->unk_00.unk_00 & 0x80)) {
            v5 = v9[v0];
        } else {
            v5 = v0;

            if (v6 & 1) {
                v5 ^= 1;
            }
        }

        for (v1 = 0; v1 < v3; v1++) {
            v7 = &(param2->unk_1150[v5].unk_04[v1]);

            if ((v7->isEgg == 0) && (v7->checksumFailed == 0)) {
                param1->unk_00[v4] = v7->species;
                param1->unk_18[v4] = v7->form;
            }

            v4++;
        }
    }

    switch (param3) {
    case (UnkEnum_0202F510_01):
    case (UnkEnum_0202F510_08):
        param1->battleRegulation = *(BattleRegulation *)sub_020291E8(saveData, 0);
        break;
    case (UnkEnum_0202F510_02):
    case (UnkEnum_0202F510_09):
        param1->battleRegulation = *(BattleRegulation *)sub_020291E8(saveData, 1);
        break;
    case (UnkEnum_0202F510_03):
    case (UnkEnum_0202F510_10):
        param1->battleRegulation = *(BattleRegulation *)sub_020291E8(saveData, 2);
        break;
    case (UnkEnum_0202F510_04):
    case (UnkEnum_0202F510_11):
        param1->battleRegulation = *(BattleRegulation *)sub_020291E8(saveData, 3);
        break;
    case (UnkEnum_0202F510_05):
    case (UnkEnum_0202F510_12):
        param1->battleRegulation = *(BattleRegulation *)sub_020291E8(saveData, 4);
        break;
    case (UnkEnum_0202F510_06):
    case (UnkEnum_0202F510_13):
        param1->battleRegulation = *(BattleRegulation *)sub_020291E8(saveData, 5);
        break;
    case (UnkEnum_0202F510_00):
    case (UnkEnum_0202F510_07):
    default:
        param1->battleRegulation = *(BattleRegulation *)sub_0202925C();
        break;
    }

    param1->unk_24 = param4;
    param1->unk_26 = param3;
}
