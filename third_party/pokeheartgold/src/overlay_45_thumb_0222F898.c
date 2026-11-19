#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"
#include "ppw_lobby.h"

struct PPW_LobbyQuestionnaire;
struct PPW_LobbyQuestionnaireRecord;
struct PPW_LobbyRecruitInfo;
struct PPW_LobbySystemProfile;
struct PPW_LobbyTimeInfo;
struct PPW_LobbyVipRecord;
struct SaveData;
struct WiFiHistory;
struct WiFiList;
typedef struct SaveData SaveData;
typedef struct WiFiList WiFiList;
typedef struct WiFiHistory WiFiHistory;
s32 ov45_0222FB24(const UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2);
void *Heap_Alloc(u32 heapID, u32 size);
void ov45_0222FA40(UnkStruct_ov66_022336C4 *param0, u32 param1);
void Heap_Free(void *ptr);

void ov45_0222F898(UnkStruct_ov66_022336C4 *param0, s32 param1, const void *param2, u32 param3);
const void *ov45_0222F8D8(const UnkStruct_ov66_022336C4 *param0, s32 param1);
void ov45_0222F8FC(UnkStruct_ov66_022336C4 *param0, s32 param1);
void ov45_0222F928(UnkStruct_ov66_022336C4 *param0, s32 param1, const PPW_LobbySystemProfile *lobbySystemProfile);
const PPW_LobbySystemProfile *ov45_0222F954(const UnkStruct_ov66_022336C4 *param0, s32 param1);
void ov45_0222F978(UnkStruct_ov66_022336C4 *param0, s32 param1);
void ov45_0222F9B8(UnkStruct_ov66_022336C4 *param0, u32 param1, u32 param2, u32 heapID);
void ov45_0222FA10(UnkStruct_ov66_022336C4 *param0);
void ov45_0222FA40(UnkStruct_ov66_022336C4 *param0, u32 param1);
void ov45_0222FA74(UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2);
void ov45_0222FAF8(UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2);
s32 ov45_0222FB24(const UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2);
s32 ov45_0222FB5C(const UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2);

void ov45_0222F898(UnkStruct_ov66_022336C4 *param0, s32 param1, const void *param2, u32 param3) {
    s32 v0 = ov45_0222FB24(param0, param1, 0);
    ((v0 != 0xffffffff) ? (void)0 : GF_AssertFail());

    u8 *v1 = param0->unk_24;
    v1 = &v1[param0->unk_28 * v0];
    u32 v2;

    if (param0->unk_28 > param3) {
        v2 = param3;
        memset(v1, 0, param0->unk_28);
    } else {
        v2 = param0->unk_28;
    }

    memcpy(v1, param2, v2);
}

const void *ov45_0222F8D8(const UnkStruct_ov66_022336C4 *param0, s32 param1) {
    s32 v0 = ov45_0222FB24(param0, param1, 0);
    ((v0 != 0xffffffff) ? (void)0 : GF_AssertFail());

    const u8 *v1 = param0->unk_24;
    v1 = &v1[param0->unk_28 * v0];

    return v1;
}

void ov45_0222F8FC(UnkStruct_ov66_022336C4 *param0, s32 param1) {
    s32 v0 = ov45_0222FB24(param0, param1, 0);
    ((v0 != 0xffffffff) ? (void)0 : GF_AssertFail());

    u8 *v1 = param0->unk_24;
    v1 = &v1[param0->unk_28 * v0];

    memset(v1, 0, param0->unk_28);
}

void ov45_0222F928(UnkStruct_ov66_022336C4 *param0, s32 param1, const PPW_LobbySystemProfile *lobbySystemProfile) {
    s32 v0 = ov45_0222FB24(param0, param1, 0);
    ((v0 != 0xffffffff) ? (void)0 : GF_AssertFail());

    param0->sysProfile[v0] = *lobbySystemProfile;
}

const PPW_LobbySystemProfile *ov45_0222F954(const UnkStruct_ov66_022336C4 *param0, s32 param1) {
    s32 v0 = ov45_0222FB24(param0, param1, 0);
    ((v0 != 0xffffffff) ? (void)0 : GF_AssertFail());

    return &param0->sysProfile[v0];
}

void ov45_0222F978(UnkStruct_ov66_022336C4 *param0, s32 param1) {
    s32 v0 = ov45_0222FB24(param0, param1, 0);
    ((v0 != 0xffffffff) ? (void)0 : GF_AssertFail());

    memset(&param0->sysProfile[v0], 0, sizeof(PPW_LobbySystemProfile));
}

void ov45_0222F9B8(UnkStruct_ov66_022336C4 *param0, u32 param1, u32 param2, u32 heapID) {
    int v1;

    for (int i = 0; i < 2; i++) {
        ((param0->unk_11C[i].unk_04 == 0L) ? (void)0 : GF_AssertFail());

        if (i == 0) {
            v1 = param1;
        } else {
            v1 = param2;
        }

        param0->unk_11C[i].unk_00 = 0;
        param0->unk_11C[i].unk_04 = Heap_Alloc(heapID, sizeof(s32) * v1);
        param0->unk_11C[i].unk_02 = v1;

        ov45_0222FA40(param0, i);
    }
}

void ov45_0222FA10(UnkStruct_ov66_022336C4 *param0) {
    for (int i = 0; i < 2; i++) {
        ((param0->unk_11C[i].unk_04 != 0L) ? (void)0 : GF_AssertFail());
        Heap_Free(param0->unk_11C[i].unk_04);
        param0->unk_11C[i].unk_04 = 0L;
    }
}

void ov45_0222FA40(UnkStruct_ov66_022336C4 *param0, u32 param1) {
    param0->unk_11C[param1].unk_00 = 0;

    for (int i = 0; i < param0->unk_11C[param1].unk_02; i++) {
        param0->unk_11C[param1].unk_04[i] = -1;
    }
}

void ov45_0222FA74(UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2) {
    if (ov45_0222FB24(param0, param1, param2) != 0xffffffff) {
        return;
    }

    ((param0->unk_11C[param2].unk_00 < param0->unk_11C[param2].unk_02) ? (void)0 : GF_AssertFail());

    if (param0->unk_11C[param2].unk_00 >= param0->unk_11C[param2].unk_02) {
        return;
    }

    u32 v0;

    for (int i = 0; i < param0->unk_11C[param2].unk_02; i++) {
        if (param0->unk_11C[param2].unk_04[i] == -1) {
            v0 = i;
            break;
        }
    }

    param0->unk_11C[param2].unk_04[v0] = param1;
    param0->unk_11C[param2].unk_00++;
}

void ov45_0222FAF8(UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2) {
    u32 v0 = ov45_0222FB24(param0, param1, param2);

    if (v0 == 0xffffffff) {
        return;
    }

    param0->unk_11C[param2].unk_04[v0] = -1;
    param0->unk_11C[param2].unk_00--;
}

s32 ov45_0222FB24(const UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2) {
    for (int i = 0; i < param0->unk_11C[param2].unk_02; i++) {
        if (param0->unk_11C[param2].unk_04[i] == param1) {
            return i;
        }
    }

    return 0xffffffff;
}

s32 ov45_0222FB5C(const UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2) {
    ((param1 < param0->unk_11C[param2].unk_02) ? (void)0 : GF_AssertFail());
    return param0->unk_11C[param2].unk_04[param1];
}
