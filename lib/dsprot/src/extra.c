// DS Protect 1.27's extra.c (taxicat1/dsprot, https://github.com/taxicat1/dsprot): the library's .bss, whose address
// the obfuscated pointers and sizes of its encrypted functions are offsets from, and the garbage words that
// dsprot_main.c's decoder points at. The ROM doesn't name them.
#include "dsprot/dsprot_internal.h"

// The reference's BSS
u8 DSProt_BSS_ov230[4];

// The reference's Garbage
const u32 data_ov230_02189954[6] = {
    0xe8dffe17, 0xe43df0de, 0x2ae8335c, 0x0ac09826, 0xe7a838dc, 0xe891a6fc,
};
