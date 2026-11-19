#ifndef POKEHEARTGOLD_WORK_H
#define POKEHEARTGOLD_WORK_H

// Work: merged by tools/hg/promote.py from 1 m2c local copies (1 variants, base
// src/unk_02078834_02078834.c) and 8 accesses; fields at every accessed offset, pads elsewhere.


typedef struct Work {
    void *unk0;
    int state;
    u16 *unk8;
    u8 unkC;
    u8 unkD;
    u8 pad_E[0x2];
    void *unk10;
    int unk14;
} Work;

// sizeof(Work) == 0x18

#endif // POKEHEARTGOLD_WORK_H
