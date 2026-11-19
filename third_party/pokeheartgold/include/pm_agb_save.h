#ifndef POKEHEARTGOLD_PM_AGB_SAVE_H
#define POKEHEARTGOLD_PM_AGB_SAVE_H

#include "global.h"

typedef struct PmAgbSaveSizes {
    u8 unk_00[0x20];
    u32 trainerInfoSize;
    u32 gameStateSize;
    u8 unk_28[8];
    u32 unk_30;
    u8 unk_34[0x60];
} PmAgbSaveSizes;

typedef struct PmAgbRomHeader {
    u8 unk_00[0x50];
    u32 unk_50;
    u8 unk_54[0x14];
    PmAgbSaveSizes saveSizes;
} PmAgbRomHeader;

// The GBA-save migration record (overlay 74, ov74_0223D33C; the counter is the label ov74_0223D34C).
// Every reader of it uses this one type: never a private copy.
typedef struct PmAgbSave {
    u32 slot;
    u32 sectorsWritten;
    u8 *buffer;
    u32 firstSector;
    u32 counter;
    u32 writeStatus;
    PmAgbRomHeader header;
} PmAgbSave;

extern PmAgbSave ov74_0223D33C;

#endif // POKEHEARTGOLD_PM_AGB_SAVE_H
