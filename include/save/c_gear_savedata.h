// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/save/c_gear_savedata.h: the lines in use so
// far.
#ifndef BWPORT_SAVE_C_GEAR_SAVEDATA_H
#define BWPORT_SAVE_C_GEAR_SAVEDATA_H

#include "types.h"
#include "struct_decls.h"

// Black's save block, 0x2c bytes: nine rows of four bytes of the lamps' settings, then what the functions below read
struct CGearSettings {
    // The panel grid: nine columns of four bytes (func_02009110 to func_020091ac)
    /* 0x00 */ u8 unk_00[0x24];
    // The CRC of the downloaded skin, and whether there is one
    /* 0x24 */ u16 skinCrc;
    /* 0x26 */ u8 skinValid;
    // Whether the C-Gear is on, which the start menu reads to know if it was on when the game was saved
    /* 0x27 */ u8 on;
    // The skin, of the built-in ones
    /* 0x28 */ u8 skin;
    // Whether the C-Gear was on when it last showed: the field subscreen's (func_02009200, func_02009208)
    /* 0x29 */ u8 wasOn;
    /* 0x2a */ u8 unk_2a[2];
}; // size: 0x2c (pokebw2's names; Black 2's layout differs)

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
void func_020090d8(CGearSettings *settings, u8 value);
void func_020090e8(CGearSettings *settings, u8 skin);
u8 func_020090f0(CGearSettings *settings);
void func_020090f8(CGearSettings *settings, u8 value);
void func_02009200(CGearSettings *settings, u32 value);
u8 func_02009208(CGearSettings *settings);
u8 func_02009100(CGearSettings *settings);
void func_02009108(CGearSettings *settings, u16 crc);
u16 func_0200910c(CGearSettings *settings);
void *func_020091fc(void *param_1);
// The panel grid (unk_00, nine columns of four bytes): a cell's panel (the low nibble), whether it is set (0x80), whether
// its flag for the panel is (0x40/0x20/0x10 for 1/3/2), and the setter
u32 func_02009110(u8 *grid, s32 col, s32 row);
u32 func_02009134(u8 *grid, s32 col, s32 row);
u32 func_02009160(u8 *grid, s32 col, s32 row, s32 value);
void func_020091ac(u8 *grid, s32 col, s32 row, s32 value, s32 set, s32 flag);

#endif // BWPORT_SAVE_C_GEAR_SAVEDATA_H
