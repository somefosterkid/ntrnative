// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/system/shooter_item.c, which is the same code in
// Black 2/White 2's main module. The names are Black's dsd names; pokebw2's guesses are in the comments.
#include "system/shooter_item.h"
#include "types.h"
#include "constants/items.h"

// The items of the Wonder Launcher, which the game calls the shooter (swan's shooterWork_* in the battle client), and
// their costs in its energy. The file's name is pokebw2's guess from the table: the ROM has no string for it

typedef struct ShooterItem {
    u16 item;
    u16 cost;
} ShooterItem;

static const ShooterItem data_020a03c8[SHOOTER_ITEM_COUNT] = {
    { ITEM_ITEM_URGE, 1 },     { ITEM_POTION, 2 },       { ITEM_ABILITY_URGE, 3 }, { ITEM_X_ATTACK, 3 },
    { ITEM_X_DEFEND, 3 },      { ITEM_X_SPECIAL, 3 },    { ITEM_X_SP_DEF, 3 },     { ITEM_X_SPEED, 3 },
    { ITEM_X_ACCURACY, 3 },    { ITEM_DIRE_HIT, 3 },     { ITEM_GUARD_SPEC, 3 },   { ITEM_SUPER_POTION, 4 },
    { ITEM_ICE_HEAL, 4 },      { ITEM_ANTIDOTE, 4 },     { ITEM_AWAKENING, 4 },    { ITEM_PARLYZ_HEAL, 4 },
    { ITEM_BURN_HEAL, 4 },     { ITEM_ITEM_DROP, 5 },    { ITEM_X_ATTACK_2, 5 },   { ITEM_X_DEFEND_2, 5 },
    { ITEM_X_SPECIAL_2, 5 },   { ITEM_X_SP_DEF_2, 5 },   { ITEM_X_SPEED_2, 5 },    { ITEM_X_ACCURACY_2, 5 },
    { ITEM_DIRE_HIT_2, 5 },    { ITEM_FULL_HEAL, 6 },    { ITEM_X_ATTACK_3, 7 },   { ITEM_X_DEFEND_3, 7 },
    { ITEM_X_SPECIAL_3, 7 },   { ITEM_X_SP_DEF_3, 7 },   { ITEM_X_SPEED_3, 7 },    { ITEM_X_ACCURACY_3, 7 },
    { ITEM_DIRE_HIT_3, 7 },    { ITEM_HYPER_POTION, 8 }, { ITEM_RESET_URGE, 9 },   { ITEM_MAX_POTION, 10 },
    { ITEM_REVIVE, 11 },       { ITEM_ETHER, 12 },       { ITEM_X_ATTACK_6, 12 },  { ITEM_X_DEFEND_6, 12 },
    { ITEM_X_SPECIAL_6, 12 },  { ITEM_X_SP_DEF_6, 12 },  { ITEM_X_SPEED_6, 12 },   { ITEM_X_ACCURACY_6, 12 },
    { ITEM_FULL_RESTORE, 13 }, { ITEM_MAX_REVIVE, 14 },
};

// ShooterItem_IsEnabled
BOOL func_0202f114(const u8 *disabled, u32 index) {
    u8 byte = index / 8;
    u8 bit = index % 8;

    if (disabled[byte] & (1 << bit)) {
        return FALSE;
    }
    return TRUE;
}

// ShooterItem_GetItem
u32 func_0202f138(u32 index) {
    return data_020a03c8[index].item;
}

// ShooterItem_GetCost
u32 func_0202f144(u16 item) {
    int i;

    for (i = 0; i < SHOOTER_ITEM_COUNT; i++) {
        if (item == data_020a03c8[i].item) {
            return data_020a03c8[i].cost;
        }
    }
    return 0;
}

// ShooterItem_GetIndex
u32 func_0202f170(u16 item) {
    u32 i;

    for (i = 0; i < SHOOTER_ITEM_COUNT; i++) {
        if (item == data_020a03c8[i].item) {
            return i;
        }
    }
    return 0;
}
