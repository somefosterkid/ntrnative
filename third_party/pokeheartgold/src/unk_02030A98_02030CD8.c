#include "global.h"
#include "battle_castle_save.h"

BOOL sub_02030CD8(BattleCastleSave *save);
void sub_02030CE0(BattleCastleSave *save, BOOL didSave);
void sub_02030CF4(BattleCastleSave *save, u8 field, u8 index, u8 index2, void *value);
u32 sub_02030D84(BattleCastleSave *save, u8 field, u8 index, u8 unused, void *unused2);
void sub_02030DFC(BattleCastlePersistentSave *save);

BOOL sub_02030CD8(BattleCastleSave *save) {
    return save->didSave;
}

void sub_02030CE0(BattleCastleSave *save, BOOL didSave) {
    save->didSave = didSave;
}

void sub_02030CF4(BattleCastleSave *save, u8 field, u8 index, u8 index2, void *value) {
    switch (field) {
    case 0:
        save->challengeType = *(u8 *)value;
        break;
    case 1:
        save->currentBattleNum = *(u8 *)value;
        break;
    case 2:
        save->monHP[index] = *(u16 *)value;
        break;
    case 3:
        save->monPP[index][index2] = *(u8 *)value;
        break;
    case 4:
        save->monStatus[index] = *(u32 *)value;
        break;
    case 5:
        save->heldItems[index] = *(u16 *)value;
        break;
    case 6:
        save->trainerIDs[index] = *(u16 *)value;
        break;
    case 7:
        save->partySlots[index] = *(u8 *)value;
        break;
    case 8:
        save->monSetIDs[index] = *(u16 *)value;
        break;
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }
}

u32 sub_02030D84(BattleCastleSave *save, u8 field, u8 index, u8 unused, void *unused2) {
    switch (field) {
    case 0:
        return save->challengeType;
    case 1:
        return save->currentBattleNum;
    case 2:
        return save->monHP[index];
    case 3:
        return save->monPP[index][unused];
    case 4:
        return save->monStatus[index];
    case 5:
        return save->heldItems[index];
    case 6:
        return save->trainerIDs[index];
    case 7:
        return save->partySlots[index];
    case 8:
        return save->monSetIDs[index];
    default:
        ((0) ? (void)0 : GF_AssertFail());
        break;
    }

    return 0;
}

void sub_02030DFC(BattleCastlePersistentSave *save) {
    MI_CpuClear8(save, sizeof(BattleCastlePersistentSave));
}
