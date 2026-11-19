#ifndef POKEHEARTGOLD_COMM_QUEUE_MAN_H
#define POKEHEARTGOLD_COMM_QUEUE_MAN_H

// CommQueueMan: merged by tools/hg/promote.py from 15 m2c local copies (1 variants, base
// src/unk_02033AE0_02033BE4.c) and 2 accesses; fields at every accessed offset, pads elsewhere.

struct UnkStruct_020322D8_t;

typedef struct CommQueueMan {
    struct { struct UnkStruct_020322D8_t *unk_00; struct UnkStruct_020322D8_t *unk_04; } unk_00;
    struct { struct UnkStruct_020322D8_t *unk_00; struct UnkStruct_020322D8_t *unk_04; } unk_08;
    struct UnkStruct_020322D8_t *unk_10;
    void *unk_14;
    void *unk_18;
    int unk_1C;
} CommQueueMan;

// sizeof(CommQueueMan) == 0x20

#endif // POKEHEARTGOLD_COMM_QUEUE_MAN_H
