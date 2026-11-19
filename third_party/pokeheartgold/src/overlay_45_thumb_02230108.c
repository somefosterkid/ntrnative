#include "global.h"

#include "error_handling.h"
#include "save.h"
#include "unk_0202C034.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"


void ov45_02230108(void);

struct PPW_LobbyVipRecord;
struct WiFiHistory;
struct WiFiList;
typedef enum {
    UnkEnum_ov66_02232F38_00,
    UnkEnum_ov66_02232F38_01,
    UnkEnum_ov66_02232F38_02,
    UnkEnum_ov66_02232F38_03,
} UnkEnum_ov66_02232F38;
typedef struct WiFiHistory WiFiHistory;
BOOL ov45_0223021C(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1);
u16 ov45_0223023C(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1);
const PPW_LobbyRecruitInfo *ov45_022302B0(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1);
void ov45_02230384(const PPW_LobbyRecruitInfo *lobbyRecruitInfo, s64 *param1);
void ov45_02230144(UnkStruct_ov66_022336C4 *param0);
void ov45_02230164(UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 gameKind, s32 param2, const PPW_LobbyRecruitInfo *lobbyRecruitInfo);
BOOL ov45_022301E0(UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1, s32 param2);
BOOL ov45_0223021C(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1);
u16 ov45_0223023C(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1);
const PPW_LobbyRecruitInfo *ov45_022302B0(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1);

void ov45_02230108(void) {
    if (_022577C0->unk_6A8.unk_2D8 != 1) {
        GF_AssertFail();
    }
    _022577C0->unk_6A8.unk_2D8 = 2;
}

void ov45_02230130(void);

void ov45_02230130(void) {
    _022577C0->unk_131 = 1;
}

void ov45_02230144(UnkStruct_ov66_022336C4 *param0) {
    memset(param0->unk_5B8, 0, sizeof(UnkStruct_ov66_022336C4_sub4) * UnkEnum_ov66_02232F38_03);
    param0->gameKind = UnkEnum_ov66_02232F38_03;
}

void ov45_02230164(UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 gameKind, s32 param2, const PPW_LobbyRecruitInfo *lobbyRecruitInfo) {
    ((gameKind < UnkEnum_ov66_02232F38_03) ? (void)0 : GF_AssertFail());

    if (ov45_0223021C(param0, gameKind) == 1) {
        if (param0->unk_5B8[gameKind].unk_48 != param2) {
            return;
        }
    }

    param0->unk_5B8[gameKind].lobbyRecruitInfo = *lobbyRecruitInfo;

    if (param0->unk_5B8[gameKind].unk_44 == 0) {
        param0->unk_5B8[gameKind].unk_44 = 1;
        param0->unk_5B8[gameKind].unk_48 = param2;
        param0->unk_5B8[gameKind].unk_46 = ov45_0223023C(param0, gameKind);
    }
}

BOOL ov45_022301E0(UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1, s32 param2) {
    ((param1 < UnkEnum_ov66_02232F38_03) ? (void)0 : GF_AssertFail());

    if (ov45_0223021C(param0, param1) == 1) {
        if (param0->unk_5B8[param1].unk_48 == param2) {
            param0->unk_5B8[param1].unk_44 = 0;
            return 1;
        }
    }

    return 0;
}

BOOL ov45_0223021C(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1) {
    ((param1 < UnkEnum_ov66_02232F38_03) ? (void)0 : GF_AssertFail());
    return param0->unk_5B8[param1].unk_44;
}

u16 ov45_0223023C(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1) {
    if (ov45_0223021C(param0, param1) == 0) {
        return 0;
    }

    const PPW_LobbyRecruitInfo *recruitInfo = ov45_022302B0(param0, param1);
    s64 v0;
    ov45_02230384(recruitInfo, &v0);
    s64 v1 = param0->timeInfo.currentTime - v0;

    if (v1 > 30) {
        v1 = 30;
    } else if (v1 < 0) {
        v1 = 0;
    }

    return (60 - v1) * 30;
}

const PPW_LobbyRecruitInfo *ov45_022302B0(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_02232F38 param1) {
    ((param1 < UnkEnum_ov66_02232F38_03) ? (void)0 : GF_AssertFail());
    ((param0->unk_5B8[param1].unk_44 == 1) ? (void)0 : GF_AssertFail());
    return &param0->unk_5B8[param1].lobbyRecruitInfo;
}
