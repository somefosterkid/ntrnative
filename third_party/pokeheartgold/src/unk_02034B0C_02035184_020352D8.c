#include "global.h"

#include "comm_server_client.h"
#include "error_handling.h"
#include "player_data.h"
#include "save_link_ruleset.h"
#include "unk_02037C94.h"

s32 sub_02033240(void *, u16);

u8 sub_02033AB8(void);

PlayerProfile *sub_02035784(void);

void sub_020352D8(void);

void sub_020352D8(void) {
    PlayerProfile *profile;
    PlayerProfile *playerProfile;
    u8 v1;
    CommGameInfo *v0;
    CommGameInfo *v2;

    v1 = sub_0203993C();
    profile = sub_02035784();
    playerProfile = profile;
    if (v1 != 0xF) {
        v2 = _021D4134.sCommServerClient->unk_D88;
        if ((s32)LinkBattleRuleset_sizeof() > 0x20) {
            GF_AssertFail();
        }
        if (PlayerProfile_sizeof() != 0x20) {
            GF_AssertFail();
        }
        MI_CpuCopy8(playerProfile, &v2->unk_10, PlayerProfile_sizeof());
        MI_CpuCopy8(_021D4134.sCommServerClient->unk_D7C, v2->unk_30, LinkBattleRuleset_sizeof());
        v2->unk_00 = PlayerProfile_GetTrainerID(playerProfile);
        v2->unk_04 = sub_0203993C();
        v2->unk_05 = sub_02039954();
        MI_CpuCopy8(&_021D4134.sCommServerClient->unk_D68, &v2->unk_08, 8U);
        v2->unk_54 = sub_02033AB8();
    } else {
        v0 = _021D4134.sCommServerClient->unk_D88;
        v0->unk_00 = PlayerProfile_GetTrainerID(profile);
        v0->unk_04 = sub_0203993C();
        v0->unk_05 = sub_02039954();
        MI_CpuCopy8(_021D4134.sCommServerClient->unk_00, &v0->unk_08, 0x54U);
    }
    DC_FlushRange(_021D4134.sCommServerClient->unk_D88, 0x5CU);
    sub_02033240(_021D4134.sCommServerClient->unk_D88, 0x5C);
}

s32 sub_020339B4(void *, s32, s32, s32);

int sub_02035610(void);

void sub_020353B8(void);

void sub_020353B8(void) {
    CommGameInfo *v0;

    v0 = _021D4134.sCommServerClient->unk_D88;
    if (v0->unk_06 != sub_02035610()) {
        v0->unk_06 = sub_02035610();
        DC_FlushRange(_021D4134.sCommServerClient->unk_D88, 0x5CU);
        sub_02033240(_021D4134.sCommServerClient->unk_D88, 0x5C);
        sub_020339B4(_021D4134.sCommServerClient->unk_D88, 0x5C, _021D4134.sCommServerClient->unk_D80, _021D4134.tgid);
    }
}
