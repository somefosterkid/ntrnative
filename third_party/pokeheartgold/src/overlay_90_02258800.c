#include "global.h"

#include "save.h"
#include "overlay_45_unk_struct_ov66_0222e294.h"
#include "player_data.h"
#include "overlay_91_unk_struct_ov66_02230dbc.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"

struct PlayerProfile;
struct UnkStruct_ov66_0222DFF8_t;
int sub_020347A0(void);
PlayerProfile *sub_02034818(int netId);
u16 sub_0203769C(void);
u32 ov90_0225886C(const UnkStruct_ov114_0225C76C *param0, u32 param1);
PlayerProfile *Save_PlayerData_GetProfile(SaveData *saveData);

void ov90_02258800(UnkStruct_ov114_0225C76C *param0, BOOL param1, SaveData *saveData, BOOL param3, UnkStruct_ov66_02230DBC *param4);
u32 ov90_0225886C(const UnkStruct_ov114_0225C76C *param0, u32 param1);
u32 ov90_0225888C(const UnkStruct_ov114_0225C76C *param0, u32 param1);
BOOL ov90_022588A4(const UnkStruct_ov114_0225C76C *param0, u32 param1);
PlayerProfile *ov90_022588CC(const UnkStruct_ov114_0225C76C *param0, u32 param1);

void ov90_02258800(UnkStruct_ov114_0225C76C *param0, BOOL param1, SaveData *saveData, BOOL param3, UnkStruct_ov66_02230DBC *param4) {
    int v0;
    int v1;
    u32 v2;
    PlayerProfile *v3;

    memset(param0, 0, sizeof(UnkStruct_ov114_0225C76C));

    param0->unk_08 = sub_020347A0();
    v2 = sub_0203769C();
    v1 = 0;

    for (v0 = 0; v0 < 4; v0++) {
        v3 = sub_02034818(v0);

        if (v3 != 0L) {
            if (v2 == v0) {
                param0->unk_09 = v1;
            }

            param0->unk_04[v1] = v0;
            v1++;
        }
    }

    ((v1 == param0->unk_08) ? (void)0 : GF_AssertFail());

    if (param1 == 0) {
        param0->unk_0B = 1;
    }

    param0->saveData = saveData;
    param0->unk_0A = param3;
    param0->unk_0C = param4;
}

u32 ov90_0225886C(const UnkStruct_ov114_0225C76C *param0, u32 param1) {
    int v0;

    for (v0 = 0; v0 < param0->unk_08; v0++) {
        if (param0->unk_04[v0] == param1) {
            return v0;
        }
    }

    return 4;
}

u32 ov90_0225888C(const UnkStruct_ov114_0225C76C *param0, u32 param1) {
    ((param1 < param0->unk_08) ? (void)0 : GF_AssertFail());

    return param0->unk_04[param1];
}

BOOL ov90_022588A4(const UnkStruct_ov114_0225C76C *param0, u32 param1) {
    u32 v0;
    BOOL v1;

    if (param1 >= 4) {
        ((param1 < 4) ? (void)0 : GF_AssertFail());
        return 0;
    }

    v0 = param0->unk_0C->unk_04.unk_00[param1];

    if (v0 == 0xffffffff) {
        return 0;
    }

    v1 = param0->unk_0C->unk_08.unk_00[v0];

    return v1;
}

PlayerProfile *ov90_022588CC(const UnkStruct_ov114_0225C76C *param0, u32 param1) {
    u32 v0;

    if (param1 >= 4) {
        ((param1 < 4) ? (void)0 : GF_AssertFail());
        return 0L;
    }

    if (param0->unk_0B == 1) {
        v0 = ov90_0225886C(param0, param1);

        if (v0 == param0->unk_09) {
            return Save_PlayerData_GetProfile(param0->saveData);
        }

        return sub_02034818(param1);
    }

    ((param0->unk_0C != 0L) ? (void)0 : GF_AssertFail());
    return param0->unk_0C->unk_20[param1];
}
