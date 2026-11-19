#ifndef POKEHEARTGOLD_COMM_RING_H
#define POKEHEARTGOLD_COMM_RING_H

// CommRing: the byte ring of the comm layer (unk_02033AE0_*), merged from 17 identical local copies (hk253). 0xC on the DS.

typedef struct {
    u8 *buffer; // 0x0
    s16 startIndex; // 0x4
    volatile s16 endIndex; // 0x6
    volatile s16 backupEndIndex; // 0x8
    s16 size; // 0xA
} CommRing;

#endif // POKEHEARTGOLD_COMM_RING_H
