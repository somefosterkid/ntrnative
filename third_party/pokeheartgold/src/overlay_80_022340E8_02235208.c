#include "global.h"

#include "save.h"
#include "seal_case.h"
#include "sprite.h"
#include "overlay_80_battle_arcade.h"
#include "pokemon_types_def.h"
#include "overlay_12_field_battle_dto.h"

struct BattleArcadeSave;

struct FieldBattleDTO;



typedef u16 charcode_t;

typedef union MailIconData {
    struct {
        u16 spriteIndex : 12;
        u16 palIndex : 4;
    } asStruct;
    u16 asValue;
} MailIconData;

Pokemon *Party_GetMonByIndex(const Party *party, int slot);

void SetMonData(Pokemon *mon, int param, const void *value);

typedef struct BattleArcadeSave BattleArcadeSave;

u16 ov80_02238498(BattleArcade *battleArcade);

void ov80_02235208(BattleArcade *arcade, Party *party, u8 partySize);

typedef struct ArcadeData_0223DCA0 {
    const u16 *unk_00[3];
    const u16 *itemPools[3];
} ArcadeData_0223DCA0;

extern ArcadeData_0223DCA0 ov80_0223DCA0;

void ov80_02235208(BattleArcade *arcade, Party *party, u8 partySize) {
    u16 currentRound = ov80_02238498(arcade);

    int poolSize;
    const u16 *itemPool;
    if (currentRound < 3) {
        itemPool = ov80_0223DCA0.itemPools[0];
        poolSize = 11;
    } else if (currentRound < 6) {
        itemPool = ov80_0223DCA0.itemPools[1];
        poolSize = 13;
    } else {
        itemPool = ov80_0223DCA0.itemPools[2];
        poolSize = 11;
    }

    u16 newItem = itemPool[arcade->randomIndex % poolSize];

    for (int i = 0; i < partySize; i++) {
        Pokemon *mon = Party_GetMonByIndex(party, i);
        SetMonData(mon, MON_DATA_HELD_ITEM, &newItem);
    }
}
