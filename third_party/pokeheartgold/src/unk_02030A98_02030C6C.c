#include "global.h"
#include "battle_castle_save.h"

typedef struct BattleHallStreakFlags {
    u8 streakActiveFlags;
    u8 unused[3];
} BattleHallStreakFlags;

void sub_02030C6C(BattleHallStreakFlags *flags, u8 field, u8 challengeType, u8 unused, u8 *value);
u32 sub_02030CA0(BattleHallStreakFlags *flags, u8 field, u8 challengeType, u8 unused, void *unused2);
void sub_02030CBC(BattleCastleSave *castleSave);

void sub_02030C6C(BattleHallStreakFlags *flags, u8 field, u8 challengeType, u8 unused, u8 *value) {
    u8 *value_dupe = (u8 *)value;
    switch (field) {
    case 5:
        if (*value_dupe >= 1) {
            flags->streakActiveFlags |= (1 << challengeType);
        } else {
            flags->streakActiveFlags &= (0xff ^ (1 << challengeType));
        }
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }
}

u32 sub_02030CA0(BattleHallStreakFlags *flags, u8 field, u8 challengeType, u8 unused, void *unused2) {
    switch (field) {
    case 5:
        return (flags->streakActiveFlags >> challengeType) & 0x1;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 0;
}

void sub_02030CBC(BattleCastleSave *castleSave) {
    MI_CpuClear8(castleSave, sizeof(BattleCastleSave));
}
