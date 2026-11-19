#include "global.h"

#include "constants/heap.h"

#include "options.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "trainer_data.h"
#include "player_data.h"

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
typedef struct BattleRecording {
    u32 unk_00;
    UnkStruct_02030A80 unk_04;
    UnkStruct_0202F41C unk_84;
    UnkStruct_0202F298 unk_E8;
} BattleRecording;
int sub_0202FDA4(SaveData *saveData, BattleRecording *param1, int param2, u16 *param3);
void sub_02030250(void *param0, u32 param1, u32 param2);
extern BattleRecording *_021D2AF8;
void sub_0202FF08(SaveData *saveData, UnkStruct_0202F41C *param1, const UnkStruct_0202F298 *param2, int param3, int param4);
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

int sub_0202FE14(SaveData *saveData, int param1, int param2, int param3, u16 *param4, u16 *param5);
void sub_0202FEB8(int param0, int *param1, int *param2);

int sub_0202FE14(SaveData *saveData, int param1, int param2, int param3, u16 *param4, u16 *param5) {
    UnkStruct_0202F41C *v0;
    UnkStruct_0202F298 *v1;
    int v2;

    switch (*param4) {
    case 0:

        if (_021D2AF8 == 0L) {
            return 3;
        }

        v0 = &_021D2AF8->unk_84;
        v1 = &_021D2AF8->unk_E8;

        sub_0202FF08(saveData, v0, v1, param1, param2);

        v0->unk_48 = 0xe281;
        v0->unk_60.unk_00 = SaveArray_CalcCRC16(saveData, v0, sizeof(UnkStruct_0202F41C) - (sizeof(UnkStruct_0202F298_sub1)) - (sizeof(u64)));
        v1->unk_1BEA = 0xe281;
        v1->unk_1BEC.unk_00 = SaveArray_CalcCRC16(saveData, v1, sizeof(UnkStruct_0202F298) - (sizeof(UnkStruct_0202F298_sub1)));

        sub_02030250(v1, sizeof(UnkStruct_0202F298) - (sizeof(UnkStruct_0202F298_sub1)), v1->unk_1BEC.unk_00 + ((v1->unk_1BEC.unk_00 ^ 0xffff) << 16));

        *param5 = 0;
        (*param4)++;
        break;
    case 1:
        v2 = sub_0202FDA4(saveData, _021D2AF8, param3, param5);
        return v2;
    }

    return 0;
}

void sub_0202FEB8(int param0, int *param1, int *param2) {
    switch (param0) {
    case (UnkEnum_0202F510_17):
    case (UnkEnum_0202F510_20):
    case (UnkEnum_0202F510_23):
    case (UnkEnum_0202F510_26):
    case (UnkEnum_0202F510_29):
    case (UnkEnum_0202F510_32):
    case (UnkEnum_0202F510_14):
        *param1 = 4;
        *param2 = 6 / 2;
        break;
    default:
        *param1 = 4 / 2;
        *param2 = 6;
        break;
    }
}
