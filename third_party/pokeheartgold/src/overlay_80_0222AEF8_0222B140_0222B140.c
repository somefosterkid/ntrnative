#include "global.h"

#include "overlay_80_0222AEF8.h"
#include "save.h"
#include "sys_task.h"
#include "overlay_80_battle_factory.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct BattleFactorySave;

struct FieldBattleDTO;

struct Party;


typedef struct BattleFactorySave BattleFactorySave;

u16 sub_0203769C(void);

void ov80_0222B140(int netID, int sLotoId, void *param2, void *battleFactory) {
    int dummy;
    BattleFactory *battleFactory_dupe = battleFactory;
    const u16 *param2_dupe = param2;

    dummy = 0;
    battleFactory_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleFactory_dupe->unk_580 = param2_dupe[1];
    battleFactory_dupe->unk_582 = param2_dupe[2];
    battleFactory_dupe->unk_57E = param2_dupe[3];

    dummy += 4;
    dummy += (7 + 1);
}

BOOL sub_02037030(int cmd, const void *data, int size);

BOOL ov80_0222B174(BattleFactory *battleFactory);

BOOL ov80_0222B174(BattleFactory *battleFactory) {
    int i, dummy;
    u16 *data;
    int result, dataSize = ((int)(sizeof(battleFactory->commBuffer) / sizeof(*(battleFactory->commBuffer))));
    data = battleFactory->commBuffer;
    dummy = 0;

    for (i = 0; i < 7 * 2; i++) {
        data[i + dummy] = battleFactory->trainerIDs[i];
    }

    dummy += (7 * 2);

    if (sub_02037030(23, data, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222B1A4(int netID, int unused, void *trainerIDs, void *battleFactory) {
    int i, dummy;
    BattleFactory *battleFactory_dupe = battleFactory;
    const u16 *trainerIDs_dupe = trainerIDs;

    dummy = 0;
    battleFactory_dupe->msgsReceived++;

    if (sub_0203769C() == netID || sub_0203769C() == 0) {
        return;
    }

    for (i = 0; i < 7 * 2; i++) {
        battleFactory_dupe->trainerIDs[i] = trainerIDs_dupe[i + dummy];
    }

    dummy += (7 * 2);
}

BOOL ov80_0222B1DC(BattleFactory *battleFactory);

BOOL ov80_0222B1DC(BattleFactory *battleFactory) {
    int i, offset;
    u16 *data;
    int result, dataSize = ((int)(sizeof(battleFactory->commBuffer) / sizeof(*(battleFactory->commBuffer))));
    data = battleFactory->commBuffer;
    offset = 0;

    for (i = 0; i < 6; i++) {
        data[i] = battleFactory->partnerRentalSetIDs[i];
    }

    offset += 6;

    for (i = 0; i < 6; i++) {
        data[i + offset] = battleFactory->partnerRentalIVs[i];
    }

    offset += 6;

    for (i = 0; i < 6; i++) {
        data[i + offset] = (battleFactory->partnerRentalPersonalities[i] & 0xFFFF);
        data[i + offset + 6] = ((battleFactory->partnerRentalPersonalities[i] >> 16) & 0xFFFF);
    }

    offset += (6 * 2);

    if (sub_02037030(24, data, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}
