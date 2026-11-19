#include "global.h"

#include "overlay_80_0222AEF8.h"
#include "save.h"
#include "seal_case.h"
#include "sys_task.h"
#include "overlay_80_battle_factory.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct BattleFactorySave;

struct FieldBattleDTO;

struct Party;


typedef struct BattleFactorySave BattleFactorySave;

BOOL sub_02037030(int cmd, const void *data, int size);

BOOL ov80_0222B2C4(BattleFactory *battleFactory);

BOOL ov80_0222B2C4(BattleFactory *battleFactory) {
    int i, offset;
    u16 *data;
    int result, dataSize = ((int)(sizeof(battleFactory->commBuffer) / sizeof(*(battleFactory->commBuffer))));
    data = battleFactory->commBuffer;
    offset = 0;

    for (i = 0; i < 4; i++) {
        data[i] = battleFactory->opponentMonSetIDs[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        data[i + offset] = battleFactory->opponentMonIVs[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        data[i + offset] = (battleFactory->opponentMonPersonalities[i] & 0xFFFF);
        data[i + offset + 4] = ((battleFactory->opponentMonPersonalities[i] >> 16) & 0xFFFF);
    }

    offset += (4 * 2);

    if (sub_02037030(25, data, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

u16 sub_0203769C(void);

void ov80_0222B334(int netID, int unused, void *param2, void *battleFactory) {
    int i, offset;
    BattleFactory *battleFactory_dupe = battleFactory;
    const u16 *param2_dupe = param2;

    offset = 0;
    battleFactory_dupe->msgsReceived++;

    if (sub_0203769C() == netID || sub_0203769C() == 0) {
        return;
    }

    for (i = 0; i < 4; i++) {
        battleFactory_dupe->opponentMonSetIDs[i] = param2_dupe[i];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleFactory_dupe->opponentMonIVs[i] = param2_dupe[i + offset];
    }

    offset += 4;

    for (i = 0; i < 4; i++) {
        battleFactory_dupe->opponentMonPersonalities[i] = param2_dupe[i + offset];
        battleFactory_dupe->opponentMonPersonalities[i] |= (param2_dupe[i + offset + 4] << 16);
    }

    offset += (4 * 2);
}

BOOL ov80_0222B3B0(BattleFactory *battleFactory, u8 param1);

BOOL ov80_0222B3B0(BattleFactory *battleFactory, u8 param1) {
    u16 *data;
    int result, dataSize = ((int)(sizeof(battleFactory->commBuffer) / sizeof(*(battleFactory->commBuffer))));
    data = battleFactory->commBuffer;

    data[0] = param1;

    if (sub_02037030(26, data, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222B3D4(int netID, int unused, void *param2, void *battleFactory) {
    BattleFactory *battleFactory_dupe = battleFactory;
    const u16 *param2_dupe = param2;

    battleFactory_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleFactory_dupe->unk_57C = *param2_dupe;
}

BOOL ov80_0222B3FC(BattleFactory *battleFactory, u8 param1);

BOOL ov80_0222B3FC(BattleFactory *battleFactory, u8 param1) {
    u16 *data;
    int result, dataSize = ((int)(sizeof(battleFactory->commBuffer) / sizeof(*(battleFactory->commBuffer))));
    data = battleFactory->commBuffer;

    data[0] = param1;

    if (sub_02037030(27, data, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}

void ov80_0222B420(int netID, int unused, void *param2, void *battleFactory) {
    BattleFactory *battleFactory_dupe = battleFactory;
    const u16 *param2_dupe = param2;

    battleFactory_dupe->msgsReceived++;

    if (sub_0203769C() == netID) {
        return;
    }

    battleFactory_dupe->unk_57D = *param2_dupe;
}


typedef u16 charcode_t;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

u32 GetMonData(Pokemon *mon, int param, void *dest);

u8 ov80_02236DD4(u8 challengeType);

BOOL ov80_0222B448(BattleFactory *battleFactory);

BOOL ov80_0222B448(BattleFactory *battleFactory) {
    u8 partySize;
    int i, offset;
    u16 *data;
    int result, dataSize;
    u32 personality[2];
    u8 ivs[2];
    Pokemon *pokemon;

    dataSize = ((int)(sizeof(battleFactory->commBuffer) / sizeof(*(battleFactory->commBuffer))));
    data = battleFactory->commBuffer;
    offset = 0;
    partySize = ov80_02236DD4(battleFactory->challengeType);

    for (i = 0; i < partySize; i++) {
        pokemon = Party_GetMonByIndex(battleFactory->playersParty, i);
        ivs[i] = GetMonData(pokemon, MON_DATA_ATK_IV, 0L);
        personality[i] = GetMonData(pokemon, MON_DATA_PERSONALITY, 0L);
    }

    for (i = 0; i < partySize; i++) {
        data[i] = battleFactory->playerMonSetIDs[i];
    }

    offset += partySize;

    for (i = 0; i < partySize; i++) {
        data[i + offset] = ivs[i];
    }

    offset += partySize;

    for (i = 0; i < partySize; i++) {
        data[i + offset] = (personality[i] & 0xFFFF);
        data[i + offset + partySize] = ((personality[i] >> 16) & 0xFFFF);
    }

    offset += (partySize * 2);

    if (sub_02037030(28, data, dataSize) == 1) {
        result = 1;
    } else {
        result = 0;
    }

    return result;
}
