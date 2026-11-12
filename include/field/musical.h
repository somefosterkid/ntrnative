// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/musical.h, the lines in use so far.
#ifndef BWPORT_FIELD_MUSICAL_H
#define BWPORT_FIELD_MUSICAL_H

// The Pokémon Musical

#include "types.h"
#include "app/ircbattlematch.h"
#include "field/mus_comm_func.h"
#include "gfl/heap.h"
#include "struct_decls.h"

// Black 2's; twinned code confirms the offsets of event and comm in Black
struct MusicalCommWork {
    MusicalEventWork *event;
    // Overlay 211's communication work
    void *comm;
    // Overlay 36's menu
    void *menu;
    IrcBattleMatchParam ircMatch;
    u16 *result;
    u16 value;
};

GameEvent *func_ov010_02155a98(GameSystem *gsys, GameData *gameData, u8 slot, BOOL online, MusicalCommWork *comm);
// The entry order of the Pokémon at a stage position
u8 func_ov010_021568e8(MusicalEventWork *work, u8 pos);
u8 func_ov010_02156908(MusicalEventWork *work);
// The program's title
StrBuf *func_ov010_02156914(MusicalEventWork *work, HeapID heapId);
// The points of the Pokémon at a stage position, at most 255
u8 func_ov010_02156934(MusicalEventWork *work, u8 pos);
u16 func_ov010_02156954(MusicalEventWork *work);
u16 func_ov010_02156978(MusicalEventWork *work);
// The stage position that finished at a rank
u8 func_ov010_0215699c(MusicalEventWork *work, u8 rank);
u8 func_ov010_021569a4(MusicalEventWork *work, u8 pos);
void func_ov010_02156bac(MusicalEventWork *work);
// Whether the connection was lost
BOOL func_ov010_02156bcc(MusicalEventWork *work);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The trainer class shown for the maintainer of the Pokémon at a stage position
u8 func_ov010_021569ec(MusicalEventWork *work, u8 pos);
// Sets a word to the maintainer's name, or to the Pokémon's
void func_ov010_02156a3c(MusicalEventWork *work, u8 pos, WordSet *wordSet, u32 wordIndex);
void func_ov010_02156ad4(MusicalEventWork *work, u8 pos, WordSet *wordSet, u32 wordIndex);

// The position of the player's Pokémon on the stage
u8 func_ov010_021568e0(MusicalEventWork *work);

#endif // BWPORT_FIELD_MUSICAL_H
