#include "global.h"

#include "comm_server_client.h"
#include "save_link_ruleset.h"

void sub_02035854(void *arg0);

void sub_02035854(void *src) {
    MI_CpuCopy8(src, _021D4134.sCommServerClient->unk_D7C, LinkBattleRuleset_sizeof());
}

LinkBattleRuleset *sub_02035878(void);

LinkBattleRuleset *sub_02035878(void) {
    return _021D4134.sCommServerClient->unk_D7C;
}

s32 sub_020339B4(void *, s32, s32, s32);

s32 sub_020352D8(void);

void sub_0203588C(void);

void sub_0203588C(void) {
    sub_020352D8();
    sub_020339B4(_021D4134.sCommServerClient->unk_D88, 0x5C, _021D4134.sCommServerClient->unk_D80, _021D4134.tgid);
}

s32 sub_02033A44(void);

s32 sub_020358B0(void);

s32 sub_020358B0(void) {
    return sub_02033A44();
}

void sub_020358B8(void *arg0);

void sub_020358B8(void *src) {
    MI_CpuCopy8(src, _021D4134.sCommServerClient, 0x54U);
    sub_0203588C();
}
