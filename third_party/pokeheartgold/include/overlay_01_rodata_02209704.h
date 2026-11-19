#ifndef POKEHEARTGOLD_OVERLAY_01_RODATA_02209704_H
#define POKEHEARTGOLD_OVERLAY_01_RODATA_02209704_H

// Rodata at 0x02209704 (asm/overlay_01_022053EC.s): a 3-word vector, then four 4-word tables (ov01_02209710/20/30/40,
// not exported). mwcc addressed the tables from the first label, so m2c wrote (u8 *)&ov01_02209704 + 0xC/0x1C/0x2C/0x3C.
// One struct over the block keeps the reads inside the object on 64-bit.
typedef struct Ov01_02209704 {
    s32 unk_00[3];
    s32 tables[4][4]; // 0x0C: ov01_02209710, 0x1C: ov01_02209720, 0x2C: ov01_02209730, 0x3C: ov01_02209740
} Ov01_02209704;

extern const Ov01_02209704 ov01_02209704;

#endif // POKEHEARTGOLD_OVERLAY_01_RODATA_02209704_H
