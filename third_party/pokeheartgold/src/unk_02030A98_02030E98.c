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

BOOL sub_02030E98(BattleCastleSave *save);
void sub_02030EA0(BattleCastleSave *save, BOOL didSave);
void sub_02030EB4(BattleArcadeSave *save, u8 field, u8 index, u8 unused, const void *value);
u32 sub_02030F34(BattleArcadeSave *save, u8 field, u8 index, u8 unused, void *unused2);
void sub_02030F94(BattleCastlePersistentSave *save);

BOOL sub_02030E98(BattleCastleSave *save) {
    return save->didSave;
}

void sub_02030EA0(BattleCastleSave *save, BOOL didSave) {
    save->didSave = didSave;
}

void sub_02030EB4(BattleArcadeSave *save, u8 field, u8 index, u8 unused, const void *value) {
    switch (field) {
    case 0:
        save->challengeType = *(u8 *)value;
        break;
    case 1:
        save->cursorRandomized = *(u8 *)value;
        break;
    case 2:
        save->currentBattle = *(u8 *)value;
        break;
    case 3:
        save->rouletteSpeed = *(u8 *)value;
        break;
    case 4:
        save->heldItems[index] = *(u16 *)value;
        break;
    case 5:
        save->trainerIDs[index] = *(u16 *)value;
        break;
    case 6:
        save->partySlots[index] = *(u8 *)value;
        break;
    case 7:
        save->monSetIDs[index] = *(u16 *)value;
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }
}

u32 sub_02030F34(BattleArcadeSave *save, u8 field, u8 index, u8 unused, void *unused2) {
    switch (field) {
    case 0:
        return save->challengeType;
    case 1:
        return save->cursorRandomized;
    case 2:
        return save->currentBattle;
    case 3:
        return save->rouletteSpeed;
    case 4:
        return save->heldItems[index];
    case 5:
        return save->trainerIDs[index];
    case 6:
        return save->partySlots[index];
    case 7:
        return save->monSetIDs[index];
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 0;
}

void sub_02030F94(BattleCastlePersistentSave *save) {
    MI_CpuClear8(save, sizeof(BattleCastlePersistentSave));
}
