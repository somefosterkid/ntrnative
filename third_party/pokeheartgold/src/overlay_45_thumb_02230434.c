#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

BOOL ov45_02230434(const UnkStruct_ov66_022336C4_sub5 *param0, s32 param1);

void ov45_0223048C(UnkStruct_ov66_022336C4_sub6 *param0, const PPW_LobbyQuestionnaire *lobbyQuestionnaire);

BOOL ov45_02230434(const UnkStruct_ov66_022336C4_sub5 *param0, s32 param1) {
    for (int i = 0; i < param0->unk_06; i++) {
        if (param0->lobbyVipRecord[i].profileId == param1) {
            return 1;
        }
    }

    return 0;
}

s32 ov45_0223045C(const UnkStruct_ov66_022336C4_sub5 *param0, s32 param1);

s32 ov45_0223045C(const UnkStruct_ov66_022336C4_sub5 *param0, s32 param1) {
    for (int i = 0; i < param0->unk_06; i++) {
        if (param0->lobbyVipRecord[i].profileId == param1) {
            return param0->lobbyVipRecord[i].key;
        }
    }

    return 0;
}

void ov45_0223048C(UnkStruct_ov66_022336C4_sub6 *param0, const PPW_LobbyQuestionnaire *lobbyQuestionnaire) {
    memcpy(&param0->lobbyQuestionnaire, lobbyQuestionnaire, sizeof(PPW_LobbyQuestionnaire));
}
