#include "global.h"

#include "pm_string.h"

typedef struct UnkStruct_ov66_02231FB0_t {
    u8 unk_00;
    u32 unk_04[3];
    String *unk_10[4];
    u16 unk_20[4];
    u16 unk_28;
    s16 unk_2A;
    struct UnkStruct_ov66_02231FB0_t *unk_2C;
    struct UnkStruct_ov66_02231FB0_t *unk_30;
} UnkStruct_ov66_02231FB0;

typedef struct {
    UnkStruct_ov66_02231FB0 unk_00[8];
    UnkStruct_ov66_02231FB0 unk_188;
} UnkStruct_ov66_02231E94;

void ov45_0222E094(UnkStruct_ov66_02231FB0 *param0, UnkStruct_ov66_02231FB0 *param1);

void ov45_0222E0A4(UnkStruct_ov66_02231E94 *param0, UnkStruct_ov66_02231FB0 *param1);

typedef u16 charcode_t;
typedef struct TrainerInfo {
    charcode_t name[7 + 1];
    u32 id;
    u32 money;
    u8 gender;
    u8 language;
    u8 badgeMask;
    u8 appearance;
    u8 gameCode;
    u8 isMainStoryCleared : 1;
    u8 hasNationalDex : 1;
    u8 : 6;
} TrainerInfo;
void PlayerName_FlatToString(const TrainerInfo *info, String *name);
void ov45_0222E0E0(UnkStruct_ov66_02231FB0 *param0, u32 param1, u32 param2, u32 param3, const TrainerInfo *param4, const TrainerInfo *param5, const TrainerInfo *param6, const TrainerInfo *param7, u16 param8, u16 param9, u16 param10, u16 param11, u32 param12, u32 param13, u32 param14);

void ov45_0222E0A4(UnkStruct_ov66_02231E94 *param0, UnkStruct_ov66_02231FB0 *param1) {
    {
        UnkStruct_ov66_02231FB0 *v0;

        v0 = param0->unk_188.unk_30;

        while (v0 != &param0->unk_188) {
            if (v0->unk_28 < param1->unk_28) {
                break;
            }

            v0 = v0->unk_30;
        }

        ov45_0222E094(v0, param1);
    }
}

void ov45_0222E0CC(UnkStruct_ov66_02231E94 *param0, UnkStruct_ov66_02231FB0 *param1);

void ov45_0222E0CC(UnkStruct_ov66_02231E94 *param0, UnkStruct_ov66_02231FB0 *param1) {
    param1->unk_30->unk_2C = param1->unk_2C;
    param1->unk_2C->unk_30 = param1->unk_30;
    param1->unk_30 = NULL;
    param1->unk_2C = NULL;
}

void ov45_0222E0E0(UnkStruct_ov66_02231FB0 *param0, u32 param1, u32 param2, u32 param3, const TrainerInfo *param4, const TrainerInfo *param5, const TrainerInfo *param6, const TrainerInfo *param7, u16 param8, u16 param9, u16 param10, u16 param11, u32 param12, u32 param13, u32 param14) {
    GF_ASSERT(param14 < 9);
    param0->unk_00 = param14;
    param0->unk_2A = param12;
    param0->unk_28 = param13;
    param0->unk_04[0] = param1;
    param0->unk_04[1] = param2;
    param0->unk_04[2] = param3;
    param0->unk_20[0] = param8;
    param0->unk_20[1] = param9;
    param0->unk_20[2] = param10;
    param0->unk_20[3] = param11;

    if (param4) {
        PlayerName_FlatToString(param4, param0->unk_10[0]);
    }

    if (param5) {
        PlayerName_FlatToString(param5, param0->unk_10[1]);
    }

    if (param6) {
        PlayerName_FlatToString(param6, param0->unk_10[2]);
    }

    if (param7) {
        PlayerName_FlatToString(param7, param0->unk_10[3]);
    }
}
