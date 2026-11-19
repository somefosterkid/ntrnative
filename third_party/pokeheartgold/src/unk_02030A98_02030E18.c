#include "global.h"
#include "battle_castle_save.h"

typedef struct BattleArcadeSave {
    u8 challengeType : 3;
    u8 didSave : 1;
    u8 cursorRandomized : 1;
    u8 currentBattle;
    u8 rouletteSpeed;
    u8 partySlots[3];
    u16 heldItems[4];
    u16 trainerIDs[14];
    u16 monSetIDs[14];
    u16 unused;
} BattleArcadeSave;

void sub_02030E18(BattleCastlePersistentSave *save, u8 field, u8 challengeType, u8 unused, u8 *value);
u32 sub_02030E58(BattleCastlePersistentSave *save, u8 field, u8 challengeType, u8 unused, void *unused2);
void sub_02030E7C(BattleArcadeSave *arcadeSave);

void sub_02030E18(BattleCastlePersistentSave *save, u8 field, u8 challengeType, u8 unused, u8 *value) {
    u8 *value_dupe = (u8 *)value;
    switch (field) {
    case 9:
        if (*value_dupe >= 1) {
            save->streakActiveFlags |= 1 << challengeType;
        } else {
            save->streakActiveFlags &= 0xff ^ (1 << challengeType);
        }
        break;
    case 10:
        save->haveHeardIntro = 1;
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }
}

u32 sub_02030E58(BattleCastlePersistentSave *save, u8 field, u8 challengeType, u8 unused, void *unused2) {
    switch (field) {
    case 9:
        return (save->streakActiveFlags >> challengeType) & 0x1;
    case 10:
        return save->haveHeardIntro;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 0;
}

void sub_02030E7C(BattleArcadeSave *arcadeSave) {
    MI_CpuClear8(arcadeSave, sizeof(BattleArcadeSave));
}
