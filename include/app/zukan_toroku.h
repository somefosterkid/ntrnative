// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/zukan_toroku.h: the lines in use so far.
#ifndef BWPORT_APP_ZUKAN_TOROKU_H
#define BWPORT_APP_ZUKAN_TOROKU_H

#include "types.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of nickname in Black
typedef struct {
    // 0 for a species caught for the first time, which shows its whole entry; otherwise only the cry is played
    u32 mode;
    PartyPkm *pkm;
    BOOL nationalDex;
    // The message about the box the Pokémon goes to, or NULL when it joins the party
    StrBuf *boxMessage;
    BoxSaveAccessor *boxes;
    u32 box;
    GameData *gameData;
    // TRUE when the player chose to give the Pokémon a nickname
    u32 nickname;
} ZukanTorokuParam;

// The zukan registration's proc functions (overlay 207)
extern const GameProcFunctions data_ov207_0220bab0;

#endif // BWPORT_APP_ZUKAN_TOROKU_H
