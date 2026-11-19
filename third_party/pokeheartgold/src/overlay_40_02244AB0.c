#include "global.h"

#include "constants/heap.h"

#include "bag_cursor.h"
#include "bag_types_def.h"
#include "options.h"
#include "overlay_40.h"
#include "pm_string.h"
#include "sav_chatot.h"
#include "save.h"
#include "seal_case.h"
#include "system.h"
#include "trainer_data.h"
#include "player_data.h"

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
typedef struct UnkStruct_0202F298_t {
    UnkStruct_0202FAA8 unk_00;
    UnkStruct_0202FAA8_sub1 unk_150;
    UnkStruct_0202FD30 unk_1150[4];
    PlayerProfile unk_1B68[4];
    Options unk_1BE8;
    u16 unk_1BEA;
    UnkStruct_0202F298_sub1 unk_1BEC;
} UnkStruct_0202F298;
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
typedef struct UnkStruct_0202F41C_t {
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
} UnkStruct_0202F41C;
typedef struct UnkStruct_02030A80_t {
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
} UnkStruct_02030A80;
typedef struct BattleRecording {
    u32 unk_00;
    UnkStruct_02030A80 unk_04;
    UnkStruct_0202F41C unk_84;
    UnkStruct_0202F298 unk_E8;
} BattleRecording;
int sub_0202FDA4(SaveData *saveData, BattleRecording *param1, int param2, u16 *param3);
void sub_02030250(void *param0, u32 param1, u32 param2);
extern BattleRecording *_021D2AF8;
int sub_02027134(SaveData *saveData, BattleRecording *rec, int recNum);

int ov40_02244BBC(SaveData *saveData, int param1);

int ov40_02244AB0(SaveData *saveData, int param1, u8 param2, u16 *param3, u16 *param4) {
    int v0;

    switch (*param3) {
    case 0:
        ((_021D2AF8) ? (void)0 : GF_AssertFail());

        if (param2 == 1) {
            _021D2AF8->unk_84.unk_27 = param2;

            _021D2AF8->unk_84.unk_48 = 0xe281;
            _021D2AF8->unk_84.unk_60.unk_00 = SaveArray_CalcCRC16(saveData, &_021D2AF8->unk_84, sizeof(UnkStruct_0202F41C) - (sizeof(UnkStruct_0202F298_sub1)) - (sizeof(u64)));
        }

        sub_02030250(&_021D2AF8->unk_E8, sizeof(UnkStruct_0202F298) - (sizeof(UnkStruct_0202F298_sub1)), _021D2AF8->unk_E8.unk_1BEC.unk_00 + ((_021D2AF8->unk_E8.unk_1BEC.unk_00 ^ 0xffff) << 16));
        (*param3)++;
        break;
    case 1:
        v0 = sub_0202FDA4(saveData, _021D2AF8, param1, param4);
        return v0;
    }

    return 0;
}

void ov40_02244B3C(SaveData *saveData) {
    ((_021D2AF8) ? (void)0 : GF_AssertFail());
    sub_02030250(&_021D2AF8->unk_E8, sizeof(UnkStruct_0202F298) - (sizeof(UnkStruct_0202F298_sub1)), _021D2AF8->unk_E8.unk_1BEC.unk_00 + ((_021D2AF8->unk_E8.unk_1BEC.unk_00 ^ 0xffff) << 16));
}

int ov40_02244B70(SaveData *saveData, u64 param1, u16 *param2, u16 *param3) {
    int v0;

    switch (*param2) {
    case 0:
        ((_021D2AF8) ? (void)0 : GF_AssertFail());

        _021D2AF8->unk_84.unk_58 = param1;
        (*param2)++;
        break;
    case 1:
        v0 = sub_0202FDA4(saveData, _021D2AF8, 0, param3);
        return v0;
    }

    return 0;
}

int ov40_02244BBC(SaveData *saveData, int param1) {
    int v0;

    ((_021D2AF8 != 0L) ? (void)0 : GF_AssertFail());

    _021D2AF8->unk_84.unk_27 = 1;
    _021D2AF8->unk_84.unk_48 = 0xe281;
    _021D2AF8->unk_84.unk_60.unk_00 = SaveArray_CalcCRC16(saveData, &_021D2AF8->unk_84, sizeof(UnkStruct_0202F41C) - (sizeof(UnkStruct_0202F298_sub1)) - (sizeof(u64)));

    sub_02030250(&_021D2AF8->unk_E8, sizeof(UnkStruct_0202F298) - (sizeof(UnkStruct_0202F298_sub1)), _021D2AF8->unk_E8.unk_1BEC.unk_00 + ((_021D2AF8->unk_E8.unk_1BEC.unk_00 ^ 0xffff) << 16));
    sub_0201A728(1 << 3);

    v0 = sub_02027134(saveData, _021D2AF8, param1);

    if (v0 == 2) {
        v0 = SaveGameNormal(saveData);
    }

    sub_0201A738(1 << 3);
    return v0;
}
