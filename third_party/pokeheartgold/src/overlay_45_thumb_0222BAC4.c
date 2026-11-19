#include "global.h"
#include "overlay_88_unk_struct_ov66_02231428.h"

struct SaveData;
typedef struct {
    u32 questionNumber;
    u32 selectedAnswer;
} UnkStruct_ov66_0222E908;
typedef u16 charcode_t;
typedef struct SaveData SaveData;
u16 SaveArray_CalcCRC16(const SaveData *saveData, const void *startAddress, u32 size);
typedef struct WiFiPlazaProfile {
    s32 unk_00;
    u32 trainerID;
    charcode_t trainerName[7 + 1];
    UnkStruct_ov66_02231428 unk_18;
    UnkStruct_ov66_02231428 unk_1C;
    u16 partySpecies[6];
    u8 partyForms[6];
    u8 partyIsEgg[6];
    u8 trainerGender;
    u8 language;
    u16 trainerAppearanceIdx;
    u16 country;
    u8 region;
    u8 isNationalDexObtained;
    u8 isMainStoryCleared;
    u8 unk_41;
    u8 gameVersion;
    u8 unk_43;
    s64 startTimestamp;
    u8 unk_4C[12];
    s32 unk_58[12];
    u16 unk_88[2];
    UnkStruct_ov66_0222E908 unk_8C;
} WiFiPlazaProfile;
typedef struct {
    u16 unk_00[8];
    u16 unk_10[8];
    WiFiPlazaProfile profile;
    u32 unk_B4;
} UnkStruct_ov66_0222F6C4;

void ov45_0222BAC4(UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData);
BOOL ov45_0222BADC(const UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData);
void ov45_0222BB00(UnkStruct_ov66_0222F6C4 *param0, const WiFiPlazaProfile *param1, u32 param2);

void ov45_0222BB58(void *arg0);

void ov45_0222BAC4(UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData) {
    param0->unk_B4 = SaveArray_CalcCRC16(saveData, &param0->profile, sizeof(WiFiPlazaProfile));
}

BOOL ov45_0222BADC(const UnkStruct_ov66_0222F6C4 *param0, const SaveData *saveData) {
    u32 v0 = SaveArray_CalcCRC16(saveData, &param0->profile, sizeof(WiFiPlazaProfile));

    if (v0 != param0->unk_B4) {
        ((0) ? (void)0 : GF_AssertFail());
        return 0;
    }

    return 1;
}

void ov45_0222BB00(UnkStruct_ov66_0222F6C4 *param0, const WiFiPlazaProfile *param1, u32 param2) {
    param0->profile = *param1;

    if (sizeof(WiFiPlazaProfile) >= param2) {
        MI_CpuCopy8(param1, &param0->profile, param2);
    } else {
        MI_CpuCopy8(param1, &param0->profile, sizeof(WiFiPlazaProfile));
    }

    MI_CpuCopy8(param1->trainerName, param0->unk_10, sizeof(u16) * (7 + 1));
    MI_CpuCopy8(param0->unk_00, param0->profile.trainerName, sizeof(u16) * (7 + 1));
}

void ov45_0222BB58(void *arg0) {
    *(s32 *)((u8 *)arg0 + 8) = 0;
}
