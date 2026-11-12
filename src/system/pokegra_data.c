#include "types.h"

// The trainer sprite file of each trainer class. The link places it after pokegra.c's other .rodata, which no order
// of declarations in one file gives (it sorts before the 150-byte Spinda spot table), so it is an object of its own.
// The split is inferred from that order alone: no name in the ROM or in pokebw2 confirms this file. If a rule of
// MWCC's data order explains the layout in one file, this table goes back to pokegra.c.
const u8 data_020a023c[106] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 40, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
    57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
    85, 86, 87, 88, 89, 90, 71, 4,  70, 74, 75, 69, 42, 41, 91, 40, 92, 93, 94, 0,
};

