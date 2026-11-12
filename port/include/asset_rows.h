// The rows of config/<version>/assets.tsv as the PC build's loader reads them (tools/port_assets.py rows writes the table,
// port/src/asset_load.c fills the game's tables from the player's ROM by it). No ROM bytes: names, DS ranges, layouts.
#ifndef ASSET_ROWS_H
#define ASSET_ROWS_H

typedef struct {
    const char *name;   // the row's name (asset/<name>.inc)
    int module;         // -1 the ARM9 static module (main), N overlay N, -2 another (not loaded)
    unsigned start;     // the DS range [start, end)
    unsigned end;
    const char *layout; // a plain element type (u8 ... s32, fx16, fx32) or the fields of a record (tools/asset_table.py)
} BwAssetRow;

typedef struct {
    const char *code;   // the ROM header's game code: IRBO Black, IRAO White
    const BwAssetRow *rows;
    int count;
} BwAssetGame;

extern const BwAssetGame bwasset_games[];   // ends with a NULL code

#endif
