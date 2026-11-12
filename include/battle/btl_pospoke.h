// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btl_pospoke.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTL_POSPOKE_H
#define BWPORT_BATTLE_BTL_POSPOKE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define BTL_POSPOKE_NUM_POS 6

// Black 2's, not checked against Black
// A battle position: whether it exists in this battle style, the client it belongs to, and the mon on it
struct BtlPosPokeEntry {
    u8 exists;
    u8 clientId;
    u8 monId; // BTL_POSPOKE_NO_MON when empty
};

// Black 2's, not checked against Black
struct BtlPosPoke {
    BtlPosPokeEntry entries[BTL_POSPOKE_NUM_POS];
    u8 posOfMon[BTL_POSPOKE_NUM_POS]; // the position of a mon ID, BTL_POS_NONE if it has none
};

// Black 2's, not checked against Black
#define BTL_POS_NONE 6
#define BTL_POSPOKE_NO_MON 0x1f

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlPosPoke_Init
// Sets up the positions of a battle style and the mons that start on them (battleStyle is not used)
void data_ov095_0689d1fd(BtlPosPoke *posPoke, BtlMainModule *mainModule, BtlPokeCon *pokeCon, u32 battleStyle);
// pokebw2's BtlPosPoke_GetEmptyPositions
// Collects the empty positions of a client into positions and returns how many there are
u8 data_ov095_0689d559(BtlPosPoke *posPoke, u8 clientId, u8 *positions);
// pokebw2's BtlPosPoke_Rotate
// Rotates the mons of a client in a rotation battle: the party slot that comes in swaps with the active one
void data_ov095_0689d339(BtlPosPoke *posPoke, u8 slot, u8 clientId, BattleMon *mon, BtlPokeCon *pokeCon);
BOOL DoesBattleMonExist(BtlPosPoke *posPoke, u8 monId);
// The position of a mon, or BTL_POS_NONE
u8 GetBattlePos(BtlPosPoke *posPoke, u8 monId);
// pokebw2's BtlPosPoke_Swap
// Swaps the mons of two positions
void data_ov095_0689d4f1(BtlPosPoke *posPoke, u8 firstPos, u8 secondPos);
// pokebw2's BtlPosPoke_SetMon
// Puts a mon on a position, and has it and the opponents on the field record each other (for EXP)
void data_ov095_0689d321(BtlPosPoke *posPoke, u8 pos, u8 monId, BtlPokeCon *pokeCon);
// pokebw2's BtlPosPoke_RemoveMon
// Forgets the mon with an ID, leaving its position empty
void data_ov095_0689d2f9(BtlPosPoke *posPoke, u8 monId);
// pokebw2's BtlPosPoke_IsMonAtActivePos
// Whether a mon has a position that the battle's main module accepts (name guessed)
BOOL data_ov095_0689d59d(BtlPosPoke *posPoke, BtlMainModule *mainModule, u8 monId);
// The mon on a position, or BTL_POSPOKE_NO_MON
u8 GetExistPokeID(BtlPosPoke *posPoke, u8 pos);
// pokebw2's BtlPosPoke_GetPosOfMon
// The position that a mon ID is recorded to be at, even if it has left the position since
u8 data_ov095_0689d5f5(BtlPosPoke *posPoke, u8 monId);

#endif // BWPORT_BATTLE_BTL_POSPOKE_H
