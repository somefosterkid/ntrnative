#include "global.h"

u16 ov98_0221EFA4(u16, s32);
s32 ov98_0221EFB4(u8, s32, u16);
void MI_CpuCopy8(const void *src, void *dest, u32 size);

typedef struct Ov99Entry {
    u16 species;
    u16 moves[3];
} Ov99Entry;

typedef struct Ov99Group {
    Ov99Entry entries[5];
    u32 unk28;
} Ov99Group;

typedef struct Ov99Moves {
    u16 moves[3];
} Ov99Moves;

typedef struct Ov99Work {
    u8 filler_00[0xBC];
    u32 unkBC[10];
} Ov99Work;

static const Ov99Moves sDefMoves = { { 152, 155, 158 } };

void ov99_021E92EC(Ov99Work *work, Ov99Group *src, Ov99Group *dst) {
    int i;
    int j;
    int k;
    Ov99Moves moves;
    Ov99Moves defMoves;

    MI_CpuCopy8(src, dst, sizeof(Ov99Group) * 10);
    i = 0;
    defMoves = sDefMoves;
    for (; i < 10; i++) {
        for (j = 0; j < 5; j++) {
            if (ov98_0221EFB4(i, 0, src[i].entries[j].species) == 0) {
                break;
            }
        }
        if (j == 0) {
            work->unkBC[i] = 1;
        }
        for (k = 0; k < 5; k++) {
            if (k == j) {
                moves = defMoves;
                MI_CpuCopy8(&moves, dst[i].entries[k].moves, 6);
                dst[i].entries[k].species = ov98_0221EFA4(i, 0);
            } else if (k > j) {
                dst[i].entries[k] = src[i].entries[k - 1];
            }
        }
    }
}
