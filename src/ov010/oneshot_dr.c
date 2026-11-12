// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov012/oneshot_dr.c, which is the same code in
// Black 2/White 2's overlay 12, with Black's keys. Function names from swan
// (https://github.com/ds-pokemon-hacking/swan, GPL-3.0).
#include "types.h"
#include "save/player_info.h"
#include "save/trainer_card.h"

// The gifts given only once, which the trainer card's table keeps as keys made from the player's ID

#define ONESHOT_DR_COUNT 8

// Black uses two of the eight
static const u32 data_ov010_02172508[ONESHOT_DR_COUNT] = {
    0x0132b4e5, 0x0132b536, 0, 0, 0, 0, 0, 0,
};

void setOneShotDRObtained(TrainerCardSave *trainerCard, int flag, PlayerInfo *playerInfo) {
    if (flag < ONESHOT_DR_COUNT) {
        setToTrainerCardDRTable(trainerCard, flag, getIDAsUInt(playerInfo) ^ data_ov010_02172508[flag]);
    }
}

BOOL isOneShotDRObtained(TrainerCardSave *trainerCard, int flag, PlayerInfo *playerInfo) {
    u32 id;
    u32 key;

    if (flag >= ONESHOT_DR_COUNT) {
        return FALSE;
    }
    id = getIDAsUInt(playerInfo);
    key = getOneShotDRKeyFromSave(trainerCard, flag);
    if ((key ^ id) == data_ov010_02172508[flag]) {
        return TRUE;
    }
    return FALSE;
}
