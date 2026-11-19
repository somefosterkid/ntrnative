#include "global.h"
#include "battle_castle_save.h"

typedef struct BattleFactoryStreakFlags {
    u8 streakActiveFlags;
    u8 unused[3];
} BattleFactoryStreakFlags;

void sub_02030A98(BattleCastlePersistentSave *save);
void sub_02030AA4(BattleFactoryStreakFlags *flags, u8 field, u8 challengeType, u8 *value);
u32 sub_02030AD4(BattleFactoryStreakFlags *flags, u8 field, u8 challengeType, void *unused);

void sub_02030A98(BattleCastlePersistentSave *save) {
    MI_CpuClear8(save, sizeof(BattleCastlePersistentSave));
}

void sub_02030AA4(BattleFactoryStreakFlags *flags, u8 field, u8 challengeType, u8 *value) {
    u8 *value_dupe = (u8 *)value;
    if (field == 10) {
        if (*value_dupe >= 1) {
            flags->streakActiveFlags |= (1 << challengeType);
        } else {
            flags->streakActiveFlags &= (0xff ^ (1 << challengeType));
        }
    }
}

u32 sub_02030AD4(BattleFactoryStreakFlags *flags, u8 field, u8 challengeType, void *unused) {
    switch (field) {
    case 10:
        return (flags->streakActiveFlags >> challengeType) & 0x1;
    }

    return 0;
}
