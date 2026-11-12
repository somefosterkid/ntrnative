// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/musical_dist_save.h: the lines in use
// so far.
#ifndef BWPORT_SAVE_MUSICAL_DIST_SAVE_H
#define BWPORT_SAVE_MUSICAL_DIST_SAVE_H

#include "types.h"
#include "struct_decls.h"

// Black's, from func_0200c520 and func_0200c52c (Black 2: 0x17c00 and 0x18000)
#define MUSICAL_DIST_SAVE_SIZE 0x1fc00
#define MUSICAL_DIST_BUFFER_SIZE 0x20000

// Black 2's; twinned code confirms the offsets of size in Black
typedef struct {
    // The size of the archive
    u32 size;
    u8 unk4[0xc];
} MusicalDistHeader;

// Black 2's; matching code confirms the offsets of heapId, seq, header, block, blockHeader, data and gameData in Black
// The work that reads the block, or that writes a downloaded musical to it
typedef struct {
    u16 heapId;
    // Whether the block was read
    BOOL loaded;
    u32 seq;
    MusicalDistHeader header;
    // The block, its header and the archive after it
    void *block;
    MusicalDistHeader *blockHeader;
    void *data;
    GameData *gameData;
} MusicalDistWork; // size: 0x2c

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// The size of the block and its init
u32 func_0200c520(void);
void func_0200c528(void *block);

#endif // BWPORT_SAVE_MUSICAL_DIST_SAVE_H
