#include "global.h"

#include "error_handling.h"
#include "save.h"
#include "unk_0202C034.h"
#include "ppw_lobby.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

struct PPW_LobbyQuestionnaire;

struct PPW_LobbyQuestionnaireRecord;

struct PPW_LobbyRecruitInfo;

struct PPW_LobbySystemProfile;

struct PPW_LobbyTimeInfo;

struct PPW_LobbyVipRecord;

struct SaveData;

struct WiFiHistory;

struct WiFiList;

typedef enum {
    UnkEnum_ov66_022336B8_00,
    UnkEnum_ov66_022336B8_01,
    UnkEnum_ov66_022336B8_02,
    UnkEnum_ov66_022336B8_03,
    UnkEnum_ov66_022336B8_04,
} UnkEnum_ov66_022336B8;

typedef struct WiFiHistory WiFiHistory;

PPW_LOBBY_CHANNEL_KIND ov45_0222F7B0(UnkEnum_ov66_022336B8 param0);

s32 ov45_0222FB5C(const UnkStruct_ov66_022336C4 *param0, s32 param1, u32 param2);

BOOL ov45_0222FBD8(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_022336B8 param1);

BOOL ov45_0222FBD8(const UnkStruct_ov66_022336C4 *param0, UnkEnum_ov66_022336B8 param1) {
    static const u8 v4[UnkEnum_ov66_022336B8_04] = {
        8,
        8,
        4,
        4,
    };

    ((param1 < UnkEnum_ov66_022336B8_04) ? (void)0 : GF_AssertFail());

    u32 v3 = ov45_0222F7B0(param1);
    int v1 = 0;
    s32 v2;

    for (int i = 0; i < param0->unk_11C[0].unk_02; i++) {
        v2 = ov45_0222FB5C(param0, i, 0);

        if (v2 != 0xffffffff) {
            if (param0->sysProfile[i].subChannelKind == v3) {
                v1++;
            }
        }
    }

    if (v4[param1] > v1) {
        return 1;
    }

    return 0;
}
