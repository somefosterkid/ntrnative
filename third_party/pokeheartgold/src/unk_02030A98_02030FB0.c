#include "global.h"
#include "overlay_80_frontier_opponent.h"
#include "mail_message_types.h"
#include "unk_0202D230.h"
#include "overlay_73_wifi_battle_tower_app_state.h"
#include "battle_castle_save.h"
#include "save_frontier.h"


void sub_02030FB0(BattleArcadeStreakFlags *flags, u8 field, u8 challengeType, u8 unused, const void *value);
u32 sub_02030FE4(BattleArcadeStreakFlags *flags, u8 field, u8 challengeType, u8 unused, void *unused2);
u32 Save_Frontier_sizeof(void);

s32 sub_02030A98(void *);
s32 sub_02030C50(void *);
s32 sub_02030DFC(void *);
s32 sub_02030F94(void *);
extern s32 _021D2AFC;
void Save_Frontier_Init(void *arg0);

void sub_02030FB0(BattleArcadeStreakFlags *flags, u8 field, u8 challengeType, u8 unused, const void *value) {
    u8 *value_dupe = (u8 *)value;
    switch (field) {
    case 8:
        if (*value_dupe >= 1) {
            flags->streakActiveFlags |= 1 << challengeType;
        } else {
            flags->streakActiveFlags &= 0xff ^ (1 << challengeType);
        }
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }
}

u32 sub_02030FE4(BattleArcadeStreakFlags *flags, u8 field, u8 challengeType, u8 unused, void *unused2) {
    switch (field) {
    case 8:
        return (flags->streakActiveFlags >> challengeType) & 0x1;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 0;
}

u32 Save_Frontier_sizeof(void) {
    return sizeof(FrontierSave);
}

void Save_Frontier_Init(void *arg0) {
    MI_CpuFill8(arg0, 0U, 0x1628U);
    sub_0202D240(arg0 + 0x954);
    sub_0202D254(arg0 + 0xABC);
    sub_0202D274(arg0 + 0xADC);
    sub_02030A98(arg0 + 0x1618);
    sub_02030C50(arg0 + 0x161C);
    sub_02030DFC(arg0 + 0x1620);
    sub_02030F94(arg0 + 0x1624);
    MI_CpuCopy8(arg0, &_021D2AFC, 0x1628U);
}
