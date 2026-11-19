#ifndef POKEHEARTGOLD_OVERLAY_43_RODATA_0222ED5C_H
#define POKEHEARTGOLD_OVERLAY_43_RODATA_0222ED5C_H

// Rodata at 0x0222ED5C (asm/overlay_43.s): small byte/halfword tables mwcc pooled and addressed from the first label
// (_0222ED5C, ov43_0222ED74/7A/94 are exported, ov43_0222ED5F..6F / ED82 are not). m2c read _0222ED5C[0x17..0x1A], which
// is ov43_0222ED82 + 8: four u16 screen-data member ids. One struct over the block keeps the read inside the object.
typedef struct Ov43Rodata_0222ED5C {
    u8 unk_00[0x2E];
    u16 unk_2E[4]; // 0x2E (ov43_0222ED82 + 8): 0x0D, 0x0C, 0x0A, 0x09
    u8 unk_36[2];
} Ov43Rodata_0222ED5C;

extern const Ov43Rodata_0222ED5C _0222ED5C;

#endif // POKEHEARTGOLD_OVERLAY_43_RODATA_0222ED5C_H
