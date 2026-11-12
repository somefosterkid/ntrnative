// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/memory_link_savedata.h: the lines in
// use so far.
#ifndef BWPORT_SAVE_MEMORY_LINK_SAVEDATA_H
#define BWPORT_SAVE_MEMORY_LINK_SAVEDATA_H

#include "types.h"
#include "struct_decls.h"

// Black 2's, not checked against Black
#define MEMORY_LINK_PLAYER_COUNT 3
#define MEMORY_LINK_FLAG_COUNT 16
#define MEMORY_LINK_VAR_COUNT 3
#define MEMORY_LINK_UNK2C_COUNT 6

// Black 2's, not checked against Black
// The player of a Black or White save
struct MemoryLinkPlayer {
    u32 id;
    u16 name[8];
    u8 gender;
    u8 unk15;
    // Set in the Memory Link's data once the save is linked
    u8 linked;
}; // size: 0x18

// Black 2's, not checked against Black
// What a Black or White save gives to unlock the memories
struct MemoryLinkMemories {
    // Bits from event flags and from a word of block 52
    u8 flags[MEMORY_LINK_FLAG_COUNT / 8];
    // Event work variables
    u16 vars[MEMORY_LINK_VAR_COUNT];
    // The musical props owned
    u8 props[0x20];
    u16 unk28;
    u16 unk2A;
    u8 unk2C[MEMORY_LINK_UNK2C_COUNT][0x3c];
}; // size: 0x194

// Black 2's, not checked against Black
// The Memory Link's data in the save. Each player is the one whose save gave a part of the memories: the flags and
// variables, the musical props, and unk2C
struct MemoryLinkSave {
    MemoryLinkPlayer players[MEMORY_LINK_PLAYER_COUNT];
    MemoryLinkMemories memories;
}; // size: 0x1dc

#endif // BWPORT_SAVE_MEMORY_LINK_SAVEDATA_H
